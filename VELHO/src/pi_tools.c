/*
 * Código baseado no tutorial disponível em:
 * 		  https://wiki.allegro.cc/index.php?title=Achieving_Resolution_Independence
 */
#include "allegro5/allegro5.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "pi_data.h"

/* Para ativar/desativar o debug, descomentar/comentar a linha abaixo */
//#define _SET_DEBUG_ON
//#define _SET_DEBUG_FILE
//#define _SET_DEBUG_STRING

#ifdef 	_SET_DEBUG_ON
	#define DEBUG_ON printf
#endif
#ifndef _SET_DEBUG_ON
	#define DEBUG_ON //
#endif

#ifdef 	_SET_DEBUG_FILE
	#define DEBUG_FILE printf
#endif
#ifndef _SET_DEBUG_FILE
	#define DEBUG_FILE //
#endif

#ifdef 	_SET_DEBUG_STRING
	#define DEBUG_STRING printf
#endif
#ifndef _SET_DEBUG_STRING
	#define DEBUG_STRING //
#endif

//======================================================================
void pi_drawGraphics(SpriteSheet *s){
	int counter = 0, ci = 0, cj = 0, i, j;

	al_hold_bitmap_drawing(true);
	if (s->bitmap != NULL){

		/** Desenho específico para fumaça **/
		if (s->id == ID_FUMACA){
			for (i = 0; i < s->sheetSizeX; i++){
				if (s->habilitado[i] == SIM){
				//	printf("\nID:%d",i);

					if (s->contIntervalo[i] <= 0){
						s->contIntervalo[i] = s->intervalo[i];
						s->contRegressiva[i]--;

						s->colunaAtual[i]++;

						if(s->colunaAtual[i] >= s->sheetSizeX){
							s->colunaAtual[i] = 0;
							s->linhaAtual[i]++;
						}
					}

					if (s->contRegressiva[i] < 0){
						s->colunaAtual		[i]	= 0;
						s->linhaAtual		[i]	= 0;
						s->habilitado		[i]	= NAO; // desabilita se a contagem regressiva chegou a zero.
						s->contRegressiva	[i] = 0;
						s->contIntervalo 	[i] = s->intervalo[i];
						s->loop				[i] = NAO;
						s->contRegressiva	[i] = 8;
						return;
					}

					ci = s->colunaAtual[i];
					cj = s->linhaAtual [i];
					s->contIntervalo[i]--;
					//printf("\n ci:%d   cj:%d   contReg:%d   sheetX:%d   ID:%d", s->colunaAtual[i], s->linhaAtual[i], s->contRegressiva[i], s->sheetSizeX, i);

					al_draw_bitmap_region(s->bitmap, s->x1 + (s->largura * ci),
						s->y1 + (s->altura * cj), s->largura, s->altura,
						s->posX[i], s->posY[i], 0);
				}
			}
			return;
		}

		/** Desenho para cenário e fantasmas **/
		for (i = 0; i < s->sheetSizeX + s->repetirElementosX; i++){
			for (j = 0; j < s->sheetSizeY + s->repetirElementosY; j++){

				// se colunaAtual ou linhaAtual for maior do que as dimensões X ou Y do spritesheet, então
				// significa que ultrapassou o tamanho real,
				// portanto iniciou o looping a partir do primeiro elemento
				// novamente.
				if (ci >= s->sheetSizeX)
					ci = 0;
				if (cj >= s->sheetSizeY)
					cj = 0;

				if (s->habilitado[counter] == SIM){
					if (s->novaAltura != -1 && s->novaLargura != -1){
						// desenha imagem com escala
						al_draw_scaled_bitmap(s->bitmap, s->x1 + (s->novaLargura * ci),
								s->y1 + (s->novaAltura * cj), s->largura, s->altura,
								s->posX[counter], s->posY[counter], s->novaLargura, s->novaAltura, 0);
					}
					else{
						 // desenha imagem sem escala
						al_draw_bitmap_region(s->bitmap, s->x1 + (s->largura * ci),
								s->y1 + (s->altura * cj), s->largura, s->altura,
								s->posX[counter], s->posY[counter], 0);
					}
				}

				ci++;
				cj++;
				counter++;
			}
		}
	}
	al_hold_bitmap_drawing(false);
}
//----------------------------------------------------------------------
int pi_AnimarSpriteSheet(SpriteSheet *st, SpriteSheet *fumaca, GameDisplay *display){
	int i, j, k, counter = 0, recomecou = 0;
	float deltaX, deltaY, correcaoY = 0.0;
	float ret, r;
	float val = PI / 180;

	/** cálculo especifico para a fumacinha dos fantasmas **/
	/** atualiza cada sprite da animação da fumaça para as coordenadas atuais **/
	if (st->id == ID_FUMACA){
		for (i = 0; i < st->sheetSizeX * st->sheetSizeY; i++){
			st->posX[i] += (st->velX[i] * st->dirX[i] * st->profundidade[i]);
			st->posY[i] += (st->velY[i] * st->dirY[i]);
		}
		return 0;
	}

	/** cálculo para os fantasmas e elementos do cenário **/
	for (i = 0; i < st->sheetSizeX + st->repetirElementosX; i++){
		for (j = 0; j < st->sheetSizeY + st->repetirElementosY; j++){

			srand(time(NULL) * 1000000);

			float x1 		= st->x1;
			float y1 		= st->y1;
			float posX 		= st->posX		[counter];
			float posY 		= st->posY		[counter];
			float inicioX 	= st->inicioX	[counter];
			float inicioY 	= st->inicioY	[counter];
			float offsetX 	= st->offsetX	[counter];
			float offsetY 	= st->offsetY	[counter];
			float fimX 		= st->fimX		[counter];
			float fimY		= st->fimY		[counter];
			float velX		= st->velX		[counter];
			float velY		= st->velY		[counter];
			float dirX		= st->dirX		[counter];
			float dirY 		= st->dirY		[counter];
			int id			= st->spriteId	[counter];
			float profundidade	= st->profundidade[counter];
			float largura		= st->largura;
			float altura		= st->altura;

			ret = 0;

			r = (rand() / 100000000.0);

			posX += (velX * dirX * profundidade);

			// Calculo especifico para fantasma
			if (st->id == ID_GHOST){
				if (posY < (display->altura - (st->altura / 1.2)))
					posY = (display->altura - (st->altura / 1.2));

				if (st->eliminado[counter] == SIM){
					st->eliminado[counter] = NAO;

					if (fumaca->habilitado[counter] == NAO){
						fumaca->habilitado  [counter] 	= SIM;
						fumaca->colunaAtual	[counter]	= 0;
						fumaca->linhaAtual	[counter]	= 0;
						fumaca->dirX		[counter]	= dirX;
						fumaca->posX		[counter]	= posX;
						fumaca->posY		[counter]	= posY;
						fumaca->loop		[counter]	= SIM;
						fumaca->contRegressiva[counter] = 8;
					}

					posX = 1920;
				}

				if ((int)ret % 2 == 1)
					ret = (r/12) * sin(posX * val);
				else
					ret = (r/12) * cos(posX * val);
			}

			// não continua processando caso o objeto não esteja habilitado
			if(st->habilitado[counter] == NAO){
				return 0;
			}


			posY += (velY * dirY) + (ret * 1.5);

			deltaX = fimX - inicioX;
			deltaY = fimY - inicioY;

			// verifica se a imagem saiu da tela e reposiciona de novo
			if (st->loop[counter] == SIM){
				if (deltaX < 0){
					if ((posX + largura) <= fimX){
						posX = inicioX + offsetX;
						recomecou = 1;
					}
				}
				else{
					if (posX > fimX){
						posX = inicioX + offsetX;
						recomecou = 1;
					}
				}
			}
/*

			if (deltaY < 0){
				if (posY + altura < fimY)
				posY = inicioY + offsetY;
				//recomecou = 1;
			}
			else{
				if (posY > fimY)
				posY = inicioY + offsetY;
				//recomecou = 1;
			}
*/
			if (recomecou == 1){
				if(st->id == ID_FUMACA){
				//	st->habilitado[0] = NAO;
				}
				if (st->id == ID_GHOST){
					// Estabelece velocidades mínimas e máximas
					r = (rand() / 10000000.0);
					int extraY = r;
					extraY = (extraY % 2 == 1 ? 20:-10);
					//posY = (inicioY + offsetY) + (extraY * r);

					if (r * (0.25) < 0.7)
						velX = 0.7;
					else if (r * (0.25) > 1.0)
						velX = 1.0;

					st->velX[counter] = velX;

					/** ao reiniciar não deixa os fantasmas ultrapassarem os limites abaixo no eixo Y especificados abaixo**/
					dirY = 1.0;
					if (posY > (display->altura - (st->altura / 1.2))){
						posY = display->altura - (500 * (r/1000));
						dirY = -1.0;
					}
					if (posY < 550){
						posY = 500 + (500 * (r/1000));
						dirY = 1.0;
					}

				}
				recomecou = 0;
			}

			st->posX[counter] = posX;
			st->posY[counter] = posY;
			counter++;
		}
	}
		//		printf("\nsheetSizeX = %d", st->sheetSizeX);

	return 0;
}
//----------------------------------------------------------------------
void pi_iniImagens(SpriteSheet *sLapidesCruzes, SpriteSheet *sFantasmas, SpriteSheet *sGrama1,
					SpriteSheet *sGrama2, SpriteSheet *sArvores1, SpriteSheet *sArvores2,
					SpriteSheet *sNevoa1, SpriteSheet *sNevoa2, SpriteSheet *sNevoa3, SpriteSheet *sNevoa4,
					SpriteSheet *sNevoa5, SpriteSheet *sNevoa6, SpriteSheet *sFumacas,
					GameScreen *telaJogo){

	int i, j, counter;
	srand(time(NULL));
    float r;

    /** configura spritesheet lápides e cruzes**/
	sLapidesCruzes->bitmap = al_load_bitmap("img/tomb/png/lapides_cruzes.png");
    sLapidesCruzes->x1         = 0.0;
    sLapidesCruzes->y1         = 0.0;
    sLapidesCruzes->largura    = 72.0;
    sLapidesCruzes->altura     = 77.0;
    sLapidesCruzes->sheetSizeX = 4;
    sLapidesCruzes->sheetSizeY = 1;
    sLapidesCruzes->id         = ID_LAPIDE_CRUZ;
    sLapidesCruzes->repetirElementosX = 5;
    sLapidesCruzes->repetirElementosY = 0;
	sLapidesCruzes->novaAltura  = -1;
	counter = 0;

    for (i = 0; i < sLapidesCruzes->sheetSizeX + sLapidesCruzes->repetirElementosX; i++){
        for (j = 0; j < sLapidesCruzes->sheetSizeY + sLapidesCruzes->repetirElementosY; j++){
			srand(i+j+counter*4);
			r = (rand() / 100000000.0);
			sLapidesCruzes->colunaAtual		[counter] = 0;
			sLapidesCruzes->linhaAtual 		[counter] = 0;
            sLapidesCruzes->posX			[counter] = (counter) * sLapidesCruzes->largura + (200 * r);
            sLapidesCruzes->posY			[counter] = telaJogo->altura - sLapidesCruzes->altura;
            sLapidesCruzes->offsetX			[counter] = sLapidesCruzes->posX[counter];
            sLapidesCruzes->offsetY			[counter] = 0;
            sLapidesCruzes->inicioX			[counter] = -1;
            sLapidesCruzes->dirX			[counter] = -1;
            sLapidesCruzes->dirY			[counter] = 1;
            sLapidesCruzes->velX			[counter] = 1.0;
            sLapidesCruzes->velY			[counter] = 0.0;
            sLapidesCruzes->inicioY			[counter] = sLapidesCruzes->posY[counter];
            sLapidesCruzes->inicioX			[counter] = telaJogo->largura;
            sLapidesCruzes->fimY			[counter] = sLapidesCruzes->posY[counter];
            sLapidesCruzes->fimX			[counter] = 0.0;
            sLapidesCruzes->spriteId		[counter] = i+j;
            sLapidesCruzes->loop			[counter] = SIM;
            sLapidesCruzes->profundidade	[i+j]     = 1.3;
            sLapidesCruzes->intervalo	 	[counter] = 0;
            sLapidesCruzes->contIntervalo	[counter] = 0;
            sLapidesCruzes->habilitado		[counter] = SIM;
//            printf("\nY:%0.1f", sLapidesCruzes->posY[counter]);
            counter++;
        }
    }

    /** configura spritesheet fumaça dos fantasmas eliminados**/
    sFumacas->bitmap = al_load_bitmap("img/fumaca/png/fumaca.png");
    sFumacas->x1         = 0.0;
    sFumacas->y1         = 0.0;
    sFumacas->largura    = 64.0;
    sFumacas->altura     = 80.0;
    sFumacas->sheetSizeX = 4;
    sFumacas->sheetSizeY = 2;
    sFumacas->id         = ID_FUMACA;
    sFumacas->repetirElementosX = 0;
    sFumacas->repetirElementosY = 0;
	sFumacas->novaAltura  = -1;
	sFumacas->novaLargura = -1;

	for(i = 0; i < 3; i++){
		sFumacas->colunaAtual	[i] = 0;
		sFumacas->linhaAtual	[i] = 0;
		sFumacas->posX			[i] = telaJogo->largura + 100;
		sFumacas->posY			[i] = telaJogo->altura - 120;
		sFumacas->offsetX		[i] = 0;
		sFumacas->offsetY		[i] = 0;
		sFumacas->profundidade	[i] = 1.5;
		sFumacas->dirX			[i] = -1;
		sFumacas->dirY			[i] = 1;
		sFumacas->velX			[i] = 1.0;
		sFumacas->velY			[i] = 0.0;
		sFumacas->inicioY		[i] = sFumacas->posY[i];
		sFumacas->inicioX		[i] = telaJogo->largura;
		sFumacas->fimY			[i] = sFumacas->posY[i];
		sFumacas->fimX			[i] = 0.0;
		sFumacas->spriteId		[i] = 0; // O ID da fumaça corresponde ao ID do fantasma
		sFumacas->loop			[i] = NAO;
		sFumacas->eliminado		[i] = 0;
		sFumacas->intervalo		[i] = 3;
		sFumacas->contIntervalo	[i] = 3;
		sFumacas->contRegressiva[i] = 8;
		sFumacas->habilitado	[i] = NAO;
	}

    /** configura spritesheet Fantasmas**/
    sFantasmas->bitmap = al_load_bitmap("img/ghost/png/fantasmas.png");
    sFantasmas->x1         = 0.0;
    sFantasmas->y1         = 0.0;
    sFantasmas->largura    = 64.0;
    sFantasmas->altura     = 77.0;
    sFantasmas->sheetSizeX = 3;
    sFantasmas->sheetSizeY = 1;
    sFantasmas->id         = ID_GHOST;
    sFantasmas->repetirElementosX = 0;
    sFantasmas->repetirElementosY = 0;
	sFantasmas->novaAltura  = -1;
	sFantasmas->novaLargura = -1;
	counter = 0;

    for (i = 0; i < sFantasmas->sheetSizeX + sFantasmas->repetirElementosX; i++){
        for (j = 0; j < sFantasmas->sheetSizeY + sFantasmas->repetirElementosY; j++){
            float r = (rand() / 1000000000.0);
            float r2 = (rand() / 1000000000.0);
            sFantasmas->posX		[counter] = (telaJogo->largura + (r * sFantasmas->largura)) * counter;
            sFantasmas->posY		[counter] = telaJogo->altura - 120;
            sFantasmas->offsetX		[counter] = 0;
            sFantasmas->offsetY		[counter] = 0;
            sFantasmas->profundidade[counter] = 1.5;
            sFantasmas->dirX		[counter] = -1;
            sFantasmas->dirY		[counter] = 1;
            sFantasmas->velX		[counter] = (0.9 + r);
            sFantasmas->velY		[counter] = 0.0;
            sFantasmas->inicioY		[counter] = sFantasmas->posY[counter];
            sFantasmas->inicioX		[counter] = telaJogo->largura;
            sFantasmas->fimY		[counter] = sFantasmas->posY[counter];
            sFantasmas->fimX		[counter] = 0.0;
            sFantasmas->spriteId	[counter] = counter;
            sFantasmas->loop		[counter] = SIM;
			sFantasmas->eliminado	[counter] = 0;
            sFantasmas->intervalo	 [counter] = 0;
            sFantasmas->contIntervalo[counter] = 0;
            sFantasmas->habilitado	 [counter] = SIM;
           counter++;
        }
    }

    /** configura spritesheet do gramado**/
	sGrama1->bitmap = al_load_bitmap("img/grass/png/grama.png");
    sGrama1->x1         = 0.0;
    sGrama1->y1         = 0.0;
    sGrama1->largura    = 256.0;
    sGrama1->altura     = 51;
    sGrama1->sheetSizeX = 1;
    sGrama1->sheetSizeY = 6;
    sGrama1->id         = ID_GRAMA;
    sGrama1->repetirElementosX = 0;
    sGrama1->repetirElementosY = 3;
	sGrama1->novaAltura  = -1;
	sGrama1->novaLargura = -1;
	counter = 0;

    for (i = 0; i < sGrama1->sheetSizeX + sGrama1->repetirElementosX; i++){
        for (j = 0; j < sGrama1->sheetSizeY + sGrama1->repetirElementosY; j++){
			sGrama1->colunaAtual	[counter] = 0;
			sGrama1->linhaAtual 	[counter] = 0;
            sGrama1->posX			[counter] = counter * sGrama1->largura;
            sGrama1->posY			[counter] = telaJogo->altura - sGrama1->altura + 15;
            sGrama1->offsetX		[counter] = 0;
            sGrama1->offsetY		[counter] = 0;
            sGrama1->dirX			[counter] = -1;
            sGrama1->dirY			[counter] = 1;
            sGrama1->velX			[counter] = 1.0;
            sGrama1->velY			[counter] = 0.0;
            sGrama1->inicioY		[counter] = sGrama1->posY[counter];
            sGrama1->inicioX		[counter] = telaJogo->largura;
            sGrama1->fimY			[counter] = sGrama1->posY[counter];
            sGrama1->fimX			[counter] = 0.0;
            sGrama1->spriteId		[counter] = counter;
            sGrama1->loop			[counter] = SIM;
            sGrama1->profundidade	[counter] = 1.5;
            sGrama1->intervalo		[counter] = 0;
            sGrama1->contIntervalo	[counter] = 0;
            sGrama1->habilitado	 	[counter] = 1;
           counter++;
        }
    }

	sGrama2->bitmap = al_load_bitmap("img/grass/png/grama.png");
    sGrama2->x1         = 0.0;
    sGrama2->y1         = 0.0;
    sGrama2->largura    = 256.0;
    sGrama2->altura     = 51;
    sGrama2->sheetSizeX = 1;
    sGrama2->sheetSizeY = 6;
    sGrama2->id         = ID_GRAMA;
    sGrama2->repetirElementosX = 0;
    sGrama2->repetirElementosY = 3;
	sGrama2->novaAltura  = -1;
	sGrama2->novaLargura = -1;
	counter = 0;

    for (i = 0; i < sGrama2->sheetSizeX + sGrama2->repetirElementosX; i++){
        for (j = 0; j < sGrama2->sheetSizeY + sGrama2->repetirElementosY; j++){
			sGrama2->colunaAtual	[counter] = 0;
			sGrama2->linhaAtual 	[counter] = 0;
			sGrama2->posX			[counter] = counter * sGrama2->largura;
			sGrama2->posY			[counter] = telaJogo->altura - sGrama2->altura;
			sGrama2->offsetX		[counter] = 0;
			sGrama2->offsetY		[counter] = 0;
			sGrama2->dirX			[counter] = -1;
			sGrama2->dirY			[counter] = 1;
			sGrama2->velX			[counter] = 1.0;
			sGrama2->velY			[counter] = 0.0;
			sGrama2->inicioY		[counter] = sGrama2->posY[counter];
			sGrama2->inicioX		[counter] = telaJogo->largura;
			sGrama2->fimY			[counter] = sGrama2->posY[counter];
			sGrama2->fimX			[counter] = 0.0;
			sGrama2->spriteId		[counter] = counter;
			sGrama2->loop			[counter] = SIM;
			sGrama2->profundidade	[counter] = 1.1;
			sGrama2->intervalo	 	[counter] = 0;
			sGrama2->contIntervalo	[counter] = 0;
			sGrama2->habilitado	 	[counter] = 1;
			counter++;
        }
    }


    /** configura arvores 1 **/
	sArvores1->bitmap = al_load_bitmap("img/trees/png/arvores3.png");
    sArvores1->x1         = 0.0;
    sArvores1->y1         = 0.0;
    sArvores1->largura    = 170.0;
    sArvores1->altura     = 179;
    sArvores1->sheetSizeX = 3;
    sArvores1->sheetSizeY = 1;
    sArvores1->id         = ID_ARVORE;
    sArvores1->repetirElementosX = 0;
    sArvores1->repetirElementosY = 0;
	sArvores1->novaAltura  = -1;
	sArvores1->novaLargura = -1;
	counter = 0;

    for (i = 0; i < sArvores1->sheetSizeX + sArvores1->repetirElementosX; i++){
        for (j = 0; j < sArvores1->sheetSizeY + sArvores1->repetirElementosY; j++){
 			srand(i+j+counter*2);
			r = (rand() / 10000000.0);
			sArvores1->colunaAtual		[counter] = 0;
			sArvores1->linhaAtual 		[counter] = 0;
            sArvores1->posX				[counter] = counter * sArvores1->largura + (r*10);
            sArvores1->posY				[counter] = telaJogo->altura - sArvores1->altura - 15;
            sArvores1->offsetX			[counter] = 0;
            sArvores1->offsetY			[counter] = 0;
            sArvores1->dirX				[counter] = -1;
            sArvores1->dirY				[counter] = 1;
            sArvores1->velX				[counter] = 1.0;
            sArvores1->velY				[counter] = 0.0;
            sArvores1->inicioY			[counter] = sArvores1->posY[counter];
            sArvores1->inicioX			[counter] = telaJogo->largura;
            sArvores1->fimY				[counter] = sArvores1->posY[counter];
            sArvores1->fimX				[counter] = 0.0;
            sArvores1->spriteId			[counter] = counter;
            sArvores1->loop				[counter] = SIM;
            sArvores1->profundidade		[counter] = 0.9;
            sArvores1->intervalo		[counter] = 0;
            sArvores1->contIntervalo	[counter] = 0;
			sArvores1->habilitado	 	[counter] = 1;
            counter++;
        }
    }

    /** configura arvores 2 **/
	sArvores2->bitmap = al_load_bitmap("img/trees/png/arvores4.png");
    sArvores2->x1         = 0.0;
    sArvores2->y1         = 0.0;
    sArvores2->largura    = 512.0;
    sArvores2->altura     = 100;
    sArvores2->sheetSizeX = 3;
    sArvores2->sheetSizeY = 1;
    sArvores2->id         = ID_ARVORE;
    sArvores2->repetirElementosX = 1;
    sArvores2->repetirElementosY = 0;
	sArvores2->novaAltura  = -1;
	sArvores2->novaLargura = -1;
	counter = 0;

    for (i = 0; i < sArvores2->sheetSizeX + sArvores2->repetirElementosX; i++){
        for (j = 0; j < sArvores2->sheetSizeY + sArvores2->repetirElementosY; j++){
			srand(i+j+counter);
			r = (rand() / 10000000.0);
			sArvores2->colunaAtual		[counter] = 0;
			sArvores2->linhaAtual 		[counter] = 0;
            sArvores2->posX			[counter] = counter * sArvores2->largura + (r * 2);
            sArvores2->posY			[counter] = telaJogo->altura - sArvores2->altura - 20;
            sArvores2->offsetX		[counter] = 0;
            sArvores2->offsetY		[counter] = 0;
            sArvores2->dirX			[counter] = -1;
            sArvores2->dirY			[counter] = 1;
            sArvores2->velX			[counter] = 1.0;
            sArvores2->velY			[counter] = 0.0;
            sArvores2->inicioY		[counter] = sArvores2->posY[counter];
            sArvores2->inicioX		[counter] = telaJogo->largura;
            sArvores2->fimY			[counter] = sArvores2->posY[counter];
            sArvores2->fimX			[counter] = 0.0;
            sArvores2->spriteId		[counter] = counter;
            sArvores2->loop			[counter] = SIM;
            sArvores2->profundidade	[counter] = 0.7;
            sArvores2->intervalo	[counter] = 0;
            sArvores2->contIntervalo[counter] = 0;
			sArvores2->habilitado 	[counter] = 1;
            counter++;
        }
    }

    /** configura nevoa **/
	sNevoa1->bitmap = al_load_bitmap("img/fog/png/fog2.png");
    sNevoa1->x1         = 0.0;
    sNevoa1->y1         = 0.0;
    sNevoa1->largura    = 128.0;
    sNevoa1->altura     = 32;
    sNevoa1->sheetSizeX = 1;
    sNevoa1->sheetSizeY = 1;
    sNevoa1->id         = ID_NEVOA;
    sNevoa1->repetirElementosX = 3;
    sNevoa1->repetirElementosY = 0;
    sNevoa1->novaAltura  = 128;
    sNevoa1->novaLargura = 256;

	sNevoa2->bitmap = al_load_bitmap("img/fog/png/fog3.png");
    sNevoa2->x1         = 0.0;
    sNevoa2->y1         = 0.0;
    sNevoa2->largura    = 128.0;
    sNevoa2->altura     = 32;
    sNevoa2->sheetSizeX = 1;
    sNevoa2->sheetSizeY = 1;
    sNevoa2->id         = ID_NEVOA;
    sNevoa2->repetirElementosX = 3;
    sNevoa2->repetirElementosY = 0;
    sNevoa2->novaAltura  = 128;
    sNevoa2->novaLargura = 256;

	sNevoa3->bitmap = al_load_bitmap("img/fog/png/fog4.png");
    sNevoa3->x1         = 0.0;
    sNevoa3->y1         = 0.0;
    sNevoa3->largura    = 128.0;
    sNevoa3->altura     = 32;
    sNevoa3->sheetSizeX = 1;
    sNevoa3->sheetSizeY = 1;
    sNevoa3->id         = ID_NEVOA;
    sNevoa3->repetirElementosX = 3;
    sNevoa3->repetirElementosY = 0;
    sNevoa3->novaAltura  = 256;
    sNevoa3->novaLargura = 512;

	sNevoa4->bitmap = al_load_bitmap("img/fog/png/fog6.png");
    sNevoa4->x1         = 0.0;
    sNevoa4->y1         = 0.0;
    sNevoa4->largura    = 128.0;
    sNevoa4->altura     = 32;
    sNevoa4->sheetSizeX = 1;
    sNevoa4->sheetSizeY = 1;
    sNevoa4->id         = ID_NEVOA;
    sNevoa4->repetirElementosX = 3;
    sNevoa4->repetirElementosY = 0;
    sNevoa4->novaAltura  = 128;
    sNevoa4->novaLargura = 256;

	counter = 0;

    for (i = 0; i < sNevoa1->sheetSizeX + sNevoa1->repetirElementosX; i++){
        for (j = 0; j < sNevoa1->sheetSizeY + sNevoa1->repetirElementosY; j++){
			srand(i+j+counter);
			r = (rand() / 100000000.0);
			sNevoa1->colunaAtual	[counter] = 0;
			sNevoa1->linhaAtual		[counter] = 0;
            sNevoa1->posX			[counter] = counter * sNevoa1->largura + (r * 4);
            sNevoa1->posY			[counter] = telaJogo->altura - sNevoa1->altura - 60;
            sNevoa1->offsetX		[counter] = 0;
            sNevoa1->offsetY		[counter] = 0;
            sNevoa1->dirX			[counter] = -1;
            sNevoa1->dirY			[counter] = 1;
            sNevoa1->velX			[counter] = 1.0;
            sNevoa1->velY			[counter] = 0.0;
            sNevoa1->inicioY		[counter] = sNevoa1->posY[counter];
            sNevoa1->inicioX		[counter] = telaJogo->largura;
            sNevoa1->fimY			[counter] = sNevoa1->posY[counter];
            sNevoa1->fimX			[counter] = 0.0;
            sNevoa1->spriteId		[counter] = counter;
            sNevoa1->loop			[counter] = SIM;
            sNevoa1->profundidade	[counter] = 0.5;
            sNevoa1->intervalo	 	[counter] = 0;
            sNevoa1->contIntervalo	[counter] = 0;
			sNevoa1->habilitado 	[counter] = 1;

			sNevoa2->colunaAtual	[counter] = 0;
			sNevoa2->linhaAtual		[counter] = 0;
            sNevoa2->posX			[counter] = counter * sNevoa2->largura + (r * 2);
            sNevoa2->posY			[counter] = telaJogo->altura - sNevoa2->altura - 60;
            sNevoa2->offsetX		[counter] = 0;
            sNevoa2->offsetY		[counter] = 0;
            sNevoa2->dirX			[counter] = -1;
            sNevoa2->dirY			[counter] = 1;
            sNevoa2->velX			[counter] = 1.0;
            sNevoa2->velY			[counter] = 0.0;
            sNevoa2->inicioY		[counter] = sNevoa2->posY[counter];
            sNevoa2->inicioX		[counter] = telaJogo->largura;
            sNevoa2->fimY			[counter] = sNevoa2->posY[counter];
            sNevoa2->fimX			[counter] = 0.0;
            sNevoa2->spriteId		[counter] = counter;
            sNevoa2->loop			[counter] = SIM;
            sNevoa2->profundidade	[counter] = 0.6;
            sNevoa2->intervalo	 	[counter] = 0;
            sNevoa2->contIntervalo	[counter] = 0;
			sNevoa2->habilitado 	[counter] = 1;

			sNevoa3->colunaAtual	[counter] = 0;
			sNevoa3->linhaAtual		[counter] = 0;
            sNevoa3->posX			[counter] = counter * sNevoa2->largura + (r * 2) + 200;
            sNevoa3->posY			[counter] = telaJogo->altura - sNevoa3->altura - 60;
            sNevoa3->offsetX		[counter] = 0;
            sNevoa3->offsetY		[counter] = 0;
            sNevoa3->dirX			[counter] = -1;
            sNevoa3->dirY			[counter] = 1;
            sNevoa3->velX			[counter] = 1.0;
            sNevoa3->velY			[counter] = 0.0;
            sNevoa3->inicioY		[counter] = sNevoa3->posY[counter];
            sNevoa3->inicioX		[counter] = telaJogo->largura;
            sNevoa3->fimY			[counter] = sNevoa3->posY[counter];
            sNevoa3->fimX			[counter] = 0.0;
            sNevoa3->spriteId		[counter] = counter;
            sNevoa3->loop			[counter] = SIM;
            sNevoa3->profundidade	[counter] = 0.7;
            sNevoa3->intervalo	 	[counter] = 0;
            sNevoa3->contIntervalo	[counter] = 0;
			sNevoa3->habilitado 	[counter] = 1;

			sNevoa4->colunaAtual	[counter] = 0;
			sNevoa4->linhaAtual		[counter] = 0;
            sNevoa4->posX			[counter] = counter * sNevoa4->largura + (r * 2) + 200;
            sNevoa4->posY			[counter] = telaJogo->altura - sNevoa4->altura - 60;
            sNevoa4->offsetX		[counter] = 0;
            sNevoa4->offsetY		[counter] = 0;
            sNevoa4->dirX			[counter] = -1;
            sNevoa4->dirY			[counter] = 1;
            sNevoa4->velX			[counter] = 1.0;
            sNevoa4->velY			[counter] = 0.0;
            sNevoa4->inicioY		[counter] = sNevoa4->posY[counter];
            sNevoa4->inicioX		[counter] = telaJogo->largura;
            sNevoa4->fimY			[counter] = sNevoa4->posY[counter];
            sNevoa4->fimX			[counter] = 0.0;
            sNevoa4->spriteId		[counter] = counter;
            sNevoa4->loop			[counter] = SIM;
            sNevoa4->profundidade	[counter] = 1.4;
            sNevoa5->intervalo	 	[counter] = 0;
            sNevoa5->contIntervalo	[counter] = 0;
			sNevoa5->habilitado 	[counter] = 1;

            counter++;
        }
    }

    return;
}
//----------------------------------------------------------------------
int pi_iniScreens(GameScreen *telaJogo){
	/* Inicializa todas as telas principais de desenho do jogo */

	telaJogo->canvas	= al_create_bitmap(1280, 720);
	telaJogo->x1		= 0;
	telaJogo->x2		= 1280;
	telaJogo->y1		= 0;
	telaJogo->y2		= 720;
	telaJogo->largura	= 1280;
	telaJogo->altura	= 720;
	telaJogo->scaledX	= 1.0;
	telaJogo->scaledY	= 1.0;
	telaJogo->scaledW	= 1280.0;
	telaJogo->scaledH	= 720.0;
	telaJogo->id		= NATIVE_SCREEN;
}
//----------------------------------------------------------------------
int pi_iniAllegroAddons(GameDisplay *display){
	DEBUG_ON("\n----debug:iniAllegroAddons():start");

	/* ATENÇÃO! Todos os addons e estrutura de dados devem ser incializados dentro desta função */

	if (!al_init()){
		al_show_native_message_box(display->backbuffer, "Erro!", "Allegro 5.x", "Falha ao inicializar o Allegro 5!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if (!al_init_image_addon()){
		al_show_native_message_box(display->backbuffer, "Erro!", "Image:", "Falha ao inicializar al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if (!al_init_primitives_addon()){
		al_show_native_message_box(display->backbuffer, "Erro!", "Primitives:", "Falha ao inicializar allegro_primitives!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if (!al_install_keyboard()){
		al_show_native_message_box(display->backbuffer, "Erro!", "Keyboard:", "Falha ao inicializar o teclado!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	DEBUG_ON("\n----debug:iniAllegroAddons():end");
	return 0;
}
//----------------------------------------------------------------------
void pi_setDisplayScale(GameScreen *telaJogo, GameDisplay *displayJogo){
	/* Calcula o valor da escala do monitor de acordo
	   com a resolução inicial em que o jogo foi criado e a do monitor em uso */

	DEBUG_ON("\n----debug:setDisplayScale():start");
	// Calcula a proporção da escala
	float sX, sY;

	sX = (float)(displayJogo->largura) / telaJogo->largura;
	sY = (float)(displayJogo->altura ) / telaJogo->altura;

	/* Acha qual é o menor entre sx e sy e calcula a escala.
	   O cálculo é feito pelo menor valor para evitar que a imagem não ultrapasse os limites do monitor.*/
	if (sX < sY){
		displayJogo->scale = sX;
	}
	else{
		displayJogo->scale = sY;
	}

	/* Calcula as escalas a serem utilizadas */
	telaJogo->scaledW = telaJogo->largura  * displayJogo->scale;
	telaJogo->scaledH = telaJogo->altura * displayJogo->scale;

	/* Configura as novas escalas para X e Y */

	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	al_scale_transform(&trans, sX, sY);
	al_use_transform(&trans);

	DEBUG_ON("\ndebug:displayJogo:scale:%f",	displayJogo->scale);
	DEBUG_ON("\ndebug:telaJogo:scaledW:%f",		telaJogo->scaledW);
	DEBUG_ON("\ndebug:telaJogo:scaledH:%f",		telaJogo->scaledH);
	DEBUG_ON("\ndebug:displayJogo:largura:%d",	displayJogo->largura);
	DEBUG_ON("\ndebug:displayJogo:altura:%d",	displayJogo->altura);
	DEBUG_ON("\ndebug:telaJogo:largura:%d",		telaJogo->largura);
	DEBUG_ON("\ndebug:telaJogo:altura:%d\n",	telaJogo->altura);

	DEBUG_ON("\n----debug:setDisplayScale():end\n");
	return;
}
//----------------------------------------------------------------------BGIma
int pi_criaDisplay(GameScreen *telaJogo, GameDisplay *display){

	DEBUG_ON("\n----debug:setFullScreen():start");
	// Configura para tela cheia.

	//al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_16_WITH_ALPHA);

	ALLEGRO_DISPLAY_MODE disp_data;

	al_set_new_display_option(ALLEGRO_RENDER_METHOD,		1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER,		0, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_SWAP_METHOD,			2, ALLEGRO_SUGGEST);

//	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_flags(ALLEGRO_WINDOWED);

	display->largura = telaJogo->largura;
	display->altura  = telaJogo->altura;

	// Cria o buffer de desenho da tela. Todas as outras imagens são criadas dentro dele, assim basta mudar a escala dele.
	// e todo o resto é ajustado automaticamente.
	display->backbuffer = al_create_display(display->largura, display->altura); // Cria o display em tela cheia.
	telaJogo->canvas	= al_create_bitmap(telaJogo->largura, telaJogo->altura);


	if (!display->backbuffer){
		al_show_native_message_box(display->backbuffer, "Erro", "Erro", "Falha ao inicializar o display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display->backbuffer);
		return -1;
	}

	if (!telaJogo->canvas){
		al_show_native_message_box(display->backbuffer, "Erro", "Erro", "Falha ao inicializar o displayBuffer!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	DEBUG_ON("\n----debug:setFullScreen():end\n");
	return 0;
}
//----------------------------------------------------------------------
