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
File        : USBH_Int.h
Purpose     : Internals used accross different layers of the USB device stack
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _USBH_INT_H_ // Avoid multiple/recursive inclusion
#define _USBH_INT_H_

#include <stdlib.h>  // for atoi(), exit()
#include <string.h>
#include <stdio.h>

#include "Segger.h"
#include "USBH.h"
#include "USBH_ConfDefaults.h"

#if defined(__cplusplus)
  extern "C" { // Make sure we have C-declarations in C++ programs
#endif

#ifdef USBHCORE_C
  #define EXTERN
#else
  #define EXTERN extern
#endif

#if USBH_SUPPORT_LOG
  #define USBH_LOG(p) USBH_Logf p
#else
  #define USBH_LOG(p)
#endif

#if USBH_SUPPORT_WARN
  #define USBH_WARN(p) USBH_Warnf p
#else
  #define USBH_WARN(p)
#endif

#if USBH_DEBUG >= 3
  #define USBH_WARN_INTERNAL(p) USBH_Warnf p
#else
  #define USBH_WARN_INTERNAL(p)
#endif

// Useful macros:
#define USBH_MIN(x,y)    ((x)  <  (y)   ?  (x)   :  (y))
#define USBH_MAX(x,y)    ((x)  >  (y)   ?  (x)   :  (y))
#define USBH_COUNTOF(a)  (sizeof(a)/sizeof(a[0]))

typedef void USBH_DRIVER_REMOVE_FUNC (void);

typedef struct USBH_EXT_HUB_API {
  void      (*pfStartHub)                          (USB_DEVICE * pEnumDev);
  USBH_BOOL (*pfReStartHubPort)                    (USBH_HOST_CONTROLLER * pHostController);
  void      (*pfDisablePort)                       (USB_DEVICE * pDevice, USBH_HUB_PORT * pParentPort);
  void      (*pfDeleteHub)                         (USBH_HUB * pUsbHub);
  void      (*pfMarkParentAndChildDevicesAsRemoved)(USB_DEVICE * pDevice);
  void      (*pfSetEnumState)(USB_DEVICE * pDevice);
} USBH_EXT_HUB_API;


#if USBH_SUPPORT_VIRTUALMEM
  #define USBH_V2P(p)                 USBH_v2p(p)
#else 
  #define USBH_V2P(p)                 (U32)(p)
#endif

/*********************************************************************
*
*       USBH_GLOBAL
*/

typedef struct USBH_GLOBAL {
  U8                      ConfigCompleted;
  U8                      InitCompleted;
  USBH_HC_HANDLE          hHC;
  USBH_HC_BD_HANDLE       hHCBD;
  USBH_HOST_DRIVER_INST   DriverInst;
  USBH_HOST_DRIVER      * pDriver;
  USBH_ON_SETCONFIGURATION_FUNC * pfOnSetConfiguration;
  void                          * pOnSetConfigContext;
  USBH_EXT_HUB_API              * pExtHubApi;
  struct {
    U32 TransferBufferSize;
    U8  NumRootHubs;
    U8  RootHubPortsAlwaysPowered;
    U8  RootHubPerPortPowered;
    U8  RootHubSupportOvercurrent;
    U8  NumUSBDevices;
    U8  NumBulkEndpoints;
    U8  NumIntEndpoints;
    U8  NumIsoEndpoints;
//    U8  SupportExternalHubs;
    U8  SetHighIsPowerOn;
    U32 DefaultPowerGoodTime;
  } Config;

  U8 IsRunning;
  U8 TimerTaskIsRunning;
  U8 ISRTaskIsRunning;
  USBH_DRIVER_REMOVE_FUNC * pfDriverRemove;
#if USBH_SUPPORT_VIRTUALMEM
  USBH_V2P_FUNC           * pfV2P;
#endif
} USBH_GLOBAL;

EXTERN USBH_GLOBAL USBH_Global;

#define USBH_PRINT_STATUS_VALUE(Type, status) USBH_WARN((Type, "%s", USBH_GetStatusStr(status)))

#if (USBH_DEBUG > 1)
  #define USBH_ASSERT(condition)           if (!(condition)) { USBH_WARN((USBH_MTYPE_CORE, "\nASSERTION FAILED: %s(%d)\n", __FILE__, __LINE__)); }
  #define USBH_ASSERT_PTR(Ptr)             USBH_ASSERT(Ptr != NULL)
  #define USBH_ASSERT_MAGIC(p,type)        USBH_ASSERT(USBH_IS_PTR_VALID((p),type))
  #define USBH_ASSERT0                     USBH_WARN((USBH_MTYPE_CORE, "\nASSERT0: %s(%d)\n", __FILE__, __LINE__));
#else
  #define USBH_ASSERT(condition)
  #define USBH_ASSERT_PTR(Ptr)
  #define USBH_ASSERT_MAGIC(p, type)
  #define USBH_ASSERT0
#endif

#define FOUR_CHAR_ULONG(c1,c2,c3,c4)    (((U32)(c1)) | (((U32)(c2)) << 8) | (((U32)(c3)) << 16) | (((U32)(c4)) << 24)) // Generates a magic ulong (four char code)
#define TWO_CHAR_USHORT(c1,c2)          (((U16)(c1)) | ( (U16)(c2)  << 8))                                         // Generates a magic ulong (four char code)
#define GET_MASK_FROM_BITNUMBER(BitNb)  (((U32)(1)) << (BitNb))
// Calculate the pointer to the base of an object given its type and a pointer to a field within the object.
#define USBH_ZERO_MEMORY(mem,count)     USBH_MEMSET((mem), 0,   (count))
#define USBH_ZERO_STRUCT(s)             USBH_ZERO_MEMORY(&(s),sizeof(s))
#define USBH_ZERO_ARRAY(s)              USBH_ZERO_MEMORY( (s),sizeof(s))
#define USBH_ARRAY_ELEMENTS(a)          (sizeof(a)     / sizeof(a[0]))
#define USBH_ARRAY_LIMIT(a)             (&a[USBH_ARRAY_ELEMENTS(a)])

#if (USBH_DEBUG > 1)
  #define USBH_IS_PTR_VALID(p,type) ((p)!=NULL && (p)->Magic==type##_MAGIC) // Takes a pointer and its type and compares the Magic field with a constant
#else
  #define USBH_IS_PTR_VALID(p,type)
#endif

// Helper macro, used to convert enum constants to string values
// lint -save -e773
#define USBH_ENUM_TO_STR(e) (x==(e)) ? #e
// lint -restore
#define USBH_IS_ALIGNED(val,size) (((val) &  ((size)-1)) == 0)          // Returns true if the given value is aligned to a 'size' boundary
#define USBH_ALIGN_UP(  val,size) (((val) +  ((size)-1)) & ~((size)-1)) // Round up a value to the next 'size' boundary
#define USBH_ALIGN_DOWN(val,size) ( (val) & ~((size)-1))                // Round down a value to the next 'size' boundary

#if (USBH_DEBUG > 1)                                                  // Handy macro to enable code in debug builds only
  #define IFDBG(x) { x; }
#else
  #define IFDBG(x)
#endif

/* xyxy */
#define URB_BUFFER_POOL_MAGIC                   FOUR_CHAR_ULONG('T','P','O','O')
#define ENUM_ERROR_NOTIFICATION_MAGIC           FOUR_CHAR_ULONG('E','N','O','T')
#define USBH__PNP_NOTIFICATION_MAGIC            FOUR_CHAR_ULONG('P','N','P','N')
#define DELAYED_PNP_NOTIFY_CONTEXT_MAGIC        FOUR_CHAR_ULONG('P','N','P','D')
#define INTERFACE_ENTRY_MAGIC                   FOUR_CHAR_ULONG('I','F','A','E')
#define OHD_EP0_MAGIC                           FOUR_CHAR_ULONG('E','P','0','M')
#define DEFAULT_EP_MAGIC                        FOUR_CHAR_ULONG('E','P','0',' ')
#define USB_ENDPOINT_MAGIC                      FOUR_CHAR_ULONG('E','N','D','P')
#define USB_INTERFACE_MAGIC                     FOUR_CHAR_ULONG('U','I','F','U')
#define USB_DEVICE_MAGIC                        FOUR_CHAR_ULONG('U','D','E','V')
#define USBH_HOST_DRIVER_INST_MAGIC             FOUR_CHAR_ULONG('U','D','R','V')
#define ROOT_HUB_MAGIC                          FOUR_CHAR_ULONG('R','H','U','B')
#define USBH_HUB_MAGIC                          FOUR_CHAR_ULONG('U','H','U','B')
#define USBH_HUB_PORT_MAGIC                     FOUR_CHAR_ULONG('P','O','R','T')
#define USBH_HOST_CONTROLLER_MAGIC              FOUR_CHAR_ULONG('H','O','S','T')
#define USBH_HCM_POOL_MAGIC                     FOUR_CHAR_ULONG('P','O','O','L')
#define USBH_HCM_ITEM_HEADER_MAGIC              FOUR_CHAR_ULONG('I','T','E','M')


#define USBH_HCM_POOL_VALID(pPool)               USBH_ASSERT(USBH_IS_PTR_VALID((pPool),       USBH_HCM_POOL))
#define USBH_HCM_ASSERT_ITEM_HEADER(pItemHeader) USBH_ASSERT(USBH_IS_PTR_VALID((pItemHeader), USBH_HCM_ITEM_HEADER))
#define USBH_OCHI_IS_DEV_VALID(pDev)             USBH_ASSERT(USBH_IS_PTR_VALID(pDev,          USBH_OHCI_DEVICE))

// Some Macros used for calculation of structure pointers.

// Calculate the byte offset of a field in a structure of type type.
// @func long | STRUCT_FIELD_OFFSET |
//   This macro calculates the offset of <p field> relative to the base of the structure <p type>.
// @parm IN<spc>| type |
//   Type name of the structure
// @parm IN<spc>| field |
//   Field name
// @rdesc
//   Offset of the field <p field> relative to the base of the structure <p type>.
// lint -emacro({413},STRUCT_FIELD_OFFSET)
// lint -emacro({613},STRUCT_FIELD_OFFSET)
#define STRUCT_FIELD_OFFSET(type, field)((long)&(((type *)0)->field) )

// Calculate the pointer to the base of the structure given its type and a pointer to a field within the structure.
// @func (type *) | STRUCT_BASE_POINTER |
//   This macro calculates the pointer to the base of the structure given its type and a pointer to a field within the structure.
// @parm IN<spc>| fieldptr |
//   Pointer to the field <p field> of the structure
// @parm IN<spc>| type |
//   Type name of the structure
// @parm IN<spc>| field |
//   Field name
// @rdesc
//   Address of the structure which contains <p field>.
// @comm
//   The returned pointer is of type 'pointer to <p type>'.
// lint -emacro({413},STRUCT_BASE_POINTER)
// lint -emacro({613},STRUCT_BASE_POINTER)
#define STRUCT_BASE_POINTER(fieldptr, type, field)            ((type *)(((char *)(fieldptr)) - ((char *)(&(((type *)0)->field)))))



// Needs the struct and the name of the list entry inside the struct
#define GET_HCMITEM_FROM_ENTRY(pListEntry)                    STRUCT_BASE_POINTER(pListEntry, USBH_HCM_ITEM_HEADER,       Link.ListEntry)
#define GET_BUFFER_FROM_ENTRY(pListEntry)                     STRUCT_BASE_POINTER(pListEntry, USBH_URB_BUFFER,            ListEntry)
#define GET_ENUM_ERROR_NOTIFICATION_FROM_ENTRY(pListEntry)    STRUCT_BASE_POINTER(pListEntry, ENUM_ERROR_NOTIFICATION,    ListEntry)
#define GET_PNP_NOTIFICATION_FROM_ENTRY(pListEntry)           STRUCT_BASE_POINTER(pListEntry, USBH__PNP_NOTIFICATION,     ListEntry)
#define GET_DELAYED_PNP_NOTIFY_CONTEXT_FROM_ENTRY(pListEntry) STRUCT_BASE_POINTER(pListEntry, DELAYED_PNP_NOTIFY_CONTEXT, ListEntry)
#define GET_INTERFACE_ENTRY_FROM_ENTRY(pListEntry)            STRUCT_BASE_POINTER(pListEntry, INTERFACE_ENTRY,            ListEntry)
#define GET_CONTROL_EP_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_OHCI_EP0,              ListEntry)
#define GET_HCM_ITEM_HEADER_FROM_ENTRY(pListEntry)            STRUCT_BASE_POINTER(pListEntry, USBH_HCM_ITEM_HEADER,       Link.ListEntry)
#define GET_URB_HEADER_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_HEADER,                ListEntry)
#define GET_CONTROL_EP_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_OHCI_EP0,              ListEntry)
#define GET_BULKINT_EP_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_OHCI_BULK_INT_EP,      ListEntry)
#define GET_ISO_EP_FROM_ENTRY(pListEntry)                     STRUCT_BASE_POINTER(pListEntry, USBH_OHCI_ISO_EP,           ListEntry)
#define GET_HUB_PORT_PTR(pListEntry)                          STRUCT_BASE_POINTER(pListEntry, USBH_HUB_PORT,              ListEntry)
#define GET_BULKINT_EP_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_OHCI_BULK_INT_EP,      ListEntry)
#define GET_USB_DEVICE_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USB_DEVICE,                 ListEntry)
#define GET_USB_DEVICE_FROM_TEMP_ENTRY(pListEntry)            STRUCT_BASE_POINTER(pListEntry, USB_DEVICE,                 TempEntry)
#define GET_HOST_CONTROLLER_FROM_ENTRY(pListEntry)            STRUCT_BASE_POINTER(pListEntry, USBH_HOST_CONTROLLER,       ListEntry)
#define GET_USB_ENDPOINT_FROM_ENTRY(pListEntry)               STRUCT_BASE_POINTER(pListEntry, USB_ENDPOINT,               ListEntry)
#define GET_USB_INTERFACE_FROM_ENTRY(pListEntry)              STRUCT_BASE_POINTER(pListEntry, USB_INTERFACE,              ListEntry)

// The following defines must not be changed!
#define USBH_DEV_MAX_TRANSFER_DESC            (9)                           // default value: 7 (reserved for enumeration)
#define USBH_DEV_DEV_ENUMERATION_CONTROL_EP    4                            // +1=dummy endpoint

#define USBH_MAX_BULK_EP            (USBH_Global.Config.NumBulkEndpoints + 1) // +1=dummy endpoint
#define USBH_MAX_CONTROL_EP         (USBH_Global.Config.NumUSBDevices + USBH_DEV_DEV_ENUMERATION_CONTROL_EP + 1)
#define USBH_MAX_INT_EP             (USBH_Global.Config.NumIntEndpoints)
#define USBH_MAX_ISO_EP             (USBH_Global.Config.NumIsoEndpoints)

// OHD_BULK_INT_EP AbortMask values
#define EP_ABORT_MASK                 0x0001UL // 1. Endpoint is skipped and an timer with an Timeout of about two frames is started
#define EP_ABORT_SKIP_TIME_OVER_MASK  0x0002UL // 2. Endpoint skip Timeout is over
#define EP_ABORT_PROCESS_FLAG_MASK    0x0004UL // 3. temporary flag to process the aborted endpoint
#define EP_ABORT_START_TIMER_MASK     0x0008UL // additional flag to restart the abort timer in the timer routine if another endpoint

#define USBH_EP_STOP_DELAY_TIME  2 // Delay time in ms until an stopped endpoint list is not processed for the HC


// typedef struct URB_BUFFER_POOL {
//   U32                   Magic;
//   USBH_DLIST            ListEntry;
//   U32                   NumberOfBuffer; // Allocated number number of buffers in pool
//   U32                   BufferCt;       // Number of buffers in buffer pool
//   U32                   Size;           // Size of one buffer in bytes
//   USBH_INTERFACE_HANDLE hInterface;
//   U8                    Endpoint;
//   U32                   Index;
//   int                   ResetFlag;
//   int                   BusMasterMemoryFlag;
// } URB_BUFFER_POOL;
//
//
// typedef struct URB_BUFFER {
//   USBH_DLIST        ListEntry;
//   U8              * pTransferBuffer; // Transfer buffer
//   USBH_URB          Urb;             // Allocated URB
//   U32               Size;            // Size of buffer in bytes
//   URB_BUFFER_POOL * pPool;           // Owning pool
//   U32               Index;           // Index number for debugging
// } URB_BUFFER;

// URB_BUFFER_POOL * USBH_URB_CreateTransferBufferPool(USBH_INTERFACE_HANDLE IfaceHandle, U8 Endpoint, U32 SizePerBuffer, U32 BufferNumbers, int BusMasterTransferMemoryFlag);

// URB_BUFFER * USBH_URB_GetFromTransferBufferPool  (URB_BUFFER_POOL * Pool);
// void         USBH_URB_PutToTransferBufferPool    (URB_BUFFER      * Buffer);
// void         USBH_URB_DeleteTransferBufferPool   (URB_BUFFER_POOL * Pool);
// void         USBH_URB_InitUrbBulkTransfer        (URB_BUFFER      * Buffer, USBH_ON_COMPLETION_FUNC * pfOnCompletion, void * Context);
// U32          USBH_URB_GetPendingCounterBufferPool(URB_BUFFER_POOL * Pool);

// Allocates always USBH_TRANSFER_BUFFER_ALIGNMENT aligned transfer buffer from the heap
void * USBH_URB_BufferAllocateTransferBuffer(U32 size);
// Frees buffer allocated with USBH_URB_BufferAllocateTransferBuffer
void   USBH_URB_BufferFreeTransferBuffer(void * pMemBlock);

// This macro need the struct and the name of the list entry inside the struct.

typedef struct ENUM_ERROR_NOTIFICATION {
#if (USBH_DEBUG > 1)
  U32                       Magic;
#endif
  USBH_DLIST                ListEntry;         // To store this object in the BUS_DRIVER object
  void                    * pContext;           // User context / A copy of the parameter passed to USBH_RegisterEnumErrorNotification
  USBH_ON_ENUM_ERROR_FUNC * pfOnEnumError;
} ENUM_ERROR_NOTIFICATION;

void   USBH_BD_FireEnumErrorNotification                (USBH_ENUM_ERROR * pEnumError);                                                          // Walk trough the device driver enum error notification list and call registered notify callback routines!
void   USBH_BD_SetEnumErrorNotificationRootPortReset    (USBH_HUB_PORT   * pPort, USBH_ROOT_HUB_PORTRESET_STATE state,  USBH_STATUS status);      // Notify about an root hub pPort reset error
void   USBH_BD_SetEnumErrorNotificationProcessDeviceEnum(USBH_HUB_PORT   * pPort, DEV_ENUM_STATE state,      USBH_STATUS status, int hub_flag);   // Notify about a USB device enumeration error
void   USBH_BD_SetEnumErrorNotificationHubPortReset(     USBH_HUB_PORT   * pPort, USBH_HUB_PORTRESET_STATE state, USBH_STATUS status);  // Notify about an external hub pPort reset error

#define USBH_MAX_RECURSIVE 20

#if (USBH_DEBUG > 1)
  #define INC_RECURSIVE_CT(funcname)                                                   \
    static int USBH_recursive_ct;                                                      \
    USBH_recursive_ct++;                                                               \
    if (USBH_recursive_ct > USBH_MAX_RECURSIVE) {                                      \
      USBH_WARN((USBH_MTYPE_CORE, ""#funcname ":recursive-ct:%ld",USBH_recursive_ct)); \
    }

  // The second test is only for testing of the macro
  #define DEC_RECURSIVE_CT(funcname)                                                        \
    if(0>=USBH_recursive_ct){                                                               \
      USBH_WARN((USBH_MTYPE_CORE, ""#funcname ":recursive <= 0 ct:%ld",USBH_recursive_ct)); \
    }                                                                                       \
    USBH_recursive_ct--
#else
  #define INC_RECURSIVE_CT(funcname)
  #define DEC_RECURSIVE_CT(funcname)
#endif

// Return the pointer to the beginning of the descriptor or NULL if not Desc. is found
// const void * PrevDesc - Pointer to a descriptor
// int * Length          - IN:  Remaining bytes from Desc.
//                         OUT: If the descriptor is found then that is the remaining length from the beginning of the returned descriptor
// int DescType          - Descriptor type, see USB spec
const void * USBH_SearchNextDescriptor(const void * PrevDesc, int * Length, int DescType);


// This macro need the struct and the name of the list entry inside the struct.

typedef struct USBH__PNP_NOTIFICATION { // The USB device object
#if (USBH_DEBUG > 1)
  U32                   Magic;
#endif
  USBH_DLIST                 ListEntry;       // To store this object in the BUS_DRIVER object
  USBH_PNP_NOTIFICATION UbdNotification; // A copy of the notification passed to USBH_RegisterPnPNotification
} USBH__PNP_NOTIFICATION;


// Used for indirect calling of the user notification routine
typedef struct DELAYED_PNP_NOTIFY_CONTEXT {
#if (USBH_DEBUG > 1)
  U32 Magic;
#endif
  // To store this object in the BUS_DRIVER object
  USBH_DLIST               ListEntry;
  void                   * pContext;
  USBH_PNP_EVENT           Event;
  USBH_ON_PNP_EVENT_FUNC * pfNotifyCallback;
  USBH_INTERFACE_ID        Id;
} DELAYED_PNP_NOTIFY_CONTEXT;

USBH__PNP_NOTIFICATION * USBH_PNP_NewNotification    (USBH_PNP_NOTIFICATION  * pfOnUbdNotification);
void                     USBH_PNP_ReleaseNotification(USBH__PNP_NOTIFICATION * pfOnPnpNotification);

// If this interface matches with the interface Mask of pfOnPnpNotification the event notification function is called with the event.
// Parameters:
//   pfOnPnpNotification: Pointer to the notification
//   pDev:             Pointer to an device
//   event:           device is connected, device is removed!
//                    Normally one device at the time is changed!
void USBH_PNP_ProcessDeviceNotifications(USBH__PNP_NOTIFICATION * pfOnPnpNotification, USB_DEVICE * pDev, USBH_PNP_EVENT Event);

// Check the notification against all interfaces. If an device is removed or connected and the interface matches
// and the event has been not sent the notification function is called.
void USBH_PNP_ProcessNotification(USBH__PNP_NOTIFICATION * PnpNotification);
void USBH_PNP_NotifyWrapperCallbackRoutine(void    * Context);

// This macro need the struct and the name of the list entry inside the struct

// The interface list object based on one host controller!
typedef struct INTERFACE_LIST {
  USBH_DLIST   UsbInterfaceEntryList; // List for interfaces of type INTERFACE_ENTRY
  unsigned int InterfaceCount;        // Number of entries in the UsbInterfaceList
} INTERFACE_LIST;

// the entry to keep this object in the InterfaceList
typedef struct INTERFACE_ENTRY {
#if (USBH_DEBUG > 1)
  U32                      Magic;
#endif
  USBH_DLIST               ListEntry;
  USBH_HOST_CONTROLLER   * HostController; // Pointer to the owning host controller
  USBH_INTERFACE_ID        InterfaceID;    // The interface ID
} INTERFACE_ENTRY;


U8   USBH_ReadReg8  (U8  * pAddr);
U16  USBH_ReadReg16 (U16 * pAddr);
void USBH_WriteReg32(U8  * pAddr, U32 Value);
U32  USBH_ReadReg32 (U8  * pAddr);

// Endpoint states
typedef enum USBH_EP_STATE {
  USBH_EP_STATE_IDLE,        // The endpoint is not linked
  USBH_EP_STATE_UNLINK,      // If the timer routine runs then the endpoint is removed and deleted
  USBH_EP_STATE_LINKED,        // The endpoint is linked
  USBH_EP_STATED_UNLINKED_TIMER // Endpoint is unlinked but the current timer routine must restart the timer
} USBH_EP_STATE;


// Control endpoint states
typedef enum USBH_EP0_PHASE {
  ES_IDLE   = 0,
  ES_SETUP,
  ES_DATA,
  ES_COPY_DATA,
  ES_PROVIDE_HANDSHAKE,
  ES_HANDSHAKE,
  ES_ERROR
} USBH_EP0_PHASE;

typedef struct SETUP_BUFFER {
  // Recommended!!!:
  //   first filed:  USBH_HCM_ITEM_HEADER
  //   second field: U8 EndpointType
  USBH_HCM_ITEM_HEADER ItemHeader;
} SETUP_BUFFER;

void             USBH_OnISREvent                     (void);
void             USBH_Free                           (void               * pMemBlock);
void           * USBH_Malloc                         (U32                  Size);
void           * USBH_MallocZeroed                   (U32                  Size);
void           * USBH_TryMalloc                      (U32                  Size);
void           * USBH_AllocTransferMemory            (U32                  NumBytes, unsigned Alignment);

USBH_HC_HANDLE USBH_STM32_CreateController(void * pBaseAddress);
typedef void REMOVE_HC_COMPLETION_FUNC (void * pContext);


/*********************************************************************
*
*       Utility functions
*
*  RS: Maybe we should move them into a UTIL module some time ? (We can keep macros here for compatibility)
*
**********************************************************************
*/
I32          USBH_BringInBounds(I32 v, I32 Min, I32 Max);
U32          USBH_LoadU32BE(const U8 * pData);
U32          USBH_LoadU32LE(const U8 * pData);
U32          USBH_LoadU32TE(const U8 * pData);
unsigned     USBH_LoadU16BE(const U8 * pData);
unsigned     USBH_LoadU16LE(const U8 * pData);
void         USBH_StoreU16BE     (U8 * p, unsigned v);
void         USBH_StoreU16LE     (U8 * p, unsigned v);
void         USBH_StoreU32BE     (U8 * p, U32      v);
void         USBH_StoreU32LE     (U8 * p, U32      v);
U32          USBH_SwapU32                (U32      v);

void         USBH_HC_ClrActivePortReset(USBH_HOST_CONTROLLER * pHost, USBH_HUB_PORT * pEnumPort);
void         USBH_HC_SetActivePortReset(USBH_HOST_CONTROLLER * pHost, USBH_HUB_PORT * pEnumPort);

const char * USBH_PortSpeed2Str(USBH_SPEED    x);
const char * USBH_GetStatusStr(USBH_STATUS   x);
const char * USBH_PortState2Str(PORT_STATE    x);
const char * USBH_UrbFunction2Str(USBH_FUNCTION x);

const char * USBH_HubPortResetState2Str   (USBH_HUB_PORTRESET_STATE x);
const char * USBH_HubNotificationState2Str(USBH_HUB_NOTIFY_STATE    x);
const char * USBH_HubEnumState2Str        (USBH_HUB_ENUM_STATE      x);
const char * USBH_EnumState2Str       (DEV_ENUM_STATE        x);
const char * USBH_RhPortResetState2Str(USBH_ROOT_HUB_PORTRESET_STATE    x);
const char * USBH_HcState2Str         (HOST_CONTROLLER_STATE x);
const char * USBH_Ep0State2Str        (USBH_EP0_PHASE x);

void         USBH_DLIST_Append     (USBH_DLIST * ListHead, USBH_DLIST * List);
void         USBH_DLIST_InsertTail (USBH_DLIST * ListHead, USBH_DLIST * Entry);
void         USBH_DLIST_InsertHead (USBH_DLIST * ListHead, USBH_DLIST * Entry);
void         USBH_DLIST_InsertEntry(USBH_DLIST * Entry,    USBH_DLIST * NewEntry);
void         USBH_DLIST_RemoveTail (USBH_DLIST * ListHead, USBH_DLIST ** Entry);
void         USBH_DLIST_RemoveHead (USBH_DLIST * ListHead, USBH_DLIST ** Entry);
void         USBH_DLIST_RemoveEntry(USBH_DLIST * Entry);
USBH_DLIST * USBH_DLIST_GetPrev    (USBH_DLIST * Entry);
USBH_DLIST * USBH_DLIST_GetNext    (USBH_DLIST * Entry);
int          USBH_DLIST_IsEmpty    (USBH_DLIST * ListHead);
void         USBH_DLIST_Init       (USBH_DLIST * ListHead);
void         USBH_DLIST_Move       (USBH_DLIST * pHead, USBH_DLIST * pItem);
int          USBH_DLIST_Contains1Item(USBH_DLIST * pList);
void         USBH_DLIST_Delete       (USBH_DLIST * Entry);
void         USBH_DLIST_AddItem      (USBH_DLIST * pHead, USBH_DLIST * pNew);
void         USBH_DLIST_JoinTail     (USBH_DLIST * pHead, USBH_DLIST * pList);


/*********************************************************************
*
*       USBH_DLIST
*/
typedef struct USBH_DLIST_ITEM {
  struct USBH_DLIST_ITEM * pNext;
  struct USBH_DLIST_ITEM * pPrev;
} USBH_DLIST_ITEM;

typedef struct {
  struct USBH_DLIST_ITEM * pFirst;
  int NumItems;
} USBH_DLIST_HEAD;

#define USBH_DLIST_LIST_ENTRY(p, Type, Member)                              STRUCT_BASE_POINTER(p, Type, Member)
#define USBH_DLIST_FOR_EACH_ITEM_SAFE(pPos, nItem, pHead)                   for (pPos  = (pHead)->pNext, nItem = pPos->pNext; pPos != (pHead); pPos = nItem, nItem = pPos->pNext)
#define USBH_DLIST_FOR_EACH_ENTRY_SAFE(pPos, Type, pItem, pHead, Member)    for (pPos  = USBH_DLIST_LIST_ENTRY((pHead)->pNext, Type, Member),  \
                                                                                 pItem = USBH_DLIST_LIST_ENTRY(pPos->Member.pNext, Type, Member); \
                                                                                 &pPos->Member != (pHead);                               \
                                                                                 pPos = pItem, pItem = USBH_DLIST_LIST_ENTRY(pItem->Member.pNext, Type, Member))


void USBH_DLIST_Remove(USBH_DLIST_HEAD * pHead, USBH_DLIST_ITEM * pItem);
void USBH_DLIST_Add   (USBH_DLIST_HEAD * pHead, USBH_DLIST_ITEM * pNew);

void     USBH__ConvSetupPacketToBuffer(const USBH_SETUP_PACKET * Setup, U8 * pBuffer);
unsigned USBH_BITFIELD_CalcNumBitsUsed(U32 NumItems);
U32      USBH_BITFIELD_ReadEntry      (const U8 * pBase, U32 Index, unsigned NumBits);
void     USBH_BITFIELD_WriteEntry     (      U8 * pBase, U32 Index, unsigned NumBits, U32 v);
unsigned USBH_BITFIELD_CalcSize       (U32 NumItems, unsigned BitsPerItem);


unsigned USBH_CountLeadingZeros(U32 Value);

#undef EXTERN

#if defined(__cplusplus)
  }
#endif

#endif

/******************************* EOF ********************************/
