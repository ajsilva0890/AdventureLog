#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "structs.h" 
#include "equacoes.h"
#include <stdio.h>
#include "pi_tools.h"
#include "pi_data.h"

void acertou(listaQuestao *, bonecoJog *);
void errou(listaQuestao *);



void posicaoAlternativas(int largura, int altura, telaAlternativa *alternativas, bonecoJog *jogador){
    
    //definicao do inicio da Tela de Poderes -- 1/3 da tela total a esquerda --
    //int meio2 = (largura/3)*2;    
    int meioLargura = largura / 2;
    int meioAltura = altura / 2;
    int meio13Altura = altura / 3;

    alternativas->alt_Ax      = meioLargura + (meioLargura * 0.2);       //meio2+50;                        
    alternativas->alt_Bx      = (meioLargura / 2) * 3;                   //(largura-meio2)/2 + meio2; 
    alternativas->alt_Cx      = largura - (meioLargura * 0.2);           //largura-50;                      
    alternativas->pergunta_x  = (meioLargura / 2) * 3;                   //(largura-meio2)/2 + meio2;
    
    alternativas->pontos_x    = meioLargura - (meioLargura * 0.2);       //(largura-meio2)/2 + meio2;
    alternativas->tempo_x     = meioLargura * 0.2;                       //meio2+10;
    alternativas->acertos_x   = meioLargura * 0.2;                       //largura-50;
    alternativas->resultado_x = meioLargura - (meioLargura * 0.2);   
    jogador->forca_x          = meioLargura * 0.2;



    alternativas->alt_Ay      = meio13Altura - (meio13Altura * 0.2);
    alternativas->alt_By      = meio13Altura - (meio13Altura * 0.2);
    alternativas->alt_Cy      = meio13Altura - (meio13Altura * 0.2);
    alternativas->pergunta_y  = meio13Altura * 0.2;
    
    alternativas->pontos_y    = meio13Altura / 2;
    alternativas->tempo_y     = meio13Altura / 2;
    alternativas->acertos_y   = meio13Altura * 0.2;
    alternativas->resultado_y = meio13Altura * 0.2;
    jogador->forca_y          = meio13Altura - (meio13Altura * 0.2);

    //cor da alternativa inicial
    alternativas->altCorA = 245;
    alternativas->altCorB = 245;
    alternativas->altCorC = 245;
}



void setVeriavel(listaQuestao *questionario){
    // flag para verificar se jogador acertou a pergunta
    questionario->acertou   = -1;

    // flag para guardar quantidade de pontos e consequentemente nivel de dificuldade multiplicações
    questionario->pontos    = 0;  

    // tempo inicial em segundo do jogo
    questionario->tempo     = 20;

    //quantidade de acertos para elimitar fantasma
    questionario->QntdRespostaMatarFantasma = 2;

    //totl de pontos inicial para jogador eliminar fantasma --- 0/5 
    questionario->matarFantasma     = 0;
}



void setBoneco(int largura, int altura, bonecoJog *jogador, ALLEGRO_BITMAP *boneco){

    jogador->pos_x          = 100;
    jogador->pos_y          = altura - (altura * 0.18);

    jogador->pos_fixa_y     = altura - (altura * 0.18);
    
    jogador->alturaPulo     = altura - (altura * 0.15)-((al_get_bitmap_height(boneco)/2));
    jogador->alturaAbaixar  = altura - (altura * 0.11);


    jogador->cur_frame      = 0;
    jogador->frame_count    = 0;
    jogador->acao           = 0;
    jogador->forca          = 99;


    jogador->frame_alt      = al_get_bitmap_height(boneco)/3;
    jogador->frame_larg     = al_get_bitmap_width(boneco)/8;
}

void acertou(listaQuestao *questionario, bonecoJog *jogador){

    questionario->acertou    = 1; //se jogador acetou pergunta flag indicando que jogador acertou a questao é ativa
    questionario->pontos    += 3; // e jogador soma tres pontos por acerto 
    questionario->tempo     += +5;

    jogador->forca          += 0.37;

}

void errou(listaQuestao *questionario){

    questionario->acertou   = 0; // flag indica 0 para respota errada
    questionario->pontos   += -3; //jogador perde 3 pontos

}




void verificarResposta(ALLEGRO_SAMPLE *som_acertou, ALLEGRO_SAMPLE *som_errou, telaAlternativa *posicaoAlt, listaQuestao *questionario, SpriteSheet *sFantasmas, bonecoJog *jogador){
    //verificar clique 
    //printf("X: %d - Y: %d\n", posicaoAlt->mousePoderes_x, posicaoAlt->mousePoderes_y);
    //printf("alt A x: %d - y: %d", posicaoAlt->alt_Ax, posicaoAlt->alt_Ay);

    if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Ax - 50 /* 765 */ && 
        posicaoAlt->mousePoderes_x < posicaoAlt->alt_Ax + 50 /* 878 */ &&
        posicaoAlt->mousePoderes_y > posicaoAlt->alt_Ay - 20 /* 172 */ &&
        posicaoAlt->mousePoderes_y < posicaoAlt->alt_Ay + 80 /* 266 */ ){
    
           
            if(questionario->matarFantasma>=questionario->QntdRespostaMatarFantasma){
                questionario->matarFantasma = -1;
                sFantasmas->eliminado[0] = SIM;

            }
            else{
                //flag para que no próximo loop seja atualizado a questao na tela
                questionario->atualizarQuestao=1;
                // if para verificar se alternativa A é igual a resposta correta
                if (questionario->alternativa_A == questionario->resposta){
                    acertou(questionario, jogador);
                    al_play_sample(som_acertou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                }
                else{
                    errou(questionario);
                    al_play_sample(som_errou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                }   
            } 

    }     /* ISSO É FEITO EXATAMENTE IGUAL PARA AS ALTERNATIVAS B E C, OU SEJA, É FEITA A VERIFICAÇÃO EM TODOS OS CASOS QUE O JOGADOR
        CLICA NA ALTERNATIVA A, B OU C */


        else if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Bx -  90  && 
                posicaoAlt->mousePoderes_x  < posicaoAlt->alt_Bx +  70   &&
                posicaoAlt->mousePoderes_y  > posicaoAlt->alt_By -  0    &&
                posicaoAlt->mousePoderes_y  < posicaoAlt->alt_By +  100){

                    if(questionario->matarFantasma >= questionario->QntdRespostaMatarFantasma){
                        questionario->matarFantasma = -1;
                        sFantasmas->eliminado[1] = SIM;
                    }

                    else{
                        questionario->atualizarQuestao=1;

                        if (questionario->alternativa_B == questionario->resposta){
                            acertou(questionario, jogador);
                            al_play_sample(som_acertou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }

                        else{
                            errou(questionario);
                            al_play_sample(som_errou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }    
                    }
        }

            else if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Cx -  90  && 
                     posicaoAlt->mousePoderes_x < posicaoAlt->alt_Cx +  70  &&
                     posicaoAlt->mousePoderes_y > posicaoAlt->alt_Cy -  0   &&
                     posicaoAlt->mousePoderes_y < posicaoAlt->alt_Cy +  100){

                        if(questionario->matarFantasma >= questionario->QntdRespostaMatarFantasma){
                            questionario->matarFantasma = -1;
                            sFantasmas->eliminado[2] = SIM;

                        }
                        else{
                                questionario->atualizarQuestao = 1;

                                if (questionario->alternativa_C == questionario->resposta){
                                    acertou(questionario, jogador);
                                    al_play_sample(som_acertou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
 
                                }
                                else{
                                    errou(questionario);
                                    al_play_sample(som_errou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }    
                        }                
            }

    //-------------------------------------------------------------------------------------------------------------

    //atualizando variavel contaora de quesdoes acertadas para eliminar fantasma
    if( questionario->matarFantasma <= 5 && questionario->acertou == 1){
        questionario->matarFantasma++;
    }

}



void ponteiroMouse (ALLEGRO_DISPLAY *janela, telaAlternativa *posicaoAlt){

    if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Ax -  90  /* 765 */ && 
        posicaoAlt->mousePoderes_x < posicaoAlt->alt_Ax +  70  /* 878 */ &&
        posicaoAlt->mousePoderes_y > posicaoAlt->alt_Ay -  0  /* 172 */  &&
        posicaoAlt->mousePoderes_y < posicaoAlt->alt_Ay +  100  /* 266 */){

            al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
            posicaoAlt->altCorA = 100;

    }

    else if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Bx -  90  && 
             posicaoAlt->mousePoderes_x < posicaoAlt->alt_Bx +  70  &&
             posicaoAlt->mousePoderes_y > posicaoAlt->alt_By -  0   &&
             posicaoAlt->mousePoderes_y < posicaoAlt->alt_By +  100 ){

           al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
           posicaoAlt->altCorB = 100;

    }

    else if (posicaoAlt->mousePoderes_x > posicaoAlt->alt_Cx -  90  && 
             posicaoAlt->mousePoderes_x < posicaoAlt->alt_Cx +  70  &&
             posicaoAlt->mousePoderes_y > posicaoAlt->alt_Cy -  0   &&
             posicaoAlt->mousePoderes_y < posicaoAlt->alt_Cy +  100 ){
         
           al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
           posicaoAlt->altCorC = 100;

    }

    else{
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
        posicaoAlt->altCorA = 245;
        posicaoAlt->altCorB = 245;
        posicaoAlt->altCorC = 245;
    }


}




void atualizarPergunta(listaQuestao *questionario){

    if(questionario->atualizarQuestao == 1){
                
        //  Os próximos IFs verifica quantidade de pontos e atualiza o nível para a  próxima questão

        if(questionario->pontos <= 12)
            ale_parte1(questionario);

        else if(questionario->pontos <= 21)
            ale_parte2(questionario);

        else if(questionario->pontos <= 36)
            ale_parte3(questionario);

        else    
            ale_parte4(questionario);

        // Flag que verifica se jogador já respondeu pergunta é zerada
        questionario->atualizarQuestao = 0;
    }
    
            
    /* Temos duas dormas de representar as questões na telae elas se diferenciam nos níveis 1 e 3 para o nivel 2 e 4
        por isso temos esse IF para verificar qual nível jogador se encontra para realizar o al_draw_textf com as variaveis corretas
        FORMATO PARA NIVEL 1 E 3:   X * Y 
                           2 E 4:   x * Y + Z     */
    if (questionario->pontos <= 12 || questionario->pontos > 21 && questionario->pontos <= 36 )
        questionario->nivel = 13;
    else
        questionario->nivel = 24;

}



void cronometroRegressivo(ALLEGRO_TIMER *contador, ALLEGRO_EVENT_QUEUE *fila_contador, listaQuestao *questionario, int *sair){

    ALLEGRO_EVENT evento;

    if (!al_is_event_queue_empty(fila_contador)){
            
            //ALLEGRO_EVENT evento;
            al_wait_for_event(fila_contador, &evento);
            //Atualizando contador, subtraindo 1 segundo
            if (evento.type == ALLEGRO_EVENT_TIMER){
                questionario->tempo--;
            }
        }

        //saindo do jogo caso tempo zere
        if(questionario->tempo == 0){
            (*sair) = 1;
        }
}



/*///////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        
 /////////// AREA DE DESENHAR TELA DE PODERES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */

void drawResposdeu(ALLEGRO_FONT *fonte_equacao, listaQuestao *questionario, telaAlternativa *posicaoAlt){

    switch(questionario->acertou){
        case 1:   
            al_draw_text(fonte_equacao, al_map_rgb(222, 0, 0), posicaoAlt->resultado_x, posicaoAlt->resultado_y, ALLEGRO_ALIGN_CENTRE, "Acertou");
            break;

        case 0:
            al_draw_text(fonte_equacao, al_map_rgb(222, 0, 0), posicaoAlt->resultado_x, posicaoAlt->resultado_y, ALLEGRO_ALIGN_CENTRE, "Errou");
            break;
        
        case -1:
            al_draw_text(fonte_equacao, al_map_rgb(222, 0, 0), posicaoAlt->resultado_x, posicaoAlt->resultado_y, ALLEGRO_ALIGN_CENTRE, "COMEÇOU!!");
            break;
    }

}



void drawPergunta(ALLEGRO_FONT *fonte_eliminar, ALLEGRO_FONT *fonte_equacao, listaQuestao *questionario, telaAlternativa *posicaoAlt, ALLEGRO_BITMAP *chapeu, ALLEGRO_BITMAP *oculos, ALLEGRO_BITMAP *gravata){
    /* usar para verificar tamanho do botão
    al_draw_filled_rectangle(posicaoAlt->alt_Ax - 90, posicaoAlt->alt_Ay, posicaoAlt->alt_Ax + 70, posicaoAlt->alt_Ay + 100 , al_map_rgb(0, 0, 255));
    al_draw_filled_rectangle(posicaoAlt->alt_Bx - 90, posicaoAlt->alt_By, posicaoAlt->alt_Bx + 70, posicaoAlt->alt_By + 100, al_map_rgb(0, 255, 255));
    al_draw_filled_rectangle(posicaoAlt->alt_Cx - 90, posicaoAlt->alt_Cy, posicaoAlt->alt_Cx + 70, posicaoAlt->alt_Cy + 100, al_map_rgb(255, 0, 255));
    */

    if(questionario->matarFantasma<questionario->QntdRespostaMatarFantasma){
        //De acordo com o comentário da linha 'VERIFICAÇÃO SE JOGADOR JA RESPONDEU PERGUNTA', usamos o switch case abaixo
        switch (questionario->nivel){
            case 13:
                 al_draw_textf(fonte_equacao, al_map_rgb(222, 0, 0), posicaoAlt->pergunta_x, posicaoAlt->pergunta_y, ALLEGRO_ALIGN_CENTRE , "%d * %d =", questionario->produto, questionario->numero);
                 break;
            case 24:  
                 al_draw_textf(fonte_equacao, al_map_rgb(222, 0, 0), posicaoAlt->pergunta_x, posicaoAlt->pergunta_y, ALLEGRO_ALIGN_CENTRE , "%d * %d + %d =", questionario->produto, questionario->numero, questionario->soma);
                 break;
        }


        // Print na tela da pergunta e alternativas
        al_draw_textf(fonte_equacao, al_map_rgb(posicaoAlt->altCorA, posicaoAlt->altCorA, posicaoAlt->altCorA), posicaoAlt->alt_Ax,     posicaoAlt->alt_Ay, ALLEGRO_ALIGN_CENTRE    , "a) %d ", questionario->alternativa_A);
        al_draw_textf(fonte_equacao, al_map_rgb(posicaoAlt->altCorB, posicaoAlt->altCorB, posicaoAlt->altCorB), posicaoAlt->alt_Bx,     posicaoAlt->alt_By, ALLEGRO_ALIGN_CENTRE    , "b) %d ", questionario->alternativa_B);
        al_draw_textf(fonte_equacao, al_map_rgb(posicaoAlt->altCorC, posicaoAlt->altCorC, posicaoAlt->altCorC), posicaoAlt->alt_Cx,     posicaoAlt->alt_Cy, ALLEGRO_ALIGN_CENTRE    , "c) %d ", questionario->alternativa_C);
        
    }

    // condicao para habilitar escolha de matar fantasma pela cor
    //if(questionario.matarFantasma>=questionario.QntdRespostaMatarFantasma){

    else {    //possicao exata para clique dos fantasmas -- desenho na tela --
        al_draw_text(fonte_eliminar, al_map_rgb(0, 0, 255), posicaoAlt->pergunta_x , posicaoAlt->pergunta_y,  ALLEGRO_ALIGN_CENTRE, "Eliminar fantasma com:");
        // Retângulo: x1, y1, x2, y2, cor, espessura
        //al_draw_filled_rectangle(posicaoAlt->alt_Ax - 35, posicaoAlt->alt_Ay, posicaoAlt->alt_Ax + 30, posicaoAlt->alt_Ay + 50 , al_map_rgb(0, 0, 255));
        //al_draw_filled_rectangle(posicaoAlt->alt_Bx - 35, posicaoAlt->alt_By, posicaoAlt->alt_Bx + 30, posicaoAlt->alt_By + 50 , al_map_rgb(0, 255, 255));
        //al_draw_filled_rectangle(posicaoAlt->alt_Cx - 35, posicaoAlt->alt_Cy, posicaoAlt->alt_Cx + 30, posicaoAlt->alt_Cy + 50 , al_map_rgb(255, 0, 255));
        
        al_draw_bitmap(chapeu,  posicaoAlt->alt_Ax - al_get_bitmap_width(chapeu) + 50 , posicaoAlt->alt_Ay /*- al_get_bitmap_width (chapeu)*/, 0);
        al_draw_bitmap(gravata, posicaoAlt->alt_Bx - al_get_bitmap_width(gravata) + 50, posicaoAlt->alt_By + 25 /*- al_get_bitmap_width (gravata)*/, 0);
        al_draw_bitmap(oculos,  posicaoAlt->alt_Cx - al_get_bitmap_width(oculos) + 50,  posicaoAlt->alt_Cy + 35 /*- al_get_bitmap_width (oculos)*/, 0);

    }
}



    void drawPontos (ALLEGRO_FONT *fonte_pontos, listaQuestao *questionario, telaAlternativa *posicaoAlt, bonecoJog *jogador){
        // Print na tela da quantidade de pontos do jogador
        al_draw_textf(fonte_pontos, al_map_rgb(255, 0, 0), posicaoAlt->pontos_x,   posicaoAlt->pontos_y, ALLEGRO_ALIGN_CENTRE  , "Pontos: %d ", questionario->pontos);

        al_draw_textf(fonte_pontos, al_map_rgb(255, 0, 0), posicaoAlt->tempo_x,   posicaoAlt->tempo_y, ALLEGRO_ALIGN_CENTRE  , "Tempo: %d s ", questionario->tempo);

        al_draw_textf(fonte_pontos, al_map_rgb(255, 0, 0), posicaoAlt->acertos_x,   posicaoAlt->acertos_y, ALLEGRO_ALIGN_CENTRE  , "%d / 5 ", questionario->matarFantasma);

        al_draw_textf(fonte_pontos, al_map_rgb(255, 0, 0), jogador->forca_x,   jogador->forca_y, ALLEGRO_ALIGN_CENTRE  , "Força: %.1f", jogador->forca);
    //----------------------------------------------------------------

}