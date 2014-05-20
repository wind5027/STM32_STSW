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
File    : USBH_ConfDefaults.h
Purpose :
--------------------------  END-OF-HEADER  ---------------------------
*/

#ifndef   USBH_CONFDEFAULTS_H
#define   USBH_CONFDEFAULTS_H

#include "USBH_Conf.h"

/*********************************************************************
*
*       Basic types
*/
#define     USBH_BOOL char

#ifndef     FALSE
  #define   FALSE        (1 == 0)
#endif
#ifndef     TRUE
  #define   TRUE         (1 == 1)
#endif
#ifndef     USBH_DEBUG
  #define   USBH_DEBUG   (0)
#endif
#ifndef     USBH_MEMCPY
  #define   USBH_MEMCPY  memcpy
#endif
#ifndef     USBH_MEMSET
  #define   USBH_MEMSET  memset
#endif
#ifndef     USBH_MEMMOVE
  #define   USBH_MEMMOVE memmove
#endif
#ifndef     USBH_MEMCMP
  #define   USBH_MEMCMP  memcmp
#endif
#ifndef     USBH_OPTIMIZE
  #define   USBH_OPTIMIZE
#endif
#ifndef     USBH_IS_BIG_ENDIAN
  #define   USBH_IS_BIG_ENDIAN 0      // Little endian is default
#endif

#ifndef     USBH_PANIC
  #if       USBH_DEBUG
    #define USBH_PANIC(s) USBH_Panic(s)
  #else
    #define USBH_PANIC(s)
  #endif
#endif
#ifndef     USBH_SUPPORT_LOG
  #if       USBH_DEBUG > 1
    #define USBH_SUPPORT_LOG   1
  #else
    #define USBH_SUPPORT_LOG   0
  #endif
#endif
#ifndef     USBH_SUPPORT_WARN
  #if       USBH_DEBUG > 1
    #define USBH_SUPPORT_WARN  1
  #else
    #define USBH_SUPPORT_WARN  0
  #endif
#endif

#ifndef    USBH_SUPPORT_ISO_TRANSFER
  #define  USBH_SUPPORT_ISO_TRANSFER                     0
#endif

#define USBH_TRANSFER_BUFFER_ALIGNMENT                   1
#define USBH_IS_VALID_TRANSFER_BUFFER_RANGE(    pBuffer) (TRUE)
#define USBH_IS_VALID_TRANSFER_BUFFER_ALIGNMENT(pBuffer) (FALSE)
#define USBH_IS_VALID_TRANSFER_MEM(             pBuffer) (USBH_IS_VALID_TRANSFER_BUFFER_RANGE(pBuffer) && USBH_IS_VALID_TRANSFER_BUFFER_ALIGNMENT(pBuffer) ? TRUE : FALSE)

#ifndef   USBH_SUPPORT_VIRTUALMEM
  #define USBH_SUPPORT_VIRTUALMEM    1
#endif


//#if HC_ISO_ENABLE && (HC_DEVICE_ISO_ENDPOINTS==0)
//  #error error HC_DEVICE_ISO_ENDPOINTS
//#endif

#ifndef USBH_MSD_EP0_TIMEOUT
  #define USBH_MSD_EP0_TIMEOUT       5000 // Specifies the default timeout, in milliseconds, to be used for synchronous operations on the control endpoint.
#endif
#ifndef USBH_MSD_COMMAND_TIMEOUT
  #define USBH_MSD_COMMAND_TIMEOUT   3000 // Specifies the maximum time in milliseconds, for reading all bytes with the bulk read operation.
#endif

#ifndef USBH_MSD_READ_TIMEOUT
  #define USBH_MSD_READ_TIMEOUT      10000 // Specifies the maximum time in milliseconds, for reading all bytes with the bulk read operation.
#endif
#ifndef USBH_MSD_WRITE_TIMEOUT
  #define USBH_MSD_WRITE_TIMEOUT     10000 // Specifies the maximum time, in milliseconds, for writing all bytes with the bulk write operation.
#endif
// Must be a multiple of the maximum packet length for bulk data endpoints.
// That are 64 bytes for a USB 1.1 device and 512 bytes for a USB 2.0 high speed device.
#ifndef USBH_MSD_MAX_TRANSFER_SIZE
  #define USBH_MSD_MAX_TRANSFER_SIZE (32 * 1024) // [bytes]
#endif
#ifndef USBH_MSD_DEFAULT_SECTOR_SIZE
  #define USBH_MSD_DEFAULT_SECTOR_SIZE    512 // Specifies the default sector size in bytes to be used for reading and writing.
#endif
#ifndef USBH_MSD_MAX_DEVICES
  #define USBH_MSD_MAX_DEVICES            2   // Maximum number of USB Mass Storage devices that are supported from the library. A lower value saves memory.
#endif
#ifndef USBH_MSD_MAX_LUNS_PER_DEVICE
  #define USBH_MSD_MAX_LUNS_PER_DEVICE    2   // Maximum number of logical units per device that are supported from the library. A lower value saves memory.
#endif

//
// The reset on the root hub is aborted after this time
//
#define DEFAULT_RESET_TIMEOUT        80
//
// The host controller waits this time after reset before the Set Address command is sent. Some devices require some time
// before they can answer correctly. It is not required by the USB specification but Windows makes this gap with 80 ms.
//
#define WAIT_AFTER_RESET            80

//
// The bus driver waits this time before the next command is sent after Set Address. The device must answer to SetAddress on USB address 0 with the
// handshake and than set the new address. This is a potential racing condition if this step is performed in the firmware.
// Give the device this time to set the new address.
//
#define WAIT_AFTER_SETADDRESS       30
// If the device makes an error during USB reset and set address the enumeration process is repeated. The repeat count is defined by this define.
// Possible errors are OverCurrent, remove during reset, no answer to SetAddress.
#define RESET_RETRY_COUNTER         10
#define DELAY_FOR_REENUM            1000 // Describes the time before a USB reset is restarted if the last try has failed.
// The default size of the buffer to get descriptors from the device. If the buffer is too small for the configuration descriptor,
// a new buffer is dynamically allocated. The default size must be at least 256 bytes to read all possible string descriptors.
#define DEFAULT_TRANSFERBUFFER_SIZE 256
#define DEFAULT_SETUP_TIMEOUT       500  // Default timeout for all setup requests. After this time a not completed setup request is terminated.
// If the enumeration of the device (get descriptors, set configuration) fails, it is repeated after a time gap of this value
#define DEFAULT_RETRY_TIMEOUT       1000

#define USBH_HUB_RESET_TIMEOUT  ((DEFAULT_RESET_TIMEOUT) + (200))

// In order to avoid warnings for undefined parameters
#ifndef USBH_USE_PARA
  #if defined(NC30) || defined(NC308)
    #define USBH_USE_PARA(para)
  #else
    #define USBH_USE_PARA(para) (void)para;
  #endif
#endif

#endif

/*************************** End of file ****************************/
