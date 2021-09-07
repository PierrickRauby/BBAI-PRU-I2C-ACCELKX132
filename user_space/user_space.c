/*
 * user_space.pru1_1.c
 * Modified by Pierrick Rauby < PierrickRauby - pierrick.rauby@gmail.com >
 * Based on the examples distributed by TI
 * Cedistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the
 *      distribution.
 *
 *    * Neither the name of Texas Instruments Incorporated nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 * To compile use: make 
*/


#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PRU_SHAREDMEM        0x4B210000      // Start of PRU memory Page 184 am335x TRM
#define MAX_BUFFER_SIZE        512
char readBuf[MAX_BUFFER_SIZE];
#define DEVICE_NAME        "/dev/rpmsg_pru30"


int readmemory() {
  off_t offset = 0x4B210000;
  size_t len = 2048;
  // Truncate offset to a multiple of the page size, or mmap will fail.
  size_t pagesize = sysconf(_SC_PAGE_SIZE);
  off_t page_base = (PRU_SHAREDMEM / pagesize) * pagesize;
  off_t page_offset = PRU_SHAREDMEM - page_base;
  int output_file = open("./acquisition.csv", O_RDWR);
  if (output_file == NULL) {
    printf("Error, cannot open ");
    exit(1);
  }
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
  for (i = 0; i < len-2; i+=2){
    lsb=mem[page_offset + i];
    msb=mem[page_offset + i+1];
    combined = (msb << 8 ) | (lsb & 0xff);
    result=0.0002441407513657033*(float)combined;
    printf("  result: %0.6f\n",result);
  }
  return 0;
}


int main(void)
{
    /* Trigger the Acquisition by sending information to the PRU*/
    struct pollfd pollfds[1];
    /*int i;*/
    long result = 0;
    /* Open the rpmsg_pru character device file */
    pollfds[0].fd = open(DEVICE_NAME, O_RDWR);
    /*
     * If the RPMsg channel doesn't exist yet the character device
     * won't either.
     * Make sure the PRU firmware is loaded and that the rpmsg_pru
     * module is inserted.
     */
    if (pollfds[0].fd < 0) {
        printf("Failed to open %s\n", DEVICE_NAME);
        return -1;
    }
    /* The RPMsg channel exists and the character device is opened */
    printf("Opened %s, starting acquisition \n", DEVICE_NAME);
    /* In this example only the adress to read with the PRU is sent*/
    result = write(pollfds[0].fd, "Place Add", 10);
    if (result > 0)
      printf("Waiting for acquisition to complete on: %s\n", DEVICE_NAME);
    /* Poll until we receive a message from the PRU and then print it */
    result = read(pollfds[0].fd, readBuf, 11);
    char str[1];
    sprintf(str, "%s", readBuf);
    if (result > 0){
      if(!strcmp(str,"0")){
        printf("Acquisition sucess, retrieving results");
      }else{
        printf("Acquisition error\n");
      }
    }
    /* Close the rpmsg_pru character device file */
    close(pollfds[0].fd);

    /* Read results from memory and save them in a csv*/
    readmemory();
    return 0;
}
