#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "jobs.h"

static Job tabla_jobs[MAX_JOBS];

void inicializar_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        tabla_jobs[i].activo = 0;
    }
}

void agregar_job(pid_t pid, const char *comando_txt) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!tabla_jobs[i].activo) {
            tabla_jobs[i].pid = pid;
            strncpy(tabla_jobs[i].comando_txt, comando_txt, 255);
            tabla_jobs[i].activo = 1;
            printf("[%d] %d\n", i + 1, pid); [cite: 76]
            return;
        }
    }
    fprintf(stderr, "ucvsh: Tabla de trabajos llena\n");
}

void listar_jobs(void) {
    limpiar_jobs_zombies(); [cite: 43]
    for (int i = 0; i < MAX_JOBS; i++) {
        if (tabla_jobs[i].activo) {
            printf("[%d] En Ejecución\t%s &\n", i + 1, tabla_jobs[i].comando_txt); [cite: 48, 78]
        }
    }
}

void limpiar_jobs_zombies(void) {
    int status;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (tabla_jobs[i].activo) {
            // WNOHANG verifica el estado sin bloquear la shell
            pid_t res = waitpid(tabla_jobs[i].pid, &status, WNOHANG);
            if (res > 0) { // El hijo ya terminó
                tabla_jobs[i].activo = 0;
            }
        }
    }
}