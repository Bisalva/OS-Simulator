#include <stdio.h>
#include <stdlib.h>
#include "memoria.h"

// Funcion para crear bloques de memoria
MemoryBlock* create_memory(int total_memory, int block_size) {
    MemoryBlock* head = NULL;
    MemoryBlock* actual = NULL;

    for (int i = 0; i < total_memory; i += block_size) {
        MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        new_block->size = block_size;
        new_block->free = 1;  // bloques inicialmente libres
        new_block->next = NULL;

        if (head == NULL) {
            head = new_block;
            actual = head;
        } else {
            actual->next = new_block;
            actual = actual->next;
        }
    }

    return head;  // Devolver la lista de bloques de memoria
}

// Implementacion de First Fit
int first_fit(MemoryBlock* head, int memory_required) {
    MemoryBlock* current_block = head;
    MemoryBlock* start_block = NULL;
    int accumulated_memory = 0;

    while (current_block != NULL) {
        if (current_block->free) {
            if (start_block == NULL) {
                start_block = current_block;  // Primer bloque libre
            }
            accumulated_memory += current_block->size;

            // Si hemos acumulado suficiente memoria, asignamos los bloques
            if (accumulated_memory >= memory_required) {
                current_block = start_block;  // Volvemos al bloque inicial
                accumulated_memory = 0;

                while (accumulated_memory < memory_required) {
                    current_block->free = 0;  // Asignamos el bloque
                    accumulated_memory += current_block->size;
                    current_block = current_block->next;
                }

                printf("Memoria asignada: %d KB\n", memory_required);
                return 1;  // Asignacion exitosa
            }
        } else {
            // Si encontramos un bloque ocupado, reiniciamos
            start_block = NULL;
            accumulated_memory = 0;
        }

        current_block = current_block->next;
    }

    printf("No hay bloques disponibles para %d KB\n", memory_required);
    return 0;  // No se pudo asignar memoria
}

// Implementacion de Best Fit
int best_fit(MemoryBlock* head, int memory_required) {
    MemoryBlock* best_block_start = NULL;
    MemoryBlock* current_block = head;
    MemoryBlock* start_block = NULL;
    int accumulated_memory = 0;
    int found = 0;

    while (current_block != NULL) {
        if (current_block->free) {
            accumulated_memory += current_block->size;
            if (start_block == NULL) {
                start_block = current_block;
            }
        } else {
            accumulated_memory = 0;
            start_block = NULL;
        }

        if (accumulated_memory >= memory_required) {
            best_block_start = start_block;
            found = 1;
            break;
        }

        current_block = current_block->next;
    }

    if (found && best_block_start != NULL) {
        current_block = best_block_start;
        accumulated_memory = 0;

        while (accumulated_memory < memory_required) {
            current_block->free = 0;
            accumulated_memory += current_block->size;
            current_block = current_block->next;
        }

        printf("Memoria asignada: %d KB\n", memory_required);
        return 1;  // Asignacion exitosa
    }

    printf("No hay bloques de memoria disponibles para %d KB\n", memory_required);
    return 0;  // No se pudo asignar memoria
}

// Implementacion de Worst Fit
int worst_fit(MemoryBlock* head, int memory_required) {
    MemoryBlock* current_block = head;
    MemoryBlock* worst_block_start = NULL;
    MemoryBlock* start_block = NULL;
    int worst_block_size = 0;
    int accumulated_memory = 0;

    // Recorrer todos los bloques de memoria para encontrar el bloque libre mas grande
    while (current_block != NULL) {
        if (current_block->free) {
            if (start_block == NULL) {
                start_block = current_block;  // inicio de un bloque libre
            }
            accumulated_memory += current_block->size;

            // Si encontramos un bloque mas grande que el actual, actualizamos
            if (accumulated_memory >= memory_required && accumulated_memory > worst_block_size) {
                worst_block_start = start_block;
                worst_block_size = accumulated_memory;
            }
        } else {
            // Reiniciar si encontramos un bloque ocupado
            start_block = NULL;
            accumulated_memory = 0;
        }
        current_block = current_block->next;
    }

    // Si encontramos el peor bloque (mas grande) para la asignacion
    if (worst_block_start != NULL && worst_block_size >= memory_required) {
        accumulated_memory = 0;
        current_block = worst_block_start;

        // Asignar la memoria en el peor bloque encontrado
        while (accumulated_memory < memory_required) {
            current_block->free = 0;
            accumulated_memory += current_block->size;
            current_block = current_block->next;
        }

        printf("memoria asignada: %d KB\n", memory_required);
        return 1;  // asignacion exitosa
    }

    // No se encontro un bloque lo suficientemente grande
    printf("no hay bloques disponibles para %d KB\n", memory_required);
    return 0;  // no se pudo asignar memoria
}

// funcion para liberar memoria
void free_memory(MemoryBlock* head, int memory_required) {
    MemoryBlock* actual = head;
    int memoria_liberada = 0;

    printf("liberando %d KB de memoria...\n", memory_required);

    // Recorrer los bloques de memoria y liberar hasta alcanzar el total requerido
    while (actual != NULL && memoria_liberada < memory_required) {
        if (!actual->free) {  // si el bloque esta ocupado
            actual->free = 1;  // liberar el bloque
            memoria_liberada += actual->size;
            printf("bloque de %d KB liberado\n", actual->size);
        }
        actual = actual->next;
    }

    // Verificar si se libero la cantidad total de memoria requerida
    if (memoria_liberada >= memory_required) {
        printf("memoria total de %d KB liberada correctamente\n", memory_required);
    } else {
        printf("error: no se pudo liberar la cantidad requerida de %d KB\n", memory_required);
    }
}

// funcion para imprimir el estado de la memoria
void print_memory_state(MemoryBlock* head) {
    MemoryBlock* actual = head;
    int bloques_libres = 0;
    int bloques_ocupados = 0;

    printf("\n[Estado de la memoria]\n");
    while (actual != NULL) {
        if (actual->free) {
            bloques_libres++;
        } else {
            printf("Bloque de %d KB - Ocupado\n", actual->size);
            bloques_ocupados++;
        }
        actual = actual->next;
    }

    if (bloques_libres > 0) {
        printf("Bloques libres: %d\n", bloques_libres);
    }

    if (bloques_ocupados == 0) {
        printf("Todos los bloques estan libres\n");
    }
}
