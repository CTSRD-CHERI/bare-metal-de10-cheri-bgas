#include "uart16550.h"

#include <stdint.h>

// uart address
extern volatile uint8_t uart16550[];

// some devices require a shifted register index
// (e.g. 32 bit registers instead of 8 bit registers)
#ifndef UART16550_REG_SHIFT
#define UART16550_REG_SHIFT 2
#endif

#ifndef UART16550_BAUD
#define UART16550_BAUD 115200
#endif

#ifndef UART16550_CLK_HZ
#define UART16550_CLK_HZ 50000000
#endif

// uart 16550 register indices
#define UART_REG_QUEUE (0 << UART16550_REG_SHIFT) // rx/tx fifo data
#define UART_REG_DLL   (0 << UART16550_REG_SHIFT) // divisor latch (LSB)
#define UART_REG_IER   (1 << UART16550_REG_SHIFT) // interrupt enable register
#define UART_REG_DLM   (1 << UART16550_REG_SHIFT) // divisor latch (MSB)
#define UART_REG_FCR   (2 << UART16550_REG_SHIFT) // fifo control register
#define UART_REG_LCR   (3 << UART16550_REG_SHIFT) // line control register
#define UART_REG_MCR   (4 << UART16550_REG_SHIFT) // modem control register
#define UART_REG_LSR   (5 << UART16550_REG_SHIFT) // line status register
#define UART_REG_MSR   (6 << UART16550_REG_SHIFT) // modem status register
#define UART_REG_SCR   (7 << UART16550_REG_SHIFT) // scratch register

#define UART_REG_STATUS_RX 0x01
#define UART_REG_STATUS_TX 0x20

void uart16550_putchar(uint8_t c)
{
  while (!(uart16550[UART_REG_LSR] & UART_REG_STATUS_TX));
  uart16550[UART_REG_QUEUE] = c;
}

int uart16550_getchar()
{
  if (uart16550[UART_REG_LSR] & UART_REG_STATUS_RX)
    return uart16550[UART_REG_QUEUE];
  return -1;
}

void uart16550_init()
{
    uint32_t divisor = UART16550_CLK_HZ / ( 16 * UART16550_BAUD );

    /* http://wiki.osdev.org/Serial_Ports */
    /* Disable all interrupts */
    uart16550[UART_REG_IER] = 0x00;
    /* Enable DLAB (set baud rate divisor) */
    uart16550[UART_REG_LCR] = 0x80;
    /* Set divisor (lo byte) baud */
    uart16550[UART_REG_DLL] = divisor & 0xff;
    /* Set divisor to (hi byte) baud */
    uart16550[UART_REG_DLM] = ( divisor >> 8 ) & 0xff;
    /* 8 bits, no parity, one stop bit */
    uart16550[UART_REG_LCR] = 0x03;
    /* Enable FIFO, clear them, with 14-byte threshold */
    uart16550[UART_REG_FCR] = 0xC7;
    //uart16550[UART_REG_FCR] = 0x01;
}
