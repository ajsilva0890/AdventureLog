#include "equacoes.h"
#include <stdio.h>

int main() {  

  int tam1a=5, tam1b=10;
  int parte1[6];  

  ale_parte1(tam1a, tam1b, parte1);
  
  printf("%d * %d = \n", parte1[0], parte1[1]);
  printf("%d --- %d --- %d \n", parte1[3], parte1[4], parte1[5]);
  printf("\n \n");


  ale_parte2(tam1a, tam1b, parte1);
  
  printf("%d * %d + %d = \n", parte1[0], parte1[1], parte1[2] );
  printf("%d --- %d --- %d \n", parte1[3], parte1[4], parte1[5]);
  printf("\n \n");


  ale_parte3(tam1b, tam1b, parte1);

  printf("%d * %d = \n", parte1[0], parte1[1]);
  printf("%d --- %d --- %d \n", parte1[3], parte1[4], parte1[5]);
  printf("\n \n");

  ale_parte4(tam1b, tam1b, parte1);

  printf("%d * %d + %d = \n", parte1[0], parte1[1], parte1[2] );
  printf("%d --- %d --- %d \n", parte1[3], parte1[4], parte1[5]);
  printf("\n \n");

  return 0;
}