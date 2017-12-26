
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 Time 初始化、定时器、计数器赋值，T2捕捉功能等
                       定时器中断处理
*******************************************************************************/

#if 0
#include "CH552.h"
#include "Debug.h"
#include "Timer.h"
#include "stdio.h"

#pragma  NOAREGS

UINT8 TIMER_FLAG;
UINT16 Cap[8] = {0};

void timer_main()
{
    CfgFsys( );                                                                //CH554时钟选择配置
    mDelaymS(5);                                                               //修改主频，建议稍加延时等待主频稳定
    mInitSTDIO( );                                                             //串口初始化
    printf("start ...\n");


#ifdef T0_INT
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

#ifdef T1_INT
    printf("T1 Test ...\n");
	mTimer1Clk12DivFsys();
	mTimer_x_ModInit(1,0);
	mTimer_x_SetData(1,0x19FF);
	//T1定时器启动
    mTimer1RunCTL(1);
	//T1定时器中断开启
    ET1 = 1;
    EA = 1;
    while(1);
#endif


#ifdef T2_INT
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


#ifdef T2_CAP
    printf("T2_CAP Test ...\n");
    mTimer2ClkFsys();                                                         //T2定时器时钟设置
    mTimer_x_SetData(2,0);                                                    //T2 定时器模式设置捕捉模式
    CAP2Init(1);	                                                            //T2 CAP2设置，任意沿捕捉
    CAP1Init(1);	                                                            //T2 CAP2设置，任意沿捕捉
    mTimer2RunCTL(1);                                                         //T2定时器启动
    ET2 = 1;                                                                  //T2定时器中断开启
    EA = 1;
    while(1);
#endif

#ifdef USING_T0_T2
	/* mTimer0Clk4DivFsys(); */
	T2MOD &= ~bTMR_CLK;
	T2MOD |= bT0_CLK;
    mTimer_x_ModInit(0,2);
	mTimer_x_SetData(0,0x26FF);

	mTimer2ClkFsys();
    /* mTimer2Clk12DivFsys(); */
	/* mTimer2Clk12DivFsys( ) {T2MOD &= ~(bTMR_CLK | bT2_CLK);C_T2 = 0;}      //定时器,时钟=Fsys/12 T2标准时钟 */
    mTimer_x_ModInit(2,1);

	/* mTimer_x_SetData(2,0x0115); //560us */
	/* mTimer_x_SetData(2,0x034C); //1690us */
	/* mTimer_x_SetData(2,0x08D1); //4500us */
	PWM2 = 0;
	LED_B = 0;
	mTimer_x_SetData(2,0xD300); //4500us

#if 0
	//T1定时器启动
    mTimer0RunCTL(1);                                                          //T0定时器启动
    ET0 = 1;                                                                   //T0定时器中断开启
    EA = 1;
#endif
#if 1
	//T2定时器启动
    mTimer2RunCTL(1);
    ET2 = 1;
    EA = 1;
    /* while(1); */
#endif
#endif

    while(1);
}
#endif
