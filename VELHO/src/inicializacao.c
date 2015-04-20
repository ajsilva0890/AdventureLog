// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//Biblioteca dos structs
#include "structs.h" 
//Biblioteca Tela de Poderes (Todas as funcoes relativas a tela de poderes)
#include "funcPoderes.h"
//Biblioteca que gera questoes
#include "equacoes.h"

#include "inicializacao.h"
//#include "destruir.h"
#include <stdio.h>

              
int inicializadores(int *LARGURA_TELA, int *ALTURA_TELA, float *FPS, ALLEGRO_DISPLAY *janela , ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *poderes, ALLEGRO_FONT *fonte_equacao, ALLEGRO_FONT *fonte_pontos, ALLEGRO_TIMER *timer){

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

    fonte_equacao = al_load_font("fontes/letra_equacao.ttf", 36, 0);
    if (!fonte_equacao){
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }

    fonte_pontos = al_load_font("fontes/letra_equacao.ttf", 26, 0);
    if (!fonte_pontos){
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

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

}













/*
int inicializadores(){

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

    fonte_equacao = al_load_font("fontes/letra_equacao.ttf", 36, 0);
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

}
*/