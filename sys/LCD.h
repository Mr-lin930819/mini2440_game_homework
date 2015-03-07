#ifndef _LCD_H
#define _LCD_H

#include <s3c2440.h>
#include "sys.h"


//RGB:5:6:5?????
#define RGB(R,G,B) (  (((unsigned short)(R >> 3))<<11)\
					| (((unsigned short)(G >> 2))<<5)\
					| (((unsigned short)(B >> 3))<<0))

#define LCD_T35

//---------------------------------------------
#ifdef LCD_W35
#define LCD_WIDTH  320
#define LCD_HEIGHT 240

#define LCD_VBPD   (9)		//VBPD
#define LCD_VFPD   (3)		//VFPD
#define LCD_VSPW   (0)		//VSPW

#define LCD_HBPD   (43)		//HBPD
#define LCD_HFPD   (3)		//HFPD
#define LCD_HSPW   (0)		//HSPW

//---------------------------------------------
#elif defined LCD_X35
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

#define LCD_VBPD   (0)		//VBPD
#define LCD_VFPD   (4)		//VFPD
#define LCD_VSPW   (9)		//VSPW

#define LCD_HBPD   (19)		//HBPD
#define LCD_HFPD   (0)		//HFPD
#define LCD_HSPW   (4)		//HSPW

//---------------------------------------------
#elif defined LCD_T35
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

#define LCD_VBPD   (1)		//VBPD
#define LCD_VFPD   (4)		//VFPD
#define LCD_VSPW   (1)		//VSPW

#define LCD_HBPD   (25)		//HBPD
#define LCD_HFPD   (0)		//HFPD
#define LCD_HSPW   (4)		//HSPW

#endif

// ??????
#define SCR_XSIZE LCD_WIDTH
#define SCR_YSIZE LCD_HEIGHT

//?????????0
#define CLR_BIT(iVariant,iBit) iVariant &= ~(1<<iBit)

//??????????
#define SET_BIT(iVariant,iBit) iVariant |= (1<<iBit)

//=================LCDCON1===================
/* CLKVAL [17:8] Determine the rates of VCLK and CLKVAL[9:0].
	TFT: VCLK = HCLK / [(CLKVAL+1) x 2]    ( CLKVAL >= 0 ) */
#define LCD_CLKVAL 0x8

/* PNRMODE [6:5] Select the display mode.
	00 = 4-bit dual scan display mode (STN)
	01 = 4-bit single scan display mode (STN)
	10 = 8-bit single scan display mode (STN)
	11 = TFT LCD panel */
#define LCD_PNRMODE 0x3

/* BPPMODE [4:1] Select the BPP (Bits Per Pixel) mode.
	0000 = 1 bpp for STN, Monochrome mode
	0001 = 2 bpp for STN, 4-level gray mode
	0010 = 4 bpp for STN, 16-level gray mode
	0011 = 8 bpp for STN, color mode (256 color)
	0100 = packed 12 bpp for STN, color mode (4096 color)
	0101 = unpacked 12 bpp for STN, color mode (4096 color)
	0110 = 16 bpp for STN, color mode (4096 color)
	1000 = 1 bpp for TFT
	1001 = 2 bpp for TFT
	1010 = 4 bpp for TFT
	1011 = 8 bpp for TFT
	1100 = 16 bpp for TFT
	1101 = 24 bpp for TFT */
#define LCD_BPPMODE 0xC

/* ENVID [0] LCD video output and the logic enable/disable.
	0 = Disable the video output and the LCD control signal.
	1 = Enable the video output and the LCD control signal. */
#define LCD_ENVID 0

#define LCDCON1_VAL ( (LCD_CLKVAL<<8)|(LCD_PNRMODE<<5)|(LCD_BPPMODE<<1)|(LCD_ENVID<<0) )


//=================LCDCON2===================
/*VBPD [31:24] TFT: Vertical back porch
  LINEVAL [23:14] TFT: These bits determine the vertical size of LCD panel.
  VFPD [13:6] TFT: Vertical front porch
  VSPW [5:0] TFT: Vertical sync pulse width*/

#define LCDCON2_VAL ( (LCD_VBPD<<24)|((LCD_HEIGHT-1)<<14)|(LCD_VFPD<<6)|(LCD_VSPW<<0) )


//=================LCDCON3===================
/*HBPD (TFT) [25:19] TFT: Horizontal back porch
  HOZVAL [18:8] TFT: These bits determine the horizontal size of LCD panel.
  HFPD (TFT) [7:0] TFT: Horizontal front porch*/

#define LCDCON3_VAL ( (LCD_HBPD<<19)|((LCD_WIDTH-1)<<8)|(LCD_HFPD<<0) )

 
//=================LCDCON4===================
//HSPW(TFT) [7:0] TFT: Horizontal sync pulse width

#define LCDCON4_VAL (LCD_HSPW<<0)


//=================LCDCON5===================
//BPP24BL [12] TFT: This bit determines the order of 24 bpp video memory.
//0 = LSB valid                         1 = MSB Valid
#define LCD_BPP24BL   (0)

//FRM565 [11] TFT: This bit selects the format of 16 bpp output video data.
//0 = 5:5:5:1 Format                 1 = 5:6:5 Format
#define LCD_FRM565    (1)

//INVVCLK [10] TFT: This bit controls the polarity of the VCLK active edge.
//0 = The video data is fetched at VCLK falling edge
//1 = The video data is fetched at VCLK rising edge
#define LCD_INVVCLK   (0)

//INVVLINE [9] TFT: This bit indicates the VLINE/HSYNC pulse polarity.
//0 = Normal                      1 = Inverted
#define LCD_INVVLINE  (1)

//INVVFRAME [8] TFT: This bit indicates the VFRAME/VSYNC pulse polarity.
//0 = Normal                             1 = Inverted
#define LCD_INVVFRAME (1)

//INVVD [7] TFT: This bit indicates the VD (video data) pulse polarity.
//0 = Normal          1 = VD is inverted.
#define LCD_INVVD     (0)

//INVVDEN [6] TFT: This bit indicates the VDEN signal polarity.
//0 = normal              1 = inverted
#define LCD_INVVDEN   (1)

//INVPWREN [5] TFT: This bit indicates the PWREN signal polarity.
//0 = normal                1 = inverted
#define LCD_INVPWREN  (0)

//INVLEND [4] TFT: This bit indicates the LEND signal polarity.
//0 = normal               1 = inverted
#define LCD_INVLEND   (0)

//PWREN [3] TFT: LCD_PWREN output signal enable/disable.
//0 = Disable PWREN signal 1 = Enable PWREN signal
#define LCD_PWREN     (1)

//ENLEND [2] TFT: LEND output signal enable/disable.
//0 = Disable LEND signal  1 = Enable LEND signal
#define LCD_ENLEND    (0)

//BSWP [1] TFT: Byte swap control bit.
//0 = Swap Disable            1 = Swap Enable
#define LCD_BSWP      (0)

//HWSWP [0] TFT: Half-Word swap control bit.
//0 = Swap Disable            1 = Swap Enable
#define LCD_HWSWP     (1)

#define LCDCON5_VAL ( (LCD_BPP24BL<<12)|(LCD_FRM565<<11)|(LCD_INVVCLK<<10)|(LCD_INVVLINE<<9) \
					|(LCD_INVVFRAME<<8)|(LCD_INVVD<<7)|(LCD_INVVDEN<<6)|(LCD_INVPWREN<<5) \
					|(LCD_INVLEND<<4)|(LCD_PWREN<<3)|(LCD_ENLEND<<2)|(LCD_BSWP<<1)|(LCD_HWSWP<<0) )


//=================LCDSADDR1===================
//LCDBANK [29:21]
//These bits indicate A[30:22] of the bank location for the video buffer in the system memory.
#define LCD_LCDBANK   (((unsigned int)LCD_BUFFER>>22)<<21)

//LCDBASEU [20:0]
//For single-scan LCD : These bits indicate A[21:1] of the start address of the LCD frame buffer.
#define LCD_LCDBASEU  (((unsigned int)LCD_BUFFER&0x3FFFFF)>>1)


//=================LCDSADDR2===================
//LCDBASEL [20:0]
//For single scan LCD: These bits indicate A[21:1] of the end address of the LCD frame buffer.
//LCDBASEL = ((the frame end address) >>1) + 1
//         = LCDBASEU + (PAGEWIDTH+OFFSIZE) x (LINEVAL+1)
//#define LCD_LCDBASEL (LCD_LCDBASEU+LCD_WIDTH*LCD_HEIGHT)
#define LCD_LCDBASEL ( (((unsigned int)LCD_BUFFER+LCD_WIDTH*LCD_HEIGHT*2)&0x3FFFFF)>>1 )


//=================LCDSADDR3===================
//OFFSIZE [21:11]
//Virtual screen offset size (the number of half words).
#define LCD_OFFSIZE    (0)

//PAGEWIDTH [10:0]
//Virtual screen page width (the number of half words).
#define LCD_PAGEWIDTH  (LCD_WIDTH)					


//=================LCDINTMSK===================
//FIWSEL [2] Determine the trigger level of LCD FIFO.
//0 = 4 words                                     1 = 8 words
#define LCD_FIWSEL    (0)

//INT_FrSyn [1] Mask LCD frame synchronized interrupt.
//0 = The interrupt service is available.
//1 = The interrupt service is masked.
#define LCD_INT_FrSyn (0)

//INT_FiCnt [0] Mask LCD FIFO interrupt.
//0 = The interrupt service is available.
//1 = The interrupt service is masked.
#define LCD_INT_FiCnt (0)

// ?????????
extern unsigned char __CHS[];
extern unsigned char __VGA[];
extern unsigned char ZM_16x16[][32];

// BMP图片
//extern unsigned char bg[];
//extern unsigned char main_img[];
//extern unsigned char Loffy1[];
//extern unsigned char Loffy2[];

extern unsigned char game_scene_ui[];
extern unsigned char game_level_bg[];		
extern unsigned char game_pass[];

extern unsigned char t_bmp_1[];
extern unsigned char t_bmp_2[];			
extern unsigned char t_bmp_3[];		
extern unsigned char t_bmp_4[];		
extern unsigned char t_bmp_5[];		
extern unsigned char t_bmp_6[];		
extern unsigned char t_bmp_7[];		
extern unsigned char t_bmp_8[];		
extern unsigned char xp_1[];		
extern unsigned char xp_2[];		


void PutPixel(uint32 x,uint32 y, uint16 c );
void Lcd_ClearScr(uint32 c);	//LCD清屏或填充指定颜色
void Lcd_Init(void);	// Lcd初始化
//void Lcd_Test(void);	// LCD显示测试
//void Fjnu_Run(void);	// FJNU图像滚动显示
//void Fan_Run(void);
//void Net_Run(void);
void Lcd_printf(uint32 x, uint32 y, uint16 c, uint16 bk_c, uint8 is_bk, char *fmt,...);
void Paint_Bmp(uint16 x,uint16 y, uint16 w, uint16 h, unsigned char bmp[]);

#endif
