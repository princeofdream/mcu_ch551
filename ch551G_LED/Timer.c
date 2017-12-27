#include "CH552.h"
#include "Debug.h"
#include "Timer.h"
#include "stdio.h"

#pragma  NOAREGS

#define NO_IR_PWM
/* #undef NO_IR_PWM */
static int    rt_protocol     = 1;
static int    rt_head_code    = 0;
static int    logic_pluse     = 0;

UINT8 mTimer_x_ModInit(UINT8 x ,UINT8 mode)
{
    if(x == 0) {
		TMOD = TMOD & 0xf0 | mode;
	} else if(x == 1) {
		TMOD = TMOD & 0x0f | (mode<<4);
	} else if(x == 2) {
		RCLK = 0;
		TCLK = 0;
		CP_RL2 = 0;
	} else {
		return FAIL;
	}
    return SUCCESS;
}

void mTimer_x_SetData(UINT8 x,UINT16 dat)
{
    UINT16 tmp;
    tmp = 0xffff - dat;
	if(x == 0) {
		TL0 = tmp & 0xff;
		TH0 = (tmp>>8) & 0xff;
	} else if(x == 1) {
		TL1 = tmp & 0xff;
		TH1 = (tmp>>8) & 0xff;
	} else if(x == 2) {
      RCAP2L = TL2 = tmp & 0xff;                                               //16位自动重载定时器
      RCAP2H = TH2 = (tmp>>8) & 0xff;
    }
}

void mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1
{
#ifndef NO_IR_PWM
	if(logic_pluse == 1)
	{
		PWM2 = ~PWM2;
		LED_B = ~LED_B;
	}
	else
	{
		PWM2 = LOGIC_ZERO;
		LED_B = LOGIC_ZERO;
	}
#else
	PWM2 = ~PWM2;
	LED_B = ~LED_B;
	mTimer0RunCTL(0);
	/* printf("int test by JamesL <%s:%d>\n",__FILE__,__LINE__); */
#endif
	return;
}

void mTimer2Interrupt( void ) interrupt INT_NO_TMR2 using 3                //timer2中断服务程序,使用寄存器组3
{
    mTimer2RunCTL(0);
	ET2=0;
    if(TF2)
    {
        TF2 = 0;                                                             //清空定时器2溢出中断
		if(rt_protocol & IR_PROTOCOL_NEC == IR_PROTOCOL_NEC)
		{
#if 1
			start_nec_transfer(rt_head_code);
			rt_head_code++;
			if(rt_head_code > 64 + 2 + 2)
			{
				PWM2 = LOGIC_ZERO;
				LED_B = LOGIC_ZERO;
				mDelaymS(10);                                                          //修改主频等待内部晶振稳定,必加
				ET2=0;
				mTimer2RunCTL( 0 );
				rt_head_code = 0;
				printf("!END!\n");
				return;
			}
#else
			if(rt_head_code == 0)
			{
				logic_pluse = 2;
				rt_head_code++;
				mTimer_x_SetData(2,0xD300); //4500us
				ET2=1;
				mTimer2RunCTL( 1 );                                                      //开定时器
				return;
			}
			else if( rt_head_code == 1)
			{
				rt_head_code++;
				PWM2 = ~PWM2;
				LED_B = ~LED_B;
				mTimer_x_SetData(2,0xD300); //4500us
				logic_pluse = 2;
				ET2=1;
				mTimer2RunCTL( 1 );                                                      //开定时器
				return;
			}
			else if( rt_head_code <= 32+32) //NEC
			{
				rt_head_code++;
				if(logic_pluse == 1)
				{
#ifdef NO_IR_PWM
					PWM2 = ~PWM2;
					LED_B = ~LED_B;
#endif
					/* mTimer_x_SetData(2,0x19FF); //560us */
					mTimer_x_SetData(2,0x4DFF); //1690us
					logic_pluse = 0;
				}
				else
				{
#ifdef NO_IR_PWM
					PWM2 = ~PWM2;
					LED_B = ~LED_B;
#endif
					logic_pluse = 1;
					/* mTimer_x_SetData(2,0x4DFF); //1690us */
					mTimer_x_SetData(2,0x19FF); //560us
				}
			}
			else
			{
				PWM2 = LOGIC_ZERO;
				LED_B = LOGIC_ZERO;
				mDelaymS(10);                                                          //修改主频等待内部晶振稳定,必加
				ET2=0;
				mTimer2RunCTL( 0 );
				rt_head_code = 0;
				logic_pluse = 0;
				printf("!END!\n");
				return;
			}
#endif
		}
		else
		{
			PWM2 = ~PWM2;
			LED_B = ~LED_B;
		}
    }
	ET2=1;
	mTimer2RunCTL(1);                                               //开定时器
	return;
}

void set_protocol(int protocol)
{
	rt_protocol = protocol;
	return;
}
