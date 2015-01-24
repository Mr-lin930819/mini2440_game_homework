#ifndef _ADC_H
#define _ADC_H

// ADCCON�Ĵ���
#define PRESCALE_EN		(1 << 14)
#define PRSCVL(x)		((x) << 6)
#define ADC_START		(1 << 0)

// ADCTSC�Ĵ���
#define UD_SEN			(1 << 8)
#define DOWN_INT		(UD_SEN*0)
#define UP_INT			(UD_SEN*1)
#define YM_SEN			(1 << 7)
#define YM_HIZ			(YM_SEN*0)
#define YM_GND			(YM_SEN*1)
#define YP_SEN			(1 << 6)
#define YP_EXTVLT		(YP_SEN*0)
#define YP_AIN			(YP_SEN*1)
#define XM_SEN			(1 << 5)
#define XM_HIZ			(XM_SEN*0)
#define XM_GND			(XM_SEN*1)
#define XP_SEN			(1 << 4)
#define XP_EXTVLT		(XP_SEN*0)
#define XP_AIN			(XP_SEN*1)
#define XP_PULL_UP		(1 << 3)
#define XP_PULL_UP_EN	(XP_PULL_UP*0)
#define XP_PULL_UP_DIS	(XP_PULL_UP*1)
#define AUTO_PST		(1 << 2)
#define CONVERT_MAN		(AUTO_PST*0)
#define CONVERT_AUTO	(AUTO_PST*1)
#define XP_PST(x)		(x << 0)

#define NOP_MODE		0
#define X_AXIS_MODE		1
#define Y_AXIS_MODE		2
#define WAIT_INT_MODE	3

//���ý���ȴ��ж�ģʽ��XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
//λ[8]Ϊ0��1ʱ�ֱ��ʾ�ȴ�Pen Down�жϻ�Pen Up�ж�

//����"�ȴ��ж�ģʽ"���ȴ�������������
//YP��Ϊģ������, YM�ӵ�
//XP������Ϊģ������, XM����, 
#define WaitDownInt() { ADCTSC = DOWN_INT | XP_PULL_UP_EN | XP_AIN | \
								 XM_HIZ | YP_AIN | YM_GND | \
                                 XP_PST(WAIT_INT_MODE); }

//����"�ȴ��ж�ģʽ"���ȴ����������ɿ� 
//YP��Ϊģ������, YM�ӵ�
//XP������Ϊģ������, XM����, 
#define WaitUpInt()   { ADCTSC = UP_INT | XP_PULL_UP_EN | XP_AIN | \
								 XM_HIZ | YP_AIN | YM_GND | \
								 XP_PST(WAIT_INT_MODE); }

//�����Զ�(����) X/Y������ת��ģʽ
//����X_ADC: YP��Ϊģ������, YM����, XP�ӵ�Դ, XM�ӵ�
//����Y_ADC: YP�ӵ�Դ, YM�ӵ�, XP��Ϊģ������, XM����
#define ModeAutoXY()  { ADCTSC = CONVERT_AUTO | XP_PULL_UP_DIS | \
								 XP_PST(NOP_MODE); }

struct LcdDot
{
	int x;			// Xλ��
	int y;			// Yλ��
};

extern uint32 xData,yData;
extern uint8 AD2LCD;

void ADC_Init(void);
unsigned char Get_AD_XY(void);
unsigned char GetTouch(struct LcdDot *lcdXY);
void Touch_to_LCD(void);
unsigned char Get_Touch( struct LcdDot *lcdXY );

#endif
