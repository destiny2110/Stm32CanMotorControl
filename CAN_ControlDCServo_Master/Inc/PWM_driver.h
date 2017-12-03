#ifndef __PWM_DRIVER_H
#define __PWM_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Definition for TIMx clock resources */
#define TIM_PWM                           TIM3
#define TIM_PWM_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for USARTx Pins */
#define TIM_PWM_CLK_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM_DIR_CLK_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM_EN_CLK_CHANNEL_GPIO_PORT()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define TIM_PWM_GPIO_PORT				       				GPIOB
#define PWM_DIR_GPIO_PORT				       				GPIOB
#define PWM_EN_GPIO_PORT				       				GPIOD
#define TIM_PWM_GPIO_PIN_CHANNEL1             GPIO_PIN_4
#define PWM_GPIO_PIN_DIR		              		GPIO_PIN_3
#define PWM_GPIO_PIN_EN      			        		GPIO_PIN_7

/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define TIM_ENCODER                           TIM1
#define TIM_ENCODER_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Pins */
#define TIM_ENCODER_CHANNEL_1_GPIO_PORT()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define TIM_ENCODER_CHANNEL_2_GPIO_PORT()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define TIM_ENCODER_GPIO_PORT_1                GPIOE
#define TIM_ENCODER_GPIO_PORT_2                GPIOE
#define TIM_ENCODER_GPIO_PIN_CHANNEL_1         GPIO_PIN_9
#define TIM_ENCODER_GPIO_PIN_CHANNEL_2         GPIO_PIN_11
#define TIM_ENCODER_GPIO_AF_TIM                GPIO_AF1_TIM1


/* Exported functions ------------------------------------------------------- */
void init_Timer_PWM (void);
void init_Timer_ENCODER(void);
int32_t get_current_posistion(void);
void PWM0_Set_Duty(int16_t d);
void PWM0_Set_Freq(uint32_t freq);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
