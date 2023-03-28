/* import "lib/numbertypes.c" */
/* import <stdlib.h>          */

/* convert BGRA values to RGB */
uint8* m_BGRA_to_RGB(uint8 *BGRA, uint64 size);

uint8* 
m_BGRA_to_RGB(uint8 *BGRA, uint64 size)
{
  uint8 *RGB = (uint8 *) malloc((size / 4) * 3);
  uint32 i_BGRA = 0;
  uint32 i_RGB = 0;
  while (i_BGRA < size) {
      RGB[i_RGB    ] = BGRA[i_BGRA + 2];
      RGB[i_RGB + 1] = BGRA[i_BGRA + 1];
      RGB[i_RGB + 2] = BGRA[i_BGRA    ];
      i_BGRA += 4;
      i_RGB += 3;
  }
  return(RGB);
}
