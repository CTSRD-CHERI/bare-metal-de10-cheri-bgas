#include <cheribgas_de10_bare.h>
#include <stdint.h>

void memtest (void* start_addr, void* end_addr)
{
  volatile uint8_t* ptr = start_addr;
  int err = 0;
  while (ptr <= (uint8_t*)end_addr)
  {
    volatile uint8_t*   ptr8 = ptr;
    volatile uint16_t* ptr16 = (uint16_t*) ptr;
    volatile uint32_t* ptr32 = (uint32_t*) ptr;
    volatile uint64_t* ptr64 = (uint64_t*) ptr;
    *ptr64 = 0x7766554433221100;
    if (ptr64[0] != 0x7766554433221100) err++;
    if (ptr32[0] != 0x33221100) err++;
    if (ptr32[1] != 0x77665544) err++;
    if (ptr16[0] != 0x1100) err++;
    if (ptr16[1] != 0x3322) err++;
    if (ptr16[2] != 0x5544) err++;
    if (ptr16[3] != 0x7766) err++;
    if (ptr8[0] != 0x00) err++;
    if (ptr8[1] != 0x11) err++;
    if (ptr8[2] != 0x22) err++;
    if (ptr8[3] != 0x33) err++;
    if (ptr8[4] != 0x44) err++;
    if (ptr8[5] != 0x55) err++;
    if (ptr8[6] != 0x66) err++;
    if (ptr8[7] != 0x77) err++;

    if (err > 0)
    {
      puts("ERROR on address:");
      puts(printhex((long)ptr));
      while(1);
    }
    ptr++;
  }
}

int main()
{
  uart_init();
  puts("test cached");
  memtest((void*)0xc0080000, (void*)0xc0080100);
  puts("test cached done");
  puts("test uncached");
  memtest((void*)0x80080000, (void*)0x80080100);
  puts("test uncached done");
}
