#include "stm32f10x.h"                  // Device header
#include "W25Q64.h"
#include "string.h"
#include "OLED.h"
#include "Serial.h"
#include "stdlib.h"
#include "math.h"

#define GPS_BUFFER_SIZE 256

uint8_t gpsBuffer[GPS_BUFFER_SIZE];
uint16_t gpsDataIndex = 0;

typedef struct {
    char utc_time[11];   // UTCʱ��:hhmmss.ss
    char status;         // ״̬:A=��Ч,V=��Ч
    char latitude[10];   // γ��:ddmm.mmmm
    char ns_indicator;   // N/Sָʾ��:N=��γ,S=��γ
    char longitude[11];  // ����:dddmm.mmmm
    char ew_indicator;   // E/Wָʾ��:E=����,W=����
    char speed[6];       // �ٶ�:��λ��
    char course[6];      // ����:��λ��
    char date[7];        // ����:ddmmyy
} GPRMC_Data;

GPRMC_Data gps_data;


void GPS_ParseNMEA(void);
void GPS_SaveToFlash(void);
void GPS_UpdateOLED(void);

void GPS_Init(void)
{
	// ???USART3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // USART3 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // USART3 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;  // GPS NEO-7M 
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

    // ??DMA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)gpsBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = GPS_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

    USART_Cmd(USART3, ENABLE);
}


void GPS_Process(void)
{
    static uint16_t lastIndex = 0;
    uint16_t currentIndex = GPS_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);

    if (currentIndex != lastIndex)
    {
        while (lastIndex != currentIndex)
        {
            if (gpsBuffer[lastIndex] == '$')
            {
                gpsDataIndex = 0;
            }
            
            if (gpsDataIndex < GPS_BUFFER_SIZE - 1)
            {
                gpsBuffer[gpsDataIndex++] = gpsBuffer[lastIndex];
            }

            if (gpsBuffer[lastIndex] == '\n')
            {
                GPS_ParseNMEA();
                gpsDataIndex = 0;
            }

            lastIndex = (lastIndex + 1) % GPS_BUFFER_SIZE;
        }
    }
}

int Parse_GPRMC(char *nmea_sentence, GPRMC_Data *gps_data)
{
    if (strncmp(nmea_sentence, "$GPRMC", 6) != 0) {
//        Serial_Printf("Error, %s\r\n", nmea_sentence);
        return -1;
    }

    char *token;
    int field_index = 0;

    // ʹ�ö��ŷָ����
    token = strtok(nmea_sentence, ",");
    while (token != NULL) {
        switch (field_index) {
            case 1: // UTCʱ��
                strncpy(gps_data->utc_time, token, sizeof(gps_data->utc_time) - 1);
                gps_data->utc_time[sizeof(gps_data->utc_time) - 1] = '\0';
                break;
            case 2: // ״̬
                gps_data->status = token[0];
                break;
            case 3: // γ��
                strncpy(gps_data->latitude, token, sizeof(gps_data->latitude) - 1);
                gps_data->latitude[sizeof(gps_data->latitude) - 1] = '\0';
                break;
            case 4: // N/Sָʾ��
                gps_data->ns_indicator = token[0];
                break;
            case 5: // ����
                strncpy(gps_data->longitude, token, sizeof(gps_data->longitude) - 1);
                gps_data->longitude[sizeof(gps_data->longitude) - 1] = '\0';
                break;
            case 6: // E/Wָʾ��
                gps_data->ew_indicator = token[0];
                break;
            case 7: // �ٶ�
                strncpy(gps_data->speed, token, sizeof(gps_data->speed) - 1);
                gps_data->speed[sizeof(gps_data->speed) - 1] = '\0';
                break;
            case 8: // ����
                strncpy(gps_data->course, token, sizeof(gps_data->course) - 1);
                gps_data->course[sizeof(gps_data->course) - 1] = '\0';
                break;
            case 9: // ����
                strncpy(gps_data->date, token, sizeof(gps_data->date) - 1);
                gps_data->date[sizeof(gps_data->date) - 1] = '\0';
                break;
            default:
                break;
        }
        // ��ȡ��һ���ֶ�
        token = strtok(NULL, ",");
        field_index++;
    }
    return 0;
}


void GPS_ParseNMEA(void)
{
    if (Parse_GPRMC((char*)gpsBuffer, &gps_data) == 0) {
        // GPRMC���ݽ����ɹ�
        GPS_SaveToFlash();
        GPS_UpdateOLED();
    }
}

void GPS_SaveToFlash(void)
{
    
    static uint32_t flashAddress = 0;
    W25Q64_PageProgram(flashAddress, gpsBuffer, gpsDataIndex);
    flashAddress += gpsDataIndex;
    if (flashAddress >= 4096) 
    {
        flashAddress = 0;
        W25Q64_SectorErase(0); 
    }
}

void formatTime(char* input, char* output)
{
    // ��ȡСʱ�����ӡ���
    int hour = (input[0] - '0') * 10 + (input[1] - '0');
    int minute = (input[2] - '0') * 10 + (input[3] - '0');
    int second = (input[4] - '0') * 10 + (input[5] - '0');

    // ����8Сʱ
    hour += 8;

    // �������24Сʱ������е���
    if (hour >= 24) {
        hour -= 24;
    }

    // �������ʽ���� output ��
    snprintf(output, 9, "%02d:%02d:%02d", hour, minute, second);
}

double convertToDegrees(const char* input)
{
    double value = atof(input);
    int degrees = (int)(value / 100);
    double minutes = value - degrees * 100;
    double result = degrees + minutes / 60.0;
    
    // ����һλС��
    result = round(result * 10) / 10.0;
    
    return result;
}

// ���� GPS ����
void processGPSData(GPRMC_Data* gps_data)
{
    char timeStr[9];
    formatTime(gps_data->utc_time, timeStr);
    strcpy(gps_data->utc_time, timeStr);

    double latitude = convertToDegrees(gps_data->latitude);
    double longitude = convertToDegrees(gps_data->longitude);

    sprintf(gps_data->latitude, "%.1f", latitude);
    sprintf(gps_data->longitude, "%.1f", longitude);
}


void GPS_UpdateOLED(void)
{
	processGPSData(&gps_data);
	
    OLED_Clear();
    OLED_ShowString(1, 1, "Time:");
    OLED_ShowString(1, 6, gps_data.utc_time);
    OLED_ShowString(2, 1, gps_data.latitude);
	
    // �� N/S �� E/W ָʾ��ת��Ϊ���ַ��ַ���
    char ns_indicator_str[] = { gps_data.ns_indicator, '\0' };  // ����ȷ������ '\0' ��β���ַ���
    char ew_indicator_str[] = { gps_data.ew_indicator, '\0' };  // ͬ��

    OLED_ShowString(2, 5, ns_indicator_str);  // ��ʾ N/S ָʾ��
    OLED_ShowString(2, 7, gps_data.longitude);
    OLED_ShowString(2, 12, ew_indicator_str);  // ��ʾ E/W ָʾ��
	
	
	
	
}
