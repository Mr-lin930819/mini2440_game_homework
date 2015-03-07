#ifndef _GUI_H
#define _GUI_H

#include "sys.h"
#include "adc.h"
//�õ�����ɫ
#define BLACK (RGB(0x00,0x00,0x00))
#define LIGHTGRAY (RGB(0xC0,0xC0,0xC0))
#define LIGHTBLUE (RGB(0x84,0x70,0xFF))
#define LIGHTGREEN (RGB(0x20,0xB2,0xAA))
#define WHITE (RGB(0xFF,0xFF,0xFF))
#define BLUE (RGB(0x00,0x00,0xFF))
#define RED (RGB(0xFF,0x00,0x00))
#define CYAN (RGB(0x00,0xFF,0xFF))
//����û�ѡ��Ĺ���ģ��
uint8 GetIcon(struct LcdDot *tmpXY);

//���ƴ�ֱֱ��
uint8 GUI_DrawVLine(uint16 Left,uint16 Top,uint16 Bottom,uint16 Color);

//����ˮƽֱ��
uint8 GUI_DrawHLine(uint16 Left,uint16 Top,uint16 Right,uint16 Color);

//���ƾ��ο�
uint8 GUI_DrawRect(uint16 X0, uint16 Y0, uint16 X1, uint16 Y1, uint16 Color);

//����ֱ��
uint8 GUI_DrawLine(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color);

//����ɫ������
void GUI_FillRect(uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint16 Color);

//��ˮ�Ʋ���
void LED_Test(void);

//ƴ�����뷨
void PY_Test(void);

//��ʼ��Ϸ
void PlayGame(int game_level);
//void PlayGame(void);

#endif
