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
File        : USBH_HW_STM32F2xx_FS.h
Purpose     : Header for the STM32F2xx/F4xx FullSpeed emUSB Host driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __USBH_HW_STM32F2XX_FS_H__
#define __USBH_HW_STM32F2XX_FS_H__

#include "USBH_Int.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif


#define OTG_REG_BASE        (0x50000000UL)
#define OTG_FS_NUM_CHANNELS 8

typedef struct {
  volatile U32  HCCHAR;      // OTG_FS host channel characteristics register
  volatile U32  Reserved;
  volatile U32  HCINT;       // OTG_FS host channel interrupt register
  volatile U32  HCINTMSK;    // OTG_FS host channel interrupt mask register
  volatile U32  HCTSIZ;      // OTG_FS host channel transfer size register
  volatile U32  aReserved[3];
} USBH_STM32F2_FS_HCCHANNEL;

typedef struct {
  volatile U32  GOTGCTL;
  volatile U32  GOTGINT;
  volatile U32  GAHBCFG;
  volatile U32  GUSBCFG;
  volatile U32  GRSTCTL;
  volatile U32  GINTSTS;
  volatile U32  GINTMSK;
  volatile U32  GRXSTSR;
  volatile U32  GRXSTSP;
  volatile U32  GRXFSIZ;
  volatile U32  GNPTXFSIZ;
  volatile U32  GNPTXSTS;
  volatile U32  aReserved0[2];
  volatile U32  GCCFG;
  volatile U32  CID;
  volatile U32  aReserved1[0x30];
  volatile U32  HPTXFSIZ;
  volatile U32  aReserved2[0xbf];
  volatile U32  HCFG;                // OTG_FS host configuration register
  volatile U32  HFIR;                // OTG_FS Host frame interval register
  volatile U32  HFNUM;               // OTG_FS host frame number/frame time remaining register
  volatile U32  aReserved3[1];
  volatile U32  HPTXSTS;             // Host periodic transmit FIFO/queue Status register
  volatile U32  HAINT;               // OTG_FS Host all channels interrupt register
  volatile U32  HAINTMSK;            // OTG_FS host all channels interrupt mask register
  volatile U32  aReserved4[0x09];
  volatile U32  HPRT;                // OTG_FS host port control and Status register
  volatile U32  aReserved5[0x2f];
  USBH_STM32F2_FS_HCCHANNEL     aHChannel[OTG_FS_NUM_CHANNELS];
  volatile U32  aReserved6[0x1ff];
  volatile U32  PCGCCTL;             // OTG_FS power and clock gating control register
} USBH_STM32F2_FS_HWREGS;

#define OTG_FS_FIFO_BASE   0x50001000
#define OTG_FS_FIFO_OFF    0x1000

#define START_OF_FRAME_INT   (1UL <<  3)   // Start Of Frame
#define HOST_RXFLVL          (1UL <<  4)   // RxFIFO non-empty
#define HOST_NPTXFE          (1UL <<  5)   // Non-periodic TxFIFO empty
#define HOST_CHANNEL_INT     (1UL << 25)   // Host channel
#define HOST_PORT_INT        (1UL << 24)   // USB host port
#define HOST_PTXFE           (1UL << 26)   // Periodic TxFIFO empty


#define CHANNEL_DTERR   (1 << 10) // Data toggle error
#define CHANNEL_FRMOR   (1 <<  9) // Frame overrun
#define CHANNEL_BBERR   (1 <<  8) // Babble error
#define CHANNEL_TXERR   (1 <<  7) // Transaction error Indicates one of the following errors occurred on the USB.
                                  // CRC check failure
                                  // Timeout
                                  // Bit stuff error
                                  // False EOP
#define CHANNEL_ACK     (1UL <<  5) //  ACK response received/transmitted interrupt
#define CHANNEL_NAK     (1UL <<  4) // NAK response received interrupt
#define CHANNEL_STALL   (1UL <<  3) // STALL response received interrupt
#define CHANNEL_CHH     (1UL <<  1) // Channel halted
                                  // Indicates the transfer completed abnormally either because of any USB transaction error or in
                                  // response to disable request by the application.
#define CHANNEL_XFRC    (1UL <<  0) //  Transfer completed Transfer completed normally without any errors.

#define DATA_PID_DATA0  (0)
#define DATA_PID_DATA1  (2)
#define DATA_PID_DATA2  (1)
#define DATA_PID_MDATA  (3)
#define DATA_PID_SETUP  (3)


#define STATUS_IN_PACKET_RECEIVED                 2  // IN data packet received
#define STATUS_XFER_COMP                          3  // IN transfer completed (triggers an interrupt)
#define STATUS_DATA_TOGGLE_ERROR                  5  // Data toggle error (triggers an interrupt)
#define STATUS_CHANNEL_HALTED                     7  // Channel halted (triggers an interrupt)


#define EP0_VALID(pEPInfo)                                    USBH_HCM_ASSERT_ITEM_HEADER(&pEPInfo->ItemHeader)
#define EPX_VALID(pEPx)                                    USBH_HCM_ASSERT_ITEM_HEADER(&pEPx->ItemHeader)
#define GET_EP0INFO_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_STM32F2_FS_EP0_INFO, ListEntry)
#define GET_EPXINFO_FROM_ENTRY(pListEntry)                 STRUCT_BASE_POINTER(pListEntry, USBH_STM32F2_FS_EPX_INFO, ListEntry)
#define USBH_STM32F2_FS_INST_MAGIC                              FOUR_CHAR_ULONG('S','T','M','I')

typedef struct USBH_STM32F2_FS_CHANNEL_INFO {
  volatile U8                InUse;
  volatile void            * pEPInfo;
  volatile U8                EndpointAddress;
  volatile U32               NumBytes2Transfer;
  volatile U32               NumBytesTransferred;
  volatile U32               NumBytesPushed;
  volatile U8                ErrorCount;
  volatile U8                TransferDone;
  volatile USBH_STATUS       Status;
           U8              * pBuffer;
  volatile USBH_TIMER_HANDLE hIntervalTimer;
} USBH_STM32F2_FS_CHANNEL_INFO;

typedef struct USBH_STM32F2_FS_INST { // The global driver object. The object is cleared in the function USBH_HostInit!
#if (USBH_DEBUG > 1)
  U32 Magic;
#endif
  // EP pools
  USBH_HCM_POOL        ControlEPPool;
  USBH_HCM_POOL        BulkEPPool;
  USBH_HCM_POOL        IntEPPool;
  // Control endpoints
  USBH_DLIST           ControlEpList; // Number of pending endpoints on the HC
  U32                  ControlEpCount;
  USBH_TIMER_HANDLE    ControlEpRemoveTimer;
  USBH_BOOL            ControlEpRemoveTimerRunFlag;        // True if timer is started, set to false in the timer routine
  USBH_BOOL            ControlEpRemoveTimerCancelFlag;
  USBH_BOOL            ControlEpAbortTimerRunFlag;
  USBH_BOOL            ControlEpAbortTimerCancelFlag;
  // Bulk endpoints
  USBH_DLIST           BulkEpList;
  U32                  BulkEpCount;
  USBH_TIMER_HANDLE    hBulkEpRemoveTimer;
  USBH_BOOL            BulkEpRemoveTimerRunFlag;           // True if timer is started, set to false in the timer routine
  USBH_BOOL            BulkEpRemoveTimerCancelFlag;
  USBH_BOOL            BulkEpAbortTimerRunFlag;            // True if timer is started, set to false in the timer routine
  USBH_BOOL            BulkEpAbortTimerCancelFlag;
  // Int endpoints
  USBH_DLIST           IntEpList;
  U32                  IntEpCount; // IntEpCount is an reference counter that counts the number of active interrupt endpoints without the dummy interrupt endpoints
  USBH_TIMER_HANDLE    hIntEpRemoveTimer;
  USBH_BOOL            IntEpRemoveTimerRunFlag;            // True if timer is started, set to false in the timer routine
  USBH_BOOL            IntEpRemoveTimerCancelFlag;
  USBH_BOOL            IntEpAbortTimerRunFlag;             // True if timer is started, set to false in the timer routine
  USBH_BOOL            IntEpAbortTimerCancelFlag;
#if (USBH_DEBUG > 1)
  USBH_BOOL            IntRemoveFlag;
#endif
  USBH_DLIST           IsoEpList;
  U32                  IsoEpCount;
  USBH_TIMER_HANDLE    hInitDeviceTimer; // This timer is used in the initialization routine
  volatile U32         FrameCounter;
  USBH_STM32F2_FS_HWREGS  * pHWReg;         // Register base address
  USBH_DEV_STATE       State;          // Devices state
  USBH_HC_BD_HANDLE    hBusDriver;
  // pfHostExit Callback function and pContext
  REMOVE_HC_COMPLETION_FUNC       * pfRemoveCompletion;
  void                            * pRemoveContext;
  USBH_ROOT_HUB_NOTIFICATION_FUNC * pfUbdRootHubNotification;
  void                            * pRootHubNotificationContext;
  U32                               PortStatus;
  U32                               IntStatus;
  I32                               TxIntCnt;
  I32                               NTxIntCnt;
  I32                               RxIntCnt;
  USBH_STM32F2_FS_CHANNEL_INFO           aChannelInfo[OTG_FS_NUM_CHANNELS];

} USBH_STM32F2_FS_INST;

typedef struct USBH_STM32F2_FS_GENERIC_EP_DATA {
  U8                    EndpointType;
  U8                    DeviceAddress;
  U8                    EndpointAddress;
  U16                   MaxPacketSize;
  U16                   IntervalTime;
  U8                    LastDataPid;
  USBH_SPEED            Speed;
  USBH_STM32F2_FS_INST     * pDev;
  U8                    State;
  U8                    AbortMask;
} USBH_STM32F2_FS_GENERIC_EP_DATA;

typedef struct {
  USBH_HCM_ITEM_HEADER  ItemHeader;
  USBH_STM32F2_FS_GENERIC_EP_DATA       EPData;
  USBH_EP0_PHASE        Phase;
  USBH_DLIST            ListEntry;
  USBH_DLIST            UrbList;
  U32                   UrbCount;
  USBH_URB            * pPendingUrb;
  U8                    aSetup[8];            //
  USBH_RELEASE_EP_COMPLETION_FUNC * pfOnReleaseCompletion;
  void                            * pReleaseContext;
} USBH_STM32F2_FS_EP0_INFO;

typedef struct USBH_STM32F2_FS_EPX_INFO {
  USBH_HCM_ITEM_HEADER              ItemHeader;
  USBH_STM32F2_FS_GENERIC_EP_DATA        EPData;
  USBH_DLIST                        UrbList;
  USBH_DLIST                        ListEntry;
  U32                               UrbCount;
  USBH_URB                        * pPendingUrb;
  USBH_BOOL                         CancelPendingFlag;
  USBH_BOOL                         HaltFlag;
  USBH_RELEASE_EP_COMPLETION_FUNC * pfOnReleaseCompletion;
  void                            * pReleaseContext;
} USBH_STM32F2_FS_EPX_INFO;

#define USBH_STM32F2_FS_IS_DEV_VALID(pDev)       USBH_ASSERT(USBH_IS_PTR_VALID(pDev, USBH_STM32F2_FS_INST))
#define USBH_STM32F2_FS_HANDLE_TO_PTR(pDev,USBH_hc_handle)     (pDev) = ((USBH_STM32F2_FS_INST *)(USBH_hc_handle))



#define USBH_STM32F4_FS_Add(pBase)          USBH_STM32F2_FS_Add(pBase)


void USBH_STM32F2_FS_Add(void * pBase);
USBH_HC_HANDLE USBH_STM32F2_FS_CreateController(void * pBaseAddress);

void         USBH_STM32F2_FS_ROOTHUB_HandlePortInt   (USBH_STM32F2_FS_INST * pInst);
unsigned int USBH_STM32F2_FS_ROOTHUB_GetPortCount    (USBH_HC_HANDLE hHostController);
unsigned int USBH_STM32F2_FS_ROOTHUB_GetPowerGoodTime(USBH_HC_HANDLE hHostController);
U32          USBH_STM32F2_FS_ROOTHUB_GetHubStatus    (USBH_HC_HANDLE hHostController);
void         USBH_STM32F2_FS_ROOTHUB_ClearHubStatus  (USBH_HC_HANDLE hHostController, U16 FeatureSelector);
U32          USBH_STM32F2_FS_ROOTHUB_GetPortStatus   (USBH_HC_HANDLE hHostController, U8  Port);
void         USBH_STM32F2_FS_ROOTHUB_ClearPortStatus (USBH_HC_HANDLE hHostController, U8  Port, U16 FeatureSelector);
void         USBH_STM32F2_FS_ROOTHUB_SetPortPower    (USBH_HC_HANDLE hHostController, U8  Port, U8 PowerOn);
void         USBH_STM32F2_FS_ROOTHUB_ResetPort       (USBH_HC_HANDLE hHostController, U8  Port);
void         USBH_STM32F2_FS_ROOTHUB_DisablePort     (USBH_HC_HANDLE hHostController, U8  Port);
void         USBH_STM32F2_FS_ROOTHUB_SetPortSuspend  (USBH_HC_HANDLE hHostController, U8  Port, USBH_PORT_POWER_STATE State);

U8                        USBH_STM32F2_FS_CHANNEL_Allocate     (USBH_STM32F2_FS_INST * pInst, U8 EndpointType);
USBH_STM32F2_FS_CHANNEL_INFO * USBH_STM32F2_FS_CHANNEL_GetInfo      (USBH_STM32F2_FS_INST * pInst, U32 Channel);
void                      USBH_STM32F2_FS_CHANNEL_Open         (USBH_STM32F2_FS_INST * pInst, U32 Channel, USBH_STM32F2_FS_GENERIC_EP_DATA * pEPData);
void                      USBH_STM32F2_FS_CHANNEL_StartTransfer(USBH_STM32F2_FS_INST * pInst, U32 Channel, USBH_STM32F2_FS_GENERIC_EP_DATA * pEPData, U8 * pBuffer, U32 NumBytes2Transfer, U8 DataPid);
USBH_STATUS               USBH_STM32F2_FS_CHANNEL_GetStatus    (USBH_STM32F2_FS_INST * pInst, U32 Channel);
void                      USBH_STM32F2_FS_CHANNEL_Disable      (USBH_STM32F2_FS_INST * pInst, U32 Channel);
void                      USBH_STM32F2_FS_CHANNEL_DeAllocate   (USBH_STM32F2_FS_INST * pInst, U32 Channel);
void                      USBH_STM32F2_FS_CHANNEL_ReStartTx    (USBH_STM32F2_FS_INST * pInst, U32 Channel, U8 IsPeriodic);
void                      USBH_STM32F2_FS_CHANNEL_ReStartRx    (USBH_STM32F2_FS_INST * pInst, U32 Channel);


void                  USBH_STM32F2_FS_OnRemoveEPxBulkTimer (void * pContext);
void                  USBH_STM32F2_FS_OnRemoveEPxIntTimer  (void * pContext);
void                  USBH_STM32F2_FS_OnIntIntervalTimer   (void * pContext);
void                  USBH_STM32F2_FS_OnReleaseEP0Timer    (void * pContext);

USBH_STATUS           USBH_STM32F2_FS_AbortEP0             (USBH_STM32F2_FS_EP0_INFO * pEPInfo);
USBH_STATUS           USBH_STM32F2_FS_AddUrb2EP0           (USBH_STM32F2_FS_EP0_INFO * pEPInfo, USBH_URB * pUrb);
USBH_STATUS           USBH_STM32F2_FS_AllocEP0Pool         (USBH_HCM_POOL * pEpPool, U32 NumItems);
USBH_STM32F2_FS_EP0_INFO * USBH_STM32F2_FS_GetEP0FromPool       (USBH_HCM_POOL * pEpPool);
void                  USBH_STM32F2_FS_HandleEP0            (USBH_STM32F2_FS_INST * pInst, U8 Channel);
USBH_STATUS           USBH_STM32F2_FS_InitEP0              (USBH_STM32F2_FS_EP0_INFO * pEPInfo, USBH_STM32F2_FS_INST * pDev, U8 DeviceAddress, U8 EndpointAddress, U16 MaxFifoSize, USBH_SPEED Speed);
void                  USBH_STM32F2_FS_InsertEP0            (USBH_STM32F2_FS_EP0_INFO * pEPInfo);
void                  USBH_STM32F2_FS_PutEP0ToPool         (USBH_STM32F2_FS_EP0_INFO * pEPInfo);
void                  USBH_STM32F2_FS_ReleaseEP0           (USBH_STM32F2_FS_EP0_INFO * pEPInfo, USBH_RELEASE_EP_COMPLETION_FUNC * pfReleaseEpCompletion, void * pContext);

USBH_STATUS           USBH_STM32F2_FS_AbortEPx             (USBH_STM32F2_FS_EPX_INFO * pEPInfo);
USBH_STATUS           USBH_STM32F2_FS_AddUrb2EPx           (USBH_STM32F2_FS_EPX_INFO * pEP, USBH_URB * pUrb);
USBH_STATUS           USBH_STM32F2_FS_AllocEPxPool         (USBH_HCM_POOL * pEpPool, unsigned int MaxEps);
USBH_STM32F2_FS_EPX_INFO * USBH_STM32F2_FS_GetEPxFromPool       (USBH_HCM_POOL * pEpPool);
void                  USBH_STM32F2_FS_HandleEPx            (USBH_STM32F2_FS_INST * pInst, U8 Channel);
USBH_STM32F2_FS_EPX_INFO * USBH_STM32F2_FS_InitEPx              (USBH_STM32F2_FS_EPX_INFO * pEP, USBH_STM32F2_FS_INST * pDev, U8 EndpointType, U8 DeviceAddress, U8 EndpointAddress, U16 MaxFifoSize, U16 IntervalTime, USBH_SPEED Speed, U32 Flags);
void                  USBH_STM32F2_FS_InsertEPBulk         (USBH_STM32F2_FS_EPX_INFO * pEP);
void                  USBH_STM32F2_FS_InsertEPInt          (USBH_STM32F2_FS_EPX_INFO * pEP);
void                  USBH_STM32F2_FS_PutEPxToPool         (USBH_STM32F2_FS_EPX_INFO * pEP);
void                  USBH_STM32F2_FS_ReleaseEPx           (USBH_STM32F2_FS_EPX_INFO * pEPInfo, USBH_RELEASE_EP_COMPLETION_FUNC * pfReleaseEpCompletion, void * pContext);
void                  USBH_STM32F2_FS_SubmitEPxUrbsFromList(USBH_STM32F2_FS_EPX_INFO * pEPInfo);

#if defined(__cplusplus)
  }
#endif

#endif // __USBH_HW_STM32F2XX_FS_H__

/********************************* EOF ******************************/
