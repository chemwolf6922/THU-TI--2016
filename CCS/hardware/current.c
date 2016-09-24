/*
 * current.c
 *
 *  Created on: 2016Äê5ÔÂ4ÈÕ
 *      Author: Feng
 */
#include "current.h"
#include "HL_adc.h"
#include "usart.h"

void current_init(void){
	adcInit();
}

void current_get_data(currentData_t* data){
	adcData_t adc_data[12];
	adcStartConversion(adcREG1,adcGROUP2);
	while((adcIsConversionComplete(adcREG1,adcGROUP2))==0);
	adcGetData(adcREG1, adcGROUP2,adc_data);
	int i=0;
	for(i=0;i!=12;i++){
		switch(adc_data[i].id){
		case 0:
			data->fleft=(float)adc_data[i].value*1.61;
			break;
		case 7:
			data->fright=(float)adc_data[i].value*1.61;
			break;
		case 10:
			data->bleft=(float)adc_data[i].value*1.61;
			break;
		case 2:
			data->bright=(float)adc_data[i].value*1.61;
			break;
		}
	}
}

void current_test(void){
	currentData_t temp;
	current_get_data(&temp);
	printf("%.2f\t%.2f\t%.2f\t%.2f\r\n",temp.fleft,temp.fright,temp.bleft,temp.bright);
}

