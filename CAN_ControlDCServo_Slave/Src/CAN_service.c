/* Includes ------------------------------------------------------------------*/
#include "CAN_driver.h"
#include "CAN_service.h"
#include "pid.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CAN_HandleTypeDef CanHandle;
Message CAN_Message_SEND = Message_Initializer;
Message CAN_Message_RECIEVE = Message_Initializer;

uint16_t u16_master_device_id = 0x01;
uint16_t u16_current_device_id = 0x02;
uint16_t u16_CAN_device_id;

uint8_t type_message = 0;
uint8_t u8_CAN_control = 0;
uint16_t u16_buf_idx = 0;
uint32_t u32_Kp;
uint32_t u32_Ki;
uint32_t u32_Kd;
uint32_t u32_Setpoint;
uint32_t u32_Period;
uint32_t u32_Duty;
uint8_t u8_control;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void v_CAN_service_init(void)
{
	CAN_MspInit();
	CAN_Config();
}

void v_CAN_heart_beat(void)
{
	u16_buf_idx = 0;
	CAN_Message_SEND.cob_id = u16_current_device_id + 0x700;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	CAN_Message_SEND.data[u16_buf_idx++] = 0x01;
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_Kp(uint32_t value, uint16_t device_id)
{
	uint32_t u32_temp;
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = device_id + 0x100;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	u32_temp = (uint32_t) value* 1000.0;
	
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_Ki(uint32_t value, uint16_t device_id)
{
	uint32_t u32_temp;
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = device_id + 0x200;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	u32_temp = (uint32_t) value* 1000.0;
	
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;

	canSend(&CAN_Message_SEND);
}

void v_CAN_Kd(uint32_t value, uint16_t device_id)
{
	uint32_t u32_temp;
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = device_id + 0x300;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	u32_temp = (uint32_t) value* 1000.0;
	
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_Setpoint(uint32_t setpoint, uint32_t period, uint32_t duty, uint16_t device_id)
{
	u16_buf_idx = 0;
	uint32_t u32_temp;
	
	CAN_Message_SEND.cob_id = device_id + 0x400;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	u32_temp = (uint32_t) setpoint* 1000.0;
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	u32_temp = (uint32_t) period* 1000.0;
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	u32_temp = (uint32_t) duty* 1000.0;
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_get_param(uint16_t device_id)
{
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = device_id + 0x500;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	CAN_Message_SEND.data[u16_buf_idx++] = 0x01;
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_sensor_value(uint32_t value)
{
	uint32_t u32_temp;
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = u16_current_device_id + 0x600;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	u32_temp = (uint32_t) value*1000.0;
	
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u32_temp & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}


// 00: Stop
// 01: Start

void v_CAN_control(uint8_t u8_control, uint16_t device_id)
{
	u16_buf_idx = 0;
	
	CAN_Message_SEND.cob_id = device_id + 0x800;
	CAN_Message_SEND.rtr = CAN_RTR_DATA;
	
	CAN_Message_SEND.data[u16_buf_idx++] = (uint8_t)(u8_control & 0xFF);
	
	CAN_Message_SEND.len = u16_buf_idx;
	
	canSend(&CAN_Message_SEND);
}

void v_CAN_process(void)
{
	CAN_Message_RECIEVE = m_CAN_Get_Message(&CanHandle);
	if ((CAN_Message_RECIEVE.cob_id != 0) | (CAN_Message_RECIEVE.len != 0))
	{
		u16_CAN_device_id = (uint8_t)(CAN_Message_SEND.cob_id & 0xFF);
		type_message = (uint8_t)(CAN_Message_SEND.cob_id>>8 & 0xFF);
		CAN_Message_RECIEVE.cob_id = 0;
		
		if (u16_CAN_device_id == u16_current_device_id)
		{
			switch(type_message)
			{
				case 0x01:	//Kp
				{
					//Copy to struct
					u32_Kp = (*(uint32_t *)&CAN_Message_RECIEVE.data[0]);
					pid_P.ProportionalGain_Gain = (double) u32_Kp / 1000.0;
					break;
				}
				case 0x02:	//Ki
				{
					//Copy to struct
					u32_Ki = (*(uint32_t *)&CAN_Message_RECIEVE.data[0]);
					pid_P.IntegralGain_Gain = (double) u32_Ki / 1000.0;
					break;
				}
				case 0x03:	//Kd
				{				
					//Copy to struct
					u32_Kd = (*(uint32_t *)&CAN_Message_RECIEVE.data[0]);
					pid_P.DerivativeGain_Gain = (double) u32_Kd / 1000.0;
					break;
				}
				case 0x04:	//Setpoint
				{
					//Copy to struct
					u32_Setpoint = (*(uint32_t *)&CAN_Message_RECIEVE.data[0]);
					u32_Period = (*(uint32_t *)&CAN_Message_RECIEVE.data[4]);
					u32_Duty = (*(uint32_t *)&CAN_Message_RECIEVE.data[8]);
					u32_Duty = u32_Duty * u32_Period / 100;
					
					pid_P.Setpointdeg_Amp = (double) u32_Setpoint / 1000.0;
					pid_P.Setpointdeg_Period = (double) u32_Period / 1000.0;
					pid_P.Setpointdeg_Duty = (double) u32_Duty / 1000.0;
					break;
				}
				case 0x05:	//Request Param
				{
					u32_Kp = (uint32_t) pid_P.ProportionalGain_Gain;
					u32_Ki = (uint32_t) pid_P.IntegralGain_Gain;
					u32_Kd = (uint32_t) pid_P.DerivativeGain_Gain;
					//Send param to slave
					v_CAN_Kp(u32_Kp, u16_current_device_id);
					delay_01ms(1);
					v_CAN_Ki(u32_Ki, u16_current_device_id);
					delay_01ms(1);
					v_CAN_Kd(u32_Kd, u16_current_device_id);
					break;
				}
				case 0x06:	//Sensor value
					break;
				case 0x07:	//Heart beat
					break;
				case 0x08:	//Control
					u8_control = (uint8_t) CAN_Message_RECIEVE.data[0];
					break;
			}
		}
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
