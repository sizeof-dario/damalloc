#ifndef DAMALLOC_INTERNALS_H
#define DAMALLOC_INTERNALS_H 1

#include "damalloc.h"

struct da_arenaheader_t
{
    pthread_mutex_t    arena_lock;
    
    void              *arena_brk;
    void              *arena_end;

    void            *(*arena_sbrk)(intptr_t, struct da_arenaheader_t *);
};

__attribute__((visibility("hidden")))
void *da_arenainit_internal(void *region, size_t size, int flags, int *warning);



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

#endif /* DAMALLOC_INTERNALS_H. */
