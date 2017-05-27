/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include "ConsoleIO.h"
#include <mcg.h>
#include <Systick.h>
#include"circbuf.h"
#include"data.h"
#include"log.h"
#include"memory.h"
#include"main.h"
#include "uart.h"
#include "profile.h"
#include "String.h"
#include "I2C.h"

#define RED_LED (1<<18)
#define GREEN_LED (1<<19)
#define BLUE_LED (1<<1)

uint8_t uart_getchar ();
void put(char *ptr_str);
void uart_putchar (char ch);
char *data1,str1[100],str2[100],y[10],*data2,index;
int s,e;
int main(void)
{
	pll_init(8000000,LOW_POWER,CRYSTAL,4,24,MCGOUT);
	uint16_t ubd;					/*Variable to save the baud rate*/
	uint8_t temp;
	
	uint8_t temp_in_binary;
	
	//Clock gating
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      /*Enable the UART clock*/
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); //Select  48MHz clock
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;	/*Enable the PORTA clock*/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTD_MASK;
	
	//Set the function of the pins
	PORTD_PCR6 |= PORT_PCR_MUX(3); //Set Rx pin for the UART
	PORTD_PCR7 |= PORT_PCR_MUX(3); //Set Tx pin for the UART
	PORTB_PCR19 =(PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);
	PORTB_PCR18 = (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);
	PORTD_PCR1 = (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK);
	
	GPIOB_PSOR |= RED_LED;
					   		
	GPIOB_PSOR |= GREEN_LED;
						
	GPIOD_PSOR |= BLUE_LED;
	
	 //Set the GPIO pins as output
	GPIOB_PDDR = RED_LED|GREEN_LED;
	GPIOD_PDDR = BLUE_LED;
			
	UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );  /*Disable Tx and Rx*/
	UART0_C1 = 0; /*Default settings of the register*/
	
	ubd = (uint16_t)((48000*1000)/(38400 * 17));/* Calculate baud settings */
    
	temp = UART0_BDH & ~(UART_BDH_SBR(0x1F));/*Save the value of UART1_BDH except SBR*/
	UART0_BDH = temp | (((ubd & 0x1F00) >> 8));
	UART0_BDL = (uint8_t)(ubd & UART_BDL_SBR_MASK);
	
	//Set NVIC interrupts and CPU interrupts
	//NVIC_EnableIRQ(UART0_IRQn);
	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK; //Setup receiver interrupt
						

	UART0_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );    /* Enable receiver and transmitter */

	
	//Set all LEDs off
	GPIOB_PSOR |= RED_LED;
			   		
	GPIOB_PSOR |= GREEN_LED;
			   		
	GPIOD_PSOR |= BLUE_LED;

	
	Init_I2C1();
	Init_Systick();
	ConsoleIO_Init();
	//__enable_irq();
	int temperature;//variable to store temperature
	char update;
	
	
	
	while(1){
		temperature= I2C_ReadRegister(0x48,0x00);
		
		
		puts("AT\r");
					delay_ms(500);
					
					//reset the module to a known state
					puts("AT+RST\r");
					delay_ms(3000);
					
					//join wifi network
					puts("AT+CWJAP=\"kandagor\",\"Kiptoon@123\"");
					delay_ms(4000);
					
					//set module operation to workstation
					puts("AT+CWMODE_CUR=1\r");
					delay_ms(500);
					
					//set module to accept multiple log ins
					puts("AT+CIPMUX=1\r");
					delay_ms(1000);
					
					//start a TCP connetion to the thingspeak website 
					//set module to listen to port 80
					puts("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r");
					delay_ms(2000);
					
					
					//prepare module to expect 75 characters
					puts("AT+CIPSEND=4,78\r");
					delay_ms(1000);
					
					update="GET https://api.thingspeak.com/update?api_key=DK8PGER1CBBKJF65&field1=%i",temperature;
					
					//puts(update);
					//delay_ms(1000);
					puts("GET https://api.thingspeak.com/update?api_key=DK8PGER1CBBKJF65&field1=27");
					delay_ms(1000);
					
					//puts("\"GET https://api.thingspeak.com/update?api_key=DK8PGER1CBBKJF65&field1=%d\",temperature\r");
					//delay_ms(1000);
					
					puts("AT+CIPCLOSE\r");
					delay_ms(10000);
					
					
		
		
		
		
		

		
	}
	return 0;
}

void put(char *ptr_str){
	while(*ptr_str)
	uart_putchar(*ptr_str++);
}
void uart_putchar(char ch){
	//Wait until space is available in the FIFO
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	//Send the character
	UART0_D = (uint8_t)ch;
}
uint8_t uart_getchar(){
	//Wait until character has been received
	while(!(UART0_S1 & UART_S1_RDRF_MASK));
	//Return the 8-bit data from the receiver
	return UART0_D;
}
char * string_receive1(){
		
		unsigned char  x,i = 0;
		//receive the characters until "," character comes
		while((x = uart_getchar())!=','){
			
			//and store the received characters into the array string[] one-by-one
			str2[i++] = x;
		}
		//insert NULL to terminate the string
		str2[i] = '\0';
		//return the received string
		return str2;
	}
char *  string_receive2()
{
	unsigned char  x,i = 0;
	//receive the characters until "K" character comes
	while((x = uart_getchar())!='K')
	{
		//and store the received characters into the array string[] one-by-one
		str2[i++] = x;
	}
	//insert NULL to terminate the string
	str2[i] = '\0';
	//return the received string
	return str2;
}
char *compare(){
			int i = 0;
			// s = start of new string
			// e = end of new string
			while(s<e ){
				//and store the received characters into the array string[] one-by-one
				y[i++] = data1[s++];
			}
			//insert NULL to terminate the string
			y[i] = '\0';
			//return the received string
		return (y);
}
