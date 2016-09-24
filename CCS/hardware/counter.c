/*
 * counter.c
 *
 *  Created on: 2016Äê5ÔÂ1ÈÕ
 *      Author: Feng
 */
#include "counter.h"
#define setting 2000000

void counter_init(void){
	ecapInit();
}


int counter_get_status(void){
	if((ecapREG2->TSCTR>setting)&&(ecapREG3->TSCTR>setting)){
		return 1;
	}else{
		return 0;
	}
}

float counter_get_speed(ecapBASE_t* dir){
	if(dir->TSCTR>setting){
		return 0.0;
	}else{
		return 1620.0/(dir->CAP1/75.0);
	}
}

float counter_get_avg(){
	return((counter_get_speed(COUNTER_LEFT)+counter_get_speed(COUNTER_RIGHT))/2.0);
}

