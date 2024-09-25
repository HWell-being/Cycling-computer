#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_W_Reg(uint8_t RegAddr , uint8_t Data);
uint8_t MPU6050_R_Reg(uint8_t RegAddr );
void MPU6050_Init();
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
					 int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ);

void DisplaySlopeAngle(void);

#endif
