/**
  @page ewarm EWARM Project Template for STM32F4xx devices
  
  @verbatim
  ******************* (C) COPYRIGHT 2014 STMicroelectronics ********************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   This sub-directory contains all the user-modifiable files needed to
  *          create a new project linked with the STM32F4xx Standard Peripherals 
  *          Library and working with IAR Embedded Workbench for ARM (EWARM)
  *          software toolchain.
  ******************************************************************************
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
  @endverbatim
 
 @note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

 - project .ewd/.eww/.ewp: A pre-configured project file with the provided library 
                           structure that produces an executable image with IAR 
                           Embedded Workbench.
                
 - stm32f4xx_flash.icf : This file is the IAR Linker configuration file used to 
                         place program code (readonly) in internal FLASH and data
                         (readwrite, Stack and Heap)in internal SRAM. 
                         You can customize this file to your need.                                                                                

 - stm32f4xx_flash_extsram.icf: This file is the IAR Linker configuration file 
                                used to place program code (readonly) in internal 
                                FLASH and data (readwrite, Stack and Heap)in 
                                external SRAM. You can customize this file to your need.  
  
 - stm32f4xx_flash_extsdram.icf: This file is the IAR Linker configuration file 
                                 used to place program code (readonly) in internal 
                                 FLASH and data (readwrite, Stack and Heap)in 
                                 external SDRAM. You can customize this file to your need.  
                                                              
 - stm32f4xx_ram.icf:  This file is the IAR Linker configuration file used to 
                       place program code (readonly) and data (readwrite, Stack 
                       and Heap)in internal SRAM. 
                       You can customize this file to your need.                    
                             
 @par How to use it ?

 - Open the Project.eww workspace.
  - In the workspace toolbar select the project config:          
     - STM324x9I_EVAL: to configure the project for STM32F429x/439x devices.
     @note The needed define symbols for this config are already declared in the
           preprocessor section: USE_STM324x9I_EVAL, STM32F429X, USE_HAL_DRIVER
           
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
