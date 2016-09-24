/*
 * a9800.h
 *
 *  Created on: 2016Äê5ÔÂ22ÈÕ
 *      Author: Feng
 */

#ifndef HARDWARE_A9800_H_
#define HARDWARE_A9800_H_

#include "sys.h"
#include "HL_mibspi.h"
#include "HL_gio.h"
#include "delay.h"
#include "usart.h"

#define SPI_A9800_CS_LOW()       gioSetBit(gioPORTB,2,0)
#define SPI_A9800_CS_HIGH()      gioSetBit(gioPORTB,2,1)


void A9800_Init(void);
void A9800_PowerUp(void);
void A9800_getmotion(s16* motion);
float A9800_getSpeed(void);
s16 A9800_getMotion(void);
u8 A9800RW(u8 data);
u8 A9800ReadReg(u8 addr);
void A9800WriteReg(u8 addr, u8 data);
void A9800UploadFiremware(u8 *firmwareAddr, u16 firmwareLength);


#define SPI_A9800_REG_Product_ID                           0x00
#define SPI_A9800_REG_Revision_ID                          0x01
#define SPI_A9800_REG_Motion                               0x02
#define SPI_A9800_REG_Delta_X_L                            0x03
#define SPI_A9800_REG_Delta_X_H                            0x04
#define SPI_A9800_REG_Delta_Y_L                            0x05
#define SPI_A9800_REG_Delta_Y_H                            0x06
#define SPI_A9800_REG_SQUAL                                0x07
#define SPI_A9800_REG_Pixel_Sum                            0x08
#define SPI_A9800_REG_Maximum_Pixel                        0x09
#define SPI_A9800_REG_Minimum_Pixel                        0x0a
#define SPI_A9800_REG_Shutter_Lower                        0x0b
#define SPI_A9800_REG_Shutter_Upper                        0x0c
#define SPI_A9800_REG_Frame_Period_Lower                   0x0d
#define SPI_A9800_REG_Frame_Period_Upper                   0x0e
#define SPI_A9800_REG_Configuration_I                      0x0f
#define SPI_A9800_REG_Configuration_II                     0x10
#define SPI_A9800_REG_Frame_Capture                        0x12
#define SPI_A9800_REG_SROM_Enable                          0x13
#define SPI_A9800_REG_Run_Downshift                        0x14
#define SPI_A9800_REG_Rest1_Rate                           0x15
#define SPI_A9800_REG_Rest1_Downshift                      0x16
#define SPI_A9800_REG_Rest2_Rate                           0x17
#define SPI_A9800_REG_Rest2_Downshift                      0x18
#define SPI_A9800_REG_Rest3_Rate                           0x19
#define SPI_A9800_REG_Frame_Period_Max_Bound_Lower         0x1a
#define SPI_A9800_REG_Frame_Period_Max_Bound_Upper         0x1b
#define SPI_A9800_REG_Frame_Period_Min_Bound_Lower         0x1c
#define SPI_A9800_REG_Frame_Period_Min_Bound_Upper         0x1d
#define SPI_A9800_REG_Shutter_Max_Bound_Lower              0x1e
#define SPI_A9800_REG_Shutter_Max_Bound_Upper              0x1f
#define SPI_A9800_REG_LASER_CTRL0                          0x20
#define SPI_A9800_REG_Observation                          0x24
#define SPI_A9800_REG_Data_Out_Lower                       0x25
#define SPI_A9800_REG_Data_Out_Upper                       0x26
#define SPI_A9800_REG_SROM_ID                              0x2a
#define SPI_A9800_REG_Lift_Detection_Thr                   0x2e
#define SPI_A9800_REG_Configuration_V                      0x2f
#define SPI_A9800_REG_Configuration_IV                     0x39
#define SPI_A9800_REG_Power_Up_Reset                       0x3a
#define SPI_A9800_REG_Shutdown                             0x3b
#define SPI_A9800_REG_Inverse_Product_ID                   0x3f
#define SPI_A9800_REG_Motion_Burst                         0x50
#define SPI_A9800_REG_SROM_Load_Burst                      0x62
#define SPI_A9800_REG_Pixel_Burst                          0x64

#endif /* HARDWARE_A9800_H_ */
