#include "art.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <dlfcn.h>

#include <X11/Xlib.h>

static void update_image(void *dll, Image *img, Display *dpy, Window win){
  if(!dll)
    return;

  XWindowAttributes attr;
  XGetWindowAttributes(dpy, win, &attr);
  ColorRGB *px = calloc(sizeof(*px), attr.width * attr.height);

  free(img->px);
  img->width = attr.width;
  img->height = attr.height;
  img->px = px;

  size_t mem_size = ((size_t (*)(Image *))dlsym(dll, "art_size"))(img);
  Platform p = {
    .gen = { .max = RAND_MAX, .next = rand, },
    .mem = { .size = mem_size, .buf = calloc(1, mem_size), }
  };

  srand(time(0));

  ((void (*)(Image *, Platform *))dlsym(dll, "art_generate"))(img, &p);

  free(p.mem.buf);
}

int main(int argc, const char *argv[]) {

  Display *dpy = XOpenDisplay(NULL);
  if(!dpy) {
    fprintf(stderr, "Could not open the X11 display!\n");
    return 1;
  }

  Window win = XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),0,0,1024,1024,1,0,0);
  XSelectInput(dpy,win,ExposureMask | KeyPressMask);
  XMapWindow(dpy,win);

  Image img = { 0 };

  int depth = 24;
  int screen     = DefaultScreen(dpy);
  Visual *visual = DefaultVisual(dpy, screen);
  GC gc = DefaultGC(dpy, screen);

  void *handle = NULL;

  XEvent ev;
  bool running = true;
  while(running) {
    if(handle)
      dlclose(handle);
    handle = dlopen("./art.so", RTLD_NOW | RTLD_LOCAL);
    update_image(handle, &img, dpy, win);

    XImage *x_img = XCreateImage(dpy, visual, depth, ZPixmap, 0, (char *) img.px, img.width, img.height, 32, 0);
    XPutImage(dpy, win, gc, x_img, 0, 0, 0, 0, img.width, img.height);

    while(XPending(dpy))
      XNextEvent(dpy, &ev);
  }

  XCloseDisplay(dpy);
  return 0;
}
