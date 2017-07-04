#include "../crt.h"

static errno_t __libc_string_to_uint32(const char* restrict str, char** restrict endstr, int base, uint32_t* restrict result) {
    //
    // Check base param values.
    //
    if (base < 0 || base == 1 || base > 36) {
        (*endstr) = (char*)str;
        (*result) = 0;
        return EINVAL;
    }

    const char* it = str;
    int character;

    //
    // Skip any whitespaces.
    //
    do {
        character = (int)(unsigned char)(*it++);
    } while (isspace(character));

    //
    // Check potential sign of value.
    //
    int is_negative;
    if (character == '-') {
        is_negative = 1;
        character = (int)(unsigned char)(*it++);
    } else {
        is_negative = 0;
        if (character == '+') {
            character = (int)(unsigned char)(*it++);
        }
    }

    //
    // Check string prefix.
    //
    if ((base == 0 || base == 16) && (character == '0' && (*it == 'x' || *it == 'X'))) {
        //
        // We have hex string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 16;
    } else if ((base == 0 || base == 2) && (character == '0' && (*it == 'b' || *it == 'B'))) {
        //
        // We have binary string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 2;
    }

    if (base == 0) {
        //
        // Either dec or oct.
        //
        base = (character == '0') ? 8 : 10;
    }

    uint32_t cutoff = UINT32_MAX / (uint32_t)(unsigned int)base;
    uint32_t cutlim = UINT32_MAX % (uint32_t)(unsigned int)base;

    uint32_t accumulator = 0;

    int direction = 0;

    errno_t status = 0;
    for (;; character = (int)(unsigned char)(*it++)) {
        if (isdigit(character)) {
            character -= '0';
        } else if (isalpha(character)) {
            character -= (isupper(character) ? ('A' - 10) : ('a' - 10));
        } else {
            break;
        }

        if (character >= base) {
            break;
        }

        if (direction < 0) {
            continue;
        }

        if (accumulator > cutoff || (accumulator == cutoff && (unsigned)character > cutlim)) {
            direction = -1;
            accumulator = UINT32_MAX;
            status = ERANGE;
        } else {
            direction = 1;
            accumulator *= (uint32_t)base;
            accumulator += character;
        }
    }

    if (is_negative && direction > 0) {
        //
        // Update accumulator value
        //
        accumulator = (uint32_t)-(int32_t)accumulator;
    }

    (*endstr) = (char*)(direction ? (it - 1) : str);
    (*result) = accumulator;

    return status;
}

static errno_t __libc_string_to_int32(const char* restrict str, char** restrict endstr, int base, int32_t* restrict result) {
    //
    // Check base param values.
    //
    if (base < 0 || base == 1 || base > 36) {
        (*endstr) = (char*)str;
        (*result) = 0;
        return EINVAL;
    }

    const char* it = str;
    int character;

    //
    // Skip any whitespaces.
    //
    do {
        character = (int)(unsigned char)(*it++);
    } while (isspace(character));


    //
    // Check potential sign of value.
    //
    int is_negative;
    if (character == '-') {
        is_negative = 1;
        character = (int)(unsigned char)(*it++);
    } else {
        is_negative = 0;
        if (character == '+') {
            character = (int)(unsigned char)(*it++);
        }
    }

    //
    // Check string prefix.
    //
    if ((base == 0 || base == 16) && (character == '0' && (*it == 'x' || *it == 'X'))) {
        //
        // We have hex string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 16;
    } else if ((base == 0 || base == 2) && (character == '0' && (*it == 'b' || *it == 'B'))) {
        //
        // We have binary string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 2;
    }

    if (base == 0) {
        //
        // Either dec or oct.
        //
        base = (character == '0') ? 8 : 10;
    }

    //
    // Compute parsing cutoff bounds.
    //
    int32_t cutoff = (is_negative ? INT32_MIN : INT32_MAX);
    int32_t cutlim = cutoff % base;
    cutoff /= base;

    //
    // Adjust limits for negative ranges.
    //
    if (is_negative) {
        if (cutlim > 0) {
            cutlim -= base;
            cutoff += 1;
        }

        cutlim = -cutlim;
    }

    int32_t accumulator = 0;

    int direction = 0;

    errno_t status = 0;

    //
    // Parse string.
    //
    for (;; character = (int)(unsigned char)(*it++)) {
        if (isdigit(character)) {
            character -= '0';
        } else if (isalpha(character)) {
            character -= (isupper(character) ? 'A' - 10 : 'a' - 10);
        } else {
            //
            // Invalid digit range.
            //
            break;
        }

        if (character >= base) {
            //
            // Disallow alpha digits larger than base.
            //
            break;
        }

        if (direction < 0) {
            //
            // Skip parsing until end of invalid / out of range number.
            //
            continue;
        }

        if (is_negative) {
            if (accumulator < cutoff || (accumulator == cutoff && character > cutlim)) {
                //
                // Value out of valid range.
                //
                direction = -1;
                accumulator = INT32_MIN;
                status = ERANGE;
            } else {
                //
                // Update accumulator.
                //
                direction = 1;
                accumulator *= base;
                accumulator -= character;
            }
        } else {
            if (accumulator > cutoff || (accumulator == cutoff && character > cutlim)) {
                //
                // Value out of valid range.
                //
                direction = -1;
                accumulator = INT32_MAX;
                status = ERANGE;
            } else {
                //
                // Update accumulator.
                //
                direction = 1;
                accumulator *= base;
                accumulator += character;
            }
        }
    }

    (*endstr) = (char*)(direction ? (it - 1) : str);
    (*result) = accumulator;

    return status;
}


static errno_t __libc_string_to_uint64(const char* restrict str, char** restrict endstr, int base, uint64_t* restrict result) {
    //
    // Check base param values.
    //
    if (base < 0 || base == 1 || base > 36) {
        (*endstr) = (char*)str;
        (*result) = 0;
        return EINVAL;
    }

    const char* it = str;
    int character;

    //
    // Skip any whitespaces.
    //
    do {
        character = (int)(unsigned char)(*it++);
    } while (isspace(character));

    //
    // Check potential sign of value.
    //
    int is_negative;
    if (character == '-') {
        is_negative = 1;
        character = (int)(unsigned char)(*it++);
    } else {
        is_negative = 0;
        if (character == '+')
        {
            character = (int)(unsigned char)(*it++);
        }
    }

    //
    // Check string prefix.
    //
    if ((base == 0 || base == 16) && (character == '0' && (*it == 'x' || *it == 'X'))) {
        //
        // We have hex string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 16;
    } else if ((base == 0 || base == 2) && (character == '0' && (*it == 'b' || *it == 'B'))) {
        //
        // We have binary string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 2;
    }

    if (base == 0) {
        //
        // Either dec or oct.
        //
        base = (character == '0') ? 8 : 10;
    }

    uint64_t cutoff = UINT64_MAX / (uint64_t)(unsigned int)base;
    uint64_t cutlim = UINT64_MAX % (uint64_t)(unsigned int)base;

    uint64_t accumulator = 0;

    int direction = 0;

    errno_t status = 0;
    for (;; character = (int)(unsigned char)(*it++)) {
        if (isdigit(character)) {
            character -= '0';
        } else if (isalpha(character)) {
            character -= (isupper(character) ? ('A' - 10) : ('a' - 10));
        } else {
            break;
        }

        if (character >= base) {
            break;
        }

        if (direction < 0) {
            continue;
        }

        if (accumulator > cutoff || (accumulator == cutoff && (unsigned)character > cutlim)) {
            direction = -1;
            accumulator = UINT64_MAX;
            status = ERANGE;
        } else {
            direction = 1;
            accumulator *= (uint64_t)base;
            accumulator += character;
        }
    }

    if (is_negative && direction > 0) {
        //
        // Update accumulator value
        //
        accumulator = (uint64_t)-(int64_t)accumulator;
    }

    (*endstr) = (char*)(direction ? (it - 1) : str);
    (*result) = accumulator;

    return status;
}

static errno_t __libc_string_to_int64(const char* restrict str, char** restrict endstr, int base, int64_t* restrict result) {
    //
    // Check base param values.
    //
    if (base < 0 || base == 1 || base > 36) {
        (*endstr) = (char*)str;
        (*result) = 0;
        return EINVAL;
    }

    const char* it = str;
    int character;

    //
    // Skip any whitespaces.
    //
    do {
        character = (int)(unsigned char)(*it++);
    } while (isspace(character));


    //
    // Check potential sign of value.
    //
    int is_negative;
    if (character == '-') {
        is_negative = 1;
        character = (int)(unsigned char)(*it++);
    } else {
        is_negative = 0;
        if (character == '+')
        {
            character = (int)(unsigned char)(*it++);
        }
    }

    //
    // Check string prefix.
    //
    if ((base == 0 || base == 16) && (character == '0' && (*it == 'x' || *it == 'X'))) {
        //
        // We have hex string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 16;
    } else if ((base == 0 || base == 2) && (character == '0' && (*it == 'b' || *it == 'B'))) {
        //
        // We have binary string.
        //
        character = (int)(unsigned char)it[1];
        it += 2;
        base = 2;
    }

    if (base == 0) {
        //
        // Either dec or oct.
        //
        base = (character == '0') ? 8 : 10;
    }

    //
    // Compute parsing cutoff bounds.
    //
    int64_t cutoff = (is_negative ? INT64_MIN : INT64_MAX);
    int64_t cutlim = cutoff % base;
    cutoff /= base;

    //
    // Adjust limits for negative ranges.
    //
    if (is_negative) {
        if (cutlim > 0)
        {
            cutlim -= base;
            cutoff += 1;
        }

        cutlim = -cutlim;
    }

    int64_t accumulator = 0;

    int direction = 0;

    errno_t status = 0;

    //
    // Parse string.
    //
    for (;; character = (int)(unsigned char)(*it++)) {
        if (isdigit(character)) {
            character -= '0';
        } else if (isalpha(character)) {
            character -= (isupper(character) ? 'A' - 10 : 'a' - 10);
        } else {
            //
            // Invalid digit range.
            //
            break;
        }

        if (character >= base) {
            //
            // Disallow alpha digits larger than base.
            //
            break;
        }

        if (direction < 0) {
            //
            // Skip parsing until end of invalid / out of range number.
            //
            continue;
        }

        if (is_negative) {
            if (accumulator < cutoff || (accumulator == cutoff && character > cutlim)) {
                //
                // Value out of valid range.
                //
                direction = -1;
                accumulator = INT64_MIN;
                status = ERANGE;
            } else {
                //
                // Update accumulator.
                //
                direction = 1;
                accumulator *= base;
                accumulator -= character;
            }
        } else {
            if (accumulator > cutoff || (accumulator == cutoff && character > cutlim)) {
                //
                // Value out of valid range.
                //
                direction = -1;
                accumulator = INT64_MAX;
                status = ERANGE;
            } else {
                //
                // Update accumulator.
                //
                direction = 1;
                accumulator *= base;
                accumulator += character;
            }
        }
    }

    (*endstr) = (char*)(direction ? (it - 1) : str);
    (*result) = accumulator;

    return status;
}


// LIBC public functions

long int (strtol)(const char* restrict str, char** restrict endstr, int base) {
    char* end = NULL;

#if __SIZEOF_LONG__ == __SIZEOF_LONG_LONG__
    int64_t result = 0;
    __libc_string_to_int64(str, &end, base, &result);
#else
    int32_t result = 0;
    __libc_string_to_int32(str, &end, base, &result);
#endif

    if (endstr != NULL) {
        (*endstr) = end;
    }

    return (long int)result;
}

long long int (strtoll)(const char* restrict str, char** restrict endstr, int base) {
    char* end = NULL;
    int64_t result = 0;

     __libc_string_to_int64(str, &end, base, &result);

    if (endstr != NULL) {
        (*endstr) = end;
    }

    return (long long int)result;
}

unsigned long int (strtoul)(const char* restrict str, char** restrict endstr, int base) {
    char* end = NULL;

#if __SIZEOF_LONG__ == __SIZEOF_LONG_LONG__
    uint64_t result = 0;
    __libc_string_to_uint64(str, &end, base, &result);
#else
    uint32_t result = 0;
    __libc_string_to_uint32(str, &end, base, &result);
#endif

    if (endstr != NULL) {
        (*endstr) = end;
    }

    return (unsigned long int)result;
}

unsigned long long int (strtoull)(const char* restrict str, char** restrict endstr, int base) {
    char* end = NULL;
    uint64_t result = 0;

    __libc_string_to_uint64(str, &end, base, &result);

    if (endstr != NULL) {
        (*endstr) = end;
    }

    return (unsigned long long int)result;
}
