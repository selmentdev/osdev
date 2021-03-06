#include "common.h"
#include "idt.h"
#include "terminal.h"
#include "terminal_backend_b8000.h"
#include "terminal_backend_uart16550.h"
#include "crt.h"
#include "hal.h"


int div(int a, int b) {
  return a/b;
}

void _start(void* kernel_location) {
  UNUSED(kernel_location);
  SetIDTR();

  Console = TerminalBackendUART16550();
  TerminalBackend *con = Console;

  T_Initialize(con);
  T_ClearScreen(con);
  T_SetColor(con, COLOR_WHITE, 0, 0);
  T_PutText(con, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\t\tXXXX\n");
  T_Printf(con, "%p %x %i %s %c %u\n",
      con, 0x41424344, -1234, "alamakota", 'X', 1234567890123LL);

  //unsigned long long addr = (unsigned long long)kernel_location;
  //unsigned long long addr = (unsigned long long)k;
  //for (int i = 0; i < 16; i++) {
  //  textvram[i * 2] = "0123456789ABCDEF"[(addr >> 60) & 0xf];
  //  addr <<= 4;
  //}

  char buffer[128];
  snprintf(buffer, sizeof(buffer), "Hello from %p", _start);
  T_Printf(con, "%s", buffer);

  int a = 0, b = 0;
  __asm__ volatile("div %2\n"
       : "=d" (a), "=a" (b)
       : "r" (a), "d" (a), "a" (a));

  T_PutText(con, "It came back!");

  HAL_PauseKernel();
}
