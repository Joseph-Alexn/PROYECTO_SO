#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"
#include "jobs.h"

#define MAX_BUFFER 1024

int main() {
    char buffer[MAX_BUFFER];
    ComandoParsed comandos[MAX_COMANDOS_LINEA];
    int total_comandos;

    inicializar_jobs();
    printf("Iniciando ucvsh...\n");

    while (1) {
        limpiar_jobs_zombies();

        printf("ucvsh> ");
        fflush(stdout);

        if (fgets(buffer, MAX_BUFFER, stdin) == NULL) {
            printf("\n");
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            continue;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Saliendo de ucvsh...\n");
            break;
        }

        total_comandos = 0;
        parsear_linea(buffer, comandos, &total_comandos);

        if (total_comandos > 0) {
            ejecutar_cadena_comandos(comandos, total_comandos);
        }
    }
    return 0;
}