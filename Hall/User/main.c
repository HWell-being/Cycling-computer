#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint32_t lastCaptureTime = 0;
uint32_t wheelRPM = 0;
uint32_t wheelSpeed_kmh = 0;


int main(void)
{

	OLED_Init();
	TIM2_Capture_Init();
	
	OLED_ShowString(1,1,"RPM:000/m");
	OLED_ShowString(2,1,"Speed:000.00km/h");
		
	while(1)
	{
		OLED_ShowNum(1,5,wheelRPM,3);
		OLED_ShowNum(2,7,wheelSpeed_kmh*0.01,3);
		OLED_ShowNum(2,11,wheelSpeed_kmh%100,2);

		
	}
	
} 
