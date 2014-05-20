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
File        : USBH_CDC.h
Purpose     : API of the USB host stack
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __USBH_CDC_H__
#define __USBH_CDC_H__

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

//
// Baud Rates
//
#define USBH_CDC_BAUD_300         300
#define USBH_CDC_BAUD_600         600
#define USBH_CDC_BAUD_1200       1200
#define USBH_CDC_BAUD_2400       2400
#define USBH_CDC_BAUD_4800       4800
#define USBH_CDC_BAUD_9600       9600
#define USBH_CDC_BAUD_14400     14400
#define USBH_CDC_BAUD_19200     19200
#define USBH_CDC_BAUD_38400     38400
#define USBH_CDC_BAUD_57600     57600
#define USBH_CDC_BAUD_115200   115200
#define USBH_CDC_BAUD_230400   230400
#define USBH_CDC_BAUD_460800   460800
#define USBH_CDC_BAUD_921600   921600

//
// Word Lengths
//
#define USBH_CDC_BITS_8        (U8)8
#define USBH_CDC_BITS_7        (U8)7
#define USBH_CDC_BITS_6        (U8)6
#define USBH_CDC_BITS_5        (U8)5

//
// Stop Bits
//
#define USBH_CDC_STOP_BITS_1    (U8)0
#define USBH_CDC_STOP_BITS_1_5  (U8)1
#define USBH_CDC_STOP_BITS_2    (U8)2

//
// Parity
//
#define USBH_CDC_PARITY_NONE    (U8)0
#define USBH_CDC_PARITY_ODD     (U8)1
#define USBH_CDC_PARITY_EVEN    (U8)2
#define USBH_CDC_PARITY_MARK    (U8)3
#define USBH_CDC_PARITY_SPACE   (U8)4

//
// Flow Control
//
#define USBH_CDC_FLOW_NONE        0x0000
#define USBH_CDC_FLOW_RTS_CTS     0x0100
#define USBH_CDC_FLOW_DTR_DSR     0x0200
#define USBH_CDC_FLOW_XON_XOFF    0x0400

//
// Purge rx and tx buffers
//
#define USBH_CDC_PURGE_RX         1
#define USBH_CDC_PURGE_TX         2

/*********************************************************************
*
*       USBH_FT232
*/

typedef int USBH_CDC_HANDLE;

typedef struct {
  U16  VendorId;
  U16  ProductId;
  U8   acSerialNo[255];
} USBH_CDC_DEVICE_INFO;

typedef struct {
  U8 bRxCarrier;  // State of receiver carrier detection mechanism of device. This signal
                  // corresponds to V.24 signal 109 and RS-232 signal DCD.
  U8 bTxCarrier;  // State of transmission carrier. This signal corresponds to V.24 signal 106 and
                  // RS-232 signal DSR.
  U8 bBreak;      // State of break detection mechanism of the device. 
  U8 bRingSignal; // State of ring signal detection of the device. 
  U8 bFraming;    // A framing error has occurred. 
  U8 bParity;     // A parity error has occurred. 
  U8 bOverRun;    // Received data has been discarded due to overrun in the device. 
}  USBH_CDC_SERIALSTATE;

typedef void USBH_CDC_USER_FUNC(void * pContext);

USBH_BOOL         USBH_CDC_Init                     (void);
void              USBH_CDC_Exit                     (void);
void              USBH_CDC_RegisterNotification     (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
void              USBH_CDC_ConfigureDefaultTimeout  (U32 ReadTimeout, U32 WriteTimeout);
USBH_CDC_HANDLE   USBH_CDC_Open                     (unsigned Index);
USBH_STATUS       USBH_CDC_Close                    (USBH_CDC_HANDLE hDevice);

USBH_STATUS       USBH_CDC_AllowShortRead           (USBH_CDC_HANDLE hDevice, U8 AllowShortRead);
USBH_STATUS       USBH_CDC_GetDeviceInfo            (USBH_CDC_HANDLE hDevice, USBH_CDC_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_CDC_Read                     (USBH_CDC_HANDLE hDevice,       U8 * pBuffer, U32 nBufferSize, U32 * pBytesReturned);
USBH_STATUS       USBH_CDC_Write                    (USBH_CDC_HANDLE hDevice, const U8 * pBuffer, U32 nBufferSize, U32 * pBytesWritten);
USBH_STATUS       USBH_CDC_SetBreak                 (USBH_CDC_HANDLE hDevice, U16 Duration);
USBH_STATUS       USBH_CDC_SetBreakOn               (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetBreakOff              (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetCommParas             (USBH_CDC_HANDLE hDevice, U32 BaudRate,    U8 Length,   U8 StopBits,  U8 Parity);
USBH_STATUS       USBH_CDC_SetTimeouts              (USBH_CDC_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS       USBH_CDC_SetDtr                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_ClrDtr                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetRts                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_ClrRts                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_GetQueueStatus           (USBH_CDC_HANDLE hDevice, U32 * pRxBytes);
USBH_STATUS       USBH_CDC_GetSerialState           (USBH_CDC_HANDLE hDevice, USBH_CDC_SERIALSTATE * pSerialState);

#if 0  // Will be implemented in the next version of emUSBH
USBH_STATUS       USBH_CDC_CancelIo                 (USBH_CDC_HANDLE hDevice);
#endif

#if defined(__cplusplus)
  }
#endif

#endif // __USBH_CDC_H__

/******************************** EOF ******************************/
