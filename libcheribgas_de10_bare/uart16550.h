#pragma once

#include <stdint.h>

void uart16550_putchar(uint8_t c);
int uart16550_getchar();
void uart16550_init();
