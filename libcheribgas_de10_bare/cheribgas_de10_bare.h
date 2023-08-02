#pragma once

void uart_init();
int putchar(int c);
int puts(const char *s);
int getchar(void);
void swap(char* x, char* y);
char* reverse(char str[], int length);
char* itoa(int value, char* str, int base);
char* printhex(int value);
