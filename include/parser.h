#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 64
#define MAX_COMANDOS_LINEA 10

typedef enum {
    OP_NINGUNO,
    OP_SECUENCIAL,  // ;
    OP_AND,         // &&
    OP_OR,          // ||
    OP_PIPE         // |
} OperadorLogico;

typedef struct {
    char *comando_principal;
    char *argumentos[MAX_ARGS];
    char *archivo_redireccion; // Para > archivo
    int es_background;
    OperadorLogico operador_siguiente;
} ComandoParsed;

void parsear_linea(char *linea_cruda, ComandoParsed comandos[], int *total_comandos);

#endif