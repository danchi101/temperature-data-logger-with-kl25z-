/*
 * i2c_common.c
 *
 *  Created on: Jan 30, 2017
 *      Author: Danchi
 */
#include "derivative.h"
#include "i2c_common.h"
#include "I2C.h"
#include <stdio.h>
#include <math.h>

int convertBinaryToDecimal(long long n);
void Init_I2C1(void)
{
    SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK; //Turn on clock to I2C1 module
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;//enable C ports
    /* Configure GPIO for I2C1 function */
    PORTC_PCR11 = PORT_PCR_MUX(2);// IC1 SDA
    PORTC_PCR10 = PORT_PCR_MUX(2);//IC1 SCL

    I2C1_F  |= I2C_F_ICR(0x14);       /* set MULT and ICR */

    I2C1_C1 = I2C_C1_IICEN_MASK;       /* enable IIC */
}


unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress)
{
	unsigned char result;
	  
	I2C_Start();	          
	I2C1_D = u8SlaveAddress << 0;	// Send I2C device address with W/R bit = 0
	I2C_Wait();										

	I2C1_D = u8RegisterAddress;		// Send register address 
	I2C_Wait();

	I2C_RepeatedStart();
	
	I2C1_D = (u8SlaveAddress << 1) | 0x01;// Send I2C device address this time with W/R bit 
	I2C_Wait();	

	I2C_EnterRxMode();
	I2C_DisableAck();

	result = I2C1_D;												
	I2C_Wait();
	I2C_Stop();  
	result = I2C1_D;
	Pause(50);
	return result;
}

void Pause(int number)
{
	int cnt;
	for(cnt=0; cnt<number; cnt++)
	{
		asm("nop");
	};
}


