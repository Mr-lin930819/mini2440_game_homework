/*
 * 文 件 名：MMU.h
 * 芯    片：S3C2440
 * 晶    振：12MHz
 * 创 建 者：冷月
 * 创建日期：2010.12.18
 * 修 改 者：
 * 修改日期：
 * 功能描述：内存管理单元MMU，地址映射
 *			 使用一级页表，以段的方式进行地址映射
 *			 一级页表中使用4096个描述符来表示4GB空间，
 *			 每个描述符对应1MB物理地址
 *			 初始化MMU涉及协处理器操作, 因此应在启动代码进入用户模式前调用!!
 *			 如:
 *			 ;启动MMU进行地址映射
 *			 IMPORT MMU_Init
 *			 BL MMU_Init
 */

#ifndef	MMU_H_
#define	MMU_H_

//页表基址, 对应协处理器CP15的寄存器C2
//保存一级页表描述符, 每个描述符4Byte, 
//所以一级页表占用16KB内存空间 
#define TTB_BASE			0x33FF8000

//程序保存在内存中的起始地址
#define	START_PROGRAM		0x30000000


//段描述符[11][10]=AP
//如果段描述符[8..5]域对应的域访问控制寄存器(CP15的C3)对应位为01, 
//则使用描述符中的AP位进行权限检查
#define AP_RW		(3<<10) 	//supervisor=RW, user=RW
#define AP_RO		(2<<10) 	//supervisor=RW, user=RO

//段描述符[8..5]=DOMAIN
#define DOMAIN0			(0<<5)
#define DOMAIN1			(1<<5)

//段描述符[3][2]=Ctt Btt
//往Ccr位(CP15的C1[2])写1可启动DCaches
//往Icr位(CP15的C1[12])写1可启动ICaches
#define NCNB		(0<<2)  	//Non-cached, non-buffered
#define NCB         (1<<2)  	//Non-cached buffered
#define CNB			(2<<2)  	//Cached, write-through(WT) 
#define CB			(3<<2)  	//Cached, write-back(WB)

//段描述符[4]=1, [1][0] = 1 0
#define DESC_SEC	((1<<4)|0x02)

//段描述符访问权限
#define RW_CB			(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB			(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB			(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT		(AP_RW|DOMAIN1|NCNB|DESC_SEC)


//设置域访问控制寄存器(CP15的C3)
//域0使用段描述符的AP进行权限检查
//域1无访问权限
#define DOMAIN0_ATTR	(1<<0) 
#define DOMAIN1_ATTR	(0<<2) 


/*
 * 函 数 名：MMU_Init
 * 调    用：MMU_Init();
 * 功能描述：开启MMU, 进行地址映射, 
 *			 虚拟地址0x0000000对应0x30000000, 以使中断发生时能进入中断向量表 
 */
void MMU_Init(void);

void MMU_SetAsyncBusMode(void);
void MMU_SetFastBusMode(void);
#endif
