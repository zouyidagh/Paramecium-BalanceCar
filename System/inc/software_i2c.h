#ifndef __SOFTWARE_I2C_H
#define __SOFTWARE_I2C_H

#ifdef USE_STDPERIPH_DRIVER
    #include "stm32f10x.h"
#endif

#ifdef USE_HAL_DRIVER
    #include "main.h"
#endif

uint8_t SoftW_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);
uint8_t SoftW_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);
uint8_t SoftW_I2C_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
uint8_t SoftW_I2C_Master_Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size);

#endif // __SOFTWARE_I2C_H