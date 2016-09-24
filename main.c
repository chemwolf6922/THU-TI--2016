#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
//#include "6050.h"
#include "ccd.h"
#include "remote.h"
#include "strategy.h"
#include "counter.h"
#include "current.h"
#include "remote.h"
#include "a9800.h"
#include "HL_mibspi.h"

void system_init(void){
	esmInit();
	usart_init(115200);
	delay_init();
	motor_init();
//	motor_sleep(Sleep);
	irdistence_init();
	remote_init();
	mpu6050_init();
	ccd_init();
	A9800_Init();
	A9800_PowerUp();
	strategy_init();
	counter_init();
//  _ENABLE_FIQ();
}



void main(void){
	system_init();
	motor1_break();
	motor2_break();
	motor3_break();
	motor4_break();
	delay_ms(200);
	while(gioGetBit(gioPORTB,4));		//never move before this!!!!!
	gioSetBit(gioPORTB,6,1);
	while(1){
		while(read_sys_status()==0);
		set_sys_status(0);
		car_tick();
	}
}
