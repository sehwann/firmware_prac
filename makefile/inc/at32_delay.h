#ifndef _AT32_DELAY_H_
#define _AT32_DELAY_H_
#include <global.h>


/*Delay function*/
void Delay_init(void);
void Delay_us(u32 nus);
void Delay_ms(u16 nms);
void Delay_sec(u16 sec);

#endif
