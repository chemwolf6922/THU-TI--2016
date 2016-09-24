/*
 * strategy.h
 *
 *  Created on: 2016Äê3ÔÂ22ÈÕ
 *      Author: Feng
 */

#ifndef __STRATEGY_H
#define __STRATEGY_H

#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "6050.h"
#include "ccd.h"
#include "motor.h"
#include "irdistence.h"
#include "counter.h"
#include "current.h"
#include "a9800.h"

typedef struct selftestData{
	boolean status;
	int time;
	int FLtime;
	int FRtime;
	int BLtime;
	int BRtime;
	int warning;
} selftestData_t;

void strategy_init(void);
void car_tick(void);
void car_startselftest(void);

#endif /* HARDWARE_STRATEGY_H_ */
