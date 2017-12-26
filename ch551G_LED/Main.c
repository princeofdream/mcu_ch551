
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 触摸按键中断和查询方式进行采集并报告当前采样通道按键状态，包含初始化和按键采样等演示函数
*******************************************************************************/
#include "CompatibilityHID.h"
#include "Timer.h"

#pragma  NOAREGS

main()
{
	//init mcu
	CfgFsys();                                                           //CH559时钟选择配置
	mDelaymS(5);                                                          //修改主频等待内部晶振稳定,必加
	mInitSTDIO( );                                                        //串口0初始化

	IR_PIN = 1;
	LED_B=1;
	/* timer_main(); */
	hid_main();
	/* nec_init(); */
	/* nec_enable_timer_interrupt(0); */
	/* nec_enable_timer_interrupt(2); */
	while(1);
	return 0;
}


