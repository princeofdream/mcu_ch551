
/********************************** (C) COPYRIGHT *******************************
* File Name		  : Main.C
* Author			 : WCH
* Version			: V1.0
* Date			   : 2017/01/20
* Description		: CH554 PWM初始化，占空比设置，PWM默认电平设置
					   支持中断方式修改PWM占空比
*******************************************************************************/

#if 0
#include "CH552.h"
#include "Debug.h"
#include "PWM.h"
#include "stdio.h"

#pragma  NOAREGS

void pwm_main( )
{
	printf("start ...\n");

	P1_MOD_OC &= ~(bPWM1 | bPWM2);											 //设置PWM引脚为推挽输出
	P1_DIR_PU |= bPWM1 | bPWM2;

	//PWM时钟配置	，4分频
	/* SetPWMClk(4); */
	ForceClearPWMFIFO( );													  //强制清除PWM FIFO和COUNT
	CancleClearPWMFIFO( );													 //取消清除PWM FIFO和COUNT
	PWM1OutEnable( );														  //允许PWM1输出
	PWM2OutEnable( );														  //允许PWM2输出

	PWM1OutPolarHighAct( );													//PWM1输出默认低，高有效
	PWM2OutPolarLowAct( );													 //PWM2输出默认高，低有效

#if PWM_INTERRUPT
	PWMInterruptEnable();
	EA = 1;
	SetPWM1Dat(0x10);
	SetPWM2Dat(0x40);
	while(1);
#endif
	//占空比0x10/0xFF
	SetPWM1Dat(0x10);
	SetPWM2Dat(0x40);
	while(1)
	{
		if(PWM_CTRL&bPWM_IF_END)
		{
			PWM_CTRL |= bPWM_IF_END;											   //清除PWM中断
			SetPWM1Dat(0x10);
			SetPWM2Dat(0x40);
#ifdef DE_PRINTF
			printf("PWM_CYC_END  %02X\n",(UINT16)PWM_CTRL);
#endif
		}
	}
}
#endif
