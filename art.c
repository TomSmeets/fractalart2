#ifdef _FORTIFY_SOURCE
#undef _FORTIFY_SOURCE
#endif

#include "rand.c"
#include "memory.c"
#include "color.c"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

typedef struct {
  int width;
  int height;
  ColorRGB *px;
} Image;

typedef struct {
  Rand gen;
  Memory mem;
} Platform;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void art_push_ring(Image *img, int r, int cx, int cy, ColorHSV *src) {
  int i = 0;

  if(r == 0)
    img->px[cy*img->width + cx] = color_hsv2rgb(src[i]);

  {
    int y = cy-r;
    if(y >= 0 && y < img->height)
      for(int x = cx-r; x < cx+r; ++x) {
        if(x >= 0 && x < img->width)
          img->px[y*img->width + x] = color_hsv2rgb(src[i]);
        ++i;
      }
    else
      i += r*2;
  }

  {
    int x = cx+r;
    if(x >= 0 && x < img->width)
      for(int y = cy-r; y < cy+r; ++y) {
        if(y >= 0 && y < img->height)
          img->px[y*img->width + x] = color_hsv2rgb(src[i]);
        ++i;
      }
    else
      i += r*2;
  }

  {
    int y = cy+r;
    if(y >= 0 && y < img->height)
      for(int x = cx+r; x > cx-r; --x) {
        if(x >= 0 && x < img->width)
          img->px[y*img->width + x] = color_hsv2rgb(src[i]);
        ++i;
      }
    else
      i += r*2;
  }

  {
    int x = cx-r;
    if(x >= 0 && x < img->width)
      for(int y = cy+r; y > cy-r; --y) {
        if(y >= 0 && y < img->height)
          img->px[y*img->width + x] = color_hsv2rgb(src[i]);
        ++i;
      }
    else
      i += r*2;
  }

  assert(i == 8*r);
}

#define MOD(a,b) ((((a)%(b))+(b))%(b))

#define FA_WIDTH     5
#define FA_COLORS    4
#define FA_ROUGHNESS 0.01f

static void art_iterate(ColorHSV *dst, int dst_count, ColorHSV *src, int src_count, int r, Rand *gen) {
  for(int i_dst = 0; i_dst < dst_count; ++i_dst) {
    int i_src = i_dst * src_count / dst_count;

    float h = 0, s = 0, v = 0;
    int o = rand_int(gen) % (2*FA_WIDTH + 1) - FA_WIDTH;

    h = src[(i_src + src_count + o) % src_count].h;
    s = src[(i_src + src_count + o) % src_count].s;
    v = src[(i_src + src_count + o) % src_count].v;

    if(rand_int(gen) % (r*FA_COLORS) == 0) {
      h = rand_normal(gen);
      s = rand_normal(gen);
      v = 1.0f;
    }

#if 1
    h += FA_ROUGHNESS * rand_uniform(gen);
    v += FA_ROUGHNESS * rand_uniform(gen);
    s += FA_ROUGHNESS * rand_uniform(gen);
#endif

    dst[i_dst].s = s;
    dst[i_dst].v = v;
    dst[i_dst].h = h;
  }
}

static size_t art_size(int width, int height) {
  return MAX(width, height) * 8 * 2 * sizeof(ColorHSV);
}

static void art_generate(Image *img, Platform *p) {
  int r_max = MAX(img->width, img->height);
  ColorHSV *src = mem_alloc(&p->mem, r_max*8*sizeof(*src));
  ColorHSV *dst = mem_alloc(&p->mem, r_max*8*sizeof(*dst));

#if 0
  int cx = img->width  / 2;
  int cy = img->height / 2;
#else
  int cx = rand_int(&p->gen) % img->width;
  int cy = rand_int(&p->gen) % img->height;
#endif

  dst[0].h = rand_normal(&p->gen);
  dst[0].s = rand_normal(&p->gen);
  dst[0].v = 1.0f;

  art_push_ring(img, 0, cx, cy, dst);
  art_iterate(src, 8, dst, 1, 1, &p->gen);
  art_push_ring(img, 1, cx, cy, src);

  for(int i = 1; i < r_max; ++i) {
    art_iterate(dst, (i + 1)*8, src, i*8, i, &p->gen);
    art_push_ring(img, i+1, cx, cy, dst);

    ColorHSV *tmp = src;
    src = dst;
    dst = tmp;
  }
}
