#include "includes.h"

DMXData DmxData;

void DMX512_handle(void);

#define FINE		0.1 //10%


//将数据放入缓存
void receiving_dmx_data(void)
{
	if(Sys.dmx_handle ==0)
	{
		u8 len = sizeof(DMXData) ;
		memcpy( &DmxData,(&DMX512_RX_BUF[Sys.Config.dmx.addr]) ,len);
		Sys.dmx_handle = 1;
	}
}
	
void DMXTask(void)
{
	static unsigned char dmx_detect = 0;
	if(Sys.check)
		return;
	if(Sys.dmx_handle)
	{
		Sys.dmx_handle = 0;
		DMX512_handle();
		dmx_detect=200;
	}
	if(dmx_detect>0)
	{
		dmx_detect--;
	}
	
	if(dmx_detect)
	{	
		if(Sys.dmx_connect==0)
		{
			Sys.dmx_connect = 1;//连接
			Sys.dmx_insert = 1;
			Sys.last_light_mode = Sys.Config.lightmode;
			DMX_ModeTaskInit();
			Sys.Config.lightmode = DMX_M;
		}
	}
	else
	{
		if(Sys.dmx_connect==1)
		{
			Sys.dmx_connect = 0;//断开
			Sys.Config.lightmode = Sys.last_light_mode;
			GUI_DisplayInit();
		}
	}
}

int  get_offset_8bit(unsigned char grn)
 {
	if(grn<10)
	{
		return 0;
	}
	else if(11<=grn && grn<=20)
	{
		return  -100;
	}
	else if(21<=grn && grn<=119)
	{
		return -99 +( grn - 21);
	}
	else if(120<=grn && grn<=145)
	{
		return 0;
	}
	else if(146<=grn && grn<=244)
	{
		return grn - 145;
	}
	else if(245<=grn && grn<=255)	
	{
		return 100;
	}
 }
 
 int get_offset_16bit(unsigned short grn)
 {
	if(grn<=5000)
	{
		return 0;
	}
	else if(5001<=grn && grn<=10000)
	{
		return  -100;
	}
	else if(10001<=grn && grn<=29999)
	{
		return -99 +( grn - 10001)*98/19998;
	}
	else if(30000<=grn && grn<=40000)
	{
		return 0;
	}
	else if(40001<=grn && grn<=59999)
	{
		return 1 + (grn - 40001)*98/19998;
	}
	else if(60000<=grn && grn<=65535)	
	{
		return 100;
	}
 }
 

int CCT_LEDK_OUT_8BIT(CCT_LEDK_8BIT *cct_ledk)
{
	float ledk[LED_CHS];
	COORD ledk_coord;
	coord_f cct_coord,temp_coord;
	int i;
	char pos;
	int offset;
	LEDK rgbk;
	float wight = cct_ledk->w_c/255.0;
	pos = Sys.Config.cct.max_pos*cct_ledk->kvn/255;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_8bit(cct_ledk->grn);
	for(i=0;i<LED_CHS;i++)
	{
		ledk[i] = cct_ledk->ledk[i]/255.0;
	}
	LEDK_to_coordinate(led_coord,ledk,&ledk_coord);
	CCTToCoordinate(pos,offset,&cct_coord);
	temp_coord.x = wight*ledk_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*ledk_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,cct_ledk->dim/255.0,0);
	return 0;
}

int CCT_LEDK_OUT_16BIT(CCT_LEDK_16BIT *cct_ledk)
{
	float ledk[LED_CHS];
	COORD ledk_coord,cct_coord,temp_coord;
	int i;
	char pos;
	int offset;
	float wight = cct_ledk->w_c/65535.0;
	pos = Sys.Config.cct.max_pos*cct_ledk->kvn/65535;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_16bit(cct_ledk->grn);
	for(i=0;i<LED_CHS;i++)
		ledk[i] = cct_ledk->ledk[i]/65535.0;

	LEDK_to_coordinate(led_coord,ledk,&ledk_coord);
	CCTToCoordinate(pos,offset,&cct_coord);
	temp_coord.x = wight*ledk_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*ledk_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,cct_ledk->dim/65535.0,0);
	return 0;
}

int CCT_OUT_8BIT(CCT_8BIT *cct)
{
	char pos;
	int offset;
	pos = Sys.Config.cct.max_pos*cct->kvn/255;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_8bit(cct->grn);
	LightCCTOut(pos,offset,cct->dim/255.0,0);
	return 0;
}

int CCT_OUT_16BIT(CCT_16BIT *cct)
{
	char pos;
	int offset;
	pos = Sys.Config.cct.max_pos*cct->kvn/65535;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	
	offset = get_offset_16bit(cct->grn);
	LightCCTOut(pos,offset,cct->dim/65535.0,0);
	return 0;
}


int CCT_HSI_OUT_8BIT(CCT_HSI_8BIT *cct_hsi)
{
	char pos;
	int offset;
	HSI hsi;
	COORD temp_coord;
	COORD cct_coord,hsi_coord;
	float wight;
	wight = cct_hsi->w_c/255.0;
	hsi.h = cct_hsi->h*359/255.0;
	hsi.s = cct_hsi->s/255.0;
	hsi.i = cct_hsi->dim/255.0;
	pos = Sys.Config.cct.max_pos*cct_hsi->kvn/255;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	
	offset = get_offset_8bit(cct_hsi->grn);
	
	CCTToCoordinate(pos,offset,&cct_coord);
	HSIToCoordinate(&hsi,&hsi_coord);
	temp_coord.x = wight*hsi_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*hsi_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,hsi.i,0);
	return 0;
}

int CCT_HSI_OUT_16BIT(CCT_HSI_16BIT *cct_hsi)
{
	char pos;
	int offset;
	HSI hsi;
	COORD temp_coord;
	COORD cct_coord,hsi_coord;
	float wight;
	wight = cct_hsi->w_c/65535.0;
	hsi.h = cct_hsi->h*359/65535.0;
	hsi.s = cct_hsi->s/65535.0;
	hsi.i = cct_hsi->dim/65535.0;
	pos = Sys.Config.cct.max_pos*cct_hsi->kvn/65535;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_16bit(cct_hsi->grn);
	
	CCTToCoordinate(pos,offset,&cct_coord);
	HSIToCoordinate(&hsi,&hsi_coord);
	temp_coord.x = wight*hsi_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*hsi_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,hsi.i,0);
	return 0;
	
}

int LEDK_OUT(LEDK_16BIT *ledk_16bit)
{
	int i;
	float ledk[LED_CHS];
	for(i=0;i<LED_CHS;i++)
		ledk[i] = ledk_16bit->ledk[i]/65535.0;
	AllLedPowerOut(ledk,ledk_16bit->dim/65535.0);
	return 0;
}
HSI panle_hsi;
int HSI_OUT(HSI_16BIT *hsi)
{
	panle_hsi.h = hsi->h*359/65535.0;
	panle_hsi.s = hsi->s/65535.0;
	panle_hsi.i = hsi->dim/65535.0;
	ColorLightHSIOut(panle_hsi,0);
	return 0;
}
void DMX512_handle(void)
{
	int i;
	DMXData temp;
	unsigned int d32;
		switch(Sys.Config.dmx.mode)
		{
			case DMX_M1:
				CCT_LEDK_OUT_8BIT(&DmxData.cct_ledk_8bit);
				break;
			case DMX_M2:
				CCT_OUT_8BIT(&DmxData.cct_8bit);			
				break;
			case DMX_M3:
				CCT_HSI_OUT_8BIT(&DmxData.cct_hsi_8bit);
				break;
			case DMX_M4:
				temp.ledk_16bit.dim = DmxData.ledk_8bit.dim*65535/255;//转换成16位
				for(i=0;i<LED_CHS;i++)
					temp.ledk_16bit.ledk[i] = DmxData.ledk_8bit.ledk[i]*65535/255;//转换成16位
				LEDK_OUT(&temp.ledk_16bit);
				break;
			case DMX_M5:
				temp.hsi_16bit.dim = DmxData.hsi_8bit.dim*65535/255;
				temp.hsi_16bit.h = DmxData.hsi_8bit.h*65535/255;
				temp.hsi_16bit.s = DmxData.hsi_8bit.s*65535/255;
				HSI_OUT(&temp.hsi_16bit);
				break;
			case DMX_M6:
				temp.cct_ledk_16bit.dim = DmxData.cct_ledk_16bit.dim >> 8 | DmxData.cct_ledk_16bit.dim << 8;
				temp.cct_ledk_16bit.kvn = DmxData.cct_ledk_16bit.kvn >> 8 | DmxData.cct_ledk_16bit.kvn << 8;
				temp.cct_ledk_16bit.grn = DmxData.cct_ledk_16bit.grn >> 8 | DmxData.cct_ledk_16bit.grn << 8;
				temp.cct_ledk_16bit.w_c = DmxData.cct_ledk_16bit.w_c >> 8 | DmxData.cct_ledk_16bit.w_c<< 8;
				for(i=0;i<LED_CHS;i++)
					temp.cct_ledk_16bit.ledk[i] = DmxData.cct_ledk_16bit.ledk[i] >> 8 | DmxData.cct_ledk_16bit.ledk[i] << 8;
				CCT_LEDK_OUT_16BIT(&temp.cct_ledk_16bit);
				break;
			case DMX_M7:
				temp.cct_16bit.dim = DmxData.cct_16bit.dim >> 8 | DmxData.cct_16bit.dim << 8;
				temp.cct_16bit.grn = DmxData.cct_16bit.grn >> 8 | DmxData.cct_16bit.grn << 8;
				temp.cct_16bit.kvn = DmxData.cct_16bit.kvn >> 8 | DmxData.cct_16bit.kvn << 8;
				CCT_OUT_16BIT(&temp.cct_16bit);
				break;
			case DMX_M8:
				temp.cct_hsi_16bit.dim = DmxData.cct_hsi_16bit.dim >> 8 | DmxData.cct_hsi_16bit.dim << 8;
				temp.cct_hsi_16bit.kvn = DmxData.cct_hsi_16bit.kvn >> 8 | DmxData.cct_hsi_16bit.kvn << 8;
				temp.cct_hsi_16bit.grn = DmxData.cct_hsi_16bit.grn >> 8 | DmxData.cct_hsi_16bit.grn << 8;
				temp.cct_hsi_16bit.w_c = DmxData.cct_hsi_16bit.w_c >> 8 | DmxData.cct_hsi_16bit.w_c<< 8;
				temp.cct_hsi_16bit.h = DmxData.cct_hsi_16bit.h >> 8 | DmxData.cct_hsi_16bit.h << 8;
				temp.cct_hsi_16bit.s = DmxData.cct_hsi_16bit.s >> 8 | DmxData.cct_hsi_16bit.s << 8;
				CCT_HSI_OUT_16BIT(&temp.cct_hsi_16bit);
				break;
			case DMX_M9:
				temp.ledk_16bit.dim = DmxData.ledk_16bit.dim >> 8 | DmxData.ledk_16bit.dim << 8;
				for(i=0;i<LED_CHS;i++)
					temp.ledk_16bit.ledk[i] = DmxData.ledk_16bit.ledk[i] >> 8 | DmxData.ledk_16bit.ledk[i] << 8;
				LEDK_OUT(&temp.ledk_16bit);
			break;
			case DMX_M10:
				temp.hsi_16bit.dim = DmxData.hsi_16bit.dim>>8 | DmxData.hsi_16bit.dim<<8;
				temp.hsi_16bit.h = DmxData.hsi_16bit.h>>8 | DmxData.hsi_16bit.h<<8;
				temp.hsi_16bit.s = DmxData.hsi_16bit.s>>8 | DmxData.hsi_16bit.s<<8;
				HSI_OUT(&temp.hsi_16bit);
				break;
			case DMX_M11://CCT_ledk_CF
				temp.cct_ledk_8bit.dim = DmxData.cct_ledk_cf.dim.coarse + (DmxData.cct_ledk_cf.dim.fine)*FINE;if(temp.cct_ledk_8bit.dim<DmxData.cct_ledk_cf.dim.coarse)temp.cct_ledk_8bit.dim = 255;
				temp.cct_ledk_8bit.kvn = DmxData.cct_ledk_cf.kvn.coarse + (DmxData.cct_ledk_cf.kvn.fine)*FINE;if(temp.cct_ledk_8bit.kvn<DmxData.cct_ledk_cf.kvn.coarse)temp.cct_ledk_8bit.kvn= 255;
				temp.cct_ledk_8bit.grn = DmxData.cct_ledk_cf.grn;
				temp.cct_ledk_8bit.w_c = DmxData.cct_ledk_cf.w_c;
				for(i=0;i<LED_CHS;i++)
				{
					temp.cct_ledk_8bit.ledk[i] = DmxData.cct_ledk_cf.ledk[i].coarse + (DmxData.cct_ledk_cf.ledk[i].fine)*FINE;
					if(temp.cct_ledk_8bit.ledk[i]<DmxData.cct_ledk_cf.ledk[i].coarse)temp.cct_ledk_8bit.ledk[i] = 255;
				}
				CCT_LEDK_OUT_8BIT(&temp.cct_ledk_8bit);
				break;
			case DMX_M12://CCT__CF
				temp.cct_8bit.dim = DmxData.cct_ledk_cf.dim.coarse + (DmxData.cct_ledk_cf.dim.fine)*FINE;if(temp.cct_ledk_8bit.dim<DmxData.cct_ledk_cf.dim.coarse)temp.cct_ledk_8bit.dim = 255;
				temp.cct_8bit.kvn = DmxData.cct_ledk_cf.kvn.coarse + (DmxData.cct_ledk_cf.kvn.fine)*FINE;if(temp.cct_ledk_8bit.kvn<DmxData.cct_ledk_cf.kvn.coarse)temp.cct_ledk_8bit.kvn= 255;
				temp.cct_8bit.grn = DmxData.cct_ledk_cf.grn;
				CCT_OUT_8BIT(&temp.cct_8bit);
				break;
			case DMX_M13://CCT_HSI_CF
				temp.cct_hsi_8bit.dim = DmxData.cct_hsi_cf.dim.coarse + (DmxData.cct_hsi_cf.dim.fine)*FINE;if(temp.cct_hsi_8bit.dim<DmxData.cct_hsi_cf.dim.coarse)temp.cct_hsi_8bit.dim = 255;
				temp.cct_hsi_8bit.kvn = DmxData.cct_hsi_cf.kvn.coarse + (DmxData.cct_hsi_cf.kvn.fine)*FINE;if(temp.cct_hsi_8bit.kvn<DmxData.cct_hsi_cf.kvn.coarse)temp.cct_hsi_8bit.kvn= 255;
				temp.cct_hsi_8bit.grn = DmxData.cct_hsi_cf.grn;
				temp.cct_hsi_8bit.w_c = DmxData.cct_hsi_cf.w_c;
				temp.cct_hsi_8bit.h = DmxData.cct_hsi_cf.h.coarse + (DmxData.cct_hsi_cf.h.fine)*FINE;if(temp.cct_hsi_8bit.h<DmxData.cct_hsi_cf.h.coarse)temp.cct_hsi_8bit.h = 255;
				temp.cct_hsi_8bit.s = DmxData.cct_hsi_cf.s.coarse + (DmxData.cct_hsi_cf.s.fine)*FINE;if(temp.cct_hsi_8bit.s<DmxData.cct_hsi_cf.s.coarse)temp.cct_hsi_8bit.s = 255;
				CCT_HSI_OUT_8BIT(&temp.cct_hsi_8bit);
				break;
			case DMX_M14://ledk_CF
				d32 = DmxData.ledk_cf.dim.coarse*257 + (DmxData.ledk_cf.dim.fine)*257*FINE;if(d32>65535) temp.ledk_16bit.dim = 65535;else temp.ledk_16bit.dim = d32;
				for(i=0;i<LED_CHS;i++)
				{
					d32 = DmxData.ledk_cf.ledk[i].coarse*257 + (DmxData.ledk_cf.ledk[i].fine)*257*FINE;
					if(d32>65535)
						temp.ledk_16bit.ledk[i] = 65535;
					else 
						temp.ledk_16bit.ledk[i] = d32;
				}
				LEDK_OUT(&temp.ledk_16bit);
				break;
			case DMX_M15://CCT_HSI_CF
				d32  = DmxData.hsi_cf.dim.coarse*257 + (DmxData.hsi_cf.dim.fine)*257*FINE;if(d32>65535)temp.hsi_16bit.dim = 65535;else temp.hsi_16bit.dim = d32;
				d32  = DmxData.hsi_cf.h.coarse*257 + (DmxData.hsi_cf.h.fine)*257*FINE;if(d32>65535)temp.hsi_16bit.h = 65535;else temp.hsi_16bit.h = d32;
				d32  = DmxData.hsi_cf.s.coarse*257 + (DmxData.hsi_cf.s.fine)*257*FINE;if(d32>65535)temp.hsi_16bit.s = 65535;else temp.hsi_16bit.s = d32;
				HSI_OUT(&temp.hsi_16bit);
				break;
			default :break;
		}
}



