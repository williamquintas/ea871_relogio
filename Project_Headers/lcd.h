/* William Quintas de Melo - RA 188684 */
#ifndef LCD_H_
#define LCD_H_

void init_GPIO_LCD(void);
void init_LCD(void);
void clear_LCD(void);
void putchar_LCD(unsigned char);
void putcmd_LCD(unsigned char, unsigned int); 
void puts_LCD(unsigned char *);  
void setpos_LCD(unsigned int, unsigned int);

#endif /* LCD_H_ */
