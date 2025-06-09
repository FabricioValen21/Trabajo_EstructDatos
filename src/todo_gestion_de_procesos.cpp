#include <iostream>
#include <cstring>
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
Proceso* listaListos = nullptr;             // Lista de procesos en estado "Listo"
Proceso* colaCPU = nullptr;                 // Cola de prioridad de CPU
BloqueMemoria* pilaMemoria = nullptr;       // Pila de memoria asignada
ProcesoBloqueado* colaBloqueados = nullptr; // Cola de procesos bloqueados
int memoriaTotal = 1000;                    // Memoria total del sistema
int memoriaUsada = 0;                       // Memoria actualmente en uso

// ==== DECLARACIÓN DE FUNCIONES ====
void crearProceso();
void verProcesosListo();
void cambiarEstado();
void ejecutarProceso();
void asignarMemoria();
void liberarMemoria();
void liberarMemoriaDeProcesoEspecifico(int idProceso);
void verBloqueados();
void bloquearProceso();
void desbloquearProceso();
void terminarProceso();
void mostrarEstadoMemoria();
void limpiarMemoriaAlSalir();
bool existeProceso(int id);
bool validarEntradaPositiva(int valor);

// ==== FUNCIONES AUXILIARES ====
bool existeProceso(int id) {
    // Buscar en lista de listos
    Proceso* temp = listaListos;
    while (temp != nullptr) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Buscar en cola de CPU
    temp = colaCPU;
    while (temp != nullptr) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Buscar en cola de bloqueados
    ProcesoBloqueado* tempBloq = colaBloqueados;
    while (tempBloq != nullptr) {
        if (tempBloq->id == id) return true;
        tempBloq = tempBloq->siguiente;
    }
    
    return false;
}

bool validarEntradaPositiva(int valor) {
    return valor > 0;
}

void mostrarEstadoMemoria() {
    cout << "\n========== ESTADO DE MEMORIA ==========\n";
    cout << "Memoria total: " << memoriaTotal << " MB\n";
    cout << "Memoria usada: " << memoriaUsada << " MB\n";
    cout << "Memoria libre: " << (memoriaTotal - memoriaUsada) << " MB\n";
    
    if (pilaMemoria == nullptr) {
        cout << "No hay bloques de memoria asignados.\n";
    } else {
        cout << "\nBloques asignados (LIFO):\n";
        cout << "ID Proceso\tTamaño\n";
        cout << "--------------------\n";
        BloqueMemoria* temp = pilaMemoria;
        while (temp != nullptr) {
            cout << temp->idProceso << "\t\t" << temp->tamanio << " MB\n";
            temp = temp->siguiente;
        }
    }
    cout << "=======================================\n";
}

// ==== FUNCIONES PRINCIPALES ====

void crearProceso() {
    Proceso* nuevo = new Proceso();
    
    cout << "Ingrese el ID del proceso: ";
    cin >> nuevo->id;
    
    // Validaciones mejoradas
    if (!validarEntradaPositiva(nuevo->id)) {
        cout << "Error: El ID debe ser un número positivo.\n";
        delete nuevo;
        return;
    }
    
    if (existeProceso(nuevo->id)) {
        cout << "Error: Ya existe un proceso con ID " << nuevo->id << endl;
        delete nuevo;
        return;
    }
    
    cout << "Ingrese el nombre del proceso: ";
    cin.ignore();
    cin.getline(nuevo->nombre, 30);
    
    cout << "Ingrese la prioridad (0: baja, 1: media, 2: alta): ";
    cin >> nuevo->prioridad;
    
    while (nuevo->prioridad < 0 || nuevo->prioridad > 2) {
        cout << "Prioridad inválida. Ingrese 0, 1 o 2: ";
        cin >> nuevo->prioridad;
    }
    
    cout << "Ingrese la duración del proceso (en segundos): ";
    cin >> nuevo->duracion;
    
    while (!validarEntradaPositiva(nuevo->duracion)) {
        cout << "La duración debe ser mayor a 0: ";
        cin >> nuevo->duracion;
    }
    
    // Insertar al final de la lista (FIFO dentro de la misma prioridad)
    nuevo->siguiente = nullptr;
    if (listaListos == nullptr) {
        listaListos = nuevo;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    
    cout << "Proceso creado exitosamente y agregado a la lista de 'Listos'.\n";
}

void verProcesosListo() {
    if (listaListos == nullptr) {
        cout << "\nNo hay procesos en estado 'Listo'.\n";
        return;
    }
    
    cout << "\n============ PROCESOS EN ESTADO 'LISTO' ============\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------------\n";
    
    Proceso* temp = listaListos;
    int totalProcesos = 0;
    
    while (temp != nullptr) {
        cout << temp->id << "\t";
        cout << temp->nombre;
        
        int len = strlen(temp->nombre);
        if (len < 8) {
            cout << "\t\t\t";
        } else if (len < 16) {
            cout << "\t\t";
        } else {
            cout << "\t";
        }
        
        switch(temp->prioridad) {
            case 0: cout << "Baja"; break;
            case 1: cout << "Media"; break;
            case 2: cout << "Alta"; break;
            default: cout << "Desconocida";
        }
        cout << "\t\t" << temp->duracion << " seg" << endl;
        
        totalProcesos++;
        temp = temp->siguiente;
    }
    
    cout << "----------------------------------------------------\n";
    cout << "Total de procesos en estado 'Listo': " << totalProcesos << endl;
    cout << "====================================================\n";
}

void cambiarEstado() {
    if (listaListos == nullptr && colaBloqueados == nullptr) {
        cout << "No hay procesos disponibles para cambiar de estado.\n";
        return;
    }
    
    int idBuscado, tipoOperacion;
    cout << "\nTipos de cambio de estado:\n";
    cout << "1. Listo -> CPU\n";
    cout << "2. Bloqueado -> Listo\n";
    cout << "Seleccione una opción: ";
    cin >> tipoOperacion;
    
    cout << "Ingrese el ID del proceso: ";
    cin >> idBuscado;
    
    if (tipoOperacion == 1) {
        // Mover de Listo a CPU
        Proceso* anterior = nullptr;
        Proceso* actual = listaListos;
        
        while (actual != nullptr && actual->id != idBuscado) {
            anterior = actual;
            actual = actual->siguiente;
        }
        
        if (actual == nullptr) {
            cout << "Proceso no encontrado en la lista de 'Listos'.\n";
            return;
        }
        
        // Remover de lista de listos
        if (anterior == nullptr) {
            listaListos = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        
        // Insertar en cola de CPU ordenado por prioridad
        actual->siguiente = nullptr;
        if (colaCPU == nullptr || actual->prioridad > colaCPU->prioridad) {
            actual->siguiente = colaCPU;
            colaCPU = actual;
        } else {
            Proceso* temp = colaCPU;
            while (temp->siguiente != nullptr && temp->siguiente->prioridad >= actual->prioridad) {
                temp = temp->siguiente;
            }
            actual->siguiente = temp->siguiente;
            temp->siguiente = actual;
        }
        
        cout << "Proceso " << actual->nombre << " movido a la cola de CPU.\n";
        
    } else if (tipoOperacion == 2) {
        // Mover de Bloqueado a Listo
        desbloquearProceso();
    } else {
        cout << "Opción inválida.\n";
    }
}

void ejecutarProceso() {
    if (colaCPU == nullptr) {
        cout << "No hay procesos en la cola de CPU para ejecutar.\n";
        return;
    }
    
    // El proceso con mayor prioridad está al frente (ya ordenado)
    Proceso* procesoEjecutar = colaCPU;
    colaCPU = colaCPU->siguiente;
    
    cout << "Ejecutando proceso: " << procesoEjecutar->nombre 
         << " (ID: " << procesoEjecutar->id 
         << ", Prioridad: " << procesoEjecutar->prioridad << ")\n";
    
    procesoEjecutar->duracion--;
    cout << "Tiempo restante: " << procesoEjecutar->duracion << " segundos\n";
    
    if (procesoEjecutar->duracion <= 0) {
        cout << "Proceso " << procesoEjecutar->nombre << " ha terminado.\n";
        liberarMemoriaDeProcesoEspecifico(procesoEjecutar->id);
        delete procesoEjecutar;
    } else {
        // Reencolar en cola de CPU manteniendo orden de prioridad
        procesoEjecutar->siguiente = nullptr;
        if (colaCPU == nullptr || procesoEjecutar->prioridad > colaCPU->prioridad) {
            procesoEjecutar->siguiente = colaCPU;
            colaCPU = procesoEjecutar;
        } else {
            Proceso* temp = colaCPU;
            while (temp->siguiente != nullptr && temp->siguiente->prioridad >= procesoEjecutar->prioridad) {
                temp = temp->siguiente;
            }
            procesoEjecutar->siguiente = temp->siguiente;
            temp->siguiente = procesoEjecutar;
        }
    }
}

void asignarMemoria() {
    int idProceso, tamanio;
    
    cout << "Ingrese el ID del proceso: ";
    cin >> idProceso;
    
    if (!existeProceso(idProceso)) {
        cout << "Error: No existe un proceso con ID " << idProceso << endl;
        return;
    }
    
    cout << "Ingrese el tamaño de memoria a asignar (MB): ";
    cin >> tamanio;
    
    if (!validarEntradaPositiva(tamanio)) {
        cout << "Error: El tamaño debe ser positivo.\n";
        return;
    }
    
    if (memoriaUsada + tamanio > memoriaTotal) {
        cout << "Error: Memoria insuficiente. Disponible: " 
             << (memoriaTotal - memoriaUsada) << " MB\n";
        return;
    }
    
    BloqueMemoria* nuevoBloque = new BloqueMemoria();
    nuevoBloque->idProceso = idProceso;
    nuevoBloque->tamanio = tamanio;
    nuevoBloque->siguiente = pilaMemoria;
    pilaMemoria = nuevoBloque;
    memoriaUsada += tamanio;
    
    cout << "Memoria asignada: " << tamanio << " MB para el proceso " 
         << idProceso << endl;
    cout << "Memoria libre restante: " << (memoriaTotal - memoriaUsada) << " MB\n";
}

void liberarMemoria() {
    if (pilaMemoria == nullptr) {
        cout << "No hay bloques de memoria para liberar.\n";
        return;
    }
    
    BloqueMemoria* bloqueALiberar = pilaMemoria;
    pilaMemoria = pilaMemoria->siguiente;
    memoriaUsada -= bloqueALiberar->tamanio;
    
    cout << "Liberando " << bloqueALiberar->tamanio 
         << " MB del proceso " << bloqueALiberar->idProceso << endl;
    
    delete bloqueALiberar;
}

void liberarMemoriaDeProcesoEspecifico(int idProceso) {
    BloqueMemoria* anterior = nullptr;
    BloqueMemoria* actual = pilaMemoria;
    bool encontrado = false;
    
    while (actual != nullptr) {
        if (actual->idProceso == idProceso) {
            encontrado = true;
            if (anterior == nullptr) {
                pilaMemoria = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            
            memoriaUsada -= actual->tamanio;
            cout << "Liberando " << actual->tamanio 
                 << " MB del proceso " << idProceso << endl;
            
            BloqueMemoria* temp = actual;
            actual = actual->siguiente;
            delete temp;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
    
    if (!encontrado) {
        cout << "No se encontraron bloques de memoria para el proceso " << idProceso << endl;
    }
}

void verBloqueados() {
    if (colaBloqueados == nullptr) {
        cout << "\nNo hay procesos bloqueados.\n";
        return;
    }
    
    cout << "\n========== PROCESOS BLOQUEADOS ==========\n";
    cout << "ID\tNombre\t\t\tTiempo Espera\n";
    cout << "-----------------------------------------\n";
    
    ProcesoBloqueado* temp = colaBloqueados;
    int totalBloqueados = 0;
    
    while (temp != nullptr) {
        cout << temp->id << "\t" << temp->nombre;
        
        int len = strlen(temp->nombre);
        if (len < 8) {
            cout << "\t\t\t";
        } else if (len < 16) {
            cout << "\t\t";
        } else {
            cout << "\t";
        }
        
        cout << temp->tiempoEspera << " seg" << endl;
        
        totalBloqueados++;
        temp = temp->siguiente;
    }
    
    cout << "-----------------------------------------\n";
    cout << "Total de procesos bloqueados: " << totalBloqueados << endl;
    cout << "=========================================\n";
}

void bloquearProceso() {
    if (colaCPU == nullptr) {
        cout << "No hay procesos en CPU para bloquear.\n";
        return;
    }
    
    int idProceso;
    cout << "Ingrese el ID del proceso a bloquear: ";
    cin >> idProceso;
    
    Proceso* anterior = nullptr;
    Proceso* actual = colaCPU;
    
    while (actual != nullptr && actual->id != idProceso) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual == nullptr) {
        cout << "Proceso no encontrado en la cola de CPU.\n";
        return;
    }
    
    ProcesoBloqueado* nuevoBloqueado = new ProcesoBloqueado();
    nuevoBloqueado->id = actual->id;
    strcpy(nuevoBloqueado->nombre, actual->nombre);
    
    cout << "Ingrese el tiempo estimado de espera (segundos): ";
    cin >> nuevoBloqueado->tiempoEspera;
    
    while (!validarEntradaPositiva(nuevoBloqueado->tiempoEspera)) {
        cout << "El tiempo debe ser positivo: ";
        cin >> nuevoBloqueado->tiempoEspera;
    }
    
    nuevoBloqueado->siguiente = nullptr;
    
    // Agregar al final de la cola de bloqueados (FIFO)
    if (colaBloqueados == nullptr) {
        colaBloqueados = nuevoBloqueado;
    } else {
        ProcesoBloqueado* temp = colaBloqueados;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoBloqueado;
    }
    
    // Remover de la cola de CPU
    if (anterior == nullptr) {
        colaCPU = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    delete actual;
    cout << "Proceso bloqueado exitosamente.\n";
}

void desbloquearProceso() {
    if (colaBloqueados == nullptr) {
        cout << "No hay procesos bloqueados.\n";
        return;
    }
    
    int idProceso;
    cout << "Ingrese el ID del proceso a desbloquear: ";
    cin >> idProceso;
    
    ProcesoBloqueado* anterior = nullptr;
    ProcesoBloqueado* actual = colaBloqueados;
    
    while (actual != nullptr && actual->id != idProceso) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual == nullptr) {
        cout << "Proceso no encontrado en la cola de bloqueados.\n";
        return;
    }
    
    // Crear nuevo proceso para lista de listos
    Proceso* nuevoProceso = new Proceso();
    nuevoProceso->id = actual->id;
    strcpy(nuevoProceso->nombre, actual->nombre);
    
    cout << "Ingrese la prioridad del proceso (0: baja, 1: media, 2: alta): ";
    cin >> nuevoProceso->prioridad;
    
    while (nuevoProceso->prioridad < 0 || nuevoProceso->prioridad > 2) {
        cout << "Prioridad inválida. Ingrese 0, 1 o 2: ";
        cin >> nuevoProceso->prioridad;
    }
    
    cout << "Ingrese la duración restante: ";
    cin >> nuevoProceso->duracion;
    
    while (!validarEntradaPositiva(nuevoProceso->duracion)) {
        cout << "La duración debe ser positiva: ";
        cin >> nuevoProceso->duracion;
    }
    
    // Agregar al final de la lista de listos
    nuevoProceso->siguiente = nullptr;
    if (listaListos == nullptr) {
        listaListos = nuevoProceso;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoProceso;
    }
    
    // Remover de la cola de bloqueados
    if (anterior == nullptr) {
        colaBloqueados = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    delete actual;
    cout << "Proceso desbloqueado y movido a la lista de 'Listos'.\n";
}

void terminarProceso() {
    int idProceso;
    cout << "Ingrese el ID del proceso a terminar: ";
    cin >> idProceso;
    
    // Buscar en lista de listos
    Proceso* anterior = nullptr;
    Proceso* actual = listaListos;
    
    while (actual != nullptr && actual->id != idProceso) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != nullptr) {
        if (anterior == nullptr) {
            listaListos = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        
        liberarMemoriaDeProcesoEspecifico(idProceso);
        delete actual;
        cout << "Proceso terminado desde lista de 'Listos'.\n";
        return;
    }
    
    // Buscar en cola de CPU
    anterior = nullptr;
    actual = colaCPU;
    
    while (actual != nullptr && actual->id != idProceso) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != nullptr) {
        if (anterior == nullptr) {
            colaCPU = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        
        liberarMemoriaDeProcesoEspecifico(idProceso);
        delete actual;
        cout << "Proceso terminado desde cola de CPU.\n";
        return;
    }
    
    cout << "Proceso no encontrado.\n";
}

void limpiarMemoriaAlSalir() {
    // Liberar memoria de procesos listos
    while (listaListos != nullptr) {
        Proceso* temp = listaListos;
        listaListos = listaListos->siguiente;
        delete temp;
    }
    
    // Liberar memoria de cola CPU
    while (colaCPU != nullptr) {
        Proceso* temp = colaCPU;
        colaCPU = colaCPU->siguiente;
        delete temp;
    }
    
    // Liberar memoria de procesos bloqueados
    while (colaBloqueados != nullptr) {
        ProcesoBloqueado* temp = colaBloqueados;
        colaBloqueados = colaBloqueados->siguiente;
        delete temp;
    }
    
    // Liberar bloques de memoria
    while (pilaMemoria != nullptr) {
        BloqueMemoria* temp = pilaMemoria;
        pilaMemoria = pilaMemoria->siguiente;
        delete temp;
    }
    
    cout << "Memoria del sistema liberada correctamente.\n";
}

int main() {
    int opcion;
    
    cout << "===== BIENVENIDO AL SIMULADOR DE SO =====\n";
    cout << "Sistema de Gestión de Procesos v2.0\n";
    cout << "Memoria total del sistema: " << memoriaTotal << " MB\n";
    cout << "==========================================\n";
    
    do {
        cout << "\n===== SISTEMA DE GESTION DE PROCESOS =====\n";
        cout << "Simulador de Sistema Operativo Basico\n";
        cout << "==========================================\n";
        cout << "1.  Crear nuevo proceso\n";
        cout << "2.  Ver procesos en estado 'Listo'\n";
        cout << "3.  Cambiar estado de un proceso\n";
        cout << "4.  Ejecutar proceso (CPU por prioridad)\n";
        cout << "5.  Asignar memoria a un proceso\n";
        cout << "6.  Liberar memoria (LIFO)\n";
        cout << "7.  Ver procesos bloqueados\n";
        cout << "8.  Bloquear proceso (simular E/S)\n";
        cout << "9.  Desbloquear proceso\n";
        cout << "10. Terminar proceso manualmente\n";
        cout << "11. Mostrar estado de memoria\n";
        cout << "12. Salir\n";
        cout << "==========================================\n";
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
            case 8: bloquearProceso(); break;
            case 9: desbloquearProceso(); break;
            case 10: terminarProceso(); break;
            case 11: mostrarEstadoMemoria(); break;
            case 12: 
                cout << "Saliendo del sistema...\n";
                limpiarMemoriaAlSalir();
                break;
            default: 
                cout << "Opción inválida. Intente nuevamente.\n"; 
                break;
        }
    } while(opcion != 12);

    return 0;
}