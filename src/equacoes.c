#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structs.h" 
 

/* ale_parte1 é a parte fácil da Tela de Poderes
	multiplicação com primeiro número variando de 1 a 5 ( 1>=num1<=5) 
	e segundo numero variando de 1 a 10 (1>=num2<=10) */

void ale_parte1(listaQuestao *questao){
  int lima=5, limb=10;
  int margem, margem2, distancia_alt=6, distancia_alt2=2;  
  int ale_alt, qtd_alt=(3)-1, aux;

  srand(time(NULL));
  
  questao->produto = (rand() / (float) RAND_MAX) * lima + 1;

  questao->numero = (rand() / (float) RAND_MAX) * limb + 1;

  questao->alternativa_A = questao->produto * questao->numero;
  questao->resposta      = questao->produto * questao->numero;

  do{
  	margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    //margem2 = (rand() / (float) RAND_MAX) * distancia_alt2 + 0.5;  
  	questao->alternativa_B = questao->resposta + margem;
  	
  } while (questao->alternativa_B == questao->resposta || questao->alternativa_B == 0 );


  do{
  	margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    //margem2 = (rand() / (float) RAND_MAX) * distancia_alt2 + 0.5;
  	questao->alternativa_C = questao->resposta - margem;
  	
  } while (questao->alternativa_C == questao->resposta || questao->alternativa_B == questao->alternativa_C || questao->alternativa_C == 0   );


  ale_alt = (rand() / (float) RAND_MAX) * qtd_alt + 0.5;  

  switch(ale_alt) {
  	case 0: aux = questao->alternativa_B ;
  			    questao->alternativa_B  = questao->alternativa_C;
  			    questao->alternativa_C = aux;
			      break;

	  case 1: aux = questao->alternativa_A;
  			    questao->alternativa_A = questao->alternativa_C;
  			    questao->alternativa_C = aux;
			      break;

	  case 2:	aux = questao->alternativa_A;
  			    questao->alternativa_A = questao->alternativa_B;
  			    questao->alternativa_B  = aux;
			      break;  	
  }
}



/* ale_parte2 é a parte fácil da Tela de Poderes
	multiplicação com primeiro número variando de 1 a 5 ( 1>=num1<=5) 
	e segundo numero variando de 1 a 10 (1>=num2<=10) 
	mais uma adição de uma váriavel de 1 a 10
*/


void ale_parte2 (listaQuestao *questao){
  int lima=5, limb=10;
  int margem, distancia_alt=6;
  int ale_alt, qtd_alt=(3)-1, aux;

  srand(time(NULL));

  questao->produto = (rand() / (float) RAND_MAX) * lima + 1;

  questao->numero = (rand() / (float) RAND_MAX) * limb + 1;


  questao->soma = (rand() / (float) RAND_MAX) * limb + 1;

  questao->resposta = questao->produto * questao->numero + questao->soma;
  questao->alternativa_A = questao->resposta;    

  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_B = questao->resposta + margem;
    } while (questao->alternativa_B == questao->resposta);


  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_C = questao->resposta - margem;
  } while (questao->alternativa_C == questao->resposta);


  ale_alt = (rand() / (float) RAND_MAX) * qtd_alt + 0.5;  

  switch(ale_alt) {
    case 0: aux = questao->alternativa_B ;
            questao->alternativa_B  = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 1: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 2: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_B;
            questao->alternativa_B  = aux;
            break;    
  }
}



/* ale_parte3 é a parte médio da Tela de Poderes
	multiplicação com primeiro número variando de 5 a 10 ( 5>=num1<=10) 
	e segundo numero variando de 3 a 10 (3>=num2<=10) */

void ale_parte3(listaQuestao *questao){
  int lima=5, limb=10;
  int margem, distancia_alt=6;
  int ale_alt, qtd_alt=(3)-1, aux;

  srand(time(NULL));


  do{
    questao->produto = (rand() / (float) RAND_MAX) * limb + 1;
  } while(questao->produto<5);  
   
  do{
    questao->numero = (rand() / (float) RAND_MAX) * limb + 1;
  } while(questao->numero<3);  


  questao->resposta = questao->produto * questao->numero;
  questao->alternativa_A = questao->resposta;


  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_B = questao->resposta + margem;
  } while (questao->alternativa_B==questao->resposta);

  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_C = questao->resposta - margem;
  } while (questao->alternativa_C==questao->resposta);


  ale_alt = (rand() / (float) RAND_MAX) * qtd_alt + 0.5;  

  switch(ale_alt) {
    case 0: aux = questao->alternativa_B ;
            questao->alternativa_B  = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 1: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 2: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_B;
            questao->alternativa_B  = aux;
            break;    
  }
}



/* ale_parte4 é a parte médio da Tela de Poderes
	multiplicação com primeiro número variando de 5 a 10 ( 5>=num1<=10) 
	e segundo numero variando de 3 a 10 (3>=num2<=10) 
	mais uma adição com uma váriavel de 5 a 15 */
	
void ale_parte4 (listaQuestao *questao){

  int lima=5, limb=10, limsoma=15; 
  int margem, distancia_alt=6;
  int ale_alt, qtd_alt=(3)-1, aux;

  srand(time(NULL));

  do{
    questao->produto = (rand() / (float) RAND_MAX) * limb + 1;

  } while(questao->produto<5);  
   
  do{
    questao->numero = (rand() / (float) RAND_MAX) * limb + 1;
  } while(questao->numero<3);  

  do{
    questao->soma = (rand() / (float) RAND_MAX) * limsoma + 1;
  } while(questao->soma<5);


  questao->resposta= questao->produto * questao->numero + questao->soma;
  questao->alternativa_A = questao->resposta;

  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_B = questao->resposta + margem + questao->soma + 2;

  } while (questao->alternativa_B ==questao->resposta);

  do{
    margem = (rand() / (float) RAND_MAX) * distancia_alt + 0.5;  
    questao->alternativa_C = questao->resposta - margem + questao->soma - 2;

  } while (questao->alternativa_C ==questao->resposta);


  ale_alt = (rand() / (float) RAND_MAX) * qtd_alt + 0.5;  

  switch(ale_alt) {
    case 0: aux = questao->alternativa_B ;
            questao->alternativa_B  = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 1: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_C;
            questao->alternativa_C = aux;
            break;

    case 2: aux = questao->alternativa_A;
            questao->alternativa_A = questao->alternativa_B;
            questao->alternativa_B  = aux;
            break;    
  }

}