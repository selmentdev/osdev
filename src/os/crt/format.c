#include "../crt.h"

#define FORMAT_FLAGS_NONE       (0)
#define FORMAT_FLAGS_MINUS      (1 << 0)
#define FORMAT_FLAGS_PLUS       (1 << 1)
#define FORMAT_FLAGS_ALT        (1 << 2)
#define FORMAT_FLAGS_SPACE      (1 << 3)
#define FORMAT_FLAGS_ZERO       (1 << 4)
#define FORMAT_FLAGS_DONE       (1 << 5)
#define FORMAT_FLAGS_LOWERCASE  (1 << 6)
#define FORMAT_FLAGS_UNSIGNED   (1 << 7)
#define FORMAT_FLAGS_WIDE       (1 << 8)

#define FORMAT_TYPE_NONE        (0)
#define FORMAT_TYPE_CHAR        (1)
#define FORMAT_TYPE_SHORT       (2)
#define FORMAT_TYPE_LONG        (3)
#define FORMAT_TYPE_LLONG       (4)
#define FORMAT_TYPE_INTMAX      (5)
#define FORMAT_TYPE_SIZE        (6)
#define FORMAT_TYPE_UINTPTR     (7)
#define FORMAT_TYPE_PTRDIFF     (8)
#define FORMAT_TYPE_LDOUBLE     (9)

// TODO: Implement file streams.
typedef void* FILE;

typedef struct __libc_format_status_t {
    int32_t base;
    uint32_t flags;
    uint32_t type;
    size_t max_count;
    size_t processed;
    size_t current;
    int32_t width;
    int32_t precision;
    char* string;
    FILE* stream;
    va_list args;
} __libc_format_status_t;

static int __libc_putc_unlocked(int character, FILE* stream) {
    __libc_unused__(character);
    __libc_unused__(stream);
    return 0;
    //return fputc(character, stream);
}

static int __libc_format_worker_emit_char_to_string(__libc_format_status_t* status, int character, size_t count) {
    size_t processed = 0;
    for (size_t i = 0; i < count; ++i) {
        if (status->processed < status->max_count) {
            ++processed;
            status->string[status->processed] = character;
        }
        ++(status->processed);
    }

    status->current += processed;

    return processed == count;
}

static int __libc_format_worker_emit_char_to_stream(__libc_format_status_t* status, int character, size_t count) {
    size_t processed = 0;

    // TODO: optimize for small strings? For i.e. count > 16, replicate it over array and write in single batch?
    for (size_t i = 0; i < count; ++i) {
        if (status->processed < status->max_count) {
            ++processed;
            __libc_putc_unlocked(character, status->stream);
        }
        ++(status->processed);
    }

    status->current += processed;

    return processed == count;
}

static int __libc_format_worker_emit_string_to_string(__libc_format_status_t* status, const char* string, size_t length) {
    size_t processed = 0;
    // TODO: use strncpy?
    for (size_t i = 0; i < length; ++i) {
        if (status->processed < status->max_count) {
            ++processed;
            status->string[status->processed] = string[i];
        }
        ++(status->processed);
    }

    status->current += processed;

    return processed == length;
}

static int __libc_format_worker_emit_string_to_stream(__libc_format_status_t* status, const char* string, size_t length) {
    size_t processed = 0;
    // TODO: use __libc_fwrite_unlocked
    for (size_t i = 0; i < length; ++i) {
        if (status->processed < status->max_count) {
            ++processed;
            __libc_putc_unlocked(string[i], status->stream);
        }
        ++(status->processed);
    }

    status->current += processed;

    return processed == length;
}

static int __libc_format_worker_emit_char(__libc_format_status_t* status, const char character, size_t count) {
    if (status->stream != NULL) {
        return __libc_format_worker_emit_char_to_stream(status, character, count);
    } else {
        return __libc_format_worker_emit_char_to_string(status, character, count);
    }
}

static int __libc_format_worker_emit_string(__libc_format_status_t* status, const char* string, size_t length) {
    if (status->stream != NULL) {
        return __libc_format_worker_emit_string_to_stream(status, string, length);
    } else {
        return __libc_format_worker_emit_string_to_string(status, string, length);
    }
}

static int __libc_format_worker_write_string(__libc_format_status_t* status, const char* string) {
    size_t length = status->precision >= 0 ? strnlen_s(string, status->precision) : strlen(string);

    if (status->width == 0 || __libc_flag_has(status->flags, FORMAT_FLAGS_MINUS)) {
        if (status->precision > 0) {
            size_t precision = (size_t)status->precision;
            length = precision < length ? precision : length;
        }

        if (!__libc_format_worker_emit_string(status, string, length)) {
            return 0;
        }

        if ((size_t)status->width > status->current) {
            length = status->width - status->current;

            if (!__libc_format_worker_emit_char(status, ' ', length)) {
                return 0;
            }
        }
    } else {
        if ((size_t)status->width > length) {
            size_t padding = status->width - length;

            if (!__libc_format_worker_emit_char(status, ' ', padding)) {
                return 0;
            }
        }

        if (!__libc_format_worker_emit_string(status, string, length)) {
            return 0;
        }
    }

    return 1;
}

static int __libc_format_worker_write_char(__libc_format_status_t* status, const char value) {
    if (__libc_flag_has(status->flags, FORMAT_FLAGS_MINUS)) {
        if (!__libc_format_worker_emit_char(status, value, 1)) {
            return 0;
        }

        if ((size_t)(status->width) > status->current) {
            if (!__libc_format_worker_emit_char(status, ' ', status->width - status->current)) {
                return 0;
            }
        }
    } else {
        if (status->width != 0) {
            int adjust = status->width - status->current - 1;
            if (!__libc_format_worker_emit_char(status, ' ', adjust)) {
                return 0;
            }
        }

        if (!__libc_format_worker_emit_char(status, value, 1)) {
            return 0;
        }
    }

    return 1;
}

static int __libc_format_worker_write_integer(__libc_format_status_t* status, uintmax_t value) {
    char sign = '\0';

    if (!__libc_flag_has(status->flags, FORMAT_FLAGS_UNSIGNED)) {
        const intmax_t signvalue = (intmax_t)value;
        const int is_negative= signvalue < 0;

        value = is_negative ? (uintmax_t)-signvalue : (uintmax_t)signvalue;

        if (is_negative) {
            sign = '-';
        } else if (__libc_flag_has(status->flags, FORMAT_FLAGS_PLUS)) {
            sign = '+';
        } else if (__libc_flag_has(status->flags, FORMAT_FLAGS_SPACE)) {
            sign = ' ';
        }
    }

    enum {
        max_int_length = 96
    };

    char output_buffer[max_int_length];
    char* output_buffer_end = output_buffer + max_int_length;

    ptrdiff_t written = 0;

    const char* digits = __libc_flag_has(status->flags, FORMAT_FLAGS_LOWERCASE) ? __libc_ctype_digits_lowercase : __libc_ctype_digits_uppercase; // ->is_lower_case_specifier() ? libcrt_ctype::array_dec_digits : libcrt_ctype::array_hex_digits;

    //
    // Write number in specified base.
    //
    uintmax_t remaining = value;

    if (status->precision != 0 || remaining != 0) {
        do {
            uintmax_t digit = remaining % status->base;
            remaining /= status->base;
            output_buffer_end[-++written] = digits[digit];
        } while (remaining != 0);
    }

    while ((int32_t)written < status->precision) {
        output_buffer_end[-++written] = '0';
    }

    unsigned padding = 0;

    if (!__libc_flag_has(status->flags, FORMAT_FLAGS_MINUS) && __libc_flag_has(status->flags, FORMAT_FLAGS_ZERO)) {
        while (written < (ptrdiff_t)status->width) {
            output_buffer_end[-++written] = '0';
            ++padding;
        }
    }


    if (sign != 0) {
        if (padding == 0) {
            ++written;
        }

        output_buffer_end[-written] = sign;
    } else if (__libc_flag_has(status->flags, FORMAT_FLAGS_ALT)) {
        //
        // Write alternative number prefix.
        //
        switch (status->base) {
        case 2: {
                written += padding < 2 ? 2 - padding : 0;
                output_buffer_end[-written + 0] = '0';
                output_buffer_end[-written + 1] = __libc_flag_has(status->flags, FORMAT_FLAGS_LOWERCASE) ? 'b' : 'B';
                break;
            }
        case 8: {
                if (output_buffer_end[-written] != '0') {
                    output_buffer_end[-++written] = '0';
                }

                break;
            }
        case 16: {
                written += padding < 2 ? 2 - padding : 0;
                output_buffer_end[-written + 0] = '0';
                output_buffer_end[-written + 1] = __libc_flag_has(status->flags, FORMAT_FLAGS_LOWERCASE) ? 'x' : 'X';
                break;
            }
        default: {
                break;
            }
        }
    }


    if (!__libc_flag_has(status->flags, FORMAT_FLAGS_MINUS)) {
        char leading_character = (!__libc_flag_has(status->flags, FORMAT_FLAGS_MINUS) && __libc_flag_has(status->flags, FORMAT_FLAGS_ZERO)) ? '0' : ' ';

        // note: this may be replaced with batched write
        while (written < (ptrdiff_t)status->width) {
            output_buffer_end[-++written] = leading_character;
        }
    }

    return __libc_format_worker_emit_string(status, output_buffer_end - written, (size_t)written);
}

static ptrdiff_t __libc_format_worker_process(__libc_format_status_t* status, const char* specifier) {
    const char* original_specifier = specifier;

    if (*(++specifier) == '%') {
        if (!__libc_format_worker_emit_char(status, *specifier, 1)) {
            return -1;
        }

        ++specifier;
        return specifier - original_specifier;
    }

    status->flags = FORMAT_FLAGS_NONE;
    status->base = 0;
    status->type = FORMAT_TYPE_NONE;
    status->width = 0;
    status->precision = -1;
    status->current = 0;

    do {
        switch (*specifier) {
        case '-': {
                status->flags |= FORMAT_FLAGS_MINUS;
                ++specifier;
                break;
            }
        case '+': {
                status->flags |= FORMAT_FLAGS_PLUS;
                ++specifier;
                break;
            }
        case '#': {
                status->flags |= FORMAT_FLAGS_ALT;
                ++specifier;
                break;
            }
        case ' ': {
                status->flags |= FORMAT_FLAGS_SPACE;
                ++specifier;
                break;
            }
        case '0': {
                status->flags |= FORMAT_FLAGS_ZERO;
                ++specifier;
                break;
            }
        default: {
                status->flags |= FORMAT_FLAGS_DONE;
            }
        }
    } while (!__libc_flag_has(status->flags, FORMAT_FLAGS_DONE));

    if (*specifier == '*') {
        int width = va_arg(status->args, int);

        if (width < 0) {
            status->flags |= FORMAT_FLAGS_MINUS;
            width = -width;
        }

        status->width = width;
        ++specifier;
    } else {
        int width = (int)strtol(specifier, (char**)&specifier, 10);
        if (width < 0) {
            status->flags |= FORMAT_FLAGS_MINUS;
            width = -width;
        }

        status->width = width;
    }

    if (*specifier == '.') {
        ++specifier;

        if (*specifier == '*') {
            status->precision = va_arg(status->args, int);
            ++specifier;
        } else {
            status->precision = (int)strtol(specifier, (char**)&specifier, 10);
        }

        status->flags = __libc_flag_unset(status->flags, FORMAT_FLAGS_ZERO);
    }

    switch (*(specifier++)) {
    case 'h': {
            if (*specifier == 'h') {
                status->type = FORMAT_TYPE_CHAR;
                ++specifier;
            } else {
                status->type = FORMAT_TYPE_SHORT;
            }

            break;
        }
    case 'l': {
            if (*specifier == 'l') {
                status->type = FORMAT_TYPE_LLONG;
                ++specifier;
            } else {
                status->type = FORMAT_TYPE_LONG;
            }
            break;
        }
    case 'j': {
            status->type = FORMAT_TYPE_INTMAX;
            break;
        }
    case 'z': {
            status->type = FORMAT_TYPE_SIZE;
            break;
        }
    case 't': {
            status->type = FORMAT_TYPE_PTRDIFF;
            break;
        }
    case 'L': {
            status->type = FORMAT_TYPE_LDOUBLE;
            break;
        }
    default: {
            --specifier;
            break;
        }
    }

    switch (*specifier) {
    case 'b': {
            status->flags |= FORMAT_FLAGS_LOWERCASE;
        }
    case 'B': {
            status->flags |= FORMAT_FLAGS_UNSIGNED;
            status->base = 2;
            break;
        }
    case 'o': {
            status->flags |= FORMAT_FLAGS_UNSIGNED;
            status->base = 8;
            break;
        }
    case 'u': {
            status->flags |= FORMAT_FLAGS_UNSIGNED;
        }
    case 'd':
    case 'i': {
            status->base = 10;
            break;
        }
    case 'x': {
            status->flags |= FORMAT_FLAGS_LOWERCASE;
        }
    case 'X': {
            status->flags |= FORMAT_FLAGS_UNSIGNED;
            status->base = 16;
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
            char value = (char)va_arg(status->args, int);

            if (!__libc_format_worker_write_char(status, value)) {
                return -1;
            }

            ++specifier;
            return specifier - original_specifier;
        }
    case 's': {
            char* value = (char*)va_arg(status->args, char*);

            if (value == NULL) {
                if (!__libc_format_worker_write_string(status, "(null)")) {
                    return -1;
                }
            } else {
                if (!__libc_format_worker_write_string(status, value)) {
                    return -1;
                }
            }

            ++specifier;
            return specifier - original_specifier;
        }
    case 'p': {
            status->flags |= FORMAT_FLAGS_LOWERCASE;
        }
    case 'P': {
            status->flags |= FORMAT_FLAGS_UNSIGNED | FORMAT_FLAGS_ALT;
            status->base = 16;
            status->type = FORMAT_TYPE_UINTPTR;
            status->precision = sizeof(uintptr_t) * 2;
            break;
        }
    case 'n': {
            int* output = va_arg(status->args, int*);
            (*output) = (int)status->processed;
            ++specifier;
            return specifier - original_specifier;
        }
    default: {
            return 0;
        }
    }

    if (status->base != 0) {
        if (__libc_flag_has(status->flags, FORMAT_FLAGS_UNSIGNED)) {
            uintmax_t value;

            switch (status->type) {
            case FORMAT_TYPE_CHAR: {
                    value = (uintmax_t)(unsigned char)va_arg(status->args, int);
                    break;
                }
            case FORMAT_TYPE_SHORT: {
                    value = (uintmax_t)(unsigned short)va_arg(status->args, int);
                    break;
                }
            case FORMAT_TYPE_NONE: {
                    value = (uintmax_t)(unsigned int)va_arg(status->args, unsigned int);
                    break;
                }
            case FORMAT_TYPE_LONG: {
                    value = (uintmax_t)(unsigned long)va_arg(status->args, unsigned long);
                    break;
                }
            case FORMAT_TYPE_LLONG: {
                    value = (uintmax_t)(unsigned long long)va_arg(status->args, unsigned long long);
                    break;
                }
            case FORMAT_TYPE_SIZE: {
                    value = (uintmax_t)(size_t)va_arg(status->args, size_t);
                    break;
                }
            case FORMAT_TYPE_UINTPTR: {
                    value = (uintmax_t)(uintptr_t)va_arg(status->args, uintptr_t);
                    break;
                }
            case FORMAT_TYPE_PTRDIFF: {
                    value = (uintmax_t)(ptrdiff_t)va_arg(status->args, ptrdiff_t);
                    break;
                }
            case FORMAT_TYPE_INTMAX: {
                    value = (uintmax_t)(intmax_t)va_arg(status->args, intmax_t);
                    break;
                }
            default: {
                    return -1;
                }
            }

            if (!__libc_format_worker_write_integer(status, value)) {
                return -1;
            }
        } else {
            uintmax_t value;

            switch (status->type) {
            case FORMAT_TYPE_CHAR: {
                    value = (uintmax_t)(intmax_t)(char)va_arg(status->args, int);
                    break;
                }
            case FORMAT_TYPE_SHORT: {
                    value = (uintmax_t)(intmax_t)(short)va_arg(status->args, int);
                    break;
                }
            case FORMAT_TYPE_NONE: {
                    value = (uintmax_t)(intmax_t)(int)va_arg(status->args, int);
                    break;
                }
            case FORMAT_TYPE_LONG: {
                    value = (uintmax_t)(intmax_t)(long)va_arg(status->args, long);
                    break;
                }
            case FORMAT_TYPE_LLONG: {
                    value = (uintmax_t)(intmax_t)(long long)va_arg(status->args, long long);
                    break;
                }
            case FORMAT_TYPE_SIZE: {
                    value = (uintmax_t)(intmax_t)(size_t)va_arg(status->args, size_t);
                    break;
                }
            case FORMAT_TYPE_UINTPTR: {
                    value = (uintmax_t)(intmax_t)(uintptr_t)va_arg(status->args, uintptr_t);
                    break;
                }
            case FORMAT_TYPE_PTRDIFF: {
                    value = (uintmax_t)(intmax_t)(ptrdiff_t)va_arg(status->args, ptrdiff_t);
                    break;
                }
            case FORMAT_TYPE_INTMAX: {
                    value = (uintmax_t)(intmax_t)va_arg(status->args, intmax_t);
                    break;
                }
            default: {
                    return -1;
                }
            }

            if (!__libc_format_worker_write_integer(status, value)) {
                return -1;
            }
        }

        if (__libc_flag_has(status->flags, FORMAT_FLAGS_MINUS) && (status->current - status->width) != 0) {
            __libc_format_worker_emit_char(status, ' ', status->width - status->current);
        }
    }

    ++specifier;
    return specifier - original_specifier;
}

static errno_t __libc_format_worker(__libc_format_status_t* status, const char* format) {
    while (*format != '\0') {
        const char* mark = format;

        while (*format != '\0' && *format != '%') {
            ++format;
        }

        if (mark != format) {
            if (!__libc_format_worker_emit_string(status, mark, format - mark)) {
                return EINVAL;
            }
        }

        if (*format == '%') {
            ptrdiff_t consumed = __libc_format_worker_process(status, format);
            if (consumed > 0) {
                format += consumed;
            } else if (consumed == 0) {
                ++format;
                if (!__libc_format_worker_emit_char(status, *format, 1)) {
                    return EINVAL;
                }
            } else {
                return EINVAL;
            }
        }
    }

    //
    // Put NUL character at the end of string / buffer.
    //
    if (status->string != NULL) {
        if (status->processed < status->max_count) {
            status->string[status->processed] = '\0';
        } else {
            status->string[status->max_count - 1] = '\0';
        }
    }

    va_end(status->args);

    return 0;
}


// LIBC public functions

int (snprintf)(char *string, size_t size, const char *format, ...) {
    __libc_format_status_t status = {
        .base = 0,
        .flags = 0,
        .type = 0,
        .max_count = size,
        .processed = 0,
        .current = 0,
        .width = 0,
        .precision = 0,
        .string = string,
        .stream = NULL
    };

    va_start(status.args, format);
    errno_t result = __libc_format_worker(&status, format);
    va_end(status.args);

    if (result == 0) {
        return (int)status.processed;
    }

    return -1;
}

int (sprintf)(char *string, const char *format, ...) {
    __libc_format_status_t status = {
        .base = 0,
        .flags = 0,
        .type = 0,
        .max_count = SIZE_MAX,
        .processed = 0,
        .current = 0,
        .width = 0,
        .precision = 0,
        .string = string,
        .stream = NULL
    };

    va_start(status.args, format);
    errno_t result = __libc_format_worker(&status, format);
    va_end(status.args);

    if (result == 0) {
        return (int)status.processed;
    }

    return -1;
}

int (vsnprintf)(char *string, size_t size, const char *format, va_list arglist) {
    __libc_format_status_t status = {
        .base = 0,
        .flags = 0,
        .type = 0,
        .max_count = size,
        .processed = 0,
        .current = 0,
        .width = 0,
        .precision = 0,
        .string = string,
        .stream = NULL
    };

    va_copy(status.args, arglist);
    errno_t result = __libc_format_worker(&status, format);
    va_end(status.args);

    if (result == 0) {
        return (int)status.processed;
    }

    return -1;
}

int (vsprintf)(char *string, const char *format, va_list arglist) {
    __libc_format_status_t status = {
        .base = 0,
        .flags = 0,
        .type = 0,
        .max_count = SIZE_MAX,
        .processed = 0,
        .current = 0,
        .width = 0,
        .precision = 0,
        .string = string,
        .stream = NULL
    };

    va_copy(status.args, arglist);
    errno_t result = __libc_format_worker(&status, format);
    va_end(status.args);

    if (result == 0) {
        return (int)status.processed;
    }

    return -1;
}
