/* 
 * William Quintas de Melo - RA: 188684 
 * */
#include "derivative.h"
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))

void init_UART0() {
	SET_BIT(SIM_SCGC4, 10);
	SET_BIT(SIM_SCGC5, 9);
	PORTA_PCR1 &= 0xFFFFF8FF;
	SET_BIT(PORTA_PCR1, 9);
	PORTA_PCR2 &= 0xFFFFF8FF;
	SET_BIT(PORTA_PCR2, 9);
	SIM_SOPT2 &= 0xF3FFFFFF;
	SET_BIT(SIM_SOPT2, 26);
	UART0_BDH = 0x2;
	UART0_BDL = 0x22;
	UART0_C2 = 0xED;
	UART0_C2 = 0x2C;
	SET_BIT(NVIC_IPR7, 6);
	SET_BIT(NVIC_IPR7, 7);
	SET_BIT(NVIC_ISER, 12);
}

char getchar_UART0(void) {
	if (UART0_S1 & UART0_S1_RDRF_MASK)
		return UART0_D;
	return 0;
}

void putchar_UART0(char c) {
	while( !(UART0_S1 & UART0_S1_TDRE_MASK)) ;
	UART0_D = c;
}

void puts_UART0(char *palavra) {
	unsigned char cont;
	cont = 0;
	while(palavra[cont] != '\0') {
		putchar_UART0(palavra[cont]);
		cont++;
	}
}
