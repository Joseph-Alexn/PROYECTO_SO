#include <string.h>
#include <stdio.h>
#include "parser.h"

void parsear_linea(char *linea_cruda, ComandoParsed comandos[], int *total_comandos) {
    int cmd_idx = 0;
    int arg_idx = 0;
    
    // Inicializar el primer comando
    comandos[cmd_idx].comando_principal = NULL;
    comandos[cmd_idx].archivo_redireccion = NULL;
    comandos[cmd_idx].es_background = 0;
    comandos[cmd_idx].operador_siguiente = OP_NINGUNO;

    char *token = strtok(linea_cruda, " \t");

    while (token != NULL && cmd_idx < MAX_COMANDOS_LINEA) {
        if (strcmp(token, "&") == 0) {
            comandos[cmd_idx].es_background = 1;
        } 
        else if (strcmp(token, ";") == 0) {
            comandos[cmd_idx].argumentos[arg_idx] = NULL;
            comandos[cmd_idx].operador_siguiente = OP_SECUENCIAL; 
            cmd_idx++;
            arg_idx = 0;
            comandos[cmd_idx].comando_principal = NULL; 
            comandos[cmd_idx].archivo_redireccion = NULL;
            comandos[cmd_idx].es_background = 0;
            comandos[cmd_idx].operador_siguiente = OP_NINGUNO;
        } 
        else if (strcmp(token, "&&") == 0) {
            comandos[cmd_idx].argumentos[arg_idx] = NULL;
            comandos[cmd_idx].operador_siguiente = OP_AND; 
            cmd_idx++;
            arg_idx = 0;
            comandos[cmd_idx].comando_principal = NULL;
            comandos[cmd_idx].archivo_redireccion = NULL;
            comandos[cmd_idx].es_background = 0;
            comandos[cmd_idx].operador_siguiente = OP_NINGUNO;
        } 
        else if (strcmp(token, "||") == 0) {
            comandos[cmd_idx].argumentos[arg_idx] = NULL;
            comandos[cmd_idx].operador_siguiente = OP_OR; 
            cmd_idx++;
            arg_idx = 0;
            comandos[cmd_idx].comando_principal = NULL;
            comandos[cmd_idx].archivo_redireccion = NULL;
            comandos[cmd_idx].es_background = 0;
            comandos[cmd_idx].operador_siguiente = OP_NINGUNO;
        } 
        else if (strcmp(token, "|") == 0) {
            comandos[cmd_idx].argumentos[arg_idx] = NULL;
            comandos[cmd_idx].operador_siguiente = OP_PIPE;
            cmd_idx++;
            arg_idx = 0;
            comandos[cmd_idx].comando_principal = NULL;
            comandos[cmd_idx].archivo_redireccion = NULL;
            comandos[cmd_idx].es_background = 0;
            comandos[cmd_idx].operador_siguiente = OP_NINGUNO;
        } 
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t");
            if (token != NULL) {
                comandos[cmd_idx].archivo_redireccion = token;
            }
        } 
        else {
            if (arg_idx < MAX_ARGS - 1) {
                if (arg_idx == 0) {
                    comandos[cmd_idx].comando_principal = token; 
                }
                comandos[cmd_idx].argumentos[arg_idx] = token;
                arg_idx++;
            }
        }
        token = strtok(NULL, " \t");
    }

    comandos[cmd_idx].argumentos[arg_idx] = NULL;
    if (comandos[cmd_idx].comando_principal != NULL) {
        cmd_idx++;
    }
    *total_comandos = cmd_idx;
}