#include <stdio.h>

/* Para ativar o debug, descomentar a linha abaixo */
#define _SET_DEBUG_ON
#ifdef _SET_DEBUG_ON
	#define DEBUG_ON printf
#endif
#ifndef _SET_DEBUG_ON
	#define DEBUG_ON //
#endif

//----------------------------------------------------------------------
void setDisplayScale(int *startW, int *startH, int *targetW, int *targetH, float *displayScale){
	/* Calcula o valor da escala do monitor de acordo
	   com a resolução inicial e a final e salva na variável displayScale */
	
	DEBUG_ON("\ndebug:setDisplayScale\n");
	// Calcula a proporção da escala
	float sx = (*targetW / *startW) * 1.0;
	float sy = (*targetH / *startH) * 1.0;
	
	/* Acha qual é o menor entre sx e sy e calcula a escala.
	   O cálculo é feito pelo menor valor para evitar que a imagem não ultrapasse os limites do monitor.*/
	if (sx > sy){
		*displayScale = sx;
	}
	else{
		*displayScale = sy;
	}
	
	/* Calcula as escalas a serem utilizadas */
	float scaleW	= *startW * *displayScale;
	float scaleH	= *startH * *displayScale;
	
	DEBUG_ON("\ndebug:scale:%f", *displayScale);
	DEBUG_ON("\ndebug:ScaleW:%f", scaleW);
	DEBUG_ON("\ndebug:ScaleH:%f", scaleH);
	DEBUG_ON("\ndebug:displayWidth:%d", *targetW);
	DEBUG_ON("\ndebug:displayHeight:%d", *targetH);
	DEBUG_ON("\ndebug:gameScreenWidth:%d", *startW);
	DEBUG_ON("\ndebug:gameScreenHeight:%d\n", *startH);
	
	return;	
}
//----------------------------------------------------------------------
