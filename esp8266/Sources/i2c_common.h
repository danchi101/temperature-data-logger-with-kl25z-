/*
 * i2c_common.h
 *
 *  Created on: Jan 30, 2017
 *      Author: Danchi
 */

#ifndef I2C_COMMON_H_
#define I2C_COMMON_H_

#define i2c_DisableAck()       I2C1_C1 |= I2C_C1_TXAK_MASK

#define i2c_RepeatedStart()    I2C1_C1     |= 0x04;

#define i2c_Start()            I2C1_C1     |= 0x10;\
                               I2C1_C1     |= I2C_C1_MST_MASK

#define i2c_Stop()             I2C1_C1  &= ~I2C_C1_MST_MASK;\
                               I2C1_C1  &= ~I2C_C1_TX_MASK

#define i2c_EnterRxMode()      I2C1_C1   &= ~I2C_C1_TX_MASK;\
                               I2C1_C1   &= ~I2C_C1_TXAK_MASK

#define i2c_Wait()             while((I2C1_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C1_S |= I2C_S_IICIF_MASK;

#define i2c_write_byte(data)   I2C1_D = data


void Init_I2C1(void);

#endif /* I2C_COMMON_H_ */
