#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
  int n;
  int i;
  char name[99];

  srand(time(0));
  n = rand() % 10 + 1;
  printf("Enter your name: ");
  scanf("%99s", name);
  for(i = 0; i < n; i++)
  {
    if(n%2 == 0) {
      printf("%d: hello, %s!\n", n, name);
    }
    else {
      printf("%d: hi there, %s!\n", n, name);
    }
  }

  return 0;
}
