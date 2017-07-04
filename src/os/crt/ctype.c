#include "../crt.h"
#include <stdint.h>
#include <stddef.h>

const char __libc_ctype_digits_lowercase[] = "0123456789abcdefghijklmnopqrstuvwxyz";
const char __libc_ctype_digits_uppercase[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef struct __libc_ascii_data_entry_t {
    uint16_t flags;
    unsigned char upper;
    unsigned char lower;
    unsigned char collation;
} __libc_ascii_data_entry_t;

static const __libc_ascii_data_entry_t __libc_ascii_data_table[] = {
    { /* EOF */ 0,                                                      0,    0,    0 },
    { /* NUL */ CTYPE_CNTRL,                                            0x00, 0x00, 0x00 },
    { /* SOH */ CTYPE_CNTRL,                                            0x01, 0x01, 0x01 },
    { /* STX */ CTYPE_CNTRL,                                            0x02, 0x02, 0x02 },
    { /* ETX */ CTYPE_CNTRL,                                            0x03, 0x03, 0x03 },
    { /* EOT */ CTYPE_CNTRL,                                            0x04, 0x04, 0x04 },
    { /* ENQ */ CTYPE_CNTRL,                                            0x05, 0x05, 0x05 },
    { /* ACK */ CTYPE_CNTRL,                                            0x06, 0x06, 0x06 },
    { /* BEL */ CTYPE_CNTRL,                                            0x07, 0x07, 0x07 },
    { /*  BS */ CTYPE_CNTRL,                                            0x08, 0x08, 0x08 },
    { /*  HT */ CTYPE_CNTRL | CTYPE_BLANK | CTYPE_SPACE,                0x09, 0x09, 0x09 },
    { /*  LF */ CTYPE_CNTRL | CTYPE_SPACE,                              0x0A, 0x0A, 0x0A },
    { /*  VT */ CTYPE_CNTRL | CTYPE_SPACE,                              0x0B, 0x0B, 0x0B },
    { /*  FF */ CTYPE_CNTRL | CTYPE_SPACE,                              0x0C, 0x0C, 0x0C },
    { /*  CR */ CTYPE_CNTRL | CTYPE_SPACE,                              0x0D, 0x0D, 0x0D },
    { /*  SO */ CTYPE_CNTRL,                                            0x0E, 0x0E, 0x0E },
    { /*  SI */ CTYPE_CNTRL,                                            0x0F, 0x0F, 0x0F },
    { /* DLE */ CTYPE_CNTRL,                                            0x10, 0x10, 0x10 },
    { /* DC1 */ CTYPE_CNTRL,                                            0x11, 0x11, 0x11 },
    { /* DC2 */ CTYPE_CNTRL,                                            0x12, 0x12, 0x12 },
    { /* DC3 */ CTYPE_CNTRL,                                            0x13, 0x13, 0x13 },
    { /* DC4 */ CTYPE_CNTRL,                                            0x14, 0x14, 0x14 },
    { /* NAK */ CTYPE_CNTRL,                                            0x15, 0x15, 0x15 },
    { /* SYN */ CTYPE_CNTRL,                                            0x16, 0x16, 0x16 },
    { /* ETB */ CTYPE_CNTRL,                                            0x17, 0x17, 0x17 },
    { /* CAN */ CTYPE_CNTRL,                                            0x18, 0x18, 0x18 },
    { /*  EM */ CTYPE_CNTRL,                                            0x19, 0x19, 0x19 },
    { /* SUB */ CTYPE_CNTRL,                                            0x1A, 0x1A, 0x1A },
    { /* ESC */ CTYPE_CNTRL,                                            0x1B, 0x1B, 0x1B },
    { /*  FS */ CTYPE_CNTRL,                                            0x1C, 0x1C, 0x1C },
    { /*  GS */ CTYPE_CNTRL,                                            0x1D, 0x1D, 0x1D },
    { /*  RS */ CTYPE_CNTRL,                                            0x1E, 0x1E, 0x1E },
    { /*  US */ CTYPE_CNTRL,                                            0x1F, 0x1F, 0x1F },
    { /*  SP */ CTYPE_BLANK | CTYPE_SPACE,                              0x20, 0x20, 0x20 },
    { /* '!' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x21, 0x21, 0x21 },
    { /* '"' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x22, 0x22, 0x22 },
    { /* '#' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x23, 0x23, 0x23 },
    { /* '$' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x24, 0x24, 0x24 },
    { /* '%' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x25, 0x25, 0x25 },
    { /* '&' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x26, 0x26, 0x26 },
    { /* ''' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x27, 0x27, 0x27 },
    { /* '(' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x28, 0x28, 0x28 },
    { /* ')' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x29, 0x29, 0x29 },
    { /* '*' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2A, 0x2A, 0x2A },
    { /* '+' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2B, 0x2B, 0x2B },
    { /* ',' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2C, 0x2C, 0x2C },
    { /* '-' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2D, 0x2D, 0x2D },
    { /* '.' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2E, 0x2E, 0x2E },
    { /* '/' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x2F, 0x2F, 0x2F },
    { /* '0' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x30, 0x30, 0x30 },
    { /* '1' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x31, 0x31, 0x31 },
    { /* '2' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x32, 0x32, 0x32 },
    { /* '3' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x33, 0x33, 0x33 },
    { /* '4' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x34, 0x34, 0x34 },
    { /* '5' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x35, 0x35, 0x35 },
    { /* '6' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x36, 0x36, 0x36 },
    { /* '7' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x37, 0x37, 0x37 },
    { /* '8' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x38, 0x38, 0x38 },
    { /* '9' */ CTYPE_GRAPH | CTYPE_DIGIT | CTYPE_XDIGIT,               0x39, 0x39, 0x39 },
    { /* ':' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3A, 0x3A, 0x3A },
    { /* ';' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3B, 0x3B, 0x3B },
    { /* '<' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3C, 0x3C, 0x3C },
    { /* '=' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3D, 0x3D, 0x3D },
    { /* '>' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3E, 0x3E, 0x3E },
    { /* '?' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x3F, 0x3F, 0x3F },
    { /* '@' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x40, 0x40, 0x40 },
    { /* 'A' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x41, 0x61, 0x41 },
    { /* 'B' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x42, 0x62, 0x42 },
    { /* 'C' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x43, 0x63, 0x43 },
    { /* 'D' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x44, 0x64, 0x44 },
    { /* 'E' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x45, 0x65, 0x45 },
    { /* 'F' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER | CTYPE_XDIGIT, 0x46, 0x66, 0x46 },
    { /* 'G' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x47, 0x67, 0x47 },
    { /* 'H' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x48, 0x68, 0x48 },
    { /* 'I' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x49, 0x69, 0x49 },
    { /* 'J' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4A, 0x6A, 0x4A },
    { /* 'K' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4B, 0x6B, 0x4B },
    { /* 'L' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4C, 0x6C, 0x4C },
    { /* 'M' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4D, 0x6D, 0x4D },
    { /* 'N' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4E, 0x6E, 0x4E },
    { /* 'O' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x4F, 0x6F, 0x4F },
    { /* 'P' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x50, 0x70, 0x50 },
    { /* 'Q' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x51, 0x71, 0x51 },
    { /* 'R' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x52, 0x72, 0x52 },
    { /* 'S' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x53, 0x73, 0x53 },
    { /* 'T' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x54, 0x74, 0x54 },
    { /* 'U' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x55, 0x75, 0x55 },
    { /* 'V' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x56, 0x76, 0x56 },
    { /* 'W' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x57, 0x77, 0x57 },
    { /* 'X' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x58, 0x78, 0x58 },
    { /* 'Y' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x59, 0x79, 0x59 },
    { /* 'Z' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_UPPER,                0x5A, 0x7A, 0x5A },
    { /* '[' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x5B, 0x5B, 0x5B },
    { /* '\' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x5C, 0x5C, 0x5C },
    { /* ']' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x5D, 0x5D, 0x5D },
    { /* '^' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x5E, 0x5E, 0x5E },
    { /* '_' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x5F, 0x5F, 0x5F },
    { /* '`' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x60, 0x60, 0x60 },
    { /* 'a' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x41, 0x61, 0x61 },
    { /* 'b' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x42, 0x62, 0x62 },
    { /* 'c' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x43, 0x63, 0x63 },
    { /* 'd' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x44, 0x64, 0x64 },
    { /* 'e' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x45, 0x65, 0x65 },
    { /* 'f' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER | CTYPE_XDIGIT, 0x46, 0x66, 0x66 },
    { /* 'g' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x47, 0x67, 0x67 },
    { /* 'h' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x48, 0x68, 0x68 },
    { /* 'i' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x49, 0x69, 0x69 },
    { /* 'j' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4A, 0x6A, 0x6A },
    { /* 'k' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4B, 0x6B, 0x6B },
    { /* 'l' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4C, 0x6C, 0x6C },
    { /* 'm' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4D, 0x6D, 0x6D },
    { /* 'n' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4E, 0x6E, 0x6E },
    { /* 'o' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x4F, 0x6F, 0x6F },
    { /* 'p' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x50, 0x70, 0x70 },
    { /* 'q' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x51, 0x71, 0x71 },
    { /* 'r' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x52, 0x72, 0x72 },
    { /* 's' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x53, 0x73, 0x73 },
    { /* 't' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x54, 0x74, 0x74 },
    { /* 'u' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x55, 0x75, 0x75 },
    { /* 'v' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x56, 0x76, 0x76 },
    { /* 'w' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x57, 0x77, 0x77 },
    { /* 'x' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x58, 0x78, 0x78 },
    { /* 'y' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x59, 0x79, 0x79 },
    { /* 'z' */ CTYPE_GRAPH | CTYPE_ALPHA | CTYPE_LOWER,                0x5A, 0x7A, 0x7A },
    { /* '{' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x7B, 0x7B, 0x7B },
    { /* '|' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x7C, 0x7C, 0x7C },
    { /* '}' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x7D, 0x7D, 0x7D },
    { /* '~' */ CTYPE_GRAPH | CTYPE_PUNCT,                              0x7E, 0x7E, 0x7E },
    { /* DEL */ CTYPE_CNTRL,                                            0x7F, 0x7F, 0x7F },
    {           CTYPE_CBYTE,                                            0x80, 0x80, 0x80 },
    {           CTYPE_CBYTE,                                            0x81, 0x81, 0x81 },
    {           CTYPE_CBYTE,                                            0x82, 0x82, 0x82 },
    {           CTYPE_CBYTE,                                            0x83, 0x83, 0x83 },
    {           CTYPE_CBYTE,                                            0x84, 0x84, 0x84 },
    {           CTYPE_CBYTE,                                            0x85, 0x85, 0x85 },
    {           CTYPE_CBYTE,                                            0x86, 0x86, 0x86 },
    {           CTYPE_CBYTE,                                            0x87, 0x87, 0x87 },
    {           CTYPE_CBYTE,                                            0x88, 0x88, 0x88 },
    {           CTYPE_CBYTE,                                            0x89, 0x89, 0x89 },
    {           CTYPE_CBYTE,                                            0x8A, 0x8A, 0x8A },
    {           CTYPE_CBYTE,                                            0x8B, 0x8B, 0x8B },
    {           CTYPE_CBYTE,                                            0x8C, 0x8C, 0x8C },
    {           CTYPE_CBYTE,                                            0x8D, 0x8D, 0x8D },
    {           CTYPE_CBYTE,                                            0x8E, 0x8E, 0x8E },
    {           CTYPE_CBYTE,                                            0x8F, 0x8F, 0x8F },
    {           CTYPE_CBYTE,                                            0x90, 0x90, 0x90 },
    {           CTYPE_CBYTE,                                            0x91, 0x91, 0x91 },
    {           CTYPE_CBYTE,                                            0x92, 0x92, 0x92 },
    {           CTYPE_CBYTE,                                            0x93, 0x93, 0x93 },
    {           CTYPE_CBYTE,                                            0x94, 0x94, 0x94 },
    {           CTYPE_CBYTE,                                            0x95, 0x95, 0x95 },
    {           CTYPE_CBYTE,                                            0x96, 0x96, 0x96 },
    {           CTYPE_CBYTE,                                            0x97, 0x97, 0x97 },
    {           CTYPE_CBYTE,                                            0x98, 0x98, 0x98 },
    {           CTYPE_CBYTE,                                            0x99, 0x99, 0x99 },
    {           CTYPE_CBYTE,                                            0x9A, 0x9A, 0x9A },
    {           CTYPE_CBYTE,                                            0x9B, 0x9B, 0x9B },
    {           CTYPE_CBYTE,                                            0x9C, 0x9C, 0x9C },
    {           CTYPE_CBYTE,                                            0x9D, 0x9D, 0x9D },
    {           CTYPE_CBYTE,                                            0x9E, 0x9E, 0x9E },
    {           CTYPE_CBYTE,                                            0x9F, 0x9F, 0x9F },
    {           CTYPE_CBYTE,                                            0xA0, 0xA0, 0xA0 },
    {           CTYPE_CBYTE,                                            0xA1, 0xA1, 0xA1 },
    {           CTYPE_CBYTE,                                            0xA2, 0xA2, 0xA2 },
    {           CTYPE_CBYTE,                                            0xA3, 0xA3, 0xA3 },
    {           CTYPE_CBYTE,                                            0xA4, 0xA4, 0xA4 },
    {           CTYPE_CBYTE,                                            0xA5, 0xA5, 0xA5 },
    {           CTYPE_CBYTE,                                            0xA6, 0xA6, 0xA6 },
    {           CTYPE_CBYTE,                                            0xA7, 0xA7, 0xA7 },
    {           CTYPE_CBYTE,                                            0xA8, 0xA8, 0xA8 },
    {           CTYPE_CBYTE,                                            0xA9, 0xA9, 0xA9 },
    {           CTYPE_CBYTE,                                            0xAA, 0xAA, 0xAA },
    {           CTYPE_CBYTE,                                            0xAB, 0xAB, 0xAB },
    {           CTYPE_CBYTE,                                            0xAC, 0xAC, 0xAC },
    {           CTYPE_CBYTE,                                            0xAD, 0xAD, 0xAD },
    {           CTYPE_CBYTE,                                            0xAE, 0xAE, 0xAE },
    {           CTYPE_CBYTE,                                            0xAF, 0xAF, 0xAF },
    {           CTYPE_CBYTE,                                            0xB0, 0xB0, 0xB0 },
    {           CTYPE_CBYTE,                                            0xB1, 0xB1, 0xB1 },
    {           CTYPE_CBYTE,                                            0xB2, 0xB2, 0xB2 },
    {           CTYPE_CBYTE,                                            0xB3, 0xB3, 0xB3 },
    {           CTYPE_CBYTE,                                            0xB4, 0xB4, 0xB4 },
    {           CTYPE_CBYTE,                                            0xB5, 0xB5, 0xB5 },
    {           CTYPE_CBYTE,                                            0xB6, 0xB6, 0xB6 },
    {           CTYPE_CBYTE,                                            0xB7, 0xB7, 0xB7 },
    {           CTYPE_CBYTE,                                            0xB8, 0xB8, 0xB8 },
    {           CTYPE_CBYTE,                                            0xB9, 0xB9, 0xB9 },
    {           CTYPE_CBYTE,                                            0xBA, 0xBA, 0xBA },
    {           CTYPE_CBYTE,                                            0xBB, 0xBB, 0xBB },
    {           CTYPE_CBYTE,                                            0xBC, 0xBC, 0xBC },
    {           CTYPE_CBYTE,                                            0xBD, 0xBD, 0xBD },
    {           CTYPE_CBYTE,                                            0xBE, 0xBE, 0xBE },
    {           CTYPE_CBYTE,                                            0xBF, 0xBF, 0xBF },
    {           0x00,                                                   0xC0, 0xC0, 0xC0 },
    {           0x00,                                                   0xC1, 0xC1, 0xC1 },
    {           0x00,                                                   0xC2, 0xC2, 0xC2 },
    {           0x00,                                                   0xC3, 0xC3, 0xC3 },
    {           0x00,                                                   0xC4, 0xC4, 0xC4 },
    {           0x00,                                                   0xC5, 0xC5, 0xC5 },
    {           0x00,                                                   0xC6, 0xC6, 0xC6 },
    {           0x00,                                                   0xC7, 0xC7, 0xC7 },
    {           0x00,                                                   0xC8, 0xC8, 0xC8 },
    {           0x00,                                                   0xC9, 0xC9, 0xC9 },
    {           0x00,                                                   0xCA, 0xCA, 0xCA },
    {           0x00,                                                   0xCB, 0xCB, 0xCB },
    {           0x00,                                                   0xCC, 0xCC, 0xCC },
    {           0x00,                                                   0xCD, 0xCD, 0xCD },
    {           0x00,                                                   0xCE, 0xCE, 0xCE },
    {           0x00,                                                   0xCF, 0xCF, 0xCF },
    {           0x00,                                                   0xD0, 0xD0, 0xD0 },
    {           0x00,                                                   0xD1, 0xD1, 0xD1 },
    {           0x00,                                                   0xD2, 0xD2, 0xD2 },
    {           0x00,                                                   0xD3, 0xD3, 0xD3 },
    {           0x00,                                                   0xD4, 0xD4, 0xD4 },
    {           0x00,                                                   0xD5, 0xD5, 0xD5 },
    {           0x00,                                                   0xD6, 0xD6, 0xD6 },
    {           0x00,                                                   0xD7, 0xD7, 0xD7 },
    {           0x00,                                                   0xD8, 0xD8, 0xD8 },
    {           0x00,                                                   0xD9, 0xD9, 0xD9 },
    {           0x00,                                                   0xDA, 0xDA, 0xDA },
    {           0x00,                                                   0xDB, 0xDB, 0xDB },
    {           0x00,                                                   0xDC, 0xDC, 0xDC },
    {           0x00,                                                   0xDD, 0xDD, 0xDD },
    {           0x00,                                                   0xDE, 0xDE, 0xDE },
    {           0x00,                                                   0xDF, 0xDF, 0xDF },
    {           0x00,                                                   0xE0, 0xE0, 0xE0 },
    {           0x00,                                                   0xE1, 0xE1, 0xE1 },
    {           0x00,                                                   0xE2, 0xE2, 0xE2 },
    {           0x00,                                                   0xE3, 0xE3, 0xE3 },
    {           0x00,                                                   0xE4, 0xE4, 0xE4 },
    {           0x00,                                                   0xE5, 0xE5, 0xE5 },
    {           0x00,                                                   0xE6, 0xE6, 0xE6 },
    {           0x00,                                                   0xE7, 0xE7, 0xE7 },
    {           0x00,                                                   0xE8, 0xE8, 0xE8 },
    {           0x00,                                                   0xE9, 0xE9, 0xE9 },
    {           0x00,                                                   0xEA, 0xEA, 0xEA },
    {           0x00,                                                   0xEB, 0xEB, 0xEB },
    {           0x00,                                                   0xEC, 0xEC, 0xEC },
    {           0x00,                                                   0xED, 0xED, 0xED },
    {           0x00,                                                   0xEE, 0xEE, 0xEE },
    {           0x00,                                                   0xEF, 0xEF, 0xEF },
    {           0x00,                                                   0xF0, 0xF0, 0xF0 },
    {           0x00,                                                   0xF1, 0xF1, 0xF1 },
    {           0x00,                                                   0xF2, 0xF2, 0xF2 },
    {           0x00,                                                   0xF3, 0xF3, 0xF3 },
    {           0x00,                                                   0xF4, 0xF4, 0xF4 },
    {           0x00,                                                   0xF5, 0xF5, 0xF5 },
    {           0x00,                                                   0xF6, 0xF6, 0xF6 },
    {           0x00,                                                   0xF7, 0xF7, 0xF7 },
    {           0x00,                                                   0xF8, 0xF8, 0xF8 },
    {           0x00,                                                   0xF9, 0xF9, 0xF9 },
    {           0x00,                                                   0xFA, 0xFA, 0xFA },
    {           0x00,                                                   0xFB, 0xFB, 0xFB },
    {           0x00,                                                   0xFC, 0xFC, 0xFC },
    {           0x00,                                                   0xFD, 0xFD, 0xFD },
    {           0x00,                                                   0xFE, 0xFE, 0xFE },
    {           0x00,                                                   0xFF, 0xFF, 0xFF },
};

static const __libc_ascii_data_entry_t* __libc_get_ascii_data(int character) {
    //
    // We also accept EOF, for that case table is moved in single row away.
    //
    unsigned int index = (unsigned int)(character + 1); 

    if (index <= 256) {
        //
        // Return item if index is valid.
        //
        return &__libc_ascii_data_table[index];
    }

    //
    // No data available.
    //
    return NULL;
}

int (isalnum)(int n) {
    return isctype(n, CT_ALPHA | CT_DIGIT);
}

int (isalpha)(int n) {
    return isctype(n, CT_ALPHA);
}

int (isblank)(int n) {
    return isctype(n, CT_BLANK);
}

int (iscntrl)(int n) {
    return isctype(n, CT_CNTRL);
}

int (isctype)(int n, int desc) {
    const __libc_ascii_data_entry_t* data = __libc_get_ascii_data(n);

    if (data != NULL) {
        return (data->flags & desc) != 0;
    }

    return 0;
}

int (isdigit)(int n) {
    return isctype(n, CT_DIGIT);
}

int (isgraph)(int n) {
    return isctype(n, CT_PUNCT | CT_ALPHA | CT_DIGIT | CT_GRAPH);
}

int (islower)(int n) {
    return isctype(n, CT_LOWER);
}

int (isprint)(int n) {
    return isctype(n, CT_BLANK | CT_PUNCT | CT_ALPHA | CT_DIGIT);
}

int (ispunct)(int n) {
    return isctype(n, CT_PUNCT);
}

int (isspace)(int n) {
    return isctype(n, CT_SPACE);
}

int (isupper)(int n) {
    return isctype(n, CT_UPPER);
}

int (isxdigit)(int n) {
    return isctype(n, CT_XDIGIT);
}

int (tolower)(int n) {
    const __libc_ascii_data_entry_t* data = __libc_get_ascii_data(n);

    if (data != NULL) {
        return data->lower;
    }

    return n;
}

int (toupper)(int n) {
    const __libc_ascii_data_entry_t* data = __libc_get_ascii_data(n);

    if (data != NULL) {
        return data->upper;
    }

    return n;
}
