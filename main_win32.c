#include "art.c"

#include <windows.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  int width  = 0;
  int height = 0;

  width  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

  printf("Desktop Size: %dx%d\n", width, height);

  return 0;
}
