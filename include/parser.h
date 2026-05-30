#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 64

typedef struct{

	char *comando_principal;
	char *argumentos[MAX_ARGS];
	int es_background;
} ComandoParsed;

void parsear_linea(char *linea_cruda, ComandoParsed *cmd);

#endif
