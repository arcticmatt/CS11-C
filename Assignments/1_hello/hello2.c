#include <stdio.h>

int main(void)
{
  char name[99];

  printf("Enter your name: ");
  scanf("%99s", name);
  printf("hello, %s!\n", name);

  return 0;
}
