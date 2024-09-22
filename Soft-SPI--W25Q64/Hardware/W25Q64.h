#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);

void W25Q64_PageProgram(uint32_t Address ,uint8_t *Data_IN_Arry ,uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_Read_Data(uint32_t Address,uint8_t *Data_OUT_Arry ,uint32_t Count);



#endif
