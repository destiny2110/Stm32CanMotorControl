#ifndef __CAN_SERVICE_H
#define __CAN_SERVICE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h> 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void v_CAN_service_init(void);
void v_CAN_heart_beat(void);
void v_CAN_Kp(uint32_t value, uint16_t device_id);
void v_CAN_Ki(uint32_t value, uint16_t device_id);
void v_CAN_Kd(uint32_t value, uint16_t device_id);
void v_CAN_Setpoint(uint32_t setpoint, uint32_t period, uint32_t duty, uint16_t device_id);
void v_CAN_get_param(uint16_t device_id);
void v_CAN_sensor_value(uint32_t value);
void v_CAN_control(uint8_t u8_control, uint16_t device_id);
void v_CAN_process(void);



#endif /* __CAN_SERVICE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
