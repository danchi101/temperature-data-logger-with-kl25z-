/*
 * uart.c
 *
 *  Created on: 07-Oct-2016
 *      Author: Gaurav Gandhi
 */
#include "MKL25Z4.h"
#include "uart.h"
#include"circbuf.h"
#include"data.h"



volatile uint8_t da=0;
void UART0_IRQHandler()
{	state error_code = buff_empty(&tx_buf);
	
	if((UART0_S1 & UART0_S1_TDRE_MASK)&& error_code==buf_not_empty)
	{											// If buffer is not empty and TDRE flag is set then transmit the character

		UART0_D =  read_data(&tx_buf);
	}
	else if(error_code==buf_empty)
	{
		UART0_C2 &= ~UART0_C2_TIE_MASK;			// If tx buffer is empty then disable the tx interrupt
	}

	error_code = buff_full(&rx_buf);
	
	if((UART0_S1 & UART0_S1_RDRF_MASK) && error_code==buf_not_full) 
	{													
														// If buffer is not and RDRF flag is set then recieve the character from UARTD0
		uint8_t mode;
		uint32_t i;
		da= UART0_D;
		add_data(&rx_buf,da);


	}

	else if(error_code==buf_empty)	
		{
			UART0_C2 &= ~UART0_C2_RIE_MASK;  		// If rx buffer is full then disable the rx interrupt 
		}



}




