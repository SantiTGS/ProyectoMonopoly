#ifndef PROPIEDAD_H
#define PROPIEDAD_H

#include "casilla.h"
#include <string>
using namespace std;

class Jugador;

// Tipos de propiedades
const int PROP_CALLE = 0;
const int PROP_ESTACION = 1;
const int PROP_SERVICIO = 2;

// Colores de propiedades
const int COLOR_MARRON = 0;
const int COLOR_CELESTE = 1;
const int COLOR_ROSA = 2;
const int COLOR_NARANJA = 3;
const int COLOR_ROJO = 4;
const int COLOR_AMARILLO = 5;
const int COLOR_VERDE = 6;
const int COLOR_AZUL = 7;
const int COLOR_SIN_COLOR = 8;


//Clase que representa una propiedad comprable

class Propiedad : public Casilla {
protected:
    int tipoPropiedad;
    int color;
    int precioCompra;
    int rentaBase;
    Jugador* dueno;
    bool hipotecada;
    
    // Para calles
    int numCasas;
    int numHoteles;
    int precioCasa;

public:
    // Constructor para calle normal
    Propiedad(string nombre, int posicion, int precio, int renta, 
              int color, int precioCasa = 0);
    
    // Constructor para estaciones y servicios
    Propiedad(string nombre, int posicion, int precio, int tipo);
    
    ~Propiedad();
    
    void accionAlCaer(Jugador* jugador) override;
    
    // Compra y venta
    void comprar(Jugador* jugador);
    bool estaDisponible() const;
    void liberar();
    
    // Renta
    int calcularRenta() const;
    
    // Construccion (SIMPLIFICADO)
    bool construirCasa();
    bool construirHotel();
    
    // Hipoteca (SIMPLIFICADO)
    void hipotecar();
    bool estaHipotecada() const;
    
    // Getters
    int getTipoPropiedad() const;
    int getColor() const;
    int getPrecioCompra() const;
    int getRentaBase() const;
    Jugador* getDueno() const;
    int getNumCasas() const;
    int getNumHoteles() const;
    int getPrecioCasa() const;
    
    string getNombreColor() const;
    void mostrarInfo() const;
};

#endif
