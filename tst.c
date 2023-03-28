#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <math.h>

#include "lib/numbertypes.c"
#include "lib/multimedia/colors.c"
#include "lib/multimedia/png.c"
#include "lib/multimedia/jpeg.c"
#include "lib/multimedia/gif.c"
#include "lib/multimedia/ppm.c"

enum SaveType {
  PNG_SAVE,
  JPG_SAVE,
  GIF_SAVE,
  PPM_SAVE
};

enum SelectionState {
  DONE,
  STARTED,
  NOT_STARTED,
};

/* TODO (Elias):
 * 1. bug ~ crashes when only one pixel is captured.
 * */

/* NOTE (Elias): savequality is only used when JPG_SAVE is the savetype */
uint32 screenshot(char *path, uint32 savequality);

uint32
screenshot(char* path, uint32 q)
{
  uint32 status = 0;
  enum SelectionState running = NOT_STARTED;
  int32 x, y, rx, ry, w, h;
  uint8 *values;
  enum SaveType savetype = PPM_SAVE;

  Display *display = XOpenDisplay(0);
  Window root = DefaultRootWindow(display);
  Cursor cursor1 = XCreateFontCursor(display, XC_diamond_cross);
  XGCValues gcv;
  gcv.foreground = XWhitePixel(display, 0);
  gcv.background = XBlackPixel(display, 0);
  gcv.function = GXxor;
  gcv.subwindow_mode = IncludeInferiors;
  XImage *image;
  XEvent event;

  XGrabPointer(display, root, False, ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
               GrabModeAsync, GrabModeAsync, root, cursor1, CurrentTime);
  GC gc = XCreateGC(display, root,
                    GCFunction | GCForeground | GCBackground | GCSubwindowMode,
                    &gcv);

  while (!(running == DONE)) {
    while (!(running == DONE) && XPending(display)) {
      XNextEvent(display, &event);
      switch (event.type) {
      case (MotionNotify):
        if (running == STARTED) {
          XDrawRectangle(display, root, gc, rx, ry, w, h);
          w = event.xbutton.x - x;
          h = event.xbutton.y - y;
          rx = x;
          ry = y;
          if (w < 0) {
            rx = x + w;
            w = -1 * w;
          }
          if (h < 0) {
            ry = y + h;
            h = -1 * h;
          }
          XDrawRectangle(display, root, gc, rx, ry, w, h);
        }
        break;
      case (ButtonPress):
        x = event.xbutton.x;
        y = event.xbutton.y;
        running = STARTED;
        break;
      case (ButtonRelease):
        running = DONE;
        break;
      }
    }
  }

  XDrawRectangle(display, root, gc, rx, ry, w, h);
  XSync(display, 1);

  if (w && h) {
    image = XGetImage(display, root, rx, ry, w, h, AllPlanes, ZPixmap);
    values = m_BGRA_to_RGB((uint8 *)image->data, w * h * 4);
    switch (savetype) {
      case (PNG_SAVE):
        /* TODO (Elias): PNG is not implemented yet */
        m_png_write(path, values, w, h);
        break;
      case (JPG_SAVE):
        /* TODO (Elias): JPG is not implemented yet */
        m_jpg_write(path, values, w, h, q);
        break;
      case (GIF_SAVE):
        /* TODO (Elias): GIF is not implemented yet */
        m_gif_write(path, values, w, h);
        break;
      case (PPM_SAVE):
        m_ppm_write(path, values, w, h);
        break;
    }
    free(values);
    XDestroyImage(image);
  } else {
    status = 1;
  }

  XFreeGC(display, gc);
  XCloseDisplay(display);
  return(status);
}

int
main(void)
{
  screenshot("image.ppm", PPM_SAVE);
}
