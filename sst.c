typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef float real32;
typedef double real64;

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>

#include "lib/multimedia/colors.c"
#include "lib/multimedia/ppm.c"

int
main(void)
{
  Display* display = XOpenDisplay(0);

  uint32 x1 = 0;
  uint32 y1 = 0;
  uint32 x2 = 1920;
  uint32 y2 = 1080;
  char* filename = "image.ppm";

  uint32 width = x2 - x1;
  uint32 height = y2 - y1;
  Window root = DefaultRootWindow(display);
  XImage *image = XGetImage(display, root, x1, y1, width, height, AllPlanes, ZPixmap);

  char *values = BGRA_to_RGB(image->data, width * height * 4);
  ppm_write(filename, values, width, height);
  free(values);

  XDestroyImage(image);
  XCloseDisplay(display);
  return(0);
}
