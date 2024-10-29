#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "datos.h"

// leer y obtener los procesos desde un archivo
Process* getProcess(const char* filename) {
    FILE* file = fopen(filename, "r");
    Process* lista_procesos = NULL; 
    char linea[100]; 

    if (file == NULL) {
        perror("error al abrir el archivo");
        return NULL;
    }

    // leer cada linea del archivo
    while (fgets(linea, sizeof(linea), file)) {
        if (linea[0] == '#' || linea[0] == '\n') continue; // ignorar comentarios o lineas vacias

        char id[3];
        int llegada, ejecucion, memoria;
        // leer datos del proceso
        if (sscanf(linea, "%2[^,], %d, %d, %d", id, &llegada, &ejecucion, &memoria) == 4) {
            Process* nuevo_proceso = create_process(id, llegada, ejecucion, memoria);
            add_process(&lista_procesos, nuevo_proceso);  // agregar proceso a la lista
        }
    }

    fclose(file);

    return lista_procesos;  // devolver la lista de procesos
}

// leer la configuracion desde un archivo
int read_config(const char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error al abrir el archivo");
        return 0;  // retorna 0 si hay un error
    }

    char linea[256]; 
    while (fgets(linea, sizeof(linea), file)) {
        if (linea[0] == '#' || linea[0] == '\n') continue; 

        int valor;
        // leer los parametros de la configuracion
        if (sscanf(linea, "Total_Memory, %d", &valor) == 1) {
            config->total_memory = valor;
        } else if (sscanf(linea, "Block_Size, %d", &valor) == 1) {
            config->block_size = valor;
        } else if (sscanf(linea, "Algorithm, %s", config->algorithm) == 1) {
        } else if (sscanf(linea, "CPU_Cores, %d", &valor) == 1) {
            config->cpu_cores = valor;
        } else if (sscanf(linea, "Scheduling_Algorithm, %s", config->scheduling_algorithm) == 1) {
        } else if (sscanf(linea, "Time_Quantum, %d", &valor) == 1) {
            config->time_quantum = valor;
        }
    }

    fclose(file);
    return 1;  
}

// crear la pila del proceso
Stack* create_stack(int size) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->size = size;
    stack->data = (int*)malloc(size * sizeof(int));
    stack->top = -1;  // inicialmente la pila esta vacia
    return stack;
}

// agregar un valor a la pila (simular uso de stack)
void push_stack(Stack* stack, int value) {
    if (stack->top < stack->size - 1) {
        stack->top++;
        stack->data[stack->top] = value;
        printf("valor %d añadido al stack del proceso.\n", value);
    } else {
        printf("error: stack del proceso esta lleno.\n");
    }
}

// sacar un valor de la pila (simular liberacion de stack)
int pop_stack(Stack* stack) {
    if (stack->top >= 0) {
        int value = stack->data[stack->top];
        stack->top--;
        printf("valor %d sacado del stack del proceso.\n", value);
        return value;
    } else {
        printf("error: stack del proceso esta vacio.\n");
        return -1;  // codigo de error
    }
}

// liberar la memoria del stack
void free_stack(Stack* stack) {
    if (stack != NULL) {
        free(stack->data);
        free(stack);
    }
}

// crear un nuevo proceso
Process* create_process(char* id, int llegada, int ejecucion, int memoria) {
    Process* nuevo_proceso = (Process*)malloc(sizeof(Process));
    strcpy(nuevo_proceso->id, id);
    nuevo_proceso->arrival_time = llegada;
    nuevo_proceso->burst_time = ejecucion;
    nuevo_proceso->memory_required = memoria;
    nuevo_proceso->stack = create_stack(10);  // crear pila de tamaño 10 para cada proceso
    nuevo_proceso->state = NUEVO;
    nuevo_proceso->next = NULL;
    return nuevo_proceso;
}

// agregar un proceso a la lista
void add_process(Process** head, Process* nuevo_proceso) {
    if (*head == NULL) {
        *head = nuevo_proceso;
    } else {
        Process* actual = *head;
        while (actual->next != NULL) {
            actual = actual->next;
        }
        actual->next = nuevo_proceso;
    }
    nuevo_proceso->next = NULL;
}

// imprimir la lista de procesos
void print_processes(Process* head) {
    printf("\n[lista de procesos]\n");
    while (head != NULL) {
        printf("- id: %s, llegada: %d, ejecucion: %d, memoria: %d\n",
               head->id, head->arrival_time, head->burst_time, head->memory_required);
        head = head->next;
    }
    printf("\n");
}

// imprimir los parametros del sistema
void outputParameters(Config *p) {
    printf("\n[parametros]\n");
    printf("- total de memoria: %d kb\n", p->total_memory);
    printf("- tamano de bloque: %d kb\n", p->block_size);
    printf("- algoritmo: %s\n", p->algorithm);
    printf("- nucleos de cpu: %d\n", p->cpu_cores);
    printf("- algoritmo de planificacion: %s\n", p->scheduling_algorithm);
    printf("- quantum de tiempo: %d\n", p->time_quantum);
}

// liberar la memoria de la lista de procesos
void free_process_list(Process* head) {
    Process* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        
        // verificar si el proceso tiene una pila antes de liberarla
        if (temp->stack != NULL) {
            free_stack(temp->stack);
            temp->stack = NULL;  // asegurarse de que no se intente liberar nuevamente
        }
        
        free(temp);  // liberar el proceso
        printf("proceso liberado correctamente\n");
    }
}
