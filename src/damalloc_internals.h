#ifndef DAMALLOC_INTERNALS_H
#define DAMALLOC_INTERNALS_H 1

#include <errno.h>
#include <pthread.h>
#include <stdalign.h>
#include <stddef.h>
#include <unistd.h>

#include "damalloc.h"

struct da_arenaheader_t
{
    pthread_mutex_t    arena_lock;
    
    void              *arena_brk;
    void              *arena_end;

    void            *(*arena_sbrk)(intptr_t, struct da_arenaheader_t *);
};



#define ALIGN(x, to_y) ((x) + (to_y) - 1) & ~((to_y) - 1)
#define ALIGN_TOALL(x) align(x, alignof(max_align_t))

#define AL_AHDR_SIZE ALIGN_TOALL(sizeof(struct da_arenaheader_t))

__attribute__((visibility("hidden")))
da_arena_t *da_arenainit_internal(void *reg, size_t sz, int flags, int *warn);

typedef struct da_blockheader_t
{
    uint32_t                 magic;

    size_t                   payload_size;
    size_t                   canary_size;

    unsigned int             is_free    : 1;
    unsigned int             is_mmapped : 1;

    struct da_blockheader_t *bhdr_prev;
    struct da_blockheader_t *bhdr_next;
}
da_blockheader_t;

#define AL_BHDR_SIZE ALIGN_TOALL(sizeof(struct da_blockheader_t))
#define MIN_BLOCK_SIZE (AL_BHDR_SIZE + ALIGN_TOALL(1))

#endif /* DAMALLOC_INTERNALS_H. */
