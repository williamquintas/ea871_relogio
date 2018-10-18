/* William Quintas de Melo - RA 188684 */
#include "delay.h"
#include "lcd.h"
#include "uart.h"
#include "derivative.h"

int main(void) {
	init_UART0();
	init_GPIO_LCD();
	init_LCD();
	unsigned char c = 0, cs = 0, s = 0, m = 59, h = 0, count = 1, displayInTerminal = 1, displayInList = 0;
	char lcdString[50];
	while (c == 0) {
		if (count == 1) {
			cs++;
			if (cs == 60) {
				cs = 0;
				s++;
				if (s == 60) {
					s = 0;
					m++;
					if (m == 60) {
						m = 0;
						h++;
					}
				}
			}
			clear_LCD();
			sprintf(lcdString, "%02d:%02d:%02d:%02d", (int)h, (int)m, (int)s, (int)cs);
			puts_LCD(lcdString);
			if (displayInTerminal == 1) {
				if (displayInList == 0) {
					putchar_UART0(27);
					putchar_UART0('[');
					putchar_UART0('H');
					puts_UART0(lcdString);
				} else {
					puts_UART0("\r\n");
					puts_UART0(lcdString);
				}
			}
		}
		c = getchar_UART0();
		if (c == 's') {
			if (count == 1) {
				count = 0;
			} else {
				count = 1;
			}
			c = 0;
		} else if (c == 'd') {
			if (displayInTerminal == 1) {
				displayInTerminal = 0;
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('H');
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('2');
				putchar_UART0('J');
			} else {
				displayInTerminal = 1;
			}
			c = 0;
		} else if (c == 'r') {
			h = 0;
			m = 0;
			s = 0;
			cs = 0;
			clear_LCD();
			sprintf(lcdString, "%02d:%02d:%02d:%02d", (int)h, (int)m, (int)s, (int)cs);
			puts_LCD(lcdString);
			if (displayInTerminal == 1) {
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('H');
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('2');
				putchar_UART0('J');
				puts_UART0(lcdString);
			}
			c = 0;
		} else if (c == 'l') {
			if (displayInList == 1) {
				displayInList = 0;
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('H');
				putchar_UART0(27);
				putchar_UART0('[');
				putchar_UART0('2');
				putchar_UART0('J');
			} else {
				displayInList = 1;
			}
			c = 0;
		} else if (c == 27) {
			break;
		} else {
			c = 0;
		}
		delay(7000);
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
