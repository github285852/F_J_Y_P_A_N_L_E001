#include "color_light.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "includes.h"
#include "mymath.h"
#include "ledpower.h"
/*
unsigned char gama_tab[256]=
{
	0,1,4,9,16,25,36,49,65,82,101,122,145,170,198,227,258,291,327,364,403,444,488,533,581,630,681,735,790,848,907,969,1032,1098,1165,1235,1306,1380,1455,1533,1613,1694,1778,1864,1951,2041,2133,2226,2322,2420,2520,2621,2725,2831,2939,3049,3161,3274,3390,3508,3628,3750,3874,4000,4128,4258,4390,4524,4660,4798,4938,5081,5225,5371,5519,5669,5821,5976,6132,6290,6450,6612,6777,6943,7111,7282,7454,7628,7805,7983,8164,8346,8530,8717,8905,9096,9288,9483,9679,9878,10078,10281,10486,10692,10901,11111,11324,11539,11755,11974,12195,12418,12642,12869,13098,13329,13562,13796,14033,14272,14513,14756,15001,15248,15497,15748,16001,16256,16513,16772,17033,17296,17561,17828,18097,18368,18641,18916,19193,19473,19754,20037,20322,20609,20899,21190,21483,21778,22076,22375,22676,22980,23285,23593,23902,24213,24527,24842,25160,25479,25801,26124,26450,26777,27107,27439,27772,28108,28445,28785,29127,29470,29816,30164,30513,30865,31219,31575,31933,32292,32654,33018,33384,33752,34122,34493,34867,35243,35621,36001,36383,36767,37153,37541,37931,38323,38717,39113,39511,39912,40314,40718,41124,41532,41942,42355,42769,43185,43603,44024,44446,44870,45297,45725,46155,46588,47022,47458,47897,48337,48780,49224,49671,50119,50570,51022,51477,51933,52392,52852,53315,53780,54246,54715,55185,55658,56133,56610,57088,57569,58052,58537,59023,59512,60003,60496,60991,61488,61986,62487,62990,63495,64002,64511,65022,65535
};
*/


int IsSpecialPoint(coord_f point);
RGB rgb={0,0,0,0};
RGB rgbk={0,0,0,0};
COORD coord={0.3,0.5};
                     //  R   G   B   Y  W
unsigned int RGBPwm[6]={100,200,300,400,0,0};

float LEDWWX = LEDWWX_DEFAULT,
LEDWWY = LEDWWY_DEFAULT,
LEDCWX = LEDCWX_DEFAULT,
LEDCWY = LEDCWY_DEFAULT,
LEDWX = LEDWX_DEFAULT,
LEDWY = LEDWY_DEFAULT,
LEDRX = LEDRX_DEFAULT,
LEDRY = LEDRY_DEFAULT,
LEDGX = LEDGX_DEFAULT,
LEDGY = LEDGY_DEFAULT,
LEDBX = LEDBX_DEFAULT,
LEDBY = LEDBY_DEFAULT,
LEDYX = LEDYX_DEFAULT,
LEDYY = LEDYY_DEFAULT;

coord_f 
ledr={LEDRX_DEFAULT,LEDRY_DEFAULT},
ledg={LEDGX_DEFAULT,LEDGY_DEFAULT},
ledb={LEDBX_DEFAULT,LEDBY_DEFAULT},
ledww={LEDWWX_DEFAULT,LEDWWY_DEFAULT},
ledcw={LEDCWX_DEFAULT,LEDCWY_DEFAULT},
ledl={LEDLX_DEFAULT,LEDLY_DEFAULT},
leda={LEDAX_DEFAULT,LEDAY_DEFAULT};

	
	
const GEL_COORD GEL_TAB[] =
{																					//DAY  	//TUNGSTEN 
{"ROSCO R01",	"Light Bastard Amber",	0.4025, 	0.3229, 	0.5267, 	0.3914},
{"ROSCO R09",	"Pale Amber Gold",			0.3941, 	0.3795, 	0.4960, 	0.4090}, 
{"ROSCO R16",	"Light Amber",					0.4332, 	0.3875, 	0.5220, 	0.4176 },
{"ROSCO R19",	"Fire",									0.6546, 	0.3328, 	0.6586, 	0.3202 },//0.6646, 	0.3328, 	0.6786, 	0.3202
//{"ROSCO R20",	"Medium Amber",					0.5242, 	0.4448, 	0.5647, 	0.4184 },
{"ROSCO R24",	"Scarlet",							0.5608, 	0.3266, 	0.6608, 	0.3227 },
{"ROSCO R26",	"Light Red",						0.6659,		0.3159, 	0.6909, 	0.3088 },//{"ROSCO R26",	"Light Red	",					0.6759,		0.3159, 	0.6909, 	0.3088 },
{"ROSCO R31",	"Salmon Pink",					0.3964,		0.2938, 	0.5431, 	0.3574 },
{"ROSCO R33",	"No Color Pink",				0.3444,		0.3088, 	0.4801, 	0.3907 },
{"ROSCO R36",	"Medium Pink",					0.3583, 	0.2702, 	0.5142, 	0.3706 },
{"ROSCO R40",	"Light Salmon",					0.5013, 	0.3181, 	0.5952, 	0.3686 },
{"ROSCO R42",	"Deep Salmon",					0.5568,		0.2676, 	0.6711, 	0.3184 },
{"ROSCO R45",	"Rose",									0.4513, 	0.2248, 	0.6449, 	0.3082 },
{"ROSCO R54",	"Special Lavender",			0.2836, 	0.2790, 	0.4312, 	0.3789 },
{"ROSCO R57",	"Lavender",							0.2258, 	0.1914, 	0.3770, 	0.3134 },
{"ROSCO R60", "No Color Blue",				0.2673, 	0.2935, 	0.3995, 	0.3990 },
{"ROSCO R63",	"Pale Blue",						0.2593, 	0.2961, 	0.3957,		0.4018 },
{"ROSCO R65",	"Daylight Blue",				0.1896, 	0.2323, 	0.2905, 	0.3592 },
{"ROSCO R67",	"Ligth Sky Blue",				0.1747, 	0.2119, 	0.2583, 	0.3415 },
{"ROSCO R68",	"Parry Sky Blue",				0.1491, 	0.1584, 	0.1876, 	0.2628 },
{"ROSCO R70",	"Nile Blue",						0.2200, 	0.2802, 	0.3297, 	0.4151 },
//{"ROSCO R76",	"Light Green Blue",			0.1511, 	0.2344, 	0.1579, 	0.3401 },
//{"ROSCO R77",	"Green Blue",						0.1487, 	0.1720, 	0.1650, 	0.2716 },
//{"ROSCO R80",	"Primary Blue",					0.1463, 	0.1219, 	0.1582, 	0.2026 },
//{"ROSCO R85",	"Deep Blue",						0.1389, 	0.0513, 	0.1502, 	0.0569 },
{"ROSCO R86",	"Pea Green",						0.3520, 	0.5384, 	0.4300, 	0.5135 },
{"ROSCO R89",	"Moss Green",						0.2938, 	0.5646, 	0.3603, 	0.5671 },
{"ROSCO R90",	"Dark Yellow Green",		0.2134, 	0.6820, 	0.2469, 	0.6557 },
//{"ROSCO R95",	"Medium Bule Green",		0.1574, 	0.3395, 	0.1660, 	0.4610 },
{"ROSCO R99",	"Chocolate",						0.3836, 	0.3655, 	0.5092, 	0.4020 },
{"ROSCO R355",	"Pale Violet",				0.2049, 	0.1949, 	0.3395, 	0.3266 },
{"ROSCO R3202",	"Full CTB",						0.2391, 	0.2401, 	0.3695, 	0.3708 },
{"ROSCO R3204",	"1/2 CTB",						0.2754, 	0.2952, 	0.4162, 	0.3954 },
{"ROSCO R3208",	"1/4 CTB",						0.2952, 	0.3195, 	0.4433, 	0.4018 },
{"ROSCO R3304",	"Tough Plus Green",		0.3466, 	0.4374, 	0.4554, 	0.4598 },
{"ROSCO R3407",	"Roscosun CTO",				0.4494,		0.4135, 	0.5607, 	0.4157 },
{"ROSCO R3408",	"Roscosun 1/2 CTO",		0.3863, 	0.3916, 	0.5077, 	0.4146 },
{"ROSCO R3409",	"Roscosun 1/4 CTO",		0.3508, 	0.3640, 	0.4832, 	0.4167 },
{"ROSCO R4330",	"Calcolor 30 Cyan",		0.2831, 	0.3355, 	0.4079, 	0.4317 },
{"ROSCO R4360",	"Calcolor 60 Cyan",		0.2489, 	0.3337, 	0.3538, 	0.4488 },
{"ROSCO R3490",	"Calcolor 90 Cyan",		0.2170, 	0.3301, 	0.3048, 	0.4669 },
{"LEE 71",	"Tokyo Blue",							0.1501, 	0.0570, 	0.1531, 	0.0582 },
{"LEE 103",	"Straw",									0.3428, 	0.3607, 	0.4628, 	0.4193 },
//{"LEE 116",	"Medium Blue-Green",			0.1541,		0.2816, 	0.1634, 	0.4146 },
{"LEE 147",	"Apricot",								0.4502,		0.3838, 	0.5325, 	0.4018 },
{"LEE 713",	"J.winter Blue",					0.1450, 	0.0546, 	0.1477, 	0.0557 }
};


const CCT_TAB cct_tab[] = {
{25,0.4769,0.4137,0.5200,0.4820,0.5,5.555556},
{26,0.4681,0.4120,0.5100,0.4810,0.5,5.555556},
{27,0.4597,0.4105,0.4990,0.4790,0.5,5.555556},
{28,0.4518,0.4086,0.4890,0.4770,0.36,2.77778}, 
{29,0.4441,0.4064,0.4780,0.4730,0.36,2.77778},
{30,0.4369,0.4040,0.4690,0.4700,0.36,2.77778},
{31,0.4299,0.4015,0.4600,0.4660,0.24,1.470588}, 
{32,0.4233,0.3989,0.4510,0.4625,0.24,1.470588}, 
{33,0.4170,0.3962,0.4430,0.4590,0.24,1.470588}, 
{34,0.4109,0.3934,0.4350,0.4550,0.18,0.96},
{35,0.4052,0.3907,0.4280,0.4510,0.18,0.96},
{36,0.3998,0.3879,0.4210,0.4470,0.18,0.96},
{37,0.3948,0.3851,0.4145,0.4430,1.2,0.5}, 
{38,0.3895,0.3823,0.4080,0.4390,1.2,0.5},
{39,0.3849,0.3795,0.4025,0.4360,1.2,0.5},
{40,0.3804,0.3767,0.3970,0.4320,0.1,0.38},
{41,0.3761,0.3740,0.3910,0.4280,0.1,0.38},
{42,0.3720,0.3713,0.3850,0.4240,0.1,0.38}, 
{43,0.3681,0.3687,0.3800,0.4210,0.06,0.12}, 
{44,0.3643,0.3661,0.3750,0.4180,0.06,0.12},  
{45,0.3607,0.3635,0.3705,0.4140,0.06,0.12}, 
{46,0.3573,0.3611,0.3660,0.4100,0.04,0.02}, 
{47,0.3541,0.3586,0.3620,0.4070,0.04,0.02},  
{48,0.3510,0.3562,0.3580,0.4040,0.04,0.02}, 
{49,0.3480,0.3539,0.3540,0.4005,0.04,0.06}, 
{50,0.3450,0.3516,0.3500,0.3970,0.04,0.06}, 
{51,0.3423,0.3494,0.3460,0.3940,0.04,0.06}, 
{52,0.3397,0.3473,0.3425,0.3910,0.02,0}, 
{53,0.3371,0.3451,0.3390,0.3880,0.02,0},  
{54,0.3348,0.3431,0.3360,0.3850,0.02,0},  
{55,0.3324,0.3410,0.3330,0.3820,0,0},
{56,0.3302,0.3391,0.3300,0.3800,0,0},
{57,0.3281,0.3372,0.3270,0.3770,0,0},
{58,0.3260,0.3353,0.3245,0.3745,0,0},
{59,0.3240,0.3335,0.3220,0.3720,0,0},
{60,0.3222,0.3318,0.3195,0.3700,0,0},
{61,0.3203,0.3301,0.3170,0.3680,0,0}, 
{62,0.3185,0.3284,0.3150,0.3660,0,0},
{63,0.3168,0.3267,0.3130,0.3630,0,0},
{64,0.3152,0.3252,0.3110,0.3610,0,0},
{65,0.3136,0.3237,0.3090,0.3590,0,0},
{66,0.3121,0.3221,0.3070,0.3570,0,0}, 
{67,0.3105,0.3206,0.3050,0.3551,0,0.02}, 
{68,0.3092,0.3193,0.3030,0.3538,0,0.02}, 
{69,0.3078,0.3179,0.3015,0.3520,0,0.02}, 
{70,0.3065,0.3166,0.3000,0.3500,0,0.04}, 
{71,0.3051,0.3152,0.2980,0.3480,0,0.04}, 
{72,0.3038,0.3139,0.2962,0.3465,0,0.04}, 
{73,0.3027,0.3127,0.2945,0.3450,0,0.06},  
{74,0.3016,0.3116,0.2930,0.3430,0,0.06}, 
{75,0.3004,0.3104,0.2915,0.3420,0,0.06},
{76,0.2993,0.3092,0.2905,0.3390,0,0.08}, 
{77,0.2981,0.3080,0.2890,0.3380,0,0.08},  
{78,0.2972,0.3070,0.2878,0.3370,0,0.08},  
{79,0.2962,0.3060,0.2865,0.3360,0,0.08}, 
{80,0.2953,0.3049,0.2852,0.3350,0,0.08}, 
{81,0.2943,0.3039,0.2840,0.3338,0,0.08},  
{82,0.2934,0.3029,0.2830,0.3322,0,0.1},  
{83,0.2924,0.3018,0.2820,0.3310,0,0.1},
{84,0.2916,0.3009,0.2810,0.3300,0,0.1},
{85,0.2908,0.3000,0.2800,0.3290,0,0.1},
{86,0.2901,0.2992,0.2790,0.3280,0,0.1},
{87,0.2893,0.2983,0.2781,0.3270,0,0.1}, 
{88,0.2885,0.2974,0.2772,0.3258,0,0.12}, 
{89,0.2878,0.2965,0.2763,0.3245,0,0.12},
{90,0.2870,0.2956,0.2754,0.3235,0,0.12},
{91,0.2862,0.2948,0.2746,0.3224,0,0.12},
{92,0.2856,0.2941,0.2738,0.3212,0,0.12},
{93,0.2850,0.2934,0.2730,0.3200,0,0.12},
{94,0.2844,0.2927,0.2722,0.3190,0,0.12}, 
{95,0.2837,0.2919,0.2715,0.3180,0,0.12},
{96,0.2831,0.2912,0.2708,0.3170,0,0.12}, 
{97,0.2825,0.2905,0.2700,0.3160,0,0.12},
{98,0.2819,0.2898,0.2692,0.3150,0,0.12},
{99,0.2812,0.2890,0.2685,0.3135,0,0.12},
{100,0.2806,0.2883,0.2680,0.3120,0,0.12},
};
	
	
void HSI_to_RGB(HSI HSI,RGB *rgbk)
{
	float e,f,g,h;
	double temp;
	float cose;
//	int mode = HSI.h*1000;
//	mode %= 120;
//	temp = mode/1000.0;
//	e = temp*PI/180.0;	
	e = (HSI.h%120)*PI/180.0;
	f = HSI.i*(1.0-HSI.s);
	
	cose = cos(e);
	temp = 60*PI/180.0 - e;
	temp = cos(temp);
	
	g = HSI.i*(1+ HSI.s*cose/temp );
	h = 3*HSI.i - (f+g);
	if(HSI.h>=120)
	{
		if(HSI.h<240)
		{
			rgbk->r = f;
			rgbk->g = g;
			rgbk->b = h;
		}
		else
		{
			rgbk->r = h;
			rgbk->g = f;
			rgbk->b = g;
		}
	}
	else
	{
			rgbk->r = g;
			rgbk->g = h;
			rgbk->b = f;
	}
}


/*
两种颜色的灯合成坐标
A:LED1的坐标
B:LED2的坐标
Ka:LED1的亮度
Kb:LED2的亮度
OUT:cood,结果指针
*/

void TowLED_to_coordinate(COORD A,COORD B,float Ka,float Kb,COORD *co)
{
	float temp;
	temp = Ka + Kb;
	co->x = A.x - Kb*(A.x-B.x)/temp;
	co->y = A.y - Kb*(A.y-B.y)/temp;
	if((co->x<0)||(co->y<0))
	{
		Debug_printf("Function:TowLED_to_coordinate erro\r\n");
	}
}
/*
多种种颜色的灯合成坐标
ledc:led坐标指针
ledk:led CIE1931 色度比例
OUT:cood,结果指针
*/

void LEDK_to_coordinate(coord_f *ledc,float *ledk,coord_f *coord)
{
	int i;
	coord_f pre_coord={0,0},temp_coord = {0,0};
	float sumk = 0;
	for(i=0;i<LED_CHS;i++)
	{
		if(ledk[i]>0.0)
		{
			TowLED_to_coordinate(pre_coord,ledc[i],sumk,ledk[i],&temp_coord);	
			pre_coord = temp_coord;
			sumk += ledk[i];
		}
	}
	coord->x = temp_coord.x;
	coord->y = temp_coord.y;
}


void RGB_to_coordinate(RGB rgb,COORD *coord)
{
	float RBx,RBy,temp;
	COORD cia,cib;
	if(rgb.r != 0)
	{
		temp = (1+rgb.b/rgb.r);
		RBx = (RX+rgb.b*BX/rgb.r)/temp;
		RBy = (RY+rgb.b*BY/rgb.r)/temp;
		temp = 1+rgb.g/(rgb.r+rgb.b);
		coord->x = (RBx+rgb.g*GX/(rgb.b+rgb.r))/temp;
		coord->y = (RBy+rgb.g*GY/(rgb.b+rgb.r))/temp;
	}
	else //在GB线上
	{
//		coord->x = (GX*rgb.g+BX*rgb.b)/(rgb.g+rgb.b);
//		coord->y = (GY*rgb.g+BY*rgb.b)/(rgb.g+rgb.b);
		cia.x = GX;
		cia.y = GY;
		cib.x = BX;
		cib.y = BY;
		TowLED_to_coordinate(cia,cib,rgb.g,rgb.b,coord);
//		rs485_send_str("rgb.r=0\r\n");
//		sprintf(buf,"HSI(%d,%.2f,%.2f)\r\n",Hsi.h,Hsi.s,Hsi.i);
//		rs485_send_str(buf);
//		rs485_send_str("\r\n");
	}
}



void RGBWWCW_to_coordinate(RGB rgb,COORD *coord)
{
	COORD w_coord;
	COORD cia,cib;
	float RBx,RBy,temp;
	coord->x = coord->y = 0;
	if((rgb.r+rgb.g+rgb.b)>0.0)
	{
		if(rgb.r != 0)
		{
			temp = (1+rgb.b/rgb.r);
			RBx = (LEDRX+rgb.b*LEDBX/rgb.r)/temp;
			RBy = (LEDRY+rgb.b*LEDBY/rgb.r)/temp;
			temp = 1+rgb.g/(rgb.r+rgb.b);
			coord->x = (RBx+rgb.g*LEDGX/(rgb.b+rgb.r))/temp;
			coord->y = (RBy+rgb.g*LEDGY/(rgb.b+rgb.r))/temp;
		}
		else //在GB线上
		{
//			coord->x = (LEDGX*rgb.g+LEDBX*rgb.b)/(rgb.g+rgb.b);
//			coord->y = (LEDGY*rgb.g+LEDBY*rgb.b)/(rgb.g+rgb.b);
			cia.x = GX;
			cia.y = GY;
			cib.x = BX;
			cib.y = BY;
			TowLED_to_coordinate(cia,cib,rgb.g,rgb.b,coord);
		}
	}

	if(rgb.ww>0)
	{
		if(rgb.cw>0)
		{
//			w_coord.x = (LEDWWX*rgb.ww+LEDCWX*rgb.cw)/(rgb.ww+rgb.cw);
//			w_coord.y = (LEDWWY*rgb.ww+LEDCWY*rgb.cw)/(rgb.ww+rgb.cw);
			cia.x = LEDWWX;
			cia.y = LEDWWY;
			cib.x = LEDCWX;
			cib.y = LEDCWX;
			TowLED_to_coordinate(cia,cib,rgb.ww,rgb.cw,&w_coord);
		}
		else
		{
			w_coord.x = LEDWWX;
			
			w_coord.y = LEDWWY;
		}
	}
	else
	{
		if(rgb.cw>0)
		{
			w_coord.x = LEDCWX;
			w_coord.y = LEDCWY;
		}
		else
		{
			return ;
		}
	}
	if((rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw)>0)
	{
		coord->x = (w_coord.x*(rgb.ww+rgb.cw) + coord->x*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw);
		coord->y = (w_coord.y*(rgb.ww+rgb.cw) + coord->y*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw);
	}
}

void RGBLA_to_coordinate(RGB rgb,COORD *coord)
{
	COORD la_coord;
	COORD cia,cib;
	float RBx,RBy,temp;
	coord->x = coord->y = 0;
	if((rgb.r+rgb.g+rgb.b)>0.0)
	{
		if(rgb.r != 0)
		{
			temp = (1+rgb.b/rgb.r);
			RBx = (LEDRX+rgb.b*LEDBX/rgb.r)/temp;
			RBy = (LEDRY+rgb.b*LEDBY/rgb.r)/temp;
			temp = 1+rgb.g/(rgb.r+rgb.b);
			coord->x = (RBx+rgb.g*LEDGX/(rgb.b+rgb.r))/temp;
			coord->y = (RBy+rgb.g*LEDGY/(rgb.b+rgb.r))/temp;
		}
		else //在GB线上
		{
//			coord->x = (LEDGX*rgb.g+LEDBX*rgb.b)/(rgb.g+rgb.b);
//			coord->y = (LEDGY*rgb.g+LEDBY*rgb.b)/(rgb.g+rgb.b);
			cia.x = GX;
			cia.y = GY;
			cib.x = BX;
			cib.y = BY;
			TowLED_to_coordinate(cia,cib,rgb.g,rgb.b,coord);
		}
	}

	if(rgb.l>0)
	{
		if(rgb.a>0)
		{
			cia = ledl;
			cib = leda;
			TowLED_to_coordinate(cia,cib,rgb.l,rgb.a,&la_coord);
		}
		else
		{
			la_coord= ledl;
		}
	}
	else
	{
		if(rgb.a>0)
		{
			la_coord = leda;
		}
		else
		{
			return ;
		}
	}
	if((rgb.r+rgb.g+rgb.b+rgb.l+rgb.a)>0)
	{
		coord->x = (la_coord.x*(rgb.l+rgb.a) + coord->x*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.l+rgb.a);
		coord->y = (la_coord.y*(rgb.l+rgb.a) + coord->y*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.l+rgb.a);
	}
}

//已知两直线的坐标，求 其交点,特殊情况除外
int TowLineCross(COORD l1a,COORD l1b,COORD l2a,COORD l2b,COORD *crossp)
{
	float k1,k2,b1,b2;

	k1 = (l1a.y-l1b.y)/(l1a.x - l1b.x);
	b1 = l1a.y - k1*l1a.x;
	
	k2 = (l2a.y-l2b.y)/(l2a.x - l2b.x);
	b2 = l2a.y - k2*l2a.x;
	
	crossp->x = (b2-b1)/(k1-k2);
	crossp->y = crossp->x*k1 + b1;
	return 0;
}

//已知一直线上的三个坐标，求出中间点到两端的比
/*
INPUT: a、b,线段两端点坐标;amongp,中间坐标
OUTPUT：k,线段 a 对 b 的比例 的指针
RETURN ：0,OK;
*/
int LineSegmentK(COORD a,COORD b,COORD amongp,float *k)
{
	if(ABS(b.x - amongp.x)<0.0001)
		*k = 10000;
	else
		*k = (amongp.x - a.x)/(b.x - amongp.x);
	return 0;
}

//得出的RGB比例总和为1,
int coordinate_to_RGB(COORD coord,RGB *rgb)
{
	COORD R,G,B;
	COORD gb_crossp;//gb的交点
	float R_GB_K,G_B_K;//混光比例
	float GB;//GB的亮度
	float temp1;
//特殊点处理
//
	switch(IsSpecialPoint(coord))
	{
		case 1:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->r = 1;
			return 0;
		}
		case 2:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->g = 1;
			return 0;
		}
		case 3:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->b = 1;
			return 0;
		}
		case 4: //在RG线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDGX)/(LEDRX-coord.x);
			rgb->r = temp1/(1+temp1);
			rgb->g = 1 - rgb->r;
			return 0;
		}
		case 5://在RB线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDBX)/(LEDRX-coord.x);
			rgb->r = temp1/(1+temp1);
			rgb->b = 1 - rgb->r;
			return 0;
		}
		case 6://在GB线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDBX)/(LEDGX-coord.x);
			rgb->g = temp1/(1+temp1);
			rgb->b = 1 - rgb->g;
			return 0;
		}
		default :break;
	}
	R.x = LEDRX;
	R.y = LEDRY;
	G.x = LEDGX;
	G.y = LEDGY;
	B.x = LEDBX;
	B.y = LEDBY;
	//求GB的交点。
	TowLineCross(G,B,R,coord,&gb_crossp);
	//求gb交点和R对于coord的比例
	LineSegmentK(gb_crossp,R,coord,&R_GB_K); //混光比和线段比成反比
	LineSegmentK(B,G,gb_crossp,&G_B_K);
	memset(rgb,0,sizeof(RGB));
	//设总亮度为1
	rgb->r = R_GB_K/(1+R_GB_K);
	GB = 1 - rgb->r;
	if(G_B_K == 0)
	{
		rgb->g = GB;
		rgb->b = 0;
	}
	else
	{
		rgb->g = GB*G_B_K/(1+G_B_K);
		rgb->b = GB - rgb->g;
	}
	return 0;
}

int coordinate_to_threeLEDK(COORD A,COORD B,COORD C,COORD coord,float *a,float *b,float *c)
{
	COORD cb_crossp;
	float A_CB_K,C_B_K;
	float CB;
	TowLineCross(C,B,A,coord,&cb_crossp);
	LineSegmentK(cb_crossp,A,coord,&A_CB_K); //混光比和线段比成反比
	LineSegmentK(B,C,cb_crossp,&C_B_K);
	*a = A_CB_K/(1+A_CB_K);
	CB  = 1 - *a;
	*c = CB*C_B_K/(1+C_B_K);
	*b = CB - *c;
	return 0;
}


int coordinate_RGBLA(coord_f exp_point,LEDK *ledk)
{
	coord_f RA,LG;
	float ra;
	float lg;
	CCT_TAB cct = cct_tab[Sys.Config.cct.pos];

	
	TowLED_to_coordinate(ledr,leda,cct.ra_k,1,&RA);
	TowLED_to_coordinate(ledl,ledg,cct.lg_k,1,&LG);
	//先判断改点在不在BLA中
	if(InTriangle(exp_point,ledb,LG,RA))
	{
		memset(ledk,0,sizeof(LEDK)); 
		coordinate_to_threeLEDK(ledb,RA,LG,exp_point,&ledk->b,&ra,&lg);
		ledk->l = lg * cct.lg_k/(1+cct.lg_k);
		ledk->g  = lg - ledk->l;
		ledk->r = ra * cct.ra_k/(1+cct.ra_k);
		ledk->a  = ra - ledk->r;
		return 0;
	}

}

//采用WWCW的比例为 1:1，来达到较大的功率
/*
INPUT: coord，要合成的坐标，RGB_W_K,RGB合成点 和 白的比例
*/
int coordinate_to_RGBWWCW_mode1(COORD coord,float RGB_W_k,LEDK *ledk)
{
	COORD w_center;//冷白，暖白中点
  coord_f RGBp;
	COORD R,G,B;
	LEDK rgbk;//RGB之间的比例
	float RGB,W;//白的亮度，
	
	float L_RGB_W_k = 1.0/RGB_W_k; //线段比例
	w_center.x = (LEDWWX + LEDCWX)/2.0;
	w_center.y = (LEDWWY + LEDCWY)/2.0;
	if( (ABS(w_center.x-coord.x)<0.0001)||(ABS(w_center.x-coord.x)<0.0001) ) //就是 WW CW 的中点
	{
		ledk->r = 0;
		ledk->g = 0;
		ledk->b = 0;
		ledk->ww = 0.5;
		ledk->cw = 0.5;
		return 0;
	}
	//根据比例求出RGB合成的点。
	RGBp.x = coord.x - L_RGB_W_k*(w_center.x - coord.x);
	RGBp.y = coord.y - L_RGB_W_k*(w_center.y - coord.y);
	
	R.x = LEDRX;
	R.y = LEDRY;
	G.x = LEDGX;
	G.y = LEDGY;
	B.x = LEDBX;
	B.y = LEDBY;
	
	//RGB合成的点，要在三角形内，且 目标点coord在RGBp、w_center中间
	if(InTriangle(RGBp,R,G,B)==0)
	{
		Debug_printf(">>%s:exp_RGBp not in Triangle\r\n",__FUNCTION__);
		return 1;
	}
	if( ((coord.x > RGBp.x)&&(coord.x < w_center.x)) || ((coord.x > w_center.x)&&(coord.x < RGBp.x)) )
	{
		//求出RGB之间的比例rgbk
		coordinate_to_RGB(RGBp,&rgbk);//得出的RGB比例总和为1,不需要再归一化
		W = 1/(1+RGB_W_k);
		RGB = 1 - W;
		
		ledk->r = RGB * rgbk.r;
		ledk->g = RGB * rgbk.g;
		ledk->b = RGB * rgbk.b;
		ledk->ww = W/2;
		ledk->cw = W/2;
		return 0;
	}
	else
	{
		Debug_printf(">>%s:exp_RGBp not in center between w_center and exp_coord\r\n",__FUNCTION__);
		return 1;
	}
}

//采用单颗白灯一直亮度为100%,dim>0.5
/*
输出的是LED亮度值，CCT模式使用
*/
float one_w = 100;
int coordinate_to_RGBWWCW_mode2(COORD coord,float dim,LEDK *ledk)
{
	float temp,sum_light=200*dim;//总亮度200%
	temp = (LEDWWX + LEDCWX)/2.0;
	coord_f point;
	COORD R,G,B,W;
	float gk,bk,wk,rk;
	if(dim<0.5)
		return 1;
	R.x = LEDRX;
	R.y = LEDRY;
	G.x = LEDGX;
	G.y = LEDGY;
	B.x = LEDBX;
	B.y = LEDBY;
	if((coord.x<temp))//冷白单点100%亮度
	{
		sum_light -= one_w;
		//已知比例，中间点，一端点，求另一点
		point.x = one_w/sum_light*(coord.x - LEDCWX) + coord.x;
		point.y = one_w/sum_light*(coord.y - LEDCWY) + coord.y;
		W.x = LEDWWX;
		W.y = LEDWWY;
		if(InTriangle(point,G,B,W)==0)
		{
			Debug_printf(">>%s:exp_GBWW not in Triangle\r\n",__FUNCTION__);
			return 1;
		}
		coordinate_to_threeLEDK(G,B,W,point,&gk,&bk,&wk);
		ledk->r = 0;
		ledk->g = sum_light*gk;
		ledk->b = sum_light*bk;
		ledk->ww = sum_light*wk;
		ledk->cw = one_w;
	}
	else//暖白单点100%亮度
	{
		sum_light -= 100;
		point.x = 100/sum_light*(coord.x - LEDWWX) + coord.x;
		point.y = 100/sum_light*(coord.y - LEDWWY) + coord.y;
		W.x = LEDCWX;
		W.y = LEDCWY;
		if(InTriangle(point,G,R,W)==0)
		{
			Debug_printf(">>%s:exp_GRCW not in Triangle\r\n",__FUNCTION__);
			return 1;
		}
		coordinate_to_threeLEDK(G,R,W,point,&gk,&rk,&wk);
		ledk->r = sum_light*rk;
		ledk->g = sum_light*gk;
		ledk->b = 0;
		ledk->ww = 100;
		ledk->cw = sum_light*wk;
	}
	return 0;
}
//判断特殊点
/*

RETURN:
*/
int IsSpecialPoint(coord_f point)
{
	COORD R,G,B,W;
	R.x = LEDRX;
	R.y = LEDRY;
	G.x = LEDGX;
	G.y = LEDGY;
	B.x = LEDBX;
	B.y = LEDBY;
	if( (ABS(point.x-R.x)<0.0001)&&(ABS(point.y-R.y)<0.0001) )
	{
		return 1;
	}
	if( (ABS(point.x-G.x)<0.0001)&&(ABS(point.y-G.y)<0.0001) )
	{
		return 2;
	}
	if( (ABS(point.x-B.x)<0.0001)&&(ABS(point.y-B.y)<0.0001) )
	{
		return 3;
	}
	
	if(OnLineSegment(point,R,G))
	{
		return 4;
	}

	if(OnLineSegment(point,R,B))
	{
		return 5;
	}
	
	if(OnLineSegment(point,G,B))
	{
		return 6;
	}	
	return 0;
}

unsigned char coordinate_to_RGBYW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,RYx,RYy,GBx,GBy,GYx,GYy,BYx,BYy;
	float RGk,RBk,RYk,GBk,GYk,BYk,Vrage_x=0,Vrage_y,NWk;
	float temp1,temp2,temp3;
	unsigned char flag =0,valid_count=0;

	temp1 = (LEDWY-coord.y)/(LEDWX-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDRY)/(LEDYX-LEDRX);
	RYx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RYy = (RYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDGY)/(LEDYX-LEDGX);
	GYx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GYy = (GYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDBY)/(LEDYX-LEDBX);
	BYx = (coord.x*temp1-LEDBX*temp2+LEDBY-coord.y)/(temp1-temp2);
	BYy = (BYx-coord.x)*temp1 +coord.y;
	
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	RYk = (LEDRX-RYx)/(RYx-LEDYX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	GYk = (LEDGX-GYx)/(GYx-LEDYX);
	BYk = (LEDBX-BYx)/(BYx-LEDYX);
	
	if(RGk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RGx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x01;
			valid_count++;
			Vrage_x += RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x02;
			valid_count++;
			Vrage_x += RBx;
		}
	}
	if(RYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x04;
			valid_count++;
			Vrage_x += RYx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-GBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x08;
			valid_count++;
			Vrage_x += GBx;
		}
	}
	if(GYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-GYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x10;
			valid_count++;
			Vrage_x += GYx;
		}
	}
	if(BYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-BYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x20;
			valid_count++;
			Vrage_x += BYx;
		}
	}
	if(valid_count<=0)
	{
		Debug_printf("Coor_To_RGBYW No Color\r\n");
		return 1;
	}
	Vrage_x = (float)(Vrage_x/valid_count);
	Vrage_y = (Vrage_x-coord.x)*temp1+coord.y;//交点平均值y
	NWk = (Vrage_x-coord.x)/(coord.x-LEDWX);//交点和白光的线段比
	memset(rgb,0,sizeof(RGB)); 
	rgb->w = 1;
	temp2 = rgb->w/NWk;//假设白光亮度为1,计算出交点亮度
	if(flag&0x01)
	{
		temp3 = temp2/valid_count/(1+RGk);
		rgb->r += temp3;
		rgb->g += temp3*RGk;
	}
	if(flag&0x02)
	{
		temp3 = temp2/valid_count/(1+RBk);
		rgb->r += temp3;
		rgb->b += temp3*RBk;
	}
	if(flag&0x04)
	{
		temp3 = temp2/valid_count/(1+RYk);
		rgb->r += temp3;
		rgb->y += temp3*RYk;
	}
	if(flag&0x08)
	{
		temp3 = temp2/valid_count/(1+GBk);
		rgb->g += temp3;
		rgb->b += temp3*GBk;
	}
	if(flag&0x10)
	{
		temp3 = temp2/valid_count/(1+GYk);
		rgb->g += temp3;
		rgb->y += temp3*GYk;
	}
	if(flag&0x20)
	{
		temp3 = temp2/valid_count/(1+BYk);
		rgb->b += temp3;
		rgb->y += temp3*BYk;
	}
	return 0;
}

unsigned char coordinate_to_RGBYWWCW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,RYx,RYy,GBx,GBy,GYx,GYy,BYx,BYy;
	float RGk,RBk,RYk,GBk,GYk,BYk,Vrage_x=0,Vrage_y,NWk;
	float temp1,temp2,temp3;
	unsigned char flag =0,valid_count=0;

	float ledw_x,ledw_y ,point_x,point_y,ww_cw_k;//目标点向冷白和暖白所在线作垂线
//冷白和暖白合成合成的白
	temp1 = (LEDCWY-LEDWWY)/(LEDCWX-LEDWWX);
	temp2 = (LEDCWX - LEDWWX)/(LEDCWY-LEDWWY);
	point_x = (LEDWWX*temp1+coord.x*temp2+coord.y-LEDWWY)/(temp1+temp2);
	point_y = (point_x-LEDWWX)*temp1+LEDWWY;
	if(point_x>=LEDWWX)
	{
		ledw_x = LEDWWX;
	}
	else
	{
		if(point_x<=LEDCWX)
		{
			ledw_x = LEDCWX;
		}
		else
		{
			ledw_x = point_x;
		}
	}
	ledw_y = (ledw_x - LEDCWX)*temp1+LEDCWY;
	
	temp1 = (ledw_y-coord.y)/(ledw_x-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDRY)/(LEDYX-LEDRX);
	RYx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RYy = (RYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDGY)/(LEDYX-LEDGX);
	GYx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GYy = (GYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDBY)/(LEDYX-LEDBX);
	BYx = (coord.x*temp1-LEDBX*temp2+LEDBY-coord.y)/(temp1-temp2);
	BYy = (BYx-coord.x)*temp1 +coord.y;
	
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	RYk = (LEDRX-RYx)/(RYx-LEDYX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	GYk = (LEDGX-GYx)/(GYx-LEDYX);
	BYk = (LEDBX-BYx)/(BYx-LEDYX);
	
	if(RGk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RGx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x01;
			valid_count++;
			Vrage_x += RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x02;
			valid_count++;
			Vrage_x += RBx;
		}
	}
	if(RYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x04;
			valid_count++;
			Vrage_x += RYx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x08;
			valid_count++;
			Vrage_x += GBx;
		}
	}
	if(GYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x10;
			valid_count++;
			Vrage_x += GYx;
		}
	}
	if(BYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-BYx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x20;
			valid_count++;
			Vrage_x += BYx;
		}
	}
	if(valid_count==0)
	{
		Debug_printf("NO COLOR\r\n");
		return ERRO_NO_COLOR;
	}
	Vrage_x = (float)(Vrage_x/valid_count);
	Vrage_y = (Vrage_x-coord.x)*temp1+coord.y;//交点平均值y
	NWk = (Vrage_x-coord.x)/(coord.x-ledw_x);//交点和白光的线段比
	memset(rgb,0,sizeof(RGB));
	if(NWk==0)
	{
		temp2 = 1;
		rgb->ww = 0;
		rgb->cw = 0;
	}
	else
	{
		temp2 = 1/NWk;//假设白光亮度为1,计算出交点亮度
		ww_cw_k = (LEDCWX - ledw_x)/(LEDCWX-LEDWWX);//
		rgb->ww = ww_cw_k;
		rgb->cw = (1-ww_cw_k);
	}
	
	
	if(flag&0x01)
	{
		temp3 = temp2/valid_count/(1+RGk);
		rgb->r += temp3;
		rgb->g += temp3*RGk;
	}
	if(flag&0x02)
	{
		temp3 = temp2/valid_count/(1+RBk);
		rgb->r += temp3;
		rgb->b += temp3*RBk;
	}
	if(flag&0x04)
	{
		temp3 = temp2/valid_count/(1+RYk);
		rgb->r += temp3;
		rgb->y += temp3*RYk;
	}
	if(flag&0x08)
	{
		temp3 = temp2/valid_count/(1+GBk);
		rgb->g += temp3;
		rgb->b += temp3*GBk;
	}
	if(flag&0x10)
	{
		temp3 = temp2/valid_count/(1+GYk);
		rgb->g += temp3;
		rgb->y += temp3*GYk;
	}
	if(flag&0x20)
	{
		temp3 = temp2/valid_count/(1+BYk);
		rgb->b += temp3;
		rgb->y += temp3*BYk;
	}
	return 0;
}

unsigned char coordinate_to_RGBWWCW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,GBx,GBy;
	float RGk,RBk,GBk,valid_x=0,valid_y,NWk;
	float temp1,temp2,temp3,max_k;
	unsigned char flag =0;
	float ledw_x,ledw_y ,point_x,point_y,ww_cw_k;//目标点向冷白和暖白所在线作垂线
//特殊点处理
//
	switch(IsSpecialPoint(coord))
	{
		case 1:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->r = 1;
			return 0;
		}
		case 2:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->g = 1;
			return 0;
		}
		case 3:
		{
			memset(rgb,0,sizeof(RGB)); 
			rgb->b = 1;
			return 0;
		}
		case 4: //在RG线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDGX)/(LEDRX-coord.x);
			rgb->r = 1/(1+temp1);
			rgb->g = 1 - rgb->r;
			return 0;
		}
		case 5://在RB线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDBX)/(LEDRX-coord.x);
			rgb->r = 1/(1+temp1);
			rgb->b = 1 - rgb->r;
			return 0;
		}
		case 6://在GB线段上
		{
			memset(rgb,0,sizeof(RGB)); 
			temp1 = (coord.x-LEDBX)/(LEDGX-coord.x);
			rgb->g = 1/(1+temp1);
			rgb->b = 1 - rgb->g;
			return 0;
		}
		default :break;
	}

//冷白和暖白合成合成的白
	temp1 = (LEDCWY-LEDWWY)/(LEDCWX-LEDWWX);
	temp2 = (LEDCWX-LEDWWX)/(LEDCWY-LEDWWY);
	point_x = (LEDWWX*temp1+coord.x*temp2+coord.y-LEDWWY)/(temp1+temp2);
	point_y = (point_x-LEDWWX)*temp1+LEDWWY;
	if(point_x>=LEDWWX)
	{
		ledw_x = LEDWWX;
	}
	else
	{
		if(point_x<=LEDCWX)
		{
			ledw_x = LEDCWX;
		}
		else
		{
			ledw_x = point_x;
		}
	}
	ledw_y = (ledw_x - LEDCWX)*temp1+LEDCWY;
	//目标点和冷暖线上最短点做直线L1
	//目标点和最短点的斜率
	temp1 = (ledw_y-coord.y)/(ledw_x-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	//线段比例
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	
	if(RGk>0) //交点在RG中间
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RGx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x01;
			valid_x = RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x02;
			valid_x = RBx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GBx);
		if(temp2<0)//目标色在中间
		{
			flag |= 0x04;
			valid_x = GBx;
		}
	}
	if(valid_x==0)
	{
		Debug_printf("NO COLOR\r\n");
		
		return ERRO_NO_COLOR;
	}
	valid_y = (valid_x-coord.x)*temp1+coord.y;//交点y
	NWk = (valid_x-coord.x)/(coord.x-ledw_x);//交点和白光的线段比
	memset(rgb,0,sizeof(RGB)); 
	
	if(NWk==0)
	{
		temp2 = 1;
		rgb->ww = 0;
		rgb->cw = 0;
	}
	else
	{
		temp2 = 1/NWk;//假设白光亮度为1,计算出交点亮度
		ww_cw_k = (LEDCWX - ledw_x)/(LEDCWX-LEDWWX);//
		rgb->ww = ww_cw_k;
		rgb->cw = (1-ww_cw_k);
	}

	switch(flag)
	{
		case 0x01://求出r的亮度
		{
			temp3 = temp2/(1+RGk);
			rgb->r += temp3;
			rgb->g += temp3*RGk; //求出g的亮度
			break;
		}
		case 0x02:
		{
			temp3 = temp2/(1+RBk);
			rgb->r += temp3;
			rgb->b += temp3*RBk;
			break;
		}
		case 0x04:
		{
			temp3 = temp2/(1+GBk);
			rgb->g += temp3;
			rgb->b += temp3*GBk;
			break;
		}
		default :break;
	}
	//归一化处理
//	max_k = MAX(rgb->r,rgb->g);
//	max_k = MAX(max_k,rgb->b);
//	max_k = MAX(max_k,rgb->cw);
//	max_k = MAX(max_k,rgb->ww);

//	rgb->r = rgb->r/max_k;
//	rgb->g = rgb->g/max_k;
//	rgb->b = rgb->b/max_k;
//	rgb->ww = rgb->ww/max_k;
//	rgb->cw = rgb->cw/max_k; ，ledpower.c中LedkToCurrent()会做归一化处理
	return 0;
}


COORD LED_CHECK[5][10];

	
//带有电流校准的色坐标转LED比例
int check_coordinate_to_RGBWWCW(COORD coord,char dim,RGB *rgb)
{
	float temp;
	unsigned int k;
	int i;
	LEDWWX = LEDWWX_DEFAULT,
	LEDWWY = LEDWWY_DEFAULT,
	LEDCWX = LEDCWX_DEFAULT,
	LEDCWY = LEDCWY_DEFAULT,
	LEDRX = LEDRX_DEFAULT,
	LEDRY = LEDRY_DEFAULT,
	LEDGX = LEDGX_DEFAULT,
	LEDGY = LEDGY_DEFAULT,
	LEDBX = LEDBX_DEFAULT,
	LEDBY = LEDBY_DEFAULT;
	coordinate_to_RGBWWCW(coord,rgb);
	temp = rgb->r + rgb->g + rgb->b + rgb->cw + rgb->ww;//设亮度系数位temp
	temp = (float)(dim*10/temp);

	
	k = (int)(rgb->r*temp+0.5);//四舍五入
	if(k>10)
	{
		k = 10;
	}
	LEDRX = LED_CHECK[0][k-1].x;
	LEDRY = LED_CHECK[0][k-1].x;
		
	k = (int)(rgb->g*temp+0.5);//四舍五入
	if(k>10)
	{
		k = 10;
	}
	LEDGX = LED_CHECK[1][k-1].x;
	LEDGY = LED_CHECK[1][k-1].x;
	
	k = (int)(rgb->b*temp+0.5);//四舍五入
	if(k>10)
	{
		k = 10;
	}
	LEDBX = LED_CHECK[2][k-1].x;
	LEDBY = LED_CHECK[2][k-1].x;
	
	k = (int)(rgb->ww*temp+0.5);//四舍五入
	if(k>10)
	{
		k = 10;
	}
	LEDWWX = LED_CHECK[3][k-1].x;
	LEDWWY = LED_CHECK[3][k-1].x;
	
	k = (int)(rgb->cw*temp+0.5);//四舍五入
	if(k>10)
	{
		k = 10;
	}
	LEDCWX = LED_CHECK[4][k-1].x;
	LEDCWY = LED_CHECK[4][k-1].x;
	return coordinate_to_RGBWWCW(coord,rgb);
}

/*


*/
int HSIToCoordinate(HSI *Hsi,COORD *coord)
{
	HSI hsi;
	float temp;
	int i;
	hsi = *Hsi;
//	do
//	{
//		HSI_to_RGB(hsi,&rgb);
//		RGB_to_coordinate(rgb,coord);
	
//		hsi.s -= 0.01;//找到饱和度最大的那个点
//	}
//	while(coordinate_to_RGBWWCW(*coord,&rgbk)&&(hsi.s>0));
//	Debug_printf("能达到的最大饱和度=%d\r\n",(u8)(hsi.s*100));
//	hsi.s = hsi.s*Hsi->s; // 饱和度饱和度换
	
	HSI_to_RGB(hsi,&rgb);
	RGB_to_coordinate(rgb,coord);
	return 0;
	
}
/*
INPUT HSI:H=0-259;S=0.0-1.0;
pixel:0,ALL,
*/
void ColorLightHSIOut(HSI Hsi,unsigned char pixel)
{
	COORD coo;
	if(HSIToCoordinate(&Hsi,&coord))
	{
		Debug_printf("饱和度转换后任然没有\r\n");
		return;
	}
	if(CoordinateOut(&coord,Hsi.i,pixel))
	{
		return ;
	}
	
	RGBLA_to_coordinate(rgbk,&coo);
	Debug_printf("\r\n");
	Debug_printf("HSI(%d,%.2f,%.2f)\r\n",Hsi.h,Hsi.s,Hsi.i);
	Debug_printf("计算色坐标(%.4f,%.4f)\r\n",coord.x,coord.y);
	Debug_printf("反馈色坐标(%.4f,%.4f)\r\n",coo.x,coo.y);
	Debug_printf("RGBWWCW(%.4f,%.4f,%.4f,%.4f,%.4f)\r\n",rgbk.r,rgbk.g,rgbk.b,rgbk.l,rgbk.a);
	
}



void color_light_init(void)
{
	Sys.Config.cct.max_pos = sizeof(cct_tab)/sizeof(CCT_TAB) - 1;
	Sys.max_gel_number = sizeof(GEL_TAB)/sizeof(GEL_COORD);
}


/*
INPUT:pos:number;offset;
OUTPUT:target,point of coordinate
RETURN:0,OK;1,fail
*/
int CCTToCoordinate(unsigned char pos,int offset,COORD *target)
{
	COORD neg_offset;//负偏坐标
	COORD pos_offset,centre;
	if( pos >= sizeof(cct_tab)/sizeof(CCT_TAB) ||(offset>100)||(offset<-100))
	{
		return 1;
	}
	pos_offset = cct_tab[pos].offset;
	centre = cct_tab[pos].centre;
	neg_offset.x = 2*centre.x - pos_offset.x;
	neg_offset.y = 2*centre.y - pos_offset.y;
	if(offset>0)
	{
		target->x = (pos_offset.x-centre.x)*offset/100.0 + centre.x;
		target->y = (pos_offset.y-centre.y)*offset/100.0 + centre.y;
	}
	else
	{
		offset *= -1;
		target->x = (neg_offset.x-centre.x)*offset/115.0 + centre.x;
		target->y = (neg_offset.y-centre.y)*offset/115.0 + centre.y;
	}
	return 0;
}

/*
INPUT:pos:number;offset,grn;pixel,0,ALL;
OUTPUT:NO ONE
RETURN:NO ONE
*/
void LightCCTOut(unsigned char pos,int offset,float dim,unsigned char pixel)
{
	COORD coo;
	COORD target;//负偏坐标
	if(CCTToCoordinate(pos,offset,&target))
		return;
	CoordinateOut(&target,dim,pixel);
	
	LEDK_to_coordinate(led_coord,(float*)&rgbk,&coo);
	Debug_printf("\r\n");
	Debug_printf("计算色坐标(%.4f,%.4f)\r\n",target.x,target.y);
	Debug_printf("反馈色坐标2(%.4f,%.4f)\r\n",coo.x,coo.y);
	Debug_printf("LEDK(%.4f,%.4f,%.4f,%.4f,%.4f)\r\n",rgbk.r,rgbk.g,rgbk.b,rgbk.l,rgbk.a);
}

float rgb_w_k = 1;

#ifdef SPOT

int CoordinateOut(COORD *coord,float dim,unsigned char pixel)
{
	int res = 1;
	RGB ledk;
	float mode2_could_dim;//CCT mode　混光　能达到的最小亮度，这种显指较好
	float temp;
	if(dim<=0.0009)
	{
		AllLedPowerOff();
		return 0;
	}
	dim = DIM_MIN + dim*(1-DIM_MIN);//从百分之一开始
	dim = pow(dim,1.5);//伽马校正
		
	if((Sys.Config.lightmode == CCT_M)||( Sys.Config.lightmode == DMX_M &&( Sys.Config.dmx.mode==DMX_M2 ||Sys.Config.dmx.mode==DMX_M7 ||Sys.Config.dmx.mode==DMX_M12 ) ))
	{ 	
		res = coordinate_RGBLA(*coord,&rgbk);
	}
	else
	{
		res = coordinate_to_RGB(*coord,&rgbk);
	}
	if(res)
	{
		 return res;
	}
	ledk = rgbk;
	if(pixel==0)
		
		AllLedPowerOut((float *)&ledk,dim);
	else
   LedPowerOut((float *)&ledk,dim,pixel-1);
	return 0;
}

#elif defined PANLE


int CoordinateOut(COORD *coord,float dim,unsigned char pixel)
{
	int res = 1;
	RGB ledk;
	float mode2_could_dim;//CCT mode　混光　能达到的最小亮度，这种显指较好
	float temp;
	if(dim<=0.0009)
	{
		AllLedPowerOff();
		return 0;
	}
	dim = DIM_MIN + dim*(1-DIM_MIN);//从百分之一开始
	dim = pow(dim,1.5);//伽马校正

	res = coordinate_to_RGBWWCW(*coord,&rgbk);
	//	res = coordinate_to_RGB(*coord,&rgbk);

	if(res)
	{
		 return res;
	}
	ledk = rgbk;
	if(pixel==0)
		
		AllLedPowerOut((float *)&ledk,dim);
	else
   LedPowerOut((float *)&ledk,dim,pixel-1);
	return 0;
}

#endif


void LightRGBOut(unsigned int R,unsigned int G,unsigned int B,unsigned char pixel)
{
	float dim = (R+G+B)/30000.0;
	rgb.r = R;
	rgb.g = G;
	rgb.b = B;
	RGB_to_coordinate(rgb,&coord);
	if(coordinate_to_RGBWWCW(coord,&rgbk))
	{

		Debug_printf("RGB 合成不了\r\n");
		return ;
	}
	if(pixel>3)
		AllLedPowerOut((float *)&rgbk,dim);
	else
		LedPowerOut((float *)&rgbk,dim,pixel);
}

int LightGELOut(GEL *gel,unsigned char pixel)
{
	COORD coord;
	if((Sys.max_gel_number > gel->number)&&(gel->source<2))
	{
		coord = GEL_TAB[gel->number].coord[gel->source];
		return CoordinateOut(&coord,gel->dim,pixel);
	}
	else
	{
		return ERRO_PARAM;
	}
}

//Test Code
////////////////////////////////////////////////////////

void ColorTest(void)
{
	HSI hsi;
	char buf[100];
	hsi.i = 1;
	for(hsi.h=0;hsi.h<360;hsi.h++)
	{
		hsi.s = 1; //有些饱和度太高的颜色合成不了
		do
		{
			HSI_to_RGB(hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			hsi.s -= 0.01;//找到饱和度最大的那个点
		}
		while(coordinate_to_RGBYW(coord,&rgbk)&&(hsi.s>0));
		hsi.s += 0.01;

		while(hsi.s>0)
		{
			HSI_to_RGB(hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			if(coordinate_to_RGBYW(coord,&rgbk))
			{
				Debug_printf("饱和度转换后:No Color\r\n");
				Debug_printf("\r\n");
				Debug_printf("HSI(%d,%.2f,%.2f)\r\n",hsi.h,hsi.s,hsi.i);
				Debug_printf("色坐标(%.4f,%.4f)\r\n",coord.x,coord.y);					
				Debug_printf("\r\n");
			}
			hsi.s -= 0.01;
		}
	}
}

void HSI_Test(void)
{
	Sys.Config.hsi.i = 1;
	for(Sys.Config.hsi.h=0;Sys.Config.hsi.h<360;Sys.Config.hsi.h++)
	{
		Sys.Config.hsi.s = 1; //有些饱和度太高的颜色合成不了
		while(Sys.Config.hsi.s>0)
		{
			HSI_to_RGB(Sys.Config.hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			Sys.Config.hsi.s -= 0.01;
		}
	}
}







