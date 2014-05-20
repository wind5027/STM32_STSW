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
File    : SysConf.h
Purpose : Configuration of components included in SeggerDemo
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SYSCONF_H                       /* Avoid multiple inclusion */
#define SYSCONF_H

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
#ifndef   INCLUDE_FS
  #define INCLUDE_FS         (1)
#endif
#ifndef   INCLUDE_USB
  #define INCLUDE_USB        (0)
#endif
#ifndef   INCLUDE_USBH
  #define INCLUDE_USBH       (1)
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
  #define SHOW_GUIDEMO_AATEXT            (1)
#endif
#ifndef   SHOW_GUIDEMO_BARGRAPH
  #define SHOW_GUIDEMO_BARGRAPH          (1)
#endif
#ifndef   SHOW_GUIDEMO_BITMAP
  #define SHOW_GUIDEMO_BITMAP            (1)
#endif
#ifndef   SHOW_GUIDEMO_COLORBAR
  #define SHOW_GUIDEMO_COLORBAR          (1)
#endif
#ifndef   SHOW_GUIDEMO_GRAPH
  #define SHOW_GUIDEMO_GRAPH             (1)
#endif
#ifndef   SHOW_GUIDEMO_ICONVIEW
  #define SHOW_GUIDEMO_ICONVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_IMAGEFLOW
  #define SHOW_GUIDEMO_IMAGEFLOW         (1)
#endif
#ifndef   SHOW_GUIDEMO_LISTVIEW
  #define SHOW_GUIDEMO_LISTVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_RADIALMENU
  #define SHOW_GUIDEMO_RADIALMENU        (1)
#endif
#ifndef   SHOW_GUIDEMO_SPEED
  #define SHOW_GUIDEMO_SPEED             (1)
#endif
#ifndef   SHOW_GUIDEMO_TRANSPARENTDIALOG
  #define SHOW_GUIDEMO_TRANSPARENTDIALOG (1)
#endif
#ifndef   SHOW_GUIDEMO_TREEVIEW
  #define SHOW_GUIDEMO_TREEVIEW          (1)
#endif
#ifndef   SHOW_GUIDEMO_WASHINGMACHINE
  #define SHOW_GUIDEMO_WASHINGMACHINE    (1)
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
#ifndef   GUIDEMO_USE_AUTO_BK
  #define GUIDEMO_USE_AUTO_BK     (1)
#endif
#ifndef   GUIDEMO_SUPPORT_TOUCH
  #define GUIDEMO_SUPPORT_TOUCH   (0)
#endif
#ifndef   GUIDEMO_SUPPORT_CURSOR
  #define GUIDEMO_SUPPORT_CURSOR  (GUI_SUPPORT_CURSOR && GUI_SUPPORT_TOUCH)
#endif

#define GUIDEMO_CF_SHOW_SPRITES   (                 GUIDEMO_SHOW_SPRITES  <<  0    )
#define GUIDEMO_CF_USE_VNC        (                 GUIDEMO_USE_VNC       <<  1    )
#define GUIDEMO_CF_USE_AUTO_BK    (                 GUIDEMO_USE_AUTO_BK   <<  2    )
#define GUIDEMO_CF_SUPPORT_TOUCH  (GUI_WINSUPPORT ? GUIDEMO_SUPPORT_TOUCH <<  3 : 0)

/*********************************************************************
*
*       Included defaults for not configured options
*
**********************************************************************
*/

#include "SysConfDefaults.h"

#endif                                  /* Avoid multiple inclusion */

/****** End of File *************************************************/
