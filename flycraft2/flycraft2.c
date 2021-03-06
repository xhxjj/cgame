#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if(ch != EOF)
    {
      ungetc(ch, stdin);
      return 1;
    }
  return 0;
}

int getch(void)
{
  struct termios tm, tm_old;
  int fd = STDIN_FILENO, c;

  if(tcgetattr(fd, &tm) < 0)
    return -1;

  tm_old = tm;

  cfmakeraw(&tm);

  if(tcsetattr(fd, TCSANOW, &tm) < 0)
    return -1;

  c = fgetc(stdin);

  if(tcsetattr(fd, TCSANOW, &tm_old) < 0)
    return -1;

  return c;
}

//全局变量
int position_x,position_y;//飞机位置
int high, width;//游戏画面尺寸
int bullet_x,bullet_y;//子弹位置
int enemy_x,enemy_y;//敌机位置
int score;//得分
#define S 300

void startup() // 数据初始化
{
  high       =  20; 
  width      =  30;
  position_x =  high / 2;
  position_y =  width / 2;
  bullet_x   =  -1;
  bullet_y   =  position_y;
  enemy_x    =  0;
  enemy_y    =  position_y;
  score      =  0;
}

void show() //显示画面
{
  system("clear"); //清屏
  int i,j,k;
  for (i = 0; i < high; i++)
    {
      for(j = 0; j < width; j++)
	{
           if((i == position_x) && (j == position_y))
	     {
	        printf("*\n");
	        for(k = 0; k < j - 2; k++)
	          printf(" ");
	        printf("*****\n");
	        for(k = 0; k < j - 1; k++)
	          printf(" ");
	        printf("* *");
	     }// 输出飞机
	   else if((i == enemy_x) && (j == enemy_y))
	     printf("@");//输出敌机
           else if ((i == bullet_x) && (j == bullet_y))
	     printf("|");
           else
	     printf(" "); // 输出空格
	}
      printf("\n");
    }
  printf("得分：%d\n",score);
}

void updatewithoutinput() //与用户输入无关的更新
{
  if(bullet_x > -1)
    {
       bullet_x--;
       usleep(100000);
    }
  
  if((bullet_x == enemy_x) && (bullet_y == enemy_y))//子弹击中敌机
    {
      score++;//分数加1
      enemy_x = -1;
      enemy_y = rand()%width;//产生新的飞机
      bullet_x = -2;//子弹无效
    }

  if(enemy_x > high)//敌机跑出显示屏幕
    {
      enemy_x = -1;
      enemy_y = rand()%width;
    }

  static int speed = 0;
  if(speed < S)
    speed++;
  if(speed == S)
    {
      enemy_x++;
      speed = 0;
    }
}

void updatewithinput()//  与用户输入有关的更新
{
  char input ;
  if(kbhit())//判断是否有输入
    {
      input = getch();
      if(input == 'a')
	position_y--;
      if(input == 's')
	position_x++;
      if(input == 'd')
	position_y++;
      if(input == 'w')
	position_x--;
      if(input == ' ')
	{
	  bullet_x = position_x - 1; 
	  bullet_y = position_y;
	}
    }
}

int main()
{
  startup();//数据初始化

  while(1)
    {
      show(); //显示界面
      updatewithoutinput();// 与用户输入无关的更新
      updatewithinput();// 与用户输入有关的更新
    }
  return 0;
}
