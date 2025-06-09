#include <iostream>
#include <cstring>
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
// El jefe quiere revisar todos los procesos en espera.
// Este archivo muestra todos los procesos en la lista "Listo".

void verProcesosListo() {
    // Verificar si la lista está vacía
    if (listaListos == nullptr) {
        cout << "\nNo hay procesos en estado 'Listo'.\n";
        return;
    }
    
    // Mostrar encabezado de la tabla
    cout << "\n============ PROCESOS EN ESTADO 'LISTO' ============\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------------\n";
    
    // Puntero temporal para recorrer la lista sin modificarla
    Proceso* temp = listaListos;
    int totalProcesos = 0;
    
    // Recorrer la lista enlazada de procesos
    while (temp != nullptr) {
        // Mostrar ID del proceso
        cout << temp->id << "\t";
        
        // Mostrar nombre con formato
        cout << temp->nombre;
        
        // Agregar espacios para alinear
        int len = strlen(temp->nombre);
        if (len < 8) {
            cout << "\t\t\t";
        } else if (len < 16) {
            cout << "\t\t";
        } else {
            cout << "\t";
        }
        
        // Mostrar prioridad como texto
        switch(temp->prioridad) {
            case 0:
                cout << "Baja";
                break;
            case 1:
                cout << "Media";
                break;
            case 2:
                cout << "Alta";
                break;
            default:
                cout << "Desconocida";
        }
        cout << "\t\t";
        
        // Mostrar duración restante
        cout << temp->duracion << " seg" << endl;
        
        // Incrementar contador
        totalProcesos++;
        
        // Avanzar al siguiente proceso
        temp = temp->siguiente;
    }
    
    // Mostrar resumen final
    cout << "----------------------------------------------------\n";
    cout << "Total de procesos en estado 'Listo': " << totalProcesos << endl;
    cout << "====================================================\n";
}