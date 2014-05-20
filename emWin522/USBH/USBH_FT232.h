/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2013     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : USBH.h
Purpose     : API of the USB host stack
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __USBH_FT232_H__
#define __USBH_FT232_H__

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

//
// Baud Rates
//
#define USBH_FT232_BAUD_300         300
#define USBH_FT232_BAUD_600         600
#define USBH_FT232_BAUD_1200       1200
#define USBH_FT232_BAUD_2400       2400
#define USBH_FT232_BAUD_4800       4800
#define USBH_FT232_BAUD_9600       9600
#define USBH_FT232_BAUD_14400     14400
#define USBH_FT232_BAUD_19200     19200
#define USBH_FT232_BAUD_38400     38400
#define USBH_FT232_BAUD_57600     57600
#define USBH_FT232_BAUD_115200   115200
#define USBH_FT232_BAUD_230400   230400
#define USBH_FT232_BAUD_460800   460800
#define USBH_FT232_BAUD_921600   921600

//
// Word Lengths
//
#define USBH_FT232_BITS_8        (U8)8
#define USBH_FT232_BITS_7        (U8)7
#define USBH_FT232_BITS_6        (U8)6
#define USBH_FT232_BITS_5        (U8)5

//
// Stop Bits
//
#define USBH_FT232_STOP_BITS_1    (U8)0
#define USBH_FT232_STOP_BITS_1_5  (U8)1
#define USBH_FT232_STOP_BITS_2    (U8)2

//
// Parity
//
#define USBH_FT232_PARITY_NONE    (U8)0
#define USBH_FT232_PARITY_ODD     (U8)1
#define USBH_FT232_PARITY_EVEN    (U8)2
#define USBH_FT232_PARITY_MARK    (U8)3
#define USBH_FT232_PARITY_SPACE   (U8)4

//
// Flow Control
//
#define USBH_FT232_FLOW_NONE        0x0000
#define USBH_FT232_FLOW_RTS_CTS     0x0100
#define USBH_FT232_FLOW_DTR_DSR     0x0200
#define USBH_FT232_FLOW_XON_XOFF    0x0400

//
// Purge rx and tx buffers
//
#define USBH_FT232_PURGE_RX         1
#define USBH_FT232_PURGE_TX         2

/*********************************************************************
*
*       USBH_FT232
*/

typedef int USBH_FT232_HANDLE;

typedef struct {
  U16  VendorId;
  U16  ProductId;
  U16  bcdDevice;
  U8   acSerialNo[255];
} USBH_FT232_DEVICE_INFO;

//
// structure to hold program data for USBH_FT232_Program function
//
typedef struct USBH_FT232_PROGRAM_DATA_ {
  U32 Signature1; // Header - must be 0x00000000
  U32 Signature2; // Header - must be 0xffffffff
  U32 Version;    // Header - USBH_FT232_PROGRAM_DATA version
  //          0 = original
  //          1 = FT2232C extensions
  U16 VendorId;         // 0x0403
  U16 ProductId;        // 0x6001
  char * Manufacturer;   // "FTDI"
  char * ManufacturerId; // "FT"
  char * Description;    // "USB HS Serial Converter"
  char * SerialNumber;   // "FT000001" if fixed, or NULL
  U16 MaxPower;         // 0 < MaxPower <= 500
  U16 PnP;              // 0 = disabled, 1 = enabled
  U16 SelfPowered;      // 0 = bus powered, 1 = self powered
  U16 RemoteWakeup;     // 0 = not capable, 1 = capable
  //
  // Rev4 extensions
  //
  U8 Rev4;             // non-zero if Rev4 chip, zero otherwise
  U8 IsoIn;            // non-zero if in endpoint is isochronous
  U8 IsoOut;           // non-zero if out endpoint is isochronous
  U8 PullDownEnable;   // non-zero if pull down enabled
  U8 SerNumEnable;     // non-zero if serial number to be used
  U8 USBVersionEnable; // non-zero if chip uses USBVersion
  U16 USBVersion;        // BCD (0x0200 => USB2)
  //
  // FT2232C extensions
  //
  U8 Rev5;              // non-zero if Rev5 chip, zero otherwise
  U8 IsoInA;            // non-zero if in endpoint is isochronous
  U8 IsoInB;            // non-zero if in endpoint is isochronous
  U8 IsoOutA;           // non-zero if out endpoint is isochronous
  U8 IsoOutB;           // non-zero if out endpoint is isochronous
  U8 PullDownEnable5;   // non-zero if pull down enabled
  U8 SerNumEnable5;     // non-zero if serial number to be used
  U8 USBVersionEnable5; // non-zero if chip uses USBVersion
  U16 USBVersion5;        // BCD (0x0200 => USB2)
  U8 AIsHighCurrent;    // non-zero if interface is high current
  U8 BIsHighCurrent;    // non-zero if interface is high current
  U8 IFAIsFifo;         // non-zero if interface is 245 FIFO
  U8 IFAIsFifoTar;      // non-zero if interface is 245 FIFO CPU target
  U8 IFAIsFastSer;      // non-zero if interface is Fast serial
  U8 AIsVCP;            // non-zero if interface is to use VCP drivers
  U8 IFBIsFifo;         // non-zero if interface is 245 FIFO
  U8 IFBIsFifoTar;      // non-zero if interface is 245 FIFO CPU target
  U8 IFBIsFastSer;      // non-zero if interface is Fast serial
  U8 BIsVCP;            // non-zero if interface is to use VCP drivers
} USBH_FT232_PROGRAM_DATA;


typedef void USBH_FT232_USER_FUNC(void * pContext);

USBH_BOOL         USBH_FT232_Init                     (void);
void              USBH_FT232_Exit                     (void);
void              USBH_FT232_RegisterNotification     (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
void              USBH_FT232_ConfigureDefaultTimeout  (U32 ReadTimeout, U32 WriteTimeout);
USBH_FT232_HANDLE USBH_FT232_Open                     (unsigned Index);
USBH_STATUS       USBH_FT232_Close                    (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_GetDeviceInfo            (USBH_FT232_HANDLE hDevice, USBH_FT232_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_FT232_ResetDevice              (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetTimeouts              (USBH_FT232_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS       USBH_FT232_SetBaudRate              (USBH_FT232_HANDLE hDevice, U32 BaudRate);
USBH_STATUS       USBH_FT232_Read                     (USBH_FT232_HANDLE hDevice,       U8 * pBuffer, U32 nBufferSize, U32 * pBytesReturned);
USBH_STATUS       USBH_FT232_Write                    (USBH_FT232_HANDLE hDevice, const U8 * pBuffer, U32 nBufferSize, U32 * pBytesWritten);
USBH_STATUS       USBH_FT232_AllowShortRead           (USBH_FT232_HANDLE hDevice, U8 AllowShortRead);
USBH_STATUS       USBH_FT232_SetDataCharacteristics   (USBH_FT232_HANDLE hDevice, U8  Length, U8 StopBits,  U8 Parity);
USBH_STATUS       USBH_FT232_SetFlowControl           (USBH_FT232_HANDLE hDevice, U16 FlowControl, U8 XonChar,  U8 XoffChar);
USBH_STATUS       USBH_FT232_SetDtr                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_ClrDtr                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetRts                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_ClrRts                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_GetModemStatus           (USBH_FT232_HANDLE hDevice, U32 * pModemStatus);
USBH_STATUS       USBH_FT232_SetChars                 (USBH_FT232_HANDLE hDevice, U8 EventChar, U8 EventCharEnabled,  U8 ErrorChar, U8 ErrorCharEnabled);
USBH_STATUS       USBH_FT232_Purge                    (USBH_FT232_HANDLE hDevice, U32 Mask);
USBH_STATUS       USBH_FT232_GetQueueStatus           (USBH_FT232_HANDLE hDevice, U32 * pRxBytes);

USBH_STATUS       USBH_FT232_SetBreakOn               (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetBreakOff              (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetLatencyTimer          (USBH_FT232_HANDLE hDevice, U8   Latency);
USBH_STATUS       USBH_FT232_GetLatencyTimer          (USBH_FT232_HANDLE hDevice, U8 * Latency);
USBH_STATUS       USBH_FT232_SetBitMode               (USBH_FT232_HANDLE hDevice, U8   Mask, U8 Enable);
USBH_STATUS       USBH_FT232_GetBitMode               (USBH_FT232_HANDLE hDevice, U8 * pMode);


#if 0  // Currently not available, will be implemented in a future version
USBH_STATUS       USBH_FT232_CancelIo                 (USBH_FT232_HANDLE hDevice);
int               USBH_FT232_GetNumDevices            (USBH_FT232_DEVICE_INFO * paDevInfo, U32 NumItems);
USBH_STATUS       USBH_FT232_SetDivisor               (USBH_FT232_HANDLE hDevice, U16 Divisor);
                                                      
                                                      
USBH_STATUS       USBH_FT232_SetWaitMask              (USBH_FT232_HANDLE hDevice, U32   Mask);
USBH_STATUS       USBH_FT232_WaitOnMask               (USBH_FT232_HANDLE hDevice, U32 * pMask);
USBH_STATUS       USBH_FT232_GetEventStatus           (USBH_FT232_HANDLE hDevice, U32 * pEventU32);
                                                      
USBH_STATUS       USBH_FT232_ReadEE                   (USBH_FT232_HANDLE hDevice, U32 Offset, U16 * pValue);
USBH_STATUS       USBH_FT232_WriteEE                  (USBH_FT232_HANDLE hDevice, U32 Offset, U16    Value);
USBH_STATUS       USBH_FT232_EraseEE                  (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_EE_Program               (USBH_FT232_HANDLE hDevice, USBH_FT232_PROGRAM_DATA * pData);
USBH_STATUS       USBH_FT232_EE_ProgramEx             (USBH_FT232_HANDLE hDevice, USBH_FT232_PROGRAM_DATA * pData, char * pManufacturer,  char * pManufacturerId, char * pDescription, char * pSerialNumber);
USBH_STATUS       USBH_FT232_EE_Read                  (USBH_FT232_HANDLE hDevice, USBH_FT232_PROGRAM_DATA * pData);
USBH_STATUS       USBH_FT232_EE_ReadEx                (USBH_FT232_HANDLE hDevice, USBH_FT232_PROGRAM_DATA * pData, char * pManufacturer,  char * pManufacturerId, char * pDescription, char * pSerialNumber);
USBH_STATUS       USBH_FT232_EE_UASize                (USBH_FT232_HANDLE hDevice, U32 * pSize);
USBH_STATUS       USBH_FT232_EE_UAWrite               (USBH_FT232_HANDLE hDevice, U8 * pData, U32 DataLen);
USBH_STATUS       USBH_FT232_EE_UARead                (USBH_FT232_HANDLE hDevice, U8 * pData, U32 DataLen,  U32 * pBytesRead);
                                                      
USBH_STATUS USBH_FT232_ResetPort              (USBH_FT232_HANDLE hDevice);
#endif

#if defined(__cplusplus)
  }
#endif

#endif // USBH_FT232_H__

/******************************** EOF ******************************/
