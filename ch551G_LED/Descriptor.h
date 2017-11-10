//#include "mytype.h"
//#include "hiduse.h"

#include "./CH554.H"
#include "./Debug.H"

#ifndef _descriptor_h_
#define _descriptor_h_

//�����׼���豸�������ṹ
typedef struct _DEVICE_DCESCRIPTOR_STRUCT
{
	UINT8	blength;                               //�豸���������ֽ�����С
	UINT8	bDescriptorType;	                   //�豸���������ͱ��
	UINT16	bcdUSB;                                //USB�汾��
	UINT8	bDeviceClass;                          //USB������豸�����
	UINT8	bDeviceSubClass;                       //USB������������
	UINT8	bDeviceProtocol;                       //USB������豸Э�����
	UINT8	bMaxPacketSize0;                       //�˵�0��������С
	UINT16	idVendor;                              //���̱��
	UINT16	idProduct;                             //��Ʒ���
	UINT16	bcdDevice;                             //�豸�������
	UINT8	iManufacturer;                         //�豸�����ַ���������
	UINT8	iProduct;                              //������Ʒ�ַ���������
	UINT8	iSerialNumber;                         //�����豸���к��ַ���������
	UINT8	bNumConfigurations;                    //���ܵ���������
}DEVICE_DESCRIPTOR_STRUCT, *pDEVICE_DESCRIPTOR_STRUCT;


//�����׼�������������ṹ
typedef struct _CONFIGURATION_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //�������������ֽ�����С
	UINT8	bDescriptorType;                       //�������������ͱ��
	UINT16	wTotalLength;                          //�����÷��ص��������ݴ�С
	UINT8	bNumInterfaces;                        //��������֧�ֵĽӿ�����
	UINT8	bConfigurationValue;                   //Set_Configuration��������Ҫ�Ĳ���ֵ
	UINT8	iConfiguration;                        //���������õ��ַ���������ֵ
	UINT8	bmAttributes;                          //����ģʽ��ѡ��
	UINT8	MaxPower;                              //�豸��������ȡ��������
}CONFIGURATION_DESCRIPTOR_STRUCT, *pCONFIGURATION_DESCRIPTOR_STRUCT;


//�����׼�Ľӿ��������ṹ
typedef struct _INTERFACE_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                               //�ӿ����������ֽ�����С
	UINT8	bDescriptorType;                       //�ӿ������������ͱ��
	UINT8	bInterfaceNumber;                      //�ýӿڵı��
	UINT8	bAlternateSetting;                     //���õĽӿ����������
	UINT8	bNumEndpoints;                         //�ýӿ�ʹ�õĶ˵������������˵�0
	UINT8	bInterfaceClass;                       //�ӿ�����
	UINT8	bInterfaceSubClass;                    //�ӿ�������
	UINT8	bInterfaceProtocol;                    //�ӿ���ѭ��Э��
	UINT8	iInterface;                            //�����ýӿڵ��ַ�������ֵ
}INTERFACE_DESCRIPTOR_STRUCT, *pINTERFACE_DESCRIPTOR_STRUCT;


//�����׼�Ķ˵��������ṹ
typedef struct _ENDPOINT_DESCRIPTOR_STRUCT
{
	UINT8	bLength;                                //�˵��������ֽ�����С
	UINT8	bDescriptorType;                       //�˵����������ͱ��
	UINT8	bEndpointAddress;                      //�˵��ַ�������������
	UINT8	bmAttributes;                          //�˵�Ĵ�����������
	UINT16	wMaxPacketSize;                        //�˵��ա�����������С
	UINT8	bInterval;                             //������ѯ�˵��ʱ����
}ENDPOINT_DESCRIPTOR_STRUCT, * pENDPOINT_DESCRIPTOR_STRUCT;


//HID�������е��¼���������
typedef struct _HID_SUB_DESCRIPTOR_STRUCT
{
	UINT8	bDescriptorType;    //�¼������������ͱ��
	UINT16	wDescriptorLength;  //�¼��������ĳ���
}HID_SUB_DESCRIPTOR_STRUCT,*pHID_SUB_DESCRIPTOR_STRUCT;

#define NUM_SUB_DESCRIPTORS 1

//����HID�������ṹ
typedef struct _HID_DESCRIPTOR_STRUCT
{
	UINT8	bLength;
	UINT8	bDescriptorType;
	UINT16	bcdHID;
	UINT8	bCountryCode;
	UINT8	bNumDescriptors;
	HID_SUB_DESCRIPTOR_STRUCT	HidSubDescriptors[NUM_SUB_DESCRIPTORS]; 
}HID_DESCRIPTOR_STRUCT,*pHID_DESCRIPTOR_STRUCT;	


//һ���ַ����������ṹ
typedef struct _STRING_DESCRIPTOR_STRUCT
{
	UINT8	bLength;							//�ַ������������ֽ�����С
	UINT8	bDescriptorType;					//�ַ������������ͱ��
	UINT8	StringDesriptor[36];					//UNICODE������ַ���
}STRING_DESCRIPTOR_STRUCT, *pSTRING_DESCRIPTOR_STRUCT;

#define ENDPOINT_NUMBER 2
//�������ã��ӿڣ��˵���������ṹ��
typedef struct _USB_DESCRIPTOR_STRUCT
{
	CONFIGURATION_DESCRIPTOR_STRUCT	configuration_descriptor;
	INTERFACE_DESCRIPTOR_STRUCT		interface_descriptor;
	HID_DESCRIPTOR_STRUCT			hid_descriptor;
	ENDPOINT_DESCRIPTOR_STRUCT		endpoint_descriptor[ENDPOINT_NUMBER];
}USB_DESCRIPTOR_STRUCT, *pUSB_DESCRIPTOR_STRUCT;

//*******************************************************
// USB �豸������������ masks 
//*******************************************************
//(1) ���շ�: D4...D0 
#define USB_RECIPIENT            0x1F
#define USB_RECIPIENT_DEVICE     0x00
#define USB_RECIPIENT_INTERFACE  0x01
#define USB_RECIPIENT_ENDPOINT   0x02

//(2) ����: D6...D5
#define USB_REQUEST_TYPE		 0x60
#define USB_STANDARD_REQUEST     0x00
#define USB_CLASS_REQUEST        0x20
#define USB_VENDOR_REQUEST       0x40

//(3) ���ݴ��䷽��: D7
#define USB_ENDPOINT_DIRECTION_MASK   0x80

//��ַ����
#define DEVICE_ADDRESS_MASK      0x7F

//(4) USB ����
#define USB_REQUEST_MASK         (UINT8)0x0F

//(5) USB�˵㷽������
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//*******************************************************
// USB �豸����������
//*******************************************************
#define USB_DEVICE_DESCRIPTOR                0x01  //�豸������
#define USB_CONFIGURATION_DESCRIPTOR         0x02  //����������
#define USB_STRING_DESCRIPTOR                0x03  //�ַ���������
#define USB_INTERFACE_DESCRIPTOR             0x04  //�ӿ�������
#define USB_ENDPOINT_DESCRIPTOR              0x05  //�˵�������
#define USB_HID_DESCRIPTOR					 0x21  //HID������
#define USB_REPORT_DESCRIPTOR                0x22  //����������
#define USB_PHYSICAL_DESCRIPTOR              0x23  //����������

//*******************************************************
// ����Ϊ����USB 1.0�淶����ȷֵ
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
//** USB ����ѡ���
//******************************************************
#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005

//******************************************************
//** USB ����������
//******************************************************
#define USB_CONFIG_POWERED_MASK                   0xc0
#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

#define BUS_POWERED                           0x80
#define SELF_POWERED                          0x40
#define REMOTE_WAKEUP                         0x20

//******************************************************
//** USB �˵��������ṹ��: �˵㴫������
//******************************************************
#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03

/*******************************************************
** USB  �豸��
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

/*****   D12 �ӿ���������: ���˵�0�Ķ˵�������Ŀ  ******/
//#define ENDPOINT_NUMBER	4  //���ڴ��ļ�ǰ�涨��

//(1) �˵�0�������С
#define EP0_TX_FIFO_SIZE   16
#define EP0_RX_FIFO_SIZE   16
#define EP0_PACKET_SIZE    16

//(2) �˵�1 ���ջ��͵������Ϣ����С
#define EP1_TX_FIFO_SIZE   16
#define EP1_RX_FIFO_SIZE   16
#define EP1_PACKET_SIZE    16

//(3) �˵�1 ���ջ��͵������Ϣ����С
#define EP2_TX_FIFO_SIZE   64
#define EP2_RX_FIFO_SIZE   64
#define EP2_PACKET_SIZE    64

//�˵�Ŷ���
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