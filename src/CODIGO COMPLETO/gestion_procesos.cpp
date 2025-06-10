#include <iostream>
using namespace std;

// ==== ESTRUCTURAS DEL SISTEMA ====
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

// ==== VARIABLES GLOBALES ====
Proceso* listaListos = NULL;             // Lista de procesos en estado "Listo"
Proceso* colaCPU = NULL;                 // Cola de prioridad de CPU
Proceso* pilaSuspendidos = NULL;         // Pila de procesos suspendidos (LIFO)
BloqueMemoria* pilaMemoria = NULL;       // Pila de memoria asignada
ProcesoBloqueado* colaBloqueados = NULL; // Cola de procesos bloqueados

// ==== FUNCIÓN AUXILIAR: VERIFICAR ID DUPLICADO ====
bool existeID(int id) {
    // Verificar en lista de listos
    Proceso* temp = listaListos;
    while (temp != NULL) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en cola CPU
    temp = colaCPU;
    while (temp != NULL) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en pila suspendidos
    temp = pilaSuspendidos;
    while (temp != NULL) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en cola bloqueados
    ProcesoBloqueado* tempBloq = colaBloqueados;
    while (tempBloq != NULL) {
        if (tempBloq->id == id) return true;
        tempBloq = tempBloq->siguiente;
    }
    
    return false;
}

// ==== FUNCIÓN 1: CREAR PROCESO ====
void crearProceso() {
    Proceso* nuevo = new Proceso();
    
    cout << "Ingrese el ID del proceso: ";
    cin >> nuevo->id;
    
    // Validar ID duplicado
    if (existeID(nuevo->id)) {
        cout << "ERROR: Ya existe un proceso con ese ID.\n";
        delete nuevo;
        return;
    }
    
    cout << "Ingrese el nombre del proceso: ";
    cin.ignore();
    cin.getline(nuevo->nombre, 30);
    
    cout << "Ingrese la prioridad (0 = baja, 1 = media, 2 = alta): ";
    cin >> nuevo->prioridad;
    
    // Validar rango de prioridad
    if (nuevo->prioridad < 0 || nuevo->prioridad > 2) {
        cout << "ERROR: Prioridad debe ser 0, 1 o 2.\n";
        delete nuevo;
        return;
    }
    
    cout << "Ingrese la duracion del proceso: ";
    cin >> nuevo->duracion;
    
    // Validar duración positiva
    if (nuevo->duracion <= 0) {
        cout << "ERROR: La duracion debe ser mayor a 0.\n";
        delete nuevo;
        return;
    }
    
    nuevo->siguiente = NULL;
    
    // Agregar al final de la lista de procesos listos
    if (listaListos == NULL) {
        listaListos = nuevo;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    
    cout << "Proceso creado exitosamente!\n";
}

// ==== FUNCIÓN 2: VER PROCESOS EN ESTADO LISTO ====
void verProcesosListo() {
    if (listaListos == NULL) {
        cout << "\nNo hay procesos en estado 'Listo'.\n";
        return;
    }
    
    cout << "\n===== PROCESOS EN ESTADO 'LISTO' =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    Proceso* temp = listaListos;
    int contador = 0;
    
    while (temp != NULL) {
        cout << temp->id << "\t";
        cout << temp->nombre << "\t\t\t";
        
        if (temp->prioridad == 0) {
            cout << "Baja";
        } else if (temp->prioridad == 1) {
            cout << "Media";
        } else if (temp->prioridad == 2) {
            cout << "Alta";
        }
        
        cout << "\t\t" << temp->duracion << " seg\n";
        contador++;
        temp = temp->siguiente;
    }
    
    cout << "----------------------------------------------\n";
    cout << "Total de procesos: " << contador << endl;
}

// ==== FUNCIÓN 3: CAMBIAR ESTADO DE PROCESO ====
void cambiarEstado() {
    int idBuscar;
    cout << "Ingrese el ID del proceso a mover: ";
    cin >> idBuscar;
    
    // Buscar el proceso en la lista de listos
    Proceso* actual = listaListos;
    Proceso* anterior = NULL;
    
    while (actual != NULL && actual->id != idBuscar) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual == NULL) {
        cout << "Proceso no encontrado en la lista de listos.\n";
        return;
    }
    
    // Quitar el proceso de la lista de listos
    if (anterior == NULL) {
        listaListos = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    actual->siguiente = NULL;
    
    // Preguntar a donde mover el proceso
    int opcion;
    cout << "Seleccione el nuevo estado:\n";
    cout << "1. Mover a CPU (cola de prioridad)\n";
    cout << "2. Mover a bloqueados (espera por E/S)\n";
    cout << "3. Suspender proceso (pila de suspendidos)\n";
    cout << "Opcion: ";
    cin >> opcion;
    
    if (opcion == 1) {
        // Insertar en la cola de CPU ordenado por prioridad
        if (colaCPU == NULL || actual->prioridad > colaCPU->prioridad) {
            actual->siguiente = colaCPU;
            colaCPU = actual;
        } else {
            Proceso* temp = colaCPU;
            while (temp->siguiente != NULL && temp->siguiente->prioridad >= actual->prioridad) {
                temp = temp->siguiente;
            }
            actual->siguiente = temp->siguiente;
            temp->siguiente = actual;
        }
        cout << "Proceso movido a la cola de CPU.\n";
        
    } else if (opcion == 2) {
        // Crear nuevo proceso bloqueado
        ProcesoBloqueado* nuevoBloqueado = new ProcesoBloqueado();
        nuevoBloqueado->id = actual->id;
        
        // Copiar nombre manualmente
        int i = 0;
        while (actual->nombre[i] != '\0' && i < 29) {
            nuevoBloqueado->nombre[i] = actual->nombre[i];
            i++;
        }
        nuevoBloqueado->nombre[i] = '\0';
        
        nuevoBloqueado->tiempoEspera = 3; // Tiempo fijo de espera
        nuevoBloqueado->siguiente = NULL;
        
        // Agregar al final de la cola de bloqueados (FIFO)
        if (colaBloqueados == NULL) {
            colaBloqueados = nuevoBloqueado;
        } else {
            ProcesoBloqueado* temp = colaBloqueados;
            while (temp->siguiente != NULL) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoBloqueado;
        }
        
        delete actual;
        cout << "Proceso movido a bloqueados.\n";
        
    } else if (opcion == 3) {
        // Insertar en la pila de suspendidos (LIFO)
        actual->siguiente = pilaSuspendidos;
        pilaSuspendidos = actual;
        cout << "Proceso suspendido.\n";
        
    } else {
        cout << "Opcion invalida. Proceso regresado a lista de listos.\n";
        // Regresar a la lista de listos
        actual->siguiente = listaListos;
        listaListos = actual;
    }
}

// ==== FUNCIÓN 4: EJECUTAR PROCESO (CPU) ====
void ejecutarProceso() {
    if (colaCPU == NULL) {
        cout << "No hay procesos en la cola de CPU.\n";
        return;
    }
    
    // Tomar el primer proceso de la cola (mayor prioridad)
    Proceso* proceso = colaCPU;
    colaCPU = colaCPU->siguiente;
    
    cout << "\n=== EJECUTANDO PROCESO ===\n";
    cout << "ID: " << proceso->id << endl;
    cout << "Nombre: " << proceso->nombre << endl;
    cout << "Prioridad: ";
    
    if (proceso->prioridad == 0) {
        cout << "Baja";
    } else if (proceso->prioridad == 1) {
        cout << "Media";
    } else if (proceso->prioridad == 2) {
        cout << "Alta";
    }
    
    cout << "\nDuracion restante: " << proceso->duracion << " seg\n";
    
    // Simular ejecución (reducir duración en 1)
    proceso->duracion = proceso->duracion - 1;
    
    if (proceso->duracion <= 0) {
        cout << "Proceso terminado!\n";
        delete proceso;
    } else {
        cout << "Proceso interrumpido. Duracion restante: " << proceso->duracion << " seg\n";
        
        // Regresar el proceso a la lista de listos
        proceso->siguiente = NULL;
        
        if (listaListos == NULL) {
            listaListos = proceso;
        } else {
            Proceso* temp = listaListos;
            while (temp->siguiente != NULL) {
                temp = temp->siguiente;
            }
            temp->siguiente = proceso;
        }
    }
}

// ==== FUNCIÓN 5: ASIGNAR MEMORIA ====
void asignarMemoria() {
    BloqueMemoria* nuevo = new BloqueMemoria();
    
    cout << "Ingrese el ID del proceso: ";
    cin >> nuevo->idProceso;
    
    cout << "Ingrese el tamanio del bloque de memoria (KB): ";
    cin >> nuevo->tamanio;
    
    if (nuevo->tamanio <= 0) {
        cout << "ERROR: El tamanio debe ser mayor a 0.\n";
        delete nuevo;
        return;
    }
    
    // Insertar al inicio de la pila (LIFO)
    nuevo->siguiente = pilaMemoria;
    pilaMemoria = nuevo;
    
    cout << "Memoria asignada correctamente.\n";
}

// ==== FUNCIÓN 6: LIBERAR MEMORIA ====
void liberarMemoria() {
    if (pilaMemoria == NULL) {
        cout << "No hay bloques para liberar.\n";
    } else {
        BloqueMemoria* temp = pilaMemoria;
        cout << "Liberando bloque de " << temp->tamanio << " KB del proceso " << temp->idProceso << "\n";
        pilaMemoria = pilaMemoria->siguiente;
        delete temp;
        cout << "Bloque de memoria liberado correctamente.\n";
    }
}

// ==== FUNCIÓN 7: VER PROCESOS BLOQUEADOS ====
void verBloqueados() {
    if (colaBloqueados == NULL) {
        cout << "No hay procesos bloqueados actualmente.\n";
        return;
    }
    
    ProcesoBloqueado* actual = colaBloqueados;
    cout << "\n===== PROCESOS BLOQUEADOS =====\n";
    int contador = 0;
    
    while (actual != NULL) {
        cout << "ID: " << actual->id << endl;
        cout << "Nombre: " << actual->nombre << endl;
        cout << "Tiempo de espera: " << actual->tiempoEspera << " ciclos\n";
        cout << "--------------------------------\n";
        contador++;
        actual = actual->siguiente;
    }
    cout << "Total de procesos bloqueados: " << contador << endl;
}

// ==== FUNCIÓN 8: VER COLA DE CPU ====
void verColaCPU() {
    if (colaCPU == NULL) {
        cout << "No hay procesos en la cola de CPU.\n";
        return;
    }
    
    Proceso* temp = colaCPU;
    cout << "\n===== COLA DE CPU (POR PRIORIDAD) =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    int contador = 0;
    while (temp != NULL) {
        cout << temp->id << "\t";
        cout << temp->nombre << "\t\t\t";
        
        if (temp->prioridad == 0) {
            cout << "Baja";
        } else if (temp->prioridad == 1) {
            cout << "Media";
        } else if (temp->prioridad == 2) {
            cout << "Alta";
        }
        
        cout << "\t\t" << temp->duracion << " seg\n";
        contador++;
        temp = temp->siguiente;
    }
    cout << "----------------------------------------------\n";
    cout << "Total en cola CPU: " << contador << endl;
}

// ==== FUNCIÓN 9: VER ESTADO DE MEMORIA ====
void verMemoria() {
    if (pilaMemoria == NULL) {
        cout << "No hay bloques de memoria asignados.\n";
        return;
    }
    
    BloqueMemoria* temp = pilaMemoria;
    cout << "\n===== MEMORIA ASIGNADA (PILA LIFO) =====\n";
    cout << "ID Proceso\tTamanio\n";
    cout << "-------------------\n";
    
    int totalMemoria = 0;
    int bloques = 0;
    while (temp != NULL) {
        cout << temp->idProceso << "\t\t" << temp->tamanio << " KB\n";
        totalMemoria += temp->tamanio;
        bloques++;
        temp = temp->siguiente;
    }
    cout << "-------------------\n";
    cout << "Total bloques: " << bloques << endl;
    cout << "Memoria total: " << totalMemoria << " KB\n";
}

// ==== FUNCIÓN 10: VER PROCESOS SUSPENDIDOS ====
void verSuspendidos() {
    if (pilaSuspendidos == NULL) {
        cout << "No hay procesos suspendidos.\n";
        return;
    }
    
    Proceso* temp = pilaSuspendidos;
    cout << "\n===== PROCESOS SUSPENDIDOS (PILA LIFO) =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    int contador = 0;
    while (temp != NULL) {
        cout << temp->id << "\t";
        cout << temp->nombre << "\t\t\t";
        
        if (temp->prioridad == 0) {
            cout << "Baja";
        } else if (temp->prioridad == 1) {
            cout << "Media";
        } else if (temp->prioridad == 2) {
            cout << "Alta";
        }
        
        cout << "\t\t" << temp->duracion << " seg\n";
        contador++;
        temp = temp->siguiente;
    }
    cout << "----------------------------------------------\n";
    cout << "Total procesos suspendidos: " << contador << endl;
}

// ==== FUNCIÓN 11: REANUDAR PROCESO SUSPENDIDO ====
void reanudarSuspendido() {
    if (pilaSuspendidos == NULL) {
        cout << "No hay procesos suspendidos para reanudar.\n";
        return;
    }
    
    // Sacar el proceso más reciente de la pila (LIFO)
    Proceso* proceso = pilaSuspendidos;
    pilaSuspendidos = pilaSuspendidos->siguiente;
    proceso->siguiente = NULL;
    
    // Agregar al final de la lista de listos
    if (listaListos == NULL) {
        listaListos = proceso;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = proceso;
    }
    
    cout << "Proceso " << proceso->id << " (" << proceso->nombre << ") reanudado y movido a lista de listos.\n";
}

// ==== FUNCIÓN 12: DESBLOQUEAR PROCESO ====
void desbloquearProceso() {
    if (colaBloqueados == NULL) {
        cout << "No hay procesos bloqueados para desbloquear.\n";
        return;
    }
    
    // Sacar el primer proceso de la cola (FIFO)
    ProcesoBloqueado* bloqueado = colaBloqueados;
    colaBloqueados = colaBloqueados->siguiente;
    
    // Crear nuevo proceso para la lista de listos
    Proceso* nuevo = new Proceso();
    nuevo->id = bloqueado->id;
    
    // Copiar nombre
    int i = 0;
    while (bloqueado->nombre[i] != '\0' && i < 29) {
        nuevo->nombre[i] = bloqueado->nombre[i];
        i++;
    }
    nuevo->nombre[i] = '\0';
    
    // Asignar valores por defecto (el proceso perdió su contexto)
    nuevo->prioridad = 1; // Prioridad media por defecto
    nuevo->duracion = 2;  // Duración mínima
    nuevo->siguiente = NULL;
    
    // Agregar a la lista de listos
    if (listaListos == NULL) {
        listaListos = nuevo;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    
    cout << "Proceso " << bloqueado->id << " (" << bloqueado->nombre << ") desbloqueado y movido a lista de listos.\n";
    delete bloqueado;
}

// ==== FUNCIÓN 13: ELIMINAR PROCESO ====
void eliminarProceso() {
    int id;
    cout << "Ingrese el ID del proceso a eliminar: ";
    cin >> id;
    
    // Buscar en lista de listos
    Proceso* actual = listaListos;
    Proceso* anterior = NULL;
    
    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != NULL) {
        if (anterior == NULL) {
            listaListos = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        cout << "Proceso " << id << " eliminado de la lista de listos.\n";
        delete actual;
        return;
    }
    
    // Buscar en cola CPU
    actual = colaCPU;
    anterior = NULL;
    
    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != NULL) {
        if (anterior == NULL) {
            colaCPU = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        cout << "Proceso " << id << " eliminado de la cola de CPU.\n";
        delete actual;
        return;
    }
    
    // Buscar en pila suspendidos
    actual = pilaSuspendidos;
    anterior = NULL;
    
    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != NULL) {
        if (anterior == NULL) {
            pilaSuspendidos = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        cout << "Proceso " << id << " eliminado de la pila de suspendidos.\n";
        delete actual;
        return;
    }
    
    cout << "Proceso con ID " << id << " no encontrado.\n";
}

// ==== FUNCIÓN PRINCIPAL ====
int main() {
    int opcion;
    
    do {
        cout << "\n========== SISTEMA DE GESTION DE PROCESOS ==========\n";
        cout << "1.  Crear nuevo proceso\n";
        cout << "2.  Ver procesos en estado 'Listo'\n";
        cout << "3.  Cambiar estado de un proceso\n";
        cout << "4.  Ejecutar proceso (CPU)\n";
        cout << "5.  Asignar memoria a un proceso\n";
        cout << "6.  Liberar memoria\n";
        cout << "7.  Ver procesos bloqueados\n";
        cout << "8.  Ver cola de CPU\n";
        cout << "9.  Ver estado de memoria\n";
        cout << "10. Ver procesos suspendidos\n";
        cout << "11. Reanudar proceso suspendido\n";
        cout << "12. Desbloquear proceso\n";
        cout << "13. Eliminar proceso\n";
        cout << "14. Salir\n";
        cout << "===================================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            crearProceso();
        } else if (opcion == 2) {
            verProcesosListo();
        } else if (opcion == 3) {
            cambiarEstado();
        } else if (opcion == 4) {
            ejecutarProceso();
        } else if (opcion == 5) {
            asignarMemoria();
        } else if (opcion == 6) {
            liberarMemoria();
        } else if (opcion == 7) {
            verBloqueados();
        } else if (opcion == 8) {
            verColaCPU();
        } else if (opcion == 9) {
            verMemoria();
        } else if (opcion == 10) {
            verSuspendidos();
        } else if (opcion == 11) {
            reanudarSuspendido();
        } else if (opcion == 12) {
            desbloquearProceso();
        } else if (opcion == 13) {
            eliminarProceso();
        } else if (opcion == 14) {
            cout << "Saliendo del sistema...\n";
        } else {
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
        
    } while (opcion != 14);

    return 0;
}