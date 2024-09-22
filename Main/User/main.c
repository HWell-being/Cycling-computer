#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"


uint8_t MID;
uint16_t DID;

uint8_t Data_IN_Arry[] = {0xBC,0xAD,0x87,0x44};
uint8_t Data_OUT_Arry[4] ;

int main(void)
{
	OLED_Init();
	W25Q64_Init();
	
	OLED_ShowString(3,1,"IN:");
	OLED_ShowString(4,1,"OUT:"); 
	
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1,1,MID,4);
	OLED_ShowHexNum(2,1,DID,4);
	
	W25Q64_SectorErase(0x000000);
	W25Q64_PageProgram(0x000000,Data_IN_Arry,4);
	
	W25Q64_Read_Data(0x000000,Data_OUT_Arry,4);
	
	OLED_ShowHexNum(3,5,  Data_IN_Arry[0],2);
	OLED_ShowHexNum(3,8,  Data_IN_Arry[1],2);
	OLED_ShowHexNum(3,11, Data_IN_Arry[2],2);
	OLED_ShowHexNum(3,14, Data_IN_Arry[3],2);
	
	OLED_ShowHexNum(4,5,  Data_OUT_Arry[0],2);
	OLED_ShowHexNum(4,8,  Data_OUT_Arry[1],2);
	OLED_ShowHexNum(4,11, Data_OUT_Arry[2],2);
	OLED_ShowHexNum(4,14, Data_OUT_Arry[3],2);
	
	
	while(1)
	{
		
	}
	
} 	


