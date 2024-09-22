#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"


uint8_t RxData;

int main(void)
{
	OLED_Init();
	
	Serial_Init();
	
	Serial_TxPacket[0] = 0x10;
	Serial_TxPacket[1] = 0x88;
	Serial_TxPacket[2] = 0x66;
	Serial_TxPacket[3] = 0x44;

	OLED_ShowString(1,1,"RxData:");
	
	Serial_SendPacket();
	while (1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(2,1,Serial_RxPacket[0],2);
			OLED_ShowHexNum(2,4,Serial_RxPacket[1],2);
			OLED_ShowHexNum(2,7,Serial_RxPacket[2],2);
			OLED_ShowHexNum(2,10,Serial_RxPacket[3],2);
		}

		
	}
}




