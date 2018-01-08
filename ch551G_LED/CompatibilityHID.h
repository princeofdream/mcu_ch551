#ifndef __COMPATIBILITYHID_H__
#define __COMPATIBILITYHID_H__

#include "./CH552.h"
#include "./Debug.h"
#include "./Descriptor.h"
#include <stdio.h>
#include <string.h>

//mask is 0x4000
#define IR_WITH_PWM_MASK (0x1<<14)

#define IR_PROTOCOL_NEC (0x1<<0)
#define IR_PROTOCOL_RC5 (0x1<<1)
#define IR_PROTOCOL_RC6 (0x1<<2)

void hid_main();
void composite_main();


void nec_init(UINT16X *hid_buf);
void nec_enable_timer_interrupt(int value);
void nec_emmit(unsigned int vendor_code,unsigned int keycode);
void start_nec_transfer(int mode_loop);

void pwm_main();

#endif
