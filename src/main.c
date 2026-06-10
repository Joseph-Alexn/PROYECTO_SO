#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "parser.h"
#include "executor.h"
#include "jobs.h"
#include "history.h"

struct termios terminal_original;

void deshabilitar_modo_crudo() {
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal_original);
}

void habilitar_modo_crudo() {
    struct termios terminal_cruda;
    tcgetattr(STDIN_FILENO, &terminal_original);
    atexit(deshabilitar_modo_crudo); 

    terminal_cruda = terminal_original;
    terminal_cruda.c_lflag &= ~(ICANON | ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal_cruda);
}

int leer_comando_interactivo(char *buffer) {
    int pos = 0;
    char c;
    
    buffer[0] = '\0';

    while (1) {
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            return 0;
        }

        if (c == '\n') {
            printf("\n");
            buffer[pos] = '\0';
            return 1;
        } else if (c == 127 || c == '\b') { 
            if (pos > 0) {
                pos--;
                buffer[pos] = '\0';
                printf("\b \b"); 
                fflush(stdout);
            }
        } else if (c == '\033') { 
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                if (seq[1] == 'A') { 
                    if (history_count > 0 && history_index > 0) {
                        history_index--;
                        strcpy(buffer, historial[history_index]);
                        pos = strlen(buffer);
                        printf("\33[2K\rucvsh> %s", buffer);
                        fflush(stdout);
                    }
                } else if (seq[1] == 'B') { 
                    if (history_count > 0 && history_index < history_count) {
                        history_index++;
                        if (history_index == history_count) {
                            buffer[0] = '\0';
                            pos = 0;
                        } else {
                            strcpy(buffer, historial[history_index]);
                            pos = strlen(buffer);
                        }
                        printf("\33[2K\rucvsh> %s", buffer);
                        fflush(stdout);
                    }
                }
            }
        } else { 
            if (pos < MAX_LINE_LEN - 1) {
                buffer[pos++] = c;
                buffer[pos] = '\0';
                printf("%c", c);
                fflush(stdout);
            }
        }
    }
}

int main() {
    char linea_entrada[MAX_LINE_LEN];
    ComandoParsed comandos[MAX_COMANDOS_LINEA];
    int total_comandos = 0;

    inicializar_jobs();
    inicializar_historial();
    habilitar_modo_crudo();

    while (1) {
        limpiar_jobs_zombies();

        printf("ucvsh> ");
        fflush(stdout);

        if (!leer_comando_interactivo(linea_entrada)) {
            printf("\nSaliendo de ucvsh...\n");
            break; 
        }

        if (strlen(linea_entrada) == 0) {
            continue;
        }

        agregar_al_historial(linea_entrada);

        parsear_linea(linea_entrada, comandos, &total_comandos);

        if (total_comandos > 0 && comandos[0].comando_principal != NULL) {
            if (strcmp(comandos[0].comando_principal, "exit") == 0) {
                break;
            }
        }

        if (total_comandos > 0) {
            ejecutar_cadena_comandos(comandos, total_comandos);
        }
    }

    return EXIT_SUCCESS;
}
