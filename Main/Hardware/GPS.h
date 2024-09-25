#ifndef __GPS_H
#define __GPS_H

#define GPS_BUFFER_SIZE 256

extern uint8_t gpsBuffer[GPS_BUFFER_SIZE];
extern uint16_t gpsDataIndex;

typedef struct {
    char utc_time[11];   // UTC时间:hhmmss.ss
    char status;         // 状态:A=有效,V=无效
    char latitude[10];   // 纬度:ddmm.mmmm
    char ns_indicator;   // N/S指示符:N=北纬,S=南纬
    char longitude[11];  // 经度:dddmm.mmmm
    char ew_indicator;   // E/W指示符:E=东经,W=西经
    char speed[6];       // 速度:单位节
    char course[6];      // 航向:单位度
    char date[7];        // 日期:ddmmyy
} GPRMC_Data;

extern GPRMC_Data gps_data;



void GPS_Init(void);


void GPS_Process(void);


int Parse_GPRMC(char *nmea_sentence, GPRMC_Data *gps_data);


void GPS_ParseNMEA(void);


void GPS_SaveToFlash(void);


void GPS_UpdateOLED(void);



#endif
