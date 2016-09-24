#include "ccd.h"


u16 ccd_data_temp=0;
u32 integration_time=800;		//800 light on		//4600 light off
u16 ccd_data[128]={0};
u8 ccd_binary[128]={0};

void ccd_init(void)
{
//GPIO Init
	gioInit();
	
	SI0;
	CLK0;
//adc init	
	Adc_Init();
}

void ccd_integration(void)
{
	SI0;
	CLK0;
	delay_us(1);
	SI1;
	int n=0;
	for(n=0;n<128;n++)
	{
		delay_us(1);
		CLK1;
		SI0;
		delay_us(1);	
//		Get_Adc(16);
		CLK0;
	}
	
	
	for(n=0;n<(integration_time/2);n++)
	{
		delay_us(1);
		CLK1;
		delay_us(1);
		CLK0;
  }
}

void ccd_set_integrationtime(u32 time)
{
	integration_time=time;
}


void ccd_autointegration(void)
{
	u16 max=0;
	u16 min=4095;
	

	while((min>1300)|(max<3500))
	{
		ccd_get_data();
		max=0;
		min=4095;
		int i=0;
		for(i=3;i!=125;i++)
		{
			if(max<ccd_data[i])
			{
				max=ccd_data[i];
			}
			if(min>ccd_data[i])
			{
				min=ccd_data[i];
			}
		}
		if((max>5000)|(min<500))			//限制极限
		{
			break;
		}
		if(max<3500)
		{
			integration_time+=200;
		}
		else if(min>1300)
		{
			integration_time-=200;
		}
		
//	if(integration_time>3900)		//////////////////////
//	{
//		integration_time=4800;
//	}
//	else
//	{
//		integration_time=2800;
//	}
	}
//	binary_middle=((max/2)+(min/2));			//
	
}

void ccd_get_data(void)
{
	ccd_integration();				///////
	SI0;
	CLK0;
	delay_us(1);
	SI1;
	int n=0;
	for(n=0;n<128;n++)
	{
		delay_us(1);
		CLK1;
		SI0;
		delay_us(1);	
		ccd_data[n]=Get_Adc(16);
		CLK0;
	}
	
	delay_us(1);
	CLK1;
	delay_us(1);
	CLK0;
}

//////////////////////////////////////////////////////////////////
void ccd_binary_data(void)
{
	int i=0;
	for(i=0;i!=128;i++)
	{
		if(ccd_data[i]>(1500))
		{
			ccd_binary[i]=1;
		}
		else
		{
			ccd_binary[i]=0;
		}
	}

}
////////////////////////////////////////////////////////////////
void ccd_find_bound(u8* l,u8* r){
	int i;
	int j;
	*l=63;
	*r=64;
	ccd_get_data();
	for(i=64;i!=100;i++){
			if(((ccd_data[i+2]-ccd_data[i])>300)&&((ccd_data[i-2]-ccd_data[i])<150)){
				*r=i+2;
				for(j=*r-2;j!=7;j--){
					if(ccd_data[j]>(ccd_data[*r]-50)){
						*l=j;
						break;
					}
				}
				break;
			}
		}
	for(i=63;i!=30;i--){
			if(((ccd_data[i-2]-ccd_data[i])>300)&&((ccd_data[i+2]-ccd_data[i])<150)){
			*l=i-2;
			for(j=*l+2;j!=120;j++){
			if(ccd_data[j]>(ccd_data[*l]-50)){
				*r=j;
				break;
				}
			}
			break;
		}
	}
}
////////////////////////////////////////////////////////////////

u16 ccd_read_data(u8 num)
{
	return(ccd_data[num]);
}

u8 ccd_read_binary(u8 num)
{
	return(ccd_binary[num]);
}
//////////////////////////////////////////////////////////////
void ccd_test(void){
	ccd_get_data();
	int i=0;
	for(i=0;i!=128;i++){
		sciSendByte(sciREG1,(u8)(ccd_data[i]/256U));
		sciSendByte(sciREG1,(u8)(ccd_data[i]%256U));
	}
	sciSendByte(sciREG1,'\r');
	sciSendByte(sciREG1,'\n');
}

void ccd_binary_test(void){
	u8 left=0;
	u8 right=0;
	ccd_find_bound(&left,&right);
	int i=0;
	for(i=0;i!=128;i++){
		sciSendByte(sciREG1,(u8)(ccd_data[i]/256U));
		sciSendByte(sciREG1,(u8)(ccd_data[i]%256U));
	}
	sciSendByte(sciREG1,left);
	sciSendByte(sciREG1,right);
	sciSendByte(sciREG1,'\r');
	sciSendByte(sciREG1,'\n');
}
