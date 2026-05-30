#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"

#define MAX_BUFFER 1024

int main(){

	char buffer[MAX_BUFFER];
	ComandoParsed cmd;
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
			continue;
		}

		if(strcmp(buffer,"exit")==0){
			printf("Saliendo de ucvsh...\n");
			break;
		}

		parsear_linea(buffer, &cmd);

		ejecutar_comando(&cmd);

	}
	return 0;
}
