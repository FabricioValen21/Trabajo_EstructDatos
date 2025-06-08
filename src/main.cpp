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

// ==== VARIABLES GLOBALES ====
// NO ELIMINAR NI CAMBIAR NOMBRES

Proceso* listaListos = nullptr;             // Lista de procesos en estado "Listo"
Proceso* colaCPU = nullptr;                 // Cola de prioridad de CPU
BloqueMemoria* pilaMemoria = nullptr;       // Pila de memoria asignada
ProcesoBloqueado* colaBloqueados = nullptr; // Cola de procesos bloqueados

// ==== FIN DE VARIABLES GLOBALES ====


// Prototipos de funciones (declaraciÃ³n externa)
void crearProceso();
void verProcesosListo();
void cambiarEstado();
void ejecutarProceso();
void asignarMemoria();
void liberarMemoria();
void verBloqueados();

int main() {
    int opcion;
    do {
        cout << "\n===== SISTEMA DE GESTION DE PROCESOS =====\n";
        cout << "1. Crear nuevo proceso\n";
        cout << "2. Ver procesos en estado 'Listo'\n";
        cout << "3. Cambiar estado de un proceso\n";
        cout << "4. Ejecutar proceso (CPU)\n";
        cout << "5. Asignar memoria a un proceso\n";
        cout << "6. Liberar memoria\n";
        cout << "7. Ver procesos bloqueados\n";
        cout << "8. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: crearProceso(); break;
            case 2: verProcesosListo(); break;
            case 3: cambiarEstado(); break;
            case 4: ejecutarProceso(); break;
            case 5: asignarMemoria(); break;
            case 6: liberarMemoria(); break;
            case 7: verBloqueados(); break;
            case 8: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion invalida.\n"; break;
        }
    } while(opcion != 8);

    return 0;
}
