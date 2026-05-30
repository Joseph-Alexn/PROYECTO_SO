#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "path_resolver.h"

char *resolver_ruta(const char *comando){

	if(strchr(comando, '/') != NULL){
		if(access(comando, X_OK) == 0){
			return strdup(comando);
		}
		return NULL;
	}
	char *path_env = getenv("PATH");
	if(path_env == NULL){
		return NULL;
	}

	char *path_copy = strdup(path_env);
	if(path_copy == NULL){
		perror("Error de memoria al copiar PATH");
		return NULL;
	}
	char *directorio = strtok(path_copy, ":");
	char buffer_ruta[1024];

	while(directorio != NULL){
	
		snprintf(buffer_ruta, sizeof(buffer_ruta), "%s/%s", directorio, comando);
		struct stat st;
		if(stat(buffer_ruta, &st) == 0 && S_ISREG(st.st_mode) && access(buffer_ruta, X_OK) == 0){
			free(path_copy);
			return strdup(buffer_ruta);
		}
	
		directorio = strtok(NULL, ":");
	}
free(path_copy);
return NULL;
}
