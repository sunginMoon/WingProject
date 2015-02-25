// 
// STM32 Value line discovery
// Serial Communication
//

#include "stm32f10x.h"


void __attribute__((weak)) osa_mdelay(unsigned int msec);

void SerialSetup(void);
void print_byte(short  c);
void print_short(short  c);
void SendSerialAccelGryro( int16_t accelgyro[6] );



 void SerialSetup(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

	// Port ����
	// PA9  - Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA10  - Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// UART Port ����
	USART_InitStructure.USART_BaudRate    = 19200;
	USART_InitStructure.USART_WordLength   = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits    = USART_StopBits_1;
	USART_InitStructure.USART_Parity    = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode    = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

void loop(void)
{
//	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
//	{
//		char data = (u8)USART_ReceiveData(USART1);
//	}

	print_byte( '1' );
	osa_mdelay(100);
	
	print_byte( '0' );		
	osa_mdelay(100);	
}


int main(void)
{
	SerialSetup();

	while(1)
	{
		loop();
	}
}



void __attribute__((weak)) osa_mdelay(unsigned int msec)
{
	uint32_t temp;
	SysTick->LOAD=(uint32_t)msec*(HSE_VALUE/1000);
	SysTick->VAL =0x00;		// clear Count flag
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));	// wait Count flag set
	SysTick->CTRL=0x00;
	SysTick->VAL =0X00;
}

void __attribute__((weak)) delay_us(unsigned int usec)
{
	uint32_t temp;
	SysTick->LOAD=(uint32_t)usec*(HSE_VALUE/1000000);
	SysTick->VAL =0x00;		// clear Count flag
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));	// wait Count flag set
	SysTick->CTRL=0x00;
	SysTick->VAL =0X00;
}



void USART_SendData2(USART_TypeDef* USARTx, int16_t Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (int16_t)0x01FF);	
}


void print_byte(short  c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c);
}


void print_short(short  c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c >> 8);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c & 0xFF);	
}



int GetKey(char *pkey)
{
	int ret = 0;

	if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		*pkey = (u8)USART_ReceiveData(USART1);
		ret = 1;
	}
	return ret;
}
