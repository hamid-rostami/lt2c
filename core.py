from random import choices
import numpy as np
import sys
from itertools import combinations
import math
import random
import math
from cstr import *
import copy


def bitnum_to_cint(bitnum: int):
    if bitnum > 64:
        raise ValueError("bitnum greather than 64")

    k = math.ceil(bitnum / 8) - 1
    tbl = [
        ("uint8_t", 2),  # 1-8
        ("uint16_t", 4),  # 9-16
        ("uint32_t", 8),  # 17-24
        ("uint32_t", 8),  # 25-32
        ("uint64_t", 16),  # 33-40
        ("uint64_t", 16),  # 41-48
        ("uint64_t", 16),  # 49-56
        ("uint64_t", 16),  # 57-64
    ]
    return tbl[k]


def ideal_distribution(N):
    probabilities = [0, 1 / N]
    probabilities += [1 / (k * (k - 1)) for k in range(2, N + 1)]
    probabilities_sum = sum(probabilities)
    return probabilities


def robust_distribution(N):
    # The choice of M is not a part of the distribution ; it may be improved
    # We take the median and add +1 to avoid possible division by zero
    M = N // 2 + 1
    R = N / M
    ROBUST_FAILURE_PROBABILITY = 0.01

    extra_proba = [0] + [1 / (i * M) for i in range(1, M)]
    extra_proba += [math.log(R / ROBUST_FAILURE_PROBABILITY) / M]  # Spike at M
    extra_proba += [0 for k in range(M + 1, N + 1)]

    probabilities = np.add(extra_proba, ideal_distribution(N))
    probabilities /= np.sum(probabilities)
    probabilities_sum = np.sum(probabilities)
    return probabilities


class Graph:
    def __init__(self, nsymbols, ndrops):
        if ndrops <= nsymbols:
            raise ValueError(f"Number of drops must be greater that symbols")
        self.nsym = nsymbols
        self.ctype, self.ctype_w = bitnum_to_cint(self.nsym)
        probabilities = robust_distribution(nsymbols)
        population = list(range(0, nsymbols + 1))
        degrees = [1] + choices(population, probabilities, k=ndrops - 1)
        self.graph = []
        for i, d in enumerate(degrees, start=0):
            indexes = random.sample(range(nsymbols), d)
            # indexes.sort()
            self.graph.append(indexes)

    def cstr(self):
        """Generate C code to fit this graph data into
        a structure. The structure has index values
        and offset to point to the start position of each
        vector.
        """
        cstr = f"static {self.ctype} H[LT2C_NDROP] = {{\n"
        for row in self.graph:
            bitfields = 0
            for col in row:
                bitfields |= 1 << col
            cstr += f"  0x{bitfields:0{self.ctype_w}X},\n"
        cstr += "};\n"
        return cstr

    def __repr__(self):
        return f"{self.graph}"


class Drop:
    def __init__(self, idx: int, data: list):
        if idx > 255:
            raise ValueError(f"Drop index can't be greater than 255")
        self.idx = idx
        self.data = data

    def to_array(self):
        return [self.idx] + self.data

    @classmethod
    def from_array(cls, array: list):
        return cls(array[0], array[1:])

    def __eq__(self, other):
        if type(other) == int:
            return self.idx == other
        elif type(other) == Drop:
            # TODO: check data too?
            return self.idx == other.idx

    def __repr__(self):
        return f"Drop#{self.idx} {self.data}"


class LT2C:
    def __init__(self, nsymbols, ndrops, blocksz):
        self.blocksz = blocksz
        self.H = Graph(nsymbols, ndrops)

    def encode(self, symbols=None):
        """Run the encoder on the graph and generate output
        encoded drops as well as C srouce code
        """
        drops = []
        cstr = "void lt2c_encode(uint8_t* out, uint8_t* data) {\n"
        cstr += "  LT2C_Block* symbols = (LT2C_Block*) data;\n"
        cstr += "  LT2C_Block* drops = (LT2C_Block*) out;\n"
        for irow, row in enumerate(self.H.graph):
            cstr += f"  /* ----- Drop {irow} ----- */\n"
            drop = None
            for icol, col in enumerate(row):
                if drop is None:
                    if symbols is not None:
                        drop = Drop(irow, symbols[col])
                    cstr += f"  memcpy(drops[{irow}].data, symbols[{col}].data, {self.blocksz});\n"
                else:
                    if symbols is not None:
                        drop.data = np.bitwise_xor(drop.data, symbols[col])
                    for i in range(self.blocksz):
                        cstr += (
                            f"  drops[{irow}].data[{i}] ^= symbols[{col}].data[{i}];\n"
                        )
            drops.append(drop)
        cstr += "}\n\n"
        return cstr, drops

    def decode(self, drops):
        symbols = {}
        graph = copy.deepcopy(self.H.graph)

        def remove_neighbor(symid, data):
            for idrop, drop in enumerate(drops):
                row = graph[drop.idx]
                if symid not in row:
                    continue
                graph[drop.idx].remove(symid)
                drops[idrop].data = np.bitwise_xor(drops[idrop].data, data).tolist()

            # for irow, row in enumerate(graph):
            #     if symid not in row:
            #         continue
            #     graph[irow].remove(symid)
            #     drops[irow].data = np.bitwise_xor(drops[irow].data, data).tolist()

        def is_solved():
            return sorted(symbols.keys()) == list(range(self.H.nsym))

        ret = 0
        while ret == 0:
            advancing = False
            for drop in drops:
                row = graph[drop.idx]
                if len(row) == 1:
                    advancing = True
                    symbols[row[0]] = drop.data
                    if is_solved():
                        ret = 1
                        break
                    graph[drop.idx] = []
                    remove_neighbor(row[0], drop.data)
            if not advancing:
                ret = 2

        if ret == 2:
            return "", {}

        # Calculate SOLVE_VAL
        SOLVED_VAL = (2**self.H.nsym) - 1
        cstr = DECODE_FUNC.format(H_TYP=self.H.ctype, SOLVED_VAL=SOLVED_VAL)

        # TODO: convert symbols to list
        syms = [symbols[k] for k in sorted(symbols.keys())]
        return cstr, syms
