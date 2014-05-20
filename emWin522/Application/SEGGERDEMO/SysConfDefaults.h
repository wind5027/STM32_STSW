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
File    : SysConfDefaults.h
Purpose : Configuration defaults for SeggerDemo
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SYSCONFDEFAULTS_H                       /* Avoid multiple inclusion */
#define SYSCONFDEFAULTS_H

/*********************************************************************
*
*       GUI task configuration
*
**********************************************************************
*/
#ifndef   USE_GUI_TASK
  #define USE_GUI_TASK  (0)
#endif

/*********************************************************************
*
*       Included middleware components
*
**********************************************************************
*/
#ifndef   INCLUDE_GUI
  #define INCLUDE_GUI        (1)
#endif
#ifndef   INCLUDE_IP
  #define INCLUDE_IP         (0)
#endif
#ifndef   INCLUDE_FTP
  #define INCLUDE_FTP        (0)
#endif
#ifndef   INCLUDE_WEBSERVER
  #define INCLUDE_WEBSERVER  (0)
#endif
#ifndef   INCLUDE_VNC
  #define INCLUDE_VNC        (0)
#endif
#ifndef   INCLUDE_FS
  #define INCLUDE_FS         (0)
#endif
#ifndef   INCLUDE_USB
  #define INCLUDE_USB        (0)
#endif
#ifndef   INCLUDE_USBH
  #define INCLUDE_USBH       (0)
#endif
#ifndef   USE_USB_HOST_HID
  #define USE_USB_HOST_HID   (0)
#endif

/*********************************************************************
*
*       Included GUIDEMO samples
*
**********************************************************************
*/
/*********************************************************************
*
*       Configuration of modules to be used
*
**********************************************************************
*/
#ifndef   SHOW_GUIDEMO_AATEXT
  #define SHOW_GUIDEMO_AATEXT            (0)
#endif
#ifndef   SHOW_GUIDEMO_AUTOMOTIVE
  #define SHOW_GUIDEMO_AUTOMOTIVE        (0)
#endif
#ifndef   SHOW_GUIDEMO_BARGRAPH
  #define SHOW_GUIDEMO_BARGRAPH          (0)
#endif
#ifndef   SHOW_GUIDEMO_BITMAP
  #define SHOW_GUIDEMO_BITMAP            (0)
#endif
#ifndef   SHOW_GUIDEMO_COLORBAR
  #define SHOW_GUIDEMO_COLORBAR          (0)
#endif
#ifndef   SHOW_GUIDEMO_CURSOR
  #define SHOW_GUIDEMO_CURSOR            (0)
#endif
#ifndef   SHOW_GUIDEMO_FADING
  #define SHOW_GUIDEMO_FADING            (0)
#endif
#ifndef   SHOW_GUIDEMO_GRAPH
  #define SHOW_GUIDEMO_GRAPH             (0)
#endif
#ifndef   SHOW_GUIDEMO_ICONVIEW
  #define SHOW_GUIDEMO_ICONVIEW          (0)
#endif
#ifndef   SHOW_GUIDEMO_IMAGEFLOW
  #define SHOW_GUIDEMO_IMAGEFLOW         (0)
#endif
#ifndef   SHOW_GUIDEMO_LISTVIEW
  #define SHOW_GUIDEMO_LISTVIEW          (0)
#endif
#ifndef   SHOW_GUIDEMO_RADIALMENU
  #define SHOW_GUIDEMO_RADIALMENU        (0)
#endif
#ifndef   SHOW_GUIDEMO_SKINNING
  #define SHOW_GUIDEMO_SKINNING          (0)
#endif
#ifndef   SHOW_GUIDEMO_SPEED
  #define SHOW_GUIDEMO_SPEED             (0)
#endif
#ifndef   SHOW_GUIDEMO_SPEEDOMETER
  #define SHOW_GUIDEMO_SPEEDOMETER       (0)
#endif
#ifndef   SHOW_GUIDEMO_TRANSPARENTDIALOG
  #define SHOW_GUIDEMO_TRANSPARENTDIALOG (0)
#endif
#ifndef   SHOW_GUIDEMO_TREEVIEW
  #define SHOW_GUIDEMO_TREEVIEW          (0)
#endif
#ifndef   SHOW_GUIDEMO_VSCREEN
  #define SHOW_GUIDEMO_VSCREEN           (0)
#endif
#ifndef   SHOW_GUIDEMO_WASHINGMACHINE
  #define SHOW_GUIDEMO_WASHINGMACHINE    (0)
#endif
#ifndef   SHOW_GUIDEMO_ZOOMANDROTATE
  #define SHOW_GUIDEMO_ZOOMANDROTATE     (0)
#endif

/*********************************************************************
*
*       SEGGERDEMO samples configuration
*
**********************************************************************
*/
#ifndef   GUIDEMO_SHOW_SPRITES
  #define GUIDEMO_SHOW_SPRITES    (1)
#endif
#ifndef   GUIDEMO_USE_VNC
  #define GUIDEMO_USE_VNC         (0)
#endif
#ifndef   GUIDEMO_USE_AUTO_BK
  #define GUIDEMO_USE_AUTO_BK     (1)
#endif
#ifndef   GUIDEMO_SUPPORT_TOUCH
  #define GUIDEMO_SUPPORT_TOUCH   (1)
#endif
#ifndef   GUIDEMO_SUPPORT_CURSOR
  #define GUIDEMO_SUPPORT_CURSOR  (GUI_SUPPORT_CURSOR && GUI_SUPPORT_TOUCH)
#endif

#define GUIDEMO_CF_SHOW_SPRITES   (                 GUIDEMO_SHOW_SPRITES  <<  0    )
#define GUIDEMO_CF_USE_VNC        (                 GUIDEMO_USE_VNC       <<  1    )
#define GUIDEMO_CF_USE_AUTO_BK    (                 GUIDEMO_USE_AUTO_BK   <<  2    )
#define GUIDEMO_CF_SUPPORT_TOUCH  (GUI_WINSUPPORT ? GUIDEMO_SUPPORT_TOUCH <<  3 : 0)

#endif  // Avoid multiple inclusion

/*************************** End of file ****************************/
