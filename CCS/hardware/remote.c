/*
 * remote.c
 *
 *  Created on: 2016年3月22日
 *      Author: Feng
 */
#include "remote.h"
#include "usart.h"

int remote_status=0;
u8 receive[10] = {0};

void remote_init(void){

//	sciEnableNotification(sciREG1,SCI_RX_INT);
//	_enable_IRQ_interrupt_();
}

void remote_send_request(void){
		sciSendByte(sciREG1,0xFF);
}

void remote_get_data(remoteData_t* data){
	remote_status=0;
	int i=0;
	for(i=0;i!=10;i++){
		receive[i]=0;
	}
	rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK0);
	remote_send_request();

	while(remote_status<1500){

		if(sciIsRxReady(sciREG1)){
			sciReceive (sciREG1, 10, receive);		//注意数据类型
			break;
		}
	}

	data->status = receive[9];
	data->manual = receive[8];
	data->opticalflow = receive[7];
	data->selftest = receive[6];
	data->clearErrorPin = receive[5];
	data->maxspeed = receive[0];
	data->direction = receive[1];
	data->delaytime = receive[2];
	data->speedlimitcondition = receive[3];
	data->minimunspeedlimit = receive[4];
	rtiStopCounter(rtiREG1,rtiCOUNTER_BLOCK0);
}

void remote_set_status(void){
	remote_status++;
}

void remote_test(void){
	remoteData_t data;
	remote_get_data(&data);
	printf("%d\t%d\t%d\r\n",data.speedleft,data.speedright,data.status);
}

reportData_t remote_get_reportData(irData_t* ir,currentData_t* c,motorStatus_t* motor){
	reportData_t temp;
	temp.irdata = ir;
	temp.current = c;
	temp.motorstatus = motor;
	temp.error = 0;
	temp.speed = .0;
	return temp;
}

void remote_report(reportData_t* report){
	u8 transmit[5] = {0};
	u8 temp[8] = {0};
	u16 tempint = 0;
	if(report->irdata->fleft<15)temp[0] = 1;
	if(report->irdata->frount<20)temp[1] = 1;
	if(report->irdata->fright<15)temp[2] = 1;
	if(report->irdata->right<15)temp[3] = 1;
	if(report->irdata->bright<15)temp[4] = 1;
	if(report->irdata->back<15)temp[5] = 1;
	if(report->irdata->bleft<15)temp[6] = 1;
	if(report->irdata->left<15)temp[7] = 1;
	transmit[0] = (u8)((temp[0]<<7)|(temp[1]<<6)|(temp[2]<<5)|(temp[3]<<4)|(temp[4]<<3)|(temp[5]<<2)|(temp[6]<<1)|temp[7]);
	if(transmit[0] == 0xFF)transmit[0] = 0xFE;
	temp[0] = (u8)report->current->fleft;
	temp[1] = (u8)report->current->fright;
	temp[2] = (u8)report->current->bright;
	temp[3] = (u8)report->current->fleft;
	tempint = (temp[0]+temp[1]+temp[2]+temp[3])/4;							//current/4
	if(tempint>0xFE)tempint=0xFE;
	transmit[1] = (u8)tempint;
	transmit[2] = (u8)report->error;											//error
	temp[0] = report->motorstatus->FL;
	temp[1] = report->motorstatus->FR;
	temp[2] = report->motorstatus->BR;
	temp[3] = report->motorstatus->BL;
	transmit[3] = (u8)((temp[0]<<6)|(temp[1]<<4)|(temp[2]<<2)|(temp[3]));		//motorstatus
	tempint = (u8)(abs((int)(report->speed*100.0)));									//speed
	if(tempint>0xFE)tempint = 0xFE;
	transmit[4] = (u8)tempint;
	int i=0;
	for(i=0;i!=5;i++){
		if(transmit[i]==0xFF)transmit[i]=0xFE;
	}
	sciSend(sciREG1,5,transmit);
}
