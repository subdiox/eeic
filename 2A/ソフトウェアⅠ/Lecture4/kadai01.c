#include <stdio.h>
#include <stdlib.h>

int main()
{
  int *i,*sum;
  sum = malloc(sizeof(int));
  *sum = 0;

  i = malloc(sizeof(int));
  for (*i=0; *i<=10; *i+=1){
    *sum += *i;
  }

  printf("sum = %d\n", *sum);

  free(i);
  free(sum);
  
  return 0;
}
