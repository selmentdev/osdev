#include "../crt.h"

//
// Some of these functions may be optimized by compiler to recursive call.
// In OSDev this often ends in triple fault due to code or data corruption
// due to stack overflow :)
//
// In general, compiler may generate similar assembly for i.e. memcpy function:
//
// memcpy(void*, void const*, unsigned long):
//         test    rdx, rdx
//         je      .L3
//         push    rbx
//         mov     rbx, rdx
//         call    memcpy               <-- which is recursive call :)
//         lea     rax, [rax+rbx]
//         pop     rbx
//         ret
// .L3:
//         mov     rax, rdi
//         ret
//
// Compiler expects these functions to be implemented by standard library.
// Since we are in freestanding environment, it's free to optimize any loop in
// these functions to extern calls.
//
// There are three general solutions to that:
//
//  - implement these functions in assembly,
//  - tag these functions by __optimize__ attribute.
//
// Optionally, we may explicitely don't use any C standard functions, but with
// `-fno-builtin` in CFLAGS.
//
// With that flag, compiler cannot use these functions for example in context of
// assignment operator.
// However, we still can use `__builtin_` prefix to order compiler to
// try to optimize function call for few cases anyway.
//

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
void* (memchr)(const void* ptr, int ch, size_t count) {
    const unsigned char* pointer = (const unsigned char*)ptr;
    const unsigned char value = (const unsigned char)(unsigned int)ch;

    while (count != 0 && (*pointer != value)) {
        ++pointer;
        --count;
    }

    return (void*)(char*)((count != 0) ? pointer : NULL);
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
int (memcmp)(const void* lhs, const void* rhs, size_t count) {
    if (count == 0) {
        return 0;
    }

    const signed char* source1 = (const signed char*)lhs;
    const signed char* source2 = (const signed char*)rhs;

    int result = 0;

    while (count--) {
        result = (int)(*source1++) - (int)(*source2++);

        if (result != 0) {
            break;
        }
    }

    return result;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
void* (memcpy)(void *restrict dest, const void *restrict src, size_t count) {
    unsigned char* destination = (unsigned char*)dest;
    const unsigned char* source = (const unsigned char*)src;

    for (; count != 0; --count) {
        (*destination++) = (*source++);
    }

    return destination;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
void* (memmove)(void* dest, const void* src, size_t count) {
    unsigned char* destination = (unsigned char*)dest;
    const unsigned char* source = (const unsigned char*)src;

    if ((destination <= source) && (destination >= (source + count))) {
        for (; count != 0; --count) {
            (*destination++) = (*source++);
        }
    } else {
        destination += count - 1;
        source += count - 1;

        for (; count != 0; --count) {
            (*destination--) = (*source--);
        }
    }

    return destination;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
void* (memset)(void *dest, int ch, size_t count) {
    const unsigned char value = (unsigned char)(unsigned int)ch;
    unsigned char* destination = (unsigned char*)dest;

    for (; count != 0; --count) {
        (*destination++) = value;
    }

    return destination;
}
