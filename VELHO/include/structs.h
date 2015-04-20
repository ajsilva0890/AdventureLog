#ifndef STRUCT_H
#define STRUCT_H

	typedef struct {		
	  
	  int alt_Ax, altCorA,
	  	  alt_Bx, altCorB,
	  	  alt_Cx, altCorC,
	  	  pergunta_x,
	  	  pontos_x,
	  	  tempo_x,
	  	  acertos_x,
	  	  mousePoderes_x,
	  	  resultado_x,

	  	  alt_Ay,
	  	  alt_By,
	  	  alt_Cy,
	  	  pergunta_y,
	  	  pontos_y,
	  	  tempo_y,
	  	  acertos_y,
	  	  mousePoderes_y,
	  	  resultado_y;

	} telaAlternativa;


	/* Structs para as perguntas, alternativas e resposta */
	typedef struct { 
	  
	  int produto,
	  	  numero,
	  	  soma,

	  	  alternativa_A,
	 	  alternativa_B,
	  	  alternativa_C,
	  	  resposta,

	  	  acertou,
	  	  pontos,
	  	  nivel,
	  	  atualizarQuestao,

	  	  matarFantasma,
	  	  QntdRespostaMatarFantasma,
	  	  qqcoisa,
	 	  tempo;

	} listaQuestao;


	/* Struct para o fantas da tela */
	typedef struct { 
	  
	  int id;

	} fantasma;


	typedef struct { 
	  
	  int forca_x,
	  	  forca_y,	
	  	  
	  	  vida,

	  	  pos_fixa_y,

	  	  acao,
	  	  alturaPulo,
	  	  alturaAbaixar,
		  
		  max_frame,
	  	  cur_frame,
	  	  frame_count,
	  	  frame_delay;

	float forca,

		  pos_x, 
	  	  pos_y,

	  	  frame_larg,
	  	  frame_alt;

	} bonecoJog;

#endif