#include "GameScene.h"
#include <time.h>

#define SceneX 20
#define SceneY (20+80)
#define SceneWidth 200
#define SceneHeight 160
#define CellWidth (SceneWidth/column-3)
#define CellHeight (SceneHeight/row-3)

int BkGndColor=BLACK;
int BorderColor=LIGHTGRAY;
int LineColor=LIGHTBLUE;
int CellColor=WHITE;
int SelColor=BLUE;
int CurColor=RED;
int DraColor=CYAN;

int TotalTime,EndTime;
int row=0,column=0,PairsCount,is_win=0,ref_no;//PairsCount,used for re-put and success
//����������Ϸ����ƵĻ�������
Canvas main_canva;
//��Ϸ������
BoxIndex cur_bi,sel_bi,tmp_bi;

ButtonRect quit_button_rect;
ButtonRect restart_button_rect;
ButtonRect refresh_button_rect;
//��Ϸ�����ݵ��߼�����
GameBox main_array[6][5];

/* ****( ��ȡ��Ӧ���͵�ʵ��ͼ������ )**** *
 * ����: @type����ͼ������										*
 * ����ֵ: Bitmap���ͣ�ͼƬ��������				*
 ****************************************** */
Bitmap match_icon(int type)
{
    switch (type) {
    case 0:
        return t_bmp_1;
    case 1:
        return t_bmp_2;
    case 2:
        return t_bmp_3;
    case 3:
        return t_bmp_4;
    case 4:
        return t_bmp_5;
    case 5:
        return t_bmp_6;
    case 6:
        return t_bmp_7;
    case 7:
        return t_bmp_8;
    default:
        return NULL;
    }
}



/* **************( ���ƿ�����ͨ��·�� )*************** *
 * ����: @x1��@y1��@x2��@y2��@x3��@y3��@x4��@y4�����յ����� 	*
 * ����: @color����·��������ɫ														*
 * ����ֵ: ��																						*
 ******************************************************** */
void DrawPath(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,uint16 color){
		
		int px1,px2,px3,px4,py1,py2,py3,py4;
		px1 = SceneX+(CellWidth+3)/2+(CellWidth+3)*x1;
		px2 = SceneX+(CellWidth+3)/2+(CellWidth+3)*x2;
		px3 = SceneX+(CellWidth+3)/2+(CellWidth+3)*x3;
		px4 = SceneX+(CellWidth+3)/2+(CellWidth+3)*x4;
		py1 = SceneY+(CellHeight+3)/2+(CellHeight+3)*y1;
		py2 = SceneY+(CellHeight+3)/2+(CellHeight+3)*y2;
		py3 = SceneY+(CellHeight+3)/2+(CellHeight+3)*y3;
		py4 = SceneY+(CellHeight+3)/2+(CellHeight+3)*y4;
		Uart_Printf("%d||%d||%d||%d",px1,py1,px2,py2);
		if(px2 <= 0)px2 = px3 = 5;
		if(px2 >= 240)px2 = px3 = 235;
		GUI_DrawLine(px1,py1,px2,py2,color);
		GUI_DrawLine(px2,py2,px3,py3,color);
		GUI_DrawLine(px3,py3,px4,py4,color);
}



/* *****( Ѱ����ͨ·���ĺ����㷨 )***** *
 * ����: @c1��@c2������ҪѰ��·����������	*
 * ����ֵ: int���ͣ��ҵ�����1�����򷵻�0	*
 ****************************************** */
int FindPath(BoxIndex *c1,BoxIndex *c2){
    int i,j,path,min1,max1,min2,max2,left,right,top,bottom;
	//Uart_Printf("c1's type is %d::::c2's type is %d\n",main_array[c1->x_i][c1->y_i].type,main_array[c2->x_i][c2->y_i].type);
    if( main_array[c1->x_i][c1->y_i].type != main_array[c2->x_i][c2->y_i].type)
        return false;
    min1=max1=c1->x_i;
    min2=max2=c2->x_i;
    while(min1-1>=0 && main_array[min1-1][c1->y_i].is_empty==0) min1--;
    while(min2-1>=0 && main_array[min2-1][c2->y_i].is_empty==0) min2--;
    left=max(min1,min2);
    while(max1+1<column && main_array[max1+1][c1->y_i].is_empty==0) max1++;
    while(max2+1<column && main_array[max2+1][c2->y_i].is_empty==0) max2++;
    right=min(max1,max2);
    if(left==0){
        DrawPath(c1->x_i,c1->y_i,-1,c1->y_i,-1,c2->y_i,c2->x_i,c2->y_i,LineColor);
        delay(2000);
        //DrawPath(c1->x_i,c1->y_i,-1,c1->y_i,-1,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
        return true;
    }
    if(right==(column-1)){
//				Uart_Printf("RIGHT~~!");
        DrawPath(c1->x_i,c1->y_i,column,c1->y_i,column,c2->y_i,c2->x_i,c2->y_i,LineColor);
        delay(2000);
			//Uart_Printf("RIGHT!!");
        //DrawPath(c1->x_i,c1->y_i,column,c1->y_i,column,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
        return true;
    }
    for(i=left;i<=right;i++){
        path=0;
        for(j=min(c1->y_i,c2->y_i)+1;j<max(c1->y_i,c2->y_i);j++){
            path+=main_array[i][j].is_empty;
            if(path>0) break;
        }
        if(path==0){
            DrawPath(c1->x_i,c1->y_i,i,c1->y_i,i,c2->y_i,c2->x_i,c2->y_i,LineColor);
            delay(2000);
            //DrawPath(c1->x_i,c1->y_i,i,c1->y_i,i,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
            return true;
        }
    }
    min1=max1=c1->y_i;
    min2=max2=c2->y_i;
    while(min1-1>=0 && main_array[c1->x_i][min1-1].is_empty==0) min1--;
    while(min2-1>=0 && main_array[c2->x_i][min2-1].is_empty==0) min2--;
    top=max(min1,min2);
    while(max1+1<row && main_array[c1->x_i][max1+1].is_empty==0) max1++;
    while(max2+1<row && main_array[c2->x_i][max2+1].is_empty==0) max2++;
    bottom=min(max1,max2);
    if(top==0){
        DrawPath(c1->x_i,c1->y_i,c1->x_i,-1,c2->x_i,-1,c2->x_i,c2->y_i,LineColor);
        delay(2000);
        //DrawPath(c1->x_i,c1->y_i,c1->x_i,-1,c2->x_i,-1,c2->x_i,c2->y_i,BkGndColor);
				return true;
    }
    if(bottom==(row-1)){
        DrawPath(c1->x_i,c1->y_i,c1->x_i,row,c2->x_i,row,c2->x_i,c2->y_i,LineColor);
        delay(2000);
        //DrawPath(c1->x_i,c1->y_i,c1->x_i,row,c2->x_i,row,c2->x_i,c2->y_i,BkGndColor);
        return true;
    }

    for(j=top;j<=bottom;j++){
        path=0;
        for(i=min(c1->x_i,c2->x_i)+1;i<max(c1->x_i,c2->x_i); i++){
            path += main_array[i][j].is_empty;
            if(path>0)break;
        }
        if(path==0){
            DrawPath(c1->x_i,c1->y_i,c1->x_i,j,c2->x_i,j,c2->x_i,c2->y_i,LineColor);
            delay(2000);
            //DrawPath(c1->x_i,c1->y_i,c1->x_i,j,c2->x_i,j,c2->x_i,c2->y_i,BkGndColor);
            return true;
        }
    }
    return false;
}



/* -----------( ����ѡ�п����ʾ�߿� )---------- *
 * ����: @c������Ҫ���Ʊ߿�Ŀ飬@color�����߿�ɫ�� *
 * ����ֵ�� ��																			*
 * ------------------------------------------------ */
void DrawBorderRect(BoxIndex *c,uint16 color){
    GUI_DrawRect(SceneX+(c->x_i)*(CellWidth+3)+1,SceneY+(c->y_i)*(CellHeight+3)+1,SceneX+(c->x_i+1)*(CellWidth+3)-2,SceneY+(c->y_i+1)*(CellHeight+3)-2,color);
    GUI_DrawRect(SceneX+(c->x_i)*(CellWidth+3),SceneY+(c->y_i)*(CellHeight+3),SceneX+(c->x_i+1)*(CellWidth+3)-1,SceneY+(c->y_i+1)*(CellHeight+3)-1,color);
}


/* ---( �������ݵĿ��ж�Ӧͼ�������ƻ��������� )-- *
 * ������@gbs ���� ����Ϣ����															*
 * ����ֵ����
 * ------------------------------------------------------ */
void put_img_to_canva(GameBox gbs[6][5])
{
    char tmp_id[5],i,j;
    tmp_id[2] = '\0';
    for (i = 0;i<column;i++) {
        for (j = 0;j<row;j++) {
            tmp_id[0] = 'a' + i;
            tmp_id[1] = '1' + j;
					  if(gbs[i][j].is_empty)
							addBitmap(main_canva,match_icon(gbs[i][j].type),gbs[i][j].x,gbs[i][j].y,CellWidth,CellHeight,tmp_id,1);
						Uart_Printf("Bitmap(%s)added!\n",tmp_id);
        }
    }
}



/* ---( �ӻ���������ɾ���߼�λ�ÿ��Ӧ��ͼƬ���� )--- *
 * ������@x��@y ���� ��Ҫɾ���Ŀ���߼�λ��								*
 * ����ֵ����
 * -------------------------------------------------------*/
void EraseCell(int x,int y){
    char tmp_id[5];
    tmp_id[0] = 'a' + x;
    tmp_id[1] = '1' + y;
    tmp_id[2] = '\0';
    deleteBitmapById(main_canva,tmp_id);
	Uart_Printf("cell : %d , %d has been erased!",x,y);
    //update(main_canva);
}



/* ---------( �����µ�������п����� )-------	*
 * ������@w��@h ���� ��ǰ�Ѷȼ���ĺ����������		*
 * ������@count ���� ��ǰ��Ҫ���еĿ�������			*
 * ---------------------------------------------- */
void createArray(int w,int h,int count)
{
    int i,m,rand_x=0,rand_y=0,m_e;
//    char tmp_id,tmp_img;
		

		for (i = 0;i<6;i++) {
			for (m = 0;m<5;m++) {
					main_array[i][m].x = 0;
					main_array[i][m].y = 0;
					main_array[i][m].is_empty = 0;
					main_array[i][m].type = 0;
			}
		}
    //srand((unsigned)time(NULL));
    //rand_x = rand()%w;
    //rand_y = rand()%h;
		//Uart_Printf("Rand_x: %d , Rand_y: %d ..\n",rand_x,rand_y);
				
    for (m = 0;m<count/2;m++) {
				
        while (main_array[rand_x][rand_y].is_empty == 1 ) {
            //srand((unsigned)time(NULL));
            rand_x = rand()%w;
            rand_y = rand()%h;
        }
				Uart_Printf("Rand_x: %d , Rand_y: %d ..\n",rand_x,rand_y);
        main_array[rand_x][rand_y].is_empty = 1;
				if(m>=8){
					m_e = rand()%8;
					main_array[rand_x][rand_y].type = m_e;
				}else				
					main_array[rand_x][rand_y].type = m;
        main_array[rand_x][rand_y].x = SceneX + rand_x*(CellWidth+3);
        main_array[rand_x][rand_y].y = SceneY + rand_y*(CellHeight+3);

        /******(  ���ɳɶԵĿ�  )*******/
				while (main_array[rand_x][rand_y].is_empty == 1 ) {
            rand_x = rand()%w;
            rand_y = rand()%h;
        }
				Uart_Printf("Rand_x: %d , Rand_y: %d ..\n",rand_x,rand_y);
        main_array[rand_x][rand_y].is_empty = 1;
        if(m>=8)
					main_array[rand_x][rand_y].type = m_e;
				else				
					main_array[rand_x][rand_y].type = m;
        main_array[rand_x][rand_y].x = SceneX + rand_x*(CellWidth+3);
        main_array[rand_x][rand_y].y = SceneY + rand_y*(CellHeight+3);
    }
    //return ret_gb;
}



/* ***(����ϴ�Ƶĺ���	)****	*
 * ����: @level�����ؿ����� 		*
 * ����ֵ: ��								*
 * ************************** */
void refresh_boxes(int level)
{
	//if(ref_no == 0)
	//	return;
	switch(level)
	{
		case 1:
			createArray(3,2,PairsCount);break;
		case 2:
			createArray(4,3,PairsCount);break;
		case 3:
			createArray(5,4,PairsCount);break;
		case 4:
			createArray(6,5,PairsCount);break;
		default:
			break;
	}
	ref_no --;
	Paint_Bmp(0,0,240,320,game_scene_ui);
	put_img_to_canva(main_array);
	update(main_canva);
}





/* *************( get_clicked_cell	)********** *
 * ����: @TouchXY ���� ��������Ϣ					 		*
 * ����ֵ: ��																	*
 * ���ܣ� ��ȡ����Ŀ飬������Ϣ����tmp_bi		*
 * ******************************************** */
void get_clicked_cell(struct LcdDot* TouchXY)
{
	tmp_bi.x_i = (TouchXY->x - SceneX) / (CellWidth+3);
	tmp_bi.y_i = (TouchXY->y - SceneY) / (CellHeight+3);
}



/* *************( xyContainRect	)**************** *
 * ����: @TouchXY ���� ��������Ϣ							  *
 * 				@rect ���� ��Ҫ�жϵİ�������Ϣ  		  *
 * ����ֵ: uchar���ͣ�������򷵻�1�����򷵻�0 *																	*
 * ���ܣ� �жϵ���ĵ��Ƿ�λ��ָ����������		*
 * ********************************************** */
unsigned char xyContainRect(struct LcdDot *touchxy,ButtonRect *rect)
{
		Uart_Printf("====(t_pos:%d,%d)===||===(b_rect:%d,%d,%d,%d)===\n",touchxy->x,touchxy->y,
									rect->left,rect->right,rect->top,rect->bottom);
    if (touchxy->x >= rect->left && touchxy->x <= rect->right 
        && touchxy->y >= rect->top && touchxy->y <= rect->bottom) {
        return true;
    }else{
        return false;
    }
}




/*�жϷ������ ���ڼ�ʱ������Ϸʱ��*/
void HandlerIRQ(void)
{
	switch(INTOFFSET)
	{
		case 10://��ʱ��0�ж�
			EndTime++;
  	default:
			break;
	}
	ClearPending(1<<INTOFFSET); 		//���жϹ���Ĵ�������SRCPND INTPND
}


/* *************( game_init	)******************** *
 * ����: @game_level ���� ��Ϸ�Ѷ�							  	*
 * ����ֵ: ��  																	*
 * ���ܣ� ��ʼ����Ϸ���ݣ�����ʱ�䡢�����еȵ�*
 * ********************************************** */
void game_init(int game_level)
{
		
		quit_button_rect.left = 120;
    quit_button_rect.right = 240;
    quit_button_rect.top = 280;
    quit_button_rect.bottom = 320;
	
		restart_button_rect.left = 0;
    restart_button_rect.right = 120;
    restart_button_rect.top = 280;
    restart_button_rect.bottom = 320;

		refresh_button_rect.left = 175;
    refresh_button_rect.right = 224;
    refresh_button_rect.top = 50;
    refresh_button_rect.bottom = 80;

		is_win = 0;
		ref_no = 3;
		TotalTime = 0;
		EndTime = 0;
		Paint_Bmp(0,0,240,320,game_scene_ui);
	  //Uart_Printf("Game Ready To Initialize...\n");
    switch (game_level) {
    case 1:
        row = 2;column = 3;PairsCount = 6;
        //main_array = createArray(2,3,PairsCount);
				createArray(3,2,PairsCount);break;
    case 2:
        row = 3;column = 4;PairsCount = 12;
        //main_array = createArray(3,4,PairsCount);
				createArray(4,3,PairsCount);break;
    case 3:
        row = 4;column = 5;PairsCount = 20;
        //main_array = createArray(4,5,PairsCount);
				createArray(5,4,PairsCount);break;
    case 4:
        row = 5;column = 6;PairsCount = 30;
        //main_array = createArray(5,6,PairsCount);
				createArray(6,5,PairsCount);break;
    } 
		//time(&StartTime);
		Timer0Init();
		//Uart_Printf("Game Initialized...\n");
}


/* *************( game_start )******************** *
 * ����: @game_level ���� ��Ϸ�Ѷ�							  	 *
 * ����ֵ: ��  																	 *
 * ���ܣ� ��Ϸ������ѭ����������Ҫ�жϵ����	 *
 * 				���Լ���Ϸ�߼�������ȵ���Ϊ��				 *
 * *********************************************** */
void game_start(int game_level)
{
//    int key;
    struct LcdDot TouchXY;
		//BoxIndex* tmp_bi; 
  
		cur_bi.x_i = cur_bi.y_i = -1;
    sel_bi.x_i = sel_bi.y_i = -1;
		
    //DrawBorderRect(&cur_bi, CurColor);
		put_img_to_canva(main_array);
		//Paint_Bmp(0,0,240,320,game_scene_ui);
		update(main_canva);
    while(1){
				Lcd_printf(0,50,RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff),1,"����ʱ�䣺 %d ��",EndTime);
				if(ref_no > 0){
					Paint_Bmp(175,50,49,30,xp_1);
				}else{
					Paint_Bmp(175,50,49,30,xp_2);
				}
        if(GetTouch(&TouchXY)){ //if clicked (touched)
            //tmp_bi = get_clicked_cell(&TouchXY);
					
						if (xyContainRect(&TouchXY,&quit_button_rect)){
								cleanCanvas(main_canva);
                return; 
						}
            //if (/* *restart* */0) {
						else if (xyContainRect(&TouchXY,&restart_button_rect)){
							cleanCanvas(main_canva);
							//refresh_boxes(game_level);
							PlayGame(game_level);  
							return;
						}
						else if(xyContainRect(&TouchXY,&refresh_button_rect)){
							if(ref_no > 0){
								cleanCanvas(main_canva);
								refresh_boxes(game_level);
								continue;
							}
						}
						
						get_clicked_cell(&TouchXY);	
						cur_bi.x_i = tmp_bi.x_i;
						cur_bi.y_i = tmp_bi.y_i;
            
						
            if(main_array[cur_bi.x_i][cur_bi.y_i].is_empty==0)
                continue;

            if(cur_bi.x_i==sel_bi.x_i && cur_bi.y_i==sel_bi.y_i)
                continue;

						/*****(  never select before  )**** *
              ****(  set sel_bi as cur_bi )**** */
            if(sel_bi.x_i<0 || sel_bi.y_i<0){
                sel_bi.x_i=cur_bi.x_i;
                sel_bi.y_i=cur_bi.y_i;
								//Paint_Bmp(0,0,240,320,game_scene_ui);
                //update(main_canva);
                DrawBorderRect(&sel_bi,SelColor);
                continue;
            }
            /* ****( then if have selected, try to   )******
               ****( judge whether it can be removed )****** */
            
            if(FindPath(&cur_bi,&sel_bi)){
                EraseCell(sel_bi.x_i,sel_bi.y_i);
                EraseCell(cur_bi.x_i,cur_bi.y_i);
                main_array[sel_bi.x_i][sel_bi.y_i].is_empty=0;
                main_array[sel_bi.x_i][sel_bi.y_i].type=0;
                main_array[cur_bi.x_i][cur_bi.y_i].is_empty=0;
                main_array[cur_bi.x_i][cur_bi.y_i].type=0;
                //DrawBorderRect(&selCell,BkGndColor);
                sel_bi.x_i=sel_bi.y_i=-1;

								PairsCount-=2;
                if(PairsCount==0){
                    //Success
                    //DrawGameOver("CONGRATULATIONS!");
										is_win = 1;
										TotalTime = EndTime;
										cleanCanvas(main_canva);
                    return;
                }
            }else{
                
                //DrawBorderRect(&selCell,BkGndColor);
                sel_bi.x_i=sel_bi.y_i=-1;
            }
            Paint_Bmp(0,0,240,320,game_scene_ui);
            update(main_canva);
						//EndTime =time(NULL);
						//��������ʱ��
						//Lcd_printf(0,50,RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff),0,"����ʱ�䣺 %d ��",difftime(EndTime,StartTime));
            //DrawBorderRect(&cur_bi,CurColor);//draw pre-selected cell
						
						
        }
    }
}


/* *************( game_finished )***************** *
 * ����: @game_level ���� ��Ϸ�Ѷ�							  	 *
 * ����ֵ: ��  																	 *
 * ���ܣ� ��Ϸ��������ʾ�Ľ��棬������Ҫ��ʾ	 *
 * 				��Ϸ���ճɼ����Լ��ȴ��û�ѡ���Ծ��� *
 *				��Ҫ�˻ز˵��������¿�ʼ��						 *
 * *********************************************** */
void game_finished(int game_level)
{
    struct LcdDot mainTouchXY;
		if(!is_win)
			return;
    Paint_Bmp(0,0,240,320,game_scene_ui);
		Paint_Bmp(0,80,240,200,game_pass);
		Lcd_printf(50,50,RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff),0,"���ճɼ�Ϊ�� %d ��",TotalTime);
		
    while (1) {
        if(GetTouch(&mainTouchXY)) //get the touch-control
        {
            Uart_Printf("%d , %d \n",mainTouchXY.x,mainTouchXY.y);
            //if (/* *click quit* */1) {//return to the main menu
            if (xyContainRect(&mainTouchXY,&quit_button_rect))
                return; 
            //if (/* *restart* */0) {
						if (xyContainRect(&mainTouchXY,&restart_button_rect)){
								//PlayGame(game_level);
							PlayGame(game_level);  
							return;
						}
			}
    }
}



/* ****************( PlayGame )******************* *
 * ����: @game_level ���� ��Ϸ�Ѷ�							  	 *
 * ����ֵ: ��  																	 *
 * ���ܣ� ��Ϸ������ں������Ӵ˴���ʼִ����Ϸ *
 * 				��ʼ���������߼�����β��һϵ�к����� *
 * *********************************************** */
void PlayGame(int game_level)
{
	//Led_Test();
	/*uint8 i;
	GPB_Conf();						// GPIO��ʼ��
	
	Paint_Bmp(0, 0, 240, 320, bg);	// ҳ���л�
	Lcd_printf(30, 100, RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff),0,"���ڽ�����ˮ����ʾ");
	
	for(i=0; i<10; i++)
	{
		Lcd_printf(30, 125, RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff),1,"ѭ����ʾ10�Σ���ǰ��%d��", i+1);				
		if (i == 3)
			Lcd_printf(30, 200, RGB(0xff,0x00,0x00),RGB(0xff,0xff,0xff),1,"�����˳�...", i);	
		
		Led_Out(0x01);				// LED1����
		delay(3000);
		Led_Out(0x02);				// LED2����
		delay(3000);
		Led_Out(0x04);				// LED3����
		delay(3000);
		Led_Out(0x08);				// LED4����
		delay(3000); 
		Led_Out(0x0F);				// LEDȫ��
		delay(3000);
		Led_Out(0x00);				// LEDȫ��
		delay(3000);
	}*/
	
    game_init(game_level);
    game_start(game_level);
    game_finished(game_level);
}
