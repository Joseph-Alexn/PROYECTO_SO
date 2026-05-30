#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
#include "path_resolver.h"

void ejecutar_comando(ComandoParsed *cmd){
	if(cmd->comando_principal == NULL){
		return;
	}


	char *ruta_ejecutable = resolver_ruta(cmd->comando_principal);
	if(ruta_ejecutable == NULL){
		fprintf(stderr, "ucvsh: %s: no se encontro la orden\n", cmd->comando_principal);
		return;
	}
	pid_t pid = fork();
	
	if(pid < 0){
		perror("ucvsh: Error critico al ejecutor fork");
		free(ruta_ejecutable);
		return;
	}
	else if(pid == 0){
		execv(ruta_ejecutable, cmd->argumentos);
		perror("ucvsh: Error en execv");
		exit(EXIT_FAILURE);
	}
	else{
		if(cmd->es_background == 0){
			int status;
			waitpid(pid, &status, 0);
		}else{
			printf("[1] %d\n", pid);
		}
	}

	free(ruta_ejecutable);
}
