#include "configuration.h"


void RCC_Configuration(void)
{ 
	// GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOD, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
	
	// USART
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE); 
    //RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART3, ENABLE);  	
	
	// TMR
	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR2, ENABLE);
	
}

void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
	
	// USART1 : Tx		
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9; 
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    // USART1 : Rx
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10; 
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
    
    // GPIO pin : PC5(LED)
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_5 ;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  
}

void UART_Configuration(void)
{
    USART_InitType USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
	
    USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART1 Receive and Transmit interrupts */
	USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);
	USART_INTConfig(USART1, USART_INT_TDE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void NVIC_Configuration(void)
{
	NVIC_InitType NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* Enable the TMR2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TMR2_GLOBAL_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMER_Configuration(void)
{
	TMR_TimerBaseInitType  TMR_TimeBaseStructure;
	TMR_OCInitType  TMR_OCInitStructure;
	uint16_t CCR1_Val = 1000;
	uint16_t PrescalerValue = 0;
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;
	
	/* Time base configuration */
	TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
	TMR_TimeBaseStructure.TMR_Period = 1000; // max 65535, 0.1s interrupt : 100
	TMR_TimeBaseStructure.TMR_DIV = PrescalerValue;
	TMR_TimeBaseStructure.TMR_ClockDivision = 0;
	TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
	
	TMR_TimeBaseInit(TMR2, &TMR_TimeBaseStructure);
	/* Output Compare Active Mode configuration: Channel1 */
	TMR_OCStructInit(&TMR_OCInitStructure);
	TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_Inactive;
	TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
	TMR_OCInitStructure.TMR_Pulse = CCR1_Val;	// duty cycle
	TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;
	
	TMR_OC1Init(TMR2, &TMR_OCInitStructure);

	TMR_OC1PreloadConfig(TMR2, TMR_OCPreload_Disable);
	
	/* TMR INT enable */
	TMR_INTConfig(TMR2, TMR_INT_CC1, ENABLE);

	TMR_Cmd(TMR2, ENABLE);	
}





