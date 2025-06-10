#include <iostream>
using namespace std;

// ==== ESTRUCTURAS DEL SISTEMA ====
// NO ELIMINAR NI MODIFICAR NADA DE ESTO
// TODOS LOS .CPP DEBEN USAR ESTAS MISMAS ESTRUCTURAS

struct Proceso {
    int id;                 // Identificador del proceso
    char nombre[30];        // Nombre del proceso
    int prioridad;          // 0: baja, 1: media, 2: alta
    int duracion;           // Tiempo de ejecuci�n restante
    Proceso* siguiente;     // Puntero al siguiente nodo
};

struct BloqueMemoria {
    int idProceso;              // ID del proceso al que pertenece el bloque
    int tamanio;                // Tama�o de memoria asignado
    BloqueMemoria* siguiente;   // Siguiente bloque en la pila
};

struct ProcesoBloqueado {
    int id;                     // ID del proceso bloqueado
    char nombre[30];            // Nombre del proceso
    int tiempoEspera;           // Tiempo estimado de espera por E/S
    ProcesoBloqueado* siguiente;// Siguiente en la cola de bloqueados
};

// ==== FIN DE ESTRUCTURAS DEL SISTEMA ====

// ==== VARIABLES GLOBALES COMPARTIDAS ====
// NO ELIMINAR NI CAMBIAR NOMBRES NI TIPOS

extern Proceso* listaListos;
extern Proceso* colaCPU;
extern BloqueMemoria* pilaMemoria;
extern ProcesoBloqueado* colaBloqueados;

// ==== FIN DE VARIABLES ====

// CASO:
// Cuando se crea un proceso, necesita memoria.
// Este archivo agrega un bloque de memoria a la pila simulada (push).

// asignar_memoria.cpp
// Implementa la funcionalidad del men�: "5. Asignar memoria a un proceso"
// Esta funci�n simula la asignaci�n de memoria usando una pila (LIFO), agregando un bloque con ID de proceso y tama�o.

void asignarMemoria() {
    // Crear nuevo nodo de tipo BloqueMemoria
    BloqueMemoria* nuevo = new BloqueMemoria();

    // Pedir al usuario el ID del proceso
    cout << "Ingrese el ID del proceso que usara el bloque de memoria: ";
    cin >> nuevo->idProceso;

    // Pedir el tama�o del bloque de memoria
    cout << "Ingrese el tama�o del bloque de memoria: ";
    cin >> nuevo->tamanio;

    // Insertar el nuevo bloque al inicio de la pila
    nuevo->siguiente = pilaMemoria; // Aqu� el nuevo bloque apunta a lo que estaba en la cima
    pilaMemoria = nuevo;            // Ahora el nuevo bloque ES la nueva cima de la pila

    cout << "Memoria asignada correctamente al proceso.\n";
}
