// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include <stdlib.h>
#include "pi_tools.h"
#include "pi_data.h"

//Biblioteca dos structs
#include "structs.h"
//Biblioteca Tela de Poderes (Todas as funcoes relativas a tela de poderes)
#include "funcPoderes.h"
//Biblioteca que gera questoes
#include "equacoes.h"
#include "funcBoneco.h"

//#include "inicializacao.h"
//#include "destruir.h"
#include <stdio.h>



// Atributos da tela
const int LARGURA_TELA = 1280;   //960;
const int ALTURA_TELA =  720;  //540;
const float FPS = 60.0;
const float animacaoFPS = 60.0;


    GameScreen telaJogo; // tela original do jogo.
    GameDisplay displayJogo = {.mode = 0}; // display onde aparecem as telas de Aventura e Poderes
    //ALLEGRO_DISPLAY     *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos	= NULL, *menuFila = NULL;
    ALLEGRO_BITMAP      *poderes		= NULL;
    ALLEGRO_FONT        *fonte_80   	= NULL;
    ALLEGRO_FONT        *fonte_50   	= NULL;
    ALLEGRO_TIMER       *timer			= NULL, *menuTimer = NULL;
    ALLEGRO_SAMPLE      *som_errou      = NULL;
    ALLEGRO_SAMPLE      *som_acertou    = NULL;
    ALLEGRO_BITMAP      *chapeu         = NULL;
    ALLEGRO_BITMAP      *oculos         = NULL;
    ALLEGRO_BITMAP      *gravata        = NULL;
    ALLEGRO_BITMAP      *boneco         = NULL;
    ALLEGRO_BITMAP      *botaoTutorial  = NULL;
    ALLEGRO_SAMPLE      *musicaMenu     = NULL;
    ALLEGRO_SAMPLE      *musicaJogo     = NULL;

    ALLEGRO_SAMPLE_ID   musicaMenuID; 
    ALLEGRO_SAMPLE_ID   musicaJogoID; 


    ALLEGRO_BITMAP *menu     = NULL;
    ALLEGRO_BITMAP *fim      = NULL;

    ALLEGRO_BITMAP *tutorial_01 = NULL;
    ALLEGRO_BITMAP *tutorial_02 = NULL;
    ALLEGRO_BITMAP *tutorial_03 = NULL;
    ALLEGRO_BITMAP *tutorial_04 = NULL;
    ALLEGRO_BITMAP *tutorial_05 = NULL;
    ALLEGRO_BITMAP *tutorial_06 = NULL;
    ALLEGRO_BITMAP *tutorial_07 = NULL;
    ALLEGRO_BITMAP *tutorial_08 = NULL;
    ALLEGRO_BITMAP *tutorial_09 = NULL;
    ALLEGRO_BITMAP *tutorial_10 = NULL;
    ALLEGRO_BITMAP *tutorial_11 = NULL;
    ALLEGRO_BITMAP *tutorial_12 = NULL;
    ALLEGRO_BITMAP *tutorial_13 = NULL;
    ALLEGRO_BITMAP *tutorial_14 = NULL;
    ALLEGRO_BITMAP *tutorial_15 = NULL;
    ALLEGRO_BITMAP *tutorial_16 = NULL;
    ALLEGRO_BITMAP *tutorial_17 = NULL;

    ALLEGRO_BITMAP *piso = NULL;
    ALLEGRO_BITMAP *ceu = NULL; 
    ALLEGRO_BITMAP *lua = NULL; 



SpriteSheet sFantasmas, sLapidesCruzes, sGrama1, sGrama2, sArvores1, sArvores2,
        sNevoa1, sNevoa2, sNevoa3, sNevoa4, sNevoa5, sNevoa6, sFumacas, sMontanhas;

Icones iPoderes; //***** FELIPE
Icones iVidas; //***** FELIPE


int  inicializadores();
void destruir ();
int jogar(listaQuestao *);


int gameover(listaQuestao *);
int tuto();
int inicial();




/* Para ativar/desativar o debug, descomentar/comentar a linha abaixo */
//#define _SET_DEBUG_ON
#ifdef  _SET_DEBUG_ON
    #define DEBUG_ON printf
#endif
#ifndef _SET_DEBUG_ON
    #define DEBUG_ON //
#endif

//----------------------------------------------------------------------

int main(int argc, char **argv[]){

    //al_register_event_source(fila_eventos, al_get_timer_event_source(timerAnimacao));


    printf("\n================================");
    printf("\ndebug:main():start");
    printf("\n================================");


    int spriteNum = -1;
    int i, j;
    srand(10);

    /* Inicia todos os addons utilizados no jogo. */
    if (pi_iniAllegroAddons(&displayJogo) < 0)
        return -1;

    /* Configura as telas do jogo */
    pi_iniScreens(&telaJogo); // Inicializa a tela do jogo.
    DEBUG_ON("\ndebug:telaJogo:x2=%f", telaJogo.x2);

    // Inicializa as imagens e elementos do jogo
    pi_iniImagens(&sLapidesCruzes, &sFantasmas, &sGrama1, &sGrama2, &sArvores1, &sArvores2, &sNevoa1,
                    &sNevoa2, &sNevoa3, &sNevoa4, &sNevoa5, &sNevoa6, &sFumacas, &sMontanhas, &iPoderes, &iVidas, &telaJogo);

    /* Inicializa o jogo em tela cheia */
    if (pi_criaDisplay(&telaJogo, &displayJogo) < 0)
        return -1;

    
     if (inicializadores()<0){
        return -1;
    }
    

    
    int c = 2;
    listaQuestao feedback;
    al_start_timer(menuTimer);

    /*
    menuFila = al_create_event_queue();
    if (!menuFila){
         fprintf(stderr, "Falha ao criar fila do menu.\n");
    }
    al_register_event_source(menuFila, al_get_timer_event_source(menuTimer));
    al_register_event_source(menuFila, al_get_mouse_event_source());
    //FIM FILA MENU

    al_start_timer(menuTimer);
      */  
    al_play_sample(musicaMenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicaMenuID);

    while(1){

        if (c == 1){

            c = tuto();

        }

        if (c == 0) {   
            al_flush_event_queue(menuFila);

            al_stop_timer(menuTimer);
            al_destroy_event_queue(menuFila);

            fila_eventos = al_create_event_queue();
             if (!fila_eventos){
                fprintf(stderr, "Falha ao criar fila do jogo.\n");
            }
            al_register_event_source(fila_eventos, al_get_mouse_event_source());
            al_register_event_source(fila_eventos, al_get_display_event_source(displayJogo.backbuffer));
            al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
            al_register_event_source(fila_eventos, al_get_keyboard_event_source());

            al_stop_sample(&musicaMenuID);
            //al_play_sample(musicaJogo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicaJogoID);
            //al_rest(1);

            
            al_start_timer(timer);
            
            al_flush_event_queue(fila_eventos);
            c = jogar(&feedback);
            al_flush_event_queue(fila_eventos);

            al_stop_sample(&musicaJogoID);
            al_play_sample(musicaMenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicaMenuID);

            al_stop_timer(timer);
            al_destroy_event_queue(fila_eventos);
            
            //FILA MENU
            menuFila = al_create_event_queue();
            if (!menuFila){
                 fprintf(stderr, "Falha ao criar fila do menu.\n");
            }
            al_register_event_source(menuFila, al_get_timer_event_source(menuTimer));
            al_register_event_source(menuFila, al_get_mouse_event_source());

            al_register_event_source(menuFila, al_get_display_event_source(displayJogo.backbuffer));
            //FIM FILA MENU

            al_start_timer(menuTimer);
            al_flush_event_queue(menuFila);

        }

        if (c == 2){

            c = inicial();

        }

        if (c == 3){

            c = gameover(&feedback);

        }

        if (c == -1){
            break;
        }

    }


    destruir ();

    return 0;

}


int inicial(){

//al_flush_event_queue(fila_eventos);

int redrawInicial = 0, drawTuto = 0;
    cor corSair, corJogar, corTuto;


    corSair.R = 0;    corSair.G = 0;    corSair.B = 0;
    corJogar.R = 0;   corJogar.G = 0;   corJogar.B = 0; 
    corTuto.R = 240;  corTuto.G = 195;  corTuto.B = 42; 


    al_draw_bitmap(menu, 0, 0, 0);
    /*al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 1030, 540, ALLEGRO_ALIGN_CENTRE, "sair");
    al_flip_display();
    */

    while (1){
        
        ALLEGRO_EVENT eventoMENU;
        al_wait_for_event(menuFila, &eventoMENU);

        if (eventoMENU.type == ALLEGRO_EVENT_TIMER) {
           
            redrawInicial = 1;
        }

            if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_AXES){

                if (eventoMENU.mouse.x > 0 /* 765 */   && 
                    eventoMENU.mouse.x < 270 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720/* 266 */  ){ 

                    drawTuto = 1;
                    al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (eventoMENU.mouse.x > 430 /* 765 */ && 
                        eventoMENU.mouse.x < 850 /* 878 */ &&
                        eventoMENU.mouse.y > 475 /* 172 */ &&
                        eventoMENU.mouse.y < 720 /* 266 */ ){ 

                        corJogar.R = 240;
                        corJogar.G = 195;
                        corJogar.B = 42;

                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (eventoMENU.mouse.x > 1010 /* 765 */ && 
                        eventoMENU.mouse.x < 1280 /* 878 */ &&
                        eventoMENU.mouse.y > 475 /* 172 */  &&
                        eventoMENU.mouse.y < 720 /* 266 */  ){ 

                        corSair.R = 240;
                        corSair.G = 195;
                        corSair.B = 42;
                        
                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }
                else{
                        corSair.R = 0;   corSair.G = 0;   corSair.B = 0;
                        corJogar.R = 0;  corJogar.G = 0;  corJogar.B = 0;
                        drawTuto = 0;

                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

               
            }


            else if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

                if (eventoMENU.mouse.x > 0 /* 765 */   &&
                    eventoMENU.mouse.x < 270 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720/* 266 */  ){

                    return 1;
                }

                if (eventoMENU.mouse.x > 430 /* 765 */ &&
                    eventoMENU.mouse.x < 850 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720 /* 266 */ ){

                        
                    return 0;
                }

                if (eventoMENU.mouse.x > 1010 /* 765 */ &&
                    eventoMENU.mouse.x < 1280 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */  &&
                    eventoMENU.mouse.y < 720 /* 266 */  ){

                    return -1;

                }
            }

            else if (eventoMENU.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    return -1;

            }   
        


        if (redrawInicial == 1 /*&& al_is_event_queue_empty(fila_eventos)*/){
            redrawInicial = 0;
            al_draw_bitmap(menu, 0, 0, 0);

            if (drawTuto == 1){
                
                al_draw_bitmap(botaoTutorial, 27, ALTURA_TELA - al_get_bitmap_height(botaoTutorial), 0);
                al_draw_text(fonte_50, al_map_rgb(corTuto.R,  corTuto.G ,  corTuto.B),  170, 470, ALLEGRO_ALIGN_CENTRE, "tutorial");
            }
            
            al_draw_text(fonte_50, al_map_rgb(corSair.R,  corSair.G ,  corSair.B),  1030, 540, ALLEGRO_ALIGN_CENTRE, "sair");
            al_draw_text(fonte_80, al_map_rgb(corJogar.R, corJogar.G , corJogar.B), 570, 450, ALLEGRO_ALIGN_CENTRE, "Jogar");
            
            al_flip_display();
            al_rest(0.001);
            
        }
    }

}



int tuto(){

    int redrawTuto = 0;
    int telaTutorial = 1;

    cor corSair, corProx, corVoltar;
    corSair.R   = 33;    corSair.G   = 52;    corSair.B   = 73;
    corProx.R   = 0;    corProx.G   = 0;    corProx.B   = 0;
    corVoltar.R = 0;    corVoltar.G = 0;    corVoltar.B = 0;

    while (1){

        if (telaTutorial > 17){
            return 0;
        }

        ALLEGRO_EVENT eventoMENU;
        al_wait_for_event(menuFila, &eventoMENU);
        
        if (eventoMENU.type == ALLEGRO_EVENT_TIMER) {
           
            redrawTuto = 1;
        }

        if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_AXES){

                if (eventoMENU.mouse.x > 0 /* 765 */   && 
                    eventoMENU.mouse.x < 270 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720/* 266 */  ){ 

                    corVoltar.R = 240;
                    corVoltar.G = 195;
                    corVoltar.B = 42;

                    al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (eventoMENU.mouse.x > 0 /* 765 */ && 
                        eventoMENU.mouse.x < 100 /* 878 */ &&
                        eventoMENU.mouse.y > 0 /* 172 */ &&
                        eventoMENU.mouse.y < 100 /* 266 */ ){ 
                        
                        corSair.R = 240;
                        corSair.G = 195;
                        corSair.B = 42;

                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);

                }

                else if (eventoMENU.mouse.x > 1010 /* 765 */ && 
                        eventoMENU.mouse.x < 1280 /* 878 */ &&
                        eventoMENU.mouse.y > 475 /* 172 */  &&
                        eventoMENU.mouse.y < 720 /* 266 */  ){ 
                        
                        corProx.R = 240;
                        corProx.G = 195;
                        corProx.B = 42;
                        
                        
                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }
                else{
                        corSair.R   = 33;  corSair.G   = 52;  corSair.B   = 73;
                        corProx.R   = 0;  corProx.G   = 0;  corProx.B   = 0;
                        corVoltar.R = 0;  corVoltar.G = 0;  corVoltar.B = 0;

                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

               
            }


        if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

            if (eventoMENU.mouse.x > 0 /* 765 */   && 
                eventoMENU.mouse.x < 270 /* 878 */ &&
                eventoMENU.mouse.y > 475 /* 172 */ &&
                eventoMENU.mouse.y < 720/* 266 */  ){ 

                if (telaTutorial == 1){
                    return 2;
                }
                telaTutorial -= 1;
            }

            if (eventoMENU.mouse.x > 0 /* 765 */ && 
                eventoMENU.mouse.x < 100 /* 878 */ &&
                eventoMENU.mouse.y > 0 /* 172 */ &&
                eventoMENU.mouse.y < 100 /* 266 */ ){ 

                return 2;
            }

            if (eventoMENU.mouse.x > 1010 /* 765 */ && 
                eventoMENU.mouse.x < 1280 /* 878 */ &&
                eventoMENU.mouse.y > 475 /* 172 */  &&
                eventoMENU.mouse.y < 720 /* 266 */  ){ 

                
                telaTutorial += 1;
        }

        else if (eventoMENU.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                return -1;
        }



        }
      if (redrawTuto == 1 && al_is_event_queue_empty(menuFila)){
        redrawTuto = 0;
        
        switch(telaTutorial){
            case 1:
                 al_draw_bitmap(tutorial_01, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "menu");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;

            case 2:
                 al_draw_bitmap(tutorial_02, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;

            case 3:
                 al_draw_bitmap(tutorial_03, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
           
            case 4:
                 al_draw_bitmap(tutorial_04, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 5:
                 al_draw_bitmap(tutorial_05, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 6:
                 al_draw_bitmap(tutorial_06, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 7:
                 al_draw_bitmap(tutorial_07, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 8:
                 al_draw_bitmap(tutorial_08, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 9:
                 al_draw_bitmap(tutorial_09, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 10:
                 al_draw_bitmap(tutorial_10, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 11:
                 al_draw_bitmap(tutorial_11, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 12:
                 al_draw_bitmap(tutorial_12, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 13:
                 al_draw_bitmap(tutorial_13, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 14:
                 al_draw_bitmap(tutorial_14, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 15:
                 al_draw_bitmap(tutorial_15, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 16:
                 al_draw_bitmap(tutorial_16, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "prox.");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");

                 al_flip_display();
                 break;
            
            case 17:
                 al_draw_bitmap(tutorial_17, 0, 0, 0);
                 al_draw_text(fonte_50, al_map_rgb(corVoltar.R,  corVoltar.G ,  corVoltar.B),  90, 575, ALLEGRO_ALIGN_CENTRE, "voltar");
                 al_draw_text(fonte_50, al_map_rgb(corProx.R, corProx.G , corProx.B), 1190, 575, ALLEGRO_ALIGN_CENTRE, "jogar");
                 al_draw_text(fonte_50, al_map_rgb(corSair.R, corSair.G , corSair.B), 50, 20, ALLEGRO_ALIGN_CENTRE, "sair");
                 
                 al_flip_display();
                 break;

        }

        al_flush_event_queue(menuFila);
        al_rest(0.001);


        }
        else
            al_flush_event_queue(menuFila);

    }

}



int gameover(listaQuestao *feedback){

    int redrawGameover = 0;
    cor corSair, corJogar, corTuto;


    corSair.R = 0;    corSair.G = 0;    corSair.B = 0;
    corJogar.R = 0;   corJogar.G = 0;   corJogar.B = 0; 
    corTuto.R = 240;  corTuto.G = 195;  corTuto.B = 42; 


    while (1){
        
        ALLEGRO_EVENT eventoMENU;
        al_wait_for_event(menuFila, &eventoMENU);

        if (eventoMENU.type == ALLEGRO_EVENT_TIMER) {
           
            redrawGameover = 1;
        }

            if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_AXES){

                if (eventoMENU.mouse.x > 0 /* 765 */   && 
                    eventoMENU.mouse.x < 270 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720/* 266 */  ){ 

                    corSair.R = 240;
                    corSair.G = 195;
                    corSair.B = 42;

                    al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (eventoMENU.mouse.x > 430 /* 765 */ && 
                        eventoMENU.mouse.x < 850 /* 878 */ &&
                        eventoMENU.mouse.y > 475 /* 172 */ &&
                        eventoMENU.mouse.y < 720 /* 266 */ ){ 

                        

                        al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }

                else if (eventoMENU.mouse.x > 1010 /* 765 */ && 
                        eventoMENU.mouse.x < 1280 /* 878 */ &&
                        eventoMENU.mouse.y > 475 /* 172 */  &&
                        eventoMENU.mouse.y < 720 /* 266 */  ){ 
                        /*
                        corSair.R = 240;
                        corSair.G = 195;
                        corSair.B = 42;
                        */
                        
                        //al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
                }
                else{
                        corSair.R = 0;   corSair.G = 0;   corSair.B = 0;
                        //corJogar.R = 0;  corJogar.G = 0;  corJogar.B = 0;

                        //al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                }

               
            }


            else if (eventoMENU.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

                if (eventoMENU.mouse.x > 0 /* 765 */   &&
                    eventoMENU.mouse.x < 270 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720/* 266 */  ){

                    return 2;
                }

                if (eventoMENU.mouse.x > 430 /* 765 */ &&
                    eventoMENU.mouse.x < 850 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */ &&
                    eventoMENU.mouse.y < 720 /* 266 */ ){

                  //  al_stop_timer(menuTimer);
                    return 0;
                }

                if (eventoMENU.mouse.x > 1010 /* 765 */ &&
                    eventoMENU.mouse.x < 1280 /* 878 */ &&
                    eventoMENU.mouse.y > 475 /* 172 */  &&
                    eventoMENU.mouse.y < 720 /* 266 */  ){

                    return -1;

                }
            }

            else if (eventoMENU.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    return -1;

            }   

        


        if (redrawGameover == 1){

            redrawGameover = 0;
            al_draw_bitmap(fim, 0, 0, 0);

            if (feedback->tempo == 0){
                al_draw_text(fonte_80, al_map_rgb(157, 11 , 43), 1280/2, 720/4 +50, ALLEGRO_ALIGN_CENTRE, "Seu tempo acabou! ");
            }
            else {
                al_draw_text(fonte_50, al_map_rgb(157, 11 , 43), 1280/2, 720/4 +50, ALLEGRO_ALIGN_CENTRE, "Suas vidas acabaram! ");
                al_draw_textf(fonte_50, al_map_rgb(123,123,123), 1280/2, 720/4 + 150, ALLEGRO_ALIGN_CENTRE,  "Tempo restante: %d segundos ", feedback->tempo );
            }

            al_draw_text(fonte_80, al_map_rgb(157, 11 , 43), 1280/2, 100, ALLEGRO_ALIGN_CENTRE, "FIM DE JOGO!");
            al_draw_textf(fonte_50, al_map_rgb(123,123,123), 1280/2, 720/4 +250, ALLEGRO_ALIGN_CENTRE    , "Total de pontos: %d",  feedback->pontos);
            al_draw_textf(fonte_50, al_map_rgb(123,123,123), 1280/2, 720/4 +350, ALLEGRO_ALIGN_CENTRE    , "Total de acertos: %d", feedback->acertos );
            al_draw_textf(fonte_50, al_map_rgb(123,123,123), 1280/2, 720/4 +400, ALLEGRO_ALIGN_CENTRE    , "Total de erros: %d",   feedback->erros );

            al_draw_text(fonte_50, al_map_rgb(corSair.R,  corSair.G ,  corSair.B),  210, 550, ALLEGRO_ALIGN_CENTRE, "menu");
            
            al_flip_display();
            al_rest(0.001);
            
        }
    }

}



int jogar (listaQuestao *feedback){

    
    pi_iniImagens(&sLapidesCruzes, &sFantasmas, &sGrama1, &sGrama2, &sArvores1, &sArvores2, &sNevoa1,
                    &sNevoa2, &sNevoa3, &sNevoa4, &sNevoa5, &sNevoa6, &sFumacas, &sMontanhas, &iPoderes, &iVidas, &telaJogo); //***** FELIPE



    int frame = 0;
    
    
    al_set_target_backbuffer(displayJogo.backbuffer);
    al_set_clipping_rectangle(0, 0, displayJogo.largura, displayJogo.altura);

	int counter;

    al_flip_display();

    /*
    if (inicializadores()<0){
        return -1;
    }
    */

    // Flag que condicionará nosso looping do jogo
    int sair = 0;
    // Flag que condicionará nosso redesenho
    int redraw = 0;

    // Caixas da colisao Largura e Altura
    int caixaL, caixaA;
    float contagem = 0;

    //Struct para pergunta e alternativas
    listaQuestao questionario;


    //Struct para a posicao das alternativas da tela
    telaAlternativa posicaoAlt;

    /*
    //teste posicao das questoes
    printf("%d -- %d\n %d -- %d\n %d -- %d\n",posicaoAlt.alt_Ax, posicaoAlt.alt_Ay,
                                              posicaoAlt.alt_Bx, posicaoAlt.alt_By,
                                              posicaoAlt.alt_Cx, posicaoAlt.alt_Cy );
    */

    fantasma identidade;
    bonecoJog jogador;

    setBoneco(LARGURA_TELA, ALTURA_TELA, &jogador, boneco);

    posicaoAlternativas(LARGURA_TELA, ALTURA_TELA, &posicaoAlt, &jogador);

    //setar valor antes de iniciar o jogo
    setVeriavel(&questionario);

    // Inicializando perguntas com nivel de dificuldade 1 (parte1)
    ale_parte1(&questionario);

	/* Cria um timer separado para redesenhar o cenário e os fantasmas */
	ALLEGRO_TIMER *timerAnimacao = al_create_timer(1.0 / animacaoFPS);
	al_register_event_source(fila_eventos, al_get_timer_event_source(timerAnimacao));



    // timer printipal do jogo
    //al_start_timer(timer);

    /* Cria um timer separado para o redesenho dos gráficos */
	al_start_timer(timerAnimacao);
    //timer com tempo real para o cronometro do jogo
    //al_start_timer(contador);

    //som jogo
    al_play_sample(musicaJogo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicaJogoID);

while (!sair){

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        posicaoAlt.mousePoderes_x = evento.mouse.x;
        posicaoAlt.mousePoderes_y = evento.mouse.y;


        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
             return -1;
        }
        
        else if(evento.type == ALLEGRO_EVENT_TIMER) {
        // controlando FPS para redesenhar

			if (evento.timer.source == timerAnimacao){
				/* Calcula as novas coordenadas e animações dos elementos */
				pi_AnimarSpriteSheet(&sFantasmas, &sFumacas, &telaJogo);
                pi_AnimarSpriteSheet(&sMontanhas,       NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sFumacas, 		NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sLapidesCruzes, 	NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sGrama2, 			NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sGrama1, 			NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sArvores2, 		NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sArvores1, 		NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sNevoa1,          NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sNevoa2,          NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sNevoa3,          NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sNevoa4, 			NULL, &telaJogo);

				if(jogador.acao == 0){
					jogadorCorrer(&jogador);
				}
				else if(jogador.acao == 1 ){
					jogadorPulo(&jogador);
				}
				else if(jogador.acao == 2 ){
					jogadorAbaixar(&jogador);
				}
                
                contagem += 0.0166666666667;
                if (contagem >= 1){
                    cronometroRegressivo(&questionario, &sair);
                    contagem = 0;
                }
                

			}
			redraw = 1;
            DEBUG_ON("\ndebug:main():event.type:timer ");
        }


/*///////////////////////// COMANDOS ALLEGRO_EVENT_QUEUE /**fila_eventos\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//----- verifica se tipo de evento é clique no mouse --- E IFS PARA VERIFICAR QUAL ALTERNATIVA JOGADOR ESCOLHEU -------------

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

             verificarResposta(som_acertou, som_errou, &posicaoAlt, &questionario, &sFantasmas, &jogador);

              if ( questionario.acertou == 1 ){
                pi_atualizaIcones(&iPoderes, 1); //*** FELIPE
                pi_atualizaIcones(&iVidas, 1); //*** FELIPE
                questionario.acertou = -2;
              }

        }  //FIM DE VERIFICAÇÃO SOBRE A ESCOLHA DA ALTERNATIVA DO JOGADOR

        //mudando tipo de ponteiro quando jogador for responder
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){

             ponteiroMouse(displayJogo.backbuffer, &posicaoAlt);
        }


        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE ){
                //questionario.tempo = 30;
                 //al_destroy_timer(contador);
                //al_destroy_event_queue(fila_eventos);
                //al_stop_timer(timer);
                *feedback = questionario;
                return 3;
            }

            if (iPoderes.valorAtual > 0){

                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_W:
                         pi_atualizaIcones(&iPoderes, -1);
                         clique_KEY_W(&jogador);
                         break;

                    case ALLEGRO_KEY_S:
                         pi_atualizaIcones(&iPoderes, -1);
                         clique_KEY_S(&jogador);
                         break;
                }
            }
        }

/*---------------VERIFICAÇÃO COLISAO FANTASMA x JOGADOR-------------------------------------------------------*/
            if (jogador.acao == 0){
                 caixaL = -45;
                 caixaA = -15;
            }
            else if (jogador.acao == 1){
                 caixaL = -45;
                 caixaA = -10;
            }
            else if (jogador.acao == 2){
                 caixaL = -20;
                 caixaA = -60;
            }

            for (int i = 0; i < 3; i++){

                //***************** ATUALIZACAO 30/11/2014!
                if ( sFantasmas.eliminado[ i ] == NAO){
                    if (colisao (jogador.pos_x,      jogador.pos_y,      jogador.frame_larg, jogador.frame_alt,
                                 sFantasmas.posX[i], sFantasmas.posY[i], sFantasmas.largura, sFantasmas.altura,
                                 caixaL, caixaA) == 1){

                            sFantasmas.eliminado[i] = SIM;                  
                            pi_atualizaIcones(&iVidas, -1); 
                            // verifica se acabaram as vidas do jogador
                            if ( iVidas.valorAtual == iVidas.minValor ) {
                                *feedback = questionario;
                                al_rest(2);
                                return 3;                                   
                            }                                              
                    }
                }
                //*************** FIM ATUALIZACAO!
            }



/*---------------VERIFICAÇÃO ATUALIZAÇÃO DA PERGUNTA-------------------------------------------------------*/
            atualizarPergunta(&questionario);


/* ----------------- TEMPO RECRESIVO NA TELA -------------------------------------------------- */
            //cronometroRegressivo(contador, fila_contador, &questionario, &sair);



   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
/* /////////////////////// AREA DE DESENHAR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
        al_flush_event_queue(menuFila);
        if (redraw && al_is_event_queue_empty(fila_eventos) && evento.timer.source == timerAnimacao && al_is_event_queue_empty(menuFila)) {
            redraw = 0;

            pi_drawGraphics(&sMontanhas);
//            al_draw_bitmap(montanha_01, 0, 0, 0);
			/** Desenhar na ordem de profundidade no cenário. Começando pelo mais distante para o mais perto **/
			al_draw_bitmap(ceu, 0, 0, 0);
			al_draw_scaled_bitmap(lua, 0, 0, 256, 230, (telaJogo.largura - 500), (telaJogo.altura - 420), (256*2), (230*2), 0);
			//al_draw_bitmap(lua, (telaJogo.largura - 500), (telaJogo.altura - 420), 0);
			//al_draw_bitmap(piso, 0, telaJogo.altura- 10, 0);

			/* Desenha os elementos do cenario que ficarão ao fundo */
			pi_drawGraphics(&sNevoa1);
			pi_drawGraphics(&sArvores2);
			pi_drawGraphics(&sNevoa2);
			pi_drawGraphics(&sArvores1);
			pi_drawGraphics(&sNevoa3);
			pi_drawGraphics(&sGrama2);
			pi_drawGraphics(&sLapidesCruzes);
			pi_drawGraphics(&sNevoa4);
        
			drawResposdeu(fonte_80, &questionario, &posicaoAlt);
            drawPergunta(fonte_50, fonte_80, &questionario, &posicaoAlt, chapeu, oculos, gravata);
            drawPontos (fonte_50, &questionario, &posicaoAlt, &jogador);
            drawJogador(&jogador, boneco);

			/* Desenha os elementos que ficarão na frente do personagem */
			pi_drawGraphics(&sFantasmas);
			pi_drawGraphics(&sFumacas);
			pi_drawGraphics(&sGrama1);

            pi_drawIcones (&iPoderes);
            pi_drawIcones (&iVidas);    

			al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_rest(0.001);
        }

    }

    al_rest(2);
    *feedback = questionario;
    return 3;

}

int inicializadores(){

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    al_init_image_addon();

    // Inicialização do add-on para uso de fontes
    al_init_font_addon();

    al_install_keyboard();

    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return -1;
    }

    
    timer = al_create_timer(1.0 / FPS);
    if(!timer){
        al_destroy_display(displayJogo.backbuffer);
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }
    

    // Configura o título da janela
    al_set_window_title(displayJogo.backbuffer, "Adventure*Log");

    fonte_80 = al_load_font("fontes/letra_equacao.ttf", 80, 0);
    if (!fonte_80){
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }

    fonte_50 = al_load_font("fontes/letra_equacao.ttf", 50, 0);
    if (!fonte_50){
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }


    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        return -1;
    }

    /*
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        return -1;
    }
    */

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_errou = al_load_sample("sons/sample_erro.ogg");
    if (!som_errou){
        fprintf(stderr, "Falha ao carregar som_errou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_acertou = al_load_sample("sons/sample_acertou.ogg");
    if (!som_acertou){
        fprintf(stderr, "Falha ao carregar som_acertou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        return false;
    }

    
    menuTimer = al_create_timer(1.0 / FPS /*1.0*/);
    if (!menuTimer)
    {
        fprintf(stderr, "Falha ao criar timer.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
    //  al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        al_destroy_sample(som_acertou);
        return false;
    }
    
    
    menuFila = al_create_event_queue();
    if (!menuFila)
    {
        fprintf(stderr, "Falha ao criar fila do contador.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        al_destroy_sample(som_acertou);
        al_destroy_timer(menuTimer);

        return false;
    }

    musicaMenu = al_load_sample("musica/menu.ogg");
    if (!musicaMenu){
        fprintf(stderr, "Falha ao carregar som jogo.\n");
    }

    musicaJogo = al_load_sample("musica/jogo.ogg");
    if (!musicaJogo){
        fprintf(stderr, "Falha ao carregar som menu.\n");
    }
    
    
    

    oculos  = al_load_bitmap("img/itens/oculos.png");
    chapeu  = al_load_bitmap("img/itens/chapeu.png");
    gravata = al_load_bitmap("img/itens/gravata.png");


    boneco = al_load_bitmap("img/boneco/jogador.png");


    menu     = al_load_bitmap("img/menu.png");
    fim      = al_load_bitmap("img/fim.png");
    
    botaoTutorial = al_load_bitmap("img/botaotutorial.png");


    tutorial_01  = al_load_bitmap("img/tutorial/tutorial_01.png");
    tutorial_02  = al_load_bitmap("img/tutorial/tutorial_02.png");
    tutorial_03  = al_load_bitmap("img/tutorial/tutorial_03.png");
    tutorial_04  = al_load_bitmap("img/tutorial/tutorial_04.png");
    tutorial_05  = al_load_bitmap("img/tutorial/tutorial_05.png");
    tutorial_06  = al_load_bitmap("img/tutorial/tutorial_06.png");
    tutorial_07  = al_load_bitmap("img/tutorial/tutorial_07.png");
    tutorial_08  = al_load_bitmap("img/tutorial/tutorial_08.png");
    tutorial_09  = al_load_bitmap("img/tutorial/tutorial_09.png");
    tutorial_10  = al_load_bitmap("img/tutorial/tutorial_10.png");
    tutorial_11  = al_load_bitmap("img/tutorial/tutorial_11.png");
    tutorial_12  = al_load_bitmap("img/tutorial/tutorial_12.png");
    tutorial_13  = al_load_bitmap("img/tutorial/tutorial_13.png");
    tutorial_14  = al_load_bitmap("img/tutorial/tutorial_14.png");
    tutorial_15  = al_load_bitmap("img/tutorial/tutorial_15.png");
    tutorial_16  = al_load_bitmap("img/tutorial/tutorial_16.png");
    tutorial_17  = al_load_bitmap("img/tutorial/tutorial_17.png");


    piso = al_load_bitmap("img/ground/png/ground.png");
    
    ceu = al_load_bitmap("img/sky/png/sky.png");
    
    lua = al_load_bitmap("img/sky/png/lua.png");



    // Dizemos que vamos tratar os eventos vindos do mouse
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());
    //al_register_event_source(fila_eventos, al_get_display_event_source(displayJogo.backbuffer));
    //al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    //al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    al_register_event_source(menuFila, al_get_timer_event_source(menuTimer));
    al_register_event_source(menuFila, al_get_mouse_event_source());

}

/*
int pi_drawGraphics(ALLEGRO_BITMAP *image, float x, float y, int refresh){
    DEBUG_ON("\n----debug:drawGraphics():start");
    //DEBUG_ON("\ndebug:tela:%d", tela->id);

    if (refresh){
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_draw_bitmap(image, x, y, 0);

    DEBUG_ON("\n----debug:drawGraphics():end");

    return 0;
}
*/


void destruir(){

    /* DESTRUINDO PONTEIROS */
    al_destroy_bitmap(poderes);
   // al_destroy_display(janela);
    //al_destroy_event_queue(fila_eventos);
                                                                //al_destroy_event_queue(fila_contador);
    al_destroy_font(fonte_80);
    al_destroy_font(fonte_50);
    al_destroy_timer(timer);
                                                                //al_destroy_timer(contador);
    al_destroy_sample(som_errou);
    al_destroy_sample(som_acertou);
    al_destroy_bitmap(chapeu);
    al_destroy_bitmap(oculos);
    al_destroy_bitmap(gravata);
    al_destroy_bitmap(boneco);

    al_destroy_bitmap(menu);
    al_destroy_bitmap(fim);


    al_destroy_bitmap(tutorial_01);
    al_destroy_bitmap(tutorial_02);
    al_destroy_bitmap(tutorial_03);
    al_destroy_bitmap(tutorial_04);
    al_destroy_bitmap(tutorial_05);
    al_destroy_bitmap(tutorial_06);
    al_destroy_bitmap(tutorial_07);
    al_destroy_bitmap(tutorial_08);
    al_destroy_bitmap(tutorial_09);
    al_destroy_bitmap(tutorial_10);
    al_destroy_bitmap(tutorial_11);
    al_destroy_bitmap(tutorial_12);
    al_destroy_bitmap(tutorial_13);
    al_destroy_bitmap(tutorial_14);
    al_destroy_bitmap(tutorial_15);
    al_destroy_bitmap(tutorial_16);
    al_destroy_bitmap(tutorial_17);

    al_destroy_bitmap(botaoTutorial);

    al_destroy_display(displayJogo.backbuffer);
    al_destroy_bitmap(telaJogo.canvas);
    al_destroy_sample(musicaMenu);
    al_destroy_sample(musicaJogo);

    al_destroy_bitmap(piso); 
    
    al_destroy_bitmap(ceu );
    
    al_destroy_bitmap(lua );

    al_destroy_bitmap(sFantasmas.bitmap);
    al_destroy_bitmap(sLapidesCruzes.bitmap);
    al_destroy_bitmap(sGrama1.bitmap);
    al_destroy_bitmap(sGrama2.bitmap);
    al_destroy_bitmap(sArvores1.bitmap);
    al_destroy_bitmap(sArvores2.bitmap);
    al_destroy_bitmap(sNevoa1.bitmap);
    al_destroy_bitmap(sNevoa2.bitmap);
    al_destroy_bitmap(sNevoa3.bitmap);
    al_destroy_bitmap(sNevoa4.bitmap);
    al_destroy_bitmap(sNevoa5.bitmap);
    al_destroy_bitmap(sNevoa6.bitmap);
    al_destroy_bitmap(sFumacas.bitmap);
    al_destroy_bitmap(sMontanhas.bitmap);


   // al_destroy_bitmap(telaPoderes.canvas);
   // al_destroy_bitmap(telaAventura.canvas);
}
