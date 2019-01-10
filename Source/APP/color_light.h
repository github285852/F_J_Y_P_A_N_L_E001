#ifndef __COLOR_LIGHT_H
#define __COLOR_LIGHT_H

#include "mymath.h"

#define PI	3.141592653589793
//标准的RGB色坐标

////LED发出光的色坐标


#define RX 	LEDRX_DEFAULT	// 0.7
#define RY 	LEDRY_DEFAULT	// 0.28
#define GX	LEDGX_DEFAULT//0.08
#define GY	LEDGY_DEFAULT//0.83
#define BX	LEDBX_DEFAULT	//0.1
#define BY	LEDBY_DEFAULT//0.1


typedef struct{
  unsigned short h;
	float s;
	float i;
}HSI;

#ifdef SPOT

typedef struct{
	float r;
	float a;
	float l;
	float g;
	float b;
	float y;
	float w;
	float ww;
	float cw;
}RGB,LEDK;

#elif PANLE

typedef struct{
	float r;
	float ww;
	float g;
	float cw;
	float b;
	float a;
	float l;
	float y;
	float w;
}RGB,LEDK;

#endif

typedef struct{
	unsigned int *r;
	unsigned int *g;
	unsigned int *b;
	unsigned int *y;
	unsigned int *w;
	unsigned int *ww;
	unsigned int *cw;
}pRGBPwm;

typedef struct
{
	int grn;
	unsigned char kvn; //色温
	float dim;
	int pos;
	int max_pos;
}CCT;

typedef struct
{
	char num;
	COORD centre;
	COORD offset;
	float ra_k;
	float lg_k;
}CCT_TAB;

typedef struct
{
	unsigned char source;//光源
	unsigned char number;//色号
	float dim;
}GEL;

typedef struct 
{
	unsigned char *NAME[2];
	COORD coord[2];
}GEL_COORD;

void LEDK_to_coordinate(coord_f *ledc,float *ledk,coord_f *coord);
void HSI_to_RGB(HSI HSI,RGB *rgbk);
void ColorLightHSIOut(HSI Hsi,unsigned char pixel);
void LightCCTOut( unsigned char pos,int offset,float dim,unsigned char pixel);
void LightRGBOut(unsigned int R,unsigned int G,unsigned int B,unsigned char pixel);
void RGBWWCW_to_coordinate(RGB rgb,COORD *coord);
int CCTToCoordinate(unsigned char pos,int offset,COORD *target);
int HSIToCoordinate(HSI *Hsi,COORD *coord);
int CoordinateOut(COORD *coord,float dim,unsigned char pixel);
int LightGELOut(GEL *gel,unsigned char pixel);
void color_light_init(void);
void ColorTest(void);
void HSI_Test(void);
void RGB_to_coordinate(RGB rgb,COORD *coord);
unsigned char coordinate_to_RGBWWCW(COORD coord,RGB *rgb);
int coordinate_RGBLA(coord_f exp_point,LEDK *ledk);
extern unsigned int RGBPwm[6];
extern HSI Hsi;
extern const CCT_TAB cct_tab[];
extern const GEL_COORD GEL_TAB[];

#endif



