#include "./CH552.h"
#include "./Debug.h"
#include "./Descriptor.h"
#include <stdio.h>
#include <string.h>


void hid_main();
void composite_main();


void nec_init();
int nec_enable_timer_interrupt(int value);
void nec_emmit(unsigned int vendor_code,unsigned int keycode);

