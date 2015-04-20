void desalocar(){

    al_destroy_bitmap(poderes);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_font(fonte_equacao); 
    al_destroy_timer(timer);

}