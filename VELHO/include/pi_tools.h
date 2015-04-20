/*
 * Biblioteca de ferramentas para controlar o display.
 *
 * Codigo de ajuda para globalizar as variaveis display e displayBuffer:
 * 			http://cboard.cprogramming.com/game-programming/30462-draw-bitmap*-argument-allegro-objects-again.html
 *
*/

#ifndef H_PI_TOOLS
	#define H_PI_TOOLS
	#include <allegro5/allegro.h>
	#include "pi_data.h"

	int pi_iniAllegroAddons(GameDisplay*);

	void pi_setDisplayScale	(GameScreen*, GameDisplay*);
	int pi_criaDisplay		(GameScreen*, GameDisplay*);
	int pi_iniScreens		(GameScreen*);

	void pi_iniImagens(SpriteSheet*, SpriteSheet*, SpriteSheet*, SpriteSheet*,
						SpriteSheet*, SpriteSheet*, SpriteSheet*,
						SpriteSheet*, SpriteSheet*, SpriteSheet*, SpriteSheet*,
						SpriteSheet*, SpriteSheet*,GameScreen*);

	int pi_AnimarSpriteSheet(SpriteSheet*, SpriteSheet*, GameScreen*);
	void pi_drawGraphics(SpriteSheet*);

#endif
