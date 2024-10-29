#ifndef MEMORIA_H
#define MEMORIA_H

typedef struct MemoryBlock {
    int size;
    int free;  // 1 si esta libre, 0 si esta ocupado
    struct MemoryBlock* next;
} MemoryBlock;

MemoryBlock* create_memory(int total_memory, int block_size);
int first_fit(MemoryBlock* head, int memory_required);
int best_fit(MemoryBlock* head, int memory_required);
int worst_fit(MemoryBlock* head, int memory_required);
void free_memory(MemoryBlock* head, int size);
void print_memory_state(MemoryBlock* head);

#endif
