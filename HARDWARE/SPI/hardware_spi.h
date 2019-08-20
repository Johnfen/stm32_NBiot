#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

// 备注 ：程序中管脚时钟没有宏定义

#define SPI                           SPI1
#define SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_CS_PIN                    GPIO_Pin_4                  /* PA.04 */
#define SPI_CS_GPIO_PORT              GPIOA                       /* GPIOA */
#define SPI_CS_GPIO_CLK               RCC_APB2Periph_GPIOA        

#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(SPI_CS_GPIO_PORT, SPI_CS_PIN)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(SPI_CS_GPIO_PORT, SPI_CS_PIN)

void hardWareSPIInit(void);


u8 SPI_SendByte(u8 byte);


#endif /* __SPI_FLASH_H */

