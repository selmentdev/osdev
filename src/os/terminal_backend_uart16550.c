#include "common.h"
#include "crt.h"
#include "hal.h"
#include "terminal.h"

#define UART_CLOCK_RATE     ((uint32_t)115200)

enum {
  kUARTAddressCOM1                  = 0x3F8,
  kUARTAddressCOM2                  = 0x2F8,
  kUARTAddressCOM3                  = 0x3E8,
  kUARTAddressCOM4                  = 0x2E8,
};

enum {
  kUARTRegisterDTX                  = 0,
  kUARTRegisterDRX                  = 0,
  kUARTRegisterDLL                  = 0,
  kUARTRegisterDLH                  = 1,
  kUARTRegisterIER                  = 1,
  kUARTRegisterIIR                  = 2,
  kUARTRegisterFCR                  = 2,
  kUARTRegisterLCR                  = 3,
  kUARTRegisterMCR                  = 4,
  kUARTRegisterLSR                  = 5,
  kUARTRegisterMSR                  = 6,
  kUARTRegisterSCR                  = 7,
};

enum {
  kUARTLCRBits5D1S                  = 0x00,
  kUARTLCRBits6D1S                  = 0x01,
  kUARTLCRBits7D1S                  = 0x02,
  kUARTLCRBits8D1S                  = 0x03,
  kUARTLCRBits5D15S                 = 0x04,
  kUARTLCRBits6D2S                  = 0x05,
  kUARTLCRBits7D2S                  = 0x06,
  kUARTLCRBits8D2S                  = 0x07,
  kUARTLCRParityNone                = 0x00,
  kUARTLCRParityOdd                 = 0x08,
  kUARTLCRParityEven                = 0x18,
  kUARTLCRParityMark                = 0x28,
  kUARTLCRParitySpace               = 0x38,
  kUARTLCRDivisorLatch              = 0x80,
};

enum {
  kUARTLSRReceiverDataReady         = 0x01,
  kUARTLSREmptyTransmitter          = 0x20,
  kUARTLSRErrorMask                 = 0x8E,
};

static struct UART16550_Context {
  uint16_t port;
  uint32_t baud_rate;
  uint16_t cursor_position_x;
  uint16_t cursor_position_y;
} UART16550_Context;

static uint8_t UART16550_ReadRegister(uint16_t port, uint16_t offset) {
  return HAL_PortInByte(port + offset);
}

static void UART16550_WriteRegister(uint16_t port, uint16_t offset, uint8_t value) {
  HAL_PortOutByte(port + offset, value);
}

#if defined(BONUS_FUNCTION)
static bool UART16550_RecvByte(uint8_t* value) {
  UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterMSR);
  uint8_t status = 0;

  do {
    status = UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLSR);
    if (status & kUARTLSRErrorMask) {
      return false;
    }
  } while ((status & kUARTLSRReceiverDataReady) == 0);

  uint8_t received = UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterDRX);
  status = UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLSR);

  if (status & kUARTLSRErrorMask) {
    return false;
  }

  (*value) = received;
  return true;
}
#endif

static bool UART16550_SendByte(uint8_t value) {
  uint8_t status = 0;
  do {
    status = UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLSR);
    if (status & kUARTLSRErrorMask) {
      return false;
    }
  } while ((status & kUARTLSREmptyTransmitter) == 0);

  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterDTX, value);

  status = UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLSR);
  return (status & kUARTLSRErrorMask) == 0;
}

// Public Interface

static void UART16550_Initialize(TerminalBackend *tb) {
  UNUSED(tb);

  //
  // Initialize UART controller context structure.
  //
  UART16550_Context.port = kUARTAddressCOM1;
  UART16550_Context.baud_rate = 112500;

  //
  // Disable interrupts.
  //
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterLCR, 0);
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterIER, 0);
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterFCR, 0);

  //
  // Set baud rate.
  //
  uint32_t const divisor = (uint32_t)(UART_CLOCK_RATE / UART16550_Context.baud_rate);
  uint8_t const divisor_lo = (uint8_t)(divisor);
  uint8_t const divisor_hi = (uint8_t)(divisor >> 8);

  // Enable divisor latch.
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterLCR,
    UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLCR) | kUARTLCRDivisorLatch);

  // Set divisor value.
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterDLL, divisor_lo);
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterDLH, divisor_hi);

  // Disable divisor latch.
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterLCR,
    UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterLCR) & ~kUARTLCRDivisorLatch);

  // Configure data transfer.
  UART16550_WriteRegister(UART16550_Context.port, kUARTRegisterLCR, kUARTLCRBits8D1S | kUARTLCRParityNone);

  // Clear data buffer.
  (void)UART16550_ReadRegister(UART16550_Context.port, kUARTRegisterDRX);

  UART16550_SendByte(033);
  UART16550_SendByte('c');
  UART16550_SendByte(033);
  UART16550_SendByte('[');
  UART16550_SendByte('2');
  UART16550_SendByte('J');
}

static void UART16550_SetColor(TerminalBackend *tb, unsigned char fgColor, unsigned char bgColor, bool blink) {
  UNUSED(tb);
  UART16550_SendByte(033);
  UART16550_SendByte('[');
  UART16550_SendByte('0' + (blink ? 5 : 0));
  UART16550_SendByte(';');
  UART16550_SendByte('3');
  UART16550_SendByte('0' + (fgColor & 0x7));
  UART16550_SendByte(';');
  UART16550_SendByte('4');
  UART16550_SendByte('0' + (bgColor & 0x7));
  UART16550_SendByte('m');
}

static void UART16550_SetCursorPosition(TerminalBackend *tb, uint16_t x, uint16_t y) {
  UNUSED(tb);
  char tmp[16];
  snprintf(tmp, sizeof(tmp), "[%u;%uf", y, x);

  UART16550_SendByte(033);
  for (char* ptr = &tmp[0]; *ptr != '\0'; ++ptr) {
    UART16550_SendByte(*ptr);
  }

  UART16550_Context.cursor_position_x = x;
  UART16550_Context.cursor_position_y = y;
}

static void UART16550_ClearScreen(TerminalBackend *tb) {
  UNUSED(tb);
  UART16550_SendByte(033);
  UART16550_SendByte('[');
  UART16550_SendByte('2');
  UART16550_SendByte('J');
}
static void UART16550_PutCharacter(TerminalBackend *tb, uint32_t ch) {
  UNUSED(tb);
  UART16550_SendByte((uint8_t)ch);
}
static void UART16550_GetCursorPosition(TerminalBackend *tb, uint16_t *x, uint16_t *y) {
  UNUSED(tb);
  (*x) = UART16550_Context.cursor_position_x;
  (*y) = UART16550_Context.cursor_position_y;
}

static void UART16550_GetSize(TerminalBackend *tb, uint16_t *w, uint16_t *h) {
  UNUSED(tb);
  (*w) = 80;
  (*h) = 25;
}

static void UART16550_ScrollLine(TerminalBackend *tb) {
  UNUSED(tb);
  UART16550_SendByte('\n');
  UART16550_SendByte(033);
  UART16550_SendByte('E');
  UART16550_Context.cursor_position_y -= 1;
}

static const TerminalBackend UART16550_Functions = {
    .func_initialize = UART16550_Initialize,
    .func_set_cursor_position = UART16550_SetCursorPosition,
    .func_get_cursor_position = UART16550_GetCursorPosition,
    .func_clear_screen = UART16550_ClearScreen,
    .func_put_character = UART16550_PutCharacter,
    .func_get_size = UART16550_GetSize,
    .func_scroll_line = UART16550_ScrollLine,
    .func_set_color = UART16550_SetColor};

TerminalBackend *TerminalBackendUART16550(void) {
  // It's just a function table, so we declared it as const.
  // But we pass it as non-const to the functions.
  return (TerminalBackend *)&UART16550_Functions;
}
