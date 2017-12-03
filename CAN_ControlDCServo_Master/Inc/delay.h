#ifndef __DELAY_H
#define __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h> 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void delay_us(uint16_t period);
void delay_01ms(uint16_t period);

#endif /* __DELAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
