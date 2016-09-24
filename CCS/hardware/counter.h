/*
 * counter.h
 *
 *  Created on: 2016Äê5ÔÂ1ÈÕ
 *      Author: Feng
 */

#ifndef HARDWARE_COUNTER_H_
#define HARDWARE_COUNTER_H_

#define COUNTER_LEFT ecapREG2
#define COUNTER_RIGHT ecapREG3
#define COUNTER_STOP 1
#define COUNTER_MOVE 0

#include "sys.h"
#include "HL_ecap.h"

void counter_init(void);
int counter_get_status(void);
float counter_get_speed(ecapBASE_t* dir);
float counter_get_avg();

#endif /* HARDWARE_COUNTER_H_ */
