/*
 * main.c
 * Modified by Pierrick Rauby < PierrickRauby - pierrick.rauby@gmail.com >
 * This code configures and samples the KX132 
 * TODO: Add the final sampling rate at which we sample with the accelerometer
*/


#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include "hw_types.h"
#include "am572x_pru_i2c_driver.h"
#include "am572x_pru_i2c.h" 
#include "KX132.h"

volatile register unsigned int __R30;
volatile register unsigned int __R31;

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT            ((uint32_t) 1 << 31)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux 
   device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 * Be sure to change the values in resource_table_0.h too.
 */
#define TO_ARM_HOST            18
#define FROM_ARM_HOST        19
#define CHAN_NAME            "rpmsg-pru"
#define CHAN_DESC            "Channel 30"
#define CHAN_PORT            30
/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK    4


#define NUMBER_SAMPLES 1024
#define PRU_SRAM  __far __attribute__((cregister("PRU_SHAREDMEM", far)))
PRU_SRAM   uint16_t pru_mem_array[NUMBER_SAMPLES];
/*#define PRU_DMEM1 __far __attribute__((cregister("PRU_DMEM_1_0",  near)))*/
/*PRU_DMEM0 volatile uint16_t pru_mem_array[NUMBER_SAMPLES];*/
/*PRU_DMEM1  uint16_t pru_mem_array[NUMBER_SAMPLES];*/


char payload[RPMSG_BUF_SIZE];
struct pru_rpmsg_transport transport;
uint16_t src, dst, len;
volatile uint8_t *status;
unsigned long sample;
volatile pruI2C *PRU_I2Cmain=&CT_I2C4;

uint8_t configure_KX132(uint16_t address);
uint8_t sample_data(uint16_t address);

uint8_t pru_i2c_test_function( uint8_t i2cDevice){
    /* Allow OCP master port access by the PRU so the PRU can read external 
       memories */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    // This line is different between AM335x and Am5729 
    CT_INTC.SICR_bit.STATUS_CLR_INDEX = FROM_ARM_HOST;
    /* Make sure the Linux drivers are ready for RPMsg communication */
    status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0,
        &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
    /* Create the RPMsg channel between the PRU and ARM user space using the 
       transport structure. */
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, 
          CHAN_PORT) != PRU_RPMSG_SUCCESS);
    while (1) {
      /* Check bit 30 of register R31 to see if the ARM has kicked us */
      if (__R31 & HOST_INT) {
        /* Clear the event status */
        CT_INTC.SICR_bit.STATUS_CLR_INDEX = FROM_ARM_HOST;
        while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) 
            == PRU_RPMSG_SUCCESS) {
          uint8_t address=0x1F;
          pru_i2c_driver_software_reset(1);
          sample=configure_KX132(address);
          sample=sample_data(address);
          memcpy(payload, "\0\0\0\0\0\0\0\0\0\0\0", 11);
          ltoa((long)sample, payload);
          len = strlen(payload) + 1;
          /* send data to user space with RPmsg */
          pru_rpmsg_send(&transport, dst, src, payload, 11);
    }
      }
    }
}

uint8_t configure_KX132(uint16_t address){
  pru_i2c_driver_software_reset(1);
  // check WHO_AM_I
  /*uint16_t reg=0x13;*/
  uint16_t received=0;
  uint16_t to_send;
  pru_i2c_driver_receive_byte(address,KX132_WHO_AM_I,0, &received);
  if(received!=WHO_AM_I){return 1;}
  // Put Sensor in Stanby Mode
  to_send=0x00; // send 0x00 to CNTL1
  if(pru_i2c_driver_transmit_byte(address,KX132_CNTL1,1,&to_send)){return 1;}
   /*Set Output Data Rate to 50Hz*/
  to_send=0x0b; // send 0x06 to ODCNTL
  /*to_send=0x06; // send 0x06 to ODCNTL*/
  if(pru_i2c_driver_transmit_byte(address,KX132_ODCNTL,1,&to_send)){return 1;}
   /*Set the sensor in operating mode*/
  to_send=0xE0; // send 0xE0 to CNTL1
  if(pru_i2c_driver_transmit_byte(address,KX132_CNTL1,1,&to_send)){return 1;}
  return 0;
}
uint8_t sample_data(uint16_t address){
  /*long data_size=50;*/
  uint16_t received=0;
  /*uint16_t data_received[16]; //needs to be an even number for 16bit res*/
  uint16_t i;
  uint16_t low_high[2];

  for(i=0;i<512;i++){
    do{ // wait for new data to be ready in register
      pru_i2c_driver_receive_byte(address,KX132_INS2,0,&received);
    }while(!(received&0x10));
    pru_i2c_driver_receive_bytes(address,KX132_XOUT_L,2,pru_mem_array+i);

    /*pru_mem_array[i]=NUMBER_SAMPLES-1; // clear the memory */
    /*pru_mem_array[i]=0xffff; // clear the memory */
  }
  /*pru_i2c_driver_receive_byte(address,KX132_WHO_AM_I,0,&pru_mem_array[257]);*/
  /*pru_mem_array[1]=0xff;*/
  return pru_mem_array[3];

}
void main(void) {

  pru_i2c_test_function(1);
}

#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
  "/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
  "/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
  "\0\0";
