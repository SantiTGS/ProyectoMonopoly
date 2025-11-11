#include "Tablero.h"
#include "casilla.h"
#include "Propiedad.h"
#include <iostream>

Tablero::Tablero() : inicio(nullptr), numCasillas(0) {
    crearTablero();
}

Tablero::~Tablero() {
    destruirTablero();
}

void Tablero::destruirTablero() {
    if (inicio == nullptr) return;

    NodoCasilla* actual = inicio;
    NodoCasilla* siguiente;

    // Romper el enlace circular
    inicio->anterior->siguiente = nullptr;

    // Eliminar todos los nodos
    while (actual != nullptr) {
        siguiente = actual->siguiente;
        delete actual->casilla;
        delete actual;
        actual = siguiente;
    }

    inicio = nullptr;
    numCasillas = 0;
    indice.clear();
}

void Tablero::insertarCasilla(Casilla* casilla) {
    NodoCasilla* nuevo = new NodoCasilla(casilla);

    if (inicio == nullptr) {
        // Primera casilla - enlace circular consigo misma
        inicio = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
    } else {
        // Insertar al final (antes del inicio)
        NodoCasilla* ultimo = inicio->anterior;

        nuevo->siguiente = inicio;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        inicio->anterior = nuevo;
    }

    // Agregar al indice para busqueda rapida
    indice[casilla->getPosicion()] = nuevo;
    numCasillas++;
}

Casilla* Tablero::obtenerCasilla(int posicion) {
    if (posicion < 0 || posicion >= 40) {
        return nullptr;
    }

    auto it = indice.find(posicion);
    if (it != indice.end()) {
        return it->second->casilla;
    }

    return nullptr;
}

Casilla* Tablero::avanzarDesde(int posicionActual, int pasos) {
    if (inicio == nullptr || pasos < 0) return nullptr;

    NodoCasilla* actual = indice[posicionActual];

    for (int i = 0; i < pasos; i++) {
        actual = actual->siguiente;
    }

    return actual->casilla;
}

Casilla* Tablero::retrocederDesde(int posicionActual, int pasos) {
    if (inicio == nullptr || pasos < 0) return nullptr;

    NodoCasilla* actual = indice[posicionActual];

    for (int i = 0; i < pasos; i++) {
        actual = actual->anterior;
    }

    return actual->casilla;
}

int Tablero::getNumCasillas() const {
    return numCasillas;
}

Casilla* Tablero::buscarCasillaPorNombre(string nombre) {
    if (inicio == nullptr) return nullptr;

    NodoCasilla* actual = inicio;

    do {
        if (actual->casilla->getNombre() == nombre) {
            return actual->casilla;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    return nullptr;
}

void Tablero::mostrarTablero() const {
    if (inicio == nullptr) {
        cout << "El tablero esta vacio." << endl;
        return;
    }

    cout << "\n========== TABLERO DE MONOPOLY ==========" << endl;

    NodoCasilla* actual = inicio;
    int contador = 0;

    do {
        cout << "[" << actual->casilla->getPosicion() << "] "
             << actual->casilla->getNombre() << endl;
        actual = actual->siguiente;
        contador++;
    } while (actual != inicio && contador < 40);

    cout << "==========================================" << endl;
    cout << "Total de casillas: " << numCasillas << endl;
}

void Tablero::crearTablero() {
    // ========== LAS 40 CASILLAS DEL MONOPOLY ==========

    // Posicion 0: SALIDA
    insertarCasilla(new CasillaSalida(0));

    // Posicion 1: Mediterranean Avenue (Marron)
    insertarCasilla(new Propiedad("Mediterranean Avenue", 1, 60, 2, COLOR_MARRON, 50));

    // Posicion 2: Arca Comunal
    insertarCasilla(new CasillaArcaComunal(2));

    // Posicion 3: Baltic Avenue (Marron)
    insertarCasilla(new Propiedad("Baltic Avenue", 3, 60, 4, COLOR_MARRON, 50));

    // Posicion 4: Impuesto sobre la Renta
    insertarCasilla(new CasillaImpuesto("Impuesto sobre la Renta", 4, 200));

    // Posicion 5: Reading Railroad (Estacion)
    insertarCasilla(new Propiedad("Reading Railroad", 5, 200, PROP_ESTACION));

    // Posicion 6: Oriental Avenue (Celeste)
    insertarCasilla(new Propiedad("Oriental Avenue", 6, 100, 6, COLOR_CELESTE, 50));

    // Posicion 7: Casualidad
    insertarCasilla(new CasillaCasualidad(7));

    // Posicion 8: Vermont Avenue (Celeste)
    insertarCasilla(new Propiedad("Vermont Avenue", 8, 100, 6, COLOR_CELESTE, 50));

    // Posicion 9: Connecticut Avenue (Celeste)
    insertarCasilla(new Propiedad("Connecticut Avenue", 9, 120, 8, COLOR_CELESTE, 50));

    // Posicion 10: CARCEL
    insertarCasilla(new CasillaCarcel(10));

    // Posicion 11: St. Charles Place (Rosa)
    insertarCasilla(new Propiedad("St. Charles Place", 11, 140, 10, COLOR_ROSA, 100));

    // Posicion 12: Compania Electrica (Servicio)
    insertarCasilla(new Propiedad("Electric Company", 12, 150, PROP_SERVICIO));

    // Posicion 13: States Avenue (Rosa)
    insertarCasilla(new Propiedad("States Avenue", 13, 140, 10, COLOR_ROSA, 100));

    // Posicion 14: Virginia Avenue (Rosa)
    insertarCasilla(new Propiedad("Virginia Avenue", 14, 160, 12, COLOR_ROSA, 100));

    // Posicion 15: Pennsylvania Railroad (Estacion)
    insertarCasilla(new Propiedad("Pennsylvania Railroad", 15, 200, PROP_ESTACION));

    // Posicion 16: St. James Place (Naranja)
    insertarCasilla(new Propiedad("St. James Place", 16, 180, 14, COLOR_NARANJA, 100));

    // Posicion 17: Arca Comunal
    insertarCasilla(new CasillaArcaComunal(17));

    // Posicion 18: Tennessee Avenue (Naranja)
    insertarCasilla(new Propiedad("Tennessee Avenue", 18, 180, 14, COLOR_NARANJA, 100));

    // Posicion 19: New York Avenue (Naranja)
    insertarCasilla(new Propiedad("New York Avenue", 19, 200, 16, COLOR_NARANJA, 100));

    // Posicion 20: PARKING GRATIS
    insertarCasilla(new CasillaParking(20));

    // Posicion 21: Kentucky Avenue (Rojo)
    insertarCasilla(new Propiedad("Kentucky Avenue", 21, 220, 18, COLOR_ROJO, 150));

    // Posicion 22: Casualidad
    insertarCasilla(new CasillaCasualidad(22));

    // Posicion 23: Indiana Avenue (Rojo)
    insertarCasilla(new Propiedad("Indiana Avenue", 23, 220, 18, COLOR_ROJO, 150));

    // Posicion 24: Illinois Avenue (Rojo)
    insertarCasilla(new Propiedad("Illinois Avenue", 24, 240, 20, COLOR_ROJO, 150));

    // Posicion 25: B&O Railroad (Estacion)
    insertarCasilla(new Propiedad("B&O Railroad", 25, 200, PROP_ESTACION));

    // Posicion 26: Atlantic Avenue (Amarillo)
    insertarCasilla(new Propiedad("Atlantic Avenue", 26, 260, 22, COLOR_AMARILLO, 150));

    // Posicion 27: Ventnor Avenue (Amarillo)
    insertarCasilla(new Propiedad("Ventnor Avenue", 27, 260, 22, COLOR_AMARILLO, 150));

    // Posicion 28: Compania de Agua (Servicio)
    insertarCasilla(new Propiedad("Water Works", 28, 150, PROP_SERVICIO));

    // Posicion 29: Marvin Gardens (Amarillo)
    insertarCasilla(new Propiedad("Marvin Gardens", 29, 280, 24, COLOR_AMARILLO, 150));

    // Posicion 30: VE A LA CARCEL
    insertarCasilla(new CasillaIrCarcel(30));

    // Posicion 31: Pacific Avenue (Verde)
    insertarCasilla(new Propiedad("Pacific Avenue", 31, 300, 26, COLOR_VERDE, 200));

    // Posicion 32: North Carolina Avenue (Verde)
    insertarCasilla(new Propiedad("North Carolina Avenue", 32, 300, 26, COLOR_VERDE, 200));

    // Posicion 33: Arca Comunal
    insertarCasilla(new CasillaArcaComunal(33));

    // Posicion 34: Pennsylvania Avenue (Verde)
    insertarCasilla(new Propiedad("Pennsylvania Avenue", 34, 320, 28, COLOR_VERDE, 200));

    // Posicion 35: Short Line Railroad (Estacion)
    insertarCasilla(new Propiedad("Short Line", 35, 200, PROP_ESTACION));

    // Posicion 36: Casualidad
    insertarCasilla(new CasillaCasualidad(36));

    // Posicion 37: Park Place (Azul)
    insertarCasilla(new Propiedad("Park Place", 37, 350, 35, COLOR_AZUL, 200));

    // Posicion 38: Impuesto de Lujo
    insertarCasilla(new CasillaImpuesto("Impuesto de Lujo", 38, 100));

    // Posicion 39: Boardwalk (Azul)
    insertarCasilla(new Propiedad("Boardwalk", 39, 400, 50, COLOR_AZUL, 200));

    cout << "Tablero completo creado con 40 casillas!" << endl;
}
