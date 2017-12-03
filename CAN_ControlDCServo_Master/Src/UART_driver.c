/* Includes ------------------------------------------------------------------*/
#include "UART_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SERIAL_CMD_MAX_LEN                      256
#define SERIAL_CMD_START_IDX                    0
#define SERIAL_CMD_CMD_IDX                      1
#define SERIAL_CMD_DATA_LENGTH_IDX              2
#define SERIAL_CMD_DATA_IDX                     4
#define SERIAL_CMD_LENGTH_MIN                   10


#define SERIAL_HANDLE_CMD_DELAY                 10
#define SERIAL_GET_DATA_TIMEOUT                 (SERIAL_HANDLE_CMD_DELAY * 10)
#define SERIAL_HANDLE_CMD_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE * 4)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef MODULE_UartHandle;
/* Check tx and rx complete flag. */
static uint8_t u8_tx_cplt = 0;
static uint8_t u8_uart_rcv_data = 0;
static uint8_t au8_rx_buf[UART_MAX_RX_BUF];
static uint16_t u16_rx_head = 0;
static uint16_t u16_rx_tail = 0;
static uint16_t u16_rx_buf_cnt = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void UART_MSP_Init(void);

static void UART_MSP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
	USARTx_RX_GPIO_CLK_ENABLE();
	USARTx_TX_GPIO_CLK_ENABLE();
	
	USARTx_CLK_ENABLE();
		
	
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
	
	/* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin 			= USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

void UART_set_comm_port(uint32_t baudRate)
{
	MODULE_UartHandle.Instance = USARTx;
  MODULE_UartHandle.Init.BaudRate   = baudRate;
  MODULE_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  MODULE_UartHandle.Init.StopBits   = UART_STOPBITS_1;
  MODULE_UartHandle.Init.Parity     = UART_PARITY_NONE;
  MODULE_UartHandle.Init.Mode       = UART_MODE_TX_RX;
	MODULE_UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	MODULE_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&MODULE_UartHandle);
  
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn); 

  /* For next time getting data */
  HAL_UART_Receive_IT(&MODULE_UartHandle, &u8_uart_rcv_data, 1);
}

void init_UART(void) 
{
	UART_MSP_Init();	
}

int32_t s32_UART_Send (uint8_t *pu8_tx_buf, uint16_t u16_length)
{
	int32_t s32_err = 0;
    
    if (u8_tx_cplt)
    {
        return (-1);
    }
    HAL_UART_Transmit_IT (&MODULE_UartHandle, pu8_tx_buf, u16_length);
    u8_tx_cplt = 1;
        
    return (s32_err);
}

uint16_t u16_UART_Receive (uint8_t *pu8_rx_buf, uint16_t u16_num_byte_read)
{
	uint16_t u16_bytes_read = 0;

    while ((u16_rx_tail != u16_rx_head) && (u16_bytes_read < u16_num_byte_read))
    {
        if ((u16_rx_tail + 1) < UART_MAX_RX_BUF)
        {
            *pu8_rx_buf++ = au8_rx_buf[u16_rx_tail++];
            u16_bytes_read++;
            if (u16_rx_buf_cnt > 0)
            {
                u16_rx_buf_cnt--;
            }
        }
        else
        {
            *pu8_rx_buf++ = au8_rx_buf[u16_rx_tail];
            u16_rx_tail = 0;
            u16_bytes_read++;
            if (u16_rx_buf_cnt > 0)
            {
                u16_rx_buf_cnt--;
            }
        }
        if (u16_rx_tail == u16_rx_head)
        {
            u16_rx_buf_cnt = 0;
        }
    }	

    return u16_bytes_read;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &MODULE_UartHandle)
	{
		u8_tx_cplt = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &MODULE_UartHandle)
	{
		if ((u16_rx_head + 1) < UART_MAX_RX_BUF)
			{
					if ((u16_rx_head + 1) != u16_rx_tail)
					{
							au8_rx_buf[u16_rx_head++] = u8_uart_rcv_data;
							u16_rx_buf_cnt++;
					}
			}
			else
			{
					if (u16_rx_tail != 0)
					{
							au8_rx_buf[u16_rx_head] = u8_uart_rcv_data;
							u16_rx_head = 0;
							u16_rx_buf_cnt++;
					}
			}
			HAL_UART_Receive_IT (&MODULE_UartHandle, &u8_uart_rcv_data, 1);
	}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{	
	HAL_UART_DeInit(&MODULE_UartHandle);
	UART_MSP_Init();
	UART_set_comm_port(9600);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
