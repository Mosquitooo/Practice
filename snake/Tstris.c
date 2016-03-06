/***************************** SOURCE FILE INFORMATION *****************************

	TARGET ENVIRONMENT	:: Linux
	CREATED				:: 2014.10.20
	LAST UPDATED		:: 2014.10.22
	MAIL				::905595245@qq.com

************************ END OF SOURCE FILE INFORMATION ***************************/
/**********************************************************************************
OPERATE:['a' 左] ['d' 右] ['s' 下] ['w' 旋转] [others 下]
************************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <termio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

/*************************************************
宏
**************************************************/
/*方框的大小*/
#define WIDTH	10
#define HIGHT	20
/*方块的边界*/
#define WIN_X1 1
#define WIN_X2 (WIN_X1+WIDTH+1)
#define WIN_Y1 1
#define WIN_Y2 (WIN_Y1+HIGHT+1)


/*方块的初始位置*/
#define START_X	6
#define START_Y	3
#define SHAPE	'@'

/*方块的速度*/
#define MAX_TIME	800000/*0.8秒*/
#define MIN_TIME	0
#define MAX_LEVEL	10

/*方块的移动方向*/
#define DOWN	1
#define UP		0
#define LEFT	3
#define RIGHT	2

#define OVER	"Game Over!!!"
#define PIPE_NAME	"./pipe"
/*************************************************
结构体
**************************************************/
typedef struct
{
    int ZBody[4][2];/*四个格子:x y*/
    int flag;/*形状 0:条 1：方 2：正Z 3:反Z  4：正L 5:反L 6:土 */
    int direction;/*朝向  0:上1:右2:下3:左  对 Z来说是初始朝向*/
} ZzBody;
/*********************************************************
全局变量
***********************************************************/
int giScore = 0;
int giLevel = 0;	/*关数*/
int giNewBlock = 0;/*是否开始一个新的方块*/
int giMoveErr = 0;/*是否撞墙*/
int block_screen[WIN_Y2][WIN_X2] = {0};/*背景框*/
int giHight = 0;
int giPipeFd = -1;

/*************************************************
函数
*************************************************
static void ZInitBlock(ZzBody *CurrBody,int flag)；
static void settty(int iFlag);
static void ZDrawOneNode(int y,int x,int iFlag);
static void ZInitScreen();
static void ZRestoreScreen();
static void ZDrawScope();
static void zDrawBlock(ZzBody block,int iflag);
static void zNextBlock(ZzBody *block,int Dir);
static void DrawScreen(int flag);
static void JudgeNext(ZzBody block);
static int zJudgeNew(ZzBody block,int dir);
static int JudgeScreen();
static int ZGetDir();

*/




/***************************************************
*****************************************************/
/*初始化方块 或者是方块的变形*/
static void ZInitBlock(ZzBody *CurrBody,int flag/*0：表示第一次出现方块 1：方块的变相*/)
{
    int i,Zj,Lj;
    int plus,plus1;
    if(flag == 0)//0：第一次产生方块 1：方块的朝向出现变化
    {
        srand(time(NULL));
        memset(CurrBody,0,sizeof(ZzBody));
        //初始化第一个格子
        CurrBody->ZBody[0][0] = START_X;
        CurrBody->ZBody[0][1] = START_Y;
        //初始化形状
        CurrBody->flag = rand()%7;
    }


    /*初始化其余的三个格子*/

    switch(CurrBody->flag)
    {
    case 0:/*条形*/
        if(!flag)
            CurrBody->direction = rand()%2;//初始化朝向
        for(i=1; i<4; i++)
        {
            CurrBody->ZBody[i][(~CurrBody->direction)&1] = CurrBody->ZBody[0][(~CurrBody->direction)&1];
            CurrBody->ZBody[i][CurrBody->direction] = CurrBody->ZBody[0][CurrBody->direction]+i;
        }
        /*方向为0是横着的 1是竖着的 */
        break;
    case 1:/*正方形*/
        CurrBody->direction = 0;
        for(i=1; i<4; i++)
        {
            CurrBody->ZBody[i][0] = CurrBody->ZBody[0][0]+(1&i);
            CurrBody->ZBody[i][1] = CurrBody->ZBody[0][1]+((i&2)>>1);
        }
        break;
    case 2://正Z
        if(!flag)
            CurrBody->direction = rand()%2;//初始化朝向0:横1：竖着

        if(CurrBody->direction)
        {
            CurrBody->ZBody[0][0]++;
            plus = -1;
        }
        else
        {
            CurrBody->ZBody[0][0]--;
            plus = 1;
        }
        for(i=1; i<4; i++)
        {
            if(i&2)
                Lj = 1;
            else
                Lj = 0;
            if(((i&2)>>1) ^ (i&1))
                Zj = 1;
            else
                Zj = 2;

            CurrBody->ZBody[i][CurrBody->direction] = CurrBody->ZBody[0][CurrBody->direction]+Zj;
            CurrBody->ZBody[i][(~CurrBody->direction)&1] = CurrBody->ZBody[0][(~CurrBody->direction)&1]+Lj*plus;
        }
        break;
    case 3://反 Z
        if(!flag)
            CurrBody->direction = rand()%2;//初始化朝向0:横1：竖着
        if(CurrBody->direction)
        {
            CurrBody->ZBody[0][0]--;
            plus = 1;
        }
        else
        {
            CurrBody->ZBody[0][0]++;
            plus = -1;
        }
        for(i=1; i<4; i++)
        {
            if(i&2)
                Lj = 1;
            else
                Lj = 0;
            if(((i&2)>>1) ^ (i&1))
                Zj = 1;
            else
                Zj = 2;
            CurrBody->ZBody[i][CurrBody->direction] = CurrBody->ZBody[0][CurrBody->direction]+Zj*plus;
            CurrBody->ZBody[i][(~CurrBody->direction)&1] = CurrBody->ZBody[0][(~CurrBody->direction)&1]+Lj;
        }
        break;

    case 4://正L
        if(!flag)
            CurrBody->direction = rand()%4;
        Zj = CurrBody->direction;
        if(((Zj&2)>>1)^(Zj&1)) /*0 3表示减号  1 2 表示加号*/
        {
            plus = 1;
        }
        else
        {
            plus = -1;
        }
        for(i=1; i<4; i++)
        {
            if(i&2) /*1 减号 2 3 加号 */
            {
                Lj = 1;
            }
            else
            {
                Lj = -1;
            }
            CurrBody->ZBody[i][(CurrBody->direction&1)] = CurrBody->ZBody[0][(CurrBody->direction&1)]+Lj;
            CurrBody->ZBody[i][(~(CurrBody->direction&1))&1] = CurrBody->ZBody[0][(~(CurrBody->direction&1))&1]+((~(i&1))&1)*plus;
        }
        if(Zj == 2 || Zj == 3)
        {
            CurrBody->ZBody[2][Zj&1] -= 2;
        }
        break;
    case 5://反L
        if(!flag)
            CurrBody->direction = rand()%4;
        Zj = CurrBody->direction;
        if(((Zj&2)>>1)^(Zj&1)) /*0 3表示减号  1 2 表示加号*/
        {
            plus = 1;
        }
        else
        {
            plus = -1;
        }
        for(i=1; i<4; i++)
        {
            if(((i&2)>>1) ^ (i&1)) /*1 2 减号 3 加号*/
            {
                Lj = -1;
            }
            else
            {
                Lj = 1;
            }
            CurrBody->ZBody[i][(CurrBody->direction&1)] = CurrBody->ZBody[0][(CurrBody->direction&1)]+Lj;
            CurrBody->ZBody[i][(~(CurrBody->direction&1))&1] = CurrBody->ZBody[0][(~(CurrBody->direction&1))&1]+((~(i&1))&1)*plus;
        }
        if(Zj == 2 || Zj == 3)
        {
            CurrBody->ZBody[2][Zj&1] += 2;
        }
        break;
    case 6:/*半十字形*/
        if(!flag)
            CurrBody->direction = rand()%4;
        Zj = CurrBody->direction;
        if(((Zj&2)>>1)^(Zj&1)) /*0 3表示减号  1 2 表示加号*/
        {
            plus = 1;
        }
        else
        {
            plus = -1;
        }
        for(i=1; i<4; i++)
        {
            CurrBody->ZBody[i][(CurrBody->direction&1)] = CurrBody->ZBody[0][(CurrBody->direction&1)]+(i-2);
            CurrBody->ZBody[i][(~(CurrBody->direction&1))&1] = CurrBody->ZBody[0][(~(CurrBody->direction&1))&1]+((~(i&1))&1)*plus;
        }
        break;
    default:
        break;

    }

}
static void settty(int iFlag)/*控制是否将输入直接打印到面板中*/
{
    int fd;
    struct termio stTerm;

    if((fd = open(ttyname(1),O_RDWR))==-1)        return;
    if(iFlag == 1)
    {
        ioctl(fd,TCGETA,&stTerm);
        stTerm.c_lflag &= ~ICANON;
        stTerm.c_lflag &= ~ECHO;
        stTerm.c_cc[4] = 1;
        stTerm.c_cc[5] = 0;
        stTerm.c_iflag &= ~ISTRIP;
        stTerm.c_cflag |= CS8;
        stTerm.c_cflag &= ~PARENB;
        ioctl(fd,TCSETA,&stTerm);
    }
    else
    {
        ioctl(fd,TCGETA,&stTerm);
        stTerm.c_lflag |= ICANON;
        stTerm.c_lflag |= ECHO;
        stTerm.c_cc[4] = 4;
        stTerm.c_cc[5] = 5;
        stTerm.c_iflag &= ~ISTRIP;
        stTerm.c_cflag |= CS8;
        stTerm.c_cflag &= ~PARENB;
        ioctl(fd,TCSETA,&stTerm);
    }
    close(fd);
}
static void ZDrawOneNode(int y,int x,int iFlag)/*打印一个Node*/
{
        if(iFlag == 1)
                printf("\033[%dm\033[47;%dm\033[%d;%dH%c",
                        iFlag,30/*black*/,y,x,SHAPE);
        else
                printf("\033[%dm\033[47;%dm\033[%d;%dH%c",
                        iFlag,37/*white*/,y,x,' ');

        fflush(stdout);
}


static void ZInitScreen()/*初始化屏幕*/
{
    settty(1);
    printf("\033[?25l\033[2J");
}

static void ZRestoreScreen()/*复原屏幕*/
{
    printf("\033[24;1H\033[1m\033[40;34m\033[?25h");
    settty(0);
}

void ZDrawScope()/*画上边框*/
{
    int i,j;

    for(j=WIN_Y1; j<=WIN_Y2; j+=WIN_Y2-WIN_Y1)
    {
        printf("\033[%d;%dH+",j,WIN_X1);
        for(i=WIN_X1+1; i<WIN_X2; i++)
            printf("-");
        printf("+");
    }
    for(i=WIN_Y1+1; i<WIN_Y2; i++)
        printf("\033[%d;%dH|%*c|\n",i,WIN_X1,WIN_X2-WIN_X1-1,' ');
}

static int ZGetDir()/*获取方向*/
{
    fd_set fd;
    struct timeval tv;
    int dw;
    char buf;
    int len,iFlag;
    int maxfd;
    char ch[2] = {0};

    FD_ZERO(&fd);
    FD_SET(0,&fd);
    FD_SET(giPipeFd,&fd);

    maxfd = giPipeFd+1;

    tv.tv_sec = 2;/*mod 2014-1029*/
    tv.tv_usec = 0;/*MAX_TIME - (MAX_TIME-MIN_TIME)/MAX_LEVEL*giLevel;/*控制速度*/

    dw = select(maxfd,&fd,NULL,NULL,&tv);
    if(dw == 0)
    {
        return DOWN;
    }
    if(FD_ISSET(giPipeFd,&fd))
    {
        read(giPipeFd,ch,sizeof(ch));
        return DOWN;
    }
    if(FD_ISSET(0,&fd))
    {
        len = read(0,&buf,1);
        if(len == 1)
        {
            switch(buf)
            {
            case 'w':
                return UP;//UP
            case 's':
                return DOWN;//DOWN
            case 'a':
                return LEFT;//RIGHT
            case 'd':
                return RIGHT;//LEFT

            default  :
                return(DOWN);
            }
        }
    }
}

static void zDrawBlock(ZzBody block,int iflag)/*显示一个方块*/
{
    int i;
    for(i=0; i<4; i++)
    {
        ZDrawOneNode(block.ZBody[i][1],block.ZBody[i][0],iflag);
    }
}
static void zNextBlock(ZzBody *block,int Dir)/*移动或这变形后的方块的坐标*/
{
    int i;
    int x = 0;
    do
    {
        switch(Dir)
        {
        /*方块旋转*/
        case UP:
            if(block->flag == 1)
            {
			break;
            }
            else if(block->flag == 4 || block->flag ==5 || block->flag == 6)
            {
                block->direction = (++block->direction)%4;/*四种方向*/
            }
            else
            {
                block->direction = (++block->direction)%2;/*只有两种方向*/
            }
            ZInitBlock(block,1);
            break;
        /*方块下移*/
        case DOWN:

            for(i=0; i<4; i++)
            {
                block->ZBody[i][1] += 1;
            }
            break;
        /*方块左右移动*/
        case LEFT:
            x -= 2;
        case RIGHT:
            x += 1;
            for(i=0; i<4; i++)
            {
                block->ZBody[i][0] += x;
            }
            break;
        default:
            break;
        }
    }
    while(0);
}
/*
int zJudgeBlock(ZzBody block)/*丢弃这个函数
/*1：到最底 产生新的方块 2：到最左或者最右 或者中间卡住了 无法继续移动 3:到顶了 游戏结束
{

}*/
static void DrawScreen(int flag)/*背景的 全部清空(0) 或者全部显示(1)*/
{
    int x,y;
    for(y=WIN_Y2-2; y>(WIN_Y2-giHight-2); y--)
    {
        for(x = WIN_X1; x<WIN_X2-1; x++)
        {
            if(block_screen[y][x] == 1)/*表示这个坐标的位置 是有方块的*/
                ZDrawOneNode(y+1,x+1,flag);
        }
    }
}
static void JudgeScreen()/*查看是否有符合消除条件的，如果有的话进行消除*/
{
    int i,x,y;
    int len = 0;
    int sum = 0,score = 0;
    for(y=WIN_Y2-2; y>(WIN_Y2-giHight-2); y--) /*从下往上消除*/
    {

        len = 0;
        for(x = WIN_X1; x<WIN_X2-1; x++)
        {
            if(0 == block_screen[y][x])
                break;
	     len++;
        }
        if(len == WIDTH)/*把后面的往上复制*/
        {
            DrawScreen(0);
            usleep(50000);/*暂停0.05秒*/
            sum++;
            for(i = y; i>(WIN_Y2-giHight-2); i--)
            {
                memcpy(&block_screen[i][1],&block_screen[i-1][1],sizeof(int)*WIDTH);
            }
            DrawScreen(1);
            giHight--;
            y++;/*消除一行后整体下移，故y的坐标不变*/
        }
    }
    switch(sum)
    {
    case 1:
        score = 1;
        break;
    case 2:
        score = 3;
        break;
    case 3:
        score = 5;
        break;
    case 4:
        score = 10;
        break;
    default:
        break;
    }

    giScore+= score;/*判断得分*/

    giLevel = giScore/50;/*速度 关数*/
}
/*
void JudgeHight(ZzBody block)
{

}*/

static int zJudgeNew(ZzBody block,int dir)/*判断移动后的方块是否会撞到边界或是撞到已有的方块*/
{
    int i,j,height;
    if(dir == DOWN)/*是否撞到下边界  需要产生新的方块*/
    {
        for(i=0; i<4; i++)
        {
            if(block.ZBody[i][1] >= (WIN_Y2-1))
            {
                for(j=0; j<4; j++)
                {
                    height = WIN_Y2-block.ZBody[j][1];/*更新高度*/
                    if(height > giHight)
                        giHight = height;
                    block_screen[block.ZBody[j][1]-1][block.ZBody[j][0]-1] = 1;/*方块不再移动 固定在某个位置*/
                }
                //JudgeHight(block);
                if(giHight >= 20)/*到顶了 结束游戏*/
                    return -2;
                return -1;
            }
            if(block_screen[block.ZBody[i][1]][block.ZBody[i][0]-1] == 1)
            {
                for(j=0; j<4; j++)
                {
                    height = WIN_Y2-block.ZBody[j][1];
                    if(height > giHight)
                        giHight = height;
                    block_screen[block.ZBody[j][1]-1][block.ZBody[j][0]-1] = 1;
                }
                //JudgeHight(block);
                if(giHight >= 20)
                    return -2;
                return -1;
            }
        }
    }
    else if(dir == LEFT)/*是否碰到左边界*/
    {
        for(i=0; i<4; i++)
        {
            if(block.ZBody[i][0] <= (WIN_X1+1))
                return 0;
            if(block_screen[block.ZBody[i][1]-1][block.ZBody[i][0]-2] == 1)
                return 0;
        }
    }
    else if(dir == RIGHT)/*是否碰到右边界*/
    {
        for(i=0; i<4; i++)
        {
            if(block.ZBody[i][0] >= (WIN_X2-1))
                return 0;
            if(block_screen[block.ZBody[i][1]-1][block.ZBody[i][0]] == 1)
                return 0;
        }
    }
    return 1;
}

static void JudgeNext(ZzBody block)/*判断旋转后的方块是否会撞到边界或是撞到已有的方块*/
{
    int i;
    for(i=0; i<4; i++)
    {
        if(block_screen[block.ZBody[i][1]-1][block.ZBody[i][0]-1] == 1)/*撞到已有的方块*/
        {
            giMoveErr = 1;
            return;
        }
        if((block.ZBody[i][0] >= WIN_X2) || (block.ZBody[i][0] <= WIN_X1) || (block.ZBody[i][1] >= WIN_Y2))/*撞到边界*/
        {
            giMoveErr = 1;
            return;
        }
    }
}
void *Send_Down_Cmd()
{
    int dw;
    int time;
    char ch[2];
    ch[1] = 1;

    while(1)
    {
        time = MAX_TIME - (MAX_TIME-MIN_TIME)/MAX_LEVEL*giLevel;
        usleep(time);
        write(giPipeFd,ch,sizeof(ch));
    }
}


void InitThread()
{
    pthread_t thread;
    mkfifo(PIPE_NAME,0600);

    giPipeFd = open(PIPE_NAME,O_RDWR);

    if(pthread_create(&thread,0,(void*)Send_Down_Cmd,(void*)0) == 0)
    {
        printf("Error:pthread_create\n");
        return;
    }
    sleep(1);
}
/*************************************************
main 主函数
**************************************************/
int main()
{
    int i,j,zDir,flag;
    ZzBody hh,hh_bak;
    int numb=0;


    InitThread();/*add 2014-1029*/
    ZInitScreen();
    ZDrawScope();

    while(1)
    {
        if(giNewBlock == 0)/*需要产生新的*/
        {
            /*			if(numb)
            				goto LOOP;*/
            JudgeScreen();
            printf("\033[%dm\033[40;%dm\033[%d;%dH%s%d",1,1*3+30,24,1,"The Score:",giScore);
            ZInitBlock(&hh,0);
            giNewBlock = 1;
            //numb++;
        }
        if(!giMoveErr)
        {
            memcpy(&hh_bak,&hh,sizeof(ZzBody));
            zDrawBlock(hh_bak,1);
        }
        else
        {
            memcpy(&hh,&hh_bak,sizeof(ZzBody));
        }
        giMoveErr = 0;
        zDir = ZGetDir();
        printf("\033[%dm\033[40;%dm\033[%d;%dH%s%d",1,1*3+30,23,1,"The giHeight:",giHight);
#if 1
        flag = zJudgeNew(hh,zDir);
        if(flag == -2)/*判断左、右、下的移动*/
        {
            printf("\033[%dm\033[40;%dm\033[%d;%dH%s",1,1*3+30,13,3,OVER);
            printf("\033[%dm\033[40;%dm\033[%d;%dH%s%d",1,1*3+30,14,3,"Your score:",giScore);
            goto LOOP;
        }
        else if(flag == -1)
        {
            giNewBlock = 0;
            continue;
        }
        else if(flag == 0)
        {
            continue;
        }
#endif

        fflush(stdout);

        zNextBlock(&hh,zDir);

        JudgeNext(hh);

        if(!giMoveErr)
            zDrawBlock(hh_bak,0);
    }
LOOP:
    sleep(1);
    ZRestoreScreen();
#if 0
    for(i = 0; i<WIN_Y2; i++)
    {
        for(j=0; j<WIN_X2; j++)
        {
            printf(" %d",block_screen[i][j]);
        }
        printf("\n");
    }
#endif
    return 0;
    /*
    	for(i=0;i<4;i++)
    	{
    		printf(" [%d,%d]",hh.ZBody[i][0],hh.ZBody[i][1]);
    	}
    	printf("\n");
    */
}

