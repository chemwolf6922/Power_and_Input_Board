/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
#include "usbd_cdc_if.h"
uint8_t rxBuffer1[100] = {0};
uint8_t rxBuffer2[100] = {0};
uint8_t usbTxBuffer[100] = {0};
uint8_t rxBufferTail[112] = {0};
uint8_t bufferInUse = 0;
uint8_t testdata[2] = {0xaa,0xcc};
uint32_t lastTick = 0;
uint8_t nanoPIFlag = 0;
uint8_t startUpFlag = 0;
char targetString[12] = "pi@NanoPi-M1";
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_usart1_rx;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
	if(((HAL_GetTick()-lastTick)>200)&nanoPIFlag){
		char msg[] = "button pushed\n";
		HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),1);
	}
	lastTick = HAL_GetTick();
	
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
* @brief This function handles USB low priority or CAN RX0 interrupts.
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}

/**
* @brief This function handles TIM1 update interrupt.
*/
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */
  if(bufferInUse==0){
    
    bufferInUse = 1;
    HAL_UART_DMAStop(&huart1);
    HAL_UART_Receive_DMA(&huart1,rxBuffer2,100);
    uint8_t i = 0;
    for(i=0;i!=100;i++){
     if(rxBuffer1[i]==0x00){
       break;
     }else{
       usbTxBuffer[i] = rxBuffer1[i];
       rxBuffer1[i] = 0;
     }
    }

    if((i!=0x00)&&(startUpFlag==0)){
      uint8_t j = 0;
      for(j=0;j!=i;j++){
        rxBufferTail[12+j] = usbTxBuffer[j];
      }
      for(j=0;j!=i;j++){
        uint8_t k = 0;
        uint8_t foundStartPack = 0;
        for(k=0;k!=12;k++){
          foundStartPack = 1;
          if(rxBufferTail[j+k]!=(uint8_t)(targetString[k])){
            foundStartPack = 0;
            break;
          }
        }
        if(foundStartPack==1){
            startUpFlag = 1;
            break;
        }
      }
      if(i>=12){
        for(j=0;j!=12;j++){
          rxBufferTail[j] = usbTxBuffer[i-12+j];
        }
      }else{
        for(j=0;j!=12-i;j++){
          rxBufferTail[j] = 0x01;
        }
        for(j=0;j!=i;j++){
          rxBufferTail[12-i+j] = usbTxBuffer[j];
        }
      }
    }

    //testdata[0] = i;
    if(i!=0x00){
      CDC_Transmit_FS(usbTxBuffer,i);
    }
    

  }else{
    bufferInUse = 0;
    HAL_UART_DMAStop(&huart1);
    HAL_UART_Receive_DMA(&huart1,rxBuffer1,100);
    uint8_t i = 0;
    for(i=0;i!=100;i++){
      if(rxBuffer2[i]==0x00){
        break;
      }else{
        usbTxBuffer[i] = rxBuffer2[i];
        rxBuffer2[i] = 0;
      }
    }
    
    if((i!=0x00)&&(startUpFlag==0)){
      uint8_t j = 0;
      for(j=0;j!=i;j++){
        rxBufferTail[12+j] = usbTxBuffer[j];
      }
      for(j=0;j!=i;j++){
        uint8_t k = 0;
        uint8_t foundStartPack = 0;
        for(k=0;k!=12;k++){
          foundStartPack = 1;
          if(rxBufferTail[j+k]!=(uint8_t)(targetString[k])){
            foundStartPack = 0;
            break;
          }
        }
        if(foundStartPack==1){
            startUpFlag = 1;
            break;
        }
      }
      if(i>=12){
        for(j=0;j!=12;j++){
          rxBufferTail[j] = usbTxBuffer[i-12+j];
        }
      }else{
        for(j=0;j!=12-i;j++){
          rxBufferTail[j] = 0x01;
        }
        for(j=0;j!=i;j++){
          rxBufferTail[12-i+j] = usbTxBuffer[j];
        }
      }
    }
    
    //testdata[0] = i;
    if(i!=0x00){
      CDC_Transmit_FS(usbTxBuffer,i);
    }
    
  }
  /* USER CODE END TIM1_UP_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
