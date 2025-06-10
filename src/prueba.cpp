#include <iostream>
#include <cstring>  // Para strcpy
using namespace std;

// ==== ESTRUCTURAS DEL SISTEMA ====
struct Proceso {
    int id;
    char nombre[30];
    int prioridad;
    int duracion;
    Proceso* siguiente;
};

struct BloqueMemoria {
    int idProceso;
    int tamanio;
    BloqueMemoria* siguiente;
};

struct ProcesoBloqueado {
    int id;
    char nombre[30];
    int tiempoEspera;
    ProcesoBloqueado* siguiente;
};

// ==== VARIABLES GLOBALES ====
Proceso* listaListos = nullptr;
Proceso* colaCPU = nullptr;
Proceso* pilaSuspendidos = nullptr;
BloqueMemoria* pilaMemoria = nullptr;
ProcesoBloqueado* colaBloqueados = nullptr;

// ==== FUNCIÓN AUXILIAR: VERIFICAR ID DUPLICADO ====
bool existeID(int id) {
    // Verificar en lista de listos
    Proceso* temp = listaListos;
    while (temp != nullptr) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en cola CPU
    temp = colaCPU;
    while (temp != nullptr) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en pila suspendidos
    temp = pilaSuspendidos;
    while (temp != nullptr) {
        if (temp->id == id) return true;
        temp = temp->siguiente;
    }
    
    // Verificar en cola bloqueados
    ProcesoBloqueado* tempBloq = colaBloqueados;
    while (tempBloq != nullptr) {
        if (tempBloq->id == id) return true;
        tempBloq = tempBloq->siguiente;
    }
    
    return false;
}

// ==== FUNCIÓN AUXILIAR: COPIAR STRING SEGURO ====
void copiarString(char destino[], const char origen[], int maxTamanio) {
    int i = 0;
    while (origen[i] != '\0' && i < maxTamanio - 1) {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

// ==== FUNCIÓN 1: CREAR PROCESO ====
void crearProceso() {
    Proceso* nuevo = new Proceso();
    
    cout << "Ingrese el ID del proceso: ";
    if (!(cin >> nuevo->id)) {
        cout << "ERROR: ID debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        delete nuevo;
        return;
    }
    
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
    if (!(cin >> nuevo->prioridad)) {
        cout << "ERROR: Prioridad debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        delete nuevo;
        return;
    }
    
    // Validar rango de prioridad
    if (nuevo->prioridad < 0 || nuevo->prioridad > 2) {
        cout << "ERROR: Prioridad debe ser 0, 1 o 2.\n";
        delete nuevo;
        return;
    }
    
    cout << "Ingrese la duracion del proceso: ";
    if (!(cin >> nuevo->duracion)) {
        cout << "ERROR: Duracion debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        delete nuevo;
        return;
    }
    
    // Validar duración positiva
    if (nuevo->duracion <= 0) {
        cout << "ERROR: La duracion debe ser mayor a 0.\n";
        delete nuevo;
        return;
    }
    
    nuevo->siguiente = nullptr;
    
    // Agregar al final de la lista de procesos listos
    if (listaListos == nullptr) {
        listaListos = nuevo;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    
    cout << "Proceso creado exitosamente!\n";
}

// ==== FUNCIÓN 2: VER PROCESOS EN ESTADO LISTO ====
void verProcesosListo() {
    if (listaListos == nullptr) {
        cout << "\nNo hay procesos en estado 'Listo'.\n";
        return;
    }
    
    cout << "\n===== PROCESOS EN ESTADO 'LISTO' =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    Proceso* temp = listaListos;
    int contador = 0;
    
    while (temp != nullptr) {
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
    if (!(cin >> idBuscar)) {
        cout << "ERROR: ID debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    
    // Buscar el proceso en la lista de listos
    Proceso* actual = listaListos;
    Proceso* anterior = nullptr;
    
    while (actual != nullptr && actual->id != idBuscar) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual == nullptr) {
        cout << "Proceso no encontrado en la lista de listos.\n";
        return;
    }
    
    // Quitar el proceso de la lista de listos
    if (anterior == nullptr) {
        listaListos = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    actual->siguiente = nullptr;
    
    // Preguntar a donde mover el proceso
    int opcion;
    cout << "Seleccione el nuevo estado:\n";
    cout << "1. Mover a CPU (cola de prioridad)\n";
    cout << "2. Mover a bloqueados (espera por E/S)\n";
    cout << "3. Suspender proceso (pila de suspendidos)\n";
    cout << "Opcion: ";
    
    if (!(cin >> opcion)) {
        cout << "ERROR: Opcion debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        // Regresar a la lista de listos
        actual->siguiente = listaListos;
        listaListos = actual;
        return;
    }
    
    if (opcion == 1) {
        // Insertar en la cola de CPU ordenado por prioridad
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
        cout << "Proceso movido a la cola de CPU.\n";
        
    } else if (opcion == 2) {
        // Crear nuevo proceso bloqueado
        ProcesoBloqueado* nuevoBloqueado = new ProcesoBloqueado();
        nuevoBloqueado->id = actual->id;
        
        // Copiar nombre de forma segura
        copiarString(nuevoBloqueado->nombre, actual->nombre, 30);
        
        nuevoBloqueado->tiempoEspera = 3;
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
    if (colaCPU == nullptr) {
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
        proceso->siguiente = nullptr;
        
        if (listaListos == nullptr) {
            listaListos = proceso;
        } else {
            Proceso* temp = listaListos;
            while (temp->siguiente != nullptr) {
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
    if (!(cin >> nuevo->idProceso)) {
        cout << "ERROR: ID debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        delete nuevo;
        return;
    }
    
    cout << "Ingrese el tamanio del bloque de memoria (KB): ";
    if (!(cin >> nuevo->tamanio)) {
        cout << "ERROR: Tamanio debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        delete nuevo;
        return;
    }
    
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
    if (pilaMemoria == nullptr) {
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
    if (colaBloqueados == nullptr) {
        cout << "No hay procesos bloqueados actualmente.\n";
        return;
    }
    
    ProcesoBloqueado* actual = colaBloqueados;
    cout << "\n===== PROCESOS BLOQUEADOS =====\n";
    int contador = 0;
    
    while (actual != nullptr) {
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
    if (colaCPU == nullptr) {
        cout << "No hay procesos en la cola de CPU.\n";
        return;
    }
    
    Proceso* temp = colaCPU;
    cout << "\n===== COLA DE CPU (POR PRIORIDAD) =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    int contador = 0;
    while (temp != nullptr) {
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
    if (pilaMemoria == nullptr) {
        cout << "No hay bloques de memoria asignados.\n";
        return;
    }
    
    BloqueMemoria* temp = pilaMemoria;
    cout << "\n===== MEMORIA ASIGNADA (PILA LIFO) =====\n";
    cout << "ID Proceso\tTamanio\n";
    cout << "-------------------\n";
    
    int totalMemoria = 0;
    int bloques = 0;
    while (temp != nullptr) {
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
    if (pilaSuspendidos == nullptr) {
        cout << "No hay procesos suspendidos.\n";
        return;
    }
    
    Proceso* temp = pilaSuspendidos;
    cout << "\n===== PROCESOS SUSPENDIDOS (PILA LIFO) =====\n";
    cout << "ID\tNombre\t\t\tPrioridad\tDuracion\n";
    cout << "----------------------------------------------\n";
    
    int contador = 0;
    while (temp != nullptr) {
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
    if (pilaSuspendidos == nullptr) {
        cout << "No hay procesos suspendidos para reanudar.\n";
        return;
    }
    
    // Sacar el proceso más reciente de la pila (LIFO)
    Proceso* proceso = pilaSuspendidos;
    pilaSuspendidos = pilaSuspendidos->siguiente;
    proceso->siguiente = nullptr;
    
    // Agregar al final de la lista de listos
    if (listaListos == nullptr) {
        listaListos = proceso;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = proceso;
    }
    
    cout << "Proceso " << proceso->id << " (" << proceso->nombre << ") reanudado y movido a lista de listos.\n";
}

// ==== FUNCIÓN 12: DESBLOQUEAR PROCESO ====
void desbloquearProceso() {
    if (colaBloqueados == nullptr) {
        cout << "No hay procesos bloqueados para desbloquear.\n";
        return;
    }
    
    // Sacar el primer proceso de la cola (FIFO)
    ProcesoBloqueado* bloqueado = colaBloqueados;
    colaBloqueados = colaBloqueados->siguiente;
    
    // Crear nuevo proceso para la lista de listos
    Proceso* nuevo = new Proceso();
    nuevo->id = bloqueado->id;
    
    // Copiar nombre de forma segura
    copiarString(nuevo->nombre, bloqueado->nombre, 30);
    
    // Asignar valores por defecto
    nuevo->prioridad = 1;
    nuevo->duracion = 2;
    nuevo->siguiente = nullptr;
    
    // Agregar a la lista de listos
    if (listaListos == nullptr) {
        listaListos = nuevo;
    } else {
        Proceso* temp = listaListos;
        while (temp->siguiente != nullptr) {
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
    if (!(cin >> id)) {
        cout << "ERROR: ID debe ser un numero.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    
    // Buscar en lista de listos
    Proceso* actual = listaListos;
    Proceso* anterior = nullptr;
    
    while (actual != nullptr && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != nullptr) {
        if (anterior == nullptr) {
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
    anterior = nullptr;
    
    while (actual != nullptr && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != nullptr) {
        if (anterior == nullptr) {
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
    anterior = nullptr;
    
    while (actual != nullptr && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if (actual != nullptr) {
        if (anterior == nullptr) {
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

// ==== FUNCIÓN DE LIMPIEZA DE MEMORIA ====
void limpiarMemoria() {
    // Limpiar lista de listos
    while (listaListos != nullptr) {
        Proceso* temp = listaListos;
        listaListos = listaListos->siguiente;
        delete temp;
    }
    
    // Limpiar cola CPU
    while (colaCPU != nullptr) {
        Proceso* temp = colaCPU;
        colaCPU = colaCPU->siguiente;
        delete temp;
    }
    
    // Limpiar pila suspendidos
    while (pilaSuspendidos != nullptr) {
        Proceso* temp = pilaSuspendidos;
        pilaSuspendidos = pilaSuspendidos->siguiente;
        delete temp;
    }
    
    // Limpiar pila memoria
    while (pilaMemoria != nullptr) {
        BloqueMemoria* temp = pilaMemoria;
        pilaMemoria = pilaMemoria->siguiente;
        delete temp;
    }
    
    // Limpiar cola bloqueados
    while (colaBloqueados != nullptr) {
        ProcesoBloqueado* temp = colaBloqueados;
        colaBloqueados = colaBloqueados->siguiente;
        delete temp;
    }
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
        
        if (!(cin >> opcion)) {
            cout << "ERROR: Opcion debe ser un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
            case 1: crearProceso(); break;
            case 2: verProcesosListo(); break;
            case 3: cambiarEstado(); break;
            case 4: ejecutarProceso(); break;
            case 5: asignarMemoria(); break;
            case 6: liberarMemoria(); break;
            case 7: verBloqueados(); break;
            case 8: verColaCPU(); break;
            case 9: verMemoria(); break;
            case 10: verSuspendidos(); break;
            case 11: reanudarSuspendido(); break;
            case 12: desbloquearProceso(); break;
            case 13: eliminarProceso(); break;
            case 14: 
                cout << "Limpiando memoria y saliendo del sistema...\n";
                limpiarMemoria();
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
        
    } while (opcion != 14);

    return 0;
}