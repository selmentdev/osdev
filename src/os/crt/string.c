#include "../crt.h"

STATIC_ASSERT(CHAR_BIT == 8);

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strcat)(char* restrict dest, const char* restrict src) {
    char* destination = dest;

    // Skip string in dest.
    while (*destination != '\0') {
        ++destination;
    }

    // Copy string from src.
    while ((*destination++ = *src++)) {
        ;
    }

    return dest;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strchr)(const char* str, int ch) {
    char value = (char)ch;

    while (*str != '\0' && *str != value) {
        ++str;
    }

    if (*str == value) {
        return (char*)str;
    }

    return NULL;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
int (strcmp)(const char* lhs, const char* rhs) {
    int result = 0;

    const unsigned char* str1 = (const unsigned char*)lhs;
    const unsigned char* str2 = (const unsigned char*)rhs;

    unsigned char c1;
    unsigned char c2;

    do {
        c1 = (unsigned char)(*str1++);
        c2 = (unsigned char)(*str2++);

        result = (c1 - c2);
    } while (result == 0 && c1 != '\0');

    return result;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strcpy)(char* restrict dest, const char* restrict src) {
    char* destination = dest;

    while ((*destination++ = *src++)) {
        ;
    }

    return dest;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
size_t (strcspn)(const char* dest, const char* src) {
    const unsigned char* str = (const unsigned char*)dest;
    const unsigned char* ctrl = (const unsigned char*)src;

    unsigned char map[32];

    for (size_t i = 0; i < 32; ++i) {
        map[i] = 0;
    }

    while (*ctrl != '\0') {
        map[*ctrl >> 3] = (unsigned char)(map[*ctrl >> 3] | (1 << (*ctrl & 7)));
        ++ctrl;
    }

    size_t count = 0;
    map[0] |= 1; // We encountered NUL char at end of loop.

    while (!(map[*str >> 3] & (1 << (*str & 7)))) {
        ++count;
        ++str;
    }

    return count;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
size_t (strlen)(const char* str) {
    const char* eos = str;

    while (*eos != '\0') {
        ++eos;
    }

    return eos - str;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strncat)(char* restrict dest, const char* restrict src, size_t count) {
    char* start = dest;

    while (*dest != '\0') {
        ++dest;
    }

    --dest;

    while (count != '\0') {
        --count;

        if (!(*dest++ = *src++)) {
            return start;
        }
    }

    *dest = '\0';

    return start;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
int (strncmp)(const char* lhs, const char* rhs, size_t count) {
    if (count != 0) {
        int s1 = 0;
        int s2 = 0;

        do {
            s1 = (unsigned char)(*lhs++);
            s2 = (unsigned char)(*rhs++);
        } while (--count != 0 && s1 != 0 && (s1 == s2));

        return s1 - s2;
    }

    return 0;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strncpy)(char* restrict dest, const char* restrict src, size_t count) {
    char* it = dest;

    while (count != 0 && (*it++ = *src++)) {
        --count;
    }

    if (count != 0) {
        while (--count != 0) {
            *it++ = '\0';
        }
    }

    return dest;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strpbrk)(const char* dest, const char* breakset) {
    const unsigned char* str = (const unsigned char*)dest;
    const unsigned char* ctrl = (const unsigned char*)breakset;

    unsigned char map[32];

    for (size_t i = 0; i < 32; ++i) {
        map[i] = 0;
    }

    while (*ctrl != '\0') {
        map[*ctrl >> 3] = (unsigned char)(map[*ctrl >> 3] | (1 << (*ctrl & 7)));
        ++ctrl;
    }

    while (*str != '\0') {
        if (map[*str >> 3] & (1 << (*str & 7))) {
            return (char*)(unsigned char*)str;
        }

        ++str;
    }

    return NULL;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strrchr)(const char* str, int ch) {
    char* start = (char*)str;
    char value = (char)ch;

    while (*str != '\0') {
        ++str;
    }

    while (--str != start && *str != value) {
        ;
    }

    if (*str == value) {
        return (char*)str;
    }

    return NULL;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
size_t (strspn)(const char* dest, const char* src) {
    const unsigned char* str = (const unsigned char*)dest;
    const unsigned char* ctrl = (const unsigned char*)src;

    unsigned char map[32];

    for (size_t i = 0; i < 32; ++i) {
        map[i] = 0;
    }

    while (*ctrl != '\0') {
        map[*ctrl >> 3] = (unsigned char)(map[*ctrl >> 3] | (1 << (*ctrl & 7)));
        ++ctrl;
    }

    if (*str != '\0') {
        size_t count = 0;

        while (map[*str >> 3] & (1 << (*str & 7))) {
            ++count;
            ++str;
        }

        return count;
    }

    return 0;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
char* (strstr)(const char* str, const char* substr) {
    char character;

    if ((character = (*substr++)) != '\0') {
        size_t length = strlen(substr);

        do {
            char sc;
            do {
                if ((sc = (*str++)) == '\0') {
                    return NULL;
                }
            } while (sc != character);
        } while (strncmp(str, substr, length) != 0);
        --str;
    }

    return (char*)str;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
size_t (strxfrm)(char* restrict dest, const char* restrict src, size_t count) {
    size_t length = strlen(src);

    if (count != 0) {
        size_t copy_size = length < count ? length : count - 1;
        (void)memcpy(dest, src, copy_size);
        dest[copy_size] = '\0';
    }

    return length;
}

__attribute__((__optimize__("-fno-tree-loop-distribute-patterns")))
size_t (strnlen_s)(const char* str, size_t max_count)
{
    if (str != NULL)
    {
        const char* last = str + max_count;
        const char* it = str;

        while (it != last && *it != '\0')
        {
            ++it;
        }

        return (size_t)(it - str);
    }

    return 0;
}
