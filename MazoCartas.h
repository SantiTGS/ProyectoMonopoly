#ifndef MAZOCARTAS_H
#define MAZOCARTAS_H

#include <queue>
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
 * TAD MazoCartas - COLA (Queue)
 * Gestiona las cartas de Arca Comunal o Casualidad.
 * Las cartas se colocan debajo del montón después de leídas (comportamiento de cola).
 */
class MazoCartas {
private:
    queue<Carta*> cartas;  // Cola de cartas - CAMBIO: de vector a queue
    string tipo;

public:
    //pre: tipoDeMazo no vacío
    //post: MazoCartas creado con tipo asignado, cola vacía
    MazoCartas(string tipoDeMazo);

    //pre: MazoCartas existe
    //post: todas las cartas eliminadas, cola vaciada
    ~MazoCartas();

    //pre: carta != nullptr
    //post: carta agregada al final de la cola
    void agregarCarta(Carta* carta);

    //pre: cartas.size() > 0
    //post: retorna carta al frente de la cola, carta removida del frente y agregada al final (comportamiento de cola circular)
    Carta* sacarCarta();

    //pre: ninguna
    //post: cartas mezcladas aleatoriamente
    void barajar();

    //pre: ninguna
    //post: retorna tipo del mazo
    string getTipo() const;

    //pre: mazo vacío
    //post: 16 cartas de Arca Comunal creadas y agregadas a la cola, mazo barajado
    void crearMazoArcaComunal();

    //pre: mazo vacío
    //post: 16 cartas de Casualidad creadas y agregadas a la cola, mazo barajado
    void crearMazoCasualidad();
};

#endif
