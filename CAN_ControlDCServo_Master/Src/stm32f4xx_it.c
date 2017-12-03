/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
  

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup CAN_Networking
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern CAN_HandleTypeDef    CanHandle;

extern UART_HandleTypeDef MODULE_UartHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (CAN), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA1_Stream5_IRQHandler(void)
{
  
}

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA1_Stream6_IRQHandler(void)
{

}

/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(CAN1_RX0_IRQn);
  HAL_CAN_IRQHandler(&CanHandle);
}


///**
//* @brief  This function handles CAN2 RX0 interrupt request.
//* @param  None
//* @retval None
//*/
//void CAN2_RX0_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CanHandle);
//}

///**
//* @brief  This function handles CAN1 RX1 interrupt request.
//* @param  None
//* @retval None
//*/
//void CAN1_RX1_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CanHandle);
//}


///**
//* @brief  This function handles CAN2 RX1 interrupt request.
//* @param  None
//* @retval None
//*/
//void CAN2_RX1_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CanHandle);
//}

///**
//* @brief  This function handles CAN1 TX interrupt request.
//* @param  None
//* @retval None
//*/
//void CAN1_TX_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CanHandle);
//}

///**
//* @brief  This function handles CAN2 TX interrupt request.
//* @param  None
//* @retval None
//*/
//void CAN2_TX_IRQHandler(void)
//{
// HAL_CAN_IRQHandler(&CanHandle);	
//}


/**
* @brief  This function handles USART2 interrupt request.
* @param  None
* @retval None
*/
void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler (&MODULE_UartHandle);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
