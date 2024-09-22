#include "stm32f10x.h"                  // Device header


void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 );
}
void LED1_ON(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);

}

void LED1_OFF(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);

}

void LED1_TURE(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1) == 0)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	}
}



void LED2_ON(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);

}

void LED2_OFF(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);

}

void LED2_TURE(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2) == 0)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	}
}