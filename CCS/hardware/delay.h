#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"
#include "HL_rti.h"
#include "HL_esm.h"

#include "strategy.h"
#include "remote.h"

void delay_init(void);
void delay_us(uint32 t_us);
void delay_ms(uint32 t_ms);

bool read_sys_status(void);
void set_sys_status(bool a);

#endif
