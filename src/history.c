#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "history.h"

char historial[MAX_HISTORY][MAX_LINE_LEN];
int history_count = 0;
int history_index = 0;
char ruta_historial[512];

void obtener_ruta_historial(){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    snprintf(ruta_historial, sizeof(ruta_historial), "%s/.ucvsh_history", homedir);
}

void inicializar_historial(){
    obtener_ruta_historial();
    FILE *file = fopen(ruta_historial, "r");
    
    if(file == NULL){

       return;
    }

    char linea[MAX_LINE_LEN];
    while(fgets(linea, sizeof(linea), file) != NULL && history_count < MAX_HISTORY){

        linea[strcspn(linea, "\n")] = 0;
	strncpy(historial[history_count], linea, MAX_LINE_LEN);
	history_count++;

    }
    fclose(file);
    history_index = history_count;
}
void agregar_al_historial(const char *comando){

    if(strlen(comando) == 0) return;
    if(history_count > 0 && strcmp(historial[history_count - 1], comando) == 0){
       return;
    }
    if(history_count >= MAX_HISTORY){
	for(int i = 1; i < MAX_HISTORY; i++){
	    strcpy(historial[i-1], historial[i]);
	}
	history_count = MAX_HISTORY - 1;
    }
    strncpy(historial[history_count], comando, MAX_LINE_LEN);
    history_count++;
    history_index = history_count;

    guardar_historial_en_disco();
}

void guardar_historial_en_disco(){

    FILE *file = fopen(ruta_historial, "w");
    if(file == NULL) return;

    for(int i = 0; i < history_count; i++){
	fprintf(file, "%s\n", historial[i]);
    }
    fclose(file);  
}

