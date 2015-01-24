#include <s3c2440.h>
#include "mmu.h"
#include "sys.h"

//��λMHz
uint32 FCLK = 0;
uint32 HCLK = 0;
uint32 PCLK = 0;


/*******************************************************
 * �������ƣ�Set_FCLK
 * ����˵����������Ƶʱ��--FCLK��������Ӿ���Ϊ12MHz
 			 FCLK = (2 * m * Fin) / (p * 2^S)
 * ���������fclk����ֵ��200��300��400��440
			 ��Ӧ���Ƶ�ʣ�200MHz��300MHz��400MHz��440MHz
 * �����������
*******************************************************/ 
void Set_FCLK(uint32 fclk)
{
	switch (fclk)
	{
		case 200:
			FCLK = 200;
			MPLLCON = (92<<12)|(4<<4)|(1);		//(2*(92+8)*12) / ((4+2)*2) = 200MHz
			break;
		case 300:
			FCLK = 300;
			MPLLCON = (67<<12)|(1<<4)|(1);		//(2*(67+8)*12) / ((1+2)*2) = 300MHz
			break;
		case 400:
			FCLK = 400;
			MPLLCON = (92<<12)|(1<<4)|(1);		//(2*(92+8)*12) / ((1+2)*2) = 400MHz
			break;
		case 440:
			FCLK = 440;
			MPLLCON = (102<<12)|(1<<4)|(1);		//(2*(102+8)*12) / ((1+2)*2) = 440MHz			
			break;
		default:
			FCLK = 400;
			MPLLCON = (92<<12)|(1<<4)|(1);		//(2*(92+8)*12) / ((1+2)*2) = 400MHz
			break;
	}
}

 
/*******************************************************
 * �������ƣ�Set_ClkDivn
 * ����˵��������FCLK:HCLK:PCLK����
 * ���������hDivn��11--HCLK:FCLK=1:1, 12--HCLK:FCLK=1:2,
 *					13--HCLK:FCLK=1:3, 14--HCLK:FCLK=1:4,
 *					16--HCLK:FCLK=1:6, 12--HCLK:FCLK=1:8
 *			 pDivn: 11--PCLK:HCLK=1:1, 12--PCLK:HCLK=1:2
 * �����������
*******************************************************/ 
void Set_ClkDivn(uint32 hDivn, uint32 pDivn) 
{
	uint32 hDivnVal, pDivnVal;

	// ����HCLK��FCLK����
	switch (hDivn)
	{
		case 11:					//HCLK:FCLK = 1:1
			HCLK = FCLK;
			hDivnVal = 0;
			break;
		case 12:					//HCLK:FCLK = 1:2
			HCLK = FCLK / 2;
			hDivnVal = 1;
			break;
		case 13:					//HCLK:FCLK = 1:3
			CAMDIVN &= ~(1<<8);		//HCLK3_HALF = 0
			HCLK = FCLK / 3;
			hDivnVal = 3;
			break;
		case 14:					//HCLK:FCLK = 1:4
			CAMDIVN &= ~(1<<9);		//HCLK4_HALF = 0
			HCLK = FCLK / 4;
			hDivnVal = 2;
			break;
		case 16:					//HCLK:FCLK = 1:6
			CAMDIVN |= (1<<8);		//HCLK3_HALF = 1
			HCLK = FCLK / 6;
			hDivnVal = 3;
			break;
		case 18:					//HCLK:FCLK = 1:8
			CAMDIVN |= (1<<9);		//HCLK4_HALF = 1
			HCLK = FCLK / 8;
			hDivnVal = 2;
			break;

		default:					//HCLK:FCLK = 1:1
			HCLK = FCLK;
			hDivnVal = 0;
			break;
	}

	// ����HCLK��PCLK����
	switch (pDivn)
	{
		case 11:			 		//PCLK:HCLK = 1:1
			PCLK = HCLK;
			pDivnVal = 0;
			break;
		case 12:			 		//PCLK:HCLK = 1:2
			PCLK = HCLK / 2;
			pDivnVal = 1;
			break;
		default:			 		//PCLK:HCLK = 1:1
			PCLK = HCLK;
			pDivnVal = 0;
			break;
	}
	
	CLKDIVN = (hDivnVal<<1) | (pDivnVal);

	//���hDivnVal!=0��CPU������ģʽ�ӡ���������ģʽ����Ϊ���첽����ģʽ�� 
    if(hDivnVal!=0)
        MMU_SetAsyncBusMode();
    else 
        MMU_SetFastBusMode();
}
