/*
 * motor.c
 *
 *  Created on: 2016年3月20日
 *      Author: Feng
 */
#include "motor.h"

void motor_init(){
	hetInit();
	mibspiInit();
	adcInit();
	gioInit();
	motor1_nSLEEP_1;
	motor2_nSLEEP_1;
	motor3_nSLEEP_1;
	motor4_nSLEEP_1;
	motor1_MODE2_0;
	motor2_MODE2_0;
	motor3_MODE2_0;
	motor4_MODE2_0;
	motor1_MODE1_1;
	motor2_MODE1_1;
	motor3_MODE1_1;
	motor4_MODE1_1;
	motor1_PHASE_1;
	motor2_PHASE_1;
	motor3_PHASE_1;
	motor4_PHASE_1;
	pwmSetDuty(hetRAM1,pwm0,0);
	pwmSetDuty(hetRAM1,pwm1,0);
	pwmSetDuty(hetRAM1,pwm2,0);
	pwmSetDuty(hetRAM1,pwm3,0);


}

void motor1_pwm(int pwm){			//转向任需确认！！
	motor1_MODE1_1;
	if(pwm>99){
		pwm=99;
	}else if(pwm<-99){
		pwm=-99;
	}
	if(pwm>=0){
		motor1_PHASE_1;
		pwmSetDuty(hetRAM1,pwm0,pwm);
	}else{
		motor1_PHASE_0;
		pwmSetDuty(hetRAM1,pwm0,-pwm);
	}
}

void motor2_pwm(int pwm){
	motor2_MODE1_1;
	if(pwm>99){
		pwm=99;
	}else if(pwm<-99){
		pwm=-99;
	}
	if(pwm>=0){
		motor2_PHASE_1;
		pwmSetDuty(hetRAM1,pwm1,pwm);
	}else{
		motor2_PHASE_0;
		pwmSetDuty(hetRAM1,pwm1,-pwm);
	}
}

void motor3_pwm(int pwm){
	motor3_MODE1_1;
	if(pwm>99){
		pwm=99;
	}else if(pwm<-99){
		pwm=-99;
	}
	if(pwm>=0){
		motor3_PHASE_1;
		pwmSetDuty(hetRAM1,pwm2,pwm);
	}else{
		motor3_PHASE_0;
		pwmSetDuty(hetRAM1,pwm2,-pwm);
	}
}

void motor4_pwm(int pwm){
	motor4_MODE1_1;
	if(pwm>99){
		pwm=99;
	}else if(pwm<-99){
		pwm=-99;
	}
	if(pwm>=0){
		motor4_PHASE_1;
		pwmSetDuty(hetRAM1,pwm3,pwm);
	}else{
		motor4_PHASE_0;
		pwmSetDuty(hetRAM1,pwm3,-pwm);
	}
}

void motor1_break(void){
	pwmSetDuty(hetRAM1,pwm0,0);
	motor1_MODE1_1;
}

void motor2_break(void){
	pwmSetDuty(hetRAM1,pwm1,0);
	motor2_MODE1_1;
}

void motor3_break(void){
	pwmSetDuty(hetRAM1,pwm2,0);
	motor3_MODE1_1;
}

void motor4_break(void){
	pwmSetDuty(hetRAM1,pwm3,0);
	motor4_MODE1_1;
}

void motor1_sleep(bool a){
	if(a==Sleep){
		motor1_nSLEEP_0;
	}else{
		motor1_nSLEEP_1;
	}
}

void motor2_sleep(bool a){
	if(a==Sleep){
		motor2_nSLEEP_0;
	}else{
		motor2_nSLEEP_1;
	}
}

void motor3_sleep(bool a){
	if(a==Sleep){
		motor3_nSLEEP_0;
	}else{
		motor3_nSLEEP_1;
	}
}

void motor4_sleep(bool a){
	if(a==Sleep){
		motor4_nSLEEP_0;
	}else{
		motor4_nSLEEP_1;
	}
}

void motor_sleep(bool a){
	motor1_sleep(a);
	motor2_sleep(a);
	motor3_sleep(a);
	motor4_sleep(a);
}
