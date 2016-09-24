/*
 * irdistence.h
 *
 *  Created on: 2016Äê4ÔÂ20ÈÕ
 *      Author: Feng
 */

#ifndef HARDWARE_IRDISTENCE_H_
#define HARDWARE_IRDISTENCE_H_

#include "sys.h"

typedef struct irData{
	int frount;
	int fleft;
	int fright;
	int left;
	int right;
	int back;
	int bleft;
	int bright;
} irData_t;

void irdistence_init(void);
void irdistence_get_data(irData_t* data);
void irdistence_get_dis(irData_t* dis);
void irdistence_test(void);
void irdistence_test_data(void);


#endif /* HARDWARE_IRDISTENCE_H_ */
