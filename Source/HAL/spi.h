#ifndef __SPI_H
#define __SPI_H


void SPI3_init(void);
u8 SPI3_ReadWriteByte(u8 TxData);
u8 SPI3_write_bytes(unsigned char *addr,u16 num);
u8 SPI3_send_same_half_word(uint32_t *addr,u16 num);
u8 SPI3_write_half_words(uint32_t *addr,u16 num);


extern unsigned char DMAING;
#endif



