#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h> 

/* Exported types ------------------------------------------------------------*/
/** 
 * @brief The CAN message structure 
 * @ingroup can
 */
typedef struct {
  unsigned short cob_id;	/**< message's ID */
  unsigned char rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  unsigned char len;		/**< message's length (0 to 8) */
  unsigned char data[8]; /**< message's datas */
} Message;

#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor CANx instance used and associated 
   resources */
/* Definition for CANx clock resources */
#define CANx                            CAN1
#define CANx_CLK_ENABLE()               __HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
     
#define CANx_FORCE_RESET()              __HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()            __HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define CANx_TX_PIN                    GPIO_PIN_9
#define CANx_TX_GPIO_PORT              GPIOB  
#define CANx_TX_AF                     GPIO_AF9_CAN1
#define CANx_RX_PIN                    GPIO_PIN_8
#define CANx_RX_GPIO_PORT              GPIOB 
#define CANx_RX_AF                     GPIO_AF9_CAN1

/* Definition for CANx's NVIC */
#define CANx_RX_IRQn                   CAN1_RX0_IRQn
#define CANx_RX_IRQHandler             CAN1_RX0_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CAN_MspInit(void);
void CAN_MspDeInit(void);
void CAN_Config(void);
unsigned char canSend(Message *m);
int32_t s32_CAN_Get_Port_Status (void);
Message m_CAN_Get_Message (CAN_HandleTypeDef *hcan);
#endif /* __CAN_DRIVER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
