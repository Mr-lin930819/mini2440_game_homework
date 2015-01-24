/*
 * �� �� ����MMU.h
 * о    Ƭ��S3C2440
 * ��    ��12MHz
 * �� �� �ߣ�����
 * �������ڣ�2010.12.18
 * �� �� �ߣ�
 * �޸����ڣ�
 * �����������ڴ����ԪMMU����ַӳ��
 *			 ʹ��һ��ҳ���Զεķ�ʽ���е�ַӳ��
 *			 һ��ҳ����ʹ��4096������������ʾ4GB�ռ䣬
 *			 ÿ����������Ӧ1MB�����ַ
 *			 ��ʼ��MMU�漰Э����������, ���Ӧ��������������û�ģʽǰ����!!
 *			 ��:
 *			 ;����MMU���е�ַӳ��
 *			 IMPORT MMU_Init
 *			 BL MMU_Init
 */

#ifndef	MMU_H_
#define	MMU_H_

//ҳ���ַ, ��ӦЭ������CP15�ļĴ���C2
//����һ��ҳ��������, ÿ��������4Byte, 
//����һ��ҳ��ռ��16KB�ڴ�ռ� 
#define TTB_BASE			0x33FF8000

//���򱣴����ڴ��е���ʼ��ַ
#define	START_PROGRAM		0x30000000


//��������[11][10]=AP
//�����������[8..5]���Ӧ������ʿ��ƼĴ���(CP15��C3)��ӦλΪ01, 
//��ʹ���������е�APλ����Ȩ�޼��
#define AP_RW		(3<<10) 	//supervisor=RW, user=RW
#define AP_RO		(2<<10) 	//supervisor=RW, user=RO

//��������[8..5]=DOMAIN
#define DOMAIN0			(0<<5)
#define DOMAIN1			(1<<5)

//��������[3][2]=Ctt Btt
//��Ccrλ(CP15��C1[2])д1������DCaches
//��Icrλ(CP15��C1[12])д1������ICaches
#define NCNB		(0<<2)  	//Non-cached, non-buffered
#define NCB         (1<<2)  	//Non-cached buffered
#define CNB			(2<<2)  	//Cached, write-through(WT) 
#define CB			(3<<2)  	//Cached, write-back(WB)

//��������[4]=1, [1][0] = 1 0
#define DESC_SEC	((1<<4)|0x02)

//������������Ȩ��
#define RW_CB			(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB			(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB			(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT		(AP_RW|DOMAIN1|NCNB|DESC_SEC)


//��������ʿ��ƼĴ���(CP15��C3)
//��0ʹ�ö���������AP����Ȩ�޼��
//��1�޷���Ȩ��
#define DOMAIN0_ATTR	(1<<0) 
#define DOMAIN1_ATTR	(0<<2) 


/*
 * �� �� ����MMU_Init
 * ��    �ã�MMU_Init();
 * ��������������MMU, ���е�ַӳ��, 
 *			 �����ַ0x0000000��Ӧ0x30000000, ��ʹ�жϷ���ʱ�ܽ����ж������� 
 */
void MMU_Init(void);

void MMU_SetAsyncBusMode(void);
void MMU_SetFastBusMode(void);
#endif
