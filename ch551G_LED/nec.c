/*
 * =====================================================================================
 *
 *       Filename:  nec.c
 *
 *    Description:  support nec ir protocol
 *
 *        Version:  1.0
 *        Created:  12/19/2017 2:11:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (Jsl), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */


#include "CompatibilityHID.h"
#include "CH552.h"
#include "Debug.h"
#include "Timer.h"
#include <stdio.h>

#pragma  NOAREGS

#if 1

UINT16 nec_vendor_code = 0;
UINT16 nec_keycode = 0;


void start_nec_transfer(UINT8 mode_loop)
{
#if 1
	if(mode_loop == 0)
	{
		mode_loop++;
		mTimer_x_SetData(2,0xD300); //4500us
		ET2=1;
		mTimer2RunCTL( 1 );                                                      //开定时器
		return;
	}
	else if( mode_loop == 1)
	{
		PWM2 = ~PWM2;
		LED_B = ~LED_B;
		mTimer_x_SetData(2,0xD300); //4500us
		ET2=1;
		mTimer2RunCTL( 1 );                                                      //开定时器
		return;
	}
	else if( mode_loop <= 64 + 2) //NEC
	{
		PWM2 = ~PWM2;
		LED_B = ~LED_B;
		if(mode_loop%2 == 0) //plus
		{
			mTimer_x_SetData(2,0x19FF); //560us
		}
		else
		{
			mTimer_x_SetData(2,0x19FF); //560us
			if(mode_loop<=32+2)
			{
				if(nec_vendor_code & 0x8000)
					mTimer_x_SetData(2,0x4DFF); //1690us
				nec_vendor_code=nec_vendor_code<<1;
			}
			else
			{
				if(nec_keycode & 0x8000)
					mTimer_x_SetData(2,0x4DFF); //1690us
				nec_keycode=nec_keycode<<1;
			}
		}
	}
	else if( mode_loop <= 64 + 2 + 2) //NEC
	{
		PWM2 = ~PWM2;
		LED_B = ~LED_B;
		mTimer_x_SetData(2,0x19FF); //560us
	}
	else
	{
		PWM2 = LOGIC_ZERO;
		LED_B = LOGIC_ZERO;
		mDelaymS(10);                                                          //修改主频等待内部晶振稳定,必加
		ET2=0;
		mTimer2RunCTL( 0 );
		return;
	}
#endif
	return ;
}

void nec_enable_timer_interrupt(int value)
{
	if(value == 0)
	{
		mTimer0RunCTL(1);
		ET0 = 1;
		EA = 1;
	}
	else if(value == 2)
	{
		mTimer2RunCTL(1);
		ET2 = 1;
		EA = 1;
	}
	printf("<%s>\n",__FILE__);
	return ;
}
#endif

void nec_init(UINT16X *hid_buf)
{
#if 0
	/* mTimer0Clk4DivFsys(); */
	T2MOD &= ~bTMR_CLK;
	T2MOD |= bT0_CLK;
    mTimer_x_ModInit(0,2);
	mTimer_x_SetData(0,0x13FF);

    /* mTimer2ClkFsys(); */
    mTimer2Clk12DivFsys();
	/* mTimer2Clk12DivFsys( ) {T2MOD &= ~(bTMR_CLK | bT2_CLK);C_T2 = 0;}      //定时器,时钟=Fsys/12 T2标准时钟 */
    mTimer_x_ModInit(2,0);

	/* mTimer_x_SetData(2,0x0115); //560us */
	/* mTimer_x_SetData(2,0x034C); //1690us */
	/* mTimer_x_SetData(2,0x08D1); //4500us */
	mTimer_x_SetData(2,0x11A0); //9000us
#endif


#if 0 //def T0_INT
    printf("T0 Test ...\n");
	/* mTimer0Clk12DivFsys();	                                                   //T0定时器时钟设置 */
	/* mTimer0ClkFsys();	                                                   //T0定时器时钟设置 */
	/* mTimer0Clk4DivFsys(); */
	T2MOD &= ~bTMR_CLK;
	T2MOD |= bT0_CLK;
    mTimer_x_ModInit(0,2);                                                     //T0 定时器模式设置
	//T0定时器赋值
	/* mTimer_x_SetData(0,0x7FFF); //0xFFFF-0x7FFF = 0x8000 --> 47K 10.5+10.5us=21us */
	/* mTimer_x_SetData(0,0xBFFF); //0xFFFF-0xBFFF = 0x4000 --> 31.44K 16+16us=32us */
	/* mTimer_x_SetData(0,0x9FFF); //0xFFFF-0x9FFF = 0x6000 --> 37.31K 16+16us=26.8us */
	mTimer_x_SetData(0,0x26FF); //0xFFFF-0x9FFF = 0x6000 --> 38K 16+16us=26.3us
    mTimer0RunCTL(1);                                                          //T0定时器启动
    ET0 = 1;                                                                   //T0定时器中断开启
    EA = 1;
    while(1);
#endif


#if 0 //def T2_INT
    printf("T2 Test ...\n");
    mTimer2ClkFsys();
    mTimer_x_ModInit(2,1);
	/* mTimer_x_SetData(2,0x19FF); //560us */
	mTimer_x_SetData(2,0x11A0); //1690us
	//T2定时器启动
    mTimer2RunCTL(1);
	//T2定时器中断开启
    ET2 = 1;
    EA = 1;
    while(1);
#endif

	nec_vendor_code = hid_buf[1];
	printf("Start ir ext 0x%x, buf1,%x,buf2, %x\n",nec_vendor_code,hid_buf[1],hid_buf[2],hid_buf[3]);
	/* nec_vendor_code = (nec_vendor_code << 16 )||hid_buf[2]; */
	nec_keycode = hid_buf[2];
	printf("Start ir ext 0x%x\n",nec_keycode);
#if 1 //def USING_T0_T2
	// Fsys/4
	/* T2MOD &= ~bTMR_CLK; */
	/* T2MOD |= bT0_CLK; */
	// Fsys
	mTimer2ClkFsys();
	mTimer_x_ModInit(0,2);
	mTimer_x_SetData(0,0x26FF);  //26us 38K

	mTimer_x_ModInit(2,1);

	/* mTimer_x_SetData(2,0x0115); //560us */
	/* mTimer_x_SetData(2,0x034C); //1690us */
	/* mTimer_x_SetData(2,0x08D1); //4500us */
	PWM2 = LOGIC_ONE;
	LED_B = LOGIC_ONE;
	mTimer_x_SetData(2,0xD300); //4500us
#endif
	set_protocol(1);	//set to nec
    mTimer2RunCTL(1);
    ET2 = 1;
    EA = 1;
}
