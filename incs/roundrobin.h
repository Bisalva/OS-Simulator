#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "datos.h"
#include "memoria.h"  // incluir la gestion de memoria

// Ahora acepta un cuarto parametro para la opcion de asignacion de memoria
void round_robin(Process* head, int quantum, MemoryBlock* memory, int asignacion_opcion);

#endif
