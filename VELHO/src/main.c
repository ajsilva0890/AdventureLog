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
const int LARGURA_TELA = 1280;   //960;
const int ALTURA_TELA =  720;  //540;
const float FPS = 120.0;
const float animacaoFPS = 60.0;


    GameScreen telaJogo; // tela original do jogo.
    GameDisplay displayJogo = {.mode = 0}; // display onde aparecem as telas de Aventura e Poderes
    //ALLEGRO_DISPLAY     *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos	= NULL, *fila_contador = NULL;
    ALLEGRO_BITMAP      *poderes		= NULL;
    ALLEGRO_FONT        *fonte_80	    = NULL;
    ALLEGRO_FONT        *fonte_50	    = NULL;
    ALLEGRO_TIMER       *timer			= NULL, *contador = 0;
    ALLEGRO_SAMPLE      *som_errou		= NULL;
    ALLEGRO_SAMPLE      *som_acertou	= NULL;
    ALLEGRO_BITMAP      *chapeu         = NULL;
    ALLEGRO_BITMAP      *oculos         = NULL;
    ALLEGRO_BITMAP      *gravata        = NULL;
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

int main(int argc, char **argv[]){

    printf("\n================================");
    printf("\ndebug:main():start");
    printf("\n================================");


    SpriteSheet sFantasmas, sLapidesCruzes, sGrama1, sGrama2, sArvores1, sArvores2,
		sNevoa1, sNevoa2, sNevoa3, sNevoa4, sNevoa5, sNevoa6, sFumacas;

    int spriteNum = -1;
    int i, j;
    srand(10);

    /* Inicia todos os addons utilizados no jogo. */
    if (pi_iniAllegroAddons(&displayJogo) < 0)
        return -1;

    /* Configura as telas do jogo*/
    pi_iniScreens(&telaJogo); // Inicializa a tela do jogo.
    DEBUG_ON("\ndebug:telaJogo:x2=%f", telaJogo.x2);

	// Inicializa as imagens e elementos do jogo
	pi_iniImagens(&sLapidesCruzes, &sFantasmas, &sGrama1, &sGrama2, &sArvores1, &sArvores2, &sNevoa1,
					&sNevoa2, &sNevoa3, &sNevoa4, &sNevoa5, &sNevoa6, &sFumacas, &telaJogo);

    /* Inicializa o jogo em tela cheia */
    if (pi_criaDisplay(&telaJogo, &displayJogo) < 0)
        return -1;

     /* Configura a escala das coordenadas e tamanho da imagem no display */
   //pi_setDisplayScale(&telaJogo, &displayJogo);

    int frame = 0;
    ALLEGRO_BITMAP *piso = NULL;
    piso = al_load_bitmap("img/ground/png/ground.png");
    ALLEGRO_BITMAP *ceu = NULL;
    ceu = al_load_bitmap("img/sky/png/sky.png");
    ALLEGRO_BITMAP *lua = NULL;
    lua = al_load_bitmap("img/sky/png/lua.png");

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

    // Caixas da colisao Largura e Altura
    int caixaL, caixaA;

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

    //load imagem de fundo temporaria
   // poderes = al_load_bitmap("imagens/fundo.jpg");
    //desenhando imagem temporaria de fundo
//    al_draw_bitmap(poderes, 0, 0, 0);
    // timer printipal do jogo
    al_start_timer(timer);

    /* Cria um timer separado para o redesenho dos gráficos */
	al_start_timer(timerAnimacao);
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

			if (evento.timer.source == timerAnimacao){
				/* Calcula as novas coordenadas e animações dos elementos */
				pi_AnimarSpriteSheet(&sFantasmas, &sFumacas, &telaJogo);
				pi_AnimarSpriteSheet(&sFumacas, 		NULL, &telaJogo);
//pi_AnimarSpriteSheet(&sLapidesCruzes, 	NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sGrama2, 			NULL, &telaJogo);
				pi_AnimarSpriteSheet(&sGrama1, 			NULL, &telaJogo);
//pi_AnimarSpriteSheet(&sArvores2, 		NULL, &telaJogo);
//pi_AnimarSpriteSheet(&sArvores1, 		NULL, &telaJogo);
				//pi_AnimarSpriteSheet(&sNevoa1,          NULL, &telaJogo);
				//pi_AnimarSpriteSheet(&sNevoa2,          NULL, &telaJogo);
				//pi_AnimarSpriteSheet(&sNevoa3,          NULL, &telaJogo);
				//pi_AnimarSpriteSheet(&sNevoa4, 			NULL, &telaJogo);

				if(jogador.acao == 0){
					jogadorCorrer(&jogador);
				}
				else if(jogador.acao == 1 ){
					jogadorPulo(&jogador);
				}
				else if(jogador.acao == 2 ){
					jogadorAbaixar(&jogador);
				}

			}
			redraw = 1;
            DEBUG_ON("\ndebug:main():event.type:timer ");
        }


/*///////////////////////// COMANDOS ALLEGRO_EVENT_QUEUE /**fila_eventos\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

//----- verifica se tipo de evento é clique no mouse --- E IFS PARA VERIFICAR QUAL ALTERNATIVA JOGADOR ESCOLHEU -------------

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

             verificarResposta(som_acertou, som_errou, &posicaoAlt, &questionario, &sFantasmas, &jogador);

        }  //FIM DE VERIFICAÇÃO SOBRE A ESCOLHA DA ALTERNATIVA DO JOGADOR

        //mudando tipo de ponteiro quando jogador for responder
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){

             ponteiroMouse(displayJogo.backbuffer, &posicaoAlt);
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

                if (colisao (jogador.pos_x,      jogador.pos_y,      jogador.frame_larg, jogador.frame_alt,
                             sFantasmas.posX[i], sFantasmas.posY[i], sFantasmas.largura, sFantasmas.altura,
                             caixaL, caixaA) == 1){

                    //sFantasmas.posX[i] = 1920;
                    sFantasmas.eliminado[i] = SIM;
                }
            }



/*---------------VERIFICAÇÃO ATUALIZAÇÃO DA PERGUNTA-------------------------------------------------------*/
            atualizarPergunta(&questionario);


/* ----------------- TEMPO RECRESIVO NA TELA -------------------------------------------------- */
            cronometroRegressivo(contador, fila_contador, &questionario, &sair);



   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
/* /////////////////////// AREA DE DESENHAR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

        if (redraw && al_is_event_queue_empty(fila_eventos) && evento.timer.source == timerAnimacao) {
            redraw = 0;

			/** Desenhar na ordem de profundidade no cenário. Começando pelo mais distante para o mais perto **/
            al_draw_bitmap(ceu, 0, 0, 0);
			al_draw_scaled_bitmap(lua, 0, 0, 256, 230, (telaJogo.largura - 500), (telaJogo.altura - 420), (256*2), (230*2), 0);
			//al_draw_bitmap(lua, (telaJogo.largura - 500), (telaJogo.altura - 420), 0);
			al_draw_bitmap(piso, 0, telaJogo.altura- 10, 0);

			/* Desenha os elementos do cenario que ficarão ao fundo */
			//pi_drawGraphics(&sNevoa1);
//pi_drawGraphics(&sArvores2);
			//pi_drawGraphics(&sNevoa2);
//pi_drawGraphics(&sArvores1);
			//pi_drawGraphics(&sNevoa3);
//pi_drawGraphics(&sGrama2);
//pi_drawGraphics(&sLapidesCruzes);
			//pi_drawGraphics(&sNevoa4);

			drawResposdeu(fonte_80, &questionario, &posicaoAlt);
			drawPergunta(fonte_50, fonte_80, &questionario, &posicaoAlt, chapeu, oculos, gravata);
			drawPontos (fonte_50, &questionario, &posicaoAlt, &jogador);
			drawJogador(&jogador, boneco);

			/* Desenha os elementos que ficarão na frente do personagem */
			pi_drawGraphics(&sFantasmas);
			pi_drawGraphics(&sFumacas);
			pi_drawGraphics(&sGrama1);

			al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
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

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        return -1;
    }

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_install_audio()){
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_init_acodec_addon()){
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_errou = al_load_sample("sons/sample_erro.ogg");
    if (!som_errou){
        fprintf(stderr, "Falha ao carregar som_errou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    som_acertou = al_load_sample("sons/sample_acertou.ogg");
    if (!som_acertou){
        fprintf(stderr, "Falha ao carregar som_acertou.\n");
        al_destroy_display(displayJogo.backbuffer);
        al_destroy_timer(timer);
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
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
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
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
        al_destroy_font(fonte_80);
        al_destroy_font(fonte_50);
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(som_errou);
        al_destroy_sample(som_acertou);
        al_destroy_timer(contador);

        return false;
    }

    oculos  = al_load_bitmap("img/itens/oculos.png");
    chapeu  = al_load_bitmap("img/itens/chapeu.png");
    gravata = al_load_bitmap("img/itens/gravata.png");


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
    al_destroy_font(fonte_80);
    al_destroy_font(fonte_50);
    al_destroy_timer(timer);
    al_destroy_timer(contador);
    al_destroy_sample(som_errou);
    al_destroy_sample(som_acertou);
    al_destroy_bitmap(chapeu);
    al_destroy_bitmap(oculos);
    al_destroy_bitmap(gravata);
    al_destroy_bitmap(boneco);

    al_destroy_display(displayJogo.backbuffer);
    al_destroy_bitmap(telaJogo.canvas);
   // al_destroy_bitmap(telaPoderes.canvas);
   // al_destroy_bitmap(telaAventura.canvas);
}
