#include "../crt.h"


#define SCAN_FLAGS_NONE          (0)
#define SCAN_FLAGS_SUPPRESSED    (1 << 0)
#define SCAN_FLAGS_TYPE_CHAR     (1 << 1)
#define SCAN_FLAGS_TYPE_SHORT    (1 << 2)
#define SCAN_FLAGS_TYPE_LONG     (1 << 3)
#define SCAN_FLAGS_TYPE_LLONG    (1 << 4)
#define SCAN_FLAGS_TYPE_SIZE     (1 << 5)
#define SCAN_FLAGS_TYPE_PTRDIFF  (1 << 6)
#define SCAN_FLAGS_TYPE_INTPTR   (1 << 7)
#define SCAN_FLAGS_TYPE_LDOUBLE  (1 << 8)
#define SCAN_FLAGS_TYPE_INTMAX   (1 << 9)
#define SCAN_FLAGS_TYPE_UNSIGNED (1 << 31)
#define SCAN_FLAGS_ALL_TYPES     (SCAN_FLAGS_TYPE_CHAR | SCAN_FLAGS_TYPE_SHORT | SCAN_FLAGS_TYPE_LONG | SCAN_FLAGS_TYPE_LLONG | SCAN_FLAGS_TYPE_SIZE | SCAN_FLAGS_TYPE_PTRDIFF | SCAN_FLAGS_TYPE_INTPTR | SCAN_FLAGS_TYPE_LDOUBLE | SCAN_FLAGS_TYPE_INTMAX | SCAN_FLAGS_TYPE_UNSIGNED)
#define SCAN_FLAGS_TYPE_UCHAR    (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_CHAR)
#define SCAN_FLAGS_TYPE_USHORT   (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_SHORT)
#define SCAN_FLAGS_TYPE_ULONG    (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_LONG)
#define SCAN_FLAGS_TYPE_ULLONG   (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_LLONG)
#define SCAN_FLAGS_TYPE_UINTMAX  (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_INTMAX)
#define SCAN_FLAGS_TYPE_UINTPTR  (SCAN_FLAGS_TYPE_UNSIGNED | SCAN_FLAGS_TYPE_INTPTR)

// TODO: Implemement streams to use this function.
typedef void* FILE;

typedef struct __libc_scan_status_t {
    FILE* stream;
    const char* string;
    int matched;
    int flags;
    int base;
    int precision;
    size_t processed;
    size_t current;
    unsigned int width;
    va_list arg;
} __libc_scan_status_t;

static int __libc_getc_unlocked(FILE* stream) {
    (void)stream;
    return EOF;
}

static int __libc_ungetc_unlocked(int character, FILE* stream) {
    (void)character;
    (void)stream;
    return EOF;
}

static int __libc_scan_worker_get_char(__libc_scan_status_t* status) {
    int result = EOF;

    if (status->stream != NULL) {
        result = __libc_getc_unlocked(status->stream);
    } else {
        result = (*status->string == '\0') ? EOF : (int)(unsigned char)*((status->string)++);
    }

    if (result != EOF) {
        ++(status->processed);
        ++(status->current);
    }

    return result;
}

static int __libc_scan_in_scanset(const char* scanlist, const char* endscanlist, int character) {
    int previous = -1;

    while (scanlist != endscanlist) {
        if ((*scanlist == '-') && (previous != -1)) {
            if (++scanlist == endscanlist) {
                return character == '-';
            }

            while (++previous <= (unsigned char)*scanlist) {
                if (previous == character) {
                    return 1;
                }

                previous = -1;
            }
        } else {
            if (character == (int)(unsigned char)(*scanlist)) {
                return 1;
            }

            previous = (unsigned char)(*scanlist++);
        }
    }

    return 0;
}

static void __libc_scan_worker_unget_char(__libc_scan_status_t* status, int character) {
    if (status->stream != NULL) {
        __libc_ungetc_unlocked(character, status->stream);
    } else {
        --(status->string);
    }

    --(status->processed);
    --(status->current);
}

static const char* __libc_scan_worker_process(__libc_scan_status_t* status, const char* specifier) {
    int character = EOF;

    const char* original_specifier = specifier;

    if (*(++specifier) == '%') {
        character = __libc_scan_worker_get_char(status);

        switch (character) {
        case EOF: {
                if (status->matched == 0) {
                    status->matched = -1;
                }

                return NULL;
            }
        case '%': {
                return ++specifier;
            }
        default: {
                __libc_scan_worker_unget_char(status, character);
                break;
            }
        }
    }

    status->flags = 0;
    status->base = -1;
    status->current = 0;
    status->width = 0;
    status->precision = 0;

    if (*specifier == '*') {
        status->flags |= SCAN_FLAGS_SUPPRESSED;
        ++specifier;
    }

    //
    // Parse width specifier.
    //
    const char* previous_specifier = specifier;
    status->width = (int)strtol(specifier, (char**)&specifier, 10);

    if (specifier == previous_specifier) {
        status->width = UINT_MAX;
    }

    //
    // Parse specifier.
    //
    switch (*(specifier++)) {
    case 'h': {
            if (*specifier == 'h') {
                status->flags |= SCAN_FLAGS_TYPE_CHAR;
                ++specifier;
            } else {
                status->flags |= SCAN_FLAGS_TYPE_SHORT;
            }
            break;
        }
    case 'l': {
            if (*specifier == 'l') {
                status->flags |= SCAN_FLAGS_TYPE_LLONG;
                ++specifier;
            } else {
                status->flags |= SCAN_FLAGS_TYPE_LONG;
            }
            break;
        }
    case 'j': {
            status->flags |= SCAN_FLAGS_TYPE_INTMAX;
            break;
        }
    case 'z': {
            status->flags |= SCAN_FLAGS_TYPE_SIZE;
            break;
        }
    case 't': {
            status->flags |= SCAN_FLAGS_TYPE_PTRDIFF;
            break;
        }
    case 'L': {
            status->flags |= SCAN_FLAGS_TYPE_LDOUBLE;
            break;
        }
    default: {
            --specifier;
            break;
        }
    }

    //
    // Parse base specifier.
    //
    int value_parsed = 0;

    switch (*specifier) {
    case 'd': {
            status->base = 10;
            break;
        }
    case 'i': {
            status->base = 0;
            break;
        }
    case 'o': {
            status->base = 8;
            status->flags |= SCAN_FLAGS_TYPE_UNSIGNED;
            break;
        }
    case 'u': {
            status->base = 10;
            status->flags |= SCAN_FLAGS_TYPE_UNSIGNED;
            break;
        }
    case 'x': {
            status->base = 16;
            status->flags |= SCAN_FLAGS_TYPE_UNSIGNED;
            break;
        }
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'a':
    case 'A': {
            break;
        }
    case 'c': {
            char* ch = va_arg(status->arg, char*);
            if (status->width == UINT_MAX) {
                status->width = 1;
            }

            while ((status->current < status->width) && ((character = __libc_scan_worker_get_char(status)) != EOF)) {
                *(ch++) = character;
                value_parsed = 1;
            }

            if (value_parsed) {
                ++status->matched;
                return ++specifier;
            } else {
                if (status->matched == 0) {
                    status->matched = -1;
                }

                return NULL;
            }
        }
    case 's': {
            char* ch = va_arg(status->arg, char*);

            while ((status->current < status->width) && ((character = __libc_scan_worker_get_char(status)) != EOF)) {
                if (isspace(character)) {
                    __libc_scan_worker_unget_char(status, character);

                    if (value_parsed) {
                        (*ch) = '\0';
                        ++status->matched;
                        return ++specifier;
                    } else {
                        return NULL;
                    }
                } else {
                    value_parsed = 1;
                    *(ch++) = character;
                }
            }

            if (value_parsed) {
                (*ch) = '\0';
                ++status->matched;
                return ++specifier;
            } else {
                if (status->matched == 0) {
                    status->matched = -1;
                }
                return NULL;
            }
        }
    case '[': {
            const char* end_specifier = specifier;
            int negative_scanlist = 0;
            if (*(++end_specifier) == '^') {
                negative_scanlist = 1;
                ++end_specifier;
            }

            specifier = end_specifier;

            do {
                ++end_specifier;
            } while (*end_specifier != ']');

            char* ch = va_arg(status->arg, char*);

            while ((status->current < status->width) && ((character = __libc_scan_worker_get_char(status)) != EOF)) {
                if (negative_scanlist) {
                    if (__libc_scan_in_scanset(specifier, end_specifier, character)) {
                        __libc_scan_worker_unget_char(status, character);
                        break;
                    }
                } else {
                    if (!__libc_scan_in_scanset(specifier, end_specifier, character)) {
                        __libc_scan_worker_unget_char(status, character);
                        break;
                    }
                }

                value_parsed = 1;
                *(ch++) = character;
            }

            if (value_parsed) {
                (*ch) = '\0';
                ++status->matched;
                return ++end_specifier;
            } else {
                if (character == EOF) {
                    status->matched = -1;
                }

                return NULL;
            }
        }
    case 'p': {
            status->base = 16;
            status->flags = SCAN_FLAGS_TYPE_INTPTR | SCAN_FLAGS_TYPE_UNSIGNED;
            break;
        }
    case 'n': {
            int* value = va_arg(status->arg, int*);
            (*value) = status->processed;
            return ++specifier;
        }
    default: {
            return original_specifier;
        }
    }

    if (status->base != -1) {
        uintmax_t value = 0;
        int prefix_parsed = 0;
        int sign = 0;

        while ((status->current < status->width) && ((character = __libc_scan_worker_get_char(status)) != EOF)) {
            if (isspace(character)) {
                if (sign) {
                    __libc_scan_worker_unget_char(status, character);
                    break;
                } else {
                    --status->current;
                }
            } else if (!sign) {
                switch (character) {
                case '-': {
                        sign = -1;
                        break;
                    }
                case '+': {
                        sign = 1;
                        break;
                    }
                default: {
                        sign = 1;
                        __libc_scan_worker_unget_char(status, character);
                        break;
                    }
                }
            } else if (!prefix_parsed) {
                prefix_parsed = 1;

                if (character != '0') {
                    if (status->base == 0) {
                        status->base = 10;
                    }

                    __libc_scan_worker_unget_char(status, character);
                } else {
                    if ((status->current < status->width) && ((character = __libc_scan_worker_get_char(status)) != EOF)) { 
                        if (character == 'x' || character == 'X') {
                            if (status->base == 0 || status->base == 16) {
                                status->base = 16;
                            } else {
                                __libc_scan_worker_unget_char(status, character);
                                value_parsed = 1;
                            }
                        } else if (character == 'b' || character == 'B') {
                            if (status->base == 0 || status->base == 2) {
                                status->base = 2;
                            } else {
                                __libc_scan_worker_unget_char(status, character);
                                value_parsed = 1;
                            }
                        } else {
                            __libc_scan_worker_unget_char(status, character);
                            if (status->base == 0) {
                                status->base = 8;
                            }

                            value_parsed = 1;
                        }
                    } else {
                        value_parsed = 1;
                        break;
                    }
                }
            } else {
                // TODO: possible sign bug
                const char* digits = memchr(&__libc_ctype_digits_lowercase[0], tolower(character), status->base);
                if (digits == NULL) {
                    __libc_scan_worker_unget_char(status, character);
                    break;
                }

                value *= status->base;
                value += digits - &__libc_ctype_digits_lowercase[0];
                value_parsed = 1;
            }
        }

        if (!value_parsed) {
            if ((status->matched == 0) && (character == EOF)) {
                status->matched = -1;
            }

            return NULL;
        }

        if (!__libc_flag_has(status->flags, SCAN_FLAGS_SUPPRESSED)) {
            switch (status->flags & SCAN_FLAGS_ALL_TYPES) {
            case SCAN_FLAGS_TYPE_CHAR: {
                    *(va_arg(status->arg, char*)) = (char)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_UCHAR: {
                    *(va_arg(status->arg, unsigned char*)) = (unsigned char)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_SHORT: {
                    *(va_arg(status->arg, short*)) = (short)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_USHORT: {
                    *(va_arg(status->arg, unsigned short*)) = (unsigned short)(value * sign);
                    break;
                }
            case SCAN_FLAGS_NONE: {
                    *(va_arg(status->arg, int*)) = (int)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_UNSIGNED: {
                    *(va_arg(status->arg, unsigned int*)) = (unsigned int)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_LONG: {
                    *(va_arg(status->arg, long*)) = (long)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_ULONG: {
                    *(va_arg(status->arg, unsigned long*)) = (unsigned long)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_LLONG: {
                    *(va_arg(status->arg, long long*)) = (long long)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_ULLONG: {
                    *(va_arg(status->arg, unsigned long long*)) = (unsigned long long)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_INTMAX: {
                    *(va_arg(status->arg, intmax_t*)) = (intmax_t)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_UINTMAX: {
                    *(va_arg(status->arg, uintmax_t*)) = (uintmax_t)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_SIZE: {
                    *(va_arg(status->arg, size_t*)) = (size_t)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_PTRDIFF: {
                    *(va_arg(status->arg, ptrdiff_t*)) = (ptrdiff_t)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_INTPTR: {
                    *(va_arg(status->arg, intptr_t*)) = (intptr_t)(value * sign);
                    break;
                }
            case SCAN_FLAGS_TYPE_UINTPTR: {
                    *(va_arg(status->arg, uintptr_t*)) = (uintptr_t)(value * sign);
                    break;
                }
            default: {
                    // TODO: assert that state.
                    return NULL;
                }
            }
            ++status->matched;
        }
        return ++specifier;
    }

    return NULL;
}


// LIBC public functions

int (vsscanf)(const char* restrict str, const char* restrict format, va_list arglist) {
    __libc_scan_status_t status = {
        .stream = NULL,
        .string = str,
        .matched = 0,
        .flags = 0,
        .base = 0,
        .precision = 0,
        .processed = 0,
        .current = 0,
        .width = 0,
    };
    va_copy(status.arg, arglist);

    while (*format != '\0') {
        const char* rc;
        if ((*format != '%') || ((rc = __libc_scan_worker_process(&status, format)) == format)) {
            if (isspace(*format)) {
                while (isspace(*status.string)) {
                    ++status.string;
                    ++status.processed;
                }
            } else {
                if (*status.string != *format) {
                    if (*status.string == '\0' && status.matched == 0) {
                        status.matched = EOF;
                        break;
                    }

                    //return status.matched;
                    break;
                } else {
                    ++status.string;
                    ++status.processed;
                }
            }

            ++format;
        } else {
            if (rc == NULL) {
                if ((*status.string == '\n') && (status.matched == 0)) {
                    status.matched = EOF;
                }

                break;
            }

            format = rc;
        }
    }

    va_end(status.arg);

    return status.matched;
}

int (sscanf)(const char* restrict str, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    int result = vsscanf(str, format, args);
    va_end(args);
    return result;
}
