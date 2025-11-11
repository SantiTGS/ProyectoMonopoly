#include "casilla.h"
#include "Jugador.h"
#include <iostream>

// ========== CLASE BASE ==========

// ========== CONSTRUCTOR ==========
// Pre: nombre no vacío, 0 <= posicion < 40
// Post: Casilla creada con nombre, tipo y posición
Casilla::Casilla(string nombre, int tipo, int posicion)
    : nombre(nombre), tipo(tipo), posicion(posicion) {
}

// ========== DESTRUCTOR ==========
// Pre: Casilla existe
// Post: Memoria liberada
Casilla::~Casilla() {
}

// ========== GETTERS ==========

// Pre: Casilla existe
// Post: Retorna nombre
string Casilla::getNombre() const {
    return nombre;
}

// Pre: Casilla existe
// Post: Retorna tipo
int Casilla::getTipo() const {
    return tipo;
}

// Pre: Casilla existe
// Post: Retorna posicion
int Casilla::getPosicion() const {
    return posicion;
}

// ========== CASILLA SALIDA ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Salida creada con bonos
CasillaSalida::CasillaSalida(int posicion)
    : Casilla("Salida (GO)", TIPO_SALIDA, posicion),
      bonoPasar(200), bonoLlegar(400) {
}

// Pre: jugador != nullptr
// Post: Jugador recibe bonoLlegar
void CasillaSalida::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " cayo en la Salida!" << endl;
    jugador->recibirDinero(bonoLlegar);
    cout << "Recibe $" << bonoLlegar << endl;
}

// Pre: Casilla existe
// Post: Retorna bonoPasar
int CasillaSalida::getBonoPasar() const {
    return bonoPasar;
}

// ========== CASILLA PARKING ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Parking creada
CasillaParking::CasillaParking(int posicion)
    : Casilla("Parking Gratis", TIPO_PARKING_GRATIS, posicion) {
}

// Pre: jugador != nullptr
// Post: Mensaje mostrado
void CasillaParking::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " esta en Parking Gratis. A descansar!" << endl;
}

// ========== CASILLA CARCEL ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Carcel creada
CasillaCarcel::CasillaCarcel(int posicion)
    : Casilla("Carcel (Visitando)", TIPO_CARCEL, posicion) {
}

// Pre: jugador != nullptr
// Post: Mensaje mostrado
void CasillaCarcel::accionAlCaer(Jugador* jugador) {
    if (!jugador->estaEnCarcel()) {
        cout << jugador->getNombre() << " esta solo visitando la carcel." << endl;
    }
}

// ========== CASILLA IR A CARCEL ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Ir a Carcel creada
CasillaIrCarcel::CasillaIrCarcel(int posicion)
    : Casilla("Ve a la Carcel", TIPO_IR_A_CARCEL, posicion) {
}

// Pre: jugador != nullptr
// Post: Jugador enviado a cárcel
void CasillaIrCarcel::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " debe ir a la carcel!" << endl;
    jugador->irACarcel();
}

// ========== CASILLA IMPUESTO ==========

// Pre: nombre no vacío, posicion válida, monto > 0
// Post: Casilla Impuesto creada
CasillaImpuesto::CasillaImpuesto(string nombre, int posicion, int monto)
    : Casilla(nombre, TIPO_IMPUESTO, posicion), monto(monto) {
}

// Pre: jugador != nullptr
// Post: Jugador paga impuesto
void CasillaImpuesto::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " debe pagar " << nombre << ": $" << monto << endl;
    jugador->pagarDinero(monto);
}

// Pre: Casilla existe
// Post: Retorna monto
int CasillaImpuesto::getMonto() const {
    return monto;
}

// ========== CASILLA ARCA COMUNAL ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Arca Comunal creada
CasillaArcaComunal::CasillaArcaComunal(int posicion)
    : Casilla("Arca Comunal", TIPO_ARCA_COMUNAL, posicion) {
}

// Pre: jugador != nullptr
// Post: Mensaje mostrado
void CasillaArcaComunal::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " saco una carta de Arca Comunal!" << endl;
}

// ========== CASILLA CASUALIDAD ==========

// Pre: 0 <= posicion < 40
// Post: Casilla Casualidad creada
CasillaCasualidad::CasillaCasualidad(int posicion)
    : Casilla("Casualidad", TIPO_CASUALIDAD, posicion) {
}

// Pre: jugador != nullptr
// Post: Mensaje mostrado
void CasillaCasualidad::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " saco una carta de Casualidad!" << endl;
}
