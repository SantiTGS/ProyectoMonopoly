#ifndef PROPIEDAD_H
#define PROPIEDAD_H

#include "Casilla.h"
#include <string>
using namespace std;

class Jugador;

// ===== TIPOS DE PROPIEDADES =====
const int PROP_CALLE = 0;
const int PROP_ESTACION = 1;
const int PROP_SERVICIO = 2;

// ===== COLORES DE CALLES =====
const int COLOR_MARRON = 0;
const int COLOR_CELESTE = 1;
const int COLOR_ROSA = 2;
const int COLOR_NARANJA = 3;
const int COLOR_ROJO = 4;
const int COLOR_AMARILLO = 5;
const int COLOR_VERDE = 6;
const int COLOR_AZUL = 7;
const int COLOR_SIN_COLOR = 8;  // Para estaciones y servicios

/**
 * Clase Propiedad - Casilla comprable del Monopoly
 * Puede ser: Calle, Estación o Servicio Público
 */
class Propiedad : public Casilla {
protected:
    // Tipo y características
    int tipoPropiedad;      // PROP_CALLE, PROP_ESTACION, PROP_SERVICIO
    int color;              // Solo para calles
    int precioCompra;       // Precio de compra
    int rentaBase;          // Renta sin construcciones

    // Dueño
    Jugador* dueno;         // nullptr = disponible
    bool hipotecada;        // true = hipotecada

    // Construcciones (solo para calles)
    int numCasas;           // 0 a 4 casas
    int numHoteles;         // 0 o 1 hotel
    int precioCasa;         // Precio de cada casa

public:
    //pre: nombre no vacío, 0 <= posicion < 40, precio > 0, renta >= 0, color válido (0-7), precioCasa >= 0
    //post: Propiedad tipo CALLE creada con todos los atributos asignados, dueno = nullptr, hipotecada = false, numCasas = 0, numHoteles = 0
    Propiedad(string nombre, int posicion, int precio, int renta,
              int color, int precioCasa = 0);

    //pre: nombre no vacío, 0 <= posicion < 40, precio > 0, tipo válido (PROP_ESTACION o PROP_SERVICIO)
    //post: Propiedad tipo ESTACION o SERVICIO creada, color = COLOR_SIN_COLOR, dueno = nullptr, hipotecada = false
    Propiedad(string nombre, int posicion, int precio, int tipo);

    //pre: Propiedad existe
    //post: Memoria liberada
    ~Propiedad();

    //pre: jugador != nullptr
    //post: si disponible, mensaje de compra; si es del jugador, mensaje informativo; si es de otro, cobra renta (si no hipotecada)
    void accionAlCaer(Jugador* jugador) override;

    // ===== COMPRA Y VENTA =====
    //pre: jugador != nullptr
    //post: dueno = jugador, propiedad agregada a lista del jugador (NO cobra dinero)
    void asignarDueno(Jugador* jugador);

    //pre: ninguna
    //post: retorna true si dueno == nullptr, false en caso contrario
    bool estaDisponible() const;

    //pre: ninguna
    //post: si dueno != nullptr, propiedad removida de lista del jugador, dueno = nullptr, numCasas = 0, numHoteles = 0, hipotecada = false
    void liberar();

    // ===== RENTA =====
    //pre: ninguna
    //post: retorna monto de renta según estado (0 si hipotecada, rentaBase si sin construcciones, aumenta según casas/hoteles)
    int calcularRenta() const;

    // ===== CONSTRUCCIÓN =====
    //pre: tipoPropiedad == PROP_CALLE, !hipotecada, numCasas < 4, dueno != nullptr, dueno tiene dinero >= precioCasa
    //post: si exitoso, numCasas++, dueno paga precioCasa, retorna true; sino retorna false
    bool construirCasa();

    //pre: tipoPropiedad == PROP_CALLE, !hipotecada, numCasas == 4, numHoteles == 0, dueno != nullptr, dueno tiene dinero >= precioCasa * 5
    //post: si exitoso, numCasas = 0, numHoteles = 1, dueno paga precioCasa * 5, retorna true; sino retorna false
    bool construirHotel();

    // ===== HIPOTECA =====
    //pre: !hipotecada, numCasas == 0, numHoteles == 0, dueno != nullptr
    //post: hipotecada = true, dueno recibe precioCompra / 2
    void hipotecar();

    //pre: ninguna
    //post: retorna true si hipotecada == true, false en caso contrario
    bool estaHipotecada() const;

    // ===== GETTERS =====
    //pre: ninguna
    //post: retorna tipoPropiedad
    int getTipoPropiedad() const;

    //pre: ninguna
    //post: retorna color
    int getColor() const;

    //pre: ninguna
    //post: retorna precioCompra
    int getPrecioCompra() const;

    //pre: ninguna
    //post: retorna rentaBase
    int getRentaBase() const;

    //pre: ninguna
    //post: retorna puntero a dueno (puede ser nullptr)
    Jugador* getDueno() const;

    //pre: ninguna
    //post: retorna numCasas
    int getNumCasas() const;

    //pre: ninguna
    //post: retorna numHoteles
    int getNumHoteles() const;

    //pre: ninguna
    //post: retorna precioCasa
    int getPrecioCasa() const;

    // ===== INFORMACIÓN =====
    //pre: ninguna
    //post: retorna nombre del color en español según valor de color
    string getNombreColor() const;

    //pre: ninguna
    //post: información completa de la propiedad mostrada en consola
    void mostrarInfo() const;
};

#endif
