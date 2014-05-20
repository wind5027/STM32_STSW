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
File        : LCDConf.c
Purpose     : Sample display controller configuration for STM32F429
---------------------------END-OF-HEADER------------------------------
*/
#include <string.h>
#include "GUI.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_ltdc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

/*********************************************************************
*
*       Supported orientation modes (do not change)
*
**********************************************************************
*/
#define ROTATION_0   0
#define ROTATION_CW  1
#define ROTATION_180 2
#define ROTATION_CCW 3

/*********************************************************************
*
*       Supported color modes (do not change)
*
**********************************************************************
*/
//
//      Color mode       Conversion     Driver (default orientation)
//      --------------+---------------+-----------------------------
#define CM_ARGB8888 1 // GUICC_M8888I - GUIDRV_LIN_32
#define CM_RGB888   2 // GUICC_M888   - GUIDRV_LIN_24
#define CM_RGB565   3 // GUICC_M565   - GUIDRV_LIN_16
#define CM_ARGB1555 4 // GUICC_M1555I - GUIDRV_LIN_16
#define CM_ARGB4444 5 // GUICC_M4444I - GUIDRV_LIN_16
#define CM_L8       6 // GUICC_8666   - GUIDRV_LIN_8
#define CM_AL44     7 // GUICC_1616I  - GUIDRV_LIN_8
#define CM_AL88     8 // GUICC_88666I - GUIDRV_LIN_16

/*********************************************************************
*
*       DISPLAY CONFIGURATION (free to modify)
*
**********************************************************************
*/
/*********************************************************************
*
*       Common
*/
//
// Physical display size
//
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

//
// Buffers / VScreens
//
#define NUM_BUFFERS  3 // Number of multiple buffers to be used (at least 1 buffer)
#define NUM_VSCREENS 1 // Number of virtual  screens to be used (at least 1 screen)

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1
//
// For color mode GUICC_8888 we have 32 bit per pixel
//
#define BYTES_PER_PIXEL 4

/*********************************************************************
*
*       Layer 0
*/
//
// Color mode layer 0. Should be one of the above defined color modes
//
#define COLOR_MODE_0 CM_RGB565

//
// Size of layer 0. Ignored and replaced by XSIZE_PHYS and YSIZE_PHYS if GUI_NUM_LAYERS == 1
//
#define XSIZE_0 200
#define YSIZE_0 200

//
// Orientation of layer 0. Should be one of the above defined display orientations.
//
#define ORIENTATION_0 ROTATION_CW

/*********************************************************************
*
*       Background color shown where no layer is active
*/
#define BK_COLOR GUI_DARKBLUE

/*********************************************************************
*
*       Automatic selection of driver and color conversion
*
**********************************************************************
*/
#if   (COLOR_MODE_0 == CM_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
#elif (COLOR_MODE_0 == CM_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
#elif (COLOR_MODE_0 == CM_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
#elif (COLOR_MODE_0 == CM_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
#elif (COLOR_MODE_0 == CM_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
#elif (COLOR_MODE_0 == CM_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
#elif (COLOR_MODE_0 == CM_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
#elif (COLOR_MODE_0 == CM_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_0 == CM_ARGB8888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_0 == CM_RGB888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_0 == CM_RGB565)   \
   || (COLOR_MODE_0 == CM_ARGB1555) \
   || (COLOR_MODE_0 == CM_ARGB4444) \
   || (COLOR_MODE_0 == CM_AL88)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_0 == CM_L8)   \
   || (COLOR_MODE_0 == CM_AL44)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_8
  #endif
#endif

#if (GUI_NUM_LAYERS > 1)

#if   (COLOR_MODE_1 == CM_ARGB8888)
  #define COLOR_CONVERSION_1 GUICC_M8888I
#elif (COLOR_MODE_1 == CM_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
#elif (COLOR_MODE_1 == CM_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
#elif (COLOR_MODE_1 == CM_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
#elif (COLOR_MODE_1 == CM_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
#elif (COLOR_MODE_1 == CM_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
#elif (COLOR_MODE_1 == CM_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
#elif (COLOR_MODE_1 == CM_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

#if   (COLOR_MODE_1 == CM_ARGB8888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_1 == CM_RGB888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_1 == CM_RGB565)   \
   || (COLOR_MODE_1 == CM_ARGB1555) \
   || (COLOR_MODE_1 == CM_ARGB4444) \
   || (COLOR_MODE_1 == CM_AL88)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_1 == CM_L8)   \
   || (COLOR_MODE_1 == CM_AL44)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_8
  #endif
#endif

#else

/*********************************************************************
*
*       Use complete display automatically in case of only one layer
*/
#undef XSIZE_0
#undef YSIZE_0
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS
#endif

/*********************************************************************
*
*       Redirect bulk conversion to DMA2D routines
*/
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}

/*********************************************************************
*
*       H/V front/backporch and synchronization width/height
*/
#define HBP 42
#define VBP 11
#define HSW 40
#define VSW  9
#define HFP  2
#define VFP  2

/*********************************************************************
*
*       Configuration checking
*/
#if NUM_BUFFERS > 3
  #error More than 3 buffers make no sense and are not supported in this configuration file!
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_BUFFERS
  #define NUM_BUFFERS 1
#else
  #if (NUM_BUFFERS <= 0)
    #error At least one buffer needs to be defined!
  #endif
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens together with multiple buffers are not allowed!
#endif

/*********************************************************************
*
*       SFRs
*/
#define AHB1PERIPH_BASE            (PERIPH_BASE + 0x00020000)
#define APB2PERIPH_BASE            (PERIPH_BASE + 0x00010000)

#define LTDC_BASE                  (APB2PERIPH_BASE + 0x6800)
#define LTDC_LAYER1_BASE           (LTDC_BASE + 0x84)
#define LTDC_LAYER2_BASE           (LTDC_BASE + 0x104)

#define RCC_BASE                   (AHB1PERIPH_BASE + 0x3800)

#define LTDC_LAYER1                ((LTDC_LAYER_T *)LTDC_LAYER1_BASE)
#define LTDC_LAYER2                ((LTDC_LAYER_T *)LTDC_LAYER2_BASE)

#define SCS_BASE                   (0xE000E000UL)                 // System Control Space Base Address
#define NVIC_BASE                  (SCS_BASE + 0x0100UL)          // NVIC Base Address

#define DMA2D_BASE                 (AHB1PERIPH_BASE + 0xB000)

#define GPIOA_BASE                 (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE                 (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE                 (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE                 (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE                 (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE                 (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE                 (AHB1PERIPH_BASE + 0x1800)
#define GPIOI_BASE                 (AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASE                 (AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASE                 (AHB1PERIPH_BASE + 0x2800)

// Bit defines for the LCD SPI interface
#define LCD_SPI_SCK_PIN               GPIO_Pin_7                     /* PF.07 */
#define LCD_SPI_SCK_GPIO_PORT         GPIOF                          /* GPIOF */
#define LCD_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOF
#define LCD_SPI_SCK_SOURCE            GPIO_PinSource7
#define LCD_SPI_SCK_AF                GPIO_AF_SPI5
#define LCD_SPI_MISO_PIN              GPIO_Pin_8                     /* PF.08 */
#define LCD_SPI_MISO_GPIO_PORT        GPIOF                          /* GPIOF */
#define LCD_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define LCD_SPI_MISO_SOURCE           GPIO_PinSource8
#define LCD_SPI_MISO_AF               GPIO_AF_SPI5
#define LCD_SPI_MOSI_PIN              GPIO_Pin_9                     /* PF.09 */
#define LCD_SPI_MOSI_GPIO_PORT        GPIOF                          /* GPIOF */
#define LCD_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define LCD_SPI_MOSI_SOURCE           GPIO_PinSource9
#define LCD_SPI_MOSI_AF               GPIO_AF_SPI5
#define LCD_SPI                       SPI5
#define LCD_SPI_CLK                   RCC_APB2Periph_SPI5

// Common LTDC
#define GCR_MASK                   ((U32)0x0FFE888F)              // LTDC GCR Mask

#define LTDC_PCPolarity_IIPC       LTDC_GCR_PCPOL                 // Inverted input pixel clock.

#define LTDC_DEPolarity_AH         LTDC_GCR_DEPOL                 // Data Enable, is active high.

#define LTDC_VSPolarity_AH         LTDC_GCR_VSPOL                 // Vertical Synchronization is active high.

#define LTDC_HSPolarity_AH         LTDC_GCR_HSPOL                 // Horizontal Synchronization is active high.

// Common
#define __NVIC_PRIO_BITS           4                              // STM32F4XX uses 4 Bits for the Priority Levels

#define LTDC_IRQn                  88                             // LTDC global Interrupt
#define DMA2D_IRQn                 90                             // DMA2D global Interrupt
#define PLLSAION_BitNumber         0x1C

#define RCC_OFFSET                 (RCC_BASE - PERIPH_BASE)
#define CR_OFFSET                  (RCC_OFFSET + 0x00)
#define CR_PLLSAION_BB             (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLSAION_BitNumber * 4))

#define FLAG_MASK                  ((U8)0x1F)

#define DISABLE 0
#define ENABLE  1
#define RESET   0
#define SET     1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  U32 HSPolarity;             // Horizontal synchronization polarity.        Can be one value of @ref LTDC_HSPolarity
  U32 VSPolarity;             // Vertical synchronization polarity.          Can be one value of @ref LTDC_VSPolarity
  U32 DEPolarity;             // Data enable polarity.                       Can be one of value of @ref LTDC_DEPolarity
  U32 PCPolarity;             // Pixel clock polarity.                       Can be one of value of @ref LTDC_PCPolarity
  U32 HorizontalSync;         // Number of Horizontal synchronization width. Must range from 0x000 to 0xFFF.
  U32 VerticalSync;           // Number of Vertical synchronization heigh.   Must range from 0x000 to 0x7FF.
  U32 AccumulatedHBP;         // Accumulated horizontal back porch width.    Must range from LTDC_HorizontalSync to 0xFFF.
  U32 AccumulatedVBP;         // Accumulated vertical back porch heigh.      Must range from LTDC_VerticalSync to 0x7FF.
  U32 AccumulatedActiveW;     // Accumulated active width.                   Must range from LTDC_AccumulatedHBP to 0xFFF.
  U32 AccumulatedActiveH;     // Accumulated active heigh.                   Must range from LTDC_AccumulatedVBP to 0x7FF.
  U32 TotalWidth;             // Total width.                                Must range from LTDC_AccumulatedActiveW to 0xFFF.
  U32 TotalHeigh;             // Total heigh.                                Must range from LTDC_AccumulatedActiveH to 0x7FF.
  U32 BackgroundRedValue;     // Background red value.                       Must range from 0x00 to 0xFF.
  U32 BackgroundGreenValue;   // Background green value.                     Must range from 0x00 to 0xFF.
  U32 BackgroundBlueValue;    // Background blue value.                      Must range from 0x00 to 0xFF.
} LTDC_INIT_T;

typedef struct {
  U32 HorizontalStart;        // Window Horizontal Start Position.           Must range from 0x000 to 0xFFF.
  U32 HorizontalStop;         // Window Horizontal Stop Position.            Must range from 0x0000 to 0xFFFF.
  U32 VerticalStart;          // Window vertical Start Position.             Must range from 0x000 to 0xFFF.
  U32 VerticalStop;           // Window vaertical Stop Position.             Must range from 0x0000 to 0xFFFF.
  U32 PixelFormat;            // Pixel format.                               Can be one of value of @ref LTDC_Pixelformat
  U32 ConstantAlpha;          // Constant alpha used for blending.           Must range from 0x00 to 0xFF.
  U32 DefaultColorBlue;       // Default blue value.                         Must range from 0x00 to 0xFF.
  U32 DefaultColorGreen;      // Default green value.                        Must range from 0x00 to 0xFF.
  U32 DefaultColorRed;        // Default red value.                          Must range from 0x00 to 0xFF.
  U32 DefaultColorAlpha;      // Default alpha value.                        Must range from 0x00 to 0xFF.
  U32 BlendingFactor_1;       // Blending factor 1.                          Can be one of value of @ref LTDC_BlendingFactor1
  U32 BlendingFactor_2;       // Blending factor 2.                          Can be one of value of @ref LTDC_BlendingFactor2
  U32 CFBStartAdress;         // Color frame buffer address
  U32 CFBLineLength;          // Color frame buffer line length.             Must range from 0x0000 to 0x1FFF.
  U32 CFBPitch;               // Color frame buffer pitch in bytes.          Must range from 0x0000 to 0x1FFF.
  U32 CFBLineNumber;          // Number of line in frame buffer.             Must range from 0x000 to 0x7FF.
} LTDC_INIT_LAYER_T;

typedef struct {
  volatile U32 CR;            // Control Register                                  Address offset: 0x84
  volatile U32 WHPCR;         // Window Horizontal Position Configuration Register Address offset: 0x88
  volatile U32 WVPCR;         // Window Vertical Position Configuration Register   Address offset: 0x8C
  volatile U32 CKCR;          // Color Keying Configuration Register               Address offset: 0x90
  volatile U32 PFCR;          // Pixel Format Configuration Register               Address offset: 0x94
  volatile U32 CACR;          // Constant Alpha Configuration Register             Address offset: 0x98
  volatile U32 DCCR;          // Default Color Configuration Register              Address offset: 0x9C
  volatile U32 BFCR;          // Blending Factors Configuration Register           Address offset: 0xA0
  volatile U32 aRESERVED0[2]; // Reserved
  volatile U32 CFBAR;         // Color Frame Buffer Address Register               Address offset: 0xAC
  volatile U32 CFBLR;         // Color Frame Buffer Length Register                Address offset: 0xB0
  volatile U32 CFBLNR;        // ColorFrame Buffer Line Number Register            Address offset: 0xB4
  volatile U32 aRESERVED1[3]; // Reserved
  volatile U32 CLUTWR;        // CLUT Write Register                               Address offset: 0x144
} LTDC_LAYER_T;

typedef struct {
  volatile U32 aRESERVED0[2]; // Reserved, 0x00-0x04
  volatile U32 SSCR;          // Synchronization Size Configuration Register,      Address offset: 0x08
  volatile U32 BPCR;          // Back Porch Configuration Register,                Address offset: 0x0C
  volatile U32 AWCR;          // Active Width Configuration Register,              Address offset: 0x10
  volatile U32 TWCR;          // Total Width Configuration Register,               Address offset: 0x14
  volatile U32 GCR;           // Global Control Register,                          Address offset: 0x18
  volatile U32 aRESERVED1[2]; // Reserved
  volatile U32 SRCR;          // Shadow Reload Configuration Register,             Address offset: 0x24
  volatile U32 aRESERVED2[1]; // Reserved
  volatile U32 BCCR;          // Background Color Configuration Register,          Address offset: 0x2C
  volatile U32 aRESERVED3[1]; // Reserved
  volatile U32 IER;           // Interrupt Enable Register,                        Address offset: 0x34
  volatile U32 ISR;           // Interrupt Status Register,                        Address offset: 0x38
  volatile U32 ICR;           // Interrupt Clear Register,                         Address offset: 0x3C
  volatile U32 LIPCR;         // Line Interrupt Position Configuration Register,   Address offset: 0x40
  volatile U32 CPSR;          // Current Position Status Register,                 Address offset: 0x44
  volatile U32 CDSR;          // Current Display Status Register,                  Address offset: 0x48
} LTDC_T;

typedef struct {
  volatile U32 CR;            // Clock control register,                                  Address offset: 0x00
  volatile U32 PLLCFGR;       // PLL configuration register,                              Address offset: 0x04
  volatile U32 CFGR;          // Clock configuration register,                            Address offset: 0x08
  volatile U32 CIR;           // Clock interrupt register,                                Address offset: 0x0C
  volatile U32 AHB1RSTR;      // AHB1 peripheral reset register,                          Address offset: 0x10
  volatile U32 AHB2RSTR;      // AHB2 peripheral reset register,                          Address offset: 0x14
  volatile U32 AHB3RSTR;      // AHB3 peripheral reset register,                          Address offset: 0x18
  volatile U32 aRESERVED0;    // Reserved
  volatile U32 APB1RSTR;      // APB1 peripheral reset register,                          Address offset: 0x20
  volatile U32 APB2RSTR;      // APB2 peripheral reset register,                          Address offset: 0x24
  volatile U32 aRESERVED1[2]; // Reserved
  volatile U32 AHB1ENR;       // AHB1 peripheral clock register,                          Address offset: 0x30
  volatile U32 AHB2ENR;       // AHB2 peripheral clock register,                          Address offset: 0x34
  volatile U32 AHB3ENR;       // AHB3 peripheral clock register,                          Address offset: 0x38
  volatile U32 aRESERVED2;    // Reserved
  volatile U32 APB1ENR;       // APB1 peripheral clock enable register,                   Address offset: 0x40
  volatile U32 APB2ENR;       // APB2 peripheral clock enable register,                   Address offset: 0x44
  volatile U32 aRESERVED3[2]; // Reserved
  volatile U32 AHB1LPENR;     // AHB1 peripheral clock enable in low power mode register, Address offset: 0x50
  volatile U32 AHB2LPENR;     // AHB2 peripheral clock enable in low power mode register, Address offset: 0x54
  volatile U32 AHB3LPENR;     // AHB3 peripheral clock enable in low power mode register, Address offset: 0x58
  volatile U32 aRESERVED4;    // Reserved
  volatile U32 APB1LPENR;     // APB1 peripheral clock enable in low power mode register, Address offset: 0x60
  volatile U32 APB2LPENR;     // APB2 peripheral clock enable in low power mode register, Address offset: 0x64
  volatile U32 aRESERVED5[2]; // Reserved
  volatile U32 BDCR;          // Backup domain control register,                          Address offset: 0x70
  volatile U32 CSR;           // Clock control & status register,                         Address offset: 0x74
  volatile U32 aRESERVED6[2]; // Reserved
  volatile U32 SSCGR;         // Spread spectrum clock generation register,               Address offset: 0x80
  volatile U32 PLLI2SCFGR;    // PLLI2S configuration register,                           Address offset: 0x84
  volatile U32 PLLSAICFGR;    // PLLSAI configuration register,                           Address offset: 0x88
  volatile U32 DCKCFGR;       // Dedicated Clocks configuration register,                 Address offset: 0x8C
} RCC_T;

typedef struct {
  volatile U32 ISER[8];       // Offset: 0x000 (R/W)  Interrupt Set Enable Register
  volatile U32 aRESERVED0[24];
  volatile U32 ICER[8];       // Offset: 0x080 (R/W)  Interrupt Clear Enable Register
  volatile U32 aRSERVED1[24];
  volatile U32 ISPR[8];       // Offset: 0x100 (R/W)  Interrupt Set Pending Register
  volatile U32 aRESERVED2[24];
  volatile U32 ICPR[8];       // Offset: 0x180 (R/W)  Interrupt Clear Pending Register
  volatile U32 aRESERVED3[24];
  volatile U32 IABR[8];       // Offset: 0x200 (R/W)  Interrupt Active bit Register
  volatile U32 aRESERVED4[56];
  volatile U8  IP[240];       // Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide)
  volatile U32 aRESERVED5[644];
  volatile U32 STIR;          // Offset: 0xE00 ( /W)  Software Trigger Interrupt Register
} NVIC_T;

typedef struct {
  volatile U32 MODER;         // GPIO port mode register,               Address offset: 0x00
  volatile U32 OTYPER;        // GPIO port output type register,        Address offset: 0x04
  volatile U32 OSPEEDR;       // GPIO port output speed register,       Address offset: 0x08
  volatile U32 PUPDR;         // GPIO port pull-up/pull-down register,  Address offset: 0x0C
  volatile U32 IDR;           // GPIO port input data register,         Address offset: 0x10
  volatile U32 ODR;           // GPIO port output data register,        Address offset: 0x14
  volatile U16 BSRRL;         // GPIO port bit set/reset low register,  Address offset: 0x18
  volatile U16 BSRRH;         // GPIO port bit set/reset high register, Address offset: 0x1A
  volatile U32 LCKR;          // GPIO port configuration lock register, Address offset: 0x1C
  volatile U32 AFR[2];        // GPIO alternate function registers,     Address offset: 0x20-0x24
} GPIO_T;

typedef struct {
  volatile U32 CR;            // DMA2D Control Register,                         Address offset: 0x00
  volatile U32 ISR;           // DMA2D Interrupt Status Register,                Address offset: 0x04
  volatile U32 IFCR;          // DMA2D Interrupt Flag Clear Register,            Address offset: 0x08
  volatile U32 FGMAR;         // DMA2D Foreground Memory Address Register,       Address offset: 0x0C
  volatile U32 FGOR;          // DMA2D Foreground Offset Register,               Address offset: 0x10
  volatile U32 BGMAR;         // DMA2D Background Memory Address Register,       Address offset: 0x14
  volatile U32 BGOR;          // DMA2D Background Offset Register,               Address offset: 0x18
  volatile U32 FGPFCCR;       // DMA2D Foreground PFC Control Register,          Address offset: 0x1C
  volatile U32 FGCOLR;        // DMA2D Foreground Color Register,                Address offset: 0x20
  volatile U32 BGPFCCR;       // DMA2D Background PFC Control Register,          Address offset: 0x24
  volatile U32 BGCOLR;        // DMA2D Background Color Register,                Address offset: 0x28
  volatile U32 FGCMAR;        // DMA2D Foreground CLUT Memory Address Register,  Address offset: 0x2C
  volatile U32 BGCMAR;        // DMA2D Background CLUT Memory Address Register,  Address offset: 0x30
  volatile U32 OPFCCR;        // DMA2D Output PFC Control Register,              Address offset: 0x34
  volatile U32 OCOLR;         // DMA2D Output Color Register,                    Address offset: 0x38
  volatile U32 OMAR;          // DMA2D Output Memory Address Register,           Address offset: 0x3C
  volatile U32 OOR;           // DMA2D Output Offset Register,                   Address offset: 0x40
  volatile U32 NLR;           // DMA2D Number of Line Register,                  Address offset: 0x44
  volatile U32 LWR;           // DMA2D Line Watermark Register,                  Address offset: 0x48
  volatile U32 AMTCR;         // DMA2D AHB Master Timer Configuration Register,  Address offset: 0x4C
  volatile U32 RESERVED[236]; // Reserved, 0x50-0x3FF
  volatile U32 FGCLUT[256];   // DMA2D Foreground CLUT,                          Address offset:400-7FF
  volatile U32 BGCLUT[256];   // DMA2D Background CLUT,                          Address offset:800-BFF
} DMA2D_T;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static LTDC_LAYER_T * _apLayer[] = { LTDC_LAYER1, LTDC_LAYER2 };
#ifdef __GNUC__
    static U32  _VRAM[2][XSIZE_PHYS * YSIZE_PHYS * BYTES_PER_PIXEL * NUM_VSCREENS * NUM_BUFFERS / sizeof(U32)]  __attribute__ ((section (".GUI_RAM")));
#else
    #pragma location="GUI_RAM"
    static U32  _VRAM[2][XSIZE_PHYS * YSIZE_PHYS * BYTES_PER_PIXEL * NUM_VSCREENS * NUM_BUFFERS / sizeof(U32)];
#endif
static const U32      _aAddr[]   = { (U32)&_VRAM[0][0], (U32)&_VRAM[1][0] };

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[GUI_NUM_LAYERS];
static int _axSize[GUI_NUM_LAYERS];
static int _aySize[GUI_NUM_LAYERS];
static int _aBytesPerPixels[GUI_NUM_LAYERS];

//
// Prototypes of DMA2D color conversion routines
//
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

//
// Color conversion routines using DMA2D
//
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_Pixelformat_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_Pixelformat_ARGB8888) // Internal pixel format of emWin is 32 bit, because of that ARGB8888
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_Pixelformat_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_Pixelformat_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_Pixelformat_ARGB4444)

//
// Buffer for DMA2D color conversion, required because hardware does not support overlapping regions
//
static U32 _aBuffer_DMA2D[XSIZE_PHYS * sizeof(U32)];
static U32 _aBuffer_FG   [XSIZE_PHYS * sizeof(U32)];
static U32 _aBuffer_BG   [XSIZE_PHYS * sizeof(U32)];

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
void  * pLCD_VRAM = &_VRAM[0][0];

//
// Array of color conversions for each layer
//
static const LCD_API_COLOR_CONV * _apColorConvAPI[] = {
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

//
// Array of orientations for each layer
//
static const int _aOrientation[] = {
  ORIENTATION_0,
#if GUI_NUM_LAYERS > 1
  ORIENTATION_1,
#endif
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelformat
*/
static U32 _GetPixelformat(int LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if ((U32)LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) {
    return LTDC_Pixelformat_ARGB8888;
  } else if (pColorConvAPI == GUICC_M888  ) {
    return LTDC_Pixelformat_RGB888;
  } else if (pColorConvAPI == GUICC_M565  ) {
    return LTDC_Pixelformat_RGB565;
  } else if (pColorConvAPI == GUICC_M1555I) {
    return LTDC_Pixelformat_ARGB1555;
  } else if (pColorConvAPI == GUICC_M4444I) {
    return LTDC_Pixelformat_ARGB4444;
  } else if (pColorConvAPI == GUICC_8666  ) {
    return LTDC_Pixelformat_L8;
  } else if (pColorConvAPI == GUICC_1616I ) {
    return LTDC_Pixelformat_AL44;
  } else if (pColorConvAPI == GUICC_88666I) {
    return LTDC_Pixelformat_AL88;
  }
  while (1); // Error
}

/*********************************************************************
*
*       _GetBytesPerLine
*/
static int _GetBytesPerLine(int LayerIndex, int xSize) {
  int BitsPerPixel, BytesPerLine;

  BitsPerPixel  = LCD_GetBitsPerPixelEx(LayerIndex);
  BytesPerLine = (BitsPerPixel * xSize + 7) / 8;
  return BytesPerLine;
}

/*********************************************************************
*
*       _RCC_AHB1PeriphClockCmd
*/
static void _RCC_AHB1PeriphClockCmd(U32 RCC_AHB1Periph, int NewState) {
  if (NewState != DISABLE) {
    RCC->AHB1ENR |=  RCC_AHB1Periph;
  } else {
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
  }
}

/*********************************************************************
*
*       _RCC_APB2PeriphClockCmd
*/
static void _RCC_APB2PeriphClockCmd(U32 RCC_APB2Periph, int NewState) {
  if (NewState != DISABLE) {
    RCC->APB2ENR |= RCC_APB2Periph;
  } else {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/*********************************************************************
*
*       _RCC_PLLSAIConfig
*/
static void _RCC_PLLSAIConfig(U32 PLLSAIN, U32 PLLSAIQ, U32 PLLSAIR) {
  RCC->PLLSAICFGR = (PLLSAIN << 6) | (PLLSAIQ << 24) | (PLLSAIR << 28);
}

/*********************************************************************
*
*       _RCC_LTDCCLKDivConfig
*/
static void _RCC_LTDCCLKDivConfig(U32 RCC_PLLSAIDivR) {
  U32 tmpreg;

  tmpreg = RCC->DCKCFGR;
  tmpreg &= ~RCC_DCKCFGR_PLLSAIDIVR; // Clear PLLSAIDIVR[2:0] bits
  tmpreg |= RCC_PLLSAIDivR;          // Set PLLSAIDIVR values
  RCC->DCKCFGR = tmpreg;             // Store the new value
}

/*********************************************************************
*
*       _RCC_PLLSAICmd
*/
static void _RCC_PLLSAICmd(int NewState) {
  *(volatile U32 *)CR_PLLSAION_BB = (U32)NewState;
}

/*********************************************************************
*
*       _RCC_GetFlagStatus
*/
static int _RCC_GetFlagStatus(U8 RCC_FLAG) {
  U32 tmp, statusreg;
  int bitstatus;

  //
  // Get the RCC register index
  //
  tmp = RCC_FLAG >> 5;
  if (tmp == 1) {              /* The flag to check is in CR register */
    statusreg = RCC->CR;
  } else if (tmp == 2) {         /* The flag to check is in BDCR register */
    statusreg = RCC->BDCR;
  }  else {                      /* The flag to check is in CSR register */
    statusreg = RCC->CSR;
  }
  //
  // Get the flag position
  //
  tmp = RCC_FLAG & FLAG_MASK;
  if ((statusreg & ((U32)1 << tmp)) != (U32)RESET) {
    bitstatus = SET;
  } else {
    bitstatus = RESET;
  }
  //
  // Return the flag status
  //
  return bitstatus;
}

/*********************************************************************
*
*       _LCD_AF_GPIOConfig
*
* Purpose:
*   Pin configuration for display controller
*
*  LCD_TFT R0    - PI.15  LCD_TFT R1    - PJ.00  LCD_TFT R2    - PJ.01  LCD_TFT R3    - PJ.02
*  LCD_TFT R4    - PJ.03  LCD_TFT R5    - PJ.04  LCD_TFT R6    - PJ.05  LCD_TFT R7    - PJ.06
*  LCD_TFT G0    - PJ.07  LCD_TFT G1    - PJ.08  LCD_TFT G2    - PJ.09  LCD_TFT G3    - PJ.10
*  LCD_TFT G4    - PJ.11  LCD_TFT G5    - PK.00  LCD_TFT G6    - PK.01  LCD_TFT G7    - PK.02
*  LCD_TFT B0    - PJ.12  LCD_TFT B1    - PJ.13  LCD_TFT B2    - PJ.14  LCD_TFT B3    - PJ.15
*  LCD_TFT B4    - PK.03  LCD_TFT B5    - PK.04  LCD_TFT B6    - PK.05  LCD_TFT B7    - PK.06
*  LCD_TFT HSYNC - PI.12  LCD_TFT VSYNC - PI.13  LCD_TFT CLK   - PI.14
*  LCD_TFT DE    - PK.07  LCD_ENABLE    - PC.06  LCD_BACKLIGHT - PA.08
*/
static void _LCD_AF_GPIOConfig(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
  _RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
 |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
 |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
 |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
 |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
 |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
 -------------------------------------------------------------------------------
          |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
          |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
           -----------------------------------------------------

*/

 /* GPIOA configuration */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                             GPIO_Pin_11 | GPIO_Pin_12;

  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

 /* GPIOB configuration */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, 0x09);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                             GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

  GPIO_Init(GPIOB, &GPIO_InitStruct);

 /* GPIOC configuration */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;

  GPIO_Init(GPIOC, &GPIO_InitStruct);

 /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;

  GPIO_Init(GPIOD, &GPIO_InitStruct);

 /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;

  GPIO_Init(GPIOF, &GPIO_InitStruct);

 /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, 0x09);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | \
                             GPIO_Pin_11 | GPIO_Pin_12;

  GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/*********************************************************************
*
*       _LTDC_Init
*/
static void _LTDC_Init(LTDC_INIT_T * pInit) {
  U32 horizontalsync, accumulatedHBP, accumulatedactiveW, totalwidth, backgreen, backred;

  //
  // Sets Synchronization size
  //
  LTDC->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
  horizontalsync = (pInit->HorizontalSync << 16);
  LTDC->SSCR |= (horizontalsync | pInit->VerticalSync);
  //
  // Sets Accumulated Back porch
  //
  LTDC->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
  accumulatedHBP = (pInit->AccumulatedHBP << 16);
  LTDC->BPCR |= (accumulatedHBP | pInit->AccumulatedVBP);
  //
  // Sets Accumulated Active Width
  //
  LTDC->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
  accumulatedactiveW = (pInit->AccumulatedActiveW << 16);
  LTDC->AWCR |= (accumulatedactiveW | pInit->AccumulatedActiveH);
  //
  // Sets Total Width
  //
  LTDC->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
  totalwidth = (pInit->TotalWidth << 16);
  LTDC->TWCR |= (totalwidth | pInit->TotalHeigh);
  LTDC->GCR &= (U32)GCR_MASK;
  LTDC->GCR |=  (U32)(pInit->HSPolarity | pInit->VSPolarity | pInit->DEPolarity | pInit->PCPolarity);
  //
  // Sets the background color value
  //
  backgreen = (pInit->BackgroundGreenValue << 8);
  backred = (pInit->BackgroundRedValue << 16);
  LTDC->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
  LTDC->BCCR |= (backred | backgreen | pInit->BackgroundBlueValue);
}

/*********************************************************************
*
*       _LTDC_LayerCmd
*/
static void _LTDC_LayerCmd(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_LEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_LEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_LayerEnableColorKeying
*/
static void _LTDC_LayerEnableColorKeying(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_COLKEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_COLKEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_LayerEnableLUT
*/
static void _LTDC_LayerEnableLUT(LTDC_LAYER_T * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_CLUTEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_CLUTEN;
  }
  LTDC->SRCR = LTDC_SRCR_IMR;//LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_ReloadConfig
*/
static void _LTDC_ReloadConfig(U32 LTDC_Reload) {
  LTDC->SRCR = (U32)LTDC_Reload;
}

/*********************************************************************
*
*       _LTDC_Cmd
*/
static void _LTDC_Cmd(int NewState) {
  if (NewState != DISABLE) {
    LTDC->GCR |= (U32)LTDC_GCR_LTDCEN;
  } else {
    LTDC->GCR &= ~(U32)LTDC_GCR_LTDCEN;
  }
}

/*********************************************************************
*
*       _LTDC_ITConfig
*/
static void _LTDC_ITConfig(U32 LTDC_IT, int NewState) {
  if (NewState != DISABLE) {
    LTDC->IER |= LTDC_IT;
  } else {
    LTDC->IER &= (U32)~LTDC_IT;
  }
}

/*********************************************************************
*
*       _LTDC_SetLayerPos
*/
static void _LTDC_SetLayerPos(int LayerIndex, int xPos, int yPos) {
  int xSize, ySize;
  U32 HorizontalStart, HorizontalStop, VerticalStart, VerticalStop;

  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);
  HorizontalStart = xPos + HBP + 1;
  HorizontalStop  = xPos + HBP + xSize;
  VerticalStart   = yPos + VBP + 1;
  VerticalStop    = yPos + VBP + ySize;
  //
  // Horizontal start and stop position
  //
  _apLayer[LayerIndex]->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  _apLayer[LayerIndex]->WHPCR = (HorizontalStart | (HorizontalStop << 16));
  //
  // Vertical start and stop position
  //
  _apLayer[LayerIndex]->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  _apLayer[LayerIndex]->WVPCR  = (VerticalStart | (VerticalStop << 16));
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_SetLayerAlpha
*/
static void _LTDC_SetLayerAlpha(int LayerIndex, int Alpha) {
  //
  // Set constant alpha value
  //
  _apLayer[LayerIndex]->CACR &= ~(LTDC_LxCACR_CONSTA);
  _apLayer[LayerIndex]->CACR  = 255 - Alpha;
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR); // Reload immediately
}

/*********************************************************************
*
*       _LTDC_SetLUTEntry
*/
static void _LTDC_SetLUTEntry(int LayerIndex, U32 Color, int Pos) {
  U32 r, g, b, a;

  r = ( Color        & 0xff) << 16;
  g = ((Color >>  8) & 0xff) <<  8;
  b = ((Color >> 16) & 0xff);
  a = Pos << 24;
  _apLayer[LayerIndex]->CLUTWR &= ~(LTDC_LxCLUTWR_BLUE | LTDC_LxCLUTWR_GREEN | LTDC_LxCLUTWR_RED | LTDC_LxCLUTWR_CLUTADD);
  _apLayer[LayerIndex]->CLUTWR  = r | g | b | a;
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _LTDC_LayerInit
*/
static void _LTDC_LayerInit(LTDC_LAYER_T * LTDC_Layerx, LTDC_INIT_LAYER_T * LTDC_Layer_InitStruct) {
  U32 whsppos, wvsppos, dcgreen, dcred, dcalpha, cfbp;

  //
  // Configures the horizontal start and stop position
  //
  whsppos = LTDC_Layer_InitStruct->HorizontalStop << 16;
  LTDC_Layerx->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  LTDC_Layerx->WHPCR = (LTDC_Layer_InitStruct->HorizontalStart | whsppos);
  //
  // Configures the vertical start and stop position
  //
  wvsppos = LTDC_Layer_InitStruct->VerticalStop << 16;
  LTDC_Layerx->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  LTDC_Layerx->WVPCR  = (LTDC_Layer_InitStruct->VerticalStart | wvsppos);
  //
  // Specifies the pixel format
  //
  LTDC_Layerx->PFCR &= ~(LTDC_LxPFCR_PF);
  LTDC_Layerx->PFCR = (LTDC_Layer_InitStruct->PixelFormat);
  //
  // Configures the default color values
  //
  dcgreen = (LTDC_Layer_InitStruct->DefaultColorGreen <<  8);
  dcred   = (LTDC_Layer_InitStruct->DefaultColorRed   << 16);
  dcalpha = (LTDC_Layer_InitStruct->DefaultColorAlpha << 24);
  LTDC_Layerx->DCCR &=  ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
  LTDC_Layerx->DCCR = (LTDC_Layer_InitStruct->DefaultColorBlue | dcgreen | dcred | dcalpha);
  //
  // Specifies the constant alpha value
  //
  LTDC_Layerx->CACR &= ~(LTDC_LxCACR_CONSTA);
  LTDC_Layerx->CACR = (LTDC_Layer_InitStruct->ConstantAlpha);
  //
  // Specifies the blending factors
  //
  LTDC_Layerx->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
  LTDC_Layerx->BFCR = (LTDC_Layer_InitStruct->BlendingFactor_1 | LTDC_Layer_InitStruct->BlendingFactor_2);
  //
  // Configures the color frame buffer start address
  //
  LTDC_Layerx->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
  LTDC_Layerx->CFBAR = (LTDC_Layer_InitStruct->CFBStartAdress);
  //
  // Configures the color frame buffer pitch in byte
  //
  cfbp = (LTDC_Layer_InitStruct->CFBPitch << 16);
  LTDC_Layerx->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
  LTDC_Layerx->CFBLR  = (LTDC_Layer_InitStruct->CFBLineLength | cfbp);
  //
  // Configures the frame buffer line number
  //
  LTDC_Layerx->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
  LTDC_Layerx->CFBLNR  = (LTDC_Layer_InitStruct->CFBLineNumber);
}

/*********************************************************************
*
*       _NVIC_SetPriority
*/
static void _NVIC_SetPriority(int IRQn, U32 priority) {
  if(IRQn < 0) {
    while (1); // Not supported here, stop execution.
  } else {
    NVIC->IP[(U32)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); /* Set Priority for device specific Interrupts */
  }
}

/*********************************************************************
*
*       _NVIC_EnableIRQ
*/
static void _NVIC_EnableIRQ(int IRQn) {
  NVIC->ISER[(U32)((I32)IRQn) >> 5] = (U32)(1 << ((U32)((I32)IRQn) & (U32)0x1F)); /* Enable interrupt */
}

/*********************************************************************
*
*       _DMA2D_ITConfig
*/
static void _DMA2D_ITConfig(U32 DMA2D_IT, int NewState) {
  if (NewState != DISABLE) {
    DMA2D->CR |= DMA2D_IT;
  } else {
    DMA2D->CR &= (U32)~DMA2D_IT;
  }
}

/*********************************************************************
*
*       _DMA_ExecOperation
*/
static void _DMA_ExecOperation(void) {
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }
}
/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00000000UL | (1 << 9);         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = PixelFormat;                     // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_Fill
*/
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex) {
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00030000UL | (1 << 9);         // Control Register (Register to memory and TCIE)
  DMA2D->OCOLR   = ColorIndex;                      // Output Color Register (Color to be used)
  //
  // Set up pointers
  //
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->OOR     = OffLine;                         // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->OPFCCR  = PixelFormat;                     // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_AlphaBlendingBulk
*/
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register
  DMA2D->BGOR    = 0;                               // Background Offset Register
  DMA2D->OOR     = 0;                               // Output Offset Register
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888;       // Foreground PFC Control Register (Defines the FG pixel format)
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888;       // Background PFC Control Register (Defines the BG pixel format)
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;       // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_MixColors
*
* Purpose:
*   Function for mixing up 2 colors with the given intensity.
*   If the background color is completely transparent the
*   foreground color should be used unchanged.
*/
static LCD_COLOR _DMA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens) {
  U32 ColorFG, ColorBG, ColorDst;

  if ((BkColor & 0xFF000000) == 0xFF000000) {
    return Color;
  }
  ColorFG = Color   ^ 0xFF000000;
  ColorBG = BkColor ^ 0xFF000000;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)&ColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)&ColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)&ColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(1 << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //

  //_DMA_ExecOperation();
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }

  return ColorDst ^ 0xFF000000;
}

/*********************************************************************
*
*       _DMA_MixColorsBulk
*/
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_ConvertColor
*/
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = 0;                               // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_DrawBitmapL8
*/
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffSrc;                          // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffDst;                          // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_L8;             // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  _DMA_ExecOperation();
}

/*********************************************************************
*
*       _DMA_LoadLUT
*/
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems) {
  DMA2D->FGCMAR  = (U32)pColor;                     // Foreground CLUT Memory Address Register
  //
  // Foreground PFC Control Register
  //
  DMA2D->FGPFCCR  = LTDC_Pixelformat_RGB888         // Pixel format
                  | ((NumItems - 1) & 0xFF) << 8;   // Number of items to load
  DMA2D->FGPFCCR |= (1 << 5);                       // Start loading
  //
  // Waiting not required here...
  //
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa.
*/
static void _InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
  do {
    Color = *pColorSrc++;
    *pColorDst++ = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
                 |  (Color & 0x0000FF00)                // Green
                 | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
                 | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
  } while (--NumItems);
}

/*********************************************************************
*
*       _InvertAlpha
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*/
static void _InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;

  do {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha
  } while (--NumItems);
}

/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {
  //
  // Invert alpha values
  //
  _InvertAlpha(pColorFG, _aBuffer_FG, NumItems);
  _InvertAlpha(pColorBG, _aBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_aBuffer_FG, _aBuffer_BG, _aBuffer_DMA2D, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha(_aBuffer_DMA2D, pColorDst, NumItems);
}

/*********************************************************************
*
*       _DMA_Index2ColorBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given index values to 32 bit colors.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the color array needs to be converted after DMA2D has been used.
*/
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
  (void)SizeOfIndex;
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, _aBuffer_DMA2D, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB(_aBuffer_DMA2D, pColor, NumItems);
}

/*********************************************************************
*
*       _DMA_Color2IndexBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given 32 bit color array to index values.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the given color array needs to be converted before DMA2D can be used.
*/
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {
  (void)SizeOfIndex;
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB(pColor, _aBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_aBuffer_DMA2D, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
  unsigned int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha(pFG, _aBuffer_FG, xSize);
    _InvertAlpha(pBG, _aBuffer_BG, xSize);
    //
    //
    //
    _DMA_MixColorsBulk(_aBuffer_FG, _aBuffer_BG, _aBuffer_DMA2D, Intens, xSize);
    //
    //
    //
    _InvertAlpha(_aBuffer_DMA2D, pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}

void LCD_SPIConfig(void)
{
  SPI_InitTypeDef    SPI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable LCD_SPI_SCK_GPIO_CLK, LCD_SPI_MISO_GPIO_CLK and LCD_SPI_MOSI_GPIO_CLK clock */
  RCC_AHB1PeriphClockCmd(LCD_SPI_SCK_GPIO_CLK | LCD_SPI_MISO_GPIO_CLK | LCD_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable LCD_SPI and SYSCFG clock  */
  RCC_APB2PeriphClockCmd(LCD_SPI_CLK, ENABLE);

  /* Configure LCD_SPI SCK pin */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LCD_SPI MISO pin */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MISO_PIN;
  GPIO_Init(LCD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LCD_SPI MOSI pin */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
  GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Connect SPI SCK */
  GPIO_PinAFConfig(LCD_SPI_SCK_GPIO_PORT, LCD_SPI_SCK_SOURCE, LCD_SPI_SCK_AF);

  /* Connect SPI MISO */
  GPIO_PinAFConfig(LCD_SPI_MISO_GPIO_PORT, LCD_SPI_MISO_SOURCE, LCD_SPI_MISO_AF);

  /* Connect SPI MOSI */
  GPIO_PinAFConfig(LCD_SPI_MOSI_GPIO_PORT, LCD_SPI_MOSI_SOURCE, LCD_SPI_MOSI_AF);

  SPI_I2S_DeInit(LCD_SPI);

  /* SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(LCD_SPI, &SPI_InitStructure);

  /* SPI enable */
  SPI_Cmd(LCD_SPI, ENABLE);
}

/*********************************************************************
*
*       _LCD_DisplayOn
*/
static void _LCD_DisplayOn(void) {
  //
  // Enable LCD Backlight
  //
  GPIO_SetBits(GPIOA, GPIO_Pin_8);
  GPIO_SetBits(GPIOC, GPIO_Pin_6);
  //
  // Display On
  //
  LTDC_Cmd(ENABLE); /* display ON */
}

void LCD_CtrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal)
{
  /* Set or Reset the control line */
  GPIO_WriteBit(GPIOx, (uint16_t)CtrlPins, (BitAction)BitVal);
}

void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(LCD_NCS_GPIO_CLK, ENABLE);

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = LCD_NCS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_NCS_GPIO_PORT, &GPIO_InitStructure);

  LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
}

/*********************************************************************
*
*       _LCD_DisplayOff
*/
static void _LCD_DisplayOff(void) {
  //
  // Disable LCD Backlight
  //
  GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  GPIO_ResetBits(GPIOC, GPIO_Pin_6);
  //
  // Display Off
  //
  _LTDC_Cmd(DISABLE);
}

/**
  * @brief  Enable or Disable the LCD through CS pin
  * @param  NewState CS pin state
  * @retval None
  */
void LCD_ChipSelect(FunctionalState NewState)
{
  if (NewState == DISABLE)
  {
    GPIO_ResetBits(LCD_NCS_GPIO_PORT, LCD_NCS_PIN); /* CS pin low: LCD disabled */
  }
  else
  {
    GPIO_SetBits(LCD_NCS_GPIO_PORT, LCD_NCS_PIN); /* CS pin high: LCD enabled */
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for(index = nCount; index != 0; index--)
  {
  }
}

/**
  * @brief  Writes command to select the LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void LCD_WriteCommand(uint8_t LCD_Reg)
{
    /* Reset WRX to send command */
  LCD_CtrlLinesWrite(LCD_WRX_GPIO_PORT, LCD_WRX_PIN, Bit_RESET);

  /* Reset LCD control line(/CS) and Send command */
  LCD_ChipSelect(DISABLE);
  SPI_I2S_SendData(LCD_SPI, LCD_Reg);

  /* Wait until a data is sent(not busy), before config /CS HIGH */
  while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
  delay(10);
  LCD_ChipSelect(ENABLE);
}

/**
  * @brief  Writes data to select the LCD register.
  *         This function must be used after LCD_WriteCommand() function
  * @param  value: data to write to the selected register.
  * @retval None
  */
void LCD_WriteData(uint8_t value)
{
    /* Set WRX to send data */
  LCD_CtrlLinesWrite(LCD_WRX_GPIO_PORT, LCD_WRX_PIN, Bit_SET);

  /* Reset LCD control line(/CS) and Send data */
  LCD_ChipSelect(DISABLE);
  SPI_I2S_SendData(LCD_SPI, value);

  /* Wait until a data is sent(not busy), before config /CS HIGH */
  while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
  delay(10);
  LCD_ChipSelect(ENABLE);
}

void LCD_PowerOn(void)
{
  LCD_WriteCommand(0xCA);
  LCD_WriteData(0xC3);
  LCD_WriteData(0x08);
  LCD_WriteData(0x50);
  LCD_WriteCommand(LCD_POWERB);
  LCD_WriteData(0x00);
  LCD_WriteData(0xC1);
  LCD_WriteData(0x30);
  LCD_WriteCommand(LCD_POWER_SEQ);
  LCD_WriteData(0x64);
  LCD_WriteData(0x03);
  LCD_WriteData(0x12);
  LCD_WriteData(0x81);
  LCD_WriteCommand(LCD_DTCA);
  LCD_WriteData(0x85);
  LCD_WriteData(0x00);
  LCD_WriteData(0x78);
  LCD_WriteCommand(LCD_POWERA);
  LCD_WriteData(0x39);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x00);
  LCD_WriteData(0x34);
  LCD_WriteData(0x02);
  LCD_WriteCommand(LCD_PRC);
  LCD_WriteData(0x20);
  LCD_WriteCommand(LCD_DTCB);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteCommand(LCD_FRC);
  LCD_WriteData(0x00);
  LCD_WriteData(0x1B);
  LCD_WriteCommand(LCD_DFC);
  LCD_WriteData(0x0A);
  LCD_WriteData(0xA2);
  LCD_WriteCommand(LCD_POWER1);
  LCD_WriteData(0x10);
  LCD_WriteCommand(LCD_POWER2);
  LCD_WriteData(0x10);
  LCD_WriteCommand(LCD_VCOM1);
  LCD_WriteData(0x45);
  LCD_WriteData(0x15);
  LCD_WriteCommand(LCD_VCOM2);
  LCD_WriteData(0x90);
  LCD_WriteCommand(LCD_MAC);
  LCD_WriteData(0xC8);
  LCD_WriteCommand(LCD_3GAMMA_EN);
  LCD_WriteData(0x00);
  LCD_WriteCommand(LCD_RGB_INTERFACE);
  LCD_WriteData(0xC2);
  LCD_WriteCommand(LCD_DFC);
  LCD_WriteData(0x0A);
  LCD_WriteData(0xA7);
  LCD_WriteData(0x27);
  LCD_WriteData(0x04);

  /* colomn address set */
  LCD_WriteCommand(LCD_COLUMN_ADDR);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0xEF);
  /* Page Address Set */
  LCD_WriteCommand(LCD_PAGE_ADDR);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x01);
  LCD_WriteData(0x3F);
  LCD_WriteCommand(LCD_INTERFACE);
  LCD_WriteData(0x01);
  LCD_WriteData(0x00);
  LCD_WriteData(0x06);

  LCD_WriteCommand(LCD_GRAM);
  delay(200);

  LCD_WriteCommand(LCD_GAMMA);
  LCD_WriteData(0x01);

  LCD_WriteCommand(LCD_PGAMMA);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x29);
  LCD_WriteData(0x24);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x09);
  LCD_WriteData(0x4E);
  LCD_WriteData(0x78);
  LCD_WriteData(0x3C);
  LCD_WriteData(0x09);
  LCD_WriteData(0x13);
  LCD_WriteData(0x05);
  LCD_WriteData(0x17);
  LCD_WriteData(0x11);
  LCD_WriteData(0x00);
  LCD_WriteCommand(LCD_NGAMMA);
  LCD_WriteData(0x00);
  LCD_WriteData(0x16);
  LCD_WriteData(0x1B);
  LCD_WriteData(0x04);
  LCD_WriteData(0x11);
  LCD_WriteData(0x07);
  LCD_WriteData(0x31);
  LCD_WriteData(0x33);
  LCD_WriteData(0x42);
  LCD_WriteData(0x05);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x0A);
  LCD_WriteData(0x28);
  LCD_WriteData(0x2F);
  LCD_WriteData(0x0F);

  LCD_WriteCommand(LCD_SLEEP_OUT);
  delay(200);
  LCD_WriteCommand(LCD_DISPLAY_ON);
  /* GRAM start writing */
  LCD_WriteCommand(LCD_GRAM);
 }

static int _LCD_Init(void)
{
  LTDC_InitTypeDef       LTDC_InitStruct;
  GPIO_InitTypeDef       GPIO_InitStructure;

  /* Enable clock for NCS port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure the LCD Control pins ------------------------------------------*/
  LCD_CtrlLinesConfig();
  LCD_ChipSelect(DISABLE);
  LCD_ChipSelect(ENABLE);

  /* Configure the LCD_SPI interface -----------------------------------------*/
  LCD_SPIConfig();

  /* Power on the LCD --------------------------------------------------------*/
  LCD_PowerOn();

  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  /* Enable the DMA2D Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

  /* Configure the LCD Control pins */
  _LCD_AF_GPIOConfig();

  /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
//  SDRAM_Init();

  /* LTDC Configuration *********************************************************/
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  /* Initialize the vertical synchronization polarity as active low */
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  /* Initialize the data enable polarity as active low */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
  /* Initialize the pixel clock polarity as input pixel clock */
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

  /* Configure R,G,B component values for LCD background color */
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

  /* Configure PLLSAI prescalers for LCD */
  /* Enable Pixel Clock */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/4 = 48 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 48/8 = 6 Mhz */
  RCC_PLLSAIConfig(192, 7, 4);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

  /* Timing configuration */
  /* Configure horizontal synchronization width */
  LTDC_InitStruct.LTDC_HorizontalSync = 9;
  /* Configure vertical synchronization height */
  LTDC_InitStruct.LTDC_VerticalSync = 1;
  /* Configure accumulated horizontal back porch */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 29;
  /* Configure accumulated vertical back porch */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 3;
  /* Configure accumulated active width */
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
  /* Configure accumulated active height */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
  /* Configure total width */
  LTDC_InitStruct.LTDC_TotalWidth = 279;
  /* Configure total height */
  LTDC_InitStruct.LTDC_TotalHeigh = 327;

  LTDC_Init(&LTDC_InitStruct);
  return 0;
}

/*********************************************************************
*
*       _LCD_InitController
*/
static void _LCD_InitController(int LayerIndex) {
  LTDC_INIT_T       LTDC_InitStruct       = {0};
  LTDC_INIT_LAYER_T LTDC_Layer_InitStruct = {0};
  int xSize, ySize, BytesPerLine, BitsPerPixel, i;
  U32 Pixelformat, Color;
  static int Done;

  if (LayerIndex >= GUI_COUNTOF(_apLayer)) {
    return;
  }
  if (Done == 0) {
    Done = 1;
    //
    // Clock configuration
    //
    _RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);  // Enable LTDC Clock
    _RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); // Enable DMA2D Clock
    //
    // Configure the LCD Control pins
    //
    _LCD_Init();
    
    //
    // Configure PLLSAI prescalers for LCD:
    //   Enable Pixel Clock
    //   PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
    //   PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz
    //   PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz
    //   LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz
    //
    _RCC_PLLSAIConfig(192, 7, 3);
    _RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
    //
    // Enable PLLSAI Clock
    //
    _RCC_PLLSAICmd(ENABLE);
    //
    // Wait for PLLSAI activation
    //
    while(_RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
    //
    // Polarity configuration
    //
    LTDC_InitStruct.HSPolarity = LTDC_HSPolarity_AL;  // Horizontal synchronization polarity as active low
    LTDC_InitStruct.VSPolarity = LTDC_VSPolarity_AL;  // Vertical synchronization polarity as active low
    LTDC_InitStruct.DEPolarity = LTDC_DEPolarity_AL;  // Data enable polarity as active low
    LTDC_InitStruct.PCPolarity = LTDC_PCPolarity_IPC; // Pixel clock polarity as input pixel clock
    //
    // Configure R,G,B component values for LCD background color
    //
    LTDC_InitStruct.BackgroundRedValue   = (BK_COLOR >>  0) & 0xFF;
    LTDC_InitStruct.BackgroundGreenValue = (BK_COLOR >>  8) & 0xFF;
    LTDC_InitStruct.BackgroundBlueValue  = (BK_COLOR >> 16) & 0xFF;
    //
    // Timing configuration
    //
    LTDC_InitStruct.HorizontalSync     = HSW;                    // Horizontal synchronization width
    LTDC_InitStruct.VerticalSync       = VSW;                    // Vertical synchronization height
    LTDC_InitStruct.AccumulatedHBP     = HBP;                    // Accumulated horizontal back porch
    LTDC_InitStruct.AccumulatedVBP     = VBP;                    // Accumulated vertical back porch
    LTDC_InitStruct.AccumulatedActiveW = HBP + XSIZE_PHYS;       // Accumulated active width
    LTDC_InitStruct.AccumulatedActiveH = VBP + YSIZE_PHYS;       // Accumulated active height
    LTDC_InitStruct.TotalWidth         = HBP + XSIZE_PHYS + HFP; // Total width
    LTDC_InitStruct.TotalHeigh         = VBP + YSIZE_PHYS + VFP; // Total height
    _LTDC_Init(&LTDC_InitStruct);
    //
    // Enable line interrupt
    //
    _LTDC_ITConfig(LTDC_IER_LIE, ENABLE);
    _NVIC_SetPriority(LTDC_IRQn, 0);
    _NVIC_EnableIRQ(LTDC_IRQn);
    //
    // Enable DMA2D transfer complete interrupt
    //
    _DMA2D_ITConfig(DMA2D_CR_TCIE, ENABLE);
    _NVIC_SetPriority(DMA2D_IRQn, 0);
    _NVIC_EnableIRQ(DMA2D_IRQn);
    //
    // Clear transfer complete interrupt flag
    //
    DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
  }
  //
  // Layer configuration
  //
  if (LCD_GetSwapXYEx(LayerIndex)) {
    xSize = LCD_GetYSizeEx(LayerIndex);
    ySize = LCD_GetXSizeEx(LayerIndex);
  } else {
    xSize = LCD_GetXSizeEx(LayerIndex);
    ySize = LCD_GetYSizeEx(LayerIndex);
  }
  //
  // Windowing configuration
  //   Horizontal start = horizontal synchronization + Horizontal back porch = 43
  //   Vertical start   = vertical synchronization   + vertical back porch   = 12
  //   Horizontal stop  = Horizontal start + LCD_PIXEL_WIDTH  -1
  //   Vertical stop    = Vertical start   + LCD_PIXEL_HEIGHT -1
  //
  LTDC_Layer_InitStruct.HorizontalStart = HBP + 1;
  LTDC_Layer_InitStruct.HorizontalStop  = (xSize + HBP);
  LTDC_Layer_InitStruct.VerticalStart   = VBP + 1;
  LTDC_Layer_InitStruct.VerticalStop    = (ySize + VBP);
  //
  // Pixel Format configuration
  //
  Pixelformat = _GetPixelformat(LayerIndex);
  LTDC_Layer_InitStruct.PixelFormat = Pixelformat;
  //
  // Alpha constant (255 totally opaque)
  //
  LTDC_Layer_InitStruct.ConstantAlpha = 255;
  //
  // Default Color configuration (configure A, R, G, B component values)
  //
  LTDC_Layer_InitStruct.DefaultColorBlue  = 0;
  LTDC_Layer_InitStruct.DefaultColorGreen = 0;
  LTDC_Layer_InitStruct.DefaultColorRed   = 0;
  LTDC_Layer_InitStruct.DefaultColorAlpha = 0;
  //
  // Configure blending factors
  //
  BytesPerLine = _GetBytesPerLine(LayerIndex, xSize);
  LTDC_Layer_InitStruct.BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  LTDC_Layer_InitStruct.CFBLineLength    = BytesPerLine + 3;
  LTDC_Layer_InitStruct.CFBPitch         = BytesPerLine;
  LTDC_Layer_InitStruct.CFBLineNumber    = ySize;
  //
  // Input Address configuration
  //
  LTDC_Layer_InitStruct.CFBStartAdress = _aAddr[LayerIndex];
  _LTDC_LayerInit(_apLayer[LayerIndex], &LTDC_Layer_InitStruct);
  //
  // Enable LUT on demand
  //
  BitsPerPixel = LCD_GetBitsPerPixelEx(LayerIndex);
  if (BitsPerPixel <= 8) {
    //
    // Enable usage of LUT for all modes with <= 8bpp
    //
    _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
  } else {
    //
    // Optional CLUT initialization for AL88 mode (16bpp)
    //
    if (_apColorConvAPI[LayerIndex] == GUICC_88666I) {
      _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
      for (i = 0; i < 256; i++) {
        Color = LCD_API_ColorConv_8666.pfIndex2Color(i);
        _LTDC_SetLUTEntry(LayerIndex, Color, i);
      }
    }
  }
  //
  // Enable layer
  //
  _LTDC_LayerCmd(_apLayer[LayerIndex], ENABLE);
  //
  // Reload configuration
  //
  _LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _GetBufferSize
*/
static U32 _GetBufferSize(int LayerIndex) {
  U32 BufferSize;

  BufferSize = _axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex];
  return BufferSize;
}

/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, AddrSrc, AddrDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = _aAddr[LayerIndex] + BufferSize * IndexSrc;
  AddrDst    = _aAddr[LayerIndex] + BufferSize * IndexDst;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  _aBufferIndex[LayerIndex] = IndexDst; // After this function has been called all drawing operations are routed to Buffer[IndexDst]!
}

/*********************************************************************
*
*       _LCD_CopyRect
*/
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
  U32 BufferSize, AddrSrc, AddrDst;
  int OffLine;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex];
  OffLine = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, OffLine, OffLine);
}

/*********************************************************************
*
*       _LCD_FillRect
*/
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  U32 BufferSize, AddrDst;
  int xSize, ySize;

  if (GUI_GetDrawMode() == GUI_DM_XOR) {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/*********************************************************************
*
*       _LCD_DrawBitmap16bpp
*/
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/*********************************************************************
*
*       _LCD_DrawBitmap8bpp
*/
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  PixelFormat = _GetPixelformat(LayerIndex);
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/*********************************************************************
*
*       _LCD_GetpPalConvTable
*
* Purpose:
*   The emWin function LCD_GetpPalConvTable() normally translates the given colors into
*   index values for the display controller. In case of index based bitmaps without
*   transparent pixels we load the palette only to the DMA2D LUT registers to be
*   translated (converted) during the process of drawing via DMA2D.
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) {
  void (* pFunc)(void);
  int DoDefault = 0;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Convert palette colors from ARGB to ABGR
  //
  _InvertAlpha_SwapRB((U32 *)pLogPal->pPalEntries, _aBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_aBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _aBuffer_DMA2D;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       DMA2D_ISR_Handler
*
* Purpose:
*   Transfer-complete-interrupt of DMA2D
*/
void DMA2D_IRQHandler(void);
void DMA2D_IRQHandler(void) {
  DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/*********************************************************************
*
*       LTDC_ISR_Handler
*
* Purpose:
*   End-Of-Frame-Interrupt for managing multiple buffering
*/
void LTDC_IRQHandler(void);
void LTDC_IRQHandler(void) {
  U32 Addr;
  int i;

  LTDC->ICR = (U32)LTDC_IER_LIE;
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    if (_aPendingBuffer[i] >= 0) {
      //
      // Calculate address of buffer to be used  as visible frame buffer
      //
      Addr = _aAddr[i] + _axSize[i] * _aySize[i] * _aPendingBuffer[i] * _aBytesPerPixels[i];
      //
      // Store address into SFR
      //
      _apLayer[i]->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
      _apLayer[i]->CFBAR = Addr;
      //
      // Reload configuration
      //
      _LTDC_ReloadConfig(LTDC_SRCR_IMR);
      //
      // Tell emWin that buffer is used
      //
      GUI_MULTIBUF_ConfirmEx(i, _aPendingBuffer[i]);
      //
      // Clear pending buffer flag of layer
      //
      _aPendingBuffer[i] = -1;
    }
  }
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r = 0;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the display controller and put it into operation.
    //
    _LCD_InitController(LayerIndex);
    break;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;

    p = (LCD_X_SETORG_INFO *)pData;
    _apLayer[LayerIndex]->CFBAR = _aAddr[LayerIndex] + p->yPos * _axSize[LayerIndex] * _aBytesPerPixels[LayerIndex];
    _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
  }
  case LCD_X_SHOWBUFFER: {
    //
    // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
    //
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
    _aPendingBuffer[LayerIndex] = p->Index;
    break;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;

    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _LTDC_SetLUTEntry(LayerIndex, p->Color, p->Pos);
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOn();
    break;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOff();
    break;
  }
  case LCD_X_SETVIS: {
    //
    // Required for setting the layer visibility which is passed in the 'OnOff' element of pData
    //
    LCD_X_SETVIS_INFO * p;

    p = (LCD_X_SETVIS_INFO *)pData;
    _LTDC_LayerCmd(_apLayer[LayerIndex], p->OnOff ? ENABLE : DISABLE);
    break;
  }
  case LCD_X_SETPOS: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETPOS_INFO * p;

    p = (LCD_X_SETPOS_INFO *)pData;
    _LTDC_SetLayerPos(LayerIndex, p->xPos, p->yPos);
    break;
  }
  case LCD_X_SETSIZE: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETSIZE_INFO * p;
    int xPos, yPos;

    xPos = XSIZE_PHYS;
    yPos = YSIZE_PHYS;
    p = (LCD_X_SETSIZE_INFO *)pData;
    if (LCD_GetSwapXYEx(LayerIndex)) {
      _axSize[LayerIndex] = p->ySize;
      _aySize[LayerIndex] = p->xSize;
    } else {
      _axSize[LayerIndex] = p->xSize;
      _aySize[LayerIndex] = p->ySize;
    }
    _LTDC_SetLayerPos(LayerIndex, xPos, yPos);
    break;
  }
  case LCD_X_SETALPHA: {
    //
    // Required for setting the alpha value which is passed in the 'Alpha' element of pData
    //
    LCD_X_SETALPHA_INFO * p;

    p = (LCD_X_SETALPHA_INFO *)pData;
    _LTDC_SetLayerAlpha(LayerIndex, p->Alpha);
    break;
  }
  case LCD_X_SETCHROMAMODE: {
    //
    // Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData
    //
    LCD_X_SETCHROMAMODE_INFO * p;

    p = (LCD_X_SETCHROMAMODE_INFO *)pData;
    _LTDC_LayerEnableColorKeying(_apLayer[LayerIndex], (p->ChromaMode != 0) ? ENABLE : DISABLE);
    break;
  }
  case LCD_X_SETCHROMA: {
    //
    // Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData
    //
    LCD_X_SETCHROMA_INFO * p;
    U32 Color;

    p = (LCD_X_SETCHROMA_INFO *)pData;
    Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
    _apLayer[LayerIndex]->CKCR = Color;
    _LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
  }
  default:
    r = -1;
  }
  return r;
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  int i;
  U32 PixelFormat;

  //
  // At first initialize use of multiple buffers on demand
  //
  #if (NUM_BUFFERS > 1)
    for (i = 0; i < GUI_NUM_LAYERS; i++) {
      GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  //
  // Set size of 1st layer
  //
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_0, XSIZE_0);
    LCD_SetVSizeEx(0, YSIZE_0 * NUM_VSCREENS, XSIZE_0);
  } else {
    LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
    LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  }
  #if (GUI_NUM_LAYERS > 1)
    //
    // Set display driver and color conversion for 2nd layer
    //
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    //
    // Set size of 2nd layer
    //
    if (LCD_GetSwapXYEx(1)) {
      LCD_SetSizeEx (1, YSIZE_0, XSIZE_1);
      LCD_SetVSizeEx(1, YSIZE_1 * NUM_VSCREENS, XSIZE_1);
    } else {
      LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
      LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
    }
  #endif
  //
  // Setting up VRam address and remember pixel size
  //
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    LCD_SetVRAMAddrEx(i, (void *)(_aAddr[i]));                                                 // Setting up VRam address
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;                                       // Remember pixel size
  }
  //
  // Setting up custom functions
  //
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    if (_aOrientation[i] == ROTATION_0) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);               // Set custom function for copying complete buffers (used by multiple buffering) using DMA2D
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);                   // Set custom function for copy recxtangle areas (used by GUI_CopyRect()) using DMA2D
      //
      // Set functions for direct color mode layers. Won't work with indexed color modes because of missing LUT for DMA2D destination
      //
      PixelFormat = _GetPixelformat(i);
      if (PixelFormat <= LTDC_Pixelformat_ARGB4444) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);                 // Set custom function for filling operations using DMA2D
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp);       // Set up custom drawing routine for index based bitmaps using DMA2D
      }
      //
      // Set up drawing routine for 16bpp bitmap using DMA2D
      //
      if (PixelFormat == LTDC_Pixelformat_RGB565) {
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
      }
    }
  }
  //
  // Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination
  //
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB1555
  GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB565
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB4444
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB888
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB8888
  //
  // Set up custom alpha blending function using DMA2D
  //
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);                                                // Set up custom alpha blending function using DMA2D
  //
  // Set up custom function for translating a bitmap palette into index values.
  // Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
  //
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  //
  // Set up a custom function for mixing up single colors using DMA2D
  //
  GUI_SetFuncMixColors(_DMA_MixColors);
  //
  // Set up a custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
}

/*************************** End of file ****************************/
