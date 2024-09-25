#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"
#include "MPU6050.h"
#include "stdio.h"
#include "stdlib.h"
#include "Timer.h"
#include "GPS.h"
#include "Serial.h"


uint32_t lastCaptureTime = 0;
uint32_t wheelRPM = 0;
uint32_t wheelSpeed_kmh = 0;

char angleStr[10];

extern GPRMC_Data gps_data;  

int main(void)
{
	OLED_Init();
	
	TIM2_Capture_Init();
	W25Q64_Init();	
	MPU6050_Init();
	GPS_Init();
	Serial_Init();
	
	
	
	
	
	while(1)
	{
		GPS_Process();
		DisplaySlopeAngle();
		
		

		OLED_ShowString(3,1,"RPM:000");
		OLED_ShowString(3,9,"V:000.00");
		OLED_ShowString(4, 1, "Slope:");
		OLED_ShowString(4, 9, angleStr);
		
		
		
	}
	
} 	


