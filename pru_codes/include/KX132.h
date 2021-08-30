/*
 * KX132.h
 * Author Pierrick Rauby : pierrick.rauby@gmail.com
 * Created on August24 2021
 * Adapted from cpp version  SparkFun_Qwiic_KX132.h 
 *  Initial authorElias Santistevan @ SparkFun Electronics
 * Original Creation Date: March 2021
 * Distributed as-is; no warranty is given.
 */

#ifndef __KX132_H__
#define __KX132_H__

#define KX132_DEFAULT_ADDRESS 0x1F
#define KX132_ALT_ADDRESS 0x1E

#define WHO_AM_I  0x3D
#define TOTAL_ACCEL_DATA_16BIT 6 
#define TOTAL_ACCEL_DATA_8BIT 3 
#define MAX_BUFFER_LENGTH 32 

#define XLSB 0
#define XMSB 1
#define YLSB 2
#define YMSB 3
#define ZLSB 4
#define ZMSB 5

#define DEFAULT_SETTINGS 0xC0
#define INT_SETTINGS 0xE0
#define SOFT_INT_SETTINGS 0xE1
#define BUFFER_SETTINGS 0xE2
#define TILT_SETTINGS 0xE3

#define COTR_DEF_STATE 0x55
#define COTR_POS_STATE 0xAA

#define BUFFER_16BIT_SAMPLES 0x01
#define BUFFER_8BIT_SAMPLES 0x00
#define BUFFER_MODE_FIFO 0x00
#define BUFFER_MODE_STREAM 0x01
#define BUFFER_MODE_TRIGGER 0x02

#define HI_TILT_POSITION 0x01
#define HI_WAKE_UP 0x02
#define HI_TAP_DOUBLE_TAP 0x04
#define HI_BACK_TO_SLEEP 0x08
#define HI_DATA_READY 0x10
#define HI_WATERMARK 0x20
#define HI_BUFFER_FULL 0x40
#define HI_FREEFALL 0x80


// Accelerometer registers address
#define KX132_MAN_ID 0x00 //Retuns "KION" in ASCII
#define KX132_PART_ID 0x01 //Who am I + Silicon specific ID
#define KX132_XADP_L 0x02
#define KX132_XADP_H 0x03
#define KX132_YADP_L 0x04
#define KX132_YADP_H 0x05
#define KX132_ZADP_L 0x06
#define KX132_ZADP_H 0x07
#define KX132_XOUT_L 0x08
#define KX132_XOUT_H 0x09
#define KX132_YOUT_L 0x0A
#define KX132_YOUT_H 0x0B
#define KX132_ZOUT_L 0x0C
#define KX132_ZOUT_H 0x0D
 // 0x0E - 0x11 Reserved
#define KX132_COTR 0x12 //Command Test Register
#define KX132_WHO_AM_I 0x13
#define KXI32_TSCP 0x14 //Tilt Registers
#define KX132_TSPP 0x15
#define KX132_INS1 0x16 //Interrupt Registers
#define KX132_INS2 0x17
#define KX132_INS3 0x18
#define KX132_STATUS_REG 0x19
#define KX132_INT_REL 0x1A
#define KX132_CNTL1 0x1B //Control Registers
#define KX132_CNTL2 0x1C
#define KX132_CNTL3 0x1D
#define KX132_CNTL4 0x1E
#define KX132_CNTL5 0x1F
#define KX132_CNTL6 0x20
#define KX132_ODCNTL 0x21
#define KX132_INC1 0x22 //Controls settings for INT1
#define KX132_INC2 0x23
#define KX132_INC3 0x24
#define KX132_INC4 0x25
#define KX132_INC5 0x26
#define KX132_INC6 0x27
// 0x28 Reserved
#define KX132_TILT_TIMER 0x29
#define KX132_TDTRC 0x2A //Tap Control Regs
#define KX132_TDTC 0x2B
#define KX132_TTH 0x2C
#define KX132_TTL 0x2D
#define KX132_FTD 0x2E
#define KX132_STD 0x2F
#define KX132_TLT 0x30
#define KX132_TWS 0x31
#define KX132_FFTH 0x32
#define KX132_FFC 0x33
#define KX132_FFCNTL 0x34
// 0x35 - 0x36 Reserved
#define KX132_TILT_ANGLE_LL 0x37
#define KX132_TILT_ANGLE_HL 0x38
#define KX132_HYST_SET 0x39
#define KX132_LP_CNTL1 0x3A
#define KX132_LP_CNT2 0x3B
// 0x3C - 0x48 Reserved
#define KX132_WUFTH 0x49
#define KX132_BTSWUFTH 0x4A
#define KX132_BTSTH 0x4B
#define KX132_BTSC 0x4C
#define KX132_WUFC 0x4D
// 0x4E - 0x5C Reserved
#define KX132_SELF_TEST  0x5D
#define KX132_BUF_CNTL1 0x5E
#define KX132_BUF_CNTL2 0x5F
#define KX132_BUF_STATUS_1 0x60
#define KX132_BUF_STATUS_2 0x61
#define KX132_BUF_CLEAR 0x62
#define KX132_BUF_READ 0x63
#define KX132_ADP_CNTL1 0x64
#define KX132_ADP_CNTL2 0x65
#define KX132_ADP_CNTL3 0x66
#define KX132_ADP_CNTL4 0x67
#define KX132_ADP_CNTL5 0x68
#define KX132_ADP_CNTL6 0x69
#define KX132_ADP_CNTL7 0x6A
#define KX132_ADP_CNTL8 0x6B
#define KX132_ADP_CNTL9 0x6C
#define KX132_ADP_CNTL10 0x6D
#define KX132_ADP_CNTL11 0x6E
#define KX132_ADP_CNTL12 0x6F
#define KX132_ADP_CNTL13 0x70
#define KX132_ADP_CNTL14 0x71
#define KX132_ADP_CNTL15 0x72
#define KX132_ADP_CNTL16 0x73
#define KX132_ADP_CNTL17 0x74
#define KX132_ADP_CNTL18 0x75
#define KX132_ADP_CNTL19 0x76
  //Reserved 0x77 - 0x7F

struct outputData { 
  float xData;
  float yData;
  float zData;
};

struct rawOutputData {
  int16_t xData;
  int16_t yData;
  int16_t zData;
};

typedef enum {
  KX132_SUCCESS = 0x00,
  KX132_GENERAL_ERROR,
  KX132_I2C_ERROR,
} KX132_STATUS_t;



#endif
