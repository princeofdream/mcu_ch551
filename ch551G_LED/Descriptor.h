//#include "mytype.h"
//#include "hiduse.h"

#include "./CH554.H"
#include "./Debug.H"

#ifndef _descriptor_h_
#define _descriptor_h_

//¶¨Òå±ê×¼µÄÉè±¸ÃèÊö·û½á¹¹
typedef struct _DEVICE_DCESCRIPTOR_STRUCT
{
	UINT8	blength;                               //Éè±¸ÃèÊö·ûµÄ×Ö½ÚÊý´óÐ¡
	UINT8	bDescriptorType;	                   //Éè±¸ÃèÊö·ûÀàÐÍ±àºÅ
	UINT16	bcdUSB;                                //USB°æ±¾ºÅ
	UINT8	bDeviceClass;                          //USB·ÖÅäµÄÉè±¸Àà´úÂë
	UINT8	bDeviceSubClass;                       //USB·ÖÅäµÄ×ÓÀà´úÂë
	UINT8	bDeviceProtocol;                       //USB·ÖÅäµÄÉè±¸Ð­Òé´úÂë
	UINT8	bMaxPacketSize0;                       //¶Ëµã0µÄ×î´ó°ü´óÐ¡
	UINT16	idVendor;                              //³§ÉÌ±àºÅ
	UINT16	idProduct;                             //²úÆ·±àºÅ
	UINT16	bcdDevice;                             //Éè±¸³ö³§±àºÅ
	UINT8	iManufacturer;                         //Éè±¸³§ÉÌ×Ö·û´®µÄË÷Òý
	UINT8	iProduct;                              //ÃèÊö²úÆ·×Ö·û´®µÄË÷Òý
	UINT8	iSerialNumber;                         //ÃèÊöÉè±¸ÐòÁÐºÅ×Ö·û´®µÄË÷Òý
	UINT8	bNumConfigurations;                    //¿ÉÄÜµÄÅäÖÃÊýÁ¿
}DEVICE_DESCRIPTOR_STRUCT, *pDEVICE_DESCRIPTOR_STRUCT;


//¶¨Òå±ê×¼µÄÅäÖÃÃèÊö·û½á¹¹
typedef struct _CONFIGURATION_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //ÅäÖÃÃèÊö·ûµÄ×Ö½ÚÊý´óÐ¡
	UINT8	bDescriptorType;                       //ÅäÖÃÃèÊö·ûÀàÐÍ±àºÅ
	UINT16	wTotalLength;                          //´ËÅäÖÃ·µ»ØµÄËùÓÐÊý¾Ý´óÐ¡
	UINT8	bNumInterfaces;                        //´ËÅäÖÃËùÖ§³ÖµÄ½Ó¿ÚÊýÁ¿
	UINT8	bConfigurationValue;                   //Set_ConfigurationÃüÁîËùÐèÒªµÄ²ÎÊýÖµ
	UINT8	iConfiguration;                        //ÃèÊö¸ÃÅäÖÃµÄ×Ö·û´®µÄË÷ÒýÖµ
	UINT8	bmAttributes;                          //¹©µçÄ£Ê½µÄÑ¡Ôñ
	UINT8	MaxPower;                              //Éè±¸´Ó×ÜÏßÌáÈ¡µÄ×î´óµçÁ÷
}CONFIGURATION_DESCRIPTOR_STRUCT, *pCONFIGURATION_DESCRIPTOR_STRUCT;


//¶¨Òå±ê×¼µÄ½Ó¿ÚÃèÊö·û½á¹¹
typedef struct _INTERFACE_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //½Ó¿ÚÃèÊö·ûµÄ×Ö½ÚÊý´óÐ¡
	UINT8	bDescriptorType;                       //½Ó¿ÚÃèÊö·ûµÄÀàÐÍ±àºÅ
	UINT8	bInterfaceNumber;                      //¸Ã½Ó¿ÚµÄ±àºÅ
	UINT8	bAlternateSetting;                     //±¸ÓÃµÄ½Ó¿ÚÃèÊö·û±àºÅ
	UINT8	bNumEndpoints;                         //¸Ã½Ó¿ÚÊ¹ÓÃµÄ¶ËµãÊý£¬²»°üÀ¨¶Ëµã0
	UINT8	bInterfaceClass;                       //½Ó¿ÚÀàÐÍ
	UINT8	bInterfaceSubClass;                    //½Ó¿Ú×ÓÀàÐÍ
	UINT8	bInterfaceProtocol;                    //½Ó¿Ú×ñÑ­µÄÐ­Òé
	UINT8	iInterface;                            //ÃèÊö¸Ã½Ó¿ÚµÄ×Ö·û´®Ë÷ÒýÖµ
}INTERFACE_DESCRIPTOR_STRUCT, *pINTERFACE_DESCRIPTOR_STRUCT;


//¶¨Òå±ê×¼µÄ¶ËµãÃèÊö·û½á¹¹
typedef struct _ENDPOINT_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                                //¶ËµãÃèÊö·û×Ö½ÚÊý´óÐ¡
	UINT8	bDescriptorType;                       //¶ËµãÃèÊö·ûÀàÐÍ±àºÅ
	UINT8	bEndpointAddress;                      //¶ËµãµØÖ·¼°ÊäÈëÊä³öÊôÐÔ
	UINT8	bmAttributes;                          //¶ËµãµÄ´«ÊäÀàÐÍÊôÐÔ
	UINT16	wMaxPacketSize;                        //¶ËµãÊÕ¡¢·¢µÄ×î´ó°ü´óÐ¡
	UINT8	bInterval;                             //Ö÷»ú²éÑ¯¶ËµãµÄÊ±¼ä¼ä¸ô
}ENDPOINT_DESCRIPTOR_STRUCT, * pENDPOINT_DESCRIPTOR_STRUCT;


//HIDÃèÊö·ûÖÐµÄÏÂ¼¶ÃèÊö·ûºÅ
typedef struct _HID_SUB_DESCRIPTOR_STRUCT
{
	UINT8	bDescriptorType;    //ÏÂ¼¶ÃèÊö·ûµÄÀàÐÍ±àºÅ
	UINT16	wDescriptorLength;  //ÏÂ¼¶ÃèÊö·ûµÄ³¤¶È
}HID_SUB_DESCRIPTOR_STRUCT,*pHID_SUB_DESCRIPTOR_STRUCT;

#define NUM_SUB_DESCRIPTORS 1

//¶¨ÒåHIDÃèÊö·û½á¹¹
typedef struct _HID_DESCRIPTOR_STRUCT
{
	UINT8	bLength;
	UINT8	bDescriptorType;
	UINT16	bcdHID;
	UINT8	bCountryCode;
	UINT8	bNumDescriptors;
	HID_SUB_DESCRIPTOR_STRUCT	HidSubDescriptors[NUM_SUB_DESCRIPTORS]; 
}HID_DESCRIPTOR_STRUCT,*pHID_DESCRIPTOR_STRUCT;	


//Ò»¸ö×Ö·û´®ÃèÊö·û½á¹¹
typedef struct _STRING_DESCRIPTOR_STRUCT
{
	UINT8	bLength;							//×Ö·û´®ÃèÊö·ûµÄ×Ö½ÚÊý´óÐ¡
	UINT8	bDescriptorType;					//×Ö·û´®ÃèÊö·ûÀàÐÍ±àºÅ
	UINT8	StringDesriptor[36];					//UNICODE±àÂëµÄ×Ö·û´®
}STRING_DESCRIPTOR_STRUCT, *pSTRING_DESCRIPTOR_STRUCT;

#define ENDPOINT_NUMBER 2
//¶¨ÒåÅäÖÃ£¬½Ó¿Ú£¬¶ËµãµÈÃèÊö·û½á¹¹Ìå
typedef struct _USB_DESCRIPTOR_STRUCT
{
	CONFIGURATION_DESCRIPTOR_STRUCT	configuration_descriptor;
	INTERFACE_DESCRIPTOR_STRUCT		interface_descriptor;
	HID_DESCRIPTOR_STRUCT			hid_descriptor;
	ENDPOINT_DESCRIPTOR_STRUCT		endpoint_descriptor[ENDPOINT_NUMBER];
}USB_DESCRIPTOR_STRUCT, *pUSB_DESCRIPTOR_STRUCT;

//*******************************************************
// USB Éè±¸ÇëÇóÀàÐÍÑÚÂë masks 
//*******************************************************
//(1) ½ÓÊÕ·½: D4...D0 
#define USB_RECIPIENT            0x1F
#define USB_RECIPIENT_DEVICE     0x00
#define USB_RECIPIENT_INTERFACE  0x01
#define USB_RECIPIENT_ENDPOINT   0x02

//(2) ÀàÐÍ: D6...D5
#define USB_REQUEST_TYPE		 0x60
#define USB_STANDARD_REQUEST     0x00
#define USB_CLASS_REQUEST        0x20
#define USB_VENDOR_REQUEST       0x40

//(3) Êý¾Ý´«Êä·½Ïò: D7
#define USB_ENDPOINT_DIRECTION_MASK   0x80

//µØÖ·ÑÚÂë
#define DEVICE_ADDRESS_MASK      0x7F

//(4) USB ÇëÇó©
#define USB_REQUEST_MASK         (UINT8)0x0F

//(5) USB¶Ëµã·½ÏòÑÚÂë
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//*******************************************************
// USB Éè±¸ÃèÊö·ûÀàÐÍ
//*******************************************************
#define USB_DEVICE_DESCRIPTOR                0x01  //Éè±¸ÃèÊö·û
#define USB_CONFIGURATION_DESCRIPTOR         0x02  //ÅäÖÃÃèÊö·û
#define USB_STRING_DESCRIPTOR                0x03  //×Ö·û´®ÃèÊö·û
#define USB_INTERFACE_DESCRIPTOR             0x04  //½Ó¿ÚÃèÊö·û
#define USB_ENDPOINT_DESCRIPTOR              0x05  //¶ËµãÃèÊö·û
#define USB_HID_DESCRIPTOR					 0x21  //HIDÃèÊö·û
#define USB_REPORT_DESCRIPTOR                0x22  //±¨¸æÃèÊö·û
#define USB_PHYSICAL_DESCRIPTOR              0x23  //ÎïÀíÃèÊö·û

//*******************************************************
// ÏÂÃæÎª»ùÓÚUSB 1.0¹æ·¶µÄÕýÈ·Öµ
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
//** USB ÌØÐÔÑ¡Ôñ·û
//******************************************************
#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005

//******************************************************
//** USB ÅäÖÃÃèÊö·û
//******************************************************
#define USB_CONFIG_POWERED_MASK                   0xc0
#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

#define BUS_POWERED                           0x80
#define SELF_POWERED                          0x40
#define REMOTE_WAKEUP                         0x20

//******************************************************
//** USB ¶ËµãÃèÊö·û½á¹¹Ìå: ¶Ëµã´«ÊäÊôÐÔ
//******************************************************
#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03

/*******************************************************
** USB  Éè±¸Àà
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

/*****   D12 ½Ó¿ÚÃèÊö·ûÖÐ: ³ý¶Ëµã0µÄ¶ËµãË÷ÒýÊýÄ¿  ******/
//#define ENDPOINT_NUMBER	4  //ÒÑÔÚ´ËÎÄ¼þÇ°Ãæ¶¨Òå

//(1) ¶Ëµã0×î´ó·Ö×é´óÐ¡
#define EP0_TX_FIFO_SIZE   16
#define EP0_RX_FIFO_SIZE   16
#define EP0_PACKET_SIZE    16

//(2) ¶Ëµã1 ½ÓÊÕ»ò·¢ËÍµÄ×î´óÐÅÏ¢°ü´óÐ¡
#define EP1_TX_FIFO_SIZE   16
#define EP1_RX_FIFO_SIZE   16
#define EP1_PACKET_SIZE    16

//(3) ¶Ëµã1 ½ÓÊÕ»ò·¢ËÍµÄ×î´óÐÅÏ¢°ü´óÐ¡
#define EP2_TX_FIFO_SIZE   64
#define EP2_RX_FIFO_SIZE   64
#define EP2_PACKET_SIZE    64

//¶ËµãºÅ¶¨Òå
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