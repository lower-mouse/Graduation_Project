/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
#include "stm8l15x_it.h"
#include "variable.h"
/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  u1_printf("NonHandledInterrupt 0\n");
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  u1_printf("TRAP_IRQHandler\n");
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  u1_printf("FLASH_IRQHandler,1");
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  u1_printf("DMA1_CHANNEL0_1_IRQHandler,2 \n");
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   u1_printf("DMA1_CHANNEL2_3_IRQHandler,3 \n");
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
//INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
//{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */ 
//}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    if(EXTI_GetITStatus(EXTI_IT_PortE)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortE);   
    } 
    if(EXTI_GetITStatus(EXTI_IT_PortF)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortF);   
    }
  u1_printf("EXTIE_F_PVD_IRQHandler,5 \n");
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    if(EXTI_GetITStatus(EXTI_IT_PortG)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortG);   
    }
    if(EXTI_GetITStatus(EXTI_IT_PortB)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortB);   
    }
  u1_printf("EXTIB_G_IRQHandler,6 \n");
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    if(EXTI_GetITStatus(EXTI_IT_PortD)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortD);   
    }
    if(EXTI_GetITStatus(EXTI_IT_PortH)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_PortH);   
    }
    u1_printf("EXTID_H_IRQHandler,7 \n");
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
   if(EXTI_GetITStatus(EXTI_IT_Pin0)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin0);   
    }
  u1_printf("EXTI0_IRQHandler,8 \n");
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
     if(EXTI_GetITStatus(EXTI_IT_Pin1)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin1);   
    }
    u1_printf("EXTI1_IRQHandler,9 \n");
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    if(EXTI_GetITStatus(EXTI_IT_Pin2)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin2);   
      halt_flag = 1;
      USART3_Config();//usart1
      USART1_Config();//usart3
    }
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    if(EXTI_GetITStatus(EXTI_IT_Pin3)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin3);   
    }
    u1_printf("EXTI3_IRQHandler,11 \n");
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    if(EXTI_GetITStatus(EXTI_IT_Pin4)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin4);   
    }
    u1_printf("EXTI4_IRQHandlerH \n"); 
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
     if(EXTI_GetITStatus(EXTI_IT_Pin5)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin5);   
    }
     u1_printf("EXTI5_IRQHandlerH \n"); 
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
     if(EXTI_GetITStatus(EXTI_IT_Pin6)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin6);   
    }
    u1_printf("EXTI6_IRQHandler,14 \n");
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
     if(EXTI_GetITStatus(EXTI_IT_Pin7)!= RESET)
    {
      EXTI_ClearITPendingBit(EXTI_IT_Pin7);   
    }
     u1_printf("EXTI2_IRQHandlerH \n"); 
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("LCD_AES_IRQHandler,16 \n");
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("SWITCH_CSS_BREAK_DAC_IRQHandler,17 \n");
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("ADC1_COMP_IRQHandler,18 \n");
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19\n");
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("TIM2_CC_USART2_RX_IRQHandler,20 \n");
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
//INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
//{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
//}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
//INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
//{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
//}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("TIM1_UPD_OVF_TRG_COM_IRQHandler,23 \n");
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("TIM1_CC_IRQHandler,24 \n");
    u1_printf("EN_INIT\n");
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf(" TIM4_UPD_OVF_TRG_IRQHandler,25\n");
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */	u1_printf("SPI1_IRQHandler,26 \n");	
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
//INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
//{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    *///u1_printf(" \n");
//}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
//INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
//{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
//}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */u1_printf("I2C1_SPI2_IRQHandler,29 \n");
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/