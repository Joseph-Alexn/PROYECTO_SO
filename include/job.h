#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_JOBS 32

typedef struct {
    pid_t pid;
    char comando_txt[256];
    int activo;
} Job;

void inicializar_jobs(void);
void agregar_job(pid_t pid, const char *comando_txt);
void listar_jobs(void);
void limpiar_jobs_zombies(void);

#endif