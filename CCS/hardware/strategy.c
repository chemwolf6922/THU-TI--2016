/*
 * strategy.c
 *
 *  Created on: 2016年3月22日
 *      Author: Feng
 */
#include "strategy.h"
///////////////////////////////////////////////////////

#ifndef INSANE
#define k1 -5
#define k2 -5.0
#define k3 40.0
#else
#define k1 -7
#define k2 -5.0
#define k3 40.0
#endif
#define track_line 65
#define ti 10
#define speed_lim 50
#define bound_not_found ((temp_left==63)&&(temp_right==64))

#define NORMAL 0
#define BREAK 1
#define TURNING 2
#define BACKOFF 3

#define TURN_LEFT 0
#define TURN_RIGHT 1

#define UNBREAK 0

#define IR_F_WARNING (ir_data.frount<20)
#define IR_FL_WARNING (ir_data.fleft<15)
#define IR_FR_WARNING (ir_data.fright<15)
#define IR_B_WARNING (ir_data.back<15)
#define IR_BL_WARNING (ir_data.bleft<15)
#define IR_BR_WARNING (ir_data.bright<15)
#define IR_L_WARNING (ir_data.left<15)
#define IR_R_WARNING (ir_data.right<15)

#define selftest_NULL 0
#define selftest_FL 1
#define selftest_FR 2
#define selftest_BL 3
#define selftest_BR 4
///////////////////////////////////////////////////////
int speed_left = 0;
int speed_right = 0;
int max_speed = speed_lim;
u8 previous_middle=65;
u8 black_middle=65;
u8 temp_left = 0;
u8 temp_right = 0;
int delta_speed=0;
int delta_i[ti]={0};		//历史值
int sum_ti=0;				//积分值



irData_t ir_data;
remoteData_t remote;
selftestData_t selftest;
currentData_t current;
motorStatus_t motorstatus;
reportData_t reportdata;

u8 car_status = NORMAL;
u8 turn_status = TURN_LEFT;
u8 speed_ctrl = UNBREAK;
u8 break_time = 0;
u8 speed_ctrl_time = 0;


float speed = 0.0;
int speedcps = 0;
int break_delay=25;

////////////////////////////////////////////////////////
void ir_speed(void);
void ir_turn(void);
void ccd_move(void);
void car_move(void);
void car_turn_left(void);
void car_turn_right(void);
void car_backoff(void);
void car_break(void);
void ccd_clear(void);
void car_stop(void);
void car_avoid(void);
void car_selftest(void);
void car_startselftest(void);
///////////////////////////////////////////////////////
void strategy_init(void){
	delay_init();
	rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK1);		//100Hz
	remote.speedleft = 0;
	remote.speedright = 0;
	selftest.status = false;
	selftest.BLtime = 0;
	selftest.BRtime = 0;
	selftest.FLtime = 0;
	selftest.FRtime = 0;
	selftest.time = 0;
	selftest.warning = 0;
	reportdata.error = 0;
	car_startselftest();
	mpu6050_resetzero();

}
////////////////////////////////////////////////////////
int testint = 0;
int lastfleft = 0;
int lastfright = 0;
///////////////////////////////////////////////////////
void car_tick(void){

#define INSANE
//#define TEST
//#define TEST2
#ifdef TEST
//	ccd_binary_test();
//	irdistence_test();
//	irdistence_test_data();

	testint = (int)mpu6050_get_angle();
	if(abs(testint)>2){
		if(testint<0&&testint>-30)testint=-30;
		if(testint>0&&testint<30)testint=30;
		motor1_pwm(testint);
		motor2_pwm(testint);
		motor3_pwm(-testint);
		motor4_pwm(-testint);
	}else{
		car_stop();
	}


#else


//get information
	remote_get_data(&remote);
	current_get_data(&current);
	irdistence_get_dis(&ir_data);
	if(remote.opticalflow) speed = A9800_getSpeed();
	else speed = counter_get_avg();
	speedcps = (int)(100.0*speed);
//report infomation
	reportdata = remote_get_reportData(&ir_data,&current,&motorstatus);
	reportdata.speed = speed;
	reportdata.error = esmError();
	remote_report(&reportdata);
//manual
	if(remote.clearErrorPin){
		if(!esmREG->EPSR)esmREG->EKR = 0x5;
		while(!esmREG->EPSR);
		gioSetBit(gioPORTB,7,0);
	}
	if(remote.manual == MANUAL){
		car_status = NORMAL;
		break_time = 0;
		if(remote.selftest) car_startselftest();
		car_selftest();
		if(remote.status&&!selftest.status){
			car_avoid();
		}else if(!selftest.status){
			car_stop();
		}
	}
//auto
#ifdef TEST2
	else{

		if((ir_data.frount>60)&&(abs(ir_data.fleft-ir_data.fright)<10)){
			testint = (int)(speed*15.0);
			max_speed = 100;
		}else{
			if(testint!=0){
				testint--;
				max_speed = 0;
			}else{
				max_speed = 50;
			}
		}
		if((ir_data.frount<25)&&(abs(ir_data.fleft-ir_data.fright)<5)){
			delta_speed = (int)((float)(ir_data.left-ir_data.right)*3.5);
		}else{
			delta_speed = (int)((float)(ir_data.fleft-ir_data.fright)*3.5);
		}

		car_move();


#else
	else{
		ccd_find_bound(&temp_left,&temp_right);
		black_middle = (temp_left+temp_right)/2;
	#ifndef INSANE
		break_delay = (int)(counter_get_avg()*20);
	#else
//optimize speed by changing the delay time
		break_delay = (int)(counter_get_avg()*remote.delaytime);
	#endif
		ir_speed();

			if((car_status==NORMAL)&&!bound_not_found){
				ccd_move();
//optimize speed by changing the speed limit condition
				if((speedcps>(int)((float)ir_data.frount*2.0))&&(ir_data.frount<remote.speedlimitcondition)){
					car_stop();
				}else{
					car_move();
				}
			}else if((((ir_data.fleft-lastfleft)>20)||((ir_data.fright-lastfright)>20))&&(car_status==NORMAL)){
				car_status = BREAK;
				break_time = break_delay;
			}else if((car_status==NORMAL)&&(ir_data.frount<20)&&(speedcps>100)){
				car_status = BREAK;
				break_time = break_delay;
			}else if((car_status==NORMAL)&&bound_not_found){
				car_status = BREAK;
				break_time = break_delay;
			}

			if((car_status==BREAK)&&(break_time!=0)){
				car_break();
				break_time--;
			}else if((car_status==BREAK)&&(break_time==0)){
				if(bound_not_found){
					if(ir_data.frount<=9){
						car_backoff();
						car_status=BACKOFF;
					}else{
						ir_turn();
						car_status=TURNING;
					}
				}else if(!bound_not_found){
					car_status=NORMAL;
					ccd_clear();
					ccd_move();
					car_move();
				}
			}

			if((car_status==TURNING)&&bound_not_found){
				ir_turn();
			}else if((car_status==TURNING)&&!bound_not_found){
				car_status=NORMAL;
	 			ccd_clear();
				ccd_move();
				car_move();
			}

			if(car_status==BACKOFF){
				if(ir_data.frount<=10){
					car_backoff();
				}else{
					car_status=BREAK;
					break_time=5;
					car_break();
				}
			}

#endif
	}

	lastfleft = ir_data.fleft;
	lastfright = ir_data.fright;

#endif


}
///////////////////////////////////////////////////////
void ir_speed(void){
	int temp[2]={0};
	temp[0]=(int)(1.25*(float)ir_data.frount);

	temp[1]=100;
#ifndef INSANE
	if(abs(ir_data.fleft-ir_data.fright)>15){
		temp[1]=100-2*abs(ir_data.fleft-ir_data.fright);
	}
#else
	if(abs(ir_data.fleft-ir_data.fright)>20){
		temp[1]=100-2*abs(ir_data.fleft-ir_data.fright);
	}
#endif

	if(temp[0]<=temp[1])max_speed=temp[0];
	else max_speed=temp[1];
#ifndef INSANE
	if(max_speed<50){
		max_speed=50;
	}
#else
//optimize speed by setting the minimum speed limit
	if(max_speed<remote.minimunspeedlimit){
		max_speed=remote.minimunspeedlimit;
	}
#endif
}

void ir_turn(void){
	if(car_status==BREAK){
		if((ir_data.right+ir_data.fright)>=(ir_data.left+ir_data.fleft))turn_status=TURN_RIGHT;
		else if((ir_data.right+ir_data.fright)<(ir_data.left+ir_data.fleft))turn_status=TURN_LEFT;
	}else{
		if(turn_status==TURN_LEFT)car_turn_left();
		else car_turn_right();
	}

}

void ccd_move(void){

	sum_ti=0;
	int i=0;
	for(i=0;i<(ti-1);i++)
	{
		delta_i[i]=delta_i[i+1];
		sum_ti+=delta_i[i];
	}
	delta_i[ti-1]=black_middle-previous_middle;
	sum_ti+=delta_i[ti-1];

	if(previous_middle==black_middle)
	{
		delta_speed=k1*(black_middle-track_line);
	}
	else
	{
//		delta_speed=k1*(black_middle-track_line);
		delta_speed=k1*(black_middle-track_line) + (int)(k3*((float)(black_middle-previous_middle)/(float)abs(black_middle-track_line)));
//		delta_speed=k1*(black_middle-track_line) + (int)(k2*((float)(sum_ti)/(float)ti));
	}

}

void car_move(void){
		if(delta_speed<0)
		{
			speed_left=max_speed;
			speed_right=max_speed+delta_speed;
		}
		else
		{
			speed_right=max_speed;
			speed_left=max_speed-delta_speed;
		}
		if(speed_left<-max_speed)speed_left=-max_speed;
		if(speed_right<-max_speed)speed_right=-max_speed;
		motor1_pwm(speed_left);
		motor2_pwm(speed_left);
		motor3_pwm(speed_right);
		motor4_pwm(speed_right);
}

void car_stop(void){
	if(remote.opticalflow&&(abs(speedcps)>10)){
		motor1_pwm((int)(-speedcps/2));
		motor1_pwm((int)(-speedcps/2));
		motor1_pwm((int)(-speedcps/2));
		motor1_pwm((int)(-speedcps/2));
	}else{
	motor1_pwm(0);
	motor2_pwm(0);
	motor3_pwm(0);
	motor4_pwm(0);
	}
}

void car_turn_left(void){
	motor1_pwm(-100);
	motor2_pwm(-50);
	motor3_pwm(100);
	motor4_pwm(50);
}

void car_turn_right(void){
	motor1_pwm(100);
	motor2_pwm(50);
	motor3_pwm(-100);
	motor4_pwm(-50);
}

void car_backoff(void){
	motor1_pwm(-20);
	motor2_pwm(-20);
	motor3_pwm(-20);
	motor4_pwm(-20);
}

void car_break(void){
//#ifndef INSANE
//	motor1_break();
//	motor2_break();
//	motor3_break();
//	motor4_break();
//#else
	if(remote.opticalflow){
		if(abs(speedcps)>10){
			motor1_pwm((int)(-speedcps));
			motor1_pwm((int)(-speedcps));
			motor1_pwm((int)(-speedcps));
			motor1_pwm((int)(-speedcps));
		}else{
			motor1_break();
			motor2_break();
			motor3_break();
			motor4_break();
		}
	}else{
		if(break_time>10){
			motor1_pwm(-20);
			motor2_pwm(-20);
			motor3_pwm(-20);
			motor4_pwm(-20);
		}else{
			motor1_break();
			motor2_break();
			motor3_break();
			motor4_break();

	}
}
//#endif
}

void ccd_clear(void){
	int i=0;
	for(i=0;i!=ti;i++){
		delta_i[i]=0;
		previous_middle=65;
		sum_ti = 0;
	}
}


int abs(int a){
	if(a<0)return -a;
	else return a;
}

void car_avoid(void){
	remote.speedleft = 0;
	remote.speedright = 0;
	switch(remote.direction){
	case GO_S:
		remote.speedleft=0;
		remote.speedright=0;
		break;
	case GO_F:
		if(IR_F_WARNING||IR_FL_WARNING||IR_FR_WARNING){
			if(ir_data.frount<11){
				remote.speedleft = -50;
				remote.speedright = -50;
			}
		}else{
			if((ir_data.frount<80)&&(ir_data.frount<remote.maxspeed)){
				remote.speedleft = ir_data.frount;
				remote.speedright = ir_data.frount;
				if(speedcps>ir_data.frount*1.6){
					remote.speedleft = 0;
					remote.speedright = 0;
				}
			}else{
				remote.speedleft = remote.maxspeed;
				remote.speedright = remote.maxspeed;
			}
		}
		break;
	case GO_FL:
		if(IR_F_WARNING||IR_FR_WARNING||IR_F_WARNING||IR_FL_WARNING){
		}else{
			remote.speedleft = 0;
			remote.speedright = remote.maxspeed;
		}
		break;
	case GO_FR:
		if(IR_F_WARNING||IR_FR_WARNING||IR_R_WARNING||IR_FL_WARNING){
		}else{
			remote.speedleft = remote.maxspeed;
			remote.speedright = 0;
		}
		break;
	case GO_L:
			remote.speedleft = -remote.maxspeed;
			remote.speedright = remote.maxspeed;
		break;
	case GO_R:
		remote.speedleft = remote.maxspeed;
		remote.speedright = -remote.maxspeed;
		break;
	case GO_BL:
		if(IR_BR_WARNING||IR_B_WARNING||IR_BL_WARNING||IR_L_WARNING){
		}else{
			remote.speedleft = 0;
			remote.speedright = -remote.maxspeed;
		}
		break;
	case GO_BR:
		if(IR_R_WARNING||IR_BR_WARNING||IR_B_WARNING||IR_BL_WARNING){
		}else{
			remote.speedleft = -remote.maxspeed;
			remote.speedright = 0;
		}
		break;
	case GO_B:
		if(IR_B_WARNING||IR_BR_WARNING||IR_BL_WARNING){
		}else{
			if((ir_data.back<80)&&(ir_data.back<remote.maxspeed)){
				remote.speedleft = -ir_data.back;
				remote.speedright = -ir_data.back;
				if(speed>(float)ir_data.frount*1.6){
					remote.speedleft = 0;
					remote.speedright = 0;
				}
			}else{
				remote.speedleft = -remote.maxspeed;
				remote.speedright = -remote.maxspeed;
			}
		}
		break;
	}
	motor1_pwm(remote.speedleft);
	motor2_pwm(remote.speedleft);
	motor3_pwm(remote.speedright);
	motor4_pwm(remote.speedright);
}

void car_startselftest(void){
	selftest.status = true;
	selftest.time = 10;
	selftest.FLtime = 0;
	selftest.FRtime = 0;
	selftest.BRtime = 0;
	selftest.BLtime = 0;
	selftest.warning = selftest_NULL;
	motorstatus.FL = 0;
	motorstatus.FR = 0;
	motorstatus.BL = 0;
	motorstatus.BR = 0;
	motor_sleep(1);

}

void car_selftest(void){
	if(selftest.status){
		if(selftest.time>0){
			selftest.time--;
			motor1_pwm(-20);
			motor2_pwm(20);
			motor3_pwm(-20);
			motor4_pwm(20);
			current_get_data(&current);
			if(current.bleft<10)selftest.BLtime++;
			if(current.bright<30)selftest.BRtime++;
			if(current.fleft<30)selftest.FLtime++;
			if(current.fright<10)selftest.FRtime++;
//			printf("%f\t%f\t%f\t%f\r\n",current.fleft,current.fright,current.bleft,current.bright);
		}else{
			car_stop();
			if(selftest.BLtime>6){selftest.warning = selftest_BL;motorstatus.BL = 2;}
			else if(selftest.BRtime>6){selftest.warning = selftest_BR;motorstatus.BR = 2;}
			else if(selftest.FLtime>6){selftest.warning = selftest_FL;motorstatus.FL = 2;}
			else if(selftest.FRtime>6){selftest.warning = selftest_FR;motorstatus.FR = 2;}
			else selftest.warning = selftest_NULL;
//			printf("%d %d %d %d\r\n",selftest.FLtime,selftest.FRtime,selftest.BLtime,selftest.BRtime);
//			printf("%d\r\n",selftest.warning);
			switch(selftest.warning){
			case selftest_FL:motor3_sleep(Sleep);motorstatus.FR = 1;break;
			case selftest_FR:motor1_sleep(Sleep);motorstatus.FL = 1;break;
			case selftest_BL:motor4_sleep(Sleep);motorstatus.BR = 1;break;
			case selftest_BR:motor2_sleep(Sleep);motorstatus.BL = 1;break;
			case selftest_NULL:motor_sleep(!Sleep);break;
			}
			selftest.status = false;
		}
	}
}
