#include "Juego.h"
#include <iostream>
#include <string>
using namespace std;

void mostrarComandos() {
    cout << "\nComandos disponibles:" << endl;
    cout << "  tirar          - Lanzar dados" << endl;
    cout << "  comprar        - Comprar propiedad actual" << endl;
    cout << "  propiedades    - Ver tus propiedades" << endl;
    cout << "  casa [nombre]  - Construir casa" << endl;
    cout << "  hotel [nombre] - Construir hotel" << endl;
    cout << "  deshacer       - Deshacer ultima jugada (PILA)" << endl;  // CAMBIO: Nuevo comando
    cout << "  estado         - Ver estado del juego" << endl;
    cout << "  terminar       - Terminar turno" << endl;
    cout << "  ayuda          - Mostrar comandos" << endl;
    cout << "  salir          - Terminar juego" << endl;
}

int main() {
    cout << "\n========================================" << endl;
    cout << "         MONOPOLY - ESTRUCTURAS DE DATOS" << endl;
    cout << "========================================" << endl;
    cout << "\nEstructuras de datos implementadas:" << endl;
    cout << "  * Lista Circular Doblemente Enlazada (Tablero)" << endl;
    cout << "  * Tabla Hash / unordered_map (Propiedades)" << endl;  // CAMBIO
    cout << "  * Cola / queue (Mazos de Cartas)" << endl;  // CAMBIO
    cout << "  * Pila / stack (Estados del Juego)" << endl;  // CAMBIO
    cout << "  * Vector / STL (Jugadores)" << endl;
    cout << "========================================\n" << endl;

    Juego juego;
    juego.inicializar();

    // Crear jugadores
    cout << "\nCuantos jugadores? (2-4): ";
    int numJugadores;
    cin >> numJugadores;
    cin.ignore();

    if (numJugadores < 2) numJugadores = 2;
    if (numJugadores > 4) numJugadores = 4;

    for (int i = 0; i < numJugadores; i++) {
        cout << "Nombre del jugador " << (i+1) << ": ";
        string nombre;
        getline(cin, nombre);
        juego.agregarJugador(nombre);
    }

    juego.iniciar();
    mostrarComandos();

    // Loop principal del juego
    while (!juego.haTerminado()) {
        juego.procesarTurno();

        cout << "\n> ";
        string comando;
        getline(cin, comando);

        if (comando == "tirar") {
            juego.tirarDados();

        } else if (comando == "comprar") {
            juego.comprarPropiedad();

        } else if (comando == "propiedades") {
            juego.mostrarPropiedades();

        } else if (comando.substr(0, 4) == "casa") {
            if (comando.length() > 5) {
                string prop = comando.substr(5);
                juego.construirCasa(prop);
            } else {
                cout << "Uso: casa [nombre propiedad]" << endl;
            }

        } else if (comando.substr(0, 5) == "hotel") {
            if (comando.length() > 6) {
                string prop = comando.substr(6);
                juego.construirHotel(prop);
            } else {
                cout << "Uso: hotel [nombre propiedad]" << endl;
            }

        } else if (comando == "deshacer") {  // CAMBIO: Nuevo comando
            juego.deshacerJugada();

        } else if (comando == "estado") {
            juego.mostrarEstado();

        } else if (comando == "terminar") {
            juego.terminarTurno();

        } else if (comando == "ayuda") {
            mostrarComandos();

        } else if (comando == "salir") {
            cout << "Terminando juego..." << endl;
            break;

        } else {
            cout << "Comando desconocido. Escribe 'ayuda'" << endl;
        }
    }

    if (juego.haTerminado()) {
        juego.mostrarGanador();
    }

    cout << "\n========================================" << endl;
    cout << "           FIN DEL JUEGO" << endl;
    cout << "========================================\n" << endl;

    return 0;
}
