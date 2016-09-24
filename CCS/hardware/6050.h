#ifndef __6050_H
#define __6050_H
//硬件连接：sci3 Rx
//硬件占用：sci3 dma_ch1


#include "sys.h"
#include "sci3_dma.h"
#include "delay.h"


void mpu6050_init(void);
void mpu6050_resetzero(void);				//将当前值设置为角度零点
double mpu6050_get_angle(void);			//从上往下看从零点开始逆时针为正（无论正反） -180`180 deg  
double mpu6050_get_omega(void);    //从上往下看逆时针为正（无论正反） -2000·2000 deg/s
void mpu6050_test(void);

#endif


