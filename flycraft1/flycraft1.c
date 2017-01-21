#include <stdio.h>
#include <stdlib.h>
#include <termios.h>  
#include <unistd.h>  
#include <fcntl.h>  
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

int main()
{
  int i,j;
  int x = 5;
  int y = 10;
  char input;
  int isfire = 0;

  int ny = 5;
  int iskilled = 0;

  while(1)
    {
      system("clear");
      if(!iskilled)
	{
	  for(j = 0; j < y; j++)
	    printf(" ");
	  printf("@\n");
	}
      
      if(isfire == 0)
	{
           for(i = 0; i < x; i++)
	     printf("\n");
	}
      else 
	{
	  for(i = 0; i < x; i++)
	    {
	      for(j = 0; j < y; j++)
		printf(" ");
	      printf(" |\n");
	    }
	  isfire = 0;
	}

      for(j = 0; j < y; j++)
	printf(" ");
      printf("  *\n");
      
      for(j = 0; j < y; j++)
	printf(" ");
      printf("*****\n");

      for(j = 0; j < y; j++)
	printf(" ");
      printf(" * *\n");
      printf("\n");

      if(kbhit())
	{   
           input = getch();
           if(input == 'a')
             y--;
           if(input == 's')
	     x++;
           if(input == 'd')
	     y++;
           if(input == 'w')
	     x--;
	}
    }
  return 0;
}
