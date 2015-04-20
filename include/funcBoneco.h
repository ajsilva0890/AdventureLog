#ifndef FUNCBONECO_H
#define FUNCBONECO_H
	

void jogadorPulo(bonecoJog *);
void jogadorAbaixar(bonecoJog *);
void jogadorCorrer(bonecoJog *);

void clique_KEY_W(bonecoJog *);
void clique_KEY_S(bonecoJog *);

void drawJogador(bonecoJog *, ALLEGRO_BITMAP *);

int colisao (float , float , float , float, /*jogador*/
             float , float , float , float, /*inimigo*/
             int, int ); /*diferença da caixa de colisao*/



#endif