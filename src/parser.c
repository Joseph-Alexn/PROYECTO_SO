#include <stdio.h>
#include <string.h>
#include "parser.h"

void parsear_linea(char *linea_cruda, ComandoParsed *cmd){

	int i = 0;
	cmd->es_background = 0;
	char *token = strtok(linea_cruda, " \t");

	while(token != NULL && i < MAX_ARGS - 1){

		if(strcmp(token, "&") == 0){
		
			cmd->es_background = 1;

		}else{
		
			cmd->argumentos[i] = token;
			i++;
		}

		token = strtok(NULL, " \t");
	}

	cmd->argumentos[i] = NULL;

	if(i>0){
		cmd->comando_principal = cmd->argumentos[0];
	}else{
		cmd->comando_principal = NULL;
	}
}
