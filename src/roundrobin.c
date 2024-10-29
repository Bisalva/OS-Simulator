#include <stdio.h>
#include <stdlib.h>
#include "roundrobin.h"
#include "memoria.h"

#define MAX_INTENTOS 10

void round_robin(Process* head, int quantum, MemoryBlock* memory, int asignacion_opcion) {
    Process* actual;
    int tiempo = 0;
    int todos_listos;
    int intentos[100];
    int ciclos = 1;

    actual = head;
    int i = 0;
    while (actual != NULL) {
        intentos[i++] = 0;
        actual->state = NUEVO;
        actual = actual->next;
    }

    printf("\n=========== inicio de la simulacion round-robin ===========\n");

    do {
        todos_listos = 1;
        int cambios_realizados = 0;

        actual = head;
        i = 0;

        while (actual != NULL) {
            if (actual->arrival_time <= tiempo && actual->burst_time > 0) {
                if (actual->state == NUEVO) {
                    actual->state = LISTO;
                    printf("proceso %s cambia a estado: listo\n", actual->id);
                }
                todos_listos = 0;

                if (actual->state == LISTO || actual->state == ESPERA) {
                    actual->state = EJECUTANDO;
                    printf("\n---------- ciclo %d: procesando %s ----------\n", ciclos, actual->id);

                    // seleccion de algoritmo de asignacion de memoria
                    int asignado = 0;
                    switch (asignacion_opcion) {
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
                            printf("opcion de asignacion no valida.\n");
                            return;
                    }

                    if (asignado) {
                        if (actual->burst_time > quantum) {
                            tiempo += quantum;
                            actual->burst_time -= quantum;
                            push_stack(actual->stack, quantum);
                            actual->state = ESPERA;
                            printf("proceso %s cambia a estado: espera\n", actual->id);
                        } else {
                            tiempo += actual->burst_time;
                            printf("proceso %s finaliza en tiempo %d\n", actual->id, tiempo);
                            actual->burst_time = 0;
                            free_memory(memory, actual->memory_required);

                            if (actual->stack != NULL) {
                                printf("liberando stack del proceso %s\n", actual->id);
                                while (actual->stack->top >= 0) {
                                    pop_stack(actual->stack);
                                }
                                free_stack(actual->stack);
                                actual->stack = NULL;
                            }
                            actual->state = FINALIZADO;
                            printf("proceso %s cambia a estado: finalizado\n", actual->id);
                        }
                        cambios_realizados = 1;
                    } else {
                        actual->state = BLOQUEADO;
                        printf("estado: bloqueado (no hay memoria)\n");
                        intentos[i]++;
                        if (intentos[i] >= MAX_INTENTOS) {
                            printf("proceso %s descartado tras %d intentos.\n", actual->id, MAX_INTENTOS);
                            actual->burst_time = 0;
                            actual->state = FINALIZADO;
                            printf("proceso %s cambia a estado: finalizado\n", actual->id);
                        }
                    }
                }
            } else if (actual->state != FINALIZADO) {
                printf("proceso %s en espera (aun no llega)\n", actual->id);
            }
            actual = actual->next;
            i++;
        }

        if (cambios_realizados) {
            print_memory_state(memory);
        }

        ciclos++;

        if (!cambios_realizados && !todos_listos) {
            printf("no se pueden hacer mas cambios, deteniendo la simulacion para evitar bloqueo.\n");
            break;
        }

    } while (!todos_listos);

    printf("\n=========== fin de la simulacion round-robin ===========\n");
}
