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

// ejecutar_cpu.cpp
// Implementa la funcionalidad del men�: "4. Ejecutar proceso (CPU)"
// Esta funci�n desencola el proceso con mayor prioridad, reduce su duraci�n
// y lo elimina si finaliz� o lo reencola si a�n le falta tiempo.

void ejecutarProceso() {
    if (colaCPU == NULL) {
        cout << "No hay procesos para ejecutar." << endl;
        return;
    }

    // Desencolar el proceso de mayor prioridad (inicio de la cola)
    Proceso* proceso = colaCPU;
    colaCPU = colaCPU->siguiente;

    // Reducir duraci�n
    proceso->duracion--;

    // Mostrar informaci�n del proceso en ejecuci�n
    cout << "Ejecutando proceso: " << proceso->nombre << endl;
    cout << "ID: " << proceso->id << ", Duraci�n restante: " << proceso->duracion << endl;

    // Liberar memoria si termin�, o reencolar si a�n le falta
    if (proceso->duracion <= 0) {
        liberarMemoria(); // Llama a la funci�n de liberar memoria
        delete proceso;   // Libera el nodo
        cout << "Proceso finalizado." << endl;
    } else {
        // Reencolar el proceso al final de la cola de CPU
        proceso->siguiente = NULL;

        if (colaCPU == NULL) {
            colaCPU = proceso;
        } else {
            ProcesoCPU* temp = colaCPU;
            while (temp->siguiente != NULL) {
                temp = temp->siguiente;
            }
            temp->siguiente = proceso;
        }
    }
}



