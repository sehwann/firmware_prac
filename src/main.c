/* Includes ------------------------------------------------------------------*/
#include "global.h"


#define BUFFER_SIZE 128

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint8_t readPos;
    uint8_t writePos;
} Buffer;
Buffer uartBuffer;


void echoBufferData(void) 
{
    while(uartBuffer.readPos != uartBuffer.writePos) 
    {
		USART_SendData(USART1, uartBuffer.buffer[uartBuffer.readPos++]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TDE) == RESET)
        {
        }
		
        if (uartBuffer.readPos >= BUFFER_SIZE) {
            uartBuffer.readPos = 0;
		}
    }
}

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	UART_Configuration();
	TIMER_Configuration();
	NVIC_Configuration();
	Delay_init();
	
	while(1)
	{
	}
}

//  @brief  This function handles TMR2 global interrupt request.
void TMR2_GLOBAL_IRQHandler(void)
{
	//  after 1000ms
	if (TMR_GetINTStatus(TMR2, TMR_INT_CC1) != RESET)
	{
		TMR_ClearITPendingBit(TMR2, TMR_INT_CC1);	// Clear TMR2 Capture Compare1 interrupt pending bit
		echoBufferData();
		GPIOC->OPTDT ^=  GPIO_Pins_5; // LED Toggle
	}
}

// @brief  This function handles USART1 global interrupt request.
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_INT_RDNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_INT_RDNE);
		
		uartBuffer.buffer[uartBuffer.writePos++] = (uint8_t)USART_ReceiveData(USART1);
		
        if (uartBuffer.writePos >= BUFFER_SIZE) {
            uartBuffer.writePos = 0;
		}
	}

	if(USART_GetITStatus(USART1, USART_INT_TDE) != RESET) 
	{   
		USART_INTConfig(USART1, USART_INT_TDE, DISABLE);
	}
}
