#ifndef MAZOCARTAS_H
#define MAZOCARTAS_H

#include <vector>
#include <string>
using namespace std;

class Jugador;

// Tipos de accion de las cartas
const int ACCION_RECIBIR_DINERO = 0;
const int ACCION_PAGAR_DINERO = 1;
const int ACCION_MOVER_A_POSICION = 2;
const int ACCION_IR_CARCEL = 3;

/**
 * @brief Carta del juego
 */
class Carta {
private:
    string descripcion;
    int tipoAccion;
    int valor;

public:
    //pre: desc no vacío, tipo válido (0-3), val >= 0
    //post: Carta creada con descripcion, tipoAccion y valor asignados
    Carta(string desc, int tipo, int val = 0);

    //pre: Carta existe
    //post: Memoria liberada
    ~Carta();

    //pre: jugador != nullptr
    //post: acción ejecutada según tipoAccion (recibir/pagar dinero, mover, ir a cárcel)
    void ejecutar(Jugador* jugador);

    //pre: ninguna
    //post: retorna descripcion de la carta
    string getDescripcion() const;

    //pre: ninguna
    //post: retorna tipoAccion
    int getTipoAccion() const;

    //pre: ninguna
    //post: retorna valor
    int getValor() const;
};

/**
 * @brief TAD MazoCartas - Vector Simple
 * Gestiona las cartas de Arca Comunal o Casualidad.
 */
class MazoCartas {
private:
    vector<Carta*> cartas;
    string tipo;
    int indiceActual;

public:
    //pre: tipoDeMazo no vacío
    //post: MazoCartas creado con tipo asignado, indiceActual = 0, vector vacío
    MazoCartas(string tipoDeMazo);

    //pre: MazoCartas existe
    //post: todas las cartas eliminadas, vector vaciado
    ~MazoCartas();

    //pre: carta != nullptr
    //post: carta agregada al vector cartas
    void agregarCarta(Carta* carta);

    //pre: cartas.size() > 0
    //post: retorna carta en indiceActual, indiceActual++, si indiceActual >= size() entonces barajar y reiniciar
    Carta* sacarCarta();

    //pre: ninguna
    //post: cartas mezcladas aleatoriamente, indiceActual = 0
    void barajar();

    //pre: ninguna
    //post: retorna tipo del mazo
    string getTipo() const;

    //pre: mazo vacío
    //post: 16 cartas de Arca Comunal creadas y agregadas, mazo barajado
    void crearMazoArcaComunal();

    //pre: mazo vacío
    //post: 16 cartas de Casualidad creadas y agregadas, mazo barajado
    void crearMazoCasualidad();
};

#endif