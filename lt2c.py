from core import *
from cstr import *
import numpy as np
import random
from math import ceil
import argparse
from pathlib import Path


def dropout(drops, p):
    out = []
    for d in drops:
        if random.random() > p:
            out.append(d)
    return out


parser = argparse.ArgumentParser(
    prog="lt2c", description="C Code generator for LT-Codes"
)
parser.add_argument("symbols", type=int, help="Number of input symbols")
parser.add_argument("drops", type=int, help="Number of output drops")
parser.add_argument("block", type=int, help="Block size")
parser.add_argument(
    "-t",
    "--trials",
    type=int,
    default=100,
    help="Number of trials to verify decode success rate",
)
parser.add_argument("-s", "--success", type=float, help="Success ratio", default=0.7)
parser.add_argument(
    "-d", "--dropout", type=float, help="Dropout probability", default=0.3
)
parser.add_argument("-o", "--out", type=str, default=".")
args = parser.parse_args()

# Arguments semantic check
dropout_max = (args.drops - args.symbols) / args.drops
BOUND_CHECK = (
    ("symbols", args.symbols, 1, 64),
    ("drops", args.drops, args.symbols + 1, None),
    ("success", args.success, 0.5, 1.0),
    ("dropout", args.dropout, 0, dropout_max),
)

for b in BOUND_CHECK:
    if b[2] and b[1] < b[2]:
        print(f"{b[0]} must be greather than {b[2]}")
        exit(1)
    elif b[3] and b[1] > b[3]:
        print(f"{b[0]} must be lower than {b[3]}")
        exit(2)

# Generate random data
in_syms = (
    np.random.randint(0, 255, args.symbols * args.block)
    .reshape(args.symbols, -1)
    .tolist()
)

cstr = ""
max_success = 0
for tries in range(1000):
    # Encode and decode random data
    lt2c = LT2C(args.symbols, args.drops, args.block)
    enc_cstr, drops = lt2c.encode(in_syms)
    recover_cnt = 0
    for trial in range(args.trials):
        d = copy.deepcopy(drops)
        drops_corr = dropout(d, p=args.dropout)
        dec_cstr, out_syms = lt2c.decode(drops_corr)
        if out_syms == in_syms:
            recover_cnt += 1
            success = recover_cnt / args.trials
            if success > max_success:
                max_success = success
            if success >= args.success:
                cstr = HEAD_STR
                cstr += lt2c.H.cstr()
                cstr += enc_cstr
                cstr += dec_cstr
                break

if cstr:
    cpath = Path(args.out) / "lt2c.c"
    hpath = Path(args.out) / "lt2c.h"
    with open(cpath, "w") as f:
        f.write(cstr)
    with open(hpath, "w") as f:
        hdr = HEADER_STR.format(
            BLKSZ=args.block,
            NSYM=args.symbols,
            NDROP=args.drops,
            STSZ=ceil(args.drops / 32),
        )
        f.write(hdr)
    print(f"Done, max success rate was {max_success} (dropout {args.dropout})")
else:
    print(
        f"Failed, max success rate was {max_success}, try lowering dropout and/or success rate"
    )
    exit(3)
