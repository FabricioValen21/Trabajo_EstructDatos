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

// cambiar_estado.cpp
// Implementa la funcionalidad del menú: "3. Cambiar estado de un proceso"
// Esta función permite buscar un proceso por su ID, eliminarlo de la lista actual e insertarlo en la estructura correspondiente (CPU o bloqueado)

void cambiarEstado() {
    // Pedimos el ID del proceso que se desea mover
    int idBuscar;
    cout << "Ingrese el ID del proceso que desee mover: ";
    cin >> idBuscar;
    
    // Se busca el proceso en la lista "listaListos"
    Proceso* actual = listaListos;
    Proceso* anterior = NULL;
    
    // Se recorre la lista para encontrar el proceso del ID ingresado
    while (actual != NULL && actual->id != idBuscar) {
    	anterior = actual;
    	actual = actual->siguiente;
	}
	
	// Verificamos que el proceso no haya sido encontrado
	if (actual = NULL) {
		cout << "Proceso no encontrado.\n";
		return;
	}
	
	// Quita el node de la lista
	if (anterior == NULL) {
		// Si el nodo esta al inicio de la lista
		listaListos = actual->siguiente;
	} else {
		// Si el nodo esta en el medio o al final
		anterior->siguiente = actual->siguiente;
	}
	
	// Ahora el nodo actual esta desconectado de su lista
	actual->siguiente = NULL;
	
	// Se pregunta al usuario que estado desea mover el proceso
	int opcion;
	cout << "Seleccioneel nuevo estado del proceso:\n";
	cout << "1. Mover a el CPU (cola de prioridad)\n";
	cout << "2. Mover a bloqueados (espera por E/S)\n";
	cout << "Opcion: ";
	cin >> opcion;
	
	if (opcion == 1) {
		// Inserta el proceso en la cola de CPU (ordenando por prioridad)

		if(colaCPU == NULL || actual->prioridad > colaCPU->prioridad) {
			// Si la cola esta vacia o el proceso tiene mayor prioridad, va al inicio
			actual->siguiente = colaCPU;
			cola CPU = actual;		
		} else {
			// Buscar la posicion correcta segun prioridad
			Proceso* temp = colaCPU;
			while (temp->siguiente != NULL && temp->siguiente->prioridad >= actual->prioridad) {
				temp = temp->siguiente;
			}
			
			// Insertar el proceso en su posicion
			actual->siguiente = temp->siguiente;
			temp->siguiente = actual;
		}
	
		cout << "Proceso fue movido a la CPU.\n";
	} else if (opcion == 2) {
		
	}
	
}
