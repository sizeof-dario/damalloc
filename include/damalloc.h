#ifndef DA_MALLOC_H
#define DA_MALLOC_H 1

#include <pthread.h>
#include <stdint.h>

typedef struct da_arenaheader_t
{
    pthread_mutex_t     arena_lock;
    
    void               *arena_brk;
    void               *arena_end;

    void             *(*arena_sbrk)(intptr_t, da_arenaheader_t *);
}
da_arenaheader_t;

typedef struct da_blockheader_t
{
    uint32_t            magic;

    size_t              payload_size;
    size_t              canary_size;

    unsigned int        is_free     : 1;
    unsigned int        is_mmapped  : 1;

    da_blockheader_t   *bhdr_prev;
    da_blockheader_t   *bhdr_next;
}
da_blockhedaer_t;

void *da_arenainit(void *region, size_t size);

#endif /* DA_MALLOC_H. */
