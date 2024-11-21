#include "software_i2c.h"

/*使用GPIO引脚选择*/
#define USE_GPIOx MPU_SDA_GPIO_Port
#define USE_SCL_PIN MPU_SCL_Pin
#define USE_SDA_PIN MPU_SDA_Pin

#ifdef USE_STDPERIPH_DRIVER
    /*引脚配置*/
    #define W_SCL(x)		GPIO_WriteBit(USE_GPIOx, USE_SCL_PIN, (BitAction)(x))
    #define W_SDA(x)		GPIO_WriteBit(USE_GPIOx, USE_SDA_PIN, (BitAction)(x))
    #define R_SDA()			GPIO_ReadInputDataBit(USE_GPIOx, USE_SDA_PIN)

    /**
     * @brief 引脚初始化
     */
    void I2C_GPIO_Init(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIO_InitStructure.GPIO_Pin = USE_SCL_PIN | USE_SDA_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(USE_GPIOx, &GPIO_InitStructure);
        W_SCL(1);
        W_SDA(1);
    }
#endif

#ifdef USE_HAL_DRIVER
    /*引脚配置*/
    #define W_SCL(x)		HAL_GPIO_WritePin(USE_GPIOx, USE_SCL_PIN, (GPIO_PinState)(x))
    #define W_SDA(x)		HAL_GPIO_WritePin(USE_GPIOx, USE_SDA_PIN, (GPIO_PinState)(x))
    #define R_SDA()			HAL_GPIO_ReadPin(USE_GPIOx, USE_SDA_PIN)
#endif

/*I2C基本时序单元*/

/**
 * @brief I2C起始条件
 * @param 无
 * @retval 无
 */
void I2C_Start(void)
{
    W_SDA(1);
    W_SCL(1);
    W_SDA(0);
    W_SCL(0);
}

/**
 * @brief I2C停止条件
 * @param 无
 * @retval 无
 */
void I2C_Stop(void)
{
    W_SDA(0);
    W_SCL(1);
    W_SDA(1);
}

/**
 * @brief I2C发送一个字节
 * @param Byte 要发送的一个字节
 * @retval 无
 */
void I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        W_SDA(Byte & (0x80 >> i));  //使用掩码从高位到低位发送
        W_SCL(1);
        W_SCL(0);
    }
}

/**
 * @brief I2C接收一个字节
 * @param 无
 * @retval 接收到的字节
 */
uint8_t I2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0;
    W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        W_SCL(1);
        Byte |= R_SDA() << (7 - i);  //将接收到的数据从高位到低位存入Byte
        W_SCL(0);
    }
    return Byte;
}

/**
 * @brief I2C发送应答
 * @param Ack 应答位
 * @retval 无
 */
void I2C_SendAck(uint8_t Ack)
{
    W_SDA(Ack);
    W_SCL(1);
    W_SCL(0);
}

/**
 * @brief I2C接收应答
 * @param 无
 * @retval 应答位
 */
uint8_t I2C_ReceiveAck(void)
{
    uint8_t Ack;
    W_SDA(1);
    W_SCL(1);
    Ack = R_SDA();
    W_SCL(0);
    return Ack;
}

/*I2C操作*/

/**
 * @brief I2C指定地址连续写
 * @param DevAddress 从机地址(左对齐)
 * @param MemAddress 寄存器地址
 * @param pData 要写入的数据
 * @param Size 要写入的数据长度
 * @retval 0：成功 非0：失败
 */
uint8_t SoftW_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t Status = 0;
    I2C_Start();
    I2C_SendByte(DevAddress);  //发送从机地址与写入位
    if (I2C_ReceiveAck())
    {
        Status = 1;
    }
    I2C_SendByte(MemAddress);   //发送寄存器地址
    if (I2C_ReceiveAck())
    {
        Status = 2;
    }
    while (Size--)  //顺次发送数据
    {
        I2C_SendByte(*pData++);
        if (I2C_ReceiveAck())
        {
            Status = 3;
            break;
        }
    }
    I2C_Stop();
    return Status;
}

/**
 * @brief I2C指定地址连续读
 * @param DevAddress 从机地址(左对齐)
 * @param MemAddress 寄存器地址
 * @param pData 读取的数据
 * @param Size 读取的数据长度
 * @retval 0：成功 非0：失败
 */
uint8_t SoftW_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t Status = 0;
    I2C_Start();    //写时序指定写入寄存器地址
    I2C_SendByte(DevAddress);  //发送从机地址与写入位
    if (I2C_ReceiveAck())
    {
        Status = 1;
    }
    I2C_SendByte(MemAddress);   //发送寄存器地址设置从机当前地址寄存器值
    if (I2C_ReceiveAck())
    {
        Status = 2;
    }
    I2C_Start(); //重复起始条件转为读时序
    I2C_SendByte(DevAddress | 1);    //发送从机地址与读取位
    if (I2C_ReceiveAck())
    {
        Status = 3;
    }
    while (Size)
    {
        *pData++ = I2C_ReceiveByte();   //pData先赋值再自增
        if (--Size) //Size先自减再判断
        {
            I2C_SendAck(0);
        }
        else
        {
            I2C_SendAck(1); //最后一个数据不应答
        }
    }
    I2C_Stop();
    return Status;
}

/**
 * @brief I2C主机发送数据
 * @param DevAddress 从机地址(左对齐)
 * @param pData 要发送的数据
 * @param Size 要发送的数据长度
 * @retval 0：成功 非0：失败
 */
uint8_t SoftW_I2C_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t Status = 0;
    I2C_Start();
    I2C_SendByte(DevAddress);  //发送从机地址与写入位
    if (I2C_ReceiveAck())
    {
        Status = 1;
    }
    while (Size--)  //顺次发送数据
    {
        I2C_SendByte(*pData++);
        if (I2C_ReceiveAck())
        {
            Status = 2;
            break;
        }
    }
    I2C_Stop();
    return Status;
}

/**
 * @brief I2C主机接收数据
 * @param DevAddress 从机地址(左对齐)
 * @param pData 接收的数据
 * @param Size 接收的数据长度
 * @retval 0：成功 非0：失败
 */
uint8_t SoftW_I2C_Master_Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t Status = 0;
    I2C_Start();
    I2C_SendByte(DevAddress | 1);    //发送从机地址与读取位
    if (I2C_ReceiveAck())
    {
        Status = 1;
    }
    while (Size)    //接收数据
    {
        *pData++ = I2C_ReceiveByte();
        if (--Size)
        {
            I2C_SendAck(0);
        }
        else
        {
            I2C_SendAck(1);
        }
    }
    I2C_Stop();
    return Status;
}
