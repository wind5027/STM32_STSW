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
File        : GUIConf.h
Purpose     : Configuration of available features and default values
----------------------------------------------------------------------
*/

#ifndef GUICONF_H
#define GUICONF_H

/*********************************************************************
*
*       Debug output
*
*
* Define GUI_DEBUG_LEVEL: Debug level for GUI
*                         0: No run time checks are performed (Smallest and fastest code)
*                         1: Parameter checks are performed to avoid crashes
*                         2: Parameter checks and consistency checks are performed
*                         3: Errors are recorded
*                         4: Errors & Warnings are recorded
*                         5: Errors, Warnings and Messages are recorded
*
*/
#ifndef DEBUG
  #define DEBUG 0
#endif

#if DEBUG
  #ifndef   GUI_DEBUG_LEVEL
    #define GUI_DEBUG_LEVEL  4  // Default for debug builds
  #endif
#else
  #ifndef   GUI_DEBUG_LEVEL
    #define GUI_DEBUG_LEVEL  0  // Default for release builds
  #endif
#endif

/*********************************************************************
*
*       Multi layer/display support
*/
#define GUI_NUM_LAYERS            (16) // Maximum number of available layers

/*********************************************************************
*
*       Multi tasking support
*/
#define GUI_OS                    (1)  // Compile with multitasking support

/*********************************************************************
*
*       Configuration of available packages
*/
#ifndef   GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH       (1)  // Support touchscreen
#endif
#define GUI_SUPPORT_MOUSE         (0)  // Support a mouse
#define GUI_SUPPORT_UNICODE       (1)  // Support mixed ASCII/UNICODE strings
#define GUI_WINSUPPORT            (1)  // Window manager package available
#define GUI_SUPPORT_MEMDEV        (1)  // Memory devices available
#define GUI_SUPPORT_AA            (1)  // Anti aliasing available
#define WM_SUPPORT_STATIC_MEMDEV  (1)  // Static memory devices available

/*********************************************************************
*
*       Default font
*/
#define GUI_DEFAULT_FONT          &GUI_Font6x8

#endif  /* Avoid multiple inclusion */

/*************************** End of file ****************************/
