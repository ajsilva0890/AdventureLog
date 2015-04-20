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

// Atributos da tela
const int LARGURA_TELA = 1920;   //960;    
const int ALTURA_TELA =  1080;  //540;     
const float FPS = 60;
    

    GameScreen telaAventura;
    GameScreen telaPoderes;
    GameScreen telaJogo; // tela original do jogo.
    GameDisplay displayJogo = {.mode = 0}; // display onde aparecem as telas de Aventura e Poderes
    //ALLEGRO_DISPLAY     *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos	= NULL, *fila_contador = NULL;
    ALLEGRO_BITMAP      *poderes		= NULL;
    ALLEGRO_FONT        *fonte_equacao	= NULL;
    ALLEGRO_FONT        *fonte_pontos	= NULL;
    ALLEGRO_TIMER       *timer			= NULL, *contador = 0;
    ALLEGRO_SAMPLE      *som_errou		= NULL;
    ALLEGRO_SAMPLE      *som_acertou	= NULL;
    ALLEGRO_BITMAP      *fantasma1		= NULL;
    ALLEGRO_BITMAP      *fantasma2		= NULL;
    ALLEGRO_BITMAP      *fantasma3		= NULL;
    ALLEGRO_BITMAP      *boneco			= NULL;

int inicializadores();
void destruir ();

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


/* Para ativar/desativar o debug, descomentar/comentar a linha abaixo */
//#define _SET_DEBUG_ON
#ifdef  _SET_DEBUG_ON
    #define DEBUG_ON printf
#endif
#ifndef _SET_DEBUG_ON
    #define DEBUG_ON //
#endif

//----------------------------------------------------------------------
//int pi_drawGraphics(ALLEGRO_BITMAP *image, float x, float y, int refresh);

int main(int argc, char **argv[]){
    
    printf("\n================================");
    printf("\ndebug:main():start");
    printf("\n================================");
   
    
    SpriteGroup sCeu;
    SpriteGroup sArvores1;
    SpriteGroup sArvores2;
    SpriteGroup sLapides;
    SpriteGroup sNevoa1;
    SpriteGroup sNevoa2;
    SpriteGroup sPiso;
    SpriteSheet sFantasmas;
    SpriteSheet sLapidesCruzes;
    SpriteSheet sGrama1, sGrama2;

    int spriteNum = -1;
    int i, j;
    srand(10);

    /* Inicia todos os addons utilizados no jogo. */
    if (pi_iniAllegroAddons(&displayJogo) < 0)
        return -1;

	
    /* Configura as telas do jogo*/
    pi_iniScreens(&telaJogo, &telaAventura, &telaPoderes); // Inicializa as telas do jogo.
    DEBUG_ON("\ndebug:telaJogo:x2=%f", telaJogo.x2);

	// Inicializa as imagens e elementos do jogo
	pi_iniImagens(&sLapidesCruzes, &sFantasmas, &sGrama1, &telaJogo);

    /** Configura SPRITES **/
    pi_iniSpriteGroup(&sPiso, &telaJogo, ID_GROUP_SPRITES_GROUND);
    pi_loadStillSprite(&sPiso, "ground", "ground");

    pi_iniSpriteGroup(&sNevoa1, &telaJogo, ID_GROUP_SPRITES_FOG);
    pi_loadStillSprite(&sNevoa1, "fog2", "fog2");
    pi_loadStillSprite(&sNevoa1, "fog3", "fog3");
    pi_loadStillSprite(&sNevoa1, "fog4", "fog4");
    pi_loadStillSprite(&sNevoa1, "fog5", "fog5");
    pi_loadStillSprite(&sNevoa1, "fog6", "fog6");

    pi_iniSpriteGroup(&sNevoa2, &telaJogo, ID_GROUP_SPRITES_FOG);
    pi_loadStillSprite(&sNevoa2, "fog4", "fog4");
    pi_loadStillSprite(&sNevoa2, "fog6", "fog6");
    pi_loadStillSprite(&sNevoa2, "fog3", "fog3");
    pi_loadStillSprite(&sNevoa2, "fog5", "fog5");

    pi_iniSpriteGroup(&sCeu, &telaJogo, ID_GROUP_SPRITES_SKY);
    pi_loadStillSprite(&sCeu, "moon", "moon");
    pi_loadStillSprite(&sCeu, "sky", "moon");

    pi_iniSpriteGroup(&sArvores1, &telaJogo, ID_GROUP_SPRITES_TREES);
    pi_loadStillSprite(&sArvores1, "tree7", "tree3");
    pi_loadStillSprite(&sArvores1, "tree6", "tree6");
    pi_loadStillSprite(&sArvores1, "tree3", "tree3");
    pi_loadStillSprite(&sArvores1, "tree1", "tree1");
    pi_loadStillSprite(&sArvores1, "tree3", "tree3");
    pi_loadStillSprite(&sArvores1, "tree7", "tree7");
    pi_loadStillSprite(&sArvores1, "tree6", "tree1");

    pi_iniSpriteGroup(&sArvores2, &telaJogo, ID_GROUP_SPRITES_TREES);
    pi_loadStillSprite(&sArvores2, "tree8", "tree2");
    pi_loadStillSprite(&sArvores2, "tree2", "tree3");
    pi_loadStillSprite(&sArvores2, "tree5", "tree4");
    pi_loadStillSprite(&sArvores2, "tree2", "tree5");
    pi_loadStillSprite(&sArvores2, "tree4", "tree6");
    pi_loadStillSprite(&sArvores2, "tree8", "tree7");
    pi_loadStillSprite(&sArvores2, "tree5", "tree8");

//  spriteNum = pi_findSpriteByName(&sCeu, "moon");
    
    // Configura a nevoa
    for (i = 0; i < sNevoa1.arraySize - 1; i++){ 
        if (sNevoa1.spriteArray[i].canvas != NULL){
            sNevoa1.spriteArray[i].x1            = (i * sNevoa1.spriteArray[i].largura);
            //printf("\nx1:%f", sGrama1.spriteArray[i].x1);
            sNevoa1.spriteArray[i].y1            = 940;
            sNevoa1.spriteArray[i].offsetX       = sNevoa1.spriteArray[i].x1;
            sNevoa1.spriteArray[i].offsetY       = 940;
            sNevoa1.spriteArray[i].profundidade         = 0.85;
            sNevoa1.spriteArray[i].dirX    = -1;
            sNevoa1.spriteArray[i].dirY    = 1;
            sNevoa1.spriteArray[i].velX        = 1.0;
            sNevoa1.spriteArray[i].velY        = 0.0;
            sNevoa1.spriteArray[i].inicioY        = sNevoa1.spriteArray[i].y1;
            sNevoa1.spriteArray[i].inicioX        = 1920 + sNevoa1.spriteArray[i].x1;
            sNevoa1.spriteArray[i].fimY          = sNevoa1.spriteArray[i].y1;
            sNevoa1.spriteArray[i].fimX          = 0.0;
            //printf("\nstartX:%f", sNevoa1.spriteArray[i].inicioX);
            sNevoa1.spriteArray[i].id            = 4;
            sNevoa1.spriteArray[i].loop          = YES;
        }   
    }
    for (i = 0; i < sNevoa2.arraySize - 1; i++){    
        if (sNevoa2.spriteArray[i].canvas != NULL){
            sNevoa2.spriteArray[i].x1           = (i * sNevoa2.spriteArray[i].largura) + 200;
            //printf("\nx1:%f", sNevoa2.spriteArray[i].x1);
            sNevoa2.spriteArray[i].y1           = 940;
            sNevoa2.spriteArray[i].offsetX      = sNevoa2.spriteArray[i].x1;
            sNevoa2.spriteArray[i].offsetY      = 940;
            sNevoa2.spriteArray[i].profundidade        = 0.95;
            sNevoa2.spriteArray[i].dirX   = -1;
            sNevoa2.spriteArray[i].dirY   = 1;
            sNevoa2.spriteArray[i].velX       = 1.0;
            sNevoa2.spriteArray[i].velY       = 0.0;
            sNevoa2.spriteArray[i].inicioY       = sNevoa2.spriteArray[i].y1;
            sNevoa2.spriteArray[i].inicioX       = 1920 + sNevoa2.spriteArray[i].x1;
            sNevoa2.spriteArray[i].fimY         = sNevoa2.spriteArray[i].y1;
            sNevoa2.spriteArray[i].fimX         = 0.0;
            //printf("\nstartX:%f", sNevoa1.spriteArray[i].inicioX);
            sNevoa2.spriteArray[i].id           = 4;
            sNevoa2.spriteArray[i].loop         = YES;
        }   
    }

//  Configura a lua
        sCeu.spriteArray[0].x1            = 1200;
        sCeu.spriteArray[0].y1            = 210;
        sCeu.spriteArray[0].profundidade         = 0.1;
        sCeu.spriteArray[0].dirX    = -1;
        sCeu.spriteArray[0].dirY    = 1;
        sCeu.spriteArray[0].velX        = 0.4;
        sCeu.spriteArray[0].velY        = 0.0;
        sCeu.spriteArray[0].inicioY        = 0.0;
        sCeu.spriteArray[0].inicioX        = 0.0;
        sCeu.spriteArray[0].fimY          = 0.0;
        sCeu.spriteArray[0].fimX          = 0.0;
        sCeu.spriteArray[0].id            = 1;
        sCeu.spriteArray[0].loop          = NO;

//  Configura a o céu de fundo
        sCeu.spriteArray[1].x1            = 0;
        sCeu.spriteArray[1].y1            = 1080 - sCeu.spriteArray[1].altura;
        sCeu.spriteArray[1].profundidade         = 1.0;
        sCeu.spriteArray[1].dirX    = 1;
        sCeu.spriteArray[1].dirY    = 1;
        sCeu.spriteArray[1].velX        = 0.0;
        sCeu.spriteArray[1].velY        = 0.0;
        sCeu.spriteArray[1].inicioY        = 0.0;
        sCeu.spriteArray[1].inicioX        = 0.0;
        sCeu.spriteArray[1].fimY          = 0.0;
        sCeu.spriteArray[1].fimX          = 0.0;
        sCeu.spriteArray[1].id            = 1;
        sCeu.spriteArray[1].loop          = NO;

//  Configura o chão
        sPiso.spriteArray[0].x1             = 0;
        sPiso.spriteArray[0].y1             = 1080 - sPiso.spriteArray[0].altura;
        sPiso.spriteArray[0].profundidade          = 1.0;
        sPiso.spriteArray[0].dirX     = 1;
        sPiso.spriteArray[0].dirY     = 1;
        sPiso.spriteArray[0].velX         = 0.0;
        sPiso.spriteArray[0].velY         = 0.0;
        sPiso.spriteArray[0].inicioY         = 0.0;
        sPiso.spriteArray[0].inicioX         = 0.0;
        sPiso.spriteArray[0].fimY           = 0.0;
        sPiso.spriteArray[0].fimX           = 0.0;
        sPiso.spriteArray[0].id             = 1;
        sPiso.spriteArray[0].loop           = NO;

    // configura arvores profundidade 1
    for (i = 0; i < sArvores1.arraySize - 1; i++){   
        if (sArvores1.spriteArray[i].canvas != NULL){
            float r = (rand() / 100000000.0);
            //printf("\nrand:%f", r);
            sArvores1.spriteArray[i].x1          = (sArvores1.spriteArray[i].largura * i) * r + 50;
            sArvores1.spriteArray[i].y1          = telaJogo.scaledH - sArvores1.spriteArray[i].altura - 10;
            //printf("\ny1:%f", sArvores1.spriteArray[i].y1);
            sArvores1.spriteArray[i].offsetX     = sArvores1.spriteArray[i].x1;
            sArvores1.spriteArray[i].offsetY     = sArvores1.spriteArray[i].y1;
            sArvores1.spriteArray[i].profundidade       = 1.0;
            sArvores1.spriteArray[i].dirX  = -1;
            sArvores1.spriteArray[i].dirY  = 1;
            sArvores1.spriteArray[i].velX      = 1.0;
            sArvores1.spriteArray[i].velY      = 0.0;
            sArvores1.spriteArray[i].inicioY      = sArvores1.spriteArray[i].y1;
            sArvores1.spriteArray[i].inicioX      = telaJogo.scaledW + sArvores1.spriteArray[i].x1;
            sArvores1.spriteArray[i].fimY        = sArvores1.spriteArray[i].y1;
            sArvores1.spriteArray[i].fimX        = 0.0;
            sArvores1.spriteArray[i].id          = i;
            sArvores1.spriteArray[i].loop        = YES;
        }
    }

    // configura arvores profundidade 2
    for (i = 0; i < sArvores2.arraySize - 1; i++){  
        if (sArvores2.spriteArray[i].canvas != NULL){
            float r = (rand() / 100000000.0);
            //printf("\nrand:%f", r);
            sArvores2.spriteArray[i].x1             = 300 + (sArvores2.spriteArray[i].largura * i) * (r/2);
            sArvores2.spriteArray[i].y1             = 1080 - sArvores2.spriteArray[i].altura - 10;
            //printf("\ny1:%f", sArvores2.spriteArray[i].y1);
            sArvores2.spriteArray[i].offsetX        = sArvores2.spriteArray[i].x1;
            sArvores2.spriteArray[i].offsetY        = sArvores2.spriteArray[i].y1;
            sArvores2.spriteArray[i].profundidade          = 0.7;
            sArvores2.spriteArray[i].dirX     = -1;
            sArvores2.spriteArray[i].dirY     = 1;
            sArvores2.spriteArray[i].velX         = 1.0;
            sArvores2.spriteArray[i].velY         = 0.0;
            sArvores2.spriteArray[i].inicioY         = sArvores2.spriteArray[i].y1;
            sArvores2.spriteArray[i].inicioX         = 1920 + sArvores2.spriteArray[i].x1;
            sArvores2.spriteArray[i].fimY           = sArvores2.spriteArray[i].y1;
            sArvores2.spriteArray[i].fimX           = 0.0;
            sArvores2.spriteArray[i].id             = i;
            sArvores2.spriteArray[i].loop           = YES;

        }
    }
    
    /* Inicializa o jogo em tela cheia */
    if (pi_setFullScreen(&telaJogo, &displayJogo) < 0)
        return -1;

     /* Configura a escala das coordenadas e tamanho da imagem no display */
    pi_setDisplayScale(&telaJogo, &displayJogo);

    /* Configura as telas de Aventura e Poderes de acordo com a resolução original do jogo
       configurada em telaJogo */
    //  pi_setTelaAventura(&telaJogo, &telaAventura, &displayJogo);
    //  pi_setTelaPoderes(&telaJogo, &telaPoderes, &displayJogo);

    int frame = 0;
    ALLEGRO_BITMAP *piso = NULL;
    piso = al_load_bitmap("img/ground/png/ground.png");
    ALLEGRO_BITMAP *ceu = NULL;
    ceu = al_load_bitmap("img/sky/png/sky.png");
    ALLEGRO_BITMAP *lua = NULL;
    lua = al_load_bitmap("img/sky/png/moon.png");
    al_set_target_backbuffer(displayJogo.backbuffer);
    al_set_clipping_rectangle(0, 0, displayJogo.largura, displayJogo.altura);
	
	int counter;
	
    al_flip_display();

    if (inicializadores()<0){
        return -1;
    }

    // Flag que condicionará nosso looping do jogo
    int sair = 0;
    // Flag que condicionará nosso redesenho
    int redraw = 0;

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

    //load imagem de fundo temporaria
   // poderes = al_load_bitmap("imagens/fundo.jpg");
    //desenhando imagem temporaria de fundo
//    al_draw_bitmap(poderes, 0, 0, 0);
    // timer printipal do jogo
    al_start_timer(timer);

    //timer com tempo real para o cronometro do jogo
    al_start_timer(contador);

while (!sair){

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        posicaoAlt.mousePoderes_x = evento.mouse.x;
        posicaoAlt.mousePoderes_y = evento.mouse.y;

        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
             sair=1;
        }
        
        else if(evento.type == ALLEGRO_EVENT_TIMER) {
            // controlando FPS para redesenhar
			pi_AnimateSprite(&sArvores2, &telaJogo);
			pi_AnimateSprite(&sNevoa1, &telaJogo);
			pi_AnimateSprite(&sArvores1, &telaJogo);
			pi_AnimateSprite(&sLapides, &telaJogo);
			pi_AnimarSpriteSheet(&sFantasmas, &telaJogo);
			pi_AnimarSpriteSheet(&sLapidesCruzes, &telaJogo);
			pi_AnimarSpriteSheet(&sGrama1, &telaJogo);

            redraw = 1;

            DEBUG_ON("\ndebug:main():event.type:timer ");
                                   

//printf("\nFOI\n");
//return 0;
            
            
//          al_hold_bitmap_drawing(false);

//          al_hold_bitmap_drawing(true);
          //  al_hold_bitmap_drawing(false);

            if(jogador.acao==0){
                 jogadorCorrer(&jogador); 
            }

            else if(jogador.acao == 1 ){
                 jogadorPulo(&jogador); 
            }

            else if(jogador.acao == 2 ){
                 jogadorAbaixar(&jogador); 
            }
        }
            

/*///////////////////////// COMANDOS ALLEGRO_EVENT_QUEUE /**fila_eventos\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//----- verifica se tipo de evento é clique no mouse --- E IFS PARA VERIFICAR QUAL ALTERNATIVA JOGADOR ESCOLHEU -------------
        
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

             verificarResposta(som_acertou, som_errou, &posicaoAlt, &questionario, &identidade, &jogador);

        }  //FIM DE VERIFICAÇÃO SOBRE A ESCOLHA DA ALTERNATIVA DO JOGADOR 
       
        //mudando tipo de ponteiro quando jogador for responder
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
            
             ponteiroMouse (displayJogo.backbuffer, &posicaoAlt);
        }


        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE ){
                sair = 1;
            }

            if (jogador.forca >= 1){

                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_W:
                         clique_KEY_W(&jogador);
                         break;
                         
                    case ALLEGRO_KEY_S:
                         clique_KEY_S(&jogador);
                         break;
                }
            }
        }


/*---------------VERIFICAÇÃO ATUALIZAÇÃO DA PERGUNTA-------------------------------------------------------*/
            atualizarPergunta(&questionario);

        
/* ----------------- TEMPO RECRESIVO NA TELA -------------------------------------------------- */
            cronometroRegressivo(contador, fila_contador, &questionario, &sair);



   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        
/* /////////////////////// AREA DE DESENHAR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

        if (redraw && al_is_event_queue_empty(fila_eventos)) {
            redraw = 0;

            /** Desenhar na ordem de profundidade no cenário. Começando pelo mais distante para o mais perto **/

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(lua, sCeu.spriteArray[0].x1, 400, 0);
            al_draw_bitmap(ceu, 0, 950, 0);
            al_draw_bitmap(piso, 0, 1065, 0);


/*
            al_hold_bitmap_drawing(true);
            for (i = 0; i < sNevoa1.arraySize - 1; i++){ 
                if (sNevoa1.spriteArray[i].canvas != NULL){
                    al_draw_bitmap_region(sNevoa1.spriteArray[i].canvas, 0, 0,
                    sNevoa1.spriteArray[i].largura, sNevoa1.spriteArray[i].altura, sNevoa1.spriteArray[i].x1, sNevoa1.spriteArray[i].y1, 0);
                }
                else
                    break;
            }

            for (i = 0; i < sArvores2.arraySize - 1; i++){  
                if (sArvores2.spriteArray[i].canvas != NULL){
                    pi_drawGraphics(sArvores2.spriteArray[i].canvas, sArvores2.spriteArray[i].x1, sArvores2.spriteArray[i].y1, 0);
                }
                else
                    break;
            }

             for (i = 0; i < sGrama2.arraySize - 1; i++){  
                if (sGrama2.spriteArray[i].canvas != NULL){
                    //al_draw_bitmap(sGrama2.spriteArray[i].canvas, sGrama2.spriteArray[i].x1, sGrama2.spriteArray[i].y1, 0);
                    al_draw_bitmap_region(sGrama2.spriteArray[i].canvas, 0, 0,
								sGrama2.spriteArray[i].largura, sGrama2.spriteArray[i].altura,
								sGrama2.spriteArray[i].x1, sGrama2.spriteArray[i].y1, 0);
                }
                else
                    break;
            }

           for (i = 0; i < sArvores1.arraySize - 1; i++){   
                if (sArvores1.spriteArray[i].canvas != NULL){
                    al_draw_bitmap(sArvores1.spriteArray[i].canvas,  sArvores1.spriteArray[i].x1, sArvores1.spriteArray[i].y1, 0);
                    
                }
                else
                    break;
            }
         
            for (i = 0; i < sLapides.arraySize - 1; i++){   
                if (sLapides.spriteArray[i].canvas != NULL){
                  //  al_draw_bitmap(sLapides.spriteArray[i].canvas, sLapides.spriteArray[i].x1, sLapides.spriteArray[i].y1, 0);
                    al_draw_bitmap_region(sLapides.spriteArray[i].canvas, 0,
                    0, sLapides.spriteArray[i].largura, sLapides.spriteArray[i].altura,
                    sLapides.spriteArray[i].x1, sLapides.spriteArray[i].y1, 0);
                }
                else
                    break;
            }

			al_hold_bitmap_drawing(false);
*/
			pi_drawGraphics(&sLapidesCruzes);	
            drawResposdeu(fonte_equacao, &questionario, &posicaoAlt);
            drawPergunta(fonte_equacao, &questionario, &posicaoAlt);
            drawPontos (fonte_pontos, &questionario, &posicaoAlt, &jogador);
            drawJogador(&jogador, boneco);

          /*
            //desenhando fastasmas na tela --TESTE--
            if(identidade.id != 1){
                al_draw_bitmap(fantasma3, 20, 20, 0);
            }

            if(identidade.id != 2){
                al_draw_bitmap(fantasma2, 300, 300, 0);
            }

            if(identidade.id != 3){
                al_draw_bitmap(fantasma1, 100, 100, 0);
            }
            */

			pi_drawGraphics(&sFantasmas);	
			pi_drawGraphics(&sGrama1);	
			
            al_flip_display();
        } 
    }

    destruir ();

    return 0;
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
    
    /*
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){

        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }
    */
 
    timer = al_create_timer(1.0 / FPS);
    if(!timer){
        al_destroy_display(displayJogo.backbuffer);
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    // Configura o título da janela
    al_set_window_title(displayJogo.backbuffer, "Adventure*Log");

    fonte_equacao = al_load_font("fontes/letra_equacao.ttf", 30, 0);
    if (!fonte_equacao){
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }

    fonte_pontos = al_load_font("fontes/letra_equacao.ttf", 30, 0);
    if (!fonte_pontos){
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return -1;
    }

 
    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        return -1;
    }
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(displayJogo.backbuffer, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        return -1;
    } 

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
 
    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
 
    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_errou = al_load_sample("sons/sample_erro.ogg");
    if (!som_errou){
        fprintf(stderr, "Falha ao carregar som_errou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_acertou = al_load_sample("sons/sample_acertou.ogg");
    if (!som_acertou){
        fprintf(stderr, "Falha ao carregar som_acertou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        return false;
    }

    contador = al_create_timer(1.0);
    if (!contador)
    {
        fprintf(stderr, "Falha ao criar timer.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        al_destroy_sample(som_acertou);
        return false;
    }

    fila_contador = al_create_event_queue();
    if (!fila_contador)
    {
        fprintf(stderr, "Falha ao criar fila do contador.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_equacao);
        al_destroy_font(fonte_pontos);
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        al_destroy_sample(som_acertou);
        al_destroy_timer(contador);

        return false;
    }

    fantasma1= al_load_bitmap("img/fantasma.png");
    fantasma2= al_load_bitmap("img/fantasma2.png");
    fantasma3= al_load_bitmap("img/fantasma3.png");


    boneco = al_load_bitmap("img/boneco/jogador.png");

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(displayJogo.backbuffer));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    al_register_event_source(fila_contador, al_get_timer_event_source(contador));

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
    al_destroy_event_queue(fila_eventos);
    al_destroy_event_queue(fila_contador);
    al_destroy_font(fonte_equacao);
    al_destroy_font(fonte_pontos);
    al_destroy_timer(timer);
    al_destroy_timer(contador);
    al_destroy_sample(som_errou);
    al_destroy_sample(som_acertou);
    al_destroy_bitmap(fantasma1);
    al_destroy_bitmap(fantasma2);
    al_destroy_bitmap(fantasma3);
    al_destroy_bitmap(boneco);

    al_destroy_display(displayJogo.backbuffer);
    al_destroy_bitmap(telaJogo.canvas);
    al_destroy_bitmap(telaPoderes.canvas);
    al_destroy_bitmap(telaAventura.canvas);
}
