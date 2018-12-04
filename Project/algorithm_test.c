#include "color_light.h"
#include "mymath.h"
#include "includes.h"

void HsiToCoordTest(void)
{
	float tim = 0;
	HSI hsi;
	RGB temp_rgb;
	COORD point,target;
	COORD R,G,B;
	Debug_printf(">>HSI to coord test start.\r\n");
	tim = GetSysTime_us()/1000000.0f;
	R.x = RX;
	R.y = RY;
	B.x = BX;
	B.y = BY;
	G.x = GX;
	G.y = GY;
	for(hsi.h=0;hsi.h<360;hsi.h++)
	{
		for(hsi.s=0.0;hsi.s<0.9;hsi.s+=0.01)
		{
			HSI_to_RGB(hsi,&temp_rgb);
			RGB_to_coordinate(temp_rgb,&point);
			if(InTriangle(point,R,G,B)==0)
			{
				Debug_printf(">>H:%d,S:%0.4f,is erro\r\n",hsi.h,hsi.s);
			}
			coordinate_to_RGBWWCW(point,&temp_rgb);
			RGBWWCW_to_coordinate(temp_rgb,&target);
			if((ABS(point.x-target.x)>0.005)||(ABS(point.y-target.y)>0.005))
			{
				Debug_printf("HS(%d,%.2f)\r\n",hsi.h,hsi.s);
				Debug_printf("计算色坐标(%.4f,%.4f)\r\n",point.x,point.y);
				Debug_printf("反馈色坐标(%.4f,%.4f)\r\n",target.x,target.y);
			}
		}
	}
	tim = GetSysTime_us()/1000000.0f - tim;
	Debug_printf(">>HSI to coord end.cost tim :%0.5f\r\n",tim);
}

void AlgorithmTest(void)
{
	//HsiToCoordTest();

}
