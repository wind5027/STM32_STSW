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
File    : OS_USBH_Printer.c
Purpose : Sample program for embOS & USBH USB host stack.
          Demonstrates use of the USBH stack.
--------- END-OF-HEADER --------------------------------------------*/


#include <stdio.h>
#include "RTOS.h"
#include "BSP.h"
#include "USBH.h"

/*********************************************************************
*
*       Local defines, configurable
*
**********************************************************************
*/
#define COUNTOF(a)  (sizeof(a)/sizeof(a[0]))

enum {
  TASK_PRIO_APP,
  TASK_PRIO_USBH_MAIN,
  TASK_PRIO_USBH_ISR
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static OS_STACKPTR int     _StackMain[1024/sizeof(int)];
static OS_TASK             _TCBMain;

static OS_STACKPTR int     _StackIsr[1024/sizeof(int)];
static OS_TASK             _TCBIsr;
static volatile char       _PrinterReady;
static U8                  _DevIndex;
static unsigned char       _acPrinterJob[] = {"Hello World\f"};


/*********************************************************************
*
*       Static Code
*
**********************************************************************
*/



/*********************************************************************
*
*       _OnPrinterReady
*
*  Function description
*    Called by the application task (MainTask) when an printer device is
*    plugged in.
*    It reads and displays information about the printer and
*    prints out "Hello world".
*/
static void _OnPrinterReady(void) {
  char         ac[20];
  USBH_PRINTER_HANDLE hPrinter;
  U8                  PortStatus;
  U8                  acDeviceId[255] = {0};

  sprintf(ac, "prt%.3d", _DevIndex);
  hPrinter = USBH_PRINTER_Open(ac);
  if (hPrinter) {
    USBH_PRINTER_GetDeviceId(hPrinter, acDeviceId, sizeof(acDeviceId));
    USBH_PRINTER_GetPortStatus(hPrinter, &PortStatus);
    printf("Device Id  = %s\n", &acDeviceId[2]);
    printf("PortStatus = 0x%x ->NoError=%d, Select/OnLine=%d, PaperEmpty=%d\n", PortStatus, (PortStatus & (1 << 3)) >> 3, (PortStatus & (1 << 4)) >> 4, (PortStatus & (1 << 5)) >> 5);
    printf("Printing %s to printer\n", _acPrinterJob);
    USBH_PRINTER_Write(hPrinter, _acPrinterJob, sizeof(_acPrinterJob));
    printf("Printing completed\n");
    USBH_PRINTER_Close(hPrinter);
  }
  //
  // Wait until Printer is removed.
  //
  while (_PrinterReady) {
    OS_Delay(100);
  }
}

/*********************************************************************
*
*       _cbOnAddRemoveDevice
*
*  Function description
*    Callback, called when a device is added or removed.
*    Call in the context of the USBH_Task.
*    The functionality in this routine should not block
*/
static void _cbOnAddRemoveDevice(void * pContext, U8 DevIndex, USBH_DEVICE_EVENT Event) {
  _DevIndex = DevIndex;
  pContext = pContext;
  switch (Event) {
  case USBH_DEVICE_EVENT_ADD:
    printf("\n**** Device added\n");
    _PrinterReady = 1;
    break;
  case USBH_DEVICE_EVENT_REMOVE:
    printf("\n**** Device removed\n");
    _PrinterReady = 0;
    break;
  default:;   // Should never happen
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  USBH_Init();
  OS_SetPriority(OS_GetTaskID(), TASK_PRIO_APP);                                       // This task has highest prio for real-time application
  OS_CREATETASK(&_TCBMain, "USBH_Task", USBH_Task, TASK_PRIO_USBH_MAIN, _StackMain);   // Start USBH main task
  OS_CREATETASK(&_TCBIsr, "USBH_isr", USBH_ISRTask, TASK_PRIO_USBH_ISR, _StackIsr);    // Start USBH main task
  USBH_PRINTER_Init();
  USBH_PRINTER_RegisterNotification(_cbOnAddRemoveDevice, NULL);
  while (1) {
    BSP_ToggleLED(1);
    OS_Delay(100);
    if (_PrinterReady) {
      _OnPrinterReady();
    }
  }
}




/*************************** End of file ****************************/
