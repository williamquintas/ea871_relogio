/* William Quintas de Melo - RA 188684 */
#include "delay.h"
#include "lcd.h"
#include "uart.h"
#include "derivative.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))

/**************************************
******     VARIAVEIS GLOBAIS     ******
****************************************/
unsigned char centsecond_char0, centsecond_char1, second_char0, second_char1, minute_char0, minute_char1, hour_char0, hour_char1;
unsigned char second_flag, minute_flag, hour_flag, displayInList, displayInTerminal, timesStoped = 0, command = 0;

/**************************************
***     Declaracao de funcoes     *****
****************************************/
increment_centisecond(char *digito0, char *digito1, char *second_flag) {
	(*digito1)++;
	if (*digito1 >= '9' && *digito0 >= '9') {
		(*digito0) = '0';
		(*digito1) = '0';
		(*second_flag) = 1;
	}
	else if (*digito1 > '9') {
		(*digito1) = '0';
		(*digito0)++;
		(*second_flag) = 0;
	}
	else {
		(*second_flag) = 0;
	}
}

increment_second(char *digito0, char *digito1, char *minute_flag) {
	(*digito1)++;
	if (*digito1 > '9' && *digito0 >= '5') {
		(*digito0) = '0';
		(*digito1) = '0';
		(*minute_flag) = 1;
	}
	else if (*digito1 > '9') {
		(*digito1) = '0';
		(*digito0)++;
		(*minute_flag) = 0;
	}
	else {
		(*minute_flag) = 0;
	}
}

increment_minute(char *digito0, char *digito1, char *hour_flag) {
	(*digito1)++;
	if (*digito1 > '9' && *digito0 >= '5') {
		(*digito0) = '0';
		(*digito1) = '0';
		(*hour_flag) = 1;
	}
	else if (*digito1 > '9') {
		(*digito1) = '0';
		(*digito0)++;
		(*hour_flag) = 0;
	}
	else {
		(*hour_flag) = 0;
	}
}

increment_hour(char *digito0, char *digito1) {
	(*digito1)++;
	if (*digito1 >= '9' && *digito0 >= '9') {
		(*digito0) = '0';
		(*digito1) = '0';
	}
	else if (*digito1 > '9') {
		(*digito1) = '0';
		(*digito0)++;
	}
}

printInTerminal(unsigned char centsecond_char0, unsigned char centsecond_char1, unsigned char second_char0, unsigned char second_char1, unsigned char minute_char0, unsigned char minute_char1, unsigned char hour_char0, unsigned char hour_char1, unsigned char displayInList) {
	if (displayInList == 0) {
		//Colocando o cursor no local certo
		putchar_UART0(27);
		putchar_UART0('[');
		putchar_UART0('H');
		//Limpando a tela do terminal
		putchar_UART0(27);
		putchar_UART0('[');
		putchar_UART0('2');
		putchar_UART0('J');

		putchar_UART0(hour_char0);
		putchar_UART0(hour_char1);
		putchar_UART0(':');
		putchar_UART0(minute_char0);
		putchar_UART0(minute_char1);
		putchar_UART0(':');
		putchar_UART0(second_char0);
		putchar_UART0(second_char1);
		putchar_UART0(':');
		putchar_UART0(centsecond_char0);
		putchar_UART0(centsecond_char1);
	}
	else {
		putchar_UART0('\n');
		putchar_UART0('\r');
		putchar_UART0(hour_char0);
		putchar_UART0(hour_char1);
		putchar_UART0(':');
		putchar_UART0(minute_char0);
		putchar_UART0(minute_char1);
		putchar_UART0(':');
		putchar_UART0(second_char0);
		putchar_UART0(second_char1);
		putchar_UART0(':');
		putchar_UART0(centsecond_char0);
		putchar_UART0(centsecond_char1);
	}
}

printInLCD(unsigned char centsecond_char0, unsigned char centsecond_char1, unsigned char second_char0, unsigned char second_char1, unsigned char minute_char0, unsigned char minute_char1, unsigned char hour_char0, unsigned char hour_char1) {
	setpos_LCD(1, 0);
	putchar_LCD(hour_char0);
	putchar_LCD(hour_char1);
	putchar_LCD(':');
	putchar_LCD(minute_char0);
	putchar_LCD(minute_char1);
	putchar_LCD(':');
	putchar_LCD(second_char0);
	putchar_LCD(second_char1);
	putchar_LCD(':');
	putchar_LCD(centsecond_char0);
	putchar_LCD(centsecond_char1);
}

init_SysTick(){
	SYST_CSR = 0x7;
	SYST_CVR = 0;
	SYST_RVR = 0x33333;
}

SysTick_Handler(){
	/* Incrementa contador */
	increment_centisecond(&centsecond_char0, &centsecond_char1, &second_flag);
	if(second_flag==1) {
		increment_second(&second_char0, &second_char1, &minute_flag);
		second_flag = 0;
	}
	if(minute_flag==1) {
		increment_minute(&minute_char0, &minute_char1, &hour_flag);
		minute_flag = 0;
	}
	if(hour_flag==1) {
		increment_hour(&hour_char0, &hour_char1);
		hour_flag = 0;
	}
}

 init_PTA12(){
 	SET_BIT(SIM_SCGC5, 9);
	PORTA_PCR12 &= 0xFEF0F8FF;
 	SET_BIT(PORTA_PCR12, 8);
 	SET_BIT(PORTA_PCR12, 16);
 	SET_BIT(PORTA_PCR12, 19);
 	SET_BIT(NVIC_IPR7, 22);
 	SET_BIT(NVIC_IPR7, 23);
 	SET_BIT(NVIC_ISER, 30);
 }

 PORTA_IRQHandler(){
 	if(timesStoped%2 == 0) {
 		SYST_CSR = SYST_CSR & (0xFFFFFFFD);
 	}
 	else {
 		SET_BIT(SYST_CSR, 1);
 	}
	SET_BIT(PORTA_PCR12, 24);
 	timesStoped++;
 }

init_PTA4(){
	SET_BIT(SIM_SCGC5, 9);
	PORTA_PCR4 &= 0xFFFFF8FF;
	PORTA_PCR4 |= 0x00000700;
}

NMI_Handler(){
	centsecond_char0 = '0';
	centsecond_char1 = '0';
	second_char0 = '0';
	second_char1 = '0';
	minute_char0 = '0';
	minute_char1 = '0';
	hour_char0 = '0';
	hour_char1 = '0';
}

UART0_IRQHandler(){
	/* Recebe o proximo comando */
	command = getchar_UART0();
	//Tratando comandos
	if(command == 'd' || command == 'D'){
		if(displayInTerminal == 1)
			displayInTerminal = 0;
		else if(displayInTerminal == 0)
			displayInTerminal = 1;
	} else if(command == 'l' || command == 'L'){
		if(displayInList == 1)
			displayInList = 0;
		else if(displayInList == 0)
			displayInList = 1;
	} 
}
 
/**************************************
**********        MAIN       **********
****************************************/
int main(void) {
	init_SysTick();
	init_PTA12();
	init_PTA4();
	init_UART0();
	init_GPIO_LCD();
	init_LCD();
	clear_LCD();
	//Colocando o cursor no local certo
	putchar_UART0(27);
	putchar_UART0(91);
	putchar_UART0(72);
	//Limpando a tela do terminal
	putchar_UART0(27);
	putchar_UART0(91);
	putchar_UART0(50);
	putchar_UART0(74);
	//Inicializando variaveis
	centsecond_char0 = '0';
	centsecond_char1 = '0';
	second_char0 = '0';
	second_char1 = '0';
	minute_char0 = '0';
	minute_char1 = '0';
	hour_char0 = '0';
	hour_char1 = '0';
	second_flag = 0;
	minute_flag = 0;
	hour_flag = 0;
	displayInTerminal = 1;
	displayInList = 0;
	while (command != 27) {
		//Imprimindo coisas
		printInLCD(centsecond_char0, centsecond_char1, second_char0, second_char1, minute_char0, minute_char1, hour_char0, hour_char1);
		if (displayInTerminal == 1) {
			printInTerminal(centsecond_char0, centsecond_char1, second_char0, second_char1, minute_char0, minute_char1, hour_char0, hour_char1, displayInList);
		}
		else {
			//Colocando o cursor no local certo
			putchar_UART0(27);
			putchar_UART0('[');
			putchar_UART0('H');
			//Limpando a tela do terminal
			putchar_UART0(27);
			putchar_UART0('[');
			putchar_UART0('2');
			putchar_UART0('J');
		}
	}
	clear_LCD();
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('H');
	putchar_UART0(27);
	putchar_UART0('[');
	putchar_UART0('2');
	putchar_UART0('J');
	return 1;
}
