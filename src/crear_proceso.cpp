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

// crear_proceso.cpp
// Implementa la funcionalidad del menu: "1. Crear nuevo proceso"
// Esta funcion permite registrar un nuevo proceso y agregarlo a la lista de procesos en estado "Listo"

void crearProceso() {
    // Se crea un nuevo nodo de tipo Proceso dinamicamente
    Proceso* nuevo = new Proceso();
    
    // Pedimos al usuario el ID del proceso
    cout << "Ingrese el ID del proceso: ";
    cin >> nuevo->id;
    
    // Pedimos el nombre del proceso
    cout << "Ingrese el nombre del proceso: ";
    cin.ignore(); // Limpia el buffer por si quedo un salto de linea anterior
    cin.getline(nuevo->nombre, 30);  // Permite ingresar el nombre con espacios
                                     // "nuevo" accede a struct especificamente a nombre
    
    // Pide el nivel de prioridad del proceso
    cout << "Ingrese la prioridad (0 = baja, 1 = media, 2 = alta): ";
    cin >> nuevo->prioridad;
    
    // Pedir la duracion (tiempo estimado de ejecucion)
    cout << "Ingrese la duracion del proceso: ";
    cin >> nuevo->duracion;
    
    // Inicializa el puntero siguiente como NULL, porque todavía no se enlaza
    nuevo->siguiente = NULL;
    
    // Inserta el nuevo proceso al final de la lista "listaListos"
    if (listaListos == NULL) {
        // Si la lista está vacía, el nuevo proceso se convierte en el primero
        listaListos = nuevo;
    } else {
        // Si ya hay procesos, recorremos hasta el final de la lista
        Proceso* actual = listaListos;
        
        // Se recorre la lista hasta llegar al último nodo
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        
        // Se enlaza el nuevo proceso al final de la lista
        actual->siguiente = nuevo;
    }
}













