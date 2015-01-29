#include "GameScene.h"
#define SceneX 20
#define SceneY 20
#define SceneWidth 200
#define SceneHeight 160
#define CellWidth (SceneWidth/row)
#define CellHeight (SceneHeight/column)

int BkGndColor=BLACK;
int BorderColor=LIGHTGRAY;
int LineColor=LIGHTBLUE;
int CellColor=WHITE; 
int SelColor=BLUE;
int CurColor=RED;
int EraColor=CYAN;
//test
long StartTime;
long TotalTime;
int row=0,column=0,PairsCount;//PairsCount,used for re-put and success
struct Canvas *main_canva;
BoxIndex cur_bi,sel_bi;
GameBox **main_array;
ButtonRect quit_button_rect = {
    .left = 0,
    .right = 120,
    .top = 280,
    .bottom = 320, 
}
ButtonRect restart_button_rect = {
    .left = 120,
    .right = 240,
    .top = 280,
    .bottom = 320, 
}


/* ****************************/
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

void DrawPath(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,uint16 color){
    GUI_DrawLine(SceneX+CellWidth/2+CellWidth*x1,SceneY+CellHeight/2+CellHeight*y1
                 ,SceneX+CellWidth/2+CellWidth*x2,SceneY+CellHeight/2+CellHeight*y2,color);
    GUI_DrawLine(SceneX+CellWidth/2+CellWidth*x2,SceneY+CellHeight/2+CellHeight*y2
                 ,SceneX+CellWidth/2+CellWidth*x3,SceneY+CellHeight/2+CellHeight*y3,color);
    GUI_DrawLine(SceneX+CellWidth/2+CellWidth*x3,SceneY+CellHeight/2+CellHeight*y3
                 ,SceneX+CellWidth/2+CellWidth*x4,SceneY+CellHeight/2+CellHeight*y4,color);
}

int FindPath(BoxIndex *c1,BoxIndex *c2){
    int i,j,path,min1,max1,min2,max2,left,right,top,bottom;
    if( main_array[c1->x_i][c1->y_i].type != main_array[c2->x_i][c2->y_i].type)
        return false;
    min1=max1=c1->x_i;
    min2=max2=c2->x_i;
    while(min1-1>=0 && main_array[min1-1][c1->y_i].is_empty==0) min1--;
    while(min2-1>=0 && main_array[min2-1][c2->y_i].is_empty==0) min2--;
    left=max(min1,min2);
    while(max1+1<row && main_array[max1+1][c1->y_i][0]==0) max1++;
    while(max2+1<row && main_array[max2+1][c2->y_i][0]==0) max2++;
    right=min(max1,max2);
    if(left==0){
        DrawPath(c1->x_i,c1->y_i,-1,c1->y_i,-1,c2->y_i,c2->x_i,c2->y_i,LineColor);
        delay(6000);
        DrawPath(c1->x_i,c1->y_i,-1,c1->y_i,-1,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
        return true;
    }
    if(right==(row-1)){
        DrawPath(c1->x_i,c1->y_i,row,c1->y_i,row,c2->y_i,c2->x_i,c2->y_i,LineColor);
        delay(6000);
        DrawPath(c1->x_i,c1->y_i,row,c1->y_i,row,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
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
            delay(6000);
            DrawPath(c1->x_i,c1->y_i,i,c1->y_i,i,c2->y_i,c2->x_i,c2->y_i,BkGndColor);
            return true;
        }
    }
    min1=max1=c1->y_i;
    min2=max2=c2->y_i;
    while(min1-1>=0 && main_array[c1->x_i][min1-1][0]==0) min1--;
    while(min2-1>=0 && main_array[c2->x_i][min2-1][0]==0) min2--;
    top=max(min1,min2);
    while(max1+1<column && main_array[c1->x_i][max1+1][0]==0) max1++;
    while(max2+1<column && main_array[c2->x_i][max2+1][0]==0) max2++;
    bottom=min(max1,max2);
    if(top==0){
        DrawPath(c1->x_i,c1->y_i,c1->x_i,-1,c2->x_i,-1,c2->x_i,c2->y_i,LineColor);
        delay(6000);
        DrawPath(c1->x_i,c1->y_i,c1->x_i,-1,c2->x_i,-1,c2->x_i,c2->y_i,BkGndColor);/*?????*/
        return true;
    }
    if(bottom==(BoardHeight-1)){
        DrawPath(c1->x_i,c1->y_i,c1->x_i,column,c2->x_i,column,c2->x_i,c2->y_i,LineColor);
        delay(6000);
        DrawPath(c1->x_i,c1->y_i,c1->x_i,column,c2->x_i,column,c2->x_i,c2->y_i,BkGndColor);
        return true;
    }

    for(j=top;j<=bottom;j++){
        path=0;
        for(i=min(c1->x_i,c2->x_i)+1;i<max(c1->x_i,c2->x_i); i++){
            path += main_array[i][j].is_empty;
            if(path>0)break;
        }
        if(path==0){
            /* ????? */
            DrawPath(c1->x_i,c1->y_i,c1->x_i,j,c2->x_i,j,c2->x_i,c2->y_i,LineColor);
            delay(6000);
            DrawPath(c1->x_i,c1->y_i,c1->x_i,j,c2->x_i,j,c2->x_i,c2->y_i,BkGndColor);
            /*?????*/
            return true;
        }
    }
    /* ???????????? */
    return false;
}

void DrawBorderRect(BoxIndex *c,uint16 color){
    GUI_DrawRect(SceneX+(c->x_i)*CellWidth+1,SceneY+(c->y)*CellHeight+1,SceneX+(c->x_i+1)*CellWidth-2,SceneY+(c->y_i+1)*CellHeight-2);
    GUI_DrawRect(SceneX+(c->x_i)*CellWidth,SceneY+(c->y_i)*CellHeight,SceneX+(c->x_i+1)*CellWidth-1,SceneY+(c->y_i+1)*CellHeight-1);
}

void put_img_to_canva(GameBox &gbs)
{
    unsigned char tmp_id[5],i,j;
    tmp_id[2] = '\0';
    for (i = 0;i<row;i++) {
        for (j = 0;j<column;j++) {
            tmp_id[0] = 'a' + i;
            tmp_id[1] = '1' + j;
            addBitmap(main_canva,match_icon(gbs.type),gbs.x,gbs.y,CellWidth,CellHeight,tmp_id,1);
        }
    }
}

void EraseCell(int x,int y){
    unsigned char tmp_id[5];
    tmp_id[0] = 'a' + x;
    tmp_id[1] = '1' + y;
    tmp_id[2] = '\0';
    deleteBitmapById(main_canva,tmp_id);
    update(main_canva);
}

GameBox **createArray(int w,int h,int count)
{
    int i,m,rand_x,rand_y;
    unsigned char tmp_id,tmp_img;
    GameBox **ret_gb = (GameBox **)malloc(sizeof(GameBox *) * w);
    for (i = 0;i<w,i++) {
        ret_gb[i] = (GameBox *)malloc(sizeof(GameBox) * h);
        memset(ret_gb[i],0,sizeof(GameBox) * h);
        for (m = 0;m<h;m++) {
            ret_gb[i][m].is_empty = 0;
            ret_gb[i][m].type = 0;
        }
    }

    srand((unsigned)time(NULL));
    rand_x = rand()%w;
    rand_y = rand()%h;
    for (m = 0;m<count/2;m++) {
        while (ret_gb[rand_x][rand_y].is_empty == 1 ) {
            //srand((unsigned)time(NULL));
            rand_x = rand()%w;
            rand_y = rand()%h;
        }
        ret_gb[rand_x][rand_y].is_empty = 1;
        ret_gb[rand_x][rand_y].type = m;
        ret_gb[rand_x][rand_y].x = SceneX + rand_x*CellWidth;
        ret_gb[rand_x][rand_y].y = SceneY + rand_y*CellHeight;

        /******(  make twice  )*******/
        while (ret_gb[rand_x][rand_y].is_empty == 1 ) {
            //srand((unsigned)time(NULL));
            rand_x = rand()%w;
            rand_y = rand()%h;
        }
        ret_gb[rand_x][rand_y].is_empty = 1;
        ret_gb[rand_x][rand_y].type = m;
        ret_gb[rand_x][rand_y].x = SceneX + rand_x*CellWidth;
        ret_gb[rand_x][rand_y].y = SceneY + rand_y*CellHeight;
    }
    return ret_gb;

}

void game_init(int game_level)
{
    switch (game_level) {
    case 1:
        row = 2;column = 3;PairsCount = 6;
        main_array = createArray(2,3,PairsCount);break;
    case 2:
        row = 3;column = 4;PairsCount = 12;
        main_array = createArray(3,4,PairsCount);break;
    case 3:
        row = 4;column = 5;PairsCount = 20;
        main_array = createArray(4,5,PairsCount);break;
    case 4:
        row = 5;column = 6;PairsCount = 30;
        main_array = createArray(5,6,PairsCount);break;
    }
}

BoxIndex *get_clicked_cell(struct *LcdDot TouchXY)
{
    BoxIndex *tmp_bi = (BoxIndex *)malloc(sizeof(BoxIndex));
    memset(tmp_bi,0,sizeof(BoxIndex));
    tmp_bi->x_i = (TouchXY.x - SceneX) / CellWidth;
    tmp_bi->y_i = (TouchXY.y - SceneY) / CellHeight;
    return  tmp_bi;
}

unsigned char xyContainRect(struct *LcdDot touchxy,ButtonRect *rect)
{
    if (touchxy->x >= rect->left && touchxy->x <= rect->right 
        && touchxy->y >= rect->top && touchxy->y <= rect->bottom) {
        return true;
    }else{
        return false;
    }
}

void game_start(int game_level)
{
    int key;
    struct LcdDot TouchXY;
    cur_bi.x_i = cur_bi.y_i = 0;
    sel_bi.x_i = sel_bi.y_i = -1;
    DrawBorderRect(&curCell, CurColor);
    while(1){
        if(GetTouch(&TouchXY)){ //if clicked (touched)
            sel_bi =  get_clicked_cell(&TouchXY);
            /* ?????????? */
            /*???????????cell?*/

            if(cur_bi.x_i==sel_bi.x_i && cur_bi.y_i==sel_bi.y_i)
                DrawBorderRect(&cur_bi, SelColor);
             /*????cell?focus?? */
            else
                DrawBorderRect(&cur_bi, BkGndColor);
             /*?????focus?? */
            /* ????????cell????? */
            if(main_array[cur_bi.x_i][cur_bi.y_i].is_empty==0)
                continue;
            /* ??????????????? */
            if(cur_bi.x_i==sel_bi.x_i && cur_bi.y_i==selCell.y_i)
                continue;
            /*?????????????????????*/
            /*****(  never select before  )**** *
              ****(  set sel_bi as cur_bi )**** */
            if(sel_bi.x_i<0 || sel_bi.y_i<0){
                sel_bi.x_i=cur_bi.x_i;
                sel_bi.y_i=cur_bi.y_i;
                update(main_canva);
                DrawBorderRect(&sel_bi,SelColor);
                continue;
            }
            /* ****( then if have selected, try to   )******
               ****( judge whether it can be removed )****** */
            /*???????????????????*/
            if(FindPath(&sel_bi,&sel_bi)){
                /* ?????cell?*/
                EraseCell(sel_bi.x_i,sel_bi.y_i);
                EraseCell(cur_bi.x_i,cur_bi.y_i);
                /* ?????? */
                main_array[sel_bi.x_i][sel_bi.y_i].is_empty=0;
                main_array[sel_bi.x_i][sel_bi.y_i].type=0;
                main_array[cur_bi.x_i][cur_bi.y_i].is_empty=0;
                main_array[cur_bi.x_i][cur_bi.y_i].type=0;
                /* ??selCell */
                //DrawBorderRect(&selCell,BkGndColor);
                sel_bi.x_i=sel_bi.y_i=-1;
                /* decrease the pairs count */
                PairsCount-=2;
                if(PairsCount==0){
                    //Success
                    //DrawGameOver("CONGRATULATIONS!");
                    return;
                }
            }else{
                /* ???????cell?*/
                /* erase the selCell's focus rect!*/
                //DrawBorderRect(&selCell,BkGndColor);
                sel_bi.x_i=sel_bi.y_i=-1;
            }
            //break;
            //case K_ESC: 
                //DrawGameOver("GAME OVER!"); 
                //break;
            //default:
            //break;
            //} 
            /*????focus??*/
            update(main_canva);
            DrawBorderRect(&cur_bi,CurColor);//draw pre-selected cell
        }
    }
}

void game_finished(int game_level)
{
    struct LcdDot mainTouchXY;
    Paint_Bmp(0,0,240,320,bg);
    while (1) {
        if(GetTouch(&mainTouchXY)) //get the touch-control
        {
            Uart_Printf("%d , %d \n",TouchXY.x,TouchXY.y);
            if (/* *click quit* */1) {//return to the main menu
                return;
            }
            if (/* *restart* */0) {
                PlayGame(game_level);
                return;
            }
        }
    }
}

void PlayGame(int game_level)
{
    game_init(game_level);
    game_start(game_level);
    game_finished(game_level);
}
