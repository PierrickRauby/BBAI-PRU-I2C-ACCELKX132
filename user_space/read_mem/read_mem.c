
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PRU_ADDR        0x4B200000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU0_DRAM       0x00000         // Offset to DRAM
#define PRU1_DRAM       0x02000
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

unsigned int    *pru0DRAM_32int_ptr;        // Points to the start of local DRAM
unsigned int    *pru1DRAM_32int_ptr;        // Points to the start of local DRAM
unsigned int    *prusharedMem_32int_ptr;    // Points to the start of the shared memory

int main(int argc, char *argv[]) {
  unsigned int    *pru;       // Points to start of PRU memory.
  int fd;
  printf("Reading shared memory");
  fd = open ("/dev/mem", O_SYNC); // only need to read the memory
  if (fd == -1) {
    printf ("ERROR: could not open /dev/mem.\n\n");
    return 1;
  }
  pru = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
  if (pru == MAP_FAILED) {
    printf ("ERROR: could not map memory.\n\n");
    return 1;
  }
  close(fd);
  printf ("Using /dev/mem.\n");
  prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4; // Points to start of shared memory

  printf("%02x ", prusharedMem_32int_ptr[0]);
  /*
    off_t offset = strtoul(argv[1], NULL, 0);
    size_t len = strtoul(argv[2], NULL, 0);

    // Truncate offset to a multiple of the page size, or mmap will fail.
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    off_t page_base = (offset / pagesize) * pagesize;
    off_t page_offset = offset - page_base;

    int fd = open("/dev/mem", O_SYNC);
    unsigned char *mem = mmap(NULL, page_offset + len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, page_base);
    if (mem == MAP_FAILED) {
        perror("Can't map memory");
        return -1;
    }

    size_t i;
    for (i = 0; i < len; ++i)
        printf("%02x ", (int)mem[page_offset + i]);
*/
    return 0;
}
