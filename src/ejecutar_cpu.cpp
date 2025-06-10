#include <iostream>
using namespace std;

// ==== ESTRUCTURAS DEL SISTEMA ====
// NO ELIMINAR NI MODIFICAR NADA DE ESTO
// TODOS LOS .CPP DEBEN USAR ESTAS MISMAS ESTRUCTURAS

struct Proceso {
    int id;                 // Identificador del proceso
    char nombre[30];        // Nombre del proceso
    int prioridad;          // 0: baja, 1: media, 2: alta
    int duracion;           // Tiempo de ejecución restante
    Proceso* siguiente;     // Puntero al siguiente nodo
};

struct BloqueMemoria {
    int idProceso;              // ID del proceso al que pertenece el bloque
    int tamanio;                // Tamaño de memoria asignado
    BloqueMemoria* siguiente;   // Siguiente bloque en la pila
};

struct ProcesoBloqueado {
    int id;
    char nombre[30];
    int tiempoEspera;
    ProcesoBloqueado* siguiente;
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
// Implementa la funcionalidad del menú: "4. Ejecutar proceso (CPU)"
// Esta función desencola el proceso con mayor prioridad, reduce su duración
// y lo elimina si finalizó o lo reencola si aún le falta tiempo.

void liberarMemoria(); // Prototipo si está definida en otro archivo

void ejecutarProceso() {
    if (colaCPU == NULL) {
        cout << "No hay procesos para ejecutar." << endl;
        return;
    }

    // Desencolar el primer proceso
    Proceso* proceso = colaCPU;
    colaCPU = colaCPU->siguiente;

    // Reducir duración
    proceso->duracion--;

    // Mostrar información del proceso en ejecución
    cout << "Ejecutando proceso: " << proceso->nombre << endl;
    cout << "ID: " << proceso->id << ", Duracion restante: " << proceso->duracion << endl;

    // Verificar si el proceso terminó
    if (proceso->duracion <= 0) {
        liberarMemoria(); // Simula la liberación de memoria
        delete proceso;   // Libera el nodo del proceso
        cout << "Proceso finalizado." << endl;
    } else {
        // Reencolar el proceso al final de la cola
        proceso->siguiente = NULL;

        if (colaCPU == NULL) {
            colaCPU = proceso;
        } else {
            Proceso* temp = colaCPU;
            while (temp->siguiente != NULL) {
                temp = temp->siguiente;
            }
            temp->siguiente = proceso;
        }
    }
}

