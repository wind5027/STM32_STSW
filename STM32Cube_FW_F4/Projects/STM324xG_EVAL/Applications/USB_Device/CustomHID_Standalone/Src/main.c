/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   USB device Custom HID demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef hUSBDDevice;
ADC_HandleTypeDef  AdcHandle;
uint16_t ADCConvertedValueX = 0;
uint16_t ADCConvertedValueX1 = 0;
uint8_t SendBuffer[2];
uint8_t PrevXferDone = 1;
extern uint32_t flag;
extern uint8_t Report_buf[2];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void EXTILine10_15_Config(void);
static void ADC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 168 Mhz */
  SystemClock_Config();
  
  /* Configure the ADC */ 
  ADC_Configuration();
    
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Configure EXTI Line13 */
  EXTILine10_15_Config();
  
  /* Init Device Library */
  USBD_Init(&hUSBDDevice, &HID_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&hUSBDDevice, &USBD_CUSTOM_HID);
  
  /* Start Device Process */
  USBD_Start(&hUSBDDevice);
  
  /* Run Application (Interrupt mode) */
  while (1)
  {
  }
}

/**
  * @brief  USBD HID Data Out Callback.
  * @param  pdev: Device Handle
  * @param  epnum: Endpoint index
  * @retval None
  */
void USBD_HID_DataOutCallback(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  GPIO_PinState LedState;
  
  if(epnum == 1) 
  {
    if(Report_buf[1] == 0)
    {
      LedState = GPIO_PIN_RESET;
    }
    else 
    {
      LedState = GPIO_PIN_SET;
    }
    
    switch(Report_buf[0])
    {
    case 1: /* LED1 */
      if(LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED1);
      }
      else
      {
        BSP_LED_Off(LED1);
      }
      break;
      
    case 2: /* LED2 */
      if(LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED2);
      }
      else
      {
        BSP_LED_Off(LED2);
      }
      break;
    case 3: /* LED3 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED3);
      }
      else
      {
        BSP_LED_Off(LED3);
      }
      break;
    case 4: /* LED4 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED4);
      }
      else
      {
        BSP_LED_Off(LED4);
      }
      break;
      
    default:
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4); 
      break;
    }
  }
  USBD_LL_PrepareReceive(pdev, CUSTOM_HID_EPOUT_ADDR , Report_buf , 2);
}

/**
  * @brief  USBD HID Endpoint 0 Data Out Callback.
  * @param  pdev: Device Handle
  * @retval None
  */
void USBD_HID_EP0_DataOutCallback(USBD_HandleTypeDef *pdev)
{
  GPIO_PinState LedState;
  
  if(flag == 1)
  {
    flag = 0;
    if(Report_buf[1] == 0)
    {
      LedState = GPIO_PIN_RESET;
    }
    else 
    {
      LedState = GPIO_PIN_SET;
    }
    
    switch(Report_buf[0])
    {
    case 1: /* Led 1 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED1);
      }
      else
      {
        BSP_LED_Off(LED1);
      }
      break;
      
    case 2: /* Led 2 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED2);
      }
      else
      {
        BSP_LED_Off(LED2);
      }
      break;
    case 3: /* Led 3 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED3);
      }
      else
      {
        BSP_LED_Off(LED3);
      }
      break;
    case 4: /* Led 4 */
      if (LedState != GPIO_PIN_RESET)
      {
        BSP_LED_On(LED4);
      }
      else
      {
        BSP_LED_Off(LED4);
      }
      break;
    default:
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4); 
      break;
    }
  }
}

/**
  * @brief  Configures EXTI Line13 in interrupt mode
  * @param  None
  * @retval None
  */
static void EXTILine10_15_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __GPIOG_CLK_ENABLE();
  
  /* Configure PG15 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable and set EXTI15_10 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {
   if((PrevXferDone)  && (hUSBDDevice.dev_state == USBD_STATE_CONFIGURED))
    {
      SendBuffer[0] = 0x05; 
      
      if(BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET)
      {
        SendBuffer[1] = 0x01;
      }
      else
      {
        SendBuffer[1] = 0x00;
      }
      USBD_CUSTOM_HID_SendReport(&hUSBDDevice, SendBuffer, 2);
      PrevXferDone = 0;
    }
  }
}

/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
static void ADC_Configuration(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /* Configure the ADC peripheral */
  AdcHandle.Instance = ADCx;
  
  AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution = ADC_RESOLUTION12b;
  AdcHandle.Init.ScanConvMode = DISABLE;
  AdcHandle.Init.ContinuousConvMode = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 0;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion = 1;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.EOCSelection = DISABLE;
  HAL_ADC_Init(&AdcHandle);
   
  /* Configure ADC regular channel */  
  sConfig.Channel = ADCx_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

  /* Start the conversion process and enable interrupt */  
  HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&ADCConvertedValueX, 1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInit;
  RCC_ClkInitTypeDef RCC_ClkInit;   

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Configure RCC Oscillators: All parameters can be changed according to user’s needs */
  RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInit.HSEState = RCC_HSE_ON;
  RCC_OscInit.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInit.PLL.PLLM = 25;
  RCC_OscInit.PLL.PLLN = 336;
  RCC_OscInit.PLL.PLLP = 2;
  RCC_OscInit.PLL.PLLQ = 7;
  HAL_RCC_OscConfig (&RCC_OscInit);
  
  /* RCC Clocks: All parameters can be changed according to user’s needs */
  RCC_ClkInit.ClockType = RCC_CLOCKTYPE_SYSCLK |RCC_CLOCKTYPE_HCLK |RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig (&RCC_ClkInit, FLASH_LATENCY_5);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
