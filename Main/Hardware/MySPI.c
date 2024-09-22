#include "stm32f10x.h"                  // Device header

//此处定义为单片机接口连接外设的各个引脚，以外设引脚为命名规则
void MySPI_W_CS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

uint8_t MySPI_R_MOSO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}


void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//模式0
	MySPI_W_CS(1);
	MySPI_W_SCK(0);
	
}

void MySPI_Start()
{
	MySPI_W_CS(0);
}


void MySPI_Stop(void)
{
	MySPI_W_CS(1);
}


//模式0
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	for( i =0 ;i < 8 ; i++)
	{
		MySPI_W_MOSI(ByteSend & (0x80>>i));
		MySPI_W_SCK(1);
		if(MySPI_R_MOSO() == 1)
		{
			ByteReceive |= (0x80>>i);
		}
		MySPI_W_SCK(0);
	}
	return ByteReceive;
}
