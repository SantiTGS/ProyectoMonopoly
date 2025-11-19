#include "Jugador.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ========== CONSTRUCTOR Y DESTRUCTOR ==========

Jugador::Jugador(string nombre, int dineroInicial) 
    : nombre(nombre), 
      dinero(dineroInicial), 
      posicion(0), 
      enCarcel(false), 
      turnosEnCarcel(0),
      turnosSinCarcel(0),
      activo(true) {
    cout << nombre << " entra al juego con $" << dinero << endl;
}

Jugador::~Jugador() {
    // Las propiedades se eliminan en el Banco, aquí solo limpiamos el vector
    propiedades.clear();
}

// ========== MOVIMIENTO ==========

void Jugador::mover(int pasos) {
    // Guardar posición anterior para saber si pasamos por Salida
    int posicionAnterior = posicion;
    
    // Calcular nueva posición (módulo 40 para dar la vuelta)
    posicion = (posicion + pasos) % 40;
    
    // ¿Pasó por la Salida (casilla 0)?
    // Si la nueva posición es menor, es porque dio la vuelta
    if (posicion < posicionAnterior) {
        recibirDinero(200);
        cout << ">>> " << nombre << " paso por la SALIDA! +$200" << endl;
    }
}

void Jugador::moverA(int nuevaPosicion) {
    // Validar que la posición sea válida (0-39)
    if (nuevaPosicion < 0 || nuevaPosicion >= 40) {
        return;
    }
    
    posicion = nuevaPosicion;
    cout << nombre << " se mueve a casilla " << posicion << endl;
}

// ========== DINERO ==========

void Jugador::recibirDinero(int monto) {
    // Validar que el monto sea positivo
    if (monto <= 0) {
        return;
    }
    
    // Sumar dinero
    dinero += monto;
    cout << nombre << " recibe $" << monto 
         << " (Total: $" << dinero << ")" << endl;
}

bool Jugador::pagarDinero(int monto) {
    // Validar que el monto sea positivo
    if (monto <= 0) {
        return true;
    }
    
    // ¿Tiene suficiente dinero?
    if (dinero >= monto) {
        // Sí tiene, restar dinero
        dinero -= monto;
        cout << nombre << " paga $" << monto 
             << " (Quedan: $" << dinero << ")" << endl;
        return true;
    }
    
    // No tiene dinero suficiente
    cout << ">>> " << nombre << " NO puede pagar $" << monto 
         << " (Solo tiene: $" << dinero << ")" << endl;
    return false;
}

void Jugador::pagarRenta(int monto, Jugador* dueno) {
    // Validar que el dueño exista
    if (dueno == nullptr) {
        return;
    }
    
    // No puede pagarse a sí mismo
    if (dueno == this) {
        cout << nombre << " esta en su propia propiedad" << endl;
        return;
    }
    
    // Intentar pagar
    if (pagarDinero(monto)) {
        // Si pudo pagar, dar dinero al dueño
        dueno->recibirDinero(monto);
        cout << ">>> " << nombre << " pago renta a " 
             << dueno->getNombre() << endl;
    } else {
        // No pudo pagar - esto debería llevar a bancarrota
        cout << ">>> " << nombre << " esta en PROBLEMAS financieros!" << endl;
    }
}

// ========== PROPIEDADES ==========

void Jugador::agregarPropiedad(Propiedad* prop) {
    // Validar que la propiedad exista
    if (prop == nullptr) {
        return;
    }
    
    // Agregar al vector de propiedades
    propiedades.push_back(prop);
}

void Jugador::removerPropiedad(Propiedad* prop) {
    // Buscar la propiedad en el vector
    auto it = find(propiedades.begin(), propiedades.end(), prop);
    
    // Si la encontró, eliminarla
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

// ========== CÁRCEL ==========

void Jugador::irACarcel() {
    enCarcel = true;
    turnosEnCarcel = 0;
    posicion = 10;  // La cárcel está en la casilla 10
    cout << "\n>>> " << nombre << " va DIRECTO A LA CARCEL! <<<\n" << endl;
}

void Jugador::salirDeCarcel() {
    enCarcel = false;
    turnosEnCarcel = 0;
    cout << ">>> " << nombre << " sale de la carcel" << endl;
}

void Jugador::incrementarTurnosCarcel() {
    // Solo incrementar si está en cárcel
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
    cout << "\n*** " << nombre << " ha QUEBRADO! ***" << endl;
    cout << "    Sale del juego." << endl;
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

// ========== INFORMACIÓN ==========

void Jugador::mostrarEstado() const {
    cout << "\n========== " << nombre << " ==========" << endl;
    cout << "Dinero:      $" << dinero << endl;
    cout << "Posicion:    Casilla " << posicion << endl;
    cout << "Propiedades: " << propiedades.size() << endl;
    
    // Mostrar estado especial
    if (enCarcel) {
        cout << "Estado:      EN CARCEL (Turno " << turnosEnCarcel << "/3)" << endl;
    } else if (!activo) {
        cout << "Estado:      QUEBRADO" << endl;
    } else {
        cout << "Estado:      ACTIVO" << endl;
    }
    
    cout << "===============================" << endl;
}
