#ifndef TABLERO_H
#define TABLERO_H

#include "Casilla.h"
#include <map>
#include <string>
using namespace std;

// ========== NODO PARA LISTA CIRCULAR DOBLEMENTE ENLAZADA ==========
// Cada nodo tiene punteros al siguiente y anterior, formando un círculo
struct NodoCasilla {
    Casilla* casilla;
    NodoCasilla* siguiente;  // Apunta al siguiente nodo (el último apunta al primero)
    NodoCasilla* anterior;   // Apunta al nodo anterior (el primero apunta al último)

    //pre: cas != nullptr
    //post: NodoCasilla creado con casilla asignada, siguiente = nullptr, anterior = nullptr
    NodoCasilla(Casilla* cas) : casilla(cas), siguiente(nullptr), anterior(nullptr) {}
};

// ========== TAD TABLERO - LISTA CIRCULAR DOBLEMENTE ENLAZADA ==========
// El tablero es un círculo de 40 casillas donde:
// - El último nodo apunta al primero (circular hacia adelante)
// - El primer nodo apunta al último (circular hacia atrás)
// - Permite movimiento bidireccional infinito
class Tablero {
private:
    NodoCasilla* inicio;  // Apunta a la casilla 0 (Salida)
    int numCasillas;
    map<int, NodoCasilla*> indice;  // Para acceso rápido O(log n)

    // ========== FUNCIONES PRIVADAS ==========

    //pre: casilla != nullptr
    //post: casilla insertada al final, enlaces circulares actualizados, indice actualizado, numCasillas++
    void insertarCasilla(Casilla* casilla);

    //pre: Tablero creado
    //post: todos los nodos y casillas eliminados, inicio = nullptr, numCasillas = 0, indice vaciado
    void destruirTablero();

public:
    // ========== CONSTRUCTOR ==========

    //pre: ninguna
    //post: Tablero creado vacío con inicio = nullptr, numCasillas = 0, crearTablero() invocado (40 casillas creadas y enlazadas circularmente)
    Tablero();

    // ========== DESTRUCTOR ==========

    //pre: Tablero existe
    //post: destruirTablero() invocado, toda la memoria liberada
    ~Tablero();

    // ========== FUNCIONES DE ACCESO ==========

    //pre: 0 <= posicion < 40
    //post: retorna puntero a casilla en posicion (usando indice), nullptr si no existe
    Casilla* obtenerCasilla(int posicion);

    //pre: 0 <= posicionActual < 40, pasos >= 0
    //post: retorna casilla después de avanzar pasos desde posicionActual (aprovecha circularidad)
    Casilla* avanzarDesde(int posicionActual, int pasos);

    //pre: 0 <= posicionActual < 40, pasos >= 0
    //post: retorna casilla después de retroceder pasos desde posicionActual (aprovecha circularidad)
    Casilla* retrocederDesde(int posicionActual, int pasos);

    //pre: ninguna
    //post: retorna numCasillas
    int getNumCasillas() const;

    // ========== FUNCIONES DE BUSQUEDA ==========

    //pre: Tablero creado con casillas
    //post: retorna primera casilla con nombre coincidente (recorrido circular), nullptr si no existe
    Casilla* buscarCasillaPorNombre(string nombre);

    // ========== FUNCIONES DE VISUALIZACION ==========

    //pre: Tablero creado con casillas
    //post: información de las 40 casillas mostrada en consola
    void mostrarTablero() const;

    // ========== CREACION DE TABLERO CIRCULAR ==========

    //pre: Tablero vacío (inicio == nullptr)
    //post: 40 casillas del Monopoly creadas y enlazadas circularmente, inicio apunta a casilla 0 (Salida)
    void crearTablero();
};

#endif