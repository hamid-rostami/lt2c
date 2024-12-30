#include <stdint.h>
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

static uint16_t H[LT2C_NDROP] = {
  0x0020,
  0x029B,
  0x0009,
  0x0108,
  0x01E6,
  0x038E,
  0x0023,
  0x0104,
  0x0204,
  0x018F,
  0x02D5,
  0x0200,
  0x0008,
  0x0108,
  0x0050,
  0x02B9,
  0x035F,
  0x0060,
  0x0009,
  0x0180,
  0x0008,
  0x0014,
  0x0011,
  0x02FF,
  0x03FD,
  0x0274,
  0x0350,
  0x0012,
  0x0360,
  0x025E,
};
void lt2c_encode(uint8_t* out, uint8_t* data) {
  LT2C_Block* symbols = (LT2C_Block*) data;
  LT2C_Block* drops = (LT2C_Block*) out;
  /* ----- Drop 0 ----- */
  memcpy(drops[0].data, symbols[5].data, 4);
  /* ----- Drop 1 ----- */
  memcpy(drops[1].data, symbols[0].data, 4);
  drops[1].data[0] ^= symbols[4].data[0];
  drops[1].data[1] ^= symbols[4].data[1];
  drops[1].data[2] ^= symbols[4].data[2];
  drops[1].data[3] ^= symbols[4].data[3];
  drops[1].data[0] ^= symbols[1].data[0];
  drops[1].data[1] ^= symbols[1].data[1];
  drops[1].data[2] ^= symbols[1].data[2];
  drops[1].data[3] ^= symbols[1].data[3];
  drops[1].data[0] ^= symbols[3].data[0];
  drops[1].data[1] ^= symbols[3].data[1];
  drops[1].data[2] ^= symbols[3].data[2];
  drops[1].data[3] ^= symbols[3].data[3];
  drops[1].data[0] ^= symbols[9].data[0];
  drops[1].data[1] ^= symbols[9].data[1];
  drops[1].data[2] ^= symbols[9].data[2];
  drops[1].data[3] ^= symbols[9].data[3];
  drops[1].data[0] ^= symbols[7].data[0];
  drops[1].data[1] ^= symbols[7].data[1];
  drops[1].data[2] ^= symbols[7].data[2];
  drops[1].data[3] ^= symbols[7].data[3];
  /* ----- Drop 2 ----- */
  memcpy(drops[2].data, symbols[3].data, 4);
  drops[2].data[0] ^= symbols[0].data[0];
  drops[2].data[1] ^= symbols[0].data[1];
  drops[2].data[2] ^= symbols[0].data[2];
  drops[2].data[3] ^= symbols[0].data[3];
  /* ----- Drop 3 ----- */
  memcpy(drops[3].data, symbols[3].data, 4);
  drops[3].data[0] ^= symbols[8].data[0];
  drops[3].data[1] ^= symbols[8].data[1];
  drops[3].data[2] ^= symbols[8].data[2];
  drops[3].data[3] ^= symbols[8].data[3];
  /* ----- Drop 4 ----- */
  memcpy(drops[4].data, symbols[1].data, 4);
  drops[4].data[0] ^= symbols[2].data[0];
  drops[4].data[1] ^= symbols[2].data[1];
  drops[4].data[2] ^= symbols[2].data[2];
  drops[4].data[3] ^= symbols[2].data[3];
  drops[4].data[0] ^= symbols[6].data[0];
  drops[4].data[1] ^= symbols[6].data[1];
  drops[4].data[2] ^= symbols[6].data[2];
  drops[4].data[3] ^= symbols[6].data[3];
  drops[4].data[0] ^= symbols[7].data[0];
  drops[4].data[1] ^= symbols[7].data[1];
  drops[4].data[2] ^= symbols[7].data[2];
  drops[4].data[3] ^= symbols[7].data[3];
  drops[4].data[0] ^= symbols[5].data[0];
  drops[4].data[1] ^= symbols[5].data[1];
  drops[4].data[2] ^= symbols[5].data[2];
  drops[4].data[3] ^= symbols[5].data[3];
  drops[4].data[0] ^= symbols[8].data[0];
  drops[4].data[1] ^= symbols[8].data[1];
  drops[4].data[2] ^= symbols[8].data[2];
  drops[4].data[3] ^= symbols[8].data[3];
  /* ----- Drop 5 ----- */
  memcpy(drops[5].data, symbols[9].data, 4);
  drops[5].data[0] ^= symbols[3].data[0];
  drops[5].data[1] ^= symbols[3].data[1];
  drops[5].data[2] ^= symbols[3].data[2];
  drops[5].data[3] ^= symbols[3].data[3];
  drops[5].data[0] ^= symbols[8].data[0];
  drops[5].data[1] ^= symbols[8].data[1];
  drops[5].data[2] ^= symbols[8].data[2];
  drops[5].data[3] ^= symbols[8].data[3];
  drops[5].data[0] ^= symbols[1].data[0];
  drops[5].data[1] ^= symbols[1].data[1];
  drops[5].data[2] ^= symbols[1].data[2];
  drops[5].data[3] ^= symbols[1].data[3];
  drops[5].data[0] ^= symbols[2].data[0];
  drops[5].data[1] ^= symbols[2].data[1];
  drops[5].data[2] ^= symbols[2].data[2];
  drops[5].data[3] ^= symbols[2].data[3];
  drops[5].data[0] ^= symbols[7].data[0];
  drops[5].data[1] ^= symbols[7].data[1];
  drops[5].data[2] ^= symbols[7].data[2];
  drops[5].data[3] ^= symbols[7].data[3];
  /* ----- Drop 6 ----- */
  memcpy(drops[6].data, symbols[5].data, 4);
  drops[6].data[0] ^= symbols[0].data[0];
  drops[6].data[1] ^= symbols[0].data[1];
  drops[6].data[2] ^= symbols[0].data[2];
  drops[6].data[3] ^= symbols[0].data[3];
  drops[6].data[0] ^= symbols[1].data[0];
  drops[6].data[1] ^= symbols[1].data[1];
  drops[6].data[2] ^= symbols[1].data[2];
  drops[6].data[3] ^= symbols[1].data[3];
  /* ----- Drop 7 ----- */
  memcpy(drops[7].data, symbols[8].data, 4);
  drops[7].data[0] ^= symbols[2].data[0];
  drops[7].data[1] ^= symbols[2].data[1];
  drops[7].data[2] ^= symbols[2].data[2];
  drops[7].data[3] ^= symbols[2].data[3];
  /* ----- Drop 8 ----- */
  memcpy(drops[8].data, symbols[2].data, 4);
  drops[8].data[0] ^= symbols[9].data[0];
  drops[8].data[1] ^= symbols[9].data[1];
  drops[8].data[2] ^= symbols[9].data[2];
  drops[8].data[3] ^= symbols[9].data[3];
  /* ----- Drop 9 ----- */
  memcpy(drops[9].data, symbols[8].data, 4);
  drops[9].data[0] ^= symbols[1].data[0];
  drops[9].data[1] ^= symbols[1].data[1];
  drops[9].data[2] ^= symbols[1].data[2];
  drops[9].data[3] ^= symbols[1].data[3];
  drops[9].data[0] ^= symbols[2].data[0];
  drops[9].data[1] ^= symbols[2].data[1];
  drops[9].data[2] ^= symbols[2].data[2];
  drops[9].data[3] ^= symbols[2].data[3];
  drops[9].data[0] ^= symbols[3].data[0];
  drops[9].data[1] ^= symbols[3].data[1];
  drops[9].data[2] ^= symbols[3].data[2];
  drops[9].data[3] ^= symbols[3].data[3];
  drops[9].data[0] ^= symbols[0].data[0];
  drops[9].data[1] ^= symbols[0].data[1];
  drops[9].data[2] ^= symbols[0].data[2];
  drops[9].data[3] ^= symbols[0].data[3];
  drops[9].data[0] ^= symbols[7].data[0];
  drops[9].data[1] ^= symbols[7].data[1];
  drops[9].data[2] ^= symbols[7].data[2];
  drops[9].data[3] ^= symbols[7].data[3];
  /* ----- Drop 10 ----- */
  memcpy(drops[10].data, symbols[7].data, 4);
  drops[10].data[0] ^= symbols[9].data[0];
  drops[10].data[1] ^= symbols[9].data[1];
  drops[10].data[2] ^= symbols[9].data[2];
  drops[10].data[3] ^= symbols[9].data[3];
  drops[10].data[0] ^= symbols[4].data[0];
  drops[10].data[1] ^= symbols[4].data[1];
  drops[10].data[2] ^= symbols[4].data[2];
  drops[10].data[3] ^= symbols[4].data[3];
  drops[10].data[0] ^= symbols[0].data[0];
  drops[10].data[1] ^= symbols[0].data[1];
  drops[10].data[2] ^= symbols[0].data[2];
  drops[10].data[3] ^= symbols[0].data[3];
  drops[10].data[0] ^= symbols[6].data[0];
  drops[10].data[1] ^= symbols[6].data[1];
  drops[10].data[2] ^= symbols[6].data[2];
  drops[10].data[3] ^= symbols[6].data[3];
  drops[10].data[0] ^= symbols[2].data[0];
  drops[10].data[1] ^= symbols[2].data[1];
  drops[10].data[2] ^= symbols[2].data[2];
  drops[10].data[3] ^= symbols[2].data[3];
  /* ----- Drop 11 ----- */
  memcpy(drops[11].data, symbols[9].data, 4);
  /* ----- Drop 12 ----- */
  memcpy(drops[12].data, symbols[3].data, 4);
  /* ----- Drop 13 ----- */
  memcpy(drops[13].data, symbols[8].data, 4);
  drops[13].data[0] ^= symbols[3].data[0];
  drops[13].data[1] ^= symbols[3].data[1];
  drops[13].data[2] ^= symbols[3].data[2];
  drops[13].data[3] ^= symbols[3].data[3];
  /* ----- Drop 14 ----- */
  memcpy(drops[14].data, symbols[6].data, 4);
  drops[14].data[0] ^= symbols[4].data[0];
  drops[14].data[1] ^= symbols[4].data[1];
  drops[14].data[2] ^= symbols[4].data[2];
  drops[14].data[3] ^= symbols[4].data[3];
  /* ----- Drop 15 ----- */
  memcpy(drops[15].data, symbols[4].data, 4);
  drops[15].data[0] ^= symbols[5].data[0];
  drops[15].data[1] ^= symbols[5].data[1];
  drops[15].data[2] ^= symbols[5].data[2];
  drops[15].data[3] ^= symbols[5].data[3];
  drops[15].data[0] ^= symbols[9].data[0];
  drops[15].data[1] ^= symbols[9].data[1];
  drops[15].data[2] ^= symbols[9].data[2];
  drops[15].data[3] ^= symbols[9].data[3];
  drops[15].data[0] ^= symbols[7].data[0];
  drops[15].data[1] ^= symbols[7].data[1];
  drops[15].data[2] ^= symbols[7].data[2];
  drops[15].data[3] ^= symbols[7].data[3];
  drops[15].data[0] ^= symbols[0].data[0];
  drops[15].data[1] ^= symbols[0].data[1];
  drops[15].data[2] ^= symbols[0].data[2];
  drops[15].data[3] ^= symbols[0].data[3];
  drops[15].data[0] ^= symbols[3].data[0];
  drops[15].data[1] ^= symbols[3].data[1];
  drops[15].data[2] ^= symbols[3].data[2];
  drops[15].data[3] ^= symbols[3].data[3];
  /* ----- Drop 16 ----- */
  memcpy(drops[16].data, symbols[3].data, 4);
  drops[16].data[0] ^= symbols[1].data[0];
  drops[16].data[1] ^= symbols[1].data[1];
  drops[16].data[2] ^= symbols[1].data[2];
  drops[16].data[3] ^= symbols[1].data[3];
  drops[16].data[0] ^= symbols[0].data[0];
  drops[16].data[1] ^= symbols[0].data[1];
  drops[16].data[2] ^= symbols[0].data[2];
  drops[16].data[3] ^= symbols[0].data[3];
  drops[16].data[0] ^= symbols[4].data[0];
  drops[16].data[1] ^= symbols[4].data[1];
  drops[16].data[2] ^= symbols[4].data[2];
  drops[16].data[3] ^= symbols[4].data[3];
  drops[16].data[0] ^= symbols[2].data[0];
  drops[16].data[1] ^= symbols[2].data[1];
  drops[16].data[2] ^= symbols[2].data[2];
  drops[16].data[3] ^= symbols[2].data[3];
  drops[16].data[0] ^= symbols[9].data[0];
  drops[16].data[1] ^= symbols[9].data[1];
  drops[16].data[2] ^= symbols[9].data[2];
  drops[16].data[3] ^= symbols[9].data[3];
  drops[16].data[0] ^= symbols[6].data[0];
  drops[16].data[1] ^= symbols[6].data[1];
  drops[16].data[2] ^= symbols[6].data[2];
  drops[16].data[3] ^= symbols[6].data[3];
  drops[16].data[0] ^= symbols[8].data[0];
  drops[16].data[1] ^= symbols[8].data[1];
  drops[16].data[2] ^= symbols[8].data[2];
  drops[16].data[3] ^= symbols[8].data[3];
  /* ----- Drop 17 ----- */
  memcpy(drops[17].data, symbols[5].data, 4);
  drops[17].data[0] ^= symbols[6].data[0];
  drops[17].data[1] ^= symbols[6].data[1];
  drops[17].data[2] ^= symbols[6].data[2];
  drops[17].data[3] ^= symbols[6].data[3];
  /* ----- Drop 18 ----- */
  memcpy(drops[18].data, symbols[3].data, 4);
  drops[18].data[0] ^= symbols[0].data[0];
  drops[18].data[1] ^= symbols[0].data[1];
  drops[18].data[2] ^= symbols[0].data[2];
  drops[18].data[3] ^= symbols[0].data[3];
  /* ----- Drop 19 ----- */
  memcpy(drops[19].data, symbols[7].data, 4);
  drops[19].data[0] ^= symbols[8].data[0];
  drops[19].data[1] ^= symbols[8].data[1];
  drops[19].data[2] ^= symbols[8].data[2];
  drops[19].data[3] ^= symbols[8].data[3];
  /* ----- Drop 20 ----- */
  memcpy(drops[20].data, symbols[3].data, 4);
  /* ----- Drop 21 ----- */
  memcpy(drops[21].data, symbols[4].data, 4);
  drops[21].data[0] ^= symbols[2].data[0];
  drops[21].data[1] ^= symbols[2].data[1];
  drops[21].data[2] ^= symbols[2].data[2];
  drops[21].data[3] ^= symbols[2].data[3];
  /* ----- Drop 22 ----- */
  memcpy(drops[22].data, symbols[0].data, 4);
  drops[22].data[0] ^= symbols[4].data[0];
  drops[22].data[1] ^= symbols[4].data[1];
  drops[22].data[2] ^= symbols[4].data[2];
  drops[22].data[3] ^= symbols[4].data[3];
  /* ----- Drop 23 ----- */
  memcpy(drops[23].data, symbols[2].data, 4);
  drops[23].data[0] ^= symbols[7].data[0];
  drops[23].data[1] ^= symbols[7].data[1];
  drops[23].data[2] ^= symbols[7].data[2];
  drops[23].data[3] ^= symbols[7].data[3];
  drops[23].data[0] ^= symbols[6].data[0];
  drops[23].data[1] ^= symbols[6].data[1];
  drops[23].data[2] ^= symbols[6].data[2];
  drops[23].data[3] ^= symbols[6].data[3];
  drops[23].data[0] ^= symbols[0].data[0];
  drops[23].data[1] ^= symbols[0].data[1];
  drops[23].data[2] ^= symbols[0].data[2];
  drops[23].data[3] ^= symbols[0].data[3];
  drops[23].data[0] ^= symbols[4].data[0];
  drops[23].data[1] ^= symbols[4].data[1];
  drops[23].data[2] ^= symbols[4].data[2];
  drops[23].data[3] ^= symbols[4].data[3];
  drops[23].data[0] ^= symbols[1].data[0];
  drops[23].data[1] ^= symbols[1].data[1];
  drops[23].data[2] ^= symbols[1].data[2];
  drops[23].data[3] ^= symbols[1].data[3];
  drops[23].data[0] ^= symbols[3].data[0];
  drops[23].data[1] ^= symbols[3].data[1];
  drops[23].data[2] ^= symbols[3].data[2];
  drops[23].data[3] ^= symbols[3].data[3];
  drops[23].data[0] ^= symbols[9].data[0];
  drops[23].data[1] ^= symbols[9].data[1];
  drops[23].data[2] ^= symbols[9].data[2];
  drops[23].data[3] ^= symbols[9].data[3];
  drops[23].data[0] ^= symbols[5].data[0];
  drops[23].data[1] ^= symbols[5].data[1];
  drops[23].data[2] ^= symbols[5].data[2];
  drops[23].data[3] ^= symbols[5].data[3];
  /* ----- Drop 24 ----- */
  memcpy(drops[24].data, symbols[6].data, 4);
  drops[24].data[0] ^= symbols[2].data[0];
  drops[24].data[1] ^= symbols[2].data[1];
  drops[24].data[2] ^= symbols[2].data[2];
  drops[24].data[3] ^= symbols[2].data[3];
  drops[24].data[0] ^= symbols[3].data[0];
  drops[24].data[1] ^= symbols[3].data[1];
  drops[24].data[2] ^= symbols[3].data[2];
  drops[24].data[3] ^= symbols[3].data[3];
  drops[24].data[0] ^= symbols[4].data[0];
  drops[24].data[1] ^= symbols[4].data[1];
  drops[24].data[2] ^= symbols[4].data[2];
  drops[24].data[3] ^= symbols[4].data[3];
  drops[24].data[0] ^= symbols[7].data[0];
  drops[24].data[1] ^= symbols[7].data[1];
  drops[24].data[2] ^= symbols[7].data[2];
  drops[24].data[3] ^= symbols[7].data[3];
  drops[24].data[0] ^= symbols[0].data[0];
  drops[24].data[1] ^= symbols[0].data[1];
  drops[24].data[2] ^= symbols[0].data[2];
  drops[24].data[3] ^= symbols[0].data[3];
  drops[24].data[0] ^= symbols[5].data[0];
  drops[24].data[1] ^= symbols[5].data[1];
  drops[24].data[2] ^= symbols[5].data[2];
  drops[24].data[3] ^= symbols[5].data[3];
  drops[24].data[0] ^= symbols[9].data[0];
  drops[24].data[1] ^= symbols[9].data[1];
  drops[24].data[2] ^= symbols[9].data[2];
  drops[24].data[3] ^= symbols[9].data[3];
  drops[24].data[0] ^= symbols[8].data[0];
  drops[24].data[1] ^= symbols[8].data[1];
  drops[24].data[2] ^= symbols[8].data[2];
  drops[24].data[3] ^= symbols[8].data[3];
  /* ----- Drop 25 ----- */
  memcpy(drops[25].data, symbols[6].data, 4);
  drops[25].data[0] ^= symbols[9].data[0];
  drops[25].data[1] ^= symbols[9].data[1];
  drops[25].data[2] ^= symbols[9].data[2];
  drops[25].data[3] ^= symbols[9].data[3];
  drops[25].data[0] ^= symbols[4].data[0];
  drops[25].data[1] ^= symbols[4].data[1];
  drops[25].data[2] ^= symbols[4].data[2];
  drops[25].data[3] ^= symbols[4].data[3];
  drops[25].data[0] ^= symbols[2].data[0];
  drops[25].data[1] ^= symbols[2].data[1];
  drops[25].data[2] ^= symbols[2].data[2];
  drops[25].data[3] ^= symbols[2].data[3];
  drops[25].data[0] ^= symbols[5].data[0];
  drops[25].data[1] ^= symbols[5].data[1];
  drops[25].data[2] ^= symbols[5].data[2];
  drops[25].data[3] ^= symbols[5].data[3];
  /* ----- Drop 26 ----- */
  memcpy(drops[26].data, symbols[9].data, 4);
  drops[26].data[0] ^= symbols[8].data[0];
  drops[26].data[1] ^= symbols[8].data[1];
  drops[26].data[2] ^= symbols[8].data[2];
  drops[26].data[3] ^= symbols[8].data[3];
  drops[26].data[0] ^= symbols[4].data[0];
  drops[26].data[1] ^= symbols[4].data[1];
  drops[26].data[2] ^= symbols[4].data[2];
  drops[26].data[3] ^= symbols[4].data[3];
  drops[26].data[0] ^= symbols[6].data[0];
  drops[26].data[1] ^= symbols[6].data[1];
  drops[26].data[2] ^= symbols[6].data[2];
  drops[26].data[3] ^= symbols[6].data[3];
  /* ----- Drop 27 ----- */
  memcpy(drops[27].data, symbols[4].data, 4);
  drops[27].data[0] ^= symbols[1].data[0];
  drops[27].data[1] ^= symbols[1].data[1];
  drops[27].data[2] ^= symbols[1].data[2];
  drops[27].data[3] ^= symbols[1].data[3];
  /* ----- Drop 28 ----- */
  memcpy(drops[28].data, symbols[5].data, 4);
  drops[28].data[0] ^= symbols[8].data[0];
  drops[28].data[1] ^= symbols[8].data[1];
  drops[28].data[2] ^= symbols[8].data[2];
  drops[28].data[3] ^= symbols[8].data[3];
  drops[28].data[0] ^= symbols[9].data[0];
  drops[28].data[1] ^= symbols[9].data[1];
  drops[28].data[2] ^= symbols[9].data[2];
  drops[28].data[3] ^= symbols[9].data[3];
  drops[28].data[0] ^= symbols[6].data[0];
  drops[28].data[1] ^= symbols[6].data[1];
  drops[28].data[2] ^= symbols[6].data[2];
  drops[28].data[3] ^= symbols[6].data[3];
  /* ----- Drop 29 ----- */
  memcpy(drops[29].data, symbols[1].data, 4);
  drops[29].data[0] ^= symbols[6].data[0];
  drops[29].data[1] ^= symbols[6].data[1];
  drops[29].data[2] ^= symbols[6].data[2];
  drops[29].data[3] ^= symbols[6].data[3];
  drops[29].data[0] ^= symbols[2].data[0];
  drops[29].data[1] ^= symbols[2].data[1];
  drops[29].data[2] ^= symbols[2].data[2];
  drops[29].data[3] ^= symbols[2].data[3];
  drops[29].data[0] ^= symbols[9].data[0];
  drops[29].data[1] ^= symbols[9].data[1];
  drops[29].data[2] ^= symbols[9].data[2];
  drops[29].data[3] ^= symbols[9].data[3];
  drops[29].data[0] ^= symbols[4].data[0];
  drops[29].data[1] ^= symbols[4].data[1];
  drops[29].data[2] ^= symbols[4].data[2];
  drops[29].data[3] ^= symbols[4].data[3];
  drops[29].data[0] ^= symbols[3].data[0];
  drops[29].data[1] ^= symbols[3].data[1];
  drops[29].data[2] ^= symbols[3].data[2];
  drops[29].data[3] ^= symbols[3].data[3];
}


int lt2c_decode(uint8_t* out, uint8_t* drops_data, uint32_t* status) {
  uint16_t solved = 0;
  uint16_t h[LT2C_NDROP];
  LT2C_Block* symbols = (LT2C_Block*) out;
  LT2C_Block* drops = (LT2C_Block*) drops_data;

  memcpy(h, H, sizeof(H));

  uint8_t stuck = 0;
  while(stuck == 0) {
    stuck = 1;
    for (int idrop = 0; idrop < LT2C_NDROP; idrop++) {
      LT2C_Block* drop = &drops[idrop];
      if (drop_get_status(status, idrop) == 0 ||
          h[idrop] == 0 ||
          POPCNT(h[idrop]) > 1) {
        continue;
      }
      uint8_t symid = CTZ(h[idrop]);
      stuck = 0;

      mempcpy(symbols[symid].data, drop->data, LT2C_BLKSZ);

      solved |= 1UL << symid;
      if (solved == 0x3FF)
        return 0;

      /* Remove neighbors */
      uint16_t msk = 1UL << symid;
      h[idrop] &= ~msk;
      for (int i = 0; i < LT2C_NDROP; i++) {
        LT2C_Block* d = &drops[i];
        if (drop_get_status(status, i) == 0 ||
            (h[i] & msk) == 0) {
          continue;
        }
        h[i] &= ~msk;
        for (int j = 0; j < LT2C_BLKSZ; j++) {
          d->data[j] ^= drop->data[j];
        }
      }
    }
  }

  return -1;
}
