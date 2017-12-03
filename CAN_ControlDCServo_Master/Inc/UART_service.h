#ifndef __UART_SERVICE_H
#define __UART_SERVICE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h> 
#include "string.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void v_UART_service_init(uint32_t baud);
void v_UART_process(void);
void v_UART_param(uint32_t Kp, uint32_t Ki, uint32_t Kd, uint16_t u16_device_id);
void v_UART_Setpoint(uint32_t Setpoint, uint32_t Period, uint32_t Duty, uint16_t u16_device_id);
void v_UART_Sensor_Value(uint32_t Sensor, uint16_t u16_device_id);
void v_UART_control(uint16_t device_id, uint8_t control);

#endif /* __UART_SERVICE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
