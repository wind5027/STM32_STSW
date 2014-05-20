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
File        : USBH_Config_STM32F2xx_HS_ST_F429Discovery.c
Purpose     : emUSB Host configuration file for the ST STM32F429 Discovery
              eval board
              It uses the OTG_HS controller in FS mode (use internal phy)
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include <stdlib.h>
#include "USBH.h"
#include "BSP.h"
#include "USBH_HW_STM32F2xxHS.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define STM32_OTG_BASE_ADDRESS 0x40040000UL
#define ALLOC_SIZE                 0x10000      // Size of memory dedicated to the stack in bytes

#define USB_ISR_ID    (77)
#define USB_ISR_PRIO  254

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/

//
// RCC
//
#define RCC_BASE_ADDR             ((unsigned int)(0x40023800))
#define RCC_AHB1RSTR              (*(volatile U32 *)(RCC_BASE_ADDR + 0x10))
#define RCC_AHB2RSTR              (*(volatile U32 *)(RCC_BASE_ADDR + 0x14))
#define RCC_AHB1ENR               (*(volatile U32 *)(RCC_BASE_ADDR + 0x30))
#define RCC_AHB2ENR               (*(volatile U32 *)(RCC_BASE_ADDR + 0x34))

//
// GPIO
//
#define GPIOA_BASE_ADDR           ((unsigned int)0x40020000)
#define GPIOA_MODER               (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x00))
#define GPIOA_OTYPER              (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x04))
#define GPIOA_OSPEEDR             (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x08))
#define GPIOA_PUPDR               (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x0C))
#define GPIOA_IDR                 (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x10))
#define GPIOA_ODR                 (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x14))
#define GPIOA_BSRRL               (*(volatile U16 *)(GPIOA_BASE_ADDR + 0x18))
#define GPIOA_BSRRH               (*(volatile U16 *)(GPIOA_BASE_ADDR + 0x16))
#define GPIOA_LCKR                (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x1C))
#define GPIOA_AFRL                (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x20))
#define GPIOA_AFRH                (*(volatile U32 *)(GPIOA_BASE_ADDR + 0x24))


#define GPIOB_BASE_ADDR           ((unsigned int)0x40020400)
#define GPIOB_MODER               (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x00))
#define GPIOB_OTYPER              (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x04))
#define GPIOB_OSPEEDR             (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x08))
#define GPIOB_PUPDR               (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x0C))
#define GPIOB_IDR                 (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x10))
#define GPIOB_ODR                 (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x14))
#define GPIOB_BSRRL               (*(volatile U16 *)(GPIOB_BASE_ADDR + 0x18))
#define GPIOB_BSRRH               (*(volatile U16 *)(GPIOB_BASE_ADDR + 0x16))
#define GPIOB_LCKR                (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x1C))
#define GPIOB_AFRL                (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x20))
#define GPIOB_AFRH                (*(volatile U32 *)(GPIOB_BASE_ADDR + 0x24))

#define GPIOC_BASE_ADDR           ((unsigned int)0x40020800)
#define GPIOC_MODER               (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x00))
#define GPIOC_OTYPER              (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x04))
#define GPIOC_OSPEEDR             (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x08))
#define GPIOC_PUPDR               (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x0C))
#define GPIOC_IDR                 (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x10))
#define GPIOC_ODR                 (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x14))
#define GPIOC_BSRRL               (*(volatile U16 *)(GPIOC_BASE_ADDR + 0x18))
#define GPIOC_BSRRH               (*(volatile U16 *)(GPIOC_BASE_ADDR + 0x16))
#define GPIOC_LCKR                (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x1C))
#define GPIOC_AFRL                (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x20))
#define GPIOC_AFRH                (*(volatile U32 *)(GPIOC_BASE_ADDR + 0x24))

#define GPIOE_BASE_ADDR           ((unsigned int)0x40021000)
#define GPIOE_MODER               (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x00))
#define GPIOE_OTYPER              (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x04))
#define GPIOE_OSPEEDR             (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x08))
#define GPIOE_PUPDR               (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x0C))
#define GPIOE_IDR                 (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x10))
#define GPIOE_ODR                 (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x14))
#define GPIOE_BSRRL               (*(volatile U16 *)(GPIOE_BASE_ADDR + 0x18))
#define GPIOE_BSRRH               (*(volatile U16 *)(GPIOE_BASE_ADDR + 0x16))
#define GPIOE_LCKR                (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x1C))
#define GPIOE_AFRL                (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x20))
#define GPIOE_AFRH                (*(volatile U32 *)(GPIOE_BASE_ADDR + 0x24))


#define GPIOH_BASE_ADDR           ((unsigned int)0x40021C00)
#define GPIOH_MODER               (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x00))
#define GPIOH_OTYPER              (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x04))
#define GPIOH_OSPEEDR             (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x08))
#define GPIOH_PUPDR               (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x0C))
#define GPIOH_IDR                 (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x10))
#define GPIOH_ODR                 (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x14))
#define GPIOH_BSRRL               (*(volatile U16 *)(GPIOH_BASE_ADDR + 0x18))
#define GPIOH_BSRRH               (*(volatile U16 *)(GPIOH_BASE_ADDR + 0x16))
#define GPIOH_LCKR                (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x1C))
#define GPIOH_AFRL                (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x20))
#define GPIOH_AFRH                (*(volatile U32 *)(GPIOH_BASE_ADDR + 0x24))

#define GPIOI_BASE_ADDR           ((unsigned int)0x40022000)
#define GPIOI_MODER               (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x00))
#define GPIOI_OTYPER              (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x04))
#define GPIOI_OSPEEDR             (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x08))
#define GPIOI_PUPDR               (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x0C))
#define GPIOI_IDR                 (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x10))
#define GPIOI_ODR                 (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x14))
#define GPIOI_BSRRL               (*(volatile U16 *)(GPIOI_BASE_ADDR + 0x18))
#define GPIOI_BSRRH               (*(volatile U16 *)(GPIOI_BASE_ADDR + 0x16))
#define GPIOI_LCKR                (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x1C))
#define GPIOI_AFRL                (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x20))
#define GPIOI_AFRH                (*(volatile U32 *)(GPIOI_BASE_ADDR + 0x24))



/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static U32 _aPool[((ALLOC_SIZE + 256) / 4)];             // Memory area used by the stack. add additional 256 bytes in order to have a 256 byte aligned address

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       _InitUSBHw
*
*/
static void _InitUSBHw(void) {
  RCC_AHB1ENR |= 0
              | (1 <<  1)  // GPIOBEN: IO port B clock enable
              | (1 <<  2)  // GPIOCEN: IO port C clock enable
              ;
  //
  // PB12..15 (OTG_HS_ internal alternate function, OTG_HS_DP, OTG_HS_DM, OTG_HS_ID)
  //
  GPIOB_MODER    =   (GPIOB_MODER  & ~(0xFFUL << 24)) | (0xA6UL << 24);
  GPIOB_OTYPER  &=  ~(0x0FUL << 12);
  GPIOB_OSPEEDR |=   (0xFFUL << 24);
  GPIOB_PUPDR   &=  ~(0xFFUL << 24);
  GPIOB_AFRH     =   (GPIOB_AFRH  & ~(0xFF0FUL << 16)) | (0xCC0CUL << 16);
  //
  //  Enable clock for OTG_HS and OTGHS_ULPI
  //
  RCC_AHB1ENR    |=  (1UL << 29);
  USBH_OS_Delay(100);
  //
  // Reset OTGHS clock
  RCC_AHB1RSTR   |=  (1UL << 29);
  USBH_OS_Delay(100);
  RCC_AHB1RSTR   &= ~(1UL << 29);
  USBH_OS_Delay(400);
  GPIOC_MODER  |=  (1 << 8);
  GPIOC_BSRRL   = (1UL << (16UL + 4UL)); // Enable VBUS, set PC4 low
}

/*********************************************************************
*
*       _ISR
*
*  Function description
*/
static void _ISR(void) {
  USBH_ServiceISR(0);
}

/*********************************************************************
*
*       USBH_X_Config
*
*  Function description
*/
void USBH_X_Config(void) {
  USBH_AssignMemory((void *)(((U32)(&_aPool[0]) + 0xff) & ~0xffuL), ALLOC_SIZE);    // Assigning memory should be the first thing
//  USBH_AssignTransferMemory((void*)TRANSFER_MEMORY_BASE, TRANSFER_MEMORY_SIZE);
  USBH_ConfigSupportExternalHubs(1);
USBH_ConfigMaxUSBDevices(5);
USBH_ConfigMaxNumEndpoints(4,4,0);
  //
  // Define log and warn filter
  // Note: The terminal I/O emulation affects the timing
  // of your communication, since the debugger stops the target
  // for every terminal I/O output unless you use DCC!
  //
  USBH_SetWarnFilter(0xFFFFFFFF);               // 0xFFFFFFFF: Do not filter: Output all warnings.
  USBH_SetLogFilter(0
                    | USBH_MTYPE_INIT
  //                  | USBH_MTYPE_CORE
  //                  | USBH_MTYPE_DRIVER
  //                  | USBH_MTYPE_MEM
                    | USBH_MTYPE_APPLICATION
                    | USBH_MTYPE_HID
//                    | USBH_MTYPE_MSD
                    );
//  USBH_SetLogFilter(0xffffffff);
  _InitUSBHw();
  USBH_STM32F2_HS_Add((void*)STM32_OTG_BASE_ADDRESS);
  USBH_STM32F2_HS_Config_UsePhy(1);
  BSP_USB_InstallISR_Ex(USB_ISR_ID, _ISR, USB_ISR_PRIO);
}
/********************************* EOF ******************************/
