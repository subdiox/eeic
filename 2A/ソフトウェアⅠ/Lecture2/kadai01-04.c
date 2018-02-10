#include <stdio.h>

#define IN        1
#define OUT       0
#define MAXLEN    25
#define MAXNWORD  23000

int get_max( int array[], int N )
{
    int max = array[0];
    for (int i = 0; i < N; i ++) {
        if (max < array[i]) {
            max = array[i];
        }
    }
    return max;
}

double get_average( int array[], int N )
{
    int sum = 0;
    for (int i = 0; i < N; i ++) {
        sum += array[i];
    }
    double average = (double)sum / (double)N;
    return average;
}

double get_variance( int array[], int N )
{
    double average = get_average(array, N);
    double ns2 = 0;
    for (int i = 0; i < N; i ++) {
        ns2 += (array[i] - average) * (array[i] - average);
    }
    return ns2 / (double)N;
}

void bubble_sort( int array[], int N )
{
}


int main()
{
  int c, nl, nw, nc, state;
  int i, j;
  int word_initial, word_final, len;
  int word_length[MAXLEN];
  int numbers[MAXNWORD];

  for( i = 0 ; i < MAXLEN ; ++i ) {
    word_length[i] = 0;
  }
  for( i = 0 ; i < MAXNWORD ; ++i ) {
    numbers[i] = 0;
  }

  state = OUT;
  nl = nw = nc = 0;
  while( ( c = getchar() ) != EOF ) {
    ++nc;

    if( c == ' ' || c == '\n' || c == '\t' ) {
      if( state == IN ) {
	word_final = nc;
	len = word_final - word_initial;
	++word_length[len];

	numbers[nw] = (nl+1)*len;
	//	printf( "nw = %d, nl = %d, len = %d, numbers = %d\n", nw, nl, len, numbers[nw] );
	++nw;
      }
      state = OUT;
    }
    else if( state == OUT ) {
      state = IN;
      word_initial = nc;
    }

    if( c == '\n' ) {
      ++nl;
    }
  }

  printf( "#line = %d #word = %d #character = %d\n", nl, nw, nc );
  printf( "max = %d, avg = %.3f, var = %.3f\n", get_max( numbers, nw ),
	  get_average( numbers, nw ), get_variance( numbers, nw ) );

  return 0;
}
