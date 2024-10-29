#ifndef DATOS_H
#define DATOS_H

// estructura para manejar la pila del proceso
typedef struct Stack {
    int size;
    int *data;
    int top;
} Stack;

// estados posibles del proceso
typedef enum {
    NUEVO,
    LISTO,
    EJECUTANDO,
    ESPERA,
    BLOQUEADO,
    FINALIZADO
} ProcessState;

// estructura para los procesos
typedef struct Process {
    char id[3];
    int arrival_time;
    int burst_time;
    int memory_required;
    ProcessState state;  // estado actual del proceso
    Stack* stack;  // pila asociada al proceso
    struct Process* next;  // puntero al siguiente proceso
} Process;

// estructura para la configuracion
typedef struct Config {
    int total_memory;
    int block_size;
    char algorithm[20];
    int cpu_cores;
    char scheduling_algorithm[20];
    int time_quantum;
} Config;

// funciones para manejar la pila
Stack* create_stack(int size);
void push_stack(Stack* stack, int value);
int pop_stack(Stack* stack);
void free_stack(Stack* stack);

// funciones relacionadas a procesos y configuracion
int read_config(const char *filename, Config *config);
Process* getProcess(const char* filename);
void outputParameters(Config *config);
void free_process_list(Process* head);
Process* create_process(char* id, int arrival_time, int burst_time, int memory_required);
void add_process(Process** head, Process* new_process);
void print_processes(Process* head);

#endif
