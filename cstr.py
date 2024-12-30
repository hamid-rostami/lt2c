HEADER_STR = """#ifndef LT2C_H_
#define LT2C_H_

#include <stdint.h>

#define POPCNT(x) __builtin_popcount(x)
#define CTZ(x) __builtin_ctz(x)

#define LT2C_BLKSZ {BLKSZ}
#define LT2C_NSYM {NSYM}
#define LT2C_NDROP {NDROP}

#define LT2C_SYMBOLS_SZ (LT2C_BLKSZ * LT2C_NSYM)
#define LT2C_DROPS_SZ (LT2C_BLKSZ * LT2C_NDROP)
#define LT2C_STATUS_SZ {STSZ}

int lt2c_decode(uint8_t* out, uint8_t* drops_data, uint32_t* status);
void lt2c_encode(uint8_t* out, uint8_t* data);

#endif
"""

HEAD_STR = """#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "lt2c.h"

typedef struct {
  uint8_t data[LT2C_BLKSZ];
} LT2C_Block;

static inline void drop_set_status(uint32_t* status, uint16_t drop_id) {
  uint8_t bytenum = drop_id / 32;
  uint8_t bitnum = drop_id % 32;
  status[bytenum] |= (1<<bitnum);
}

static inline uint8_t drop_get_status(uint32_t* status, uint16_t drop_id) {
  uint8_t bytenum = drop_id / 32;
  uint8_t bitnum = drop_id % 32;
  return (status[bytenum] >> bitnum) & 0x01;
}

"""

DECODE_FUNC = """
int lt2c_decode(uint8_t* out, uint8_t* drops_data, uint32_t* status) {{
  {H_TYP} solved = 0;
  {H_TYP} h[LT2C_NDROP];
  LT2C_Block* symbols = (LT2C_Block*) out;
  LT2C_Block* drops = (LT2C_Block*) drops_data;

  memcpy(h, H, sizeof(H));

  uint8_t stuck = 0;
  while(stuck == 0) {{
    stuck = 1;
    for (int idrop = 0; idrop < LT2C_NDROP; idrop++) {{
      LT2C_Block* drop = &drops[idrop];
      if (drop_get_status(status, idrop) == 0 ||
          h[idrop] == 0 ||
          POPCNT(h[idrop]) > 1) {{
        continue;
      }}
      uint8_t symid = CTZ(h[idrop]);
      stuck = 0;

      mempcpy(symbols[symid].data, drop->data, LT2C_BLKSZ);

      solved |= 1UL << symid;
      if (solved == 0x{SOLVED_VAL:X})
        return 0;

      /* Remove neighbors */
      {H_TYP} msk = 1UL << symid;
      h[idrop] &= ~msk;
      for (int i = 0; i < LT2C_NDROP; i++) {{
        LT2C_Block* d = &drops[i];
        if (drop_get_status(status, i) == 0 ||
            (h[i] & msk) == 0) {{
          continue;
        }}
        h[i] &= ~msk;
        for (int j = 0; j < LT2C_BLKSZ; j++) {{
          d->data[j] ^= drop->data[j];
        }}
      }}
    }}
  }}

  return -1;
}}
"""
