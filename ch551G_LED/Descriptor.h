#include "./CH554.h"
#include "./Debug.h"

#ifndef _descriptor_h_
#define _descriptor_h_

//定义标准的设备描述符结构
typedef struct _DEVICE_DCESCRIPTOR_STRUCT
{
	UINT8	blength;                               //设备描述符的字节数大小
	UINT8	bDescriptorType;	                   //设备描述符类型编号
	UINT16	bcdUSB;                                //USB版本号
	UINT8	bDeviceClass;                          //USB分配的设备类代码
	UINT8	bDeviceSubClass;                       //USB分配的子类代码
	UINT8	bDeviceProtocol;                       //USB分配的设备协议代码
	UINT8	bMaxPacketSize0;                       //端点0的最大包大小
	UINT16	idVendor;                              //厂商编号
	UINT16	idProduct;                             //产品编号
	UINT16	bcdDevice;                             //设备出厂编号
	UINT8	iManufacturer;                         //设备厂商字符串的索引
	UINT8	iProduct;                              //描述产品字符串的索引
	UINT8	iSerialNumber;                         //描述设备序列号字符串的索引
	UINT8	bNumConfigurations;                    //可能的配置数量
}DEVICE_DESCRIPTOR_STRUCT, *pDEVICE_DESCRIPTOR_STRUCT;


//定义标准的配置描述符结构
typedef struct _CONFIGURATION_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //配置描述符的字节数大小
	UINT8	bDescriptorType;                       //配置描述符类型编号
	UINT16	wTotalLength;                          //此配置返回的所有数据大小
	UINT8	bNumInterfaces;                        //此配置所支持的接口数量
	UINT8	bConfigurationValue;                   //Set_Configuration命令所需要的参数值
	UINT8	iConfiguration;                        //描述该配置的字符串的索引值
	UINT8	bmAttributes;                          //供电模式的选择
	UINT8	MaxPower;                              //设备从总线提取的最大电流
}CONFIGURATION_DESCRIPTOR_STRUCT, *pCONFIGURATION_DESCRIPTOR_STRUCT;


//定义标准的接口描述符结构
typedef struct _INTERFACE_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //接口描述符的字节数大小
	UINT8	bDescriptorType;                       //接口描述符的类型编号
	UINT8	bInterfaceNumber;                      //该接口的编号
	UINT8	bAlternateSetting;                     //备用的接口描述符编号
	UINT8	bNumEndpoints;                         //该接口使用的端点数，不包括端点0
	UINT8	bInterfaceClass;                       //接口类型
	UINT8	bInterfaceSubClass;                    //接口子类型
	UINT8	bInterfaceProtocol;                    //接口遵循的协议
	UINT8	iInterface;                            //描述该接口的字符串索引值
}INTERFACE_DESCRIPTOR_STRUCT, *pINTERFACE_DESCRIPTOR_STRUCT;


//定义标准的端点描述符结构
typedef struct _ENDPOINT_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                                //端点描述符字节数大小
	UINT8	bDescriptorType;                       //端点描述符类型编号
	UINT8	bEndpointAddress;                      //端点地址及输入输出属性
	UINT8	bmAttributes;                          //端点的传输类型属性
	UINT16	wMaxPacketSize;                        //端点收、发的最大包大小
	UINT8	bInterval;                             //主机查询端点的时间间隔
}ENDPOINT_DESCRIPTOR_STRUCT, * pENDPOINT_DESCRIPTOR_STRUCT;


//HID描述符中的下级描述符号
typedef struct _HID_SUB_DESCRIPTOR_STRUCT
{
	UINT8	bDescriptorType;    //下级描述符的类型编号
	UINT16	wDescriptorLength;  //下级描述符的长度
}HID_SUB_DESCRIPTOR_STRUCT,*pHID_SUB_DESCRIPTOR_STRUCT;

#define NUM_SUB_DESCRIPTORS 1

//定义HID描述符结构
typedef struct _HID_DESCRIPTOR_STRUCT
{
	UINT8	bLength;
	UINT8	bDescriptorType;
	UINT16	bcdHID;
	UINT8	bCountryCode;
	UINT8	bNumDescriptors;
	HID_SUB_DESCRIPTOR_STRUCT	HidSubDescriptors[NUM_SUB_DESCRIPTORS]; 
}HID_DESCRIPTOR_STRUCT,*pHID_DESCRIPTOR_STRUCT;	


//一个字符串描述符结构
typedef struct _STRING_DESCRIPTOR_STRUCT
{
	UINT8	bLength;							//字符串描述符的字节数大小
	UINT8	bDescriptorType;					//字符串描述符类型编号
	UINT8	StringDesriptor[36];					//UNICODE编码的字符串
}STRING_DESCRIPTOR_STRUCT, *pSTRING_DESCRIPTOR_STRUCT;

#define ENDPOINT_NUMBER 2
//定义配置，接口，端点等描述符结构体
typedef struct _USB_DESCRIPTOR_STRUCT
{
	CONFIGURATION_DESCRIPTOR_STRUCT	configuration_descriptor;
	INTERFACE_DESCRIPTOR_STRUCT		interface_descriptor;
	HID_DESCRIPTOR_STRUCT			hid_descriptor;
	ENDPOINT_DESCRIPTOR_STRUCT		endpoint_descriptor[ENDPOINT_NUMBER];
}USB_DESCRIPTOR_STRUCT, *pUSB_DESCRIPTOR_STRUCT;

//*******************************************************
// USB 设备请求类型掩码 masks 
//*******************************************************
//(1) 接收方: D4...D0 
#define USB_RECIPIENT            0x1F
#define USB_RECIPIENT_DEVICE     0x00
#define USB_RECIPIENT_INTERFACE  0x01
#define USB_RECIPIENT_ENDPOINT   0x02

//(2) 类型: D6...D5
#define USB_REQUEST_TYPE		 0x60
#define USB_STANDARD_REQUEST     0x00
#define USB_CLASS_REQUEST        0x20
#define USB_VENDOR_REQUEST       0x40

//(3) 数据传输方向: D7
#define USB_ENDPOINT_DIRECTION_MASK   0x80

//地址掩码
#define DEVICE_ADDRESS_MASK      0x7F

//(4) USB 请求?#define USB_REQUEST_MASK         (UINT8)0x0F

//(5) USB端点方向掩码
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//*******************************************************
// USB 设备描述符类型
//*******************************************************
#define USB_DEVICE_DESCRIPTOR                0x01  //设备描述符
#define USB_CONFIGURATION_DESCRIPTOR         0x02  //配置描述符
#define USB_STRING_DESCRIPTOR                0x03  //字符串描述符
#define USB_INTERFACE_DESCRIPTOR             0x04  //接口描述符
#define USB_ENDPOINT_DESCRIPTOR              0x05  //端点描述符
#define USB_HID_DESCRIPTOR					 0x21  //HID描述符
#define USB_REPORT_DESCRIPTOR                0x22  //报告描述符
#define USB_PHYSICAL_DESCRIPTOR              0x23  //物理描述符

//*******************************************************
// 下面为基于USB 1.0规范的正确值
//*******************************************************
#define USB_REQUEST_GET_STATUS                    0x00
#define USB_REQUEST_CLEAR_FEATURE                 0x01
#define USB_REQUEST_SET_FEATURE                   0x03
#define USB_REQUEST_SET_ADDRESS                   0x05
#define USB_REQUEST_GET_DESCRIPTOR                0x06
#define USB_REQUEST_SET_DESCRIPTOR                0x07
#define USB_REQUEST_GET_CONFIGURATION             0x08
#define USB_REQUEST_SET_CONFIGURATION             0x09
#define USB_REQUEST_GET_INTERFACE                 0x0A
#define USB_REQUEST_SET_INTERFACE                 0x0B
#define USB_REQUEST_SYNC_FRAME                    0x0C

//******************************************************
//** USB 特性选择符
//******************************************************
#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005

//******************************************************
//** USB 配置描述符
//******************************************************
#define USB_CONFIG_POWERED_MASK                   0xc0
#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

#define BUS_POWERED                           0x80
#define SELF_POWERED                          0x40
#define REMOTE_WAKEUP                         0x20

//******************************************************
//** USB 端点描述符结构体: 端点传输属性
//******************************************************
#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03

/*******************************************************
** USB  设备类
********************************************************/
#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF
#define USB_CLASS_CODE_TEST_CLASS_DEVICE	0xdc

/*****   D12 接口描述符中: 除端点0的端点索引数目  ******/
//#define ENDPOINT_NUMBER	4  //已在此文件前面定义

//(1) 端点0最大分组大小
#define EP0_TX_FIFO_SIZE   16
#define EP0_RX_FIFO_SIZE   16
#define EP0_PACKET_SIZE    16

//(2) 端点1 接收或发送的最大信息包大小
#define EP1_TX_FIFO_SIZE   16
#define EP1_RX_FIFO_SIZE   16
#define EP1_PACKET_SIZE    16

//(3) 端点1 接收或发送的最大信息包大小
#define EP2_TX_FIFO_SIZE   64
#define EP2_RX_FIFO_SIZE   64
#define EP2_PACKET_SIZE    64

//端点号定义
#define CONTROL_POINT_OUT        0x00
#define CONTROL_POINT_IN         0x80
#define POINT_1_OUT              0x01
#define POINT_1_IN               0x81
#define MAIN_POINT_OUT           0x02
#define MAIN_POINT_IN            0x82

extern code DEVICE_DESCRIPTOR_STRUCT device_descriptor;
extern code USB_DESCRIPTOR_STRUCT usb_descriptor;
extern code UINT8 LANGUAGE_ID[4];
extern code UINT8 device_serial_number[22];
extern code UINT8 ManufacturerString[80];
extern code UINT8 ProducterString[96];

#endif
