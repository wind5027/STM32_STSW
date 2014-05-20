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

#ifndef __USBH_HID_H__
#define __USBH_HID_H__

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif



/*********************************************************************
*
*       USBH_HID
*/
typedef struct {
  unsigned Code;
  int      Value;
} USBH_HID_KEYBOARD_DATA;

typedef struct {
  int xChange;
  int yChange;
  int WheelChange;
  int ButtonState;
} USBH_HID_MOUSE_DATA;

typedef enum {
  USBH_HID_INPUT_REPORT = 0,
  USBH_HID_OUTPUT_REPORT,
  USBH_HID_FEATURE_REPORT
} USBH_HID_REPORT_TYPE;

typedef int USBH_HID_HANDLE;
typedef struct {
  USBH_HID_REPORT_TYPE ReportType;
  U32 ReportId;
  U32 ReportSize;
} USBH_HID_REPORT_INFO;

typedef struct {
  U16  InputReportSize;
  U16  OutputReportSize;
  U16  ProductId;
  U16  VendorId;
  char acName[7];
} USBH_HID_DEVICE_INFO;

typedef void USBH_HID_USER_FUNC(void * pContext);


typedef void (USBH_HID_ON_KEYBOARD_FUNC) (USBH_HID_KEYBOARD_DATA * pKeyData);
typedef void (USBH_HID_ON_MOUSE_FUNC)    (USBH_HID_MOUSE_DATA    * pMouseData);

void            USBH_HID_Exit(void);
USBH_BOOL       USBH_HID_Init(void);
void            USBH_HID_SetOnMouseStateChange    (USBH_HID_ON_MOUSE_FUNC    * pfOnChange);
void            USBH_HID_SetOnKeyboardStateChange (USBH_HID_ON_KEYBOARD_FUNC * pfOnChange);
USBH_STATUS     USBH_HID_Close                    (USBH_HID_HANDLE hDevice);
int             USBH_HID_GetNumDevices            (USBH_HID_DEVICE_INFO * pDevInfo, U32 NumItems);
USBH_STATUS     USBH_HID_GetReportDescriptorParsed(USBH_HID_HANDLE hDevice, USBH_HID_REPORT_INFO * pReportInfo, unsigned * pNumEntries);
USBH_STATUS     USBH_HID_GetDeviceInfo            (USBH_HID_HANDLE hDevice, USBH_HID_DEVICE_INFO * pDevInfo);
USBH_STATUS     USBH_HID_GetReportDescriptor      (USBH_HID_HANDLE hDevice, U8 * pReportDescriptor, unsigned NumBytes);
USBH_STATUS     USBH_HID_GetReport                (USBH_HID_HANDLE hDevice, U8 * pReport, U32 NumBytes, USBH_HID_USER_FUNC * pfFunc, void * pContextData);
USBH_HID_HANDLE USBH_HID_Open                     (const char * sName);
USBH_STATUS     USBH_HID_SetReport                (USBH_HID_HANDLE hDevice, const U8 * pReport, U32 NumBytes, USBH_HID_USER_FUNC * pfFunc, void * pContextData);
USBH_STATUS     USBH_HID_CancelIo                 (USBH_HID_HANDLE hDevice);
void            USBH_HID_RegisterNotification     (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
void            USBH_HID_ConfigureControlWriteRetries(U8 NumRetries);
void            USBH_HID_ConfigureControlWriteTimeout(U32 Timeout);


#if defined(__cplusplus)
  }
#endif

#endif // USBH_HID_H__

/******************************** EOF ******************************/
