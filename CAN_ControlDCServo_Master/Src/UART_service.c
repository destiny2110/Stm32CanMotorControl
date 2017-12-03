/* Includes ------------------------------------------------------------------*/
#include "UART_driver.h"
#include "UART_service.h"
#include "delay.h"
#include "CAN_service.h"

/* Private typedef -----------------------------------------------------------*/
#define SERIAL_CMD_MAX_LEN											256
#define SERIAL_CMD_LENGTH_MIN 									5
#define SERIAL_CMD_START_IDX                    0
#define SERIAL_CMD_CMD_IDX                      1
#define SERIAL_CMD_DATA_LENGTH_IDX              2
#define SERIAL_CMD_DATA_IDX                     3
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef MODULE_UartHandle;
static uint8_t au8_serial_tx[SERIAL_CMD_MAX_LEN];
static uint8_t au8_serial_rx[SERIAL_CMD_MAX_LEN];
uint16_t u16_serial_tx_idx = 0;
uint16_t u16_buf_idx = 0;
uint32_t u32_data_timeout = 0;
uint8_t u8_start = 0;
uint8_t u8_cmd = 0;
uint8_t u8_data_len = 0;
uint8_t u8_UART_control = 0;
uint16_t u16_UART_device_id;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void v_UART_service_init(uint32_t baud)
{
	init_UART();
	UART_set_comm_port(baud);
}

void v_UART_param(uint32_t Kp, uint32_t Ki, uint32_t Kd, uint16_t u16_device_id)
{
	uint32_t u32_temp;
	u16_serial_tx_idx = 0;
	
	au8_serial_tx[u16_serial_tx_idx++] = ':';			//Start	CMD
	au8_serial_tx[u16_serial_tx_idx++] = 0x01;		//CMD		param
	au8_serial_tx[u16_serial_tx_idx++] = 14;			//CMD		data_length
	
	//Device ID
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u16_device_id & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u16_device_id >> 8) & 0xFF);
	
	// Parsing Kp
	u32_temp = (uint32_t) Kp * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	// Parsing Ki
	u32_temp = (uint32_t) Ki * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	// Parsing Kd
	u32_temp = (uint32_t) Kd * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	au8_serial_tx[u16_serial_tx_idx++] = '\r';		//End CMD
	au8_serial_tx[u16_serial_tx_idx++] = '\n';
	
	s32_UART_Send(au8_serial_tx, u16_serial_tx_idx);
}

void v_UART_Setpoint(uint32_t Setpoint, uint32_t Period, uint32_t Duty, uint16_t u16_device_id)
{
	uint32_t u32_temp;
	u16_serial_tx_idx = 0;
	
	au8_serial_tx[u16_serial_tx_idx++] = ':';			//Start	CMD
	au8_serial_tx[u16_serial_tx_idx++] = 0x02;		//CMD		Setpoint
	au8_serial_tx[u16_serial_tx_idx++] = 14;			//CMD		data_length
	
	//Device ID
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u16_device_id & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u16_device_id >> 8) & 0xFF);
	
	// Parsing Setpoint
	u32_temp = (uint32_t) Setpoint * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	// Parsing Period
	u32_temp = (uint32_t) Period * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	// Parsing Kd
	u32_temp = (uint32_t) Duty * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	au8_serial_tx[u16_serial_tx_idx++] = '\r';		//End CMD
	au8_serial_tx[u16_serial_tx_idx++] = '\n';
	
	s32_UART_Send(au8_serial_tx, u16_serial_tx_idx);
}

void v_UART_Sensor_Value(uint32_t Sensor, uint16_t u16_device_id)
{
	uint32_t u32_temp;
	u16_serial_tx_idx = 0;
	
	au8_serial_tx[u16_serial_tx_idx++] = ':';			//Start	CMD
	au8_serial_tx[u16_serial_tx_idx++] = 0x03;		//CMD		Sensor Value
	au8_serial_tx[u16_serial_tx_idx++] = 6;			//CMD		data_length
	
	//Device ID
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u16_device_id & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u16_device_id >> 8) & 0xFF);
	
	// Parsing Sensor
	u32_temp = (uint32_t) Sensor * 1000.0;
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(u32_temp & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 16) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((u32_temp >> 24) & 0xFF);
	
	au8_serial_tx[u16_serial_tx_idx++] = '\r';		//End CMD
	au8_serial_tx[u16_serial_tx_idx++] = '\n';
	
	s32_UART_Send(au8_serial_tx, u16_serial_tx_idx);
}

void v_UART_control(uint16_t device_id, uint8_t control)
{
	u16_serial_tx_idx = 0;
	
	au8_serial_tx[u16_serial_tx_idx++] = ':';		//Start	CMD
	au8_serial_tx[u16_serial_tx_idx++] = 0x04;	//CMD	control device
	au8_serial_tx[u16_serial_tx_idx++] = 3;			//CMD		data_length
	
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(device_id & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)((device_id >> 8) & 0xFF);
	au8_serial_tx[u16_serial_tx_idx++] = (uint8_t)(control & 0xFF);
	
	au8_serial_tx[u16_serial_tx_idx++] = '\r';		//End CMD
	au8_serial_tx[u16_serial_tx_idx++] = '\n';
	
	s32_UART_Send(au8_serial_tx, u16_serial_tx_idx);
}

void v_UART_process(void)
{
	/* Get data. */
	if (u16_UART_Receive ((uint8_t *)&au8_serial_rx[u16_buf_idx], 1))
	{
			u16_buf_idx++;
			
			/* Detect the end of each message. */
			if ((u16_buf_idx >= SERIAL_CMD_LENGTH_MIN) &&
					(au8_serial_rx[(u16_buf_idx - 2)] == '\r') && (au8_serial_rx[(u16_buf_idx - 1)] == '\n'))
			{
				/* Check the format and length of message. */
				/* Start. */
				u8_start = au8_serial_rx[SERIAL_CMD_START_IDX];
				/* Command. */
				u8_cmd = au8_serial_rx[SERIAL_CMD_CMD_IDX];
				/* Data length. */
				u8_data_len = au8_serial_rx[SERIAL_CMD_DATA_LENGTH_IDX];
				
				u16_UART_device_id = *(uint16_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX];
				
				if ((u8_start == ':') && (u8_cmd < 0x10) &&
            ((u8_data_len + SERIAL_CMD_LENGTH_MIN) == u16_buf_idx))
        {
					switch(u8_cmd)
					{
						case 0x01: 	//param
							if (u8_data_len == 3)
							{
								if (au8_serial_rx[SERIAL_CMD_DATA_IDX] == 0xAF)
								{
									//Request Param
									v_CAN_get_param(u16_UART_device_id);
								}
							}
							else if (u8_data_len == 14)
							{
								//Send param to slave
								v_CAN_Kp(*(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 2], u16_UART_device_id);
								delay_01ms(10);
								v_CAN_Ki(*(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 6], u16_UART_device_id);
								delay_01ms(10);
								v_CAN_Kd(*(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 10], u16_UART_device_id);
							}
							break;
						case 0x02: 	//setpoint
							if (u8_data_len == 3)
							{
								if (au8_serial_rx[SERIAL_CMD_DATA_IDX] == 0xAF)
								{
									//Request Setpoint
								}
							}
							else if (u8_data_len == 14)
							{
								//Send setpoint to slave
								v_CAN_Setpoint( *(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 2], 
																*(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 6],
																*(uint32_t *)&au8_serial_rx[SERIAL_CMD_DATA_IDX + 10],
																u16_UART_device_id);
							}
							break;
						case 0x03: 	//sensor value
							if (u8_data_len == 3)
							{
								if (au8_serial_rx[SERIAL_CMD_DATA_IDX] == 0xAF)
								{
									//Request sensor value
																		
								}
							}
							break;
						case 0x04: 	//control
							if (u8_data_len == 3)
							{
								u8_UART_control = au8_serial_rx[SERIAL_CMD_DATA_IDX+2];
								v_CAN_control(u8_UART_control, u16_UART_device_id);
							}
							break;
					}
				}
				else
				{
					u8_cmd = 0;
					/* Reset buffer. */
					u16_buf_idx = 0;
					memset (au8_serial_rx, 0, SERIAL_CMD_MAX_LEN);
				}
			}
	}
	else
	{
			/* Check the timeout of received data - 100ms. */
			u32_data_timeout += 1;
			if (u32_data_timeout >= 100)
			{
					u32_data_timeout = 0;
					/* Reset buffer. */
					u16_buf_idx = 0;
					memset (au8_serial_rx, 0, SERIAL_CMD_MAX_LEN);
			}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
