#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 1024

void procesar_linea(char *linea);

int main(){

	char buffer[MAX_BUFFER];
	printf("Iniciando ucvsh...\n");
	while(1){
		printf("ucvsh> ");
		fflush(stdout);
		if(fgets(buffer,MAX_BUFFER, stdin)==NULL){
			printf("\n");
			break;
		}

		buffer[strcspn(buffer, "\n")] = '\0';

		if(strlen(buffer)==0){
			cotinue;
		}

		if(strcmp(buffer,"exit")==0){
			TODO:
			printf("Saliendo de ucvsh...\n");
			break;
		}

		procesar_linea(buffer);

	}
	return 0;
}

void procesar_linea(char *linea){

printf("[DEBUG] Comando recibido para analizar: '%s'\n", linea);

}
