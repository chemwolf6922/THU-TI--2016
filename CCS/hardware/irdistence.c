/*
 * irdistence.c
 *
 *  Created on: 2016Äê4ÔÂ20ÈÕ
 *      Author: Feng
 */
#include "irdistence.h"
#include "HL_adc.h"
#include "usart.h"

void irdistence_init(void){
	adcInit();
}


void irdistence_get_data(irData_t* data){
	adcData_t adc_data[12];
	adcStartConversion(adcREG1,adcGROUP2);
	while((adcIsConversionComplete(adcREG1,adcGROUP2))==0);
	adcGetData(adcREG1, adcGROUP2,adc_data);
	int i=0;
	for(i=0;i!=12;i++){
		switch(adc_data[i].id){
		case 3:
			data->frount=adc_data[i].value;
			break;
		case 11:
			data->fleft=adc_data[i].value;
			break;
		case 18:
			data->fright=adc_data[i].value;
			break;
		case 19:
			data->left=adc_data[i].value;
			break;
		case 20:
			data->right=adc_data[i].value;
			break;
		case 21:
			data->back=adc_data[i].value;
			break;
		case 4:
			data->bleft=adc_data[i].value;
			break;
		case 12:
			data->bright=adc_data[i].value;
			break;
		}
	}
}

void irdistence_get_dis(irData_t* dis){
	irdistence_get_data(dis);
	dis->frount=40000/dis->frount;
	if(dis->frount>80)dis->frount=81;
	dis->fleft=40000/dis->fleft;
	if(dis->fleft>80)dis->fleft=81;
	dis->fright=40000/dis->fright;
	if(dis->fright>80)dis->fright=81;
	dis->left=40000/dis->left;
	if(dis->left>80)dis->left=81;
	dis->right=40000/dis->right;
	if(dis->right>80)dis->right=81;
	dis->back=40000/dis->back;
	if(dis->back>80)dis->back=81;
	dis->bleft=16500/dis->bleft;
	if(dis->bleft>30)dis->bleft=31;
	dis->bright=16500/dis->bright;
	if(dis->bright>30)dis->bright=31;
}

void irdistence_test(void){
	irData_t temp;
	irdistence_get_dis(&temp);
	sciSendByte(sciREG1,(u8)(temp.frount%256U));
	sciSendByte(sciREG1,(u8)(temp.fleft%256U));
	sciSendByte(sciREG1,(u8)(temp.fright%256U));
	sciSendByte(sciREG1,(u8)(temp.left%256U));
	sciSendByte(sciREG1,(u8)(temp.right%256U));
	sciSendByte(sciREG1,(u8)(temp.back%256U));
	sciSendByte(sciREG1,(u8)(temp.bleft%256U));
	sciSendByte(sciREG1,(u8)(temp.bright%256U));
	sciSendByte(sciREG1,'\r');
	sciSendByte(sciREG1,'\n');
}

void irdistence_test_data(void){
	irData_t temp;
	irdistence_get_data(&temp);
//	printf("%d\t",temp.frount);
	printf("%d\t",temp.fleft);
//	printf("%d\t",temp.fright);
//	printf("%d\t",temp.left);
//	printf("%d\t",temp.right);
//	printf("%d\t",temp.back);
	printf("%d\t",temp.bleft);
	printf("%d\r\n",temp.bright);
}
