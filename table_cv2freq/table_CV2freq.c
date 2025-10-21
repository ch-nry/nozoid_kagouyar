// compilation :
// gcc -o table_CV2freq table_CV2freq.c -lm

#include <stdio.h>
#include <math.h>

void main(void){
  double const power = pow(2.0,1/12.0);
  for (int i=0; i<269; i++) {
    float tmp = pow(power, i-(128+69)) * 440.;
	printf("%0.5f, ",tmp);
    }
}
