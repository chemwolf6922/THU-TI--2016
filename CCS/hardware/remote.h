/*
 * remote.h
 *
 *  Created on: 2016Äê3ÔÂ22ÈÕ
 *      Author: Feng
 */

#ifndef __REMOTE_H
#define __REMOTE_H

//byte9 status
#define CONNECTED 1
#define DISCONNECTED 0
//byte8 manul
#define AUTO 1
#define MANUAL 0
//byte1 direction
#define GO_S 0
#define GO_F 1
#define GO_FL 2
#define GO_FR 3
#define GO_L 4
#define GO_R 5
#define GO_B 6
#define GO_BL 7
#define GO_BR 8
//byte0 maxspeed


#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "HL_esm.h"
#include "irdistence.h"
#include "current.h"

/*
 *  0	normal
 *  1	sleep
 *  2	poweroff
 */
typedef struct motorStatus{
	u8 FL;
	u8 FR;
	u8 BL;
	u8 BR;
} motorStatus_t;

typedef struct reportData{
	irData_t* irdata;
	currentData_t* current;
	motorStatus_t* motorstatus;
	float speed;
	u8 error;
} reportData_t;

typedef struct remoteData{
	int maxspeed;			//0
	int direction;			//1
	int speedleft;
	int speedright;
	int delaytime;			//2
	int speedlimitcondition;		//3
	int minimunspeedlimit;			//4
	bool selftest;					//6
	bool opticalflow;				//7
	bool manual;					//8
	bool status;					//9
	bool clearErrorPin;				//5
} remoteData_t;

void remote_init(void);
void remote_get_data(remoteData_t* data);
void remote_set_status(void);
void remote_test(void);
void remote_report(reportData_t* report);
reportData_t remote_get_reportData(irData_t* ir,currentData_t* c,motorStatus_t* motor);

#endif /* HARDWARE_REMOTE_H_ */
