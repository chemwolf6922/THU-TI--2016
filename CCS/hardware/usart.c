#include "usart.h"

void usart_init(uint32 baud){
	sciInit();
	sciSetBaudrate(sciREG1,baud);
}

int fputc(int ch, FILE *f){
	while ((sciREG1->FLR & 0x4) == 4);
	sciSendByte (sciREG1, (u8)ch);
	return ch;
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
	while ((sciREG1->FLR & 0x4) == 4);
	sciSendByte (sciREG1, (u8)_ptr[i]);
  }

  return len;
}
