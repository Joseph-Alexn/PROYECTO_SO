#ifndef HISTORY_H
#define HISTORY_H

#define MAX_HISTORY 100
#define MAX_LINE_LEN 256

extern char historial[MAX_HISTORY][MAX_LINE_LEN];
extern int history_count;
extern int history_index;

void inicializar_historial();
void agregar_al_historial(const char *comando);
void guardar_historial_en_disco();

#endif 
