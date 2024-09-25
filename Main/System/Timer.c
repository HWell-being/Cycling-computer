#include "stm32f10x.h"

extern uint32_t lastCaptureTime;
extern uint32_t wheelRPM;
extern uint32_t wheelSpeed_kmh;  // Speed in km/h * 10 (for one decimal place)
const uint16_t wheelCircumference = 1256;  // Wheel circumference in mm (adjust as needed)

void TIM2_Capture_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;  // 72MHz / 7200 = 10kHz (0.1ms resolution)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0F;  // Increased filter for debouncing
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        
        uint32_t captureTime = TIM_GetCapture1(TIM2);
        uint32_t timeDiff = captureTime - lastCaptureTime;
        lastCaptureTime = captureTime;
        
        if (timeDiff > 0)
        {
            wheelRPM = 600000 / timeDiff;  // 60 * 10000 / timeDiff
            wheelSpeed_kmh = (uint32_t)wheelCircumference * wheelRPM * 6 / 1000;  // km/h * 10
        }
        else
        {
            wheelRPM = 0;
            wheelSpeed_kmh = 0;
        }
    }
}

