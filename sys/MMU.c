#include <S3C2440.h>
#include "MMU.h"

#define	uchar	unsigned char
#define	ushort	unsigned short
#define	uint	unsigned int
#define	ulong	unsigned long

//����ԭ������
void MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr);


/*
 * �� �� ����MMU_Init
 * ����������MMU��ʼ��, ����MMU, ���е�ַӳ��,
 * �����������
 * �� �� ֵ����
 */
void MMU_Init(void)
{
    int i,j;
	uint domain;
	uint processId;
	uint reg=0;

	__asm
	{
		//MMU_DisableDCache
		mrc p15,0,reg,c1,c0,0
		orr reg,reg,(1<<2)		//��Ccrλ(CP15��C1[2])д0�ɹر�DCaches
		mcr p15,0,reg,c1,c0,0

		//MMU_DisableICache
		mrc p15,0,reg,c1,c0,0
		bic reg,reg,(1<<12)		//��Icrλ(CP15��C1[12])д0�ɹر�ICaches
		mcr p15,0,reg,c1,c0,0
	}

    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
	{
    	for(j=0;j<8;j++)
    	{
			uint val = (i<<26)|(j<<5);    
			//MMU_CleanInvalidateDCacheIndex
			__asm{mcr p15,0,val,c7,c14,2}
		}
	}


	reg = 0;
	__asm
	{
		//MMU_InvalidateICache
		mcr p15,0,reg,c7,c5,0

		//MMU_DisableMMU
		mrc p15,0,reg,c1,c0,0
		bic reg,reg,(1)			//���ƼĴ���C1��0λд0, �ر�MMU
		mcr p15,0,reg,c1,c0,0

		//MMU_InvalidateTLB
		mcr p15,0,reg,c8,c7,0
	}    
    
    //MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr)
    //MMU_SetMTT(0x00000000,0x07f00000,0x00000000,RW_CNB);  //bank0
    MMU_SetMTT(0x00000000, 0x03f00000, (uint)START_PROGRAM, RW_CB);	//bank0
    MMU_SetMTT(0x04000000, 0x07f00000, 0x00000000, RW_NCNB);  	 	//bank0
    MMU_SetMTT(0x08000000, 0x0ff00000, 0x08000000, RW_CNB);  		//bank1
    MMU_SetMTT(0x10000000, 0x17f00000, 0x10000000, RW_NCNB); 		//bank2
    MMU_SetMTT(0x18000000, 0x1ff00000, 0x18000000, RW_NCNB); 		//bank3
    MMU_SetMTT(0x20000000, 0x27f00000, 0x20000000, RW_CB); 		//bank4
    MMU_SetMTT(0x20000000, 0x27f00000, 0x20000000, RW_CNB); 		//bank4 for STRATA Flash
    MMU_SetMTT(0x28000000, 0x2ff00000, 0x28000000, RW_NCNB); 		//bank5

    //30f00000->30100000, 31000000->30200000
    MMU_SetMTT(0x30000000, 0x30100000, 0x30000000, RW_CB);	  		//bank6-1
    MMU_SetMTT(0x30200000, 0x33e00000, 0x30200000, RW_NCNB); 		//bank6-2
    MMU_SetMTT(0x33f00000, 0x33f00000, 0x33f00000, RW_CB);   		//bank6-3

    MMU_SetMTT(0x38000000, 0x3ff00000, 0x38000000, RW_NCNB); 		//bank7
    
    MMU_SetMTT(0x40000000, 0x47f00000, 0x40000000, RW_NCNB); 		//SRAM
    MMU_SetMTT(0x48000000, 0x5af00000, 0x48000000, RW_NCNB); 		//SFR
    MMU_SetMTT(0x5b000000, 0x5b000000, 0x5b000000, RW_NCNB); 		//SFR
    MMU_SetMTT(0x5b100000, 0xfff00000, 0x5b100000, RW_FAULT);		//not used

	domain = 0x55555550 | DOMAIN1_ATTR | DOMAIN0_ATTR;
	processId = 0;
	reg = 0;
	__asm
	{
		//MMU_SetTTBase
		mcr p15,0,TTB_BASE,c2,c0,0		//ҳ���ַ�Ĵ���C2

		//MMU_SetDomain
		mcr p15,0,domain,c3,c0,0		//����ʿ��ƼĴ���C3

		//MMU_SetProcessId
		mcr p15,0,processId,c13,c0,0

		//MMU_EnableAlignFault
		mrc p15,0,reg,c1,c0,0
		orr reg,reg,(1<<1)				//���ƼĴ���C1��1λд1, ��ַ������
		mcr p15,0,reg,c1,c0,0

		//MMU_EnableMMU
		mrc p15,0,reg,c1,c0,0
		orr reg,reg,(1)					//���ƼĴ���C1��0λд1, ����MMU
		mcr p15,0,reg,c1,c0,0

		//MMU_EnableICache
		mrc p15,0,reg,c1,c0,0
		orr reg,reg,(1<<12)			  //��Icrλ(CP15��C1[12])д1������ICaches
		mcr p15,0,reg,c1,c0,0

		//MMU_EnableDCache
		mrc p15,0,reg,c1,c0,0
		orr reg,reg,(1<<2)			 //��Ccrλ(CP15��C1[2])д1������DCaches
		mcr p15,0,reg,c1,c0,0
	}
}


/*
 * �� �� ����MMU_SetMTT
 * ��������������һ��ҳ��
 * ���������vaddrStart--�����ַ��ʼ,
 *			 vaddrEnd--�����ַ����,
 *			 attr--������
 * �� �� ֵ����
 */
void MMU_SetMTT(int vaddrStart, int vaddrEnd, int paddrStart, int attr)
{
    volatile int *pTT;
    volatile int i, nSec;

    pTT = (int *)TTB_BASE + (vaddrStart>>20);
    nSec = (vaddrEnd>>20) - (vaddrStart>>20);

    for(i=0; i<=nSec; i++)
	{
		*pTT++ = (((paddrStart>>20)+i)<<20) | attr;
	}
}    

void MMU_SetAsyncBusMode()
{
	unsigned int reg=0;
	__asm
	{
		swi	0
		mrc	p15,0,reg,c1,c0,0
		orr reg,reg,#0xC0000000
		mcr p15,0,reg,c1,c0,0
		MRS	reg, spsr
		MSR	CPSR_c, reg
	}
}


void MMU_SetFastBusMode()
{
	unsigned int reg=0;
	__asm
	{
		mrc p15,0,reg,c1,c0,0
		bic reg,reg,#0xC0000000
		mcr p15,0,reg,c1,c0,0
		MRS	reg, spsr
		MSR	CPSR_c, reg		
	}
}
