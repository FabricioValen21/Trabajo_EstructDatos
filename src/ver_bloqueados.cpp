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
// Algunos procesos se bloquean esperando algo (como E/S).
// Este archivo muestra todos los procesos en la cola de bloqueados.

// Función para mostrar los procesos que están en la cola de bloqueados
void verBloqueados() {

    // Verifica si la cola de procesos bloqueados está vacía
    if (colaBloqueados == NULL) {
        // Si no hay procesos bloqueados, muestra un mensaje y termina la función
        cout << "No hay procesos actualmente.\n";
        return;
    }
    // Declara un puntero auxiliar para recorrer la cola, iniciando desde el primer proceso
    ProcesoBloqueado* actual = colaBloqueados;
    cout << "\n------ PROCESOS BLOQUEADOS -----\n";
    // Bucle para recorrer cada proceso en la cola
    while (actual != NULL) {
        // Muestra el ID del proceso actual
        cout << "ID: " << actual->id << endl;
        // Muestra el nombre del proceso actual
        cout << "Nombre: " << actual->nombre << endl;
        // Muestra el tiempo de espera en ciclos del proceso actual
        cout << "Tiempo de espera(E/S): " << actual->tiempoEspera << " ciclos\n";
        cout << "----------------------------\n";
        // Avanza al siguiente nodo de la cola
        actual = actual->siguiente;
    }
}

