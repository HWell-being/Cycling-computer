#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "math.h"
#include "stdio.h"

#define MPU6050_ADDR	0xD0



void MPU6050_W_Reg(uint8_t RegAddr , uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDR);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddr);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_R_Reg(uint8_t RegAddr)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDR);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddr);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDR | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init()
{
	MyI2C_Init();
	
	MPU6050_W_Reg(MPU6050_PWR_MGMT_1,0x01);
	MPU6050_W_Reg(MPU6050_PWR_MGMT_2,0x00);
	
	MPU6050_W_Reg(MPU6050_SMPLRT_DIV	,0x09);
	MPU6050_W_Reg(MPU6050_CONFIG		,0x06);
	MPU6050_W_Reg(MPU6050_GYRO_CONFIG	,0x18);
	MPU6050_W_Reg(MPU6050_ACCEL_CONFIG	,0x18);
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_R_Reg(MPU6050_WHO_AM_I);
}


void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_Reg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_R_Reg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

void DisplaySlopeAngle(void)
{
    int16_t AccX, AccY, AccZ;
	float angle;
	extern  char angleStr[10];

    MPU6050_GetData(&AccX, &AccY, &AccZ, 0, 0, 0);
    
    angle = atan2f((float)AccX, sqrtf((float)AccY * AccY + (float)AccZ * AccZ)) * 180 / 3.14159265f;
    
    snprintf(angleStr, sizeof(angleStr), "%.2f", angle);
    
}
