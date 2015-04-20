#Pasta com as .h
IDIR=-I include

#Pasta com as .c
SDIR=src
 
#Compilador	
CC=gcc
	
#pkg config do Allegro, incluir mais se necessário
PKG_CONFIG=`pkg-config --libs allegro-5.0 allegro_image-5.0 allegro_ttf-5.0 allegro_font-5.0 allegro_primitives-5.0 allegro_audio-5.0 allegro_acodec-5.0 allegro_dialog-5.0`

# todas as minhas .c 
_SRC=main.c funcPoderes.c equacoes.c funcBoneco.c pi_tools.c 

SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

#nome do meu exe
EXECUTABLE=adventurelog

all:
	gcc $(SRC) -o $(EXECUTABLE) $(PKG_CONFIG) $(IDIR) -std=c99;
