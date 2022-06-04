#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include "lib/numbertypes.c"

int
main()
{
  Display* display = XOpenDisplay(0);

  int x1 = 0;
  int y1 = 0;
  int x2 = 1920;
  int y2 = 1080;
  char* filename = "image.ppm";

  int width = x2 - x1;
  int height = y2 - y1;
  Window root = DefaultRootWindow(display);
  XImage *image = XGetImage(display, root, x1, y1, width, height, AllPlanes, ZPixmap);

  char* values = (char *) malloc(width * height * 3);
  int value_index = 0;
  for (int i = 0; i < (width * height * 4); ++i) {
    if ((i % 4) != 3) {
      int index = i;
      if ((index % 4) == 0)
        index += 2;
      else if ((index % 4) == 2)
        index -= 2;
      values[value_index] = image->data[index];
      ++value_index;
    }
  }

  FILE *file = fopen(filename, "w");
  fprintf(file, "P6\n%d %d\n255\n", width, height);
  fwrite(values, 1, width * height * 3, file);
  fclose(file);

  free(values);
  XDestroyImage(image);
  XCloseDisplay(display);
  return(0);
}
