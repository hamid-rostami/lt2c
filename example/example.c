#include <stdio.h>
#include <stdint.h>
#include "lt2c.h"

int main(void) {
  uint8_t data1[LT2C_SYMBOLS_SZ] = {1, 2, 3, 4, 5, 6, 7, 8};
  uint8_t data2[LT2C_SYMBOLS_SZ];
  uint8_t drops[LT2C_DROPS_SZ];
  

  lt2c_encode(drops, data1);

  /* Simulate a noisy channel by making some of the drops available */
  uint32_t st[LT2C_STATUS_SZ] = {0x030F1F07};
  int ret = lt2c_decode(data2, drops, st);

  /* Dump decoded data */
  if (ret == 0) {
    printf("Decode successful, dump decoded data:\n");
    for (int i = 0; i < sizeof(data2); i++) {
      printf("%d, ", data2[i]);
    }
    printf("\n");
  } else {
    printf("Decode unsuccessful, more drops needed\n");
  }

  return 0;
}
