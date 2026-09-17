#ifndef COLA_H
#define COLA_H

#include <stddef.h>

#include "proceso.h"

typedef struct NodoProceso {
    Proceso proceso;
    struct NodoProceso *siguiente;
} NodoProceso;

typedef struct {
    NodoProceso *frente;
    NodoProceso *final;
    size_t cantidad;
} ColaProcesos;

void cola_inicializar(ColaProcesos *cola);
int cola_esta_vacia(const ColaProcesos *cola);
int cola_encolar(ColaProcesos *cola, Proceso proceso);
int cola_desencolar(ColaProcesos *cola, Proceso *proceso);
void cola_vaciar(ColaProcesos *cola);

#endif