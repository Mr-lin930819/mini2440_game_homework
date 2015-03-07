#include <stdarg.h>
#include <stdio.h>
#include "LCD.h"
#include "sys.h"

uint16 LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];
//打开LCD视频和控制信号的输出
void LCD_VideoON()
{
	SET_BIT(LCDCON1,0);
}
//停止LCD视频和控制信号的输出
void LCD_VideoOFF()
{
	CLR_BIT(LCDCON1,0);
}


//打开LCD电源
void LCD_PowerON()
{
	SET_BIT(LCDCON5,3);
}

//关闭LCD电源
void LCD_PowerOFF()
{
	CLR_BIT(LCDCON5,3);
}

/*******************************************************
 * 函数名称：Lcd_ClearScr
 * 描述说明：LCD清屏或填充指定颜色
 * 输入参数：无
 * 输出参数：无
*******************************************************/ 
void Lcd_ClearScr(uint32 c)
{
	uint32 x,y ;
		
    for( y = 0 ; y < SCR_YSIZE ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE ; x++ )
    	{
			LCD_BUFFER[y][x] = c ;
    	}
    }
}

/*******************************************************
 * 函数名称：Lcd_Init
 * 描述说明：LCD初始化
 * 输入参数：无
 * 输出参数：无
*******************************************************/ 
void Lcd_Init(void)
{
    GPCUP  = 0x00000000;	
	GPCCON = 0xaaaa02a9; 	//VD[7:0]
	 
	GPDUP  = 0x00000000;
	GPDCON = 0xaaaaaaaa; 	//VD[15:8]

	//TFT LCD功能模块初始化
	LCDCON1 = LCDCON1_VAL;
 	LCDCON2 = LCDCON2_VAL;
	LCDCON3 = LCDCON3_VAL;
	LCDCON4 = LCDCON4_VAL;	
    LCDCON5 = LCDCON5_VAL;           

	//LCD 显示缓冲区地址
	LCDSADDR1 = LCD_LCDBANK|LCD_LCDBASEU;
	LCDSADDR2 = LCD_LCDBASEL;
	LCDSADDR3 = LCD_OFFSIZE|LCD_PAGEWIDTH;

	//LCD中断
	LCDINTMSK = LCD_FIWSEL|LCD_INT_FrSyn|LCD_INT_FiCnt;

	LCD_PowerON();  //打开电源
	LCD_VideoON();  //打开视频
}

/*******************************************************
 * 函数名称：PutPixel
 * 描述说明：16Bpp TFT LCD单个像素的显示数据输出
 * 输入参数：(x,y)显示像素的坐标值,c像素显示的颜色信息
 * 输出参数：无
*******************************************************/ 
void PutPixel(uint32 x,uint32 y, uint16 c )
{
	if ( (x < SCR_XSIZE) && (y < SCR_YSIZE) )
		LCD_BUFFER[y][x] = c;
}

/*******************************************************
 * 函数名称：Paint_Bmp
 * 描述说明：在LCD屏幕上指定坐标点画一个指定大小的图片
 * 输入参数：(x,y)图片显示的起始位置, 
 *            w显示图片的宽度, h显示图片的高度
 *            bmp待显示的图片信息 
 * 输出参数：无
*******************************************************/ 
void Paint_Bmp(uint16 x,uint16 y, uint16 w, uint16 h, unsigned char bmp[])
{
	uint16 i,j ;
	uint32 c,p;
	
	for( i= 0; i< h; i++ )
	{
		p= i*w*2 ;
		for( j= 0; j< w; j++ )
		{
    		c = bmp[p+1] | (bmp[p]<<8) ;
			PutPixel(x+j,y+i,c) ;
    		p = p + 2 ;
		}
	}
}

/*******************************************************
 * 函数名称：Paint_Bmp_Part
 * 描述说明：在LCD屏幕上指定坐标点显示原始图像的一部分
 * 输入参数：(x,y)图片在LCD显示的起始位置, 
 *           (w,h)原始图片的宽度、高度
 *			 (x2,y2)显示图像在原始图像中的坐标信息
 *			 (w2,h2)显示图像的宽度、高度
 *            bmp原始图片信息 
 * 输出参数：无
*******************************************************/ 
void Paint_Bmp_Part(uint16 x,uint16 y, uint16 w, uint16 h, 
    uint16 x2,uint16 y2, uint16 w2, uint16 h2,  unsigned char bmp[])
{
	uint16 i,j ;
	uint32 c,p;
	
	for( i= 0; i< h2; i++ )
	{
		p= (i+y2)*w*2+x2*2 ;
		for( j= 0; j<  w2; j++ )
		{
    		c = bmp[p+1] | (bmp[p]<<8) ;
			PutPixel(x+j,y+i,c) ;
    		p = p + 2 ;
		}
	}
}

/*******************************************************
 * 函数名称：Lcd_Test
 * 描述说明：LCD显示测试
 * 输入参数：无
 * 输出参数：无
*******************************************************/
void Lcd_Test()
{
	Lcd_ClearScr(RGB(0,0,255));		// 蓝色
	delay(5000);
	Lcd_ClearScr(RGB(255,0,0));		// 红色
	delay(5000);
	Lcd_ClearScr(RGB(0,255,0));		// 绿色
	delay(5000);
	Lcd_ClearScr(RGB(0,0,0));		// 白色
	delay(5000);
//	Paint_Bmp(0, 0, 240, 320, Loffy1); 	// 图片1
	delay(5000);
//	Paint_Bmp(0, 0, 240, 320, Loffy2);	// 图片2
	delay(5000);
}

/*******************************************************
 * 函数名称：DispHZ_16x16
 * 描述说明：显示16*16点阵汉字
 * 输入参数：(x,y)-- 显示汉字的坐标值
 *           id   -- 显示汉字在ZM_16x16[][32]中的位置
 *           c    -- 显示汉字的颜色
 *           bk_c -- 显示汉字的背景色
 *           is_bk-- 是否显示背景色，1：显示，0：不显示
 * 输出参数：无
*******************************************************/ 
void DispHZ_16x16(uint32 x,uint32 y, uint8 id, uint16 c, uint16 bk_c, uint8 is_bk)
{
	uint16 i,j;
	uint8 *pZM;
	uint16 buf;

	pZM = &ZM_16x16[id][0];
	for( i = 0 ; i < 16 ; i++ )
	{
        buf = pZM[i*2]<<8 | pZM[i*2+1];
        for( j = 0 ; j < 16 ; j++ )
        {
            if( buf & 0x8000 )
            {
                PutPixel(x+j,y+i,c);			// 汉字显示红色
            }
			else
            {
				if(is_bk)						// 是否显示背景色
					PutPixel(x+j,y+i,bk_c);		// 显示背景白色
	
            }
            buf = buf << 1;
        } 
	}
}

/*******************************************************
 * 函数名称：Lcd_DispHZ
 * 描述说明：显示16*16点阵汉字
 * 输入参数：(x,y)-- 显示汉字的坐标值 
 *			 QW   -- 汉字区位码
 *           c    -- 显示汉字的颜色
 *           bk_c -- 显示汉字的背景色
 *           is_bk-- 是否显示背景色，1：显示，0：不显示
 * 输出参数：无
*******************************************************/ 
void Lcd_DispHZ(uint32 x, uint32 y, uint16 QW, uint16 c, uint16 bk_c, uint8 is_bk)
{
	uint16 i,j;
	uint8 *pZM;
	uint16 buf;

	pZM = &__CHS[(((QW >> 8)-1)*94+(QW&0x00FF)-1)*32];
	for( i = 0 ; i < 16 ; i++ )
	{
        buf =  pZM[i*2]<<8 | pZM[i*2+1];
        for( j = 0 ; j < 16 ; j++ )
        {
            if( buf & 0x8000)
            {
                PutPixel(x+j,y+i,c);
            }
			else
            {
                if( is_bk )
                    PutPixel(x+j,y+i,bk_c);
            }
            buf = buf << 1;
        } 
	}
}

/*******************************************************
 * 函数名称：Lcd_DispASCII
 * 描述说明：显示8*16点阵字符
 * 输入参数：(x,y)显示字符的坐标值
 *			 ch   -- 显示的字符
 *           c    -- 显示汉字的颜色
 *           bk_c -- 显示汉字的背景色
 *           is_bk-- 是否显示背景色，1：显示，0：不显示
 * 输出参数：无
*******************************************************/ 
void Lcd_DispASCII(uint32 x, uint32 y, uint8 ch, uint16 c, uint16 bk_c, uint8 is_bk)
{
	unsigned short int i,j;
	unsigned char *pZK,buf;

	pZK = &__VGA[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		buf = pZK[i];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & 0x80 )
            {
                PutPixel(x+j,y+i,c);
            }
						else
            {
                if( is_bk )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }            
            buf = buf << 1;
        }
	}
}

/*******************************************************
 * 函数名称：Lcd_printf
 * 描述说明：在LCD显示字符串
 * 输入参数：(x,y)显示字符串的坐标值,
 *           c    -- 显示汉字的颜色
 *           bk_c -- 显示汉字的背景色
 *           is_bk-- 是否显示背景色，1：显示，0：不显示
 *			 fmt待显示的字符串信息
 * 输出参数：无
*******************************************************/ 
void Lcd_printf(uint32 x, uint32 y, uint16 c, uint16 bk_c, uint8 is_bk, char *fmt,...)
{
    char __LCD_Printf_Buf[256];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;

    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
	 
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n' :
			{			
				break;
			}
			default:
			{
				if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //中文输出
            	{
	                Lcd_DispHZ( x, y, (*pStr-0xA0)<<8|(*(pStr+1)-0xA0), c, bk_c, is_bk);

	                pStr += 2;
	                i += 2;
	                x += 16;
                }
				else               //英文输出
                {
                    Lcd_DispASCII( x, y, *pStr, c, bk_c, is_bk);

					pStr++;
					i++;
                    x += 8;
                }
                break;
			}
		}
        if( i > 256 ) break;
	}   
}
