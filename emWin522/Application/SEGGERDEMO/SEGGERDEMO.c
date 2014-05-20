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
File    : SEGGERDEMO.c
Purpose : SEGGER product presentation
--------  END-OF-HEADER  ---------------------------------------------
*/

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define SEGGERDEMO_DELAY  3000  // Generic delay for presentations [ms]

/*********************************************************************
*
*       Disable any middleware except for GUI if running in
*       simulation. VNC is emulated by GUI simulation.
*       This ignores configuration in SysConf.h.
*
**********************************************************************
*/
#ifdef _WINDOWS
  #ifndef   INCLUDE_IP
    #define INCLUDE_IP         (0)
  #endif
  #ifndef   INCLUDE_VNC
    #define INCLUDE_VNC        (0)
  #endif
  #ifndef   INCLUDE_FS
    #define INCLUDE_FS         (1)
  #endif
  #ifndef   INCLUDE_USB
    #define INCLUDE_USB        (0)
  #endif
  #ifndef   INCLUDE_USBH
    #define INCLUDE_USBH       (1)
  #endif
#endif

/*********************************************************************
*
*       Includes, generic
*
**********************************************************************
*/

#include "SysConf.h"     // Configures what the SEGGERDEMO will show, overrides defines used in GUIDEMO.h
#if INCLUDE_GUI
  #include "GUIDEMO.h"
#endif

/*********************************************************************
*
*       Includes, hardware/simulation specific
*
**********************************************************************
*/

#ifndef _WINDOWS
  #include "RTOS.h"
  #include "BSP.h"
#endif

/*********************************************************************
*
*       Includes, SysConf.h specific
*
**********************************************************************
*/

#if INCLUDE_FS
  #include "FS.h"
#endif

#if INCLUDE_IP
  #include "IP.h"
#endif

#if INCLUDE_GUI
  #if INCLUDE_VNC
    #include "GUI_VNC.h"
  #endif
#endif

#if ((INCLUDE_IP == 1) || (INCLUDE_FS == 1) || (INCLUDE_USB == 1) || (INCLUDE_USBH == 1))
  #include "TaskPrio.h"
  #if INCLUDE_GUI
    #include "WM.h"  // Needed for trial expired window and IP window
  #endif
#endif

//
// Decide between USB or USBH
//
#if (INCLUDE_USBH && USE_USB_HOST_HID)
  #include "USB.h"
  #include "USBH.h"
  #include "USBH_HID.h"
#else
  #if (INCLUDE_USB && INCLUDE_FS)
    #include "USB.h"
    #include "USB_MSD.h"
  #endif
#endif

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/

#if INCLUDE_GUI
extern GUI_CONST_STORAGE GUI_BITMAP bmIMAGE_SeggerLogo_300x150_565c;
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// USB Host HID sample
//
#define MOUSE_EVENT     (1 << 0)
#define KEYBOARD_EVENT  (1 << 1)

//
// Trial window
//
#if INCLUDE_GUI
  #define TRIALWIN_XSIZE  (200)
  #define TRIALWIN_YSIZE  (70)
  #define TRIALWIN_XPOS   ((LCD_GetXSize() / 2) - (TRIALWIN_XSIZE / 2))
  #define TRIALWIN_YPOS   ((LCD_GetYSize() / 2) - (TRIALWIN_YSIZE / 2))

  #if (OS_VERSION >= 38220)
    // Since OS version 3.82t, the OS trial time limit is 12 hours = 43200 seconds = 43200000 OS ticks
    #define MAX_TRIAL_TIME_VALUE  43140000uL
    #define MAX_TRIAL_TIME_TEXT  "12 hours"
  #else
    // The OS trial time limit is 15 minutes = 900 seconds = 900000 OS ticks
    #define MAX_TRIAL_TIME_VALUE  870000uL
    #define MAX_TRIAL_TIME_TEXT  "15 minutes"
  #endif
#endif

/*********************************************************************
*
*       Static data, SEGGERDEMO
*
**********************************************************************
*/

#if INCLUDE_GUI
  #if (!defined _WINDOWS) && (((INCLUDE_IP == 1) || (INCLUDE_USB == 1) || (INCLUDE_USBH == 1) || (INCLUDE_FS == 1)))
  static OS_STACKPTR int _TrialWindow_Stack[512];
  static OS_TASK         _TrialWindow_TCB;
  #endif
#endif

#if INCLUDE_GUI
  #if ((!defined _WINDOWS) && USE_GUI_TASK)
  //
  // GUI task stack can be linked to a specific section by defining the section GUIDEMO_STACK in the linker file
  //
  #ifdef __ICCARM__
    #pragma location="GUIDEMO_STACK"
    static __no_init U32 _GUITask_Stack[1024];
  #endif
  #ifdef __CC_ARM
    U32 static _GUITask_Stack[1024] __attribute__ ((section ("GUIDEMO_STACK"), zero_init));
  #endif
  #ifdef _WINDOWS
    static U32 _aMemory[GUI_NUMBYTES / 4];
  #endif
  #ifdef __RX
    static U32 _GUITask_Stack[1024];
  #endif
  #ifdef __GNUC__
    static U32 _GUITask_Stack[1024];
  #endif

  static OS_TASK         _GUITask_TCB;
  #endif
#endif

/*********************************************************************
*
*       Static data, IP
*
**********************************************************************
*/

#if INCLUDE_IP
  extern void WebserverTask(void);
  #if (INCLUDE_FTP == 1)
    extern void FTPServerTask(void);
  #endif
#endif

#if INCLUDE_IP
  #if INCLUDE_WEBSERVER
    static OS_STACKPTR int _WEBServer_Stack[1024];
    static OS_TASK         _WEBServer_TCB;
  #endif
  #if INCLUDE_FTP
    static OS_STACKPTR int _FTPServer_Stack[4096];
    static OS_TASK         _FTPServer_TCB;
  #endif
  static OS_STACKPTR int _IP_Stack[512];
  static OS_TASK         _IP_TCB;
  static OS_STACKPTR int _IPRx_Stack[256];
  static OS_TASK         _IPRx_TCB;
  #if INCLUDE_GUI
  static OS_STACKPTR int _EthWindow_Stack[384];
  static OS_TASK         _EthWindow_TCB;

  static FRAMEWIN_Handle _hEthFrameWin;
  static FRAMEWIN_Handle _hEthWin1;
  static U32             _IPAddr;
  static char            _acIP[16];
  #endif
#endif

/*********************************************************************
*
*       Static data, USB
*
**********************************************************************
*/

#if (INCLUDE_USBH && USE_USB_HOST_HID)
  //
  // defines configurable
  //
  #define MAX_DATA_ITEMS          10
  #define MOUSE_EVENT       (1 << 0)
  #define KEYBOARD_EVENT    (1 << 1)

  //
  // typedefs
  //
  typedef struct {
    union {
      USBH_HID_KEYBOARD_DATA  Keyboard;
      USBH_HID_MOUSE_DATA     Mouse;
    } Data;
    U8 Event;
  }  HID_EVENT;
  //
  // Static data, USB host
  //
  static OS_STACKPTR int     _StackHID[1024/sizeof(int)];
  static OS_TASK             _TCBHID;

  static OS_STACKPTR int     _StackISR[1024/sizeof(int)];
  static OS_TASK             _TCBISR;

  static OS_STACKPTR int     _StackUSBH[1024/sizeof(int)];
  static OS_TASK             _TCBUSBH;

  static HID_EVENT           _aHIDEvents[MAX_DATA_ITEMS];
  static OS_MAILBOX          _HIDMailBox;
  static GUI_PID_STATE       _PIDState;


#else
  #if (INCLUDE_USB && INCLUDE_FS)
    //
    // Static data, USB device
    //
    static OS_STACKPTR int _USB_MSD_Stack[1024];
    static OS_TASK         _USB_MSD_TCB;
  #endif
#endif

/*********************************************************************
*
*       Local code, SEGGERDEMO
*
**********************************************************************
*/

#if INCLUDE_GUI
#if (!defined _WINDOWS) && (((INCLUDE_IP == 1) || (INCLUDE_USB == 1) || (INCLUDE_USBH == 1) || (INCLUDE_FS == 1)))

/*********************************************************************
*
*       _FRAMEWIN_DrawSkinFlex
*/
static int _FRAMEWIN_DrawSkinFlex(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_CREATE:
    FRAMEWIN_SetTextAlign(pDrawItemInfo->hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetTextColor(pDrawItemInfo->hWin, GUI_BLACK);
    WM_ClrHasTrans(pDrawItemInfo->hWin);
    break;
  default:
    return FRAMEWIN_DrawSkinFlex(pDrawItemInfo);
  }
  return 0;
}

/*********************************************************************
*
*       _TrialWinCallBack()
*/
static void _TrialWinCallBack(WM_MESSAGE* pMsg) {
  GUI_RECT Rect = {0, 0, TRIALWIN_XSIZE, TRIALWIN_YSIZE};
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetColor(GUI_BLACK);
    GUI_Clear();
    GUI_DispStringInRect("\n"
                         "You have reached the\n"
                         MAX_TRIAL_TIME_TEXT
                         " time limit.\n"
                         "\n"
                         "Thank you for evaluating\n"
                         "SEGGER software.",
                         &Rect, GUI_TA_CENTER);
    break;
  }
}

/*********************************************************************
*
*       _TrialWindowTask()
*/
static void _TrialWindowTask(void) {
  FRAMEWIN_SKINFLEX_PROPS Framewin_Props;
  OS_U32 t;

  t = MAX_TRIAL_TIME_VALUE;
  do {
	OS_Delay(10000);
  } while (t > (OS_U32)OS_GetTime32());
  OS_SetPriority(&_TrialWindow_TCB, 255);
  //
  // Deactivate round borders for the trial window
  //
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  FRAMEWIN_SetDefaultSkin(_FRAMEWIN_DrawSkinFlex);
  FRAMEWIN_CreateEx(TRIALWIN_XPOS, TRIALWIN_YPOS, TRIALWIN_XSIZE, TRIALWIN_YSIZE, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Trial limit reached", _TrialWinCallBack);
  WM_Exec();
  while(1);
}

#endif
#endif

/*********************************************************************
*
*       Local code, USB
*
**********************************************************************
*/

#if (INCLUDE_USBH && USE_USB_HOST_HID)
  //
  // Local code, USB host
  //

  /*********************************************************************
  *
  *       _OnMouseChange
  */
  static void _OnMouseChange(USBH_HID_MOUSE_DATA  * pMouseData) {
    HID_EVENT  HidEvent;

    HidEvent.Event = MOUSE_EVENT;
    HidEvent.Data.Mouse = *pMouseData;
    OS_PutMail(&_HIDMailBox, &HidEvent);
  }

  /*********************************************************************
  *
  *       _HIDTask
  */
  static void _HIDTask(void) {
    int xSize;
    int ySize;
    HID_EVENT  HidEvent;

    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();
    USBH_Init();
    OS_CREATETASK(&_TCBUSBH, "USBH_Task", USBH_Task,    TASKPRIO_USBH_MAIN, _StackUSBH);  // Start USBH main task
    OS_CREATETASK(&_TCBISR,  "USBH_ISR",  USBH_ISRTask, TASKPRIO_USBH_ISR,  _StackISR);   // Start USBH ISR task
    OS_CREATEMB(&_HIDMailBox, sizeof(HID_EVENT), MAX_DATA_ITEMS, &_aHIDEvents);
    USBH_HID_Init();
    USBH_HID_SetOnMouseStateChange(_OnMouseChange);
    while (1) {
      OS_GetMail(&_HIDMailBox, &HidEvent);
      if ((HidEvent.Event & (MOUSE_EVENT)) == MOUSE_EVENT) {
        int x,y;
        HidEvent.Event &= ~(MOUSE_EVENT);

        GUI_PID_GetState(&_PIDState);
        x = _PIDState.x;
        y = _PIDState.y;
        x += HidEvent.Data.Mouse.xChange;
        y += HidEvent.Data.Mouse.yChange;
        _PIDState.Pressed = HidEvent.Data.Mouse.ButtonState;
        if ((x >= 0) && (x <= xSize)) {
          _PIDState.x = x;
        }
        if ((y >= 0) && (y <= ySize)) {
          _PIDState.y = y;
        }
        GUI_PID_StoreState(&_PIDState);
      }
      OS_Delay(10);
    }
  }
#else
  #if (INCLUDE_USB && INCLUDE_FS)
    //
    // Local code, USB device
    //

    /*********************************************************************
    *
    *       _AddMSD
    *
    *  Function description
    *    Add mass storage device to USB stack
    *
    *  Notes:
    *   (1)  -   This examples uses the internal driver of the file system.
    *            The module intializes the low-level part of the file system if necessary.
    *            If FS_Init() was not previously called, none of the high level functions
    *            such as FS_FOpen, FS_Write etc will work.
    *            Only functions that are driver related will be called.
    *            Initialization, sector read/write, retrieve device information.
    *            The following members of the DriverData are used as follows:
    *              DriverData.pStart       = Index no. of the volume that shall be used.
    *              DriverData.NumSectors   = Number of sectors to be used - 0 means auto-detect.
    *              DriverData.StartSector  = The first sector that shall be used.
    *              DriverData.SectorSize will not be used.
    */
    static void _AddMSD(void) {
      static U8 _abOutBuffer[USB_MAX_PACKET_SIZE];
      USB_MSD_INIT_DATA     InitData;
      USB_MSD_INST_DATA     InstData;

      InitData.EPIn  = USB_AddEP(1, USB_TRANSFER_TYPE_BULK, USB_MAX_PACKET_SIZE, NULL, 0);
      InitData.EPOut = USB_AddEP(0, USB_TRANSFER_TYPE_BULK, USB_MAX_PACKET_SIZE, _abOutBuffer, USB_MAX_PACKET_SIZE);
      USB_MSD_Add(&InitData);
      //
      // Add logical unit 0:
      //
      memset(&InstData, 0,  sizeof(InstData));
      InstData.pAPI                    = &USB_MSD_StorageByName;    // s. Note (1)
      InstData.DriverData.pStart       = "";
      USB_MSD_AddUnit(&InstData);
    }

    /*********************************************************************
    *
    *       _USBMSDTask()
    */
    static void _USBMSDTask(void) {
      USB_Init();
      FS_Init();
      if (FS_IsLLFormatted("") == 0) {
        FS_X_Log("Low level formatting");
        FS_FormatLow("");          /* Erase & Low-level  format the volume */
      }
      if (FS_IsHLFormatted("") == 0) {
        FS_X_Log("High level formatting\n");
        FS_Format("", NULL);       /* High-level format the volume */
      }
      FS_Unmount("");
      _AddMSD();
      USB_Start();
      while (1) {
        //
        // Wait for configuration
        //
        while ((USB_GetState() & (USB_STAT_CONFIGURED | USB_STAT_SUSPENDED)) != USB_STAT_CONFIGURED) {
          USB_OS_Delay(50);
          BSP_ToggleLED(0);
        }
        USB_MSD_Task();
      }
    }

    /*********************************************************************
    *
    *       Exported USB device routines
    *
    **********************************************************************
    */

    /*********************************************************************
    *
    *       Get information that are used during enumeration
    */

    /*********************************************************************
    *
    *       USB_GetVendorName
    *
    *  Function description
    *    Returns vendor Id
    */
    U16 USB_GetVendorId(void) {
      return 0x8765;
    }

    /*********************************************************************
    *
    *       USB_GetProductId
    *
    *  Function description
    *    Returns the product Id
    */
    U16 USB_GetProductId(void) {
      return 0x1000;
    }

    /*********************************************************************
    *
    *       USB_GetVendorName
    *
    *  Function description
    *    Returns vendor name.
    */
    const char * USB_GetVendorName(void) {
      return "Vendor";
    }

    /*********************************************************************
    *
    *       USB_GetProductName
    *
    *  Function description
    *    Returns product name
    */
    const char * USB_GetProductName(void) {
      return "MSD device";
    }

    /*********************************************************************
    *
    *       USB_GetSerialNumber
    *
    *  Function description
    *    Returns serial number
    */
    const char * USB_GetSerialNumber(void) {
      return "13245678";
    }

    /*********************************************************************
    *
    *       String information routines when inquiring the volume
    */
    /*********************************************************************
    *
    *       USB_MSD_GetVendorName
    */
    const char * USB_MSD_GetVendorName(U8 Lun) {
      (void)Lun;
      return "Vendor";
    }

    /*********************************************************************
    *
    *       USB_MSD_GetProductName
    */
    const char * USB_MSD_GetProductName(U8 Lun) {
      (void)Lun;
      return "MSD Volume";
    }

    /*********************************************************************
    *
    *       USB_MSD_GetProductVer
    */
    const char * USB_MSD_GetProductVer(U8 Lun) {
      (void)Lun;
      return "1.00";
    }

    /*********************************************************************
    *
    *       USB_MSD_GetSerialNo
    */
    const char * USB_MSD_GetSerialNo(U8 Lun) {
      (void)Lun;
      return "134657890";
    }

  #endif
#endif

/*********************************************************************
*
*       Local code, IP
*
**********************************************************************
*/

#if INCLUDE_GUI
#if INCLUDE_IP

/*********************************************************************
*
*       _EthWinCallBack()
*/
static void _EthWinCallBack(WM_MESSAGE* pMsg) {
  static char acText[50];
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (IP_IFaceIsReady() == 0) {
       strcpy(acText, "IP: Not connected");
    } else {
      _IPAddr = IP_GetIPAddr(0);
      IP_PrintIPAddr(_acIP, _IPAddr, sizeof(_acIP));
      strcpy(acText,"IP: ");
      strcat(acText, _acIP);
    }
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetColor(GUI_BLACK);
    GUI_Clear();
    GUI_GotoXY(0, 3);
    GUI_DispString(acText);
    break;
  }
}

/*********************************************************************
*
*       _EthWindowTask()
*/
static void _EthWindowTask(void) {
  FRAMEWIN_SKINFLEX_PROPS Framewin_Props;
  int                     IPState = -1;

  //
  // Deactivate round borders for the ethernet window
  //
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_GetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  Framewin_Props.Radius = 0;
  FRAMEWIN_SetSkinFlexProps(&Framewin_Props, FRAMEWIN_SKINFLEX_PI_INACTIVE);
  FRAMEWIN_SetDefaultSkin(_FRAMEWIN_DrawSkinFlex);
  _hEthFrameWin = FRAMEWIN_CreateEx(0, LCD_GetYSize() - 30, 120, 30, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Ethernet", _EthWinCallBack);
  WM_Exec();
  _hEthWin1 = WM_GetClientWindow(_hEthFrameWin);
  for (;;) {
    if (IP_IFaceIsReady() != IPState) {
      WM_InvalidateWindow(_hEthWin1);
    }
    IPState = IP_IFaceIsReady();
    OS_Delay(500);
  }
}

#endif

/*********************************************************************
*
*       GUIDEMO routine table
*/
static void (* _apfTest[])(void) = {
#if (SHOW_GUIDEMO_SPEEDOMETER                         && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Speedometer,
#endif
#if (SHOW_GUIDEMO_ZOOMANDROTATE     && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_ZoomAndRotate,
#endif
#if (SHOW_GUIDEMO_RADIALMENU        && GUI_WINSUPPORT                      )
  GUIDEMO_RadialMenu,
#endif
#if (SHOW_GUIDEMO_SKINNING          && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Skinning,
#endif
#if (SHOW_GUIDEMO_BARGRAPH                            && GUI_SUPPORT_MEMDEV)
  GUIDEMO_BarGraph,
#endif
#if (SHOW_GUIDEMO_FADING                              && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Fading,
#endif
#if (SHOW_GUIDEMO_AATEXT                                                   )
  GUIDEMO_AntialiasedText,
#endif
#if (SHOW_GUIDEMO_TRANSPARENTDIALOG && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_TransparentDialog,
#endif
#if (SHOW_GUIDEMO_WASHINGMACHINE    && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_WashingMachine,
#endif
#if (SHOW_GUIDEMO_ICONVIEW          && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_IconView,
#endif
#if (SHOW_GUIDEMO_IMAGEFLOW         && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_ImageFlow,
#endif
#if (SHOW_GUIDEMO_TREEVIEW          && GUI_WINSUPPORT                      )
  GUIDEMO_Treeview,
#endif
#if (SHOW_GUIDEMO_LISTVIEW          && GUI_WINSUPPORT                      )
  GUIDEMO_Listview,
#endif
#if (SHOW_GUIDEMO_VSCREEN                                                  )
  GUIDEMO_VScreen,
#endif
#if (SHOW_GUIDEMO_GRAPH             && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Graph,
#endif
#if (SHOW_GUIDEMO_SPEED                                                    )
  GUIDEMO_Speed,
#endif
#if (SHOW_GUIDEMO_BITMAP                                                   )
  GUIDEMO_Bitmap,
#endif
#if (SHOW_GUIDEMO_CURSOR            && GUI_SUPPORT_CURSOR                  )
  GUIDEMO_Cursor,
#endif
#if (SHOW_GUIDEMO_COLORBAR                                                 )
  GUIDEMO_ColorBar,
#endif
#if (SHOW_GUIDEMO_AUTOMOTIVE                          && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Automotive,
#endif
  0
};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Config
*/
void GUIDEMO_Config(GUIDEMO_CONFIG * pConfig) {
  pConfig->apFunc   = _apfTest;
  pConfig->NumDemos = GUI_COUNTOF(_apfTest);
  pConfig->Flags    = GUIDEMO_CF_SHOW_SPRITES | GUIDEMO_CF_USE_VNC | GUIDEMO_CF_USE_AUTO_BK | GUIDEMO_CF_SUPPORT_TOUCH;
  #if GUIDEMO_USE_VNC
    pConfig->pGUI_VNC_X_StartServer = GUI_VNC_X_StartServer;
  #endif
}

#endif

/*********************************************************************
*
*       MainTask()
*/
void MainTask(void);
void MainTask(void) {
  #if INCLUDE_GUI
    //
    // Init GUI
    //
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    #if ((!defined _WINDOWS) && ((INCLUDE_IP == 1) || (INCLUDE_USB == 1) || (INCLUDE_USBH == 1) || (INCLUDE_FS == 1)))
      if (strncmp(OS_GetLibName(), "(T", 2) == 0) {
        //
        // Setup task for trial window
        //
        OS_CREATETASK(&_TrialWindow_TCB, "TrialWindow", _TrialWindowTask, TASKPRIO_TRIALWINDOW, _TrialWindow_Stack);
      }
    #endif
  #endif
  #if INCLUDE_IP
    //
    // Init IP
    //
    IP_Init();
    //
    // Start TCP/IP task
    //
    OS_CREATETASK(&_IP_TCB,   "IP_Task",   IP_Task,   TASKPRIO_IPMAIN, _IP_Stack);
    OS_CREATETASK(&_IPRx_TCB, "IP_RxTask", IP_RxTask, TASKPRIO_IPRX,   _IPRx_Stack);
    #if INCLUDE_FTP
      //
      // Start FTP server
      //
      OS_CREATETASK(&_FTPServer_TCB, "FTP Server", FTPServerTask, TASKPRIO_FTPS, _FTPServer_Stack);
    #endif
    #if INCLUDE_WEBSERVER
      //
      // Start Webserver
      //
      OS_CREATETASK(&_WEBServer_TCB, "Webserver",  WebserverTask, TASKPRIO_WEBS, _WEBServer_Stack);
    #endif
  #endif
  #if (INCLUDE_GUI && INCLUDE_VNC && (INCLUDE_IP || defined(_WINDOWS)))
    //
    // Start VNC server
    //
    GUI_VNC_X_StartServer(0, 0);
  #endif
  #if (INCLUDE_USBH && USE_USB_HOST_HID)
    //
    // Init USB Host
    //
    OS_CREATETASK(&_TCBHID, "USBH_HIDTask", _HIDTask, TASKPRIO_USB, _StackHID);
  #else
    #if (INCLUDE_USB && INCLUDE_FS)
      //
      // Start USB MSD task
      //
      OS_CREATETASK(&_USB_MSD_TCB, "USBMSDTask", _USBMSDTask, TASKPRIO_USB, _USB_MSD_Stack);
    #endif
  #endif
  #if INCLUDE_GUI
    #if GUI_SUPPORT_CURSOR
      GUI_CURSOR_Hide();  // Hide cursor before first page is shown
    #endif
    //
    // Show SEGGER logo
    //
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
    GUI_DrawBitmap(&bmIMAGE_SeggerLogo_300x150_565c, (LCD_GetXSize() - 300) / 2, (LCD_GetYSize() - 150) / 2);
    GUI_X_Delay(SEGGERDEMO_DELAY);
    #if INCLUDE_IP
      //
      // Show ethernet IP in window during demo
      //
      OS_CREATETASK(&_EthWindow_TCB, "EthWinTask", _EthWindowTask, TASKPRIO_WINDOW, _EthWindow_Stack);
    #endif
    GUI_Clear();
    //
    // Start GUIDEMO samples, GUIDEMO_Main is an endless loop itself. Therefor we can start it as task directly.
    //
    #if ((!defined _WINDOWS) && USE_GUI_TASK)
      OS_CREATETASK(&_GUITask_TCB, "GUI_Task", GUIDEMO_Main, 100, _GUITask_Stack);
      OS_Terminate(0);
    #else
      GUIDEMO_Main();
    #endif
  #else
    while (1) {
      OS_Delay(100000);
    }
  #endif
}

/*************************** End of file ****************************/
