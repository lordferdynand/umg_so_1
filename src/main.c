#include <stdio.h>

#include "cola.h"

int main(void)
{
    ColaProcesos cola;
    Proceso proceso;

    cola_inicializar(&cola);

    proceso.id = 1;
    proceso.id_padre = 0;
    snprintf(proceso.contador_programa,
             sizeof(proceso.contador_programa),
             "Proceso1");
    proceso.registros = 5;
    proceso.tamano_bytes = 1024;
    proceso.hilos = 2;
    proceso.quantum = 10;
    proceso.iteraciones = 3;

    if (!cola_encolar(&cola, proceso)) {
        fprintf(stderr, "Error: no se pudo reservar memoria para la cola.\n");
        return 1;
    }

    if (cola_desencolar(&cola, &proceso)) {
        printf("Proceso retirado de la cola: PID=%d, nombre=%s\n",
               proceso.id,
               proceso.contador_programa);
    }

    cola_vaciar(&cola);
    return 0;
}