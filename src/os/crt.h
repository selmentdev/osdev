#pragma once
#include "common.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

// <ctype.h>

#define CTYPE_NONE          0x0000
#define CTYPE_ALPHA         0x0001
#define CTYPE_BLANK         0x0002
#define CTYPE_CNTRL         0x0004
#define CTYPE_GRAPH         0x0008
#define CTYPE_PUNCT         0x0010
#define CTYPE_SPACE         0x0020
#define CTYPE_LOWER         0x0040
#define CTYPE_UPPER         0x0080
#define CTYPE_DIGIT         0x0100
#define CTYPE_XDIGIT        0x0200
#define CTYPE_CBYTE         0x0400
#define CTYPE_LEADBYTE      0x8000

#define CT_UPPER            CTYPE_UPPER
#define CT_LOWER            CTYPE_LOWER
#define CT_DIGIT            CTYPE_DIGIT
#define CT_SPACE            CTYPE_SPACE
#define CT_PUNCT            CTYPE_PUNCT
#define CT_CNTRL            CTYPE_CNTRL
#define CT_BLANK            CTYPE_BLANK
#define CT_XDIGIT           CTYPE_XDIGIT
#define CT_GRAPH            CTYPE_GRAPH
#define CT_LEADBYTE         CTYPE_LEADBYTE
#define CT_CBYTE            CTYPE_CBYTE
#define CT_ALPHA            (CTYPE_ALPHA | CTYPE_UPPER | CTYPE_LOWER)

extern int isalnum(int c);
extern int isalpha(int c);
extern int isblank(int c);
extern int iscntrl(int c);
extern int isctype(int c, int desc);
extern int isdigit(int c);
extern int isgraph(int c);
extern int islower(int c);
extern int isprint(int c);
extern int ispunct(int c);
extern int isspace(int c);
extern int isupper(int c);
extern int isxdigit(int c);
extern int tolower(int c);
extern int toupper(int c);


// <errno.h>

typedef int errno_t;

// Error Codes: 7.5 p.2, 7.5 p.4

#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
#define EILSEQ          42


// <stdio.h>

#define EOF             -1

extern int snprintf(char *string, size_t size, const char *format, ...);
extern int sprintf(char *string, const char *format, ...);
extern int vsnprintf(char *string, size_t size, const char *format, va_list arglist);
extern int vsprintf(char *string, const char *format, va_list arglist);
extern int sscanf(const char* restrict str, const char* restrict format, ...);
extern int vsscanf(const char* restrict str, const char* restrict format, va_list arglist);


// <string.h>

extern void* memchr(const void* ptr, int ch, size_t count);
extern int memcmp(const void* lhs, const void* rhs, size_t count);
extern void* memcpy(void *restrict dest, const void *restrict src, size_t count);
extern void* memmove(void* dest, const void* src, size_t count);
extern void* memset(void *dest, int ch, size_t count);
extern char* strcat(char* restrict dest, const char* restrict src);
extern char* strchr(const char* str, int ch);
extern int strcmp(const char* lhs, const char* rhs);
extern char* strcpy(char* restrict dest, const char* restrict src);
extern size_t strcspn(const char* dest, const char* src);
extern size_t strlen(const char* str);
extern char* strncat(char* restrict dest, const char* restrict src, size_t count);
extern int strncmp(const char* lhs, const char* rhs, size_t count);
extern char* strncpy(char* restrict dest, const char* restrict src, size_t count);
extern char* strpbrk(const char* dest, const char* breakset);
extern char* strrchr(const char* str, int ch);
extern size_t strspn(const char* dest, const char* src);
extern char* strstr(const char* str, const char* substr);
extern size_t strxfrm(char* restrict dest, const char* restrict src, size_t count);
extern size_t strnlen_s(const char* str, size_t max_count);

// <stdlib.h>

extern int (abs)(int n);
extern long int (labs)(long int n);
extern long long int (llabs)(long long int n);

extern long int (strtol)(const char* restrict str, char** restrict endstr, int base);
extern long long int (strtoll)(const char* restrict str, char** restrict endstr, int base);
extern unsigned long int (strtoul)(const char* restrict str, char** restrict endstr, int base);
extern unsigned long long int (strtoull)(const char* restrict str, char** restrict endstr, int base);

// Additional LIBC macros.
#define __libc_flag_set(__Value, __Mask)                    ((__Value) | (__Mask))
#define __libc_flag_unset(__Value, __Mask)                  ((__Value) & (~(__Mask)))
#define __libc_flag_has(__Value, __Mask)                    (((__Value) & (__Mask)) == (__Mask))
#define __libc_flag_any(__Value, __Mask)                    (((__Value) & (__Mask)) != 0)
#define __libc_flag_none(__Value, __Mask)                   (((__Value) & (__Mask)) == 0)