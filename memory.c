#include <stddef.h>

typedef struct {
  size_t used;
  size_t size;
  void *buf;
} Memory;

static void *mem_alloc(Memory *mem, size_t s) {
  if(mem->size < s + mem->used)
    return NULL;

  void *result = mem->buf + mem->used;
  mem->used += s;
  return result;
}
