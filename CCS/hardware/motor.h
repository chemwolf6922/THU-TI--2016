/*
 * motor.h
 *
 *  Created on: 2016年3月20日
 *      Author: Feng
 *      motor1:左前
 *      motor2:左后
 *      motor3:右前
 *      motor4:右后
 *
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"
#include "HL_het.h"
#include "HL_mibspi.h"
#include "HL_adc.h"
#include "HL_gio.h"

#define motor1_nFAULT gioGetBit(gioPORTA,1)
#define motor2_nFAULT adcGetEVTPin(adcREG2)
#define motor3_nFAULT (hetREG1->DIN>>30)&1U
#define motor4_nFAULT (hetREG1->DIN>>23)&1U

#define motor1_MODE1_1 gioSetBit(gioPORTA,2,1)
#define motor1_MODE1_0 gioSetBit(gioPORTA,2,0)
#define motor2_MODE1_1 gioSetBit(gioPORTB,0,1)
#define motor2_MODE1_0 gioSetBit(gioPORTB,0,0)
#define motor3_MODE1_1 hetREG1->DSET=(uint32)1U<<16
#define motor3_MODE1_0 hetREG1->DCLR=(uint32)1U<<16
#define motor4_MODE1_1 hetREG1->DSET=(uint32)1U<<8
#define motor4_MODE1_0 hetREG1->DCLR=(uint32)1U<<8

#define motor1_MODE2_1 gioSetBit(gioPORTA,0,1)
#define motor1_MODE2_0 gioSetBit(gioPORTA,0,0)
#define motor2_MODE2_1 hetREG1->DSET=(uint32)1U<<31
#define motor2_MODE2_0 hetREG1->DCLR=(uint32)1U<<31
#define motor3_MODE2_1 hetREG1->DSET=(uint32)1U<<14
#define motor3_MODE2_0 hetREG1->DCLR=(uint32)1U<<14
#define motor4_MODE2_1 mibspiPORT5->DSET=(uint32)1U<<8
#define motor4_MODE2_0 mibspiPORT5->DCLR=(uint32)1U<<8

#define motor1_nSLEEP_1 hetREG1->DSET=(uint32)1U<<12
#define motor1_nSLEEP_0 hetREG1->DCLR=(uint32)1U<<12
#define motor2_nSLEEP_1 hetREG1->DSET=(uint32)1U<<11
#define motor2_nSLEEP_0 hetREG1->DCLR=(uint32)1U<<11
#define motor3_nSLEEP_1 hetREG1->DSET=(uint32)1U<<25
#define motor3_nSLEEP_0 hetREG1->DCLR=(uint32)1U<<25
#define motor4_nSLEEP_1 mibspiPORT1->DSET=(uint32)1U<<25
#define motor4_nSLEEP_0 mibspiPORT1->DCLR=(uint32)1U<<25

#define motor1_PHASE_1 gioSetBit(gioPORTA,5,1)
#define motor1_PHASE_0 gioSetBit(gioPORTA,5,0)
#define motor2_PHASE_1 gioSetBit(gioPORTB,1,1)
#define motor2_PHASE_0 gioSetBit(gioPORTB,1,0)
#define motor3_PHASE_1 hetREG1->DSET=(uint32)1U<<22
#define motor3_PHASE_0 hetREG1->DCLR=(uint32)1U<<22
#define motor4_PHASE_1 hetREG1->DSET=(uint32)1U<<1
#define motor4_PHASE_0 hetREG1->DCLR=(uint32)1U<<1

#define Sleep 0U
#define Awake 1U

void motor_init(void);
void motor1_pwm(int pwm);
void motor2_pwm(int pwm);
void motor3_pwm(int pwm);
void motor4_pwm(int pwm);
void motor1_break(void);
void motor2_break(void);
void motor3_break(void);
void motor4_break(void);
void motor1_sleep(bool a);		//sleep a=0
void motor2_sleep(bool a);
void motor3_sleep(bool a);
void motor4_sleep(bool a);
void motor_sleep(bool a);

#endif /* HARDWARE_MOTOR_H_ */
