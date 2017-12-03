#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h> 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           UART4
#define USARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_0
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF8_UART4
#define USARTx_RX_PIN                    GPIO_PIN_1
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF8_UART4



/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      UART4_IRQn
#define USARTx_IRQHandler                UART4_IRQHandler

/* Size of Transmission buffer */
#define UART_MAX_RX_BUF                         1024
/* Size of Reception buffer */
#define RXBUFFERSIZE                     UART_MAX_RX_BUF
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void init_UART(void);
int32_t s32_UART_Send (uint8_t *pu8_tx_buf, uint16_t u16_length);
uint16_t u16_UART_Receive (uint8_t *pu8_rx_buf, uint16_t u16_num_byte_read);
void UART_set_comm_port(uint32_t baudRate);

#endif /* __UART_DRIVER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
