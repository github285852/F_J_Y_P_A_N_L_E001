#ifndef __DMXPROTOCOL_H
#define __DMXPROTOCOL_H




#define DMX_M1					0x01
#define DMX_M2					0x02
#define DMX_M3					0x03
#define DMX_M4					0x04
#define DMX_M5					0x05
#define DMX_M6					0x06
#define DMX_M7					0x07
#define DMX_M8					0x08
#define DMX_M9					0x09
#define DMX_M10					0x0A
#define DMX_M11					0x0B
#define DMX_M12					0x0C
#define DMX_M13					0x0D
#define DMX_M14					0x0E
#define DMX_M15					0x0F



typedef struct
{
	unsigned char dim;
	unsigned char kvn;
	unsigned char grn;
	unsigned char w_c; //在坐标上的权重
	unsigned char r;
  unsigned char g;
	unsigned char b;
	unsigned char ww;
	unsigned char cw;
	unsigned char reserved[3];
}CCT_RGBWWCW_8BIT;



typedef struct
{
	unsigned char dim;
	unsigned char kvn;
	unsigned char grn;
	unsigned char reserved[4];
}CCT_8BIT;


typedef struct
{
	unsigned char dim;
	unsigned char kvn;
	unsigned char grn;
	unsigned char w_c; //在坐标上的权重
	unsigned char h;
  unsigned char s;
	unsigned char reserved[4];
}CCT_HSI_8BIT;


typedef struct
{
	unsigned char dim;
	unsigned char r;
	unsigned char g;
	unsigned char b; 
	unsigned char ww;
  unsigned char cw;
	unsigned char reserved[3];
}RGBWWCW_8BIT;


typedef struct
{
	unsigned char dim;
	unsigned char h;
	unsigned char s;
	unsigned char reserved[4];
}HSI_8BIT;


typedef struct
{
	uint16_t dim;
	uint16_t kvn;
	uint16_t grn;
	uint16_t w_c; //在坐标上的权重
	uint16_t r;
  uint16_t g;
	uint16_t b;
	uint16_t ww;
	uint16_t cw;
}CCT_RGBWWCW_16BIT;


typedef struct
{
	uint16_t dim;
	uint16_t kvn;
	uint16_t grn;
	unsigned char  reserved[4];
}CCT_16BIT;


typedef struct
{
	uint16_t dim;
	uint16_t kvn;
	uint16_t grn;
	uint16_t w_c; //在坐标上的权重
	uint16_t h;
  uint16_t s;
	unsigned char  reserved[4];
}CCT_HSI_16BIT;


typedef struct
{
	uint16_t dim;
	uint16_t r;
	uint16_t g;
	uint16_t b; 
	uint16_t ww;
  uint16_t cw;
	unsigned char  reserved[2];
}RGBWWCW_16BIT;


typedef struct
{
	uint16_t dim;
	uint16_t h;
	uint16_t s;
	unsigned char  reserved[4];
}HSI_16BIT;

typedef struct
{
	unsigned char coarse;//粗调
	unsigned char fine;//微调
}Coarse_Fine;

typedef struct
{
	Coarse_Fine dim; 
	Coarse_Fine kvn;
	unsigned char grn;
	unsigned char w_c; //在坐标上的权重
	Coarse_Fine r;
  Coarse_Fine g;
	Coarse_Fine b;
	Coarse_Fine ww;
	Coarse_Fine cw;
	unsigned char reserved[2];
}CCT_RGBWWCW_CF;



typedef struct
{
	Coarse_Fine dim;
	Coarse_Fine kvn;
	unsigned char grn;
	unsigned char reserved[4];
}CCT_CF;


typedef struct
{
	Coarse_Fine dim;
	Coarse_Fine kvn;
	unsigned char grn;
	unsigned char w_c; //在坐标上的权重
	Coarse_Fine h;
  Coarse_Fine s;
	unsigned char reserved[4];
}CCT_HSI_CF;


typedef struct
{
	Coarse_Fine dim;
	Coarse_Fine r;
	Coarse_Fine g;
	Coarse_Fine b; 
	Coarse_Fine ww;
  Coarse_Fine cw;
	unsigned char reserved[2];
}RGBWWCW_CF;


typedef struct
{
	Coarse_Fine dim;
	Coarse_Fine h;
	Coarse_Fine s;
	unsigned char reserved[4];
}HSI_CF;


typedef union 
{
	CCT_RGBWWCW_8BIT cct_rgbwwcw_8bit;
	CCT_8BIT	cct_8bit;
	CCT_HSI_8BIT cct_hsi_8bit;
  RGBWWCW_8BIT rgbwwcw_8bit;
	HSI_8BIT hsi_8bit;
	CCT_RGBWWCW_16BIT cct_rgbwwcw_16bit;
	CCT_16BIT cct_16bit;
	CCT_HSI_16BIT cct_hsi_16bit;
	RGBWWCW_16BIT rgbwwcw_16bit;
	HSI_16BIT hsi_16bit;
	CCT_RGBWWCW_CF cct_rgbwwcw_cf;
	CCT_CF cct_CF;
	CCT_HSI_CF cct_hsi_cf;
	RGBWWCW_CF rgbwwcw_cf;
	HSI_CF hsi_cf;
}DMXData;



void DMXTask(void);
void receiving_dmx_data(void);


#endif


