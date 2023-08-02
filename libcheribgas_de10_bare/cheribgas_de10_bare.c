#include "cheribgas_de10_bare.h"
#include "uart16550.h"

#include <stdint.h>

void uart_init()
{
  uart16550_init();
}

// writes the character c, cast to an unsigned char
int putchar(int c)
{
  uart16550_putchar((uint8_t) c);
  return c;
}

// writes the string s and a trailing newline
int puts(const char *s)
{
  uint8_t* c = (uint8_t*)s;
  while (*c != '\0')
  {
    uart16550_putchar(*c);
    if (*c == '\n') uart16550_putchar('\r');
    c++;
  }
  uart16550_putchar('\n');
  uart16550_putchar('\r');
  return 0;
}

// reads the next character and returns it cast as an int
int getchar(void)
{
  return uart16550_getchar();
}

void swap(char* x, char* y)
{
  char t = *x; *x = *y; *y = t;
}

// Function to reverse a string
char* reverse(char str[], int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end) swap(&str[start++], &str[end--]);
  return str;
}

// give a string representation of an int as a string in the given base
char* itoa(int value, char* str, int base)
{
  int i = 0;
  int isNegative = 0;

  if (value == 0)
  {
      str[i++] = '0';
      str[i] = '\0';
      return str;
  }

  if (value < 0 && base == 10)
  {
      isNegative = 1;
      value = -value;
  }

  while (value != 0)
  {
      int rem = value % base;
      str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
      value = value / base;
  }

  if (isNegative) str[i++] = '-';

  str[i] = '\0';

  reverse(str, i);

  return str;
}

#define PRINTHEX_BUFF_SIZE 16
char* printhex(int value)
{
  static char buff[PRINTHEX_BUFF_SIZE+2];
  buff[0] = '0';
  buff[1] = 'x';
  itoa(value, &buff[2], 16);
}
