#include "includes.h"

DMXData DmxData;

void DMX512_handle(void);

#define FINE		0.1 //10%


//将数据放入缓存
void receiving_dmx_data(void)
{
	u8 len = sizeof(DMXData) ;
	memcpy( &DmxData,(&DMX512_RX_BUF[Sys.Config.dmx.addr]) ,len);
	Sys.dmx_handle = 1;
}
	
void DMXTask(void)
{
	static unsigned char dmx_detect = 0;
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
 
 	
int CCT_RGBWWCW_OUT_8BIT(CCT_RGBWWCW_8BIT *cct_rgb)
{
	RGB rgb;
	COORD rgb_coord,cct_coord,temp_coord;
	char pos;
	int offset;
	float wight = cct_rgb->w_c/255.0;
	pos = Sys.Config.cct.max_pos*cct_rgb->kvn/255;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_8bit(cct_rgb->grn);

	rgb.r = cct_rgb->r/255.0;
	rgb.g = cct_rgb->g/255.0;
	rgb.b = cct_rgb->b/255.0;
	rgb.ww = cct_rgb->ww/255.0;
	rgb.cw = cct_rgb->cw/255.0;
	RGBWWCW_to_coordinate(rgb,&rgb_coord);
	CCTToCoordinate(pos,offset,&cct_coord);
	temp_coord.x = wight*rgb_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*rgb_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,cct_rgb->dim/255.0,0);
	return 0;
}
int CCT_RGBWWCW_OUT_16BIT(CCT_RGBWWCW_16BIT *cct_rgb)
{
	RGB rgb;
	COORD rgb_coord,cct_coord,temp_coord;
	char pos;
	int offset;
	float wight = cct_rgb->w_c/65535.0;
	pos = Sys.Config.cct.max_pos*cct_rgb->kvn/65535;
	pos += 3;
	if(pos > Sys.Config.cct.max_pos)
	pos = Sys.Config.cct.max_pos;
	offset = get_offset_16bit(cct_rgb->grn);

	rgb.r = cct_rgb->r/65535.0;
	rgb.g = cct_rgb->g/65535.0;
	rgb.b = cct_rgb->b/65535.0;
	rgb.ww = cct_rgb->ww/65535.0;
	rgb.cw = cct_rgb->cw/65535.0;
	RGBWWCW_to_coordinate(rgb,&rgb_coord);
	CCTToCoordinate(pos,offset,&cct_coord);
	temp_coord.x = wight*rgb_coord.x + (1-wight)*cct_coord.x;
	temp_coord.y = wight*rgb_coord.y + (1-wight)*cct_coord.y;
	CoordinateOut(&temp_coord,cct_rgb->dim/65535.0,0);
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

RGB panle_ledk;
int RGBWWCW_OUT(RGBWWCW_16BIT *rgbwwcw)
{
	panle_ledk.r = (float)(rgbwwcw->r/65535.0);
	panle_ledk.g = rgbwwcw->g/65535.0;
	panle_ledk.b = rgbwwcw->b/65535.0;
	panle_ledk.ww = rgbwwcw->ww/65535.0;
	panle_ledk.cw = rgbwwcw->cw/65535.0;
	AllLedPowerOut(&panle_ledk,rgbwwcw->dim/65535.0);
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
DMXData temp;
RGBWWCW_16BIT rgbwwcw;
void DMX512_handle(void)
{
	CCT_RGBWWCW_16BIT cct_rgb;
	CCT_16BIT  cct;
	CCT_HSI_16BIT cct_hsi;
	HSI_16BIT hsi;
	unsigned int d32;
		switch(Sys.Config.dmx.mode)
		{
			case DMX_M1:
				CCT_RGBWWCW_OUT_8BIT(&DmxData.cct_rgbwwcw_8bit);
				break;
			case DMX_M2:
				CCT_OUT_8BIT(&DmxData.cct_8bit);			
				break;
			case DMX_M3:
				CCT_HSI_OUT_8BIT(&DmxData.cct_hsi_8bit);
				break;
			case DMX_M4:
				temp.rgbwwcw_16bit.dim = DmxData.rgbwwcw_8bit.dim*65535/255;//转换成16位
				temp.rgbwwcw_16bit.r = DmxData.rgbwwcw_8bit.r*65535/255;//转换成16位
				temp.rgbwwcw_16bit.g = DmxData.rgbwwcw_8bit.g*65535/255;//转换成16位
				temp.rgbwwcw_16bit.b = DmxData.rgbwwcw_8bit.b*65535/255;//转换成16位
				temp.rgbwwcw_16bit.ww = DmxData.rgbwwcw_8bit.ww*65535/255;//转换成16位
				temp.rgbwwcw_16bit.cw = DmxData.rgbwwcw_8bit.cw*65535/255;//转换成16位	
				RGBWWCW_OUT(&temp.rgbwwcw_16bit);
				break;
			case DMX_M5:
				hsi.dim = DmxData.hsi_8bit.dim*65535/255;
				hsi.h = DmxData.hsi_8bit.h*65535/255;
				hsi.s = DmxData.hsi_8bit.s*65535/255;
				HSI_OUT(&hsi);
				break;
			case DMX_M6:
				cct_rgb.dim = DmxData.cct_rgbwwcw_16bit.dim >> 8 | DmxData.cct_rgbwwcw_16bit.dim << 8;
				cct_rgb.kvn = DmxData.cct_rgbwwcw_16bit.kvn >> 8 | DmxData.cct_rgbwwcw_16bit.kvn << 8;
				cct_rgb.grn = DmxData.cct_rgbwwcw_16bit.grn >> 8 | DmxData.cct_rgbwwcw_16bit.grn << 8;
				cct_rgb.w_c = DmxData.cct_rgbwwcw_16bit.w_c >> 8 | DmxData.cct_rgbwwcw_16bit.w_c<< 8;
				cct_rgb.r = DmxData.cct_rgbwwcw_16bit.r >> 8 | DmxData.cct_rgbwwcw_16bit.r << 8;
				cct_rgb.g = DmxData.cct_rgbwwcw_16bit.g >> 8 | DmxData.cct_rgbwwcw_16bit.g << 8;
				cct_rgb.b= DmxData.cct_rgbwwcw_16bit.b >> 8 | DmxData.cct_rgbwwcw_16bit.b << 8;
				cct_rgb.ww = DmxData.cct_rgbwwcw_16bit.ww >> 8 | DmxData.cct_rgbwwcw_16bit.ww << 8;
				cct_rgb.cw= DmxData.cct_rgbwwcw_16bit.cw >> 8 | DmxData.cct_rgbwwcw_16bit.cw << 8;
				CCT_RGBWWCW_OUT_16BIT(&cct_rgb);
				break;
			case DMX_M7:
				cct.dim = DmxData.cct_16bit.dim >> 8 | DmxData.cct_16bit.dim << 8;
				cct.grn = DmxData.cct_16bit.grn >> 8 | DmxData.cct_16bit.grn << 8;
				cct.kvn = DmxData.cct_16bit.kvn >> 8 | DmxData.cct_16bit.kvn << 8;
				CCT_OUT_16BIT(&cct);
				break;
			case DMX_M8:
				cct_hsi.dim = DmxData.cct_hsi_16bit.dim >> 8 | DmxData.cct_hsi_16bit.dim << 8;
				cct_hsi.kvn = DmxData.cct_hsi_16bit.kvn >> 8 | DmxData.cct_hsi_16bit.kvn << 8;
				cct_hsi.grn = DmxData.cct_hsi_16bit.grn >> 8 | DmxData.cct_hsi_16bit.grn << 8;
				cct_hsi.w_c = DmxData.cct_hsi_16bit.w_c >> 8 | DmxData.cct_hsi_16bit.w_c<< 8;
				cct_hsi.h = DmxData.cct_hsi_16bit.h >> 8 | DmxData.cct_hsi_16bit.h << 8;
				cct_hsi.s = DmxData.cct_hsi_16bit.s >> 8 | DmxData.cct_hsi_16bit.s << 8;
				CCT_HSI_OUT_16BIT(&cct_hsi);
				break;
			case DMX_M9:
				rgbwwcw.dim = DmxData.rgbwwcw_16bit.dim >> 8 | DmxData.rgbwwcw_16bit.dim << 8;
				rgbwwcw.r = DmxData.rgbwwcw_16bit.r >> 8 | DmxData.rgbwwcw_16bit.r << 8;
				rgbwwcw.g = DmxData.rgbwwcw_16bit.g >> 8 | DmxData.rgbwwcw_16bit.g << 8;
				rgbwwcw.b= DmxData.rgbwwcw_16bit.b >> 8 | DmxData.rgbwwcw_16bit.b << 8;
				rgbwwcw.ww = DmxData.rgbwwcw_16bit.ww >> 8 | DmxData.rgbwwcw_16bit.ww << 8;
				rgbwwcw.cw= DmxData.rgbwwcw_16bit.cw >> 8 | DmxData.rgbwwcw_16bit.cw << 8;	
				RGBWWCW_OUT(&rgbwwcw);
				//RGBWWCW_OUT(&DmxData.rgbwwcw_16bit);
			break;
			case DMX_M10:
				hsi.dim = DmxData.hsi_16bit.dim>>8 | DmxData.hsi_16bit.dim<<8;
				hsi.h = DmxData.hsi_16bit.h>>8 | DmxData.hsi_16bit.h<<8;
				hsi.s = DmxData.hsi_16bit.s>>8 | DmxData.hsi_16bit.s<<8;
				HSI_OUT(&hsi);
				break;
			case DMX_M11://CCT_RGBWWCW_CF
				temp.cct_rgbwwcw_8bit.dim = DmxData.cct_rgbwwcw_cf.dim.coarse + (DmxData.cct_rgbwwcw_cf.dim.fine)*FINE;if(temp.cct_rgbwwcw_8bit.dim<DmxData.cct_rgbwwcw_cf.dim.coarse)temp.cct_rgbwwcw_8bit.dim = 255;
				temp.cct_rgbwwcw_8bit.kvn = DmxData.cct_rgbwwcw_cf.kvn.coarse + (DmxData.cct_rgbwwcw_cf.kvn.fine)*FINE;if(temp.cct_rgbwwcw_8bit.kvn<DmxData.cct_rgbwwcw_cf.kvn.coarse)temp.cct_rgbwwcw_8bit.kvn= 255;
				temp.cct_rgbwwcw_8bit.grn = DmxData.cct_rgbwwcw_cf.grn;
				temp.cct_rgbwwcw_8bit.w_c = DmxData.cct_rgbwwcw_cf.w_c;
				temp.cct_rgbwwcw_8bit.r = DmxData.cct_rgbwwcw_cf.r.coarse + (DmxData.cct_rgbwwcw_cf.r.fine)*FINE;if(temp.cct_rgbwwcw_8bit.r<DmxData.cct_rgbwwcw_cf.r.coarse)temp.cct_rgbwwcw_8bit.r = 255;
				temp.cct_rgbwwcw_8bit.g = DmxData.cct_rgbwwcw_cf.g.coarse + (DmxData.cct_rgbwwcw_cf.g.fine)*FINE;if(temp.cct_rgbwwcw_8bit.g<DmxData.cct_rgbwwcw_cf.g.coarse)temp.cct_rgbwwcw_8bit.g = 255;
				temp.cct_rgbwwcw_8bit.b = DmxData.cct_rgbwwcw_cf.b.coarse + (DmxData.cct_rgbwwcw_cf.b.fine)*FINE;if(temp.cct_rgbwwcw_8bit.b<DmxData.cct_rgbwwcw_cf.b.coarse)temp.cct_rgbwwcw_8bit.b = 255;
				temp.cct_rgbwwcw_8bit.ww = DmxData.cct_rgbwwcw_cf.ww.coarse + (DmxData.cct_rgbwwcw_cf.ww.fine)*FINE;if(temp.cct_rgbwwcw_8bit.ww<DmxData.cct_rgbwwcw_cf.ww.coarse)temp.cct_rgbwwcw_8bit.ww = 255;
				temp.cct_rgbwwcw_8bit.cw = DmxData.cct_rgbwwcw_cf.cw.coarse + (DmxData.cct_rgbwwcw_cf.cw.fine)*FINE;if(temp.cct_rgbwwcw_8bit.cw<DmxData.cct_rgbwwcw_cf.cw.coarse)temp.cct_rgbwwcw_8bit.cw = 255;	
				CCT_RGBWWCW_OUT_8BIT(&temp.cct_rgbwwcw_8bit);
				break;
			case DMX_M12://CCT__CF
				temp.cct_8bit.dim = DmxData.cct_rgbwwcw_cf.dim.coarse + (DmxData.cct_rgbwwcw_cf.dim.fine)*FINE;if(temp.cct_rgbwwcw_8bit.dim<DmxData.cct_rgbwwcw_cf.dim.coarse)temp.cct_rgbwwcw_8bit.dim = 255;
				temp.cct_8bit.kvn = DmxData.cct_rgbwwcw_cf.kvn.coarse + (DmxData.cct_rgbwwcw_cf.kvn.fine)*FINE;if(temp.cct_rgbwwcw_8bit.kvn<DmxData.cct_rgbwwcw_cf.kvn.coarse)temp.cct_rgbwwcw_8bit.kvn= 255;
				temp.cct_8bit.grn = DmxData.cct_rgbwwcw_cf.grn;
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
			case DMX_M14://RGBWWCW_CF
				d32 = DmxData.rgbwwcw_cf.dim.coarse*257 + (DmxData.rgbwwcw_cf.dim.fine)*257*FINE;if(d32>65535) temp.rgbwwcw_16bit.dim = 65535;else temp.rgbwwcw_16bit.dim = d32;
				d32 = DmxData.rgbwwcw_cf.r.coarse*257 + (DmxData.rgbwwcw_cf.r.fine)*257*FINE;if(d32>65535) temp.rgbwwcw_16bit.r = 65535;else temp.rgbwwcw_16bit.r = d32;
				d32 = DmxData.rgbwwcw_cf.g.coarse*257 + (DmxData.rgbwwcw_cf.g.fine)*257*FINE;if(d32>65535)temp.rgbwwcw_16bit.g = 65535;else temp.rgbwwcw_16bit.g = d32;
				d32 = DmxData.rgbwwcw_cf.b.coarse*257 + (DmxData.rgbwwcw_cf.b.fine)*257*FINE;if(d32>65535)temp.rgbwwcw_16bit.b = 65535;else temp.rgbwwcw_16bit.b = d32;
				d32 = DmxData.rgbwwcw_cf.ww.coarse*257 + (DmxData.rgbwwcw_cf.ww.fine)*257*FINE;if(d32>65535)temp.rgbwwcw_16bit.ww = 65535;else temp.rgbwwcw_16bit.ww = d32;
				d32 = DmxData.rgbwwcw_cf.cw.coarse*257 + (DmxData.rgbwwcw_cf.cw.fine)*257*FINE;if(d32>65535)temp.rgbwwcw_16bit.cw = 65535;else temp.rgbwwcw_16bit.cw = d32;
				RGBWWCW_OUT(&temp.rgbwwcw_16bit);
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



