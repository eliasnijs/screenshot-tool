/* import numbertypes.c */
/* import <stdio.h> */

/* read array of RGB (not RGBA) values to (char *) array ( ! only bitdepth of 255 is supported ) */
uint32 m_ppm_write(char* path, uint8* rgb, uint32 width, uint32 height);
/* write array of RGB (not RGBA) values to .ppm file */
uint32 m_ppm_write(char* path, uint8* rgb, uint32 width, uint32 height);

typedef struct PPMResult {
  uint32 width;
  uint32 height;
  uint32 bitdepth;
  uint8* rgb;
} PPMResult;

uint32
m_ppm_write(char* path, uint8* rgb, uint32 width, uint32 height)
{
  uint32 result = 0;
  FILE *file = fopen(path, "w");
  if (file) {
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    fwrite(rgb, 1, width * height * 3, file);
  } else {
    result = 1;
  }
  fclose(file);
  return(result);
}
