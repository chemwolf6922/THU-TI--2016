#include "adc.h"
#include "HL_adc.h"
#include "delay.h"

#define chs 10

void  Adc_Init(void)
{ 	
	adcInit();
}

u16 Get_Adc(u8 ch)   
{
	adcData_t adc_data;
	adcStartConversion(adcREG1,adcGROUP1);
	while((adcIsConversionComplete(adcREG1,adcGROUP1))==0);
	adcGetData(adcREG1, adcGROUP1,&adc_data);
	return adc_data.value;
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 



























