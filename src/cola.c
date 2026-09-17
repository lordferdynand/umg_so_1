#include "cola.h"

#include <stdlib.h>

void cola_inicializar(ColaProcesos *cola)
{
    cola->frente = NULL;
    cola->final = NULL;
    cola->cantidad = 0;
}

int cola_esta_vacia(const ColaProcesos *cola)
{
    return cola->frente == NULL;
}

int cola_encolar(ColaProcesos *cola, Proceso proceso)
{
    NodoProceso *nuevo = malloc(sizeof(*nuevo));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->proceso = proceso;
    nuevo->siguiente = NULL;

    if (cola_esta_vacia(cola)) {
        cola->frente = nuevo;
    } else {
        cola->final->siguiente = nuevo;
    }

    cola->final = nuevo;
    cola->cantidad++;
    return 1;
}

int cola_desencolar(ColaProcesos *cola, Proceso *proceso)
{
    NodoProceso *eliminado;

    if (cola_esta_vacia(cola) || proceso == NULL) {
        return 0;
    }

    eliminado = cola->frente;
    *proceso = eliminado->proceso;
    cola->frente = eliminado->siguiente;

    if (cola->frente == NULL) {
        cola->final = NULL;
    }

    cola->cantidad--;
    free(eliminado);
    return 1;
}

void cola_vaciar(ColaProcesos *cola)
{
    Proceso proceso;

    while (cola_desencolar(cola, &proceso)) {
    }
}