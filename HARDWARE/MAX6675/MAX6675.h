#ifndef _MAX6675_H
#define _MAX6675_H
#define 	MAX6675_CS	 		GPIO_Pin_4
#define 	MAX6675_CSL()		GPIOA->BRR = MAX6675_CS;
#define 	MAX6675_CSH()		GPIOA->BSRR = MAX6675_CS;
void SPI_MAX6675_Init(void);
unsigned char MAX6675_ReadByte(void);
#endif
