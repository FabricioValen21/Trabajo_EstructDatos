#include <iostream>
using namespace std;

// Prototipos de funciones (declaración externa)
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
