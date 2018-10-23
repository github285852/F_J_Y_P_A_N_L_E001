#ifndef __COLOR_LIGHT_H
#define __COLOR_LIGHT_H


#define PI	3.141592653589793
//��׼��RGBɫ����
#define RX  0.7
#define RY  0.28
#define GX	0.08
#define GY	0.83
#define BX	0.1
#define BY	0.1

//LED�������ɫ����
#define LEDWWX_DEFAULT	0.4546       //0.4438
#define LEDWWY_DEFAULT	0.4078       //3958
#define LEDCWX_DEFAULT	0.3263      //0.3069
#define LEDCWY_DEFAULT	0.3297      //0.3044
#define LEDWX_DEFAULT	  0.3278
#define LEDWY_DEFAULT		0.3332
#define LEDRX_DEFAULT		0.6881
#define LEDRY_DEFAULT		0.3115
#define LEDGX_DEFAULT		0.1815
#define LEDGY_DEFAULT		0.7112
#define LEDBX_DEFAULT		0.1412 //
#define LEDBY_DEFAULT		0.0250
#define LEDYX_DEFAULT		0.5722//Y //0.00783	//��
#define LEDYY_DEFAULT		0.4248//Y0.4733	// .




#define MAX(a,b)	(a>b)?a:b

typedef struct{
  unsigned short h;
	float s;
	float i;
}HSI;

typedef struct{
	float r;
	float g;
	float b;
	float y;
	float w;
	float ww;
	float cw;
}RGB;

typedef struct{
	unsigned int *r;
	unsigned int *g;
	unsigned int *b;
	unsigned int *y;
	unsigned int *w;
	unsigned int *ww;
	unsigned int *cw;
}pRGBPwm;



typedef struct{
	float x;
	float y;
}COORD;
typedef struct
{
	int grn;
	unsigned char kvn; //ɫ��
	float dim;
	int pos;
	int max_pos;
}CCT;

typedef struct
{
	char num;
	COORD centre;
	COORD offset;
}CCT_TAB;

typedef struct
{
	unsigned char source;//��Դ
	unsigned char number;//ɫ��
	int dim;
}GEL;

void HSI_to_RGB(HSI HSI,RGB *rgbk);
void ColorLightHSIOut(HSI Hsi,unsigned char pixel);
void LightCCTOut( unsigned char pos,int offset,float dim,unsigned char pixel);
void LightRGBOut(unsigned int R,unsigned int G,unsigned int B,unsigned char pixel);
void RGBWWCW_to_coordinate(RGB rgb,COORD *coord);
int CCTToCoordinate(unsigned char pos,int offset,COORD *target);
int HSIToCoordinate(HSI *Hsi,COORD *coord);
int CoordinateOut(COORD *coord,float dim,unsigned char pixel);
void color_light_init(void);
void ColorTest(void);
void HSI_Test(void);

extern unsigned int RGBPwm[6];
extern HSI Hsi;
extern const CCT_TAB cct_tab[];

#endif



