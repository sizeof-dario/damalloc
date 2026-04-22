# damalloc design route

> [NOTE!]
> Some of the concepts below may not have been implemented yet! This file is also intended as a journal to write down some decisions before they are turned into code.

## How to use the allocator
The user can build it as static or dynamic library. The dynamic one can be linked dynamically with LD_PRELOAD.

## Arenas
**damalloc** is an arena allocator. Arenas are, as far as this allocator cares, memory regions with a paricular header, providing a mean of interaction to its functions. This abstraction allows allocation not only on the heap, but also on the stack or even inside previously allocated memory blocks. 

Blocks have headers too.

Inside an arena, allocated blocks will be linked via a double linked list. It's not the most efficient way to do this, but – for now – it's the easiest. 

## How allocation works
If a user arena is used for allocation, then allocation always mimics **sbrk**() calls. If the arena is the heap, then the allocator chooses **sbrk**() or **mmap**() depending on the size of the requested memory. A threshold is used, configurable by the user.

## The API

There are one or two to use the allocator functions, depending on where allocation should happen. For example, given that the **da_malloc**() function has signature
```
void* da_malloc(size_t size, da_arena_t *arena);
```
The first and always-valid way is to pass a pointer to an arena to the function, but note that the call 
```
da_malloc(size, NULL);
```
is equivalent to
```
malloc(size);
```
Well, the opposite is also true; that is, for the second way, if one wants to allocate on the heap, they can call **malloc**() as usual.


Functions has two level of abstractions. The actual API function with a clean signature that can be used with default settings and an internal function implementation. The user shall call the API one, but this distinction allows a series of personalizations via a set of configuration parameters. Those parameters shall be thread local. When a new thread is spawn, it inherits its parent flags. This mimics how forking processes clones them.

### Dealing with the heap
The heap works a little different than other arenas, because its initialization is not something the user should care about. So, the function that deals with it shall be marked as `__attribute__((constructor))`.

> [NOTE!]
> This implies being dependant on the GCC compiler. It's not a big deal since a lot of this allocator is already bound to the Unix/GCC environment.

### The Fallback Arena
THe C runtime may need to allocate memory before library functions marked as `__attribute__((constructor))` can. Thus, while replacing **malloc** in the libc, **damalloc** can't work with the heap, because it hasn't been initialized to an arena yet! Because of this, a Fallback Arena strategy is how we'll deal with it. That's a global buffer whose size has a default value but can be configured by the user if they encounter a problem when starting an application with the allocator.

## Project structure

This is the current (local) project structure:

```
damalloc/
├── include/
│   └── damalloc.h
├── src/
│   └── damalloc.c
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── DESIGN.md
├── LICENSE
└── README.md
```