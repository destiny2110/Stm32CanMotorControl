/* Includes ------------------------------------------------------------------*/
#include "CAN_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define       CAN_OK                        0x00
#define       CAN_ERROR                     0xFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef CanHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief CAN MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
void CAN_MspInit(void)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* CAN1 Periph clock enable */
  CANx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  CANx_GPIO_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* CAN1 TX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_TX_AF;
  
  HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* CAN1 RX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_RX_AF;
  
  HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for CAN1 Reception complete interrupt */
  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0x0a, 0);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
}

/**
  * @brief CAN MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hcan: CAN handle pointer
  * @retval None
  */
void CAN_MspDeInit(void)
{
  /*##-1- Reset peripherals ##################################################*/
  CANx_FORCE_RESET();
  CANx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the CAN1 TX GPIO pin */
  HAL_GPIO_DeInit(CANx_TX_GPIO_PORT, CANx_TX_PIN);
  /* De-initialize the CAN1 RX GPIO pin */
  HAL_GPIO_DeInit(CANx_RX_GPIO_PORT, CANx_RX_PIN);
  
  /*##-4- Disable the NVIC for CAN reception #################################*/
  HAL_NVIC_DisableIRQ(CANx_RX_IRQn);
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  CAN_FilterConfTypeDef sFilterConfig;
  static CanTxMsgTypeDef TxMessage;
  static CanRxMsgTypeDef RxMessage;
  
  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandle.Instance = CANx;
  CanHandle.pTxMsg = &TxMessage;
  CanHandle.pRxMsg = &RxMessage;
    
  CanHandle.Init.TTCM = DISABLE;
  CanHandle.Init.ABOM = DISABLE;
  CanHandle.Init.AWUM = DISABLE;
  CanHandle.Init.NART = DISABLE;
  CanHandle.Init.RFLM = DISABLE;
  CanHandle.Init.TXFP = DISABLE;
  CanHandle.Init.Mode = CAN_MODE_NORMAL;
  CanHandle.Init.SJW = CAN_SJW_1TQ;
  CanHandle.Init.BS1 = CAN_BS1_14TQ;
  CanHandle.Init.BS2 = CAN_BS2_6TQ;
  CanHandle.Init.Prescaler = 4;
  
	 // 42Mhz / ( 4 * ( 1 + 6 + 5) ) = 500Kbps
  if(HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    /* Initialization Error */
  }

  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterNumber = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.BankNumber = 14;
  
  if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
  }
	if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
  }

}

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**   Function:
**      unsigned char canSend( Message *m)
**
**   Arguments:
**      (in) m   -   Pointer to CAN message to be sent out.
**
**   Return:
**      0x00     -   Send message successfully.
**      0xFF     -   Can't send message.
**
**   Description:
**      Send CAN message to CANOpen port.
**
**   Notes:
**      
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
unsigned char canSend(Message *m)
{
    uint8_t       u8_status = CAN_ERROR;
    uint32_t      u32_idx = 0;
    
    /* Transmission structure preparation. */
    CanHandle.pTxMsg->StdId = m->cob_id;
    CanHandle.pTxMsg->ExtId = 0x00;
    CanHandle.pTxMsg->RTR = m->rtr;
    CanHandle.pTxMsg->IDE = CAN_ID_STD;
    CanHandle.pTxMsg->DLC = m->len;
    /* Copy CAN data. */
    for (u32_idx = 0; u32_idx < m->len; u32_idx++)
    {
        CanHandle.pTxMsg->Data[u32_idx] = m->data[u32_idx];
    }
    
    /* Send CAN message out. */
    if (HAL_CAN_Transmit(&CanHandle, m->len) == HAL_OK)
    {
        u8_status = CAN_OK;
    }
    else
    {
        u8_status = CAN_ERROR;
    }
    
    return (u8_status);
}

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**   Function:
**      int32_t s32_CAN_Get_Port_Status (void)
**
**   Arguments:
**      N/A
**
**   Return:
**      0x00     -   OK.
**      (-1)     -   Error.
**
**   Description:
**      Get status of CANOpen port.
**
**   Notes:
**      
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int32_t s32_CAN_Get_Port_Status (void)
{
   if (HAL_CAN_GetState(&CanHandle) == HAL_CAN_STATE_ERROR)
   {
      return (-1);
   }
   else
   {
      return (0);
   }
}

Message m_CAN_Get_Message (CAN_HandleTypeDef *hcan)
{
    Message     m = Message_Initializer;
    uint32_t    u32_idx = 0;
    
    /* Set message COB ID */
    m.cob_id = hcan->pRxMsg->StdId;
    /* Set message length */
    m.len    = hcan->pRxMsg->DLC;
    /* Set if this is remote frame */
    m.rtr    = ((hcan->pRxMsg->RTR) == CAN_RTR_DATA)? 0 : 1;
    /* Get data. */
    for (u32_idx = 0; u32_idx < 8; u32_idx++)
    {
        m.data[u32_idx] = hcan->pRxMsg->Data[u32_idx];
    }
		return m;
}

/**
  * @brief  Transmission complete callback in non blocking mode 
  * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
  HAL_StatusTypeDef enm_hal_status;
//    int32_t       s32_task_switch;

    /* CANOpen CAN Handle. */
    if (hcan == &CanHandle)
    {
        /* Receive */
        enm_hal_status = HAL_CAN_Receive_IT (hcan, CAN_FIFO0);
//        portYIELD_FROM_ISR(s32_task_switch);
    }
    /* Other CAN Handles. */
    else
    {
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
