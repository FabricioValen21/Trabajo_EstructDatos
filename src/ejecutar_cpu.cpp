#include <iostream>
using namespace std;

// CASO:
// El CPU quiere ejecutar el proceso con mayor prioridad.
// Este archivo desencola un proceso, reduce su duración y decide si terminó o vuelve a la cola.

void ejecutarProceso() {
   void ejecutarProceso() {
    if (colaCPU == nullptr) {
        cout << "No hay procesos para ejecutar\n";
        return;
    }

    ProcesoCPU* proceso = desencolar();
    proceso->duracion--;

    cout << "Ejecutando proceso: " << proceso->nombre 
         << " (ID: " << proceso->id << "), Duraci�n restante: " << proceso->duracion << endl;

    if (proceso->duracion <= 0) {
        cout << "Proceso finalizado.\n";
        liberarMemoria(proceso->id);
        delete proceso;
    } else {
        encolar(proceso);
    }
}

}

