#ifndef FUNCPODERES_H
#define FUNCPODERES_H

	void posicaoAlternativas (int, int, telaAlternativa *, bonecoJog * ); 
	void verificarResposta 	(ALLEGRO_SAMPLE *, ALLEGRO_SAMPLE *, telaAlternativa *, listaQuestao *, SpriteSheet *, bonecoJog *);
	void atualizarFantasma (listaQuestao *, fantasma *);
	void atualizarPergunta (listaQuestao *);
	void cronometroRegressivo (ALLEGRO_TIMER *, ALLEGRO_EVENT_QUEUE *, listaQuestao *, int *);
	void ponteiroMouse (ALLEGRO_DISPLAY *, telaAlternativa *);

	void setVeriavel (listaQuestao *);
	void setBoneco (int, int, bonecoJog *, ALLEGRO_BITMAP *);

	void drawResposdeu (ALLEGRO_FONT *, listaQuestao *, telaAlternativa *);
	void drawPergunta (ALLEGRO_FONT *, ALLEGRO_FONT *, listaQuestao *, telaAlternativa *, ALLEGRO_BITMAP *,ALLEGRO_BITMAP *, ALLEGRO_BITMAP * );
	void drawPontos (ALLEGRO_FONT *, listaQuestao *, telaAlternativa *, bonecoJog *);


#endif