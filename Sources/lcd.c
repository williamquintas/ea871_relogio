/* William Quintas de Melo - RA 188684 */
#include "derivative.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))

void init_GPIO_LCD(void){
	SET_BIT(SIM_SCGC5, 11);		   			
	PORTC_PCR0 = PORTC_PCR0 & 0xFFFFF8FF;	
	PORTC_PCR1 = PORTC_PCR1 & 0xFFFFF8FF;
	PORTC_PCR2 = PORTC_PCR2 & 0xFFFFF8FF;
	PORTC_PCR3 = PORTC_PCR3 & 0xFFFFF8FF;
	PORTC_PCR4 = PORTC_PCR4 & 0xFFFFF8FF;
	PORTC_PCR5 = PORTC_PCR5 & 0xFFFFF8FF;
	PORTC_PCR6 = PORTC_PCR6 & 0xFFFFF8FF;
	PORTC_PCR7 = PORTC_PCR7 & 0xFFFFF8FF;
	PORTC_PCR8 = PORTC_PCR8 & 0xFFFFF8FF;
	PORTC_PCR9 = PORTC_PCR9 & 0xFFFFF8FF;
	SET_BIT(PORTC_PCR0, 8);				
	SET_BIT(PORTC_PCR1, 8);
	SET_BIT(PORTC_PCR2, 8);
	SET_BIT(PORTC_PCR3, 8);
	SET_BIT(PORTC_PCR4, 8);
	SET_BIT(PORTC_PCR5, 8);
	SET_BIT(PORTC_PCR6, 8);
	SET_BIT(PORTC_PCR7, 8);
	SET_BIT(PORTC_PCR8, 8);
	SET_BIT(PORTC_PCR9, 8);
	GPIOC_PDDR = 0x000003FF;
	GPIOC_PDOR = 0x0;
}

void putcmd_LCD(unsigned char command, unsigned int time) {
	SET_BIT(GPIOC_PCOR, 8); //RS = 0 -> COMANDO
	GPIOC_PDOR = (GPIOC_PDOR & 0xffffff00) | command;
	SET_BIT(GPIOC_PSOR, 9); //Ativa o Enable
	SET_BIT(GPIOC_PCOR, 9); //Limpa o Enable
	delay(time);
}

void clear_LCD(void) {
	putcmd_LCD(0x01, 1530);
}

void init_LCD(void) {
	putcmd_LCD(0x38, 39);
	putcmd_LCD(0x0C, 39);
	clear_LCD();
	putcmd_LCD(0x06, 3000);
}

void putchar_LCD(unsigned char caracter){
	SET_BIT(GPIOC_PSOR, 8); //RS = 1 => Dado
	GPIOC_PDOR = (GPIOC_PDOR & 0xffffff00) | caracter;
	SET_BIT(GPIOC_PSOR, 9); //Ativa o Enable
	SET_BIT(GPIOC_PCOR, 9); //Limpa o Enable
	delay(43);
}

void setpos_LCD(unsigned int linha, unsigned int coluna) {
	if(linha == 1){
		char position = 0x80;
		position |= coluna;
		putcmd_LCD(position, 40);
	}
	else {
		char position = 0xC0;
		position |= coluna;
		putcmd_LCD(position, 40);
	}
}

void puts_LCD(unsigned char *palavra){
	unsigned char cont;
	for(cont = 0; palavra[cont] != '\0'; cont++) {
		if (cont == 16) {
			setpos_LCD(2, 0);
		}
		if (cont <= 31) {
			putchar_LCD(palavra[cont]);
		}
		else {
			char primeiroSegundaLinha = palavra[cont-16];
			putcmd_LCD(0x18,1000); //shift no display inteiro
			setpos_LCD(1,cont-16);
			putchar_LCD(primeiroSegundaLinha);
			setpos_LCD(2,cont-16);
			putchar_LCD(palavra[cont]);
		}
		delay(500);
	}
}

