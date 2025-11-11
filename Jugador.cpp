#include "jugador.h"
#include <iostream>
#include <algorithm>

Jugador::Jugador(string nombre, int dineroInicial) 
    : nombre(nombre), dinero(dineroInicial), posicion(0), 
      enCarcel(false), turnosEnCarcel(0), turnosSinCarcel(0), activo(true) { 
}

Jugador::~Jugador() {
    
}

// ========== MOVIMIENTO ==========
void Jugador::mover(int pasos) {
    int nuevaPosicion = (posicion + pasos) % 40;
    
    // Si paso por la salida
    if (nuevaPosicion < posicion) {
        recibirDinero(200);
        cout << nombre << " paso por la Salida y recibe $200" << endl;
    }
    
    posicion = nuevaPosicion;
}

void Jugador::moverA(int nuevaPosicion) {
    if (nuevaPosicion >= 0 && nuevaPosicion < 40) {
        posicion = nuevaPosicion;
    }
}

// ========== DINERO ==========
void Jugador::recibirDinero(int monto) {
    dinero += monto;
    cout << nombre << " recibe $" << monto << " (Total: $" << dinero << ")" << endl;
}

bool Jugador::pagarDinero(int monto) {
    if (dinero >= monto) {
        dinero -= monto;
        cout << nombre << " paga $" << monto << " (Restante: $" << dinero << ")" << endl;
        return true;
    } else {
        cout << nombre << " no tiene suficiente dinero! (Tiene: $" << dinero << ", Necesita: $" << monto << ")" << endl;
        // Aqui se podría implementar logica de hipoteca o quiebra
        return false;
    }
}

void Jugador::pagarRenta(int monto, Jugador* dueno) {
    if (dueno == this) {
        cout << nombre << " esta en su propia propiedad." << endl;
        return;
    }
    
    if (pagarDinero(monto)) {
        dueno->recibirDinero(monto);
        cout << nombre << " pago renta de $" << monto << " a " << dueno->getNombre() << endl;
    } else {
        // El jugador no puede pagar no dinero no money
        cout << "¡" << nombre << " no puede pagar la renta!" << endl;
    }
}

// ========== PROPIEDADES ==========
void Jugador::agregarPropiedad(Propiedad* prop) {
    propiedades.push_back(prop);
}

void Jugador::removerPropiedad(Propiedad* prop) {
    auto it = find(propiedades.begin(), propiedades.end(), prop);
    if (it != propiedades.end()) {
        propiedades.erase(it);
    }
}

vector<Propiedad*> Jugador::getPropiedades() const {
    return propiedades;
}

int Jugador::contarPropiedades() const {
    return propiedades.size();
}

// ========== CARCEL ==========
void Jugador::irACarcel() {
    enCarcel = true;
    turnosEnCarcel = 0;
    posicion = 10; // Posicion de la cárcel
    cout << nombre << " va a la carcel!" << endl;
}

void Jugador::salirDeCarcel() {
    enCarcel = false;
    turnosEnCarcel = 0;
    cout << nombre << " salió de la carcel!" << endl;
}

void Jugador::incrementarTurnosCarcel() {
    if (enCarcel) {
        turnosEnCarcel++;
    }
}

bool Jugador::estaEnCarcel() const {
    return enCarcel;
}

int Jugador::getTurnosEnCarcel() const {
    return turnosEnCarcel;
}

// ========== QUIEBRA ==========
void Jugador::declararQuiebra() {
    activo = false;
    cout << "¡" << nombre << " ha quebrado y sale del juego!" << endl;
}

bool Jugador::estaActivo() const {
    return activo;
}

// ========== GETTERS ==========
string Jugador::getNombre() const {
    return nombre;
}

int Jugador::getDinero() const {
    return dinero;
}

int Jugador::getPosicion() const {
    return posicion;
}
//sigue jugando?
void Jugador::mostrarEstado() const {
    cout << "\n===== " << nombre << " =====" << endl;
    cout << "Dinero: $" << dinero << endl;
    cout << "Posición: " << posicion << endl;
    cout << "Propiedades: " << propiedades.size() << endl;
    
    if (enCarcel) {
        cout << "Estado: EN CARCEL:( (Turno " << turnosEnCarcel << "/3)" << endl;
    } else {
        cout << "Estado: " << (activo ? "ACTIVO" : "QUEBRADO") << endl;
    }
    
    if (!propiedades.empty()) {
        cout << "Lista de propiedades: ";
        for (size_t i = 0; i < propiedades.size(); i++) {
            // Mostraremos el nombre cuando implementemos Propiedad
            cout << "[Propiedad " << i+1 << "] ";
        }
        cout << endl;
    }
    cout << "==================" << endl;
}