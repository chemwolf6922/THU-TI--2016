#include "6050.h"
#include "usart.h"


double zero=0;

#pragma SET_DATA_SECTION(".sharedRAM")
u8 RecBuff[size] = {0};
#pragma SET_DATA_SECTION()

void mpu6050_reset(void)
{
	do
	{
	sci3_dma_init(RecBuff);
	while(dmaIsBusy()==0);
	delay_ms(3);
	}while((RecBuff[0]!=0x55)|(RecBuff[1]!=0x51));
}

void mpu6050_init(void)
{
	delay_init();
	mpu6050_reset();
}

double mpu6050_get_angle(void)
{
	while((RecBuff[0]!=0x55)|(RecBuff[1]!=0x51))
	{
		mpu6050_reset();
	}
	u16 YawH=RecBuff[29];
	u16 YawL=RecBuff[30];
	double Yaw=0;
	Yaw=((double)((YawH<<8)|YawL))/32768.0*180-zero;
	if(Yaw<0)
	{
		Yaw+=360;
	}
	if(Yaw>180)
	{
		Yaw-=360;
	}
	return Yaw;
}

double mpu6050_get_omega(void)
{
	while((RecBuff[0]!=0x55)|(RecBuff[1]!=0x51))
	{
		mpu6050_reset();
	}
	u16 wzL=RecBuff[17];
	u16 wzH=RecBuff[18];
	double wz=0;
	wz=((double)((wzH<<8)|wzL))/32768.0*2000;
	if(wz>2000)
	{
		wz-=4000;
	}
	return wz;
}

void mpu6050_resetzero(void)
{
	while((RecBuff[0]!=0x55)|(RecBuff[1]!=0x51))
	{
		mpu6050_reset();
	}	
	u16 YawH=RecBuff[29];
	u16 YawL=RecBuff[30];
	zero=((double)((YawH<<8)|YawL))/32768.0*180;
}

void mpu6050_test(void){
	int i=0;
	for(i=0;i!=size;i++){
		printf("%X ",RecBuff[i]);
	}
	printf("\r\n");
}
