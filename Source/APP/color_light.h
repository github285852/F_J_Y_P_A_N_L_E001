#ifndef __COLOR_LIGHT_H
#define __COLOR_LIGHT_H

#include "mymath.h"

#define SPOT01

#define PI	3.141592653589793
//标准的RGB色坐标

////LED发出光的色坐标
#ifdef SPOT01


#else defined(PANLE01)

#define LEDCWX_DEFAULT	0.3247       //0.4438
#define LEDCWY_DEFAULT	0.3247       //3958
#define LEDWWX_DEFAULT	0.4558      //0.3069
#define LEDWWY_DEFAULT	0.4046      //0.3044
#define LEDWX_DEFAULT	  0.3278
#define LEDWY_DEFAULT		0.3332
#define LEDRX_DEFAULT		0.6873
#define LEDRY_DEFAULT		0.3114
#define LEDGX_DEFAULT		0.2082
#define LEDGY_DEFAULT		0.7313

#define LEDBX_DEFAULT		0.1409 //
#define LEDBY_DEFAULT		0.0286
#define LEDYX_DEFAULT		0.5722//Y //0.00783	//。
#define LEDYY_DEFAULT		0.4248//Y0.4733	// .

#endif

//LED发出光的色坐标
//#define LEDWWX_DEFAULT	0.4546       //0.4438
//#define LEDWWY_DEFAULT	0.4078       //3958
//#define LEDCWX_DEFAULT	0.3263      //0.3069
//#define LEDCWY_DEFAULT	0.3297      //0.3044
//#define LEDWX_DEFAULT		0.3278
//#define LEDWY_DEFAULT		0.3332
//#define LEDRX_DEFAULT		0.6881
//#define LEDRY_DEFAULT		0.3115
//#define LEDGX_DEFAULT		0.1815
//#define LEDGY_DEFAULT		0.7112
//#define LEDBX_DEFAULT		0.1412 //
//#define LEDBY_DEFAULT		0.0250
//#define LEDYX_DEFAULT		0.5722//Y //0.00783	//。
//#define LEDYY_DEFAULT		0.4248//Y0.4733	// .


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



