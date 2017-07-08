#include "hal.h"

void HAL_PortOutByte(int port, unsigned char v) {
  __asm("out dx, al\n" : : "a" (v), "d" (port));
}

void HAL_PortOutWord(int port, unsigned short v) {
  __asm("out dx, ax\n" : : "a" (v), "d" (port));
}

void HAL_PortOutDword(int port, unsigned int v) {
  __asm("out dx, eax\n" : : "a" (v), "d" (port));
}

unsigned char HAL_PortInByte(int port) {
  unsigned char result;
  __asm__("in %b[result], %w[port]" : [result]"=a"(result) : [port]"Nd"(port));
  return result;
}

unsigned short HAL_PortInWord(int port) {
  unsigned short result;
  __asm__("in %w[result], %w[port]" : [result]"=a"(result) : [port]"Nd"(port));
  return result;
}

unsigned int HAL_PortInDword(int port) {
  unsigned int result;
  __asm__("in %k[result], %w[port]" : [result]"=a"(result) : [port]"Nd"(port));
  return result;
}

void HAL_PauseKernel() {
  // Disable interrupts, park CPU and wait for interrupt. Illogical, but doesn't burn VMs ;)
  __asm__ __volatile__(
    "cli\n"
    "pause\n"
    "hlt\n"
    );
}