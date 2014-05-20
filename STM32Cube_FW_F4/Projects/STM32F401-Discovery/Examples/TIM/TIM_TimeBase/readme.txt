/**
  @page TIM_TimeBase TIM_TimeBase example

  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   Description of the TIM Time Base example
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure the TIM peripheral in Output Compare Timing 
mode with the corresponding Interrupt requests for each channel in order to generate
4 different time bases.

The TIM3CLK frequency is set to SystemCoreClock / 2 (Hz), to get TIM3 counter 
clock at 60 KHz so the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1

SystemCoreClock is set to 84MHz for STM32F401xCx Devices.

The TIM3 CC1 register value is equal to 40961, 
CC1 update rate = TIM3 counter clock / CCR1_Val = 1.4648 Hz,
so the TIM3 Channel 1 generates an interrupt each 0.68 s

The TIM3 CC2 register is equal to 27309, 
CC2 update rate = TIM3 counter clock / CCR2_Val = 2.197 Hz
so the TIM3 Channel 2 generates an interrupt each 0.455 s

The TIM3 CC3 register is equal to 13654, 
CC3 update rate = TIM3 counter clock / CCR3_Val = 4.394 Hz
so the TIM3 Channel 3 generates an interrupt each 0.227 s

The TIM3 CC4 register is equal to 6826, 
CC4 update rate = TIM3 counter clock / CCR4_Val =  8.789 Hz
so the TIM3 Channel 4 generates an interrupt each 0.113 s

When the counter value reaches the Output compare registers values, the Output 
compare interrupts are generated and, in the handler routine, LED3(ORANGE, PD.13),
LED4(GREEN, PD.12), LED5(RED, PD.14) and  LED6(BLUE, PD.15) are toggled with the 
following frequencies: 

- PD.13: 1.464Hz (CC1)
- PD.12: 2.197Hz (CC2)
- PD.14: 4.394Hz (CC3) 
- PD.15: 8.789Hz (CC4)


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - TIM/TIM_TimeBase/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - TIM/TIM_TimeBase/Inc/stm32f4xx_it.h          DMA interrupt handlers header file
  - TIM/TIM_TimeBase/Inc/main.h                  Header for main.c module  
  - TIM/TIM_TimeBase/Src/stm32f4xx_it.c          DMA interrupt handlers
  - TIM/TIM_TimeBase/Src/main.c                  Main program
  - TIM/TIM_TimeBase/Src/stm32f4xx_hal_msp.c     HAL MSP file
  - TIM/TIM_TimeBase/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F401xCx devices.
    
  - This example has been tested with STM32F401-Discovery RevB board (MB1115B)and can be
    easily tailored to any other supported device and development board.    
      
  - STM32F401-Discovery RevB board (MB1115B) Set-up
    - Use LED3(ORANGE), LED4(GREEN), LED5(RED) and LED6(BLUE) connected respectively 
      to PD.13, PD.12, PD.14 and PD.15 pins and connect them on an oscilloscope to show 
      the different Time Base signals.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
