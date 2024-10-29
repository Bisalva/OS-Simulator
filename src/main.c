#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memoria.h"
#include "roundrobin.h"
#include "fifo.h"
#include "datos.h"

int main() {
    Config config;
    Process* process_list = NULL;
    MemoryBlock* memory = NULL;
    int planificacion_opcion;
    int asignacion_opcion;

    // leer la configuracion del archivo
    if (!read_config("entrada-summ.txt", &config)) {
        printf("error al leer el archivo de configuracion.\n");
        return 1;
    }
    outputParameters(&config);

    // leer la lista de procesos
    process_list = getProcess("entrada-summ.txt");
    if (process_list == NULL) {
        printf("no se pudo cargar la lista de procesos.\n");
        return 1;
    }

    print_processes(process_list);

    // crear la memoria
    memory = create_memory(config.total_memory, config.block_size);

    // seleccionar algoritmo de asignacion de memoria
    do {
        printf("seleccione el algoritmo de asignacion de memoria:\n");
        printf("1. first fit\n");
        printf("2. best fit\n");
        printf("3. worst fit\n");
        printf("ingrese su opcion: ");
        if (scanf("%d", &asignacion_opcion) != 1 || (asignacion_opcion < 1 || asignacion_opcion > 3)) {
            printf("entrada no valida. por favor, ingrese una opcion valida (1, 2 o 3).\n");
            while (getchar() != '\n');
        }
    } while (asignacion_opcion < 1 || asignacion_opcion > 3);

    // seleccionar algoritmo de planificacion
    do {
        printf("seleccione el algoritmo de planificacion:\n");
        printf("1. fifo\n");
        printf("2. round robin\n");
        printf("ingrese su opcion: ");
        if (scanf("%d", &planificacion_opcion) != 1 || (planificacion_opcion < 1 || planificacion_opcion > 2)) {
            printf("entrada no valida. por favor, ingrese una opcion valida (1 o 2).\n");
            while (getchar() != '\n');
        }
    } while (planificacion_opcion < 1 || planificacion_opcion > 2);

    // ejecutar planificador
    switch (planificacion_opcion) {
        case 1:
            printf("\niniciando planificador fifo...\n");
            fifo(process_list, memory, asignacion_opcion);
            break;
        case 2:
            printf("\niniciando planificador round robin...\n");
            round_robin(process_list, config.time_quantum, memory, asignacion_opcion);
            break;
        default:
            printf("opcion no valida.\n");
            return 1;
    }

    // liberar la lista de procesos
    free_process_list(process_list);

    return 0;
}
