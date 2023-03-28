/* import numbertypes.c
 * import <stdio.h> */

/* GIF SPECIFICATION: 
 * https://www.w3.org/Graphics/GIF/spec-gif87.txt
 * http://giflib.sourceforge.net/whatsinagif/bits_and_bytes.html */

/* NOTE (Elias): explore different ways of creating a color palette:
 *  kmeans, mean, ai, ... Exploration is key here */

/* write array of RGBA values to .gif file */

uint32 m_gif_write(char* path, uint8 *RGB, uint16 width, uint16 height);


/* TODO (Elias) 
 * 1. edgecase, not enough colors in image 
 * 2. speed median cut up drasitcally */


void
median_cut_quicksort(uint8 *v, int32 l, int32 u, uint32 gr)
{
  int32 i, j, p;
  uint8 t;
  if (l < u) {
    p = l;
    i = l;
    j = u; 
    while (i < j) {
      while (i < u && v[i + gr] <= v[p + gr])
        i += 3;
      while (j >= l && v[j + gr] > v[p + gr])
        j -= 3;
      if (i < j) {
        for (uint8 channel = 0; channel < 3; ++channel) {
          t = v[i + channel];
          v[i + channel] = v[j + channel];
          v[j + channel] = t;
        }
      }
    }
    for (uint8 channel = 0; channel < 3; ++channel) {
      t = v[i + channel];
      v[i + channel] = v[j + channel];
      v[j + channel] = t;
    }
    median_cut_quicksort(v, l, j - 3, gr);
    median_cut_quicksort(v, j + 3, u, gr);
  }
}

void
_m_median_cut(uint8 *RGB, uint32 l, uint32 h, uint32 steps, 
              uint8 *tbl, int *blocks, uint32 pgr)
{
  uint32 m, pi, gr;
  uint8 rgb_diff[6]; 
  uint32 rgb_avg[] = {0, 0, 0}; 
  if (steps)
  {
    rgb_diff[0] = rgb_diff[1] = RGB[l]; 
    rgb_diff[2] = rgb_diff[3] = RGB[l+1]; 
    rgb_diff[4] = rgb_diff[5] = RGB[l+2]; 
    for (int i = l; i <= h; i += 1) {
      pi = i * 3;
      if (RGB[pi] < rgb_diff[0])
        rgb_diff[0] = RGB[pi];
      if (RGB[pi] > rgb_diff[1])
        rgb_diff[1] = RGB[pi];
      if (RGB[pi+1] < rgb_diff[2])
        rgb_diff[2] = RGB[pi+1];
      if (RGB[pi+1] > rgb_diff[3])
        rgb_diff[3] = RGB[pi+1];
      if (RGB[pi+2] < rgb_diff[4])
        rgb_diff[4] = RGB[pi+2];
      if (RGB[pi+2] > rgb_diff[5])
        rgb_diff[5] = RGB[pi+2];
    }
    rgb_diff[0] = rgb_diff[1] - rgb_diff[0];
    rgb_diff[2] = rgb_diff[3] - rgb_diff[2];
    rgb_diff[4] = rgb_diff[5] - rgb_diff[4];
    gr = (rgb_diff[0] > rgb_diff[2])? 0 : 1;
    gr = (rgb_diff[4] > gr)? 2 : gr;

    if (gr != pgr)
      median_cut_quicksort(RGB, (l * 3), (h * 3), gr);

    m = ((l + h) / 2);
    _m_median_cut(RGB, l, m, steps - 1, tbl, blocks, gr);
    _m_median_cut(RGB, m + 1, h, steps - 1, tbl, blocks, gr);
  } else {
    uint32 i;
    for (i = l; i <= h; ++i) {
      pi = i * 3;
      rgb_avg[0] += RGB[pi];
      rgb_avg[1] += RGB[pi + 1]; 
      rgb_avg[2] += RGB[pi + 2];
    }
    ++i;
    tbl[(*blocks * 3)] = rgb_avg[0] / i;
    tbl[(*blocks * 3) + 1] = rgb_avg[1] / i; 
    tbl[(*blocks * 3) + 2] = rgb_avg[2] / i;
    *blocks += 1;
  }
}

void
m_median_cut(uint8 *RGB, uint32 RGB_size, uint8 *tbl, 
             uint32 tbl_size, uint8 bpp)
{
  int blocks = 0;
  _m_median_cut(RGB, 0, RGB_size - 1, bpp, tbl, &blocks, -1);
}

uint32 
m_gif_write(char* path, uint8 *RGB, uint16 width, uint16 height)
{
  uint32 result = 0;
  /* TODO (Elias): What do I do with this??? hmmmm.... >:| */
  for (int i = 0; i < width * height * 3; ++i) {
    if (RGB[i] < 0)
      RGB[i] = 0;
  }
  
  uint8 screen_descriptor[8]; printf("w: %d, h: %d, #pixels: %d\n", width, height, (width * height));
  bool8 glb_col_tbl_flg = true;
  uint8 glb_col_tbl_bpp = 3; /* max 8 */
  bool8 sort_flg = true;
  uint8 col_res = 7;
  uint8 bg_col_i = 0;
  uint8 pxl_aspect_ratio = 0;
  uint32 glb_col_tbl_size = (pow(2, glb_col_tbl_bpp) * 3);
  uint8 glb_col_tbl[glb_col_tbl_size];
  for (int i = 0; i < glb_col_tbl_size; ++i)
    glb_col_tbl[i] = 0;
  uint8 img_descriptor[10];
  FILE *file = fopen(path, "w");
  if (file) {
    fprintf(file, "GIF87a");
    *((uint16 *) &screen_descriptor[0]) = width;
    *((uint16 *) &screen_descriptor[2]) = height;
    screen_descriptor[4] = (
        (glb_col_tbl_flg << 7) | 
        (col_res << 4) | 
        (sort_flg << 3) | 
        (glb_col_tbl_bpp - 1));
    screen_descriptor[5] = bg_col_i;
    screen_descriptor[6] = pxl_aspect_ratio;
    /* TODO (Elias): write logical screen descriptor to file */ 
    m_median_cut(RGB, (width * height), 
                 glb_col_tbl, glb_col_tbl_size, glb_col_tbl_bpp);
    for (int i = 0; i < glb_col_tbl_size; i += 3) {
      printf("\033[48;2;%d;%d;%dm             \033[0m   ",
             glb_col_tbl[i], glb_col_tbl[i+1], glb_col_tbl[i+2]);
      printf("#%02X%02X%02X   %03d,%03d,%03d\n", 
             glb_col_tbl[i], glb_col_tbl[i+1], glb_col_tbl[i+2], 
             glb_col_tbl[i], glb_col_tbl[i+1], glb_col_tbl[i+2]);
    }
    img_descriptor[0] = 0x2C;
    *((uint16 *) &img_descriptor[1]) = 0;
    *((uint16 *) &img_descriptor[3]) = 0;
    *((uint16 *) &img_descriptor[5]) = width;
    *((uint16 *) &img_descriptor[7]) = height;
    img_descriptor[9] = 0;
    fprintf(file, "3B");
    printf("\n");
  } else {
    result = 1;
  }
  fclose(file);
  return(result);
}
