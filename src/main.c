#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cola.h"

#define MAX_LINEA 256

typedef struct {
    int procesadores;
    int hilos;
} ConfiguracionSistema;

static void recortar_espacios(char *texto)
{
    char *inicio = texto;
    char *final;

    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }

    if (inicio != texto) {
        memmove(texto, inicio, strlen(inicio) + 1);
    }

    final = texto + strlen(texto);
    while (final > texto && isspace((unsigned char)final[-1])) {
        final--;
    }
    *final = '\0';
}

static int leer_configuracion(FILE *archivo, ConfiguracionSistema *configuracion)
{
    char linea[MAX_LINEA];
    int encontrada_procesadores = 0;
    int encontrada_hilos = 0;

    while (1) {
        long posicion_linea = ftell(archivo);

        if (fgets(linea, sizeof(linea), archivo) == NULL) {
            break;
        }

        recortar_espacios(linea);

        if (linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        if (sscanf(linea, "Procesadores %d", &configuracion->procesadores) == 1) {
            encontrada_procesadores = 1;
        } else if (sscanf(linea, "Hilos %d", &configuracion->hilos) == 1) {
            encontrada_hilos = 1;
        } else {
            if (fseek(archivo, posicion_linea, SEEK_SET) != 0) {
                fprintf(stderr, "Error: no se pudo reposicionar el archivo de configuracion.\n");
                return 0;
            }
            break;
        }
    }

    if (!encontrada_procesadores || !encontrada_hilos) {
        fprintf(stderr, "Error: faltan Procesadores o Hilos en la configuracion.\n");
        return 0;
    }

    if (configuracion->procesadores <= 0 || configuracion->hilos <= 0) {
        fprintf(stderr, "Error: Procesadores y Hilos deben ser mayores que cero.\n");
        return 0;
    }

    return 1;
}

static int cargar_procesos(FILE *archivo,
                           ColaProcesos *cola,
                           const ConfiguracionSistema *configuracion)
{
    char linea[MAX_LINEA];
    int numero_linea = 2;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Proceso proceso;
        char extra;
        int campos;

        numero_linea++;
        recortar_espacios(linea);

        if (linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        campos = sscanf(linea,
                        " %d | %d | %63[^|] | %d | %d | %d | %d | %d %c",
                        &proceso.id,
                        &proceso.id_padre,
                        proceso.contador_programa,
                        &proceso.registros,
                        &proceso.tamano_bytes,
                        &proceso.hilos,
                        &proceso.quantum,
                        &proceso.iteraciones,
                        &extra);

        if (campos != 8) {
            fprintf(stderr, "Error: formato invalido en la linea %d.\n", numero_linea);
            return 0;
        }

        recortar_espacios(proceso.contador_programa);

        if (proceso.id <= 0 || proceso.id_padre < 0 ||
            proceso.contador_programa[0] == '\0' ||
            proceso.registros < 0 || proceso.tamano_bytes <= 0 ||
            proceso.hilos <= 0 || proceso.hilos > configuracion->hilos ||
            proceso.quantum <= 0 || proceso.iteraciones <= 0) {
            fprintf(stderr, "Error: valores invalidos en la linea %d.\n", numero_linea);
            return 0;
        }

        if (!cola_encolar(cola, proceso)) {
            fprintf(stderr, "Error: no se pudo reservar memoria para la cola.\n");
            return 0;
        }
    }

    return 1;
}

static void mostrar_cola(const ColaProcesos *cola)
{
    const NodoProceso *actual = cola->frente;

    printf("Cola: ");
    if (actual == NULL) {
        printf("(vacia)");
    }

    while (actual != NULL) {
        printf("P%d", actual->proceso.id);
        actual = actual->siguiente;
        if (actual != NULL) {
            printf(" -> ");
        }
    }
    printf("\n");
}

static int seleccionar_lote(const ConfiguracionSistema *configuracion,
                            ColaProcesos *cola,
                            Proceso *lote,
                            size_t capacidad,
                            size_t *cantidad_lote,
                            int *hilos_usados)
{
    size_t pendientes = cola->cantidad;
    size_t procesados = 0;
    size_t seleccionados = 0;
    int hilos_disponibles = configuracion->hilos;

    while (procesados < pendientes) {
        Proceso candidato;
        int puede_ejecutarse;

        if (!cola_desencolar(cola, &candidato)) {
            fprintf(stderr, "Error interno: no se pudo retirar un proceso.\n");
            return 0;
        }

        puede_ejecutarse = seleccionados < capacidad &&
                           candidato.hilos <= hilos_disponibles;
        if (puede_ejecutarse) {
            lote[seleccionados] = candidato;
            seleccionados++;
            hilos_disponibles -= candidato.hilos;
        } else if (!cola_encolar(cola, candidato)) {
            fprintf(stderr, "Error: no se pudo conservar un proceso en la cola.\n");
            return 0;
        }

        procesados++;
    }

    *cantidad_lote = seleccionados;
    *hilos_usados = configuracion->hilos - hilos_disponibles;
    return seleccionados > 0;
}

static int simular(const ConfiguracionSistema *configuracion, ColaProcesos *cola)
{
    long long tiempo_cpu = 0;
    int ciclo = 0;

    while (!cola_esta_vacia(cola)) {
        Proceso *lote;
        size_t cantidad_lote;
        size_t indice;
        int hilos_usados;

        lote = malloc((size_t)configuracion->procesadores * sizeof(*lote));
        if (lote == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para un ciclo.\n");
            return 0;
        }

        ciclo++;
        printf("\n--- Ciclo %d ---\n", ciclo);
        mostrar_cola(cola);

        if (!seleccionar_lote(configuracion,
                              cola,
                              lote,
                              (size_t)configuracion->procesadores,
                              &cantidad_lote,
                              &hilos_usados)) {
            free(lote);
            fprintf(stderr, "Error: no se pudo formar un lote ejecutable.\n");
            return 0;
        }
        printf("Procesos activos: %lu, hilos utilizados: %d/%d\n",
               (unsigned long)cantidad_lote,
               hilos_usados,
               configuracion->hilos);

        for (indice = 0; indice < cantidad_lote; indice++) {
            Proceso *proceso = &lote[indice];

            printf("Procesador %lu ejecuta PID=%d (%s), hilos=%d, quantum=%d, "
                   "iteraciones restantes=%d\n",
                   (unsigned long)(indice + 1),
                   proceso->id,
                   proceso->contador_programa,
                   proceso->hilos,
                   proceso->quantum,
                   proceso->iteraciones);

            tiempo_cpu += proceso->quantum;
            proceso->iteraciones--;

            if (proceso->iteraciones > 0) {
                if (!cola_encolar(cola, *proceso)) {
                    free(lote);
                    fprintf(stderr, "Error: no se pudo reencolar el proceso.\n");
                    return 0;
                }
                printf("PID=%d termina su quantum y vuelve a la cola "
                       "(iteraciones restantes=%d).\n",
                       proceso->id,
                       proceso->iteraciones);
            } else {
                printf("PID=%d finaliza.\n", proceso->id);
            }
        }

        free(lote);
        printf("Tiempo de CPU simulado acumulado: %lld pulsos\n", tiempo_cpu);
        mostrar_cola(cola);
    }

    printf("\nSimulacion finalizada: no hay procesos pendientes.\n");
    printf("Tiempo total de CPU simulado: %lld pulsos\n", tiempo_cpu);
    return 1;
}

int main(int argc, char *argv[])
{
    const char *nombre_archivo = argc == 2 ? argv[1] : "config.txt";
    ConfiguracionSistema configuracion;
    ColaProcesos cola;
    FILE *archivo;

    if (argc > 2) {
        fprintf(stderr, "Uso: %s [archivo_configuracion]\n", argv[0]);
        return EXIT_FAILURE;
    }

    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo '%s'.\n", nombre_archivo);
        return EXIT_FAILURE;
    }

    cola_inicializar(&cola);

    if (!leer_configuracion(archivo, &configuracion) ||
        !cargar_procesos(archivo, &cola, &configuracion)) {
        fclose(archivo);
        cola_vaciar(&cola);
        return EXIT_FAILURE;
    }

    fclose(archivo);

    printf("Configuracion cargada: %d procesador(es), %d hilo(s).\n",
           configuracion.procesadores,
           configuracion.hilos);
    printf("Procesos cargados: %lu\n", (unsigned long)cola.cantidad);

    if (!simular(&configuracion, &cola)) {
        cola_vaciar(&cola);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}