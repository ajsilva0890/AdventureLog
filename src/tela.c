// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

//Biblioteca das Equacoes 
#include "equacoes.h"
#include "structs.h"
 
// Para utilizarmos a função fprintf
#include <stdio.h>

// Atributos da tela
const int LARGURA_TELA = 960;
const int ALTURA_TELA = 540;
const float FPS = 1;


int main(void)
{
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_BITMAP *poderes = NULL;
    ALLEGRO_FONT *fonte_equacao = NULL;
    ALLEGRO_TIMER *timer = NULL;

    pergunta p;
    //Pontuação pelas equacoes respondidas certas
    int pontos;
    // Flag que condicionará nosso looping
    int sair = 0;
    //Variaveis para gerar eguações aleatorias
    int tam1a=5, tam1b=10;
 	int parte1[7];
 	int meio2 = (LARGURA_TELA/3)*2;

 	int meio_resp_A = (LARGURA_TELA-meio2)/4 + meio2; 
 	int meio_resp_B	= ((LARGURA_TELA-meio2)/4) * 2 + meio2;
 	int meio_resp_C	= ((LARGURA_TELA-meio2)/4) * 3 + meio2;


    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }
 	
 	al_init_image_addon();

 	// Inicialização do add-on para uso de fontes
    al_init_font_addon();
 
    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return -1;
    }
 	
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
 
    timer = al_create_timer(1.0 / FPS);
   	if(!timer){
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }

    // Configura o título da janela
    al_set_window_title(janela, "PENAS - Telas de Poderes -");

    fonte_equacao = al_load_font("letra_equacao.ttf", 36, 0);
    if (!fonte_equacao){
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }
 
    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    } 
 
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
 
    al_start_timer(timer);

    poderes = al_load_bitmap("fundo.jpg");
     // Desenha a imagem na tela

    ale_parte1(tam1a, tam1b, parte1);

    while (!sair){

    	ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
	    
	    al_draw_bitmap(poderes, 0, 0, 0);

		if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
         	sair=1;


        
        //PERGUNTA
        al_draw_textf(fonte_equacao, al_map_rgb(0, 0, 0), meio2 + (meio2/3), ALTURA_TELA/3, ALLEGRO_ALIGN_CENTRE, "%d * %d = ", parte1[0], parte1[1]);
        
        //ALTERNATIVAS
        al_draw_textf(fonte_equacao, al_map_rgb(0, 0, 0), meio_resp_A, (ALTURA_TELA/3)*2, ALLEGRO_ALIGN_CENTRE , "a) %d                 ", parte1[3]);
        al_draw_textf(fonte_equacao, al_map_rgb(0, 0, 0), meio_resp_B, (ALTURA_TELA/3)*2, ALLEGRO_ALIGN_CENTRE ,          "b) %d        ", parte1[4]);
        al_draw_textf(fonte_equacao, al_map_rgb(0, 0, 0), meio_resp_C, (ALTURA_TELA/3)*2, ALLEGRO_ALIGN_CENTRE ,                 "c) %d ", parte1[5]);
	    
        
	    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
	    	if (evento.mouse.x > meio2 && 
	    		evento.mouse.x < LARGURA_TELA &&
	    		evento.mouse.y > 0 &&
	    		evento.mouse.y < ALTURA_TELA){
            
	    	//-----------------------------------------------
	    		//VERIFICAR SE RESPOSTA ESTA CERTA




	    	//-----------------------------------------------//

            // Funcao equacoes
	    		ale_parte1(tam1a, tam1b, parte1);
	   		}	
        }

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x > meio_resp_A-20 && 
                    evento.mouse.x < meio_resp_A+20 &&
                    evento.mouse.y > (ALTURA_TELA/3)*2 - 20 &&
                    evento.mouse.y < (ALTURA_TELA/3)*2 + 20 ){

                    
                        if(parte1[6]==parte1[3])
                            pontos = 1; 
                        else
                            pontos = 0;
                }
            }

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x > meio_resp_B-20 && 
                    evento.mouse.x < meio_resp_B+20 &&
                    evento.mouse.y > (ALTURA_TELA/3)*2 - 20 &&
                    evento.mouse.y < (ALTURA_TELA/3)*2 + 20 ){

                    
                        if(parte1[6]==parte1[4])
                            pontos = 1; 
                        else
                            pontos = 0;
                }
            }

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x > meio_resp_C-200 && 
                    evento.mouse.x < meio_resp_C+200 &&
                    evento.mouse.y > (ALTURA_TELA/3)*2 - 200 &&
                    evento.mouse.y < (ALTURA_TELA/3)*2 + 200 ){

                    
                        if(parte1[6]==parte1[5])
                            pontos = 1; 
                        else
                            pontos = 0;
                }
            }




        if(parte1[6]==parte1[3])
        	al_draw_text(fonte_equacao, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "A");

        if(parte1[6]==parte1[4])
        	al_draw_text(fonte_equacao, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "B");

        if(parte1[6]==parte1[5])
        	al_draw_text(fonte_equacao, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "C");

        
        while(pontos==0 || pontos==1){
            if (pontos==1 )
                al_draw_text(fonte_equacao, al_map_rgb(255, 0, 0), 100, 100, ALLEGRO_ALIGN_LEFT, "CERTO");
            
            if (pontos==0 )
                al_draw_text(fonte_equacao, al_map_rgb(255, 0, 0), 100, 100, ALLEGRO_ALIGN_LEFT, "ERRADO");


            if(evento.type == ALLEGRO_EVENT_TIMER)
    			sair=0;

            ale_parte1(tam1a, tam1b, parte1);
        }
        // Atualiza a tela
	    al_flip_display();
	

	}
	 
	    // Desaloca os recursos utilizados na aplicação
	    al_destroy_bitmap(poderes);
	    al_destroy_display(janela);
	    al_destroy_event_queue(fila_eventos);
	    al_destroy_font(fonte_equacao);
	    al_destroy_timer(timer);
	
 
  return 0;
}