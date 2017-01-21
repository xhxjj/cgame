#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
  int i,j;
  int x = 5;
  int y = 10;

  int vecolicity_x = 1;
  int vecolicity_y = 1;

  int top = 0;
  int bottom = 10;
  int left = 0;
  int right = 20;

  while(1)
    {
      x += vecolicity_x;
      y += vecolicity_y;
      system("clear");

      for(i = 0; i < x; i++)
        printf("\n");
      for(j = 0; j < y; j++)
        printf(" ");
      printf("o");
      printf("\n");
      usleep(100000);

      if((x == top) || (x == bottom))
        vecolicity_x = -vecolicity_x;
      if((y == left) || (y == right))
        vecolicity_y = -vecolicity_y;
    }
}
