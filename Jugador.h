#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>
using namespace std;

class Propiedad;

/**
 * Clase Jugador - Representa a un jugador del Monopoly
 *
 * Responsabilidades:
 * - Manejar el dinero del jugador
 * - Controlar la posición en el tablero
 * - Gestionar las propiedades que posee
 * - Manejar el estado de cárcel
 */
class Jugador {
private:
    // Datos básicos del jugador
    string nombre;
    int dinero;
    int posicion;  // De 0 a 39 en el tablero

    // Propiedades que posee
    vector<Propiedad*> propiedades;

    // Estado del juego
    bool enCarcel;
    int turnosEnCarcel;
    int turnosSinCarcel;
    bool activo;  // false si está en quiebra

public:
    //pre: nombre no vacío, dineroInicial >= 0
    //post: Jugador creado con nombre, dineroInicial, posicion = 0, enCarcel = false, activo = true
    Jugador(string nombre, int dineroInicial = 1500);

    //pre: Jugador existe
    //post: vector de propiedades vaciado, memoria liberada
    ~Jugador();

    // ===== MOVIMIENTO =====
    //pre: pasos >= 0
    //post: posicion = (posicion + pasos) % 40, si pasó por Salida recibe $200
    void mover(int pasos);

    //pre: 0 <= posicion < 40
    //post: posicion actualizada al valor especificado
    void moverA(int posicion);

    // ===== DINERO =====
    //pre: monto > 0
    //post: dinero += monto
    void recibirDinero(int monto);

    //pre: monto > 0
    //post: si dinero >= monto, dinero -= monto y retorna true; sino retorna false
    bool pagarDinero(int monto);

    //pre: monto > 0, dueno != nullptr, dueno != this
    //post: si this puede pagar, dinero -= monto, dueno recibe monto
    void pagarRenta(int monto, Jugador* dueno);

    // ===== PROPIEDADES =====
    //pre: prop != nullptr
    //post: prop agregada al vector propiedades
    void agregarPropiedad(Propiedad* prop);

    //pre: prop != nullptr, prop está en vector propiedades
    //post: prop removida del vector propiedades
    void removerPropiedad(Propiedad* prop);

    //pre: ninguna
    //post: retorna vector con todas las propiedades del jugador
    vector<Propiedad*> getPropiedades() const;

    //pre: ninguna
    //post: retorna cantidad de propiedades que posee el jugador
    int contarPropiedades() const;

    // ===== CÁRCEL =====
    //pre: ninguna
    //post: enCarcel = true, turnosEnCarcel = 0, posicion = 10
    void irACarcel();

    //pre: ninguna
    //post: enCarcel = false, turnosEnCarcel = 0
    void salirDeCarcel();

    //pre: ninguna
    //post: si enCarcel == true, turnosEnCarcel++
    void incrementarTurnosCarcel();

    //pre: ninguna
    //post: retorna true si enCarcel == true, false en caso contrario
    bool estaEnCarcel() const;

    //pre: ninguna
    //post: retorna turnosEnCarcel
    int getTurnosEnCarcel() const;

    // ===== QUIEBRA =====
    //pre: ninguna
    //post: activo = false
    void declararQuiebra();

    //pre: ninguna
    //post: retorna true si activo == true, false en caso contrario
    bool estaActivo() const;

    // ===== GETTERS =====
    //pre: ninguna
    //post: retorna nombre del jugador
    string getNombre() const;

    //pre: ninguna
    //post: retorna dinero del jugador
    int getDinero() const;

    //pre: ninguna
    //post: retorna posicion del jugador (0-39)
    int getPosicion() const;

    // ===== INFORMACIÓN =====
    //pre: ninguna
    //post: estado completo del jugador mostrado en consola
    void mostrarEstado() const;
};

#endif