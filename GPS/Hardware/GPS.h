#ifndef __GPS_H
#define __GPS_H

#define GPS_BUFFER_SIZE 256

extern uint8_t gpsBuffer[GPS_BUFFER_SIZE];
extern uint16_t gpsDataIndex;

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

extern GPRMC_Data gps_data;



void GPS_Init(void);


void GPS_Process(void);


int Parse_GPRMC(char *nmea_sentence, GPRMC_Data *gps_data);


void GPS_ParseNMEA(void);


void GPS_SaveToFlash(void);


void GPS_UpdateOLED(void);



#endif
