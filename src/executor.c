#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "executor.h"
#include "path_resolver.h"
#include "jobs.h"

int ejecutar_comando_individual(ComandoParsed *cmd, int fd_entrada, int fd_salida) {
    if (cmd->comando_principal == NULL) return -1;

    if (strcmp(cmd->comando_principal, "cd") == 0) {
        int status = 0;
      
        if (cmd->argumentos[1] == NULL) {
            char *home = getenv("HOME");
            if (home != NULL) {
                status = chdir(home);
            }
        } else {
            status = chdir(cmd->argumentos[1]);
        }

        if (status != 0) {
            perror("ucvsh: cd"); 
            return 1; 
        }
        return 0; 
    }

    char *ruta_ejecutable = resolver_ruta(cmd->comando_principal);
    if (ruta_ejecutable == NULL) {
        fprintf(stderr, "ucvsh: %s: no se encontro la orden\n", cmd->comando_principal);
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("ucvsh: Error en fork");
        free(ruta_ejecutable);
        return -1;
    }

    if (pid == 0) {
        if (fd_entrada != STDIN_FILENO) {
            dup2(fd_entrada, STDIN_FILENO);
            close(fd_entrada);
        }

        if (fd_salida != STDOUT_FILENO) {
            dup2(fd_salida, STDOUT_FILENO);
            close(fd_salida);
        }

        if (cmd->archivo_redireccion != NULL) {
            int fd_file = open(cmd->archivo_redireccion, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_file < 0) {
                perror("ucvsh: Error abriendo archivo de redireccion");
                exit(EXIT_FAILURE);
            }
            dup2(fd_file, STDOUT_FILENO);
            close(fd_file);
        }

        execv(ruta_ejecutable, cmd->argumentos);
        perror("ucvsh: Error en execv");
        exit(EXIT_FAILURE);
    }

    free(ruta_ejecutable);

    if (cmd->es_background) {
        char comando_completo[256] = "";
        for (int i = 0; cmd->argumentos[i] != NULL; i++) {
            strcat(comando_completo, cmd->argumentos[i]);
            if (cmd->argumentos[i + 1] != NULL) {
                strcat(comando_completo, " "); 
            }
        }

        agregar_job(pid, comando_completo);
        return 0;
    } else {
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("ucvsh: Error en waitpid");
            return -1;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return -1;
    }
}

int ejecutar_cadena_comandos(ComandoParsed comandos[], int total_comandos) {
    int fd_entrada = STDIN_FILENO;
    int i = 0;

    while (i < total_comandos) {
        if (comandos[i].comando_principal != NULL && strcmp(comandos[i].comando_principal, "jobs") == 0) {
            listar_jobs();
            i++;
            continue;
        }

        int fd_pipe[2];
        int es_pipe = (comandos[i].operador_siguiente == OP_PIPE);

        if (es_pipe) {
            if (pipe(fd_pipe) < 0) {
                perror("ucvsh: Error creando pipe");
                return -1;
            }
        }

        int fd_salida = es_pipe ? fd_pipe[1] : STDOUT_FILENO;

        int resultado = ejecutar_comando_individual(&comandos[i], fd_entrada, fd_salida);

        if (fd_entrada != STDIN_FILENO) {
            close(fd_entrada);
        }

        if (es_pipe) {
            close(fd_pipe[1]);
        }

        if (es_pipe) {
            fd_entrada = fd_pipe[0];
            i++;
        } else {
            fd_entrada = STDIN_FILENO;

            if (comandos[i].operador_siguiente == OP_AND) {
                if (resultado != 0) {
                    i++; 
                }
            } else if (comandos[i].operador_siguiente == OP_OR) {
                if (resultado == 0) {
                    i++; 
                }
            }
            i++; 
        }
    }
    return 0;
}
