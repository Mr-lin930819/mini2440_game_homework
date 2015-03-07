#include <stdarg.h>
#include <stdio.h>
#include "LCD.h"
#include "sys.h"

uint16 LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];
//��LCD��Ƶ�Ϳ����źŵ����
void LCD_VideoON()
{
	SET_BIT(LCDCON1,0);
}
//ֹͣLCD��Ƶ�Ϳ����źŵ����
void LCD_VideoOFF()
{
	CLR_BIT(LCDCON1,0);
}


//��LCD��Դ
void LCD_PowerON()
{
	SET_BIT(LCDCON5,3);
}

//�ر�LCD��Դ
void LCD_PowerOFF()
{
	CLR_BIT(LCDCON5,3);
}

/*******************************************************
 * �������ƣ�Lcd_ClearScr
 * ����˵����LCD���������ָ����ɫ
 * �����������
 * �����������
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
 * �������ƣ�Lcd_Init
 * ����˵����LCD��ʼ��
 * �����������
 * �����������
*******************************************************/ 
void Lcd_Init(void)
{
    GPCUP  = 0x00000000;	
	GPCCON = 0xaaaa02a9; 	//VD[7:0]
	 
	GPDUP  = 0x00000000;
	GPDCON = 0xaaaaaaaa; 	//VD[15:8]

	//TFT LCD����ģ���ʼ��
	LCDCON1 = LCDCON1_VAL;
 	LCDCON2 = LCDCON2_VAL;
	LCDCON3 = LCDCON3_VAL;
	LCDCON4 = LCDCON4_VAL;	
    LCDCON5 = LCDCON5_VAL;           

	//LCD ��ʾ��������ַ
	LCDSADDR1 = LCD_LCDBANK|LCD_LCDBASEU;
	LCDSADDR2 = LCD_LCDBASEL;
	LCDSADDR3 = LCD_OFFSIZE|LCD_PAGEWIDTH;

	//LCD�ж�
	LCDINTMSK = LCD_FIWSEL|LCD_INT_FrSyn|LCD_INT_FiCnt;

	LCD_PowerON();  //�򿪵�Դ
	LCD_VideoON();  //����Ƶ
}

/*******************************************************
 * �������ƣ�PutPixel
 * ����˵����16Bpp TFT LCD�������ص���ʾ�������
 * ���������(x,y)��ʾ���ص�����ֵ,c������ʾ����ɫ��Ϣ
 * �����������
*******************************************************/ 
void PutPixel(uint32 x,uint32 y, uint16 c )
{
	if ( (x < SCR_XSIZE) && (y < SCR_YSIZE) )
		LCD_BUFFER[y][x] = c;
}

/*******************************************************
 * �������ƣ�Paint_Bmp
 * ����˵������LCD��Ļ��ָ������㻭һ��ָ����С��ͼƬ
 * ���������(x,y)ͼƬ��ʾ����ʼλ��, 
 *            w��ʾͼƬ�Ŀ��, h��ʾͼƬ�ĸ߶�
 *            bmp����ʾ��ͼƬ��Ϣ 
 * �����������
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
 * �������ƣ�Paint_Bmp_Part
 * ����˵������LCD��Ļ��ָ���������ʾԭʼͼ���һ����
 * ���������(x,y)ͼƬ��LCD��ʾ����ʼλ��, 
 *           (w,h)ԭʼͼƬ�Ŀ�ȡ��߶�
 *			 (x2,y2)��ʾͼ����ԭʼͼ���е�������Ϣ
 *			 (w2,h2)��ʾͼ��Ŀ�ȡ��߶�
 *            bmpԭʼͼƬ��Ϣ 
 * �����������
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
 * �������ƣ�Lcd_Test
 * ����˵����LCD��ʾ����
 * �����������
 * �����������
*******************************************************/
void Lcd_Test()
{
	Lcd_ClearScr(RGB(0,0,255));		// ��ɫ
	delay(5000);
	Lcd_ClearScr(RGB(255,0,0));		// ��ɫ
	delay(5000);
	Lcd_ClearScr(RGB(0,255,0));		// ��ɫ
	delay(5000);
	Lcd_ClearScr(RGB(0,0,0));		// ��ɫ
	delay(5000);
//	Paint_Bmp(0, 0, 240, 320, Loffy1); 	// ͼƬ1
	delay(5000);
//	Paint_Bmp(0, 0, 240, 320, Loffy2);	// ͼƬ2
	delay(5000);
}

/*******************************************************
 * �������ƣ�DispHZ_16x16
 * ����˵������ʾ16*16������
 * ���������(x,y)-- ��ʾ���ֵ�����ֵ
 *           id   -- ��ʾ������ZM_16x16[][32]�е�λ��
 *           c    -- ��ʾ���ֵ���ɫ
 *           bk_c -- ��ʾ���ֵı���ɫ
 *           is_bk-- �Ƿ���ʾ����ɫ��1����ʾ��0������ʾ
 * �����������
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
                PutPixel(x+j,y+i,c);			// ������ʾ��ɫ
            }
			else
            {
				if(is_bk)						// �Ƿ���ʾ����ɫ
					PutPixel(x+j,y+i,bk_c);		// ��ʾ������ɫ
	
            }
            buf = buf << 1;
        } 
	}
}

/*******************************************************
 * �������ƣ�Lcd_DispHZ
 * ����˵������ʾ16*16������
 * ���������(x,y)-- ��ʾ���ֵ�����ֵ 
 *			 QW   -- ������λ��
 *           c    -- ��ʾ���ֵ���ɫ
 *           bk_c -- ��ʾ���ֵı���ɫ
 *           is_bk-- �Ƿ���ʾ����ɫ��1����ʾ��0������ʾ
 * �����������
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
 * �������ƣ�Lcd_DispASCII
 * ����˵������ʾ8*16�����ַ�
 * ���������(x,y)��ʾ�ַ�������ֵ
 *			 ch   -- ��ʾ���ַ�
 *           c    -- ��ʾ���ֵ���ɫ
 *           bk_c -- ��ʾ���ֵı���ɫ
 *           is_bk-- �Ƿ���ʾ����ɫ��1����ʾ��0������ʾ
 * �����������
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
 * �������ƣ�Lcd_printf
 * ����˵������LCD��ʾ�ַ���
 * ���������(x,y)��ʾ�ַ���������ֵ,
 *           c    -- ��ʾ���ֵ���ɫ
 *           bk_c -- ��ʾ���ֵı���ɫ
 *           is_bk-- �Ƿ���ʾ����ɫ��1����ʾ��0������ʾ
 *			 fmt����ʾ���ַ�����Ϣ
 * �����������
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
				if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //�������
            	{
	                Lcd_DispHZ( x, y, (*pStr-0xA0)<<8|(*(pStr+1)-0xA0), c, bk_c, is_bk);

	                pStr += 2;
	                i += 2;
	                x += 16;
                }
				else               //Ӣ�����
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
