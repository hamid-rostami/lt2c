#ifndef LT2C_H_
#define LT2C_H_

#include <stdint.h>

#define POPCNT(x) __builtin_popcount(x)
#define CTZ(x) __builtin_ctz(x)

#define LT2C_BLKSZ 4
#define LT2C_NSYM 10
#define LT2C_NDROP 30

#define LT2C_SYMBOLS_SZ (LT2C_BLKSZ * LT2C_NSYM)
#define LT2C_DROPS_SZ (LT2C_BLKSZ * LT2C_NDROP)
#define LT2C_STATUS_SZ 1

int lt2c_decode(uint8_t* out, uint8_t* drops_data, uint32_t* status);
void lt2c_encode(uint8_t* out, uint8_t* data);

#endif
