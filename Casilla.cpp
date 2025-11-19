#include "Casilla.h"
#include "Jugador.h"
#include <iostream>
using namespace std;

// ========== CLASE BASE CASILLA ==========

Casilla::Casilla(string nombre, int tipo, int posicion)
    : nombre(nombre), tipo(tipo), posicion(posicion) {
}

Casilla::~Casilla() {
}

string Casilla::getNombre() const {
    return nombre;
}

int Casilla::getTipo() const {
    return tipo;
}

int Casilla::getPosicion() const {
    return posicion;
}

// ========== CASILLA SALIDA ==========

CasillaSalida::CasillaSalida(int posicion)
    : Casilla("Salida (GO)", TIPO_SALIDA, posicion),
      bonoPasar(200), 
      bonoLlegar(400) {
}

void CasillaSalida::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n>>> " << jugador->getNombre() 
         << " cayo en la SALIDA!" << endl;
    cout << "    Recibe $" << bonoLlegar << endl;
    
    // Dar el bono completo por caer aquí
    jugador->recibirDinero(bonoLlegar);
}

int CasillaSalida::getBonoPasar() const {
    return bonoPasar;
}

// ========== CASILLA PARKING GRATIS ==========

CasillaParking::CasillaParking(int posicion)
    : Casilla("Parking Gratis", TIPO_PARKING_GRATIS, posicion) {
}

void CasillaParking::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n>>> " << jugador->getNombre() 
         << " esta en Parking Gratis" << endl;
    cout << "    A descansar!" << endl;
}

// ========== CASILLA CÁRCEL (VISITANDO) ==========

CasillaCarcel::CasillaCarcel(int posicion)
    : Casilla("Carcel (Visitando)", TIPO_CARCEL, posicion) {
}

void CasillaCarcel::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    // Si no está en cárcel, solo está visitando
    if (!jugador->estaEnCarcel()) {
        cout << "\n>>> " << jugador->getNombre() 
             << " esta solo visitando la carcel" << endl;
    }
}

// ========== CASILLA IR A LA CÁRCEL ==========

CasillaIrCarcel::CasillaIrCarcel(int posicion)
    : Casilla("Ve a la Carcel", TIPO_IR_A_CARCEL, posicion) {
}

void CasillaIrCarcel::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n*** " << jugador->getNombre() 
         << " debe ir a la CARCEL! ***" << endl;
    
    // Enviar a la cárcel
    jugador->irACarcel();
}

// ========== CASILLA IMPUESTO ==========

CasillaImpuesto::CasillaImpuesto(string nombre, int posicion, int monto)
    : Casilla(nombre, TIPO_IMPUESTO, posicion), monto(monto) {
}

void CasillaImpuesto::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n>>> " << jugador->getNombre() 
         << " cayo en " << nombre << endl;
    cout << "    Debe pagar: $" << monto << endl;
    
    // Intentar pagar
    bool pudoPagar = jugador->pagarDinero(monto);
    
    if (!pudoPagar) {
        cout << "    No tiene suficiente dinero!" << endl;
    }
}

int CasillaImpuesto::getMonto() const {
    return monto;
}

// ========== CASILLA ARCA COMUNAL ==========

CasillaArcaComunal::CasillaArcaComunal(int posicion)
    : Casilla("Arca Comunal", TIPO_ARCA_COMUNAL, posicion) {
}

void CasillaArcaComunal::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n>>> " << jugador->getNombre() 
         << " saco una carta de ARCA COMUNAL!" << endl;
    // La carta se ejecuta en Juego.cpp
}

// ========== CASILLA CASUALIDAD ==========

CasillaCasualidad::CasillaCasualidad(int posicion)
    : Casilla("Casualidad", TIPO_CASUALIDAD, posicion) {
}

void CasillaCasualidad::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }
    
    cout << "\n>>> " << jugador->getNombre() 
         << " saco una carta de CASUALIDAD!" << endl;
    // La carta se ejecuta en Juego.cpp
}
