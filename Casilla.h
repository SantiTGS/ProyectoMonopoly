#ifndef CASILLA_H
#define CASILLA_H

#include <string>
using namespace std;

class Jugador;

// ===== TIPOS DE CASILLAS =====
// Usar estas constantes para identificar cada tipo
const int TIPO_SALIDA = 0;
const int TIPO_PROPIEDAD = 1;
const int TIPO_ARCA_COMUNAL = 2;
const int TIPO_CASUALIDAD = 3;
const int TIPO_IMPUESTO = 4;
const int TIPO_ESTACION = 5;
const int TIPO_SERVICIO = 6;
const int TIPO_CARCEL = 7;
const int TIPO_PARKING_GRATIS = 8;
const int TIPO_IR_A_CARCEL = 9;

/**
 * Clase Base Casilla
 * Representa una casilla del tablero de Monopoly
 */
class Casilla {
protected:
    string nombre;     // Nombre de la casilla
    int tipo;          // Tipo (usa las constantes int de arriba)
    int posicion;      // Posición en el tablero (0-39)

public:
    //pre: nombre no vacío, tipo válido (0-9), 0 <= posicion < 40
    //post: Casilla creada con nombre, tipo y posición asignados
    Casilla(string nombre, int tipo, int posicion);

    //pre: Casilla existe
    //post: Memoria liberada
    virtual ~Casilla();

    // Acción que se ejecuta cuando un jugador cae en esta casilla
    // Cada tipo de casilla implementa su propia versión
    //pre: jugador != nullptr
    //post: acción específica ejecutada según tipo de casilla
    virtual void accionAlCaer(Jugador* jugador) = 0;

    // Getters
    //pre: ninguna
    //post: retorna nombre de la casilla
    string getNombre() const;

    //pre: ninguna
    //post: retorna tipo de la casilla
    int getTipo() const;

    //pre: ninguna
    //post: retorna posición de la casilla (0-39)
    int getPosicion() const;
};

// ========== CASILLAS ESPECÍFICAS ==========

/**
 * Casilla SALIDA (GO)
 * Al caer directamente aquí: recibe $400
 * Al pasar por aquí: recibe $200 (manejado en Jugador::mover)
 */
class CasillaSalida : public Casilla {
private:
    int bonoPasar;     // $200 al pasar
    int bonoLlegar;    // $400 al caer

public:
    //pre: 0 <= posicion < 40
    //post: CasillaSalida creada en posicion, bonoPasar = 200, bonoLlegar = 400
    CasillaSalida(int posicion);

    //pre: jugador != nullptr
    //post: jugador recibe bonoLlegar ($400)
    void accionAlCaer(Jugador* jugador) override;

    //pre: ninguna
    //post: retorna bonoPasar ($200)
    int getBonoPasar() const;
};

/**
 * Casilla PARKING GRATIS
 * No hace nada, solo descansar
 */
class CasillaParking : public Casilla {
public:
    //pre: 0 <= posicion < 40
    //post: CasillaParking creada en posicion
    CasillaParking(int posicion);

    //pre: jugador != nullptr
    //post: mensaje informativo mostrado, sin efecto en jugador
    void accionAlCaer(Jugador* jugador) override;
};

/**
 * Casilla CÁRCEL (visitando)
 * Si solo visitas, no pasa nada
 * Si estás preso, te quedas aquí
 */
class CasillaCarcel : public Casilla {
public:
    //pre: 0 <= posicion < 40
    //post: CasillaCarcel creada en posicion
    CasillaCarcel(int posicion);

    //pre: jugador != nullptr
    //post: si jugador.estaEnCarcel() == false, mensaje de visita; sino, sin efecto
    void accionAlCaer(Jugador* jugador) override;
};

/**
 * Casilla VE A LA CÁRCEL
 * Te manda directo a la cárcel
 */
class CasillaIrCarcel : public Casilla {
public:
    //pre: 0 <= posicion < 40
    //post: CasillaIrCarcel creada en posicion
    CasillaIrCarcel(int posicion);

    //pre: jugador != nullptr
    //post: jugador enviado a cárcel (posicion = 10, enCarcel = true)
    void accionAlCaer(Jugador* jugador) override;
};

/**
 * Casilla IMPUESTO
 * Pagas un monto fijo al banco
 */
class CasillaImpuesto : public Casilla {
private:
    int monto;  // Cantidad a pagar

public:
    //pre: nombre no vacío, 0 <= posicion < 40, monto > 0
    //post: CasillaImpuesto creada con nombre, posicion y monto asignados
    CasillaImpuesto(string nombre, int posicion, int monto);

    //pre: jugador != nullptr
    //post: jugador paga monto (si tiene dinero suficiente)
    void accionAlCaer(Jugador* jugador) override;

    //pre: ninguna
    //post: retorna monto del impuesto
    int getMonto() const;
};

/**
 * Casilla ARCA COMUNAL
 * Sacas una carta de Arca Comunal
 */
class CasillaArcaComunal : public Casilla {
public:
    //pre: 0 <= posicion < 40
    //post: CasillaArcaComunal creada en posicion
    CasillaArcaComunal(int posicion);

    //pre: jugador != nullptr
    //post: mensaje informativo mostrado (carta ejecutada en Juego)
    void accionAlCaer(Jugador* jugador) override;
};

/**
 * Casilla CASUALIDAD
 * Sacas una carta de Casualidad
 */
class CasillaCasualidad : public Casilla {
public:
    //pre: 0 <= posicion < 40
    //post: CasillaCasualidad creada en posicion
    CasillaCasualidad(int posicion);

    //pre: jugador != nullptr
    //post: mensaje informativo mostrado (carta ejecutada en Juego)
    void accionAlCaer(Jugador* jugador) override;
};

#endif