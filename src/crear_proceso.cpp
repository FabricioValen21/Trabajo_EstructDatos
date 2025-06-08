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
// Una empresa quiere registrar procesos con ID, nombre, prioridad y duraciÃ³n.
// Este archivo se encarga de agregar esos procesos a una lista de espera (estado "Listo").

void crearProceso() {
    // Se crea un nuevo nodo de tipo Proceso dinamicamente
    
    Proceso* nuevo = new Proceso();
    
    // Pedimos al usuario el ID del proceso:
    cout << "Ingrese el ID del proceso: ";
    cin >> nuevo->id;
    
    // Pedimos el nombre del proceso
    cout << "Ingrese el nombre del proceso: ";
    cin.ignore(); // Limpia el buffer por si quedo un salto de linea anterior
	cin.getline(nuevo->nombre, 30); 
	
	//Algo
}

























