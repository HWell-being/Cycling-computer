#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "string.h"
#include "LED.h"
#include "W25Q64.h"
#include "GPS.h"


extern GPRMC_Data gps_data;  // ȷ�������������GPS.c�ж����




int main(void)
{
	OLED_Init();
	W25Q64_Init();
	GPS_Init();
	Serial_Init();
	
	while (1)
	{
		GPS_Process();
		
	}
}




