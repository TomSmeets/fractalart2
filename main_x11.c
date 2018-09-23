#include "art.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <X11/Xlib.h>

int main(int argc, const char *argv[]) {
  int width  = 0;
  int height = 0;

  Display *dpy = XOpenDisplay(NULL);
  if(!dpy) {
    fprintf(stderr, "Could not open the X11 display!\n");
    return 1;
  }

#if 1
  Window  win = DefaultRootWindow(dpy);
#else
  Window win = XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),0,0,1024,1024,1,0,0);
  XMapWindow(dpy,win);
  XSelectInput(dpy,win,ExposureMask);
#endif

  {
    XWindowAttributes attr;
    XGetWindowAttributes(dpy, win, &attr);
    width = attr.width;
    height = attr.height;
    printf("Detected Sreen: %dx%d\n", width, height);
  }

  ColorRGB *px = calloc(sizeof(*px), width * height);


  Image img = {
    .width  = width,
    .height = height,
    .px = px,
  };

  size_t mem_size = art_size(width, height);
  Platform p = {
    .gen = {
      .max = RAND_MAX,
      .next = rand,
    },
    .mem = {
      .size = mem_size,
      .buf  = calloc(1, mem_size),
    }
  };

  srand(time(0));
  art_generate(&img, &p);

  int screen     = DefaultScreen(dpy);
  Visual *visual = DefaultVisual(dpy, screen);
  GC gc = DefaultGC(dpy, screen);

  int depth = 24;
  XImage *x_img = XCreateImage(dpy, visual, depth, ZPixmap, 0, (char *) img.px, img.width, img.height, 32, 0);
  Pixmap x_pxm = XCreatePixmap(dpy, win, img.width, img.height, depth);
  XPutImage(dpy, x_pxm, gc, x_img, 0, 0, 0, 0, img.width, img.height);
  XSetWindowBackgroundPixmap(dpy, win, x_pxm);

#if 1
  { // refresh root window (I used xrefresh for reference)
    uint32_t mask = CWBackPixmap | CWOverrideRedirect | CWBackingStore | CWSaveUnder;
    XSetWindowAttributes attr = {
      .background_pixmap = ParentRelative,
      .override_redirect = True,
      .backing_store = NotUseful,
      .save_under = False,
    };
    Window w = XCreateWindow(dpy, win, 0, 0, img.width, img.height, 0, DefaultDepth(dpy, screen), InputOutput, visual, mask, &attr);
    XMapWindow(dpy, w);
  }
#endif

  XCloseDisplay(dpy);
  return 0;
}
