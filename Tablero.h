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

    // Inserta casilla manteniendo la circularidad
    // Pre: casilla != nullptr
    // Post: Casilla insertada, enlaces circulares actualizados
    void insertarCasilla(Casilla* casilla);

    // Libera memoria del tablero
    // Pre: Tablero creado
    // Post: Todos los nodos eliminados, inicio = nullptr
    void destruirTablero();

public:
    // ========== CONSTRUCTOR ==========

    // Pre: Ninguna
    // Post: Tablero circular con 40 casillas creado
    Tablero();

    // ========== DESTRUCTOR ==========

    // Pre: Tablero existe
    // Post: Memoria liberada
    ~Tablero();

    // ========== FUNCIONES DE ACCESO ==========

    // Obtiene casilla por posición
    // Pre: 0 <= posicion < 40
    // Post: Retorna casilla en esa posición
    Casilla* obtenerCasilla(int posicion);

    // Avanza N casillas aprovechando circularidad
    // Pre: 0 <= posicionActual < 40, pasos >= 0
    // Post: Retorna casilla después de avanzar (da vueltas si pasos > 40)
    Casilla* avanzarDesde(int posicionActual, int pasos);

    // Retrocede N casillas usando enlace anterior
    // Pre: 0 <= posicionActual < 40, pasos >= 0
    // Post: Retorna casilla después de retroceder circularmente
    Casilla* retrocederDesde(int posicionActual, int pasos);

    // Obtiene número de casillas
    // Pre: Tablero existe
    // Post: Retorna numCasillas
    int getNumCasillas() const;

    // ========== FUNCIONES DE BUSQUEDA ==========

    // Busca casilla por nombre recorriendo el círculo
    // Pre: Tablero creado
    // Post: Retorna casilla encontrada o nullptr
    Casilla* buscarCasillaPorNombre(string nombre);

    // ========== FUNCIONES DE VISUALIZACION ==========

    // Muestra tablero completo
    // Pre: Tablero creado con casillas
    // Post: Imprime información de las 40 casillas
    void mostrarTablero() const;

    // ========== CREACION DE TABLERO CIRCULAR ==========

    // Crea las 40 casillas del Monopoly conectadas circularmente
    // Pre: Tablero vacío
    // Post: 40 casillas enlazadas en círculo, inicio apunta a casilla 0
    void crearTablero();
};

#endif