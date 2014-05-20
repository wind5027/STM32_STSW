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
File    : BSP.c
Purpose : BSP for the ST STM32F4-Discovery eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"
#include "RTOS.h"
#include "stm32f4xx.h"     // Device specific header file, contains CMSIS

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** SFRs used for LED-Port **************************************/

#define GPIOG_BASE_ADDR           ((unsigned int)0x40021800u)

#define GPIOG_MODER               (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x00u))
#define GPIOG_ODR                 (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x14u))
#define GPIOG_BSRR                (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR             ((unsigned int)(0x40023800u))
#define RCC_AHB1RSTR              (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x10u))
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))

#define RCC_LEDPORT_RSTR          RCC_AHB1RSTR
#define RCC_LEDPORT_ENR           RCC_AHBENR
#define RCC_LEDPORT_BIT           (6)

/****** Assign LEDs to Ports ****************************************/

#define LED_PORT_MODER            GPIOG_MODER
#define LED_PORT_ODR              GPIOG_ODR
#define LED_PORT_BSRR             GPIOG_BSRR

#define LED0_BIT                  (13)
#define LED1_BIT                  (14)
#define LED_MASK_ALL              ((1uL << LED0_BIT) | (1uL << LED1_BIT))

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/

typedef void USB_ISR_HANDLER  (void);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static USB_ISR_HANDLER * _pfUSBISRHandler;
static USB_ISR_HANDLER * _pfUSBHISRHandler;

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  //
  // Initialize port for LEDs (sample application)
  //
  RCC_LEDPORT_ENR  &= ~(1uL << RCC_LEDPORT_BIT);
  RCC_LEDPORT_RSTR &= ~(1uL << RCC_LEDPORT_BIT);
  RCC_LEDPORT_ENR  |=  (1uL << RCC_LEDPORT_BIT);

  LED_PORT_MODER &= ~(3uL << (LED0_BIT * 2)) | (3uL << (LED1_BIT * 2));   // Reset mode; sets port to input
  LED_PORT_MODER |=  (1uL << (LED0_BIT * 2)) | (1uL << (LED1_BIT * 2));   // Set to output mode
  LED_PORT_BSRR   =  (0x10000uL << LED0_BIT) | (0x10000uL << LED1_BIT);   // Initially clear LEDs
}

/*********************************************************************
*
*       LED switching routines
*       LEDs STAT1 and STAT2 are switched on with high level on port lines
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    LED_PORT_BSRR = (1uL << LED0_BIT);       // Switch on LED0
  } else if (Index == 1) {
    LED_PORT_BSRR = (1uL << LED1_BIT);       // Switch on LED1
  }
}

void BSP_ClrLED(int Index) {
  if (Index == 0) {
    LED_PORT_BSRR = (0x10000uL << LED0_BIT); // Switch off LED0
  } else if (Index == 1) {
    LED_PORT_BSRR = (0x10000uL << LED1_BIT); // Switch off LED1
  }
}

void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if ((LED_PORT_ODR & (1uL << LED0_BIT)) == 0) {  // LED is switched off
      LED_PORT_BSRR = (1uL << LED0_BIT);            // Switch on LED0
    } else {
      LED_PORT_BSRR = (0x10000uL << LED0_BIT);      // Switch off LED0
    }
  } else if (Index == 1) {
    if ((LED_PORT_ODR & (1uL << LED1_BIT)) == 0) {  // LED is switched off
      LED_PORT_BSRR = (1uL << LED1_BIT);            // Switch on LED1
    } else {
      LED_PORT_BSRR = (0x10000uL << LED1_BIT);      // Switch off LED1
    }
  }
}

/*********************************************************************
*
*       USB
*
*  Functions for USB controllers (as far as present)
*/

/****** Declare ISR handler here to avoid "no prototype" warning. They are not declared in any CMSIS header */

#ifdef __cplusplus
extern "C" {
#endif
void OTG_FS_IRQHandler(void);
void OTG_HS_IRQHandler(void);
#ifdef __cplusplus
}
#endif

/*********************************************************************
*
*       OTG_FS_IRQHandler
*/
void OTG_FS_IRQHandler(void) {
  OS_EnterInterrupt(); // Inform embOS that interrupt code is running
  if (_pfUSBISRHandler) {
    (_pfUSBISRHandler)();
  }
  if (_pfUSBHISRHandler) {
    (_pfUSBHISRHandler)();
  }
  OS_LeaveInterrupt(); // Inform embOS that interrupt code is left
}

/*********************************************************************
*
*       OTG_HS_IRQHandler
*/
void OTG_HS_IRQHandler(void) {
  OS_EnterInterrupt(); // Inform embOS that interrupt code is running
  if (_pfUSBISRHandler) {
    (_pfUSBISRHandler)();
  }
  if (_pfUSBHISRHandler) {
    (_pfUSBHISRHandler)();
  }
  OS_LeaveInterrupt(); // Inform embOS that interrupt code is left
}

/*********************************************************************
*
*       BSP_USB_InstallISR_Ex()
*/
void BSP_USB_InstallISR_Ex(int ISRIndex, void (*pfISR)(void), int Prio){
  _pfUSBISRHandler = pfISR;

  NVIC_SetPriority((IRQn_Type)ISRIndex, (uint32_t)Prio);
  NVIC_EnableIRQ((IRQn_Type)ISRIndex);
}

/****** End Of File *************************************************/
