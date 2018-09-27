#include <stdint.h>

typedef struct {
  uint8_t r, g, b, a;
} ColorRGB;

typedef struct {
  float h, s, v;
} ColorHSV;

static ColorRGB color_hsv2rgb(ColorHSV c) {
  float h = c.h;
  float s = c.s;
  float v = c.v;

  if(h > 1) h -= (int) h;
  if(h < 0) h -= (int) h - 1;
  if(s > 1) s = 1;
  if(s < 0) s = 0;
  if(v > 1) v = 1;
  if(v < 0) v = 0;

  ColorRGB rgb;

  int sector = h * 6;
  float f = h*6 - sector; // factorial part of h
  float p = v * (1 - s);
  float q = v * (1 - s * f);
  float t = v * (1 - s * (1 - f));
  switch (sector % 6) {
  case 0:
    rgb.r = (255 * v);
    rgb.g = (255 * t);
    rgb.b = (255 * p);
    break;
  case 1:
    rgb.r = (255 * q);
    rgb.g = (255 * v);
    rgb.b = (255 * p);
    break;
  case 2:
    rgb.r = (255 * p);
    rgb.g = (255 * v);
    rgb.b = (255 * t);
    break;
  case 3:
    rgb.r = (255 * p);
    rgb.g = (255 * q);
    rgb.b = (255 * v);
    break;
  case 4:
    rgb.r = (255 * t);
    rgb.g = (255 * p);
    rgb.b = (255 * v);
    break;
  default: // case 5:
    rgb.r = (255 * v);
    rgb.g = (255 * p);
    rgb.b = (255 * q);
    break;
  }

  return rgb;
}
