#ifndef _SYS_H
#define _SYS_H

#define uint8 unsigned char
#define int8 char
#define uint16 unsigned short
#define int32 int
#define uint32 unsigned int

extern unsigned long LoopVal;
//��λMHz
extern uint32 FCLK;
extern uint32 HCLK;
extern uint32 PCLK;

//��ʱ����
void delay(unsigned int t);	

//�Ĵ���λ����, ��λ������
//reg:�Ĵ���, x:Ҫ��λ������λ
#define	SET_REG(reg, x)		(reg |=  (1<<(x)))
#define	CLR_REG(reg, x)		(reg &= ~(1<<(x)))

//ϵͳʱ����غ���
void Set_FCLK(uint32 fclk);						//������Ƶʱ��
void Set_ClkDivn(uint32 hDivn, uint32 pDivn);	//����FCLK:HCLK:PCLK����


//���������ʾ��غ���
void Uart0_Init(uint32 baud);					// ���ڳ�ʼ��
void Uart_Printf(const char *fmt, ...);		// ���ڸ�ʽ�����
uint8 Uart_GetByte(void);					// �Ӵ��ڶ�ȡһ���ַ�

//LED�����ʾ����
void Led_Out(int data);

//��ʱ��0��ʼ��
void Timer0Init(void);


#endif
