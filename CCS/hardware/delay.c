#include "delay.h"
int delay_t_us=0;
bool sys_status=0;
uint32 i;

void delay_init(void){
	rtiInit();
	rtiEnableNotification(rtiREG1,rtiNOTIFICATION_COMPARE0);
	rtiEnableNotification(rtiREG1,rtiNOTIFICATION_COMPARE1);
	_enable_IRQ_interrupt_();
}

void delay_us(uint32 t_us){
	delay_t_us=t_us;
	rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK0);
	while(delay_t_us!=0);
	rtiStopCounter(rtiREG1,rtiCOUNTER_BLOCK0);
}

void delay_ms(uint32 t_ms){
	for(i=0;i!=t_ms;i++)
	{
		delay_us(1000);
	}
}

bool read_sys_status(void){
	return sys_status;
}

void set_sys_status(bool a){
	sys_status=a;
}


//delete the same function in HL_notification.h
void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
	if(notification==rtiNOTIFICATION_COMPARE0){			//
		delay_t_us--;
		remote_set_status();
	}
	if(notification==rtiNOTIFICATION_COMPARE1){
		sys_status=1;
	}
}
