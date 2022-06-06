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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "lib/media/colors.c"
#include "lib/media/ppm.c"

int
main(void)
{
  char* filename = "image.ppm";
  uint32 x = 0;
  uint32 y = 0;
  uint32 rx = 0;
  uint32 ry = 0;
  int32 w = 0;
  int32 h = 0;
  
  Display *display = XOpenDisplay(0);
  Window root = DefaultRootWindow(display);

  Cursor cursor1 = XCreateFontCursor(display, XC_left_ptr);
  Cursor cursor2 = XCreateFontCursor(display, XC_diamond_cross);

  XGrabPointer(display, root, False, ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, 
               GrabModeAsync, GrabModeAsync, root, cursor1, CurrentTime);
  
  XGCValues xgc_values = {0};
  xgc_values.background = XBlackPixel(display, 0);
  xgc_values.foreground = XWhitePixel(display, 0);
  xgc_values.function = GXxor;
  xgc_values.subwindow_mode = IncludeInferiors;
  xgc_values.line_width = 2;
  GC gc = XCreateGC(display, root, 
                    GCFunction | GCForeground | GCBackground | GCSubwindowMode | GCLineWidth, 
                    &xgc_values);
  
  XEvent event;
  uint8 running = 2; /* 2: selection not started, 1: selection started, 0: selection done */
  while (running) {
    while (running && XPending(display)) {
      XNextEvent(display, &event);
      switch (event.type) {
      case (MotionNotify):
        if (running == 1) {
          XDrawRectangle(display, root, gc, rx, ry, w, h);
          w = event.xbutton.x - x;
          h = event.xbutton.y - y;
          rx = (w < 0) ? x + w : x;
          ry = (h < 0)? y + h : y;
          w = ((w >> 31) + w) ^ (w >> 31);
          h = ((h >> 31) + h) ^ (h >> 31);
          XDrawRectangle(display, root, gc, rx, ry, w, h);
          XFlush(display);
        }
        break;
      case (ButtonPress):
        x = event.xbutton.x; 
        y = event.xbutton.y;  
        XChangeActivePointerGrab(display, ButtonMotionMask | ButtonReleaseMask,
                                 cursor2, CurrentTime);
        XDrawRectangle(display, root, gc, x, y, w, h);
        running = 1;
        break;
      case (ButtonRelease):
        running = 0;
        break; 
      }
      XSync(display, 1);
    }
  }
  XFlush(display);
  if (w || h) {
    XDrawRectangle(display, root, gc, rx, ry, w, h);
    XImage *image = XGetImage(display, root, rx, ry, w, h, AllPlanes, ZPixmap);
    char *values = BGRA_to_RGB(image->data, w * h * 4);
    ppm_write(filename, values, w, h);
    free(values);
    XDestroyImage(image);
  }
 
  XFreeGC(display, gc);
  XCloseDisplay(display);
  return(0);
}
