
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 触摸按键中断和查询方式进行采集并报告当前采样通道按键状态，包含初始化和按键采样等演示函数
*******************************************************************************/
#include "CH552.H"
#include "Debug.H"
#include "TouchKey.H"
#include <stdio.h>
#include "CompatibilityHID.h"

#pragma  NOAREGS


int led_main()
{
	int i;

	CfgFsys( );                                                                //CH554时钟选择配置
	mDelaymS(5);
	mInitSTDIO();

	P3_DIR_PU=0xFF;
	i=6;
	printf("%x\n ",i >> 0);
	printf("%x\n ",i >> 1);
	printf("%x\n ",i >> 2);
	printf("%x\n ",i >> 3);
	printf("%x\n ",i >> 4);
	printf("%x\n ",i >> 5);
	printf("%x\n ",i >> 6);
	printf("%x\n ",i >> 7);
	printf("%x\n ",i >> 8);

	while(1)
	{
		mDelaymS(500);
		//LED=!LED;

		//P3=~P3;
		//P1=0;
		IR_PIN = ! IR_PIN;
		LED_G = ! LED_G;
		LED_B = ! LED_B;
	 
		CH554UART0SendByte('R');
		CH554UART0SendByte('S');
    }

}

main()
{
	//led_main();
	hid_main();
	//composite_main();
	return 0;
}


