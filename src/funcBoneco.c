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



 void jogadorCorrer(bonecoJog *jogador){

 	jogador->max_frame = 8;
 	jogador->frame_delay = 4;

    if(++jogador->frame_count >= jogador->frame_delay){
    	if(++jogador->cur_frame >= jogador->max_frame){
       		 jogador->cur_frame = 0;
        }

		jogador->frame_count = 0;
	}
 }



 void jogadorPulo(bonecoJog *jogador){
 	
	jogador->max_frame = 3;
	jogador->frame_delay = 15;

	if(jogador->frame_count==1){
	   jogador->pos_y = jogador->alturaPulo;
	}

    if(++jogador->frame_count >= jogador->frame_delay){

        if(++jogador->cur_frame >= jogador->max_frame){
            jogador->acao = 0;
            jogador->pos_y = jogador->pos_fixa_y;
            jogador->cur_frame = 0;
        }

        jogador->frame_count = 0;
    }
}

    

 void jogadorAbaixar(bonecoJog *jogador){

	jogador->max_frame = 8;
	jogador->frame_delay = 10;
    
	if(jogador->frame_count == 0){
		jogador->pos_y = jogador->alturaAbaixar;
	}

    if(++jogador->frame_count >= jogador->frame_delay){

        if(++jogador->cur_frame >= jogador->max_frame){
            jogador->acao = 0;
            jogador->pos_y = jogador->pos_fixa_y;
            jogador->cur_frame = 0;
        }

        jogador->frame_count = 0;
    }
}



void clique_KEY_W(bonecoJog *jogador){
     jogador->acao = 1;
     jogador->cur_frame = 0;
     jogador->frame_count = 0;
     jogador->forca += -1;
}



void clique_KEY_S(bonecoJog *jogador){
    jogador->acao = 2;
    jogador->cur_frame = 0;
    jogador->frame_count = 0;
    jogador->forca += -1;
}



int colisao(float jogadorPos_x, float jogadorPos_y, float jogadorFrame_larg, float jogadorFrame_alt, 
            float inimigoPos_x, float inimigoPos_y, float inimigoFrame_larg, float inimigoFrame_alt,
            int caixaL, int caixaA){
    
    int jogMeio_x, jogMeio_y;
    int iniMeio_x, iniMeio_y;

    iniMeio_x = inimigoPos_x + (inimigoFrame_larg/2);
    iniMeio_y = inimigoPos_y + (inimigoFrame_alt/2);

    jogMeio_x = jogadorPos_x + (jogadorFrame_larg/2);
    jogMeio_y = jogadorPos_y + (jogadorFrame_alt/2);

    int caixaColJog_x, caixaColJog_y;
    caixaColJog_x = (jogadorFrame_larg/2) +   (caixaL); //(0) valor para aumentar ou diminuir a caixa de colisao
    caixaColJog_y = (jogadorFrame_alt/2)  +   (caixaA); //(0) valor para aumentar ou diminuir a caixa de colisao


    int caixaColIni_x, caixaColIni_y;
    caixaColIni_x = (inimigoFrame_larg/2) +   (0); //(0) valor para aumentar ou diminuir a caixa de colisao
    caixaColIni_y = (inimigoFrame_alt/2)  +   (0); //(0) valor para aumentar ou diminuir a caixa de colisao

    //al_draw_filled_rectangle(jogMeio_x + caixaColJog_x, jogMeio_y + caixaColJog_y, jogMeio_x - caixaColJog_x, jogMeio_y - caixaColJog_y , al_map_rgb(255, 0, 0));

    if ( jogMeio_x + caixaColJog_x > iniMeio_x - caixaColIni_x &&
         jogMeio_x - caixaColJog_x < iniMeio_x + caixaColIni_x &&
         jogMeio_y + caixaColJog_y > iniMeio_y - caixaColIni_y &&
         jogMeio_y - caixaColJog_y < iniMeio_y + caixaColIni_y ){
            
            return 1;

    }
    else   
        return 0;

}





/* -------------- DRAW JOGADOR -------------------- */

void drawJogador(bonecoJog *jogador, ALLEGRO_BITMAP *boneco){

int fx = jogador->cur_frame  * jogador->frame_larg;
int fy = jogador->acao * jogador->frame_alt;

   al_draw_bitmap_region (boneco, fx, fy, jogador->frame_larg, jogador->frame_alt,
                          jogador->pos_x, jogador->pos_y, 0);  
                
}