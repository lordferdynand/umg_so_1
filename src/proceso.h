#ifndef PROCESO_H
#define PROCESO_H

#define MAX_NOMBRE_PROCESO 64

typedef struct {
    int id;
    int id_padre;
    char contador_programa[MAX_NOMBRE_PROCESO];
    int registros;
    int tamano_bytes;
    int hilos;
    int quantum;
    int iteraciones;
} Proceso;

#endif