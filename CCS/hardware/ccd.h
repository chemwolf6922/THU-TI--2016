#ifndef __CCD_H
#define __CCD_H
//硬件占用: ADC1_CH16
//接口：SI:PA6		CLK:PA7		AO:AIN1_16
#include "sys.h"
#include "adc.h"
#include "delay.h"
#include "usart.h"
#include "HL_gio.h"

#define SI1 gioSetBit(gioPORTA, 6, 1)
#define SI0 gioSetBit(gioPORTA, 6, 0)
#define CLK1 gioSetBit(gioPORTA, 7, 1)
#define CLK0 gioSetBit(gioPORTA, 7, 0)




void ccd_init(void);

void ccd_get_data(void);		// 曝光时间us  一般取2ms左右
void ccd_binary_data(void);			//二值化  一般取2000左右
void ccd_find_bound(u8* l,u8* r);		//
void ccd_autobinary(void);
void ccd_autointegration(void);
void ccd_set_integrationtime(u32 time);    //设置曝光时间

u16 ccd_read_data(u8 num);
u8 ccd_read_binary(u8 num);

void ccd_test(void);
void ccd_binary_test(void);

#endif

