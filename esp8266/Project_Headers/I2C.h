#define I2C_DisableAck()       I2C1_C1 |= I2C_C1_TXAK_MASK
#define I2C_EnableAck()        I2C1_C1 &= ~I2C_C1_TXAK_MASK
#define I2C_RepeatedStart()    I2C1_C1 |= I2C_C1_RSTA_MASK
#define I2C_EnterRxMode()      I2C1_C1 &= ~I2C_C1_TX_MASK
#define I2C_write_byte(data)   I2C1_D = data

#define I2C_Start()            I2C1_C1 |= I2C_C1_TX_MASK;\
                               I2C1_C1 |= I2C_C1_MST_MASK

#define I2C_Stop()             I2C1_C1 &= ~I2C_C1_MST_MASK;\
                               I2C1_C1 &= ~I2C_C1_TX_MASK

#define I2C_Wait()             while((I2C1_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C1_S |= I2C_S_IICIF_MASK;

unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress);
void Pause(int number);
int convertBinaryToDecimal(long long n);

