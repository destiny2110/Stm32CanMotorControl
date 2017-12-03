/* Includes ------------------------------------------------------------------*/
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Delay micro second
  * @param  None
  * @retval None
  */
void delay_us(uint16_t period)
{
	/* Set TIMx instance */
  __TIM6_CLK_ENABLE();
   
  TIM6->PSC = 83; // clk = SystemCorClock / 2 / (PSC+1) = 1MHz
	TIM6->ARR = period -1;
	TIM6->CNT = 0;
	TIM6->EGR = 1; //update register
	
	TIM6->SR = 0; //Clear overflow flag
	TIM6->CR1 = 1; //Enable timer 6
	
	while(!TIM6->SR);
	
	TIM6->CR1 = 0; //Stop timer 6
	__TIM6_CLK_DISABLE();
}

/**
  * @brief  Delay milisecond
  * @param  None
  * @retval None
  */
void delay_01ms(uint16_t period)
{
	/* Set TIMx instance */
  __TIM6_CLK_ENABLE();
   
  TIM6->PSC = 8399; // clk = SystemCorClock / 2 / (PSC+1) = 10kHz
	TIM6->ARR = period -1;
	TIM6->CNT = 0;
	TIM6->EGR = 1; //update register
	
	TIM6->SR = 0; //Clear overflow flag
	TIM6->CR1 = 1; //Enable timer 6
	
	while(!TIM6->SR);
	
	TIM6->CR1 = 0; //Stop timer 6
	__TIM6_CLK_DISABLE();
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
