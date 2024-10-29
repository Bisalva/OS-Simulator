#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memoria.h"
#include "fifo.h"

void fifo(Process* head, MemoryBlock* memory, int asigacion_opcion) {
    Process* actual = head;
    int tiempo = 0; // contador de tiempo
    int ciclos = 1; // contador de los ciclos (en este caso deben ser ciclos == procesos)
    int asignado; // opción para el switch (enfoque a la memoria)

    // Inicializa todos los procesos ingresados como "NUEVO"
    while (actual != NULL) { 
        actual->state = NUEVO;
        actual = actual->next;
    }

    // Reinicia la variable actual para volver al primer proceso
    actual = head;

    printf("\n=========== Inicio de la simulación FIFO ===========\n");

    while (actual != NULL) { // Mientras aún existan procesos
        if (actual->state == NUEVO) { // Entra un proceso
            printf("Proceso %s cambia a estado: listo\n", actual->id);
            actual->state = LISTO;
        }

        if (actual->state == LISTO) { // Ejecuta el proceso
            printf("\n---------- Ciclo %d: procesando %s ----------\n", ciclos, actual->id);
            actual->state = EJECUTANDO;
            printf("Estado: Ejecutando\n");

            // Asignación de memoria según los casos
            switch (asigacion_opcion) { 
                case 1:
                    asignado = first_fit(memory, actual->memory_required);
                    break;
                case 2:
                    asignado = best_fit(memory, actual->memory_required);
                    break;
                case 3:
                    asignado = worst_fit(memory, actual->memory_required);
                    break;
                default:
                    printf("Opción no válida\n");
                    asignado = 0;
                    break;
            }

            if (asignado) { // Si se le asignó un método de gestión de memoria
                if (actual->state == EJECUTANDO) { // Ejecuta el proceso
                    printf("Proceso %s ejecutando...\n", actual->id);
                    sleep(actual->burst_time); // Simula el tiempo que se demora en realizar el proceso
                    tiempo += actual->burst_time;
                    actual->state = FINALIZADO;
                    free_memory(memory, actual->memory_required); // Liberación de memoria
                }
            }
        }

        printf("Proceso %s termina en tiempo %d\n", actual->id, tiempo);

        actual = actual->next; // Pasa al siguiente proceso en la lista
        ciclos++;
    }

    printf("Se han ejecutado todos los procesos con un tiempo total de: %d segundos\n", tiempo);
    printf("\n=========== Fin de la simulación FIFO ===========\n");
}
