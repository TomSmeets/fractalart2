typedef struct {
  int max;
  int (*next)(void);
} Rand;

static int rand_int(Rand *r) { return (r->next)(); }
static float rand_normal(Rand *r) { return (float) rand_int(r) / r->max; }
static float rand_uniform(Rand *r) { return rand_normal(r) * 2.0f - 1.0f; }
