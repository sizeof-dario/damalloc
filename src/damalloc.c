#include "damalloc_internals.h"

da_arena_t *da_arenainit_internal(void *reg, size_t sz, int flags, int *warn)
{
    if(reg == NULL)
    {
        return NULL;
    }

    void *aligned_reg = (void*)ALIGN_TOALL((uintptr_t)reg);
    ptrdiff_t padding = (uintptr_t)aligned_reg - (uintptr_t)reg;

    if(padding + AL_AHDR_SIZE > sz)
    {
        errno = ENOMEM;
        return DA_ARENAINIT_FAILED;
    }

    struct da_arenaheader_t *arena = (struct da_arenaheader_t *)reg;

    int err = pthread_mutex_init(&arena->arena_lock, NULL);
    if(err > 0)
    {
        errno = err;
        return DA_ARENAINIT_FAILED;
    }

    arena->arena_brk = (void *)((uintptr_t)arena + AL_AHDR_SIZE);
    arena->arena_end = (void *)((uintptr_t)reg + sz);

    arena->arena_sbrk = NULL; /* For now. */

    char warn_msg[] = "The arena was correctly initialized but has not enough" \
                      " available space left for any memory allocation.\n";
    
    if(MIN_BLOCK_SIZE 
        > (uintptr_t)(arena->arena_end) - (uintptr_t)(arena->arena_brk))
    {
        switch (flags)
        {
        case DA_FLAG_WARN_STDERR:
            write(STDERR_FILENO, warn_msg, sizeof(warn_msg)-1);
            break;

        case DA_FLAG_WARN_PARAM:
            *warn = DA_ARENAINIT_WARNING_UNUSABLE;
            break;
        
        default: /* Same for DA_FLAG_WARN_OFF. */
            break;
        }
    }

    return (da_arena_t *)arena;
}

da_arena_t *da_arenainit(void *region, size_t size)
{
    return da_arenainit_internal(region ,size, DA_FLAG_WARN_DEFAULT, NULL);
}
