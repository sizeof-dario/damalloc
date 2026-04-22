#ifndef DAMALLOC_H
#define DAMALLOC_H 1

#include <stdint.h>

typedef struct da_arenaheader_t da_arena_t;

#define DA_FLAG_WARN_OFF    0
#define DA_FLAG_WARN_STDERR 1
#define DA_FLAG_WARN_PARAM  2
#define DA_FLAG_WARN_DEFAULT DA_FLAG_WARN_STDERR

#define DA_ARENAINIT_FAILED (void *)(-1)
#define DA_ARENAINIT_WARNING_UNUSABLE 1

da_arena_t *da_arenainit(void *region, size_t size);

#endif /* DAMALLOC_H. */
