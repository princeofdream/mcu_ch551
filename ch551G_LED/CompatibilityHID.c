
/********************************** (C) COPYRIGHT *******************************
* File Name          :CompatibilityHID.C
* Author             : WCH
* Version            : V1.1
* Date               : 2017/07/05
* Description        : CH554模拟HID兼容设备，支持中断上下传，支持设置全速，低速 
*******************************************************************************/
#if 1

#include "CompatibilityHID.h"

#define Fullspeed               1
#ifdef  Fullspeed
#define THIS_ENDP0_SIZE         64
#else
#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#endif
UINT16X  Ep0Buffer[8>(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;        //端点0 OUT&IN缓冲区，必须是偶地址
UINT16X  Ep2Buffer[128>(2*MAX_PACKET_SIZE+4)?128:(2*MAX_PACKET_SIZE+4)] _at_ 0x0044;//端点2 IN&OUT缓冲区,必须是偶地址
UINT8   SetupReq,SetupLen,Ready,UsbConfig;
UINT16X  GET_HID_WRITE = 0;
PUINT8  pDescr;                                                                    //USB配置标志
USB_SETUP_REQ   SetupReqBuf;                                                       //暂存Setup包
#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)

sbit Ep2InKey = P1^5;                                                              //K1按键
#pragma  NOAREGS
UINT16X get_buf[THIS_ENDP0_SIZE];

//#define SWAP16(x)  ((((UINT16)(x))<<8)|(((UINT16)(x))>>8))


/*设备描述符*/
UINT8C DevDesc[18] =
{
	0x12,					//sizeof(DevDesc)	设备描述符字节数大小
	USB_DEVICE_DESCRIPTOR,	//类型编号
	0x10,0x01,				//USB版本号 USB 1.1
	0x00,					//USB分配的设备类code
	0x00,					//USB分配的子类code
	0x00,					//USB分配的设备协议code
	THIS_ENDP0_SIZE,		//Endpoint 0 MAX package size
	0x31,0x51,				//VID
	0x07,0x20,				//PID
	0x00,0x00,				//出厂编号
	0x01,0x02,
	0x03,0x01
};

UINT8C CfgDesc[41] =
{
	//配置描述符
	//{
		0x09,									//描述符字节数大小
		USB_CONFIGURATION_DESCRIPTOR,
		//类型编号
		0x29,0x00,0x01,0x01,0x04,0xA0,0x23,
	//}
	//接口描述符
	//{
		0x09,							//描述符字节数大小
		USB_INTERFACE_DESCRIPTOR,		//类型编号
		0x00,0x00,0x02,0x03,0x00,0x00,0x05,
	//}
	//HID类描述符
	//{
		0x09,							//描述符字节数大小
		USB_HID_DESCRIPTOR,				//类型编号
		0x01,0x01,						//协议版本号 1.1 org //0x00,0x01,
		0x00,
		NUM_SUB_DESCRIPTORS,			//下级描述符的数量 1
		//{
			USB_REPORT_DESCRIPTOR,		//
			//0x22,0x00,					// sizeof(HIDRepDesc)  34==> 0x0022(34)// 47==> 0x002F
			0x2F,0x00,				// sizeof(HIDRepDesc)  34==> 0x0022(34)// 47==> 0x002F
		//}
	//}
	//端点描述符
	//{
#if 1
		//端点1输出
		//{
			0x07,							//描述符字节数大小
			USB_ENDPOINT_DESCRIPTOR,		//类型编号
			0x02,
			USB_ENDPOINT_TYPE_INTERRUPT,
			THIS_ENDP0_SIZE,0x00,
			0x18,
		//}
		//端点1输入
		//{
			0x07,							//描述符字节数大小
			USB_ENDPOINT_DESCRIPTOR,		//类型编号
			0x82,
			USB_ENDPOINT_TYPE_INTERRUPT,
			THIS_ENDP0_SIZE,
			0x00,
			0x18
		//}
#else
		//端点1输出
		//{
			0x07,							//描述符字节数大小
			USB_ENDPOINT_DESCRIPTOR,		//类型编号
			POINT_1_OUT,					//端点号，主输出端点 0x01
			USB_ENDPOINT_TYPE_INTERRUPT,	//传输类型，中断传输
			THIS_ENDP0_SIZE, 0x00,			//Endpoint max package size 0064 ==> 0x0040 swap --> 40 00
			0x18,							//中断扫描时间 0x0A ==> 10ms 0x18 ==> 24ms
		//}
		//端点1输入
		//{
			0x07,							//描述符字节数大小
			USB_ENDPOINT_DESCRIPTOR,		//类型编号
			POINT_1_IN,						//端点号，主输入端点 0x81
			USB_ENDPOINT_TYPE_INTERRUPT,
			THIS_ENDP0_SIZE,0x00,
			0x18
		//}
#endif
	//}
};

//字符串描述符
//{
UINT8X UserEp2Buf[64];                                            //用户数据定义
// 语言描述符
// 厂家信息
// 产品信息
// 序列号
//{ size, 类型编号, ...}
UINT8C	LANGUAGE_ID[] 			= { 0x04, USB_STRING_DESCRIPTOR, 0x09, 0x04 };
UINT8C	ManufacturerString[] 	= { 0x0E, USB_STRING_DESCRIPTOR, 'v', 0, 's', 0, 't', 0, 'e', 0, 'c', 0, 'g', 0 };
UINT8C	ProducterString[] 		= { 0x0C, USB_STRING_DESCRIPTOR, 'C', 0, 'H', 0, '5', 0, '5', 0, '1', 0 };
UINT8C	device_serial_number[]	= { 0x0C, USB_STRING_DESCRIPTOR, '1', 0, '3', 0, '5', 0, '7', 0, '9', 0 };

/*HID类报表描述符*/
UINT8C HIDRepDesc[ 47 ] =
{
	0x06, 0x00, 0xff,
	0x09, 0x01,
    0xA1, 0x01,                                                   //集合开始
	
	// The Input report < total 13 >
	0x09, 0x02,                                                   //Usage Page  用法
	0x15, 0x00,                                                   //Logical  Minimun
	0x26, 0xFF, 0x00,                                              //Logical  Maximun
	0x75, 0x08,                                                   //Report Size
	0x95, THIS_ENDP0_SIZE,                                        //Report Counet
	0x81, 0x06,                                                   //Input
	
	// The Output report < total 13 >
	0x09, 0x02,                                                   //Usage Page  用法
	0x15, 0x00,                                                   //Logical  Minimun
	0x26, 0xFF,0x00,                                              //Logical  Maximun
	0x75, 0x08,                                                   //Report Size
	0x95, THIS_ENDP0_SIZE,                                        //Report Counet
	0x91, 0x06,                                                   //Output

	// The Feature report <total 13 >
	///
	0x09, 0x05,     	// Usage ID - vendor defined XXX
    0x15, 0x00,     	// Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x75, 0x08,			// Report Size (8 bits)
    0x95, 0x02, 		// Report Count (2 fields) XXX
    0xB1, 0x02,     	// Feature (Data, Variable, Absolute)   XXX
	///
	
	0xC0
};

char code HidUseReportDescriptor[47] = 
{
	// total 7
  	0x06, 0xA0, 0xFF,	// Usage page (vendor defined) XXX
  	0x09, 0x01,			// Usage ID (vendor defined)
  	0xA1, 0x01,			// Collection (application)

	// The Input report < total 13 >
	0x09, 0x03,     	// Usage ID - vendor defined XXX
    0x15, 0x00,     	// Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255) XXX
    0x75, 0x08,     	// Report Size (8 bits)
    0x95, 0x02,     	// Report Count (2 fields) XXX
    0x81, 0x02,     	// Input (Data, Variable, Absolute)  XXX

	// The Output report < total 13 >
    0x09, 0x04,     	// Usage ID - vendor defined
    0x15, 0x00,     	// Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x75, 0x08,     	// Report Size (8 bits)
    0x95, 0x02,     	// Report Count (2 fields)
    0x91, 0x02,      	// Output (Data, Variable, Absolute)  

	// The Feature report <total 13 >
	///
    0x09, 0x05,     	// Usage ID - vendor defined XXX
    0x15, 0x00,     	// Logical Minimum (0)
    0x26, 0xFF, 0x00,   // Logical Maximum (255)
    0x75, 0x08,			// Report Size (8 bits)
    0x95, 0x02, 		// Report Count (2 fields) XXX
    0xB1, 0x02,     	// Feature (Data, Variable, Absolute)   XXX
	///

 	0xC0	// end collection
};

void hid_write_handler(UINT16X *hid_buf,UINT16X len)
{
	UINT16X protocol = 0;
#if 0
	UINT16 vendor_code = 0;
	UINT16 keycode = 0;
	UINT16 repeat = 0;
	UINT16X i=0;

	vendor_code = hid_buf[1];
	keycode     = hid_buf[2];
	repeat      = hid_buf[3];
#endif
	protocol    = hid_buf[0];
	/* printf("\nhid_buf[0]:0x%x, hid_buf[1]:0x%x, hid_buf[2]:0x%x, hid_buf[3]:0x%x, \n",hid_buf[0],hid_buf[1],hid_buf[2],hid_buf[3]); */

	printf("protocol: 0x%04x\n",protocol);
	if((protocol & IR_PROTOCOL_NEC) == IR_PROTOCOL_NEC)
	{
		printf("nec protocol\n");
		nec_init(hid_buf);
	}
#if 0
	else if((protocol & IR_PROTOCOL_RC5) == IR_PROTOCOL_RC5)
		rc5_init(protocol);
	else if((protocol & IR_PROTOCOL_RC6) == IR_PROTOCOL_RC6)
		rc6_init(protocol);
#endif
	else
		return ;

	//have pwm in pluse
	if((protocol & IR_WITH_PWM_MASK) != NULL )
	{
		//use timer 0 to generate pwm
		nec_enable_timer_interrupt(0);
	}
	nec_enable_timer_interrupt(2);
	/* nec_emmit(vendor_code, keycode); */
}


/*******************************************************************************
* Function Name  : USBDeviceInit()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceInit()
{
	IE_USB = 0;
	USB_CTRL = 0x00;                                                           // 先设定USB设备模式
#ifndef Fullspeed
	UDEV_CTRL |= bUD_LOW_SPEED;                                                //选择低速1.5M模式
#else
	UDEV_CTRL &= ~bUD_LOW_SPEED;                                               //选择全速12M模式，默认方式
#endif
	UEP2_DMA = Ep2Buffer;                                                      //端点2数据传输地址
	UEP2_3_MOD |= bUEP2_TX_EN;                                                 //端点2发送使能
	UEP2_3_MOD |= bUEP2_RX_EN;                                                 //端点2接收使能
	UEP2_3_MOD &= ~bUEP2_BUF_MOD;                                              //端点2收发各64字节缓冲区
	UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
	UEP0_DMA = Ep0Buffer;                                                      //端点0数据传输地址
	UEP4_1_MOD &= ~(bUEP4_RX_EN | bUEP4_TX_EN);                                //端点0单64字节收发缓冲区
	UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT事务返回ACK，IN事务返回NAK

	USB_DEV_AD = 0x00;
	UDEV_CTRL = bUD_PD_DIS;                                                    // 禁止DP/DM下拉电阻
	USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                      // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
	UDEV_CTRL |= bUD_PORT_EN;                                                  // 允许USB端口
	USB_INT_FG = 0xFF;                                                         // 清中断标志
	USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
	IE_USB = 1;
}

/*******************************************************************************
* Function Name  : Enp2BlukIn()
* Description    : USB设备模式端点2的批量上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2BlukIn( )
{
	//printf( "Enp2BlukIn\n" );
	memcpy( Ep2Buffer+MAX_PACKET_SIZE, UserEp2Buf, sizeof(UserEp2Buf));        //加载上传数据
	UEP2_T_LEN = THIS_ENDP0_SIZE;                                              //上传最大包长度
	UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                  //有数据时上传数据并应答ACK
	while(UEP2_CTRL&UEP_T_RES_ACK);                                            //等待传输完成
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB中断处理函数
*******************************************************************************/
void    DeviceInterrupt( void ) interrupt INT_NO_USB using 1                    //USB中断服务程序,使用寄存器组1
{
	UINT8 len,i;
	if(UIF_TRANSFER)                                                            //USB传输完成标志
	{
		switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
		{
		case UIS_TOKEN_IN | 2:                                                  //endpoint 2# 端点批量上传
			UEP2_T_LEN = 0;                                                    //预使用发送长度一定要清空
			//            UEP1_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
			UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
			break;
		//hid_write
		case UIS_TOKEN_OUT | 2:                                                 //endpoint 2# 端点批量下传
			if ( U_TOG_OK )                                                     // 不同步的数据包将丢弃
			{
				len = USB_RX_LEN;                                               //接收数据长度，数据从Ep2Buffer首地址开始存放
				for ( i = 0; i < len; i ++ )
				{
					Ep2Buffer[MAX_PACKET_SIZE+i] = Ep2Buffer[i] ^ 0xFF;         // OUT数据取反到IN由计算机验证
				}
				UEP2_T_LEN = len;
				UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;       // 允许上传
				memset(get_buf,0x0,sizeof(get_buf));
#if 1
				for ( i = 0; i < len; i ++ )
				{
					get_buf[i] = Ep2Buffer[i];
					/* printf("0x%04x ",get_buf[i]); */
				}
				/* printf("\n"); */
				GET_HID_WRITE = len;
				LED_B = ~LED_B;
#else
				LED_B = ~LED_B;
				/* T0=~T0; */
#endif
			}
			break;
		case UIS_TOKEN_SETUP | 0:                                               //SETUP事务
			len = USB_RX_LEN;
			//printf("UIS_TOKEN_OUT");
			if(len == (sizeof(USB_SETUP_REQ)))
			{
				SetupLen = UsbSetupBuf->wLengthL;
				if(UsbSetupBuf->wLengthH || SetupLen > 0x7F )
				{
					SetupLen = 0x7F;                                             // 限制总长度
				}
				len = 0;                                                         // 默认为成功并且上传0长度
				SetupReq = UsbSetupBuf->bRequest;
				if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )/*HID类命令*/
				{
					//printf("HID request %x\n",SetupReq);
					switch( SetupReq )
					{
					case 0x01:                                                   //GetReport
						break;
					case 0x02:                                                   //GetIdle
						break;
					case 0x03:                                                   //GetProtocol
						break;
					case 0x09:                                                   //SetReport
						break;
					case 0x0A:                                                   //SetIdle
						break;
					case 0x0B:                                                   //SetProtocol
						break;
					default:
						len = 0xFF;  				                                   /*命令不支持*/
						break;
					}
				}
				else                                                             //标准请求
				{
					//printf("standar request 0x%x. \t",SetupReq);
					switch(SetupReq)                                             //请求码
					{
					case USB_GET_DESCRIPTOR:
						//printf("USB_GET_STATUS 0x%x, 0x%x\n", UsbSetupBuf->wValueH,UsbSetupBuf->wValueL);
						switch( UsbSetupBuf->wValueH )
						{
						case 1:                                          // 设备描述符
							pDescr = (PUINT8)( &DevDesc[0] );
							len = sizeof( DevDesc );
							break;
						case 2:                                          // 配置描述符
							pDescr = (PUINT8)( &CfgDesc[0] );
							len = sizeof( CfgDesc );
							break;
						case 0x22:                                               //报表描述符
							pDescr = HIDRepDesc;                                 //数据准备上传
							len = sizeof(HIDRepDesc);
							Ready = 1;                                           //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
							break;

						case 3:                                          // 字符串描述符
							switch( UsbSetupBuf->wValueL )
							{
							case 1:
								pDescr = (PUINT8)( &ManufacturerString[0] );
								len = sizeof( ManufacturerString );
								Ready = 1;
								break;
							case 2:
								pDescr = (PUINT8)( &ProducterString[0] );
								len = sizeof( ProducterString );
								break;
							case 0:
								pDescr = (PUINT8)( &LANGUAGE_ID[0] );
								len = sizeof( LANGUAGE_ID );
								break;
							case 3:
								pDescr = (PUINT8)( &device_serial_number[0] );
								len = sizeof( device_serial_number );
								break;
							default:
								len = 0xFF;                               // 不支持的字符串描述符
								break;
							}
							break;
						default:
							len = 0xFF;                                  // 不支持的描述符类型
							break;
						}

						if ( SetupLen > len )
						{
							SetupLen = len;            // 限制总长度
						}
						len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;  // 本次传输长度
						memcpy( Ep0Buffer, pDescr, len );                 /* 加载上传数据 */
						SetupLen -= len;
						pDescr += len;
						break;
					case USB_SET_ADDRESS:
						//printf("USB_SET_ADDRESS\n");
						SetupLen = UsbSetupBuf->wValueL;                         //暂存USB设备地址
						break;
					case USB_GET_CONFIGURATION:
						//printf("USB_GET_CONFIGURATION\n");
						Ep0Buffer[0] = UsbConfig;
						if ( SetupLen >= 1 )
						{
							len = 1;
						}
						break;
					case USB_SET_CONFIGURATION:
						//printf("USB_SET_CONFIGURATION\n");
						UsbConfig = UsbSetupBuf->wValueL;
						break;
					case 0x0A:
						break;
					case USB_CLEAR_FEATURE:                                      //Clear Feature
						//printf("USB_CLEAR_FEATURE\n");
						if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
						{
							//printf("UsbSetupBuf->wIndexL %x\n",UsbSetupBuf->wIndexL);
							switch( UsbSetupBuf->wIndexL )
							{
							case 0x82:
								UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
								break;
							case 0x81:
								UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
								break;
							case 0x02:
								UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
								break;
							default:
								len = 0xFF;                                       // 不支持的端点
								break;
							}
						}
						else
						{
							len = 0xFF;                                           // 不是端点不支持
						}
						break;
					case USB_SET_FEATURE:                                         /* Set Feature */
						if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )        /* 设置设备 */
						{
							if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
							{
								if( CfgDesc[ 7 ] & 0x20 )
								{
									/* 设置唤醒使能标志 */
								}
								else
								{
									len = 0xFF;                                    /* 操作失败 */
								}
							}
							else
							{
								len = 0xFF;                                        /* 操作失败 */
							}
						}
						else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )    /* 设置端点 */
						{
							if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
							{
								switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
								{
								case 0x82:
									UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;	//设置端点2 IN STALL
									break;
								case 0x02:
									UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;	//设置端点2 OUT Stall
									break;
								case 0x81:
									UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;	//设置端点1 IN STALL
									break;
								default:
									len = 0xFF;                                     // 操作失败
									break;
								}
							}
							else
							{
								len = 0xFF;                                         // 操作失败
							}
						}
						else
						{
							len = 0xFF;                                             // 操作失败
						}
						break;
					case USB_GET_STATUS:
						Ep0Buffer[0] = 0x00;
						Ep0Buffer[1] = 0x00;
						if ( SetupLen >= 2 )
						{
							len = 2;
						}
						else
						{
							len = SetupLen;
						}
						break;
					default:
						len = 0xff;                                                  //操作失败
						break;
					}
				}
			}
			else
			{
				len = 0xff;                                                          //包长度错误
			}
			if(len == 0xff)
			{
				SetupReq = 0xFF;
				UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
			}
			else if(len <= THIS_ENDP0_SIZE)                                         //上传数据或者状态阶段返回0长度包
			{
				UEP0_T_LEN = len;
				UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
			}
			else
			{
				UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
				UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
			}
			break;
		case UIS_TOKEN_IN | 0:                                                      //endpoint0 IN
			switch(SetupReq)
			{
			case USB_GET_DESCRIPTOR:
				len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;     //本次传输长度
				memcpy( Ep0Buffer, pDescr, len );                                   //加载上传数据
				SetupLen -= len;
				pDescr += len;
				UEP0_T_LEN = len;
				UEP0_CTRL ^= bUEP_T_TOG;                                            //同步标志位翻转
				break;
			case USB_SET_ADDRESS:
				USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
				UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
				break;
			default:
				UEP0_T_LEN = 0;                                                      //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
				UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
				break;
			}
			break;
		case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
			len = USB_RX_LEN;
			if(SetupReq == 0x09)
			{
				if(Ep0Buffer[0])
				{
					printf("Light on Num Lock LED!\n");
				}
				else if(Ep0Buffer[0] == 0)
				{
					printf("Light off Num Lock LED!\n");
				}
			}
			UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
			UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA0,返回应答ACK
			break;
		default:
			break;
		}
		UIF_TRANSFER = 0;                                                           //写0清空中断
	}

	if(UIF_BUS_RST)                                                                 //设备模式USB总线复位中断
	{
		UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
		UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
		UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
		USB_DEV_AD = 0x00;
		UIF_SUSPEND = 0;
		UIF_TRANSFER = 0;
		UIF_BUS_RST = 0;                                                             //清中断标志
	}
	if (UIF_SUSPEND)                                                                 //USB总线挂起/唤醒完成
	{
		UIF_SUSPEND = 0;
		if ( USB_MIS_ST & bUMS_SUSPEND )                                             //挂起
		{
#ifdef DE_PRINTF
			printf( "zz\n" );                                                          //睡眠状态
#endif
			//by JamesL
			Ready = 0;
			while ( XBUS_AUX & bUART0_TX )
			{
				;    //等待发送完成
			}
			SAFE_MOD = 0x55;
			SAFE_MOD = 0xAA;
			WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO;                                   //USB或者RXD0有信号时可被唤醒
			PCON |= PD;                                                               //睡眠
			SAFE_MOD = 0x55;
			SAFE_MOD = 0xAA;
			WAKE_CTRL = 0x00;
		}
	}
	else {                                                                             //意外的中断,不可能发生的情况
		USB_INT_FG = 0xFF;                                                             //清中断标志
		//printf("UnknownInt  N");
	}
}


void hid_main()
{
	UINT16X i;
#ifdef DE_PRINTF
	printf("start ...\n");
#endif
#if 0
	for(i=0; i<64; i++)                                                   //准备演示数据
	{
		UserEp2Buf[i] = i;
	}
	UserEp2Buf[59] = 'J';
	UserEp2Buf[60] = 'a';
	UserEp2Buf[61] = 'm';
	UserEp2Buf[62] = 'e';
	UserEp2Buf[63] = 's';
#endif

	USBDeviceInit();                                                      //USB设备模式初始化
	EA = 1;                                                               //允许单片机中断
	UEP1_T_LEN = 0;                                                       //预使用发送长度一定要清空
	UEP2_T_LEN = 0;                                                       //预使用发送长度一定要清空
	Ready = 0;

	while(1)
	{
		/* if(Ready && (Ep2InKey==0)) */
		if(Ready)
		{
			//print("sent file");
			Enp2BlukIn( );
			mDelaymS( 100 );
			//Ready = 0;
		}
		if (GET_HID_WRITE)
		{
#if 1
			for ( i = 0; i < GET_HID_WRITE; i ++ )
			{
				printf("0x%04x ",get_buf[i]);
				switch (i) {
					case 8-1:
					case 24-1:
					case 40-1:
					case 56-1:
						printf("\t");
						break;
					case 16-1:
					case 32-1:
					case 48-1:
					case 64-1:
						printf("\n");
						break;
					default:
						break;
				}
			}
			printf("\n");
			printf("i: 0x%x, Len: 0x%x\n",i, GET_HID_WRITE);
#endif
			//hid_write_handler(get_buf,GET_HID_WRITE);
			GET_HID_WRITE = 0;
		}

		/* mDelaymS( 100 );                                                   //模拟单片机做其它事 */
		mDelayuS( 50 );                                                   //模拟单片机做其它事
		/* printf("loop......\n"); */
    }
}
#endif
