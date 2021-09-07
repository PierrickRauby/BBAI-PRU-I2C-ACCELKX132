#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define PRU_SHAREDMEM        0x4B210000      // Start of PRU memory Page 184 am335x TRM




int main() {

  off_t offset = 0x4B210000;
  size_t len = 2048;

  // Truncate offset to a multiple of the page size, or mmap will fail.
  size_t pagesize = sysconf(_SC_PAGE_SIZE);
  off_t page_base = (PRU_SHAREDMEM / pagesize) * pagesize;
  off_t page_offset = PRU_SHAREDMEM - page_base;

  int fd = open("/dev/mem", O_SYNC);
  unsigned char *mem = mmap(NULL, page_offset + len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, page_base);
  if (mem == MAP_FAILED) {
    perror("Can't map memory");
    return -1;
  }

  size_t i;
  int16_t combined;
  float result;
  uint8_t msb,lsb;
  for (i = 0; i < len; i+=2){
    lsb=mem[page_offset + i];
    msb=mem[page_offset + i+1];
    combined = (msb << 8 ) | (lsb & 0xff);
    result=0.0002441407513657033*(float)combined;
    printf("  result: %0.6f\n",result);
  }
  /*printf("%02x ", mem[page_offset + i]);}*/
  printf("%02x ",combined);
  return 0;
}
