#pragma once
#include <stdint.h>
#include <stdbool.h>

struct TerminalBackend;
typedef struct TerminalBackend TerminalBackend;

// XXX on creation limit size to 32000x32000

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

typedef void (*TBfunc_Initialize)(TerminalBackend *tb);
typedef void (*TBfunc_SetCursorPosition)(
    TerminalBackend *tb, uint16_t x, uint16_t y);
typedef void (*TBfunc_GetCursorPosition)(
    TerminalBackend *tb, uint16_t *x, uint16_t *y);
typedef void (*TBfunc_ClearScreen)(TerminalBackend *tb);
typedef void (*TBfunc_PutCharacter)(TerminalBackend *tb, uint32_t ch);
typedef void (*TBfunc_GetSize)(TerminalBackend *tb,
    uint16_t *w, uint16_t *h);
typedef void (*TBfunc_ScrollLine)(TerminalBackend *tb);
typedef void (*TBfunc_SetColor)(TerminalBackend *tb,
    unsigned char fgColor, unsigned char bgColor, bool blink);

struct TerminalBackend {
  TBfunc_Initialize func_initialize;
  TBfunc_SetCursorPosition func_set_cursor_position;
  TBfunc_GetCursorPosition func_get_cursor_position;
  TBfunc_ClearScreen func_clear_screen;
  TBfunc_PutCharacter func_put_character;
  TBfunc_GetSize func_get_size;
  TBfunc_ScrollLine func_scroll_line;
  TBfunc_SetColor func_set_color;
  void *backend_data;
};

void T_Initialize(TerminalBackend *tb);
void T_SetCursorPosition(
    TerminalBackend *tb, uint16_t x, uint16_t y);
void T_GetCursorPosition(
    TerminalBackend *tb, uint16_t *x, uint16_t *y);
void T_PutText(TerminalBackend *tb, const char *s);
void T_ClearScreen(TerminalBackend *tb);
void T_PutCharacter(TerminalBackend *tb, uint32_t ch);
void T_GetSize(TerminalBackend *tb, uint16_t *w, uint16_t *h);
void T_Printf(TerminalBackend *tb, const char *fmt, ...);
void T_ScrollLine(TerminalBackend *tb);
void T_SetColor(TerminalBackend *tb, unsigned char fgColor, unsigned char bgColor, bool blink);

extern TerminalBackend* Console;
