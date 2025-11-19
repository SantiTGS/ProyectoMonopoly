#include "Banco.h"
#include <iostream>
using namespace std;

// ========== destructor ==========
Banco::Banco(int dineroInicial)
    : dineroTotal(dineroInicial), casasDisponibles(32),
      hotelesDisponibles(12), bonoPasarSalida(200) {
    cout << "Banco inicializado con $" << dineroTotal << endl;
}

Banco::~Banco() {
    propiedades.clear();
}

// ===========transacciones DINERO==============
void Banco::pagarAJugador(Jugador* jugador, int monto) {
    if (!jugador || monto <= 0) return;

    dineroTotal -= monto;
    jugador->recibirDinero(monto);
    cout << "Banco paga $" << monto << " a " << jugador->getNombre() << endl;
}

void Banco::recibirDeJugador(Jugador* jugador, int monto) {
    if (!jugador || monto <= 0) return;

    if (jugador->pagarDinero(monto)) {
        dineroTotal += monto;
        cout << jugador->getNombre() << " paga $" << monto << " al banco." << endl;
    }
}

void Banco::pagarBonoPasarSalida(Jugador* jugador) {
    if (jugador) pagarAJugador(jugador, bonoPasarSalida);
}

void Banco::pagarBonoCaerSalida(Jugador* jugador) {
    if (jugador) pagarAJugador(jugador, bonoPasarSalida * 2);
}

// ========== MANEJO DE PROPIEDADES ==========
void Banco::registrarPropiedad(Propiedad* propiedad) {
    if (propiedad) propiedades.push_back(propiedad);
}

Propiedad* Banco::buscarPropiedadPorNombre(string nombre) {
    for (auto* p : propiedades) {
        if (p->getNombre() == nombre) return p;
    }
    return nullptr;
}

Propiedad* Banco::buscarPropiedadPorPosicion(int posicion) {
    for (auto* p : propiedades) {
        if (p->getPosicion() == posicion) return p;
    }
    return nullptr;
}

bool Banco::venderPropiedad(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;

    if (!propiedad->estaDisponible()) {
        cout << "Esta propiedad ya tiene dueño." << endl;
        return false;
    }

    int precio = propiedad->getPrecioCompra();
    if (jugador->getDinero() < precio) {
        cout << jugador->getNombre() << " no tiene suficiente dinero " << endl;
        return false;
    }

    jugador->pagarDinero(precio);
    dineroTotal += precio;
    propiedad->comprar(jugador);

    cout << jugador->getNombre() << " compra " << propiedad->getNombre()
         << " por $" << precio << endl;

    return true;
}

bool Banco::comprarPropiedad(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;

    if (propiedad->getDueno() != jugador) {
        cout << "Esta propiedad no pertenece a: " << jugador->getNombre() << endl;
        return false;
    }

    int valorHipoteca = propiedad->getPrecioCompra() / 2;
    pagarAJugador(jugador, valorHipoteca);
    propiedad->liberar();

    cout << "Banco compra " << propiedad->getNombre()
         << " por $" << valorHipoteca << endl;

    return true;
}

vector<Propiedad*> Banco::obtenerPropiedadesDe(Jugador* jugador) {
    vector<Propiedad*> resultado;
    if (!jugador) return resultado;

    for (auto* p : propiedades) {
        if (p->getDueno() == jugador) resultado.push_back(p);
    }
    return resultado;
}

vector<Propiedad*> Banco::obtenerPropiedadesDisponibles() {
    vector<Propiedad*> resultado;
    for (auto* p : propiedades) {
        if (p->estaDisponible()) resultado.push_back(p);
    }
    return resultado;
}

// ========== CASAS Y HOTELES ==========
bool Banco::venderCasa(Jugador* jugador, Propiedad* propiedad) {
    if (!jugador || !propiedad) return false;

    if (casasDisponibles <= 0) {
        cout << "No hay casas disponibles en el banco:((" << endl;
        return false;
    }

    if (propiedad->getDueno() != jugador) {
        cout << "No eres dueño de esta propiedad.-." << endl;
        return false;
    }

    if (propiedad->construirCasa()) {
        casasDisponibles--;
        cout << "Casa vendida! Casas restantes: " << casasDisponibles << endl;
        return true;
    }
    return false;
}

bool Banco::venderHotel(Jugador* jugador, Propiedad* propiedad) {
    if (!jugador || !propiedad) return false;

    if (hotelesDisponibles <= 0) {
        cout << "No hay hoteles disponibles en el banco:(" << endl;
        return false;
    }

    if (propiedad->getDueno() != jugador) {
        cout << "No eres dueño de esta propiedad.-." << endl;
        return false;
    }

    if (propiedad->construirHotel()) {
        hotelesDisponibles--;
        casasDisponibles += 4; // Se devuelven 4 casas
        cout << "Hotel vendido. Hoteles restantes: " << hotelesDisponibles << endl;
        return true;
    }
    return false;
}

// ========== BANCARROTA ==========
void Banco::gestionarBancarrota(Jugador* jugador, Jugador* prestador) {
    if (!jugador) return;

    cout << "\n>>> " << jugador->getNombre() << " está en bancarrota. <<<" << endl;

    vector<Propiedad*> propsJugador = jugador->getPropiedades();

    if (prestador == nullptr) {
        // quiebra ante el banco
        cout << "Propiedades liberadas al banco." << endl;
        for (auto* p : propsJugador) p->liberar();
    } else {
        // quiebra ante otro jugador
        cout << "Propiedades transferidas a " << prestador->getNombre() << "." << endl;
        for (auto* p : propsJugador) {
            p->liberar();
            p->comprar(prestador);
        }
    }

    jugador->declararQuiebra();
}

// ========== INFORMACION ==========
int Banco::getDinero() const { return dineroTotal; }
int Banco::getCasasDisponibles() const { return casasDisponibles; }
int Banco::getHotelesDisponibles() const { return hotelesDisponibles; }

void Banco::mostrarEstado() const {
    cout << "\n=== $$ESTADO DEL BANCO$$ ===" << endl;
    cout << "Dinero total: $" << dineroTotal << endl;
    cout << "Casas disponibles: " << casasDisponibles << endl;
    cout << "Hoteles disponibles: " << hotelesDisponibles << endl;
    cout << "=========================" << endl;
}

void Banco::mostrarPropiedadesDisponibles() const {
    cout << "\n>>> PROPIEDADES DISPONIBLES <<<" << endl;
    for (auto* p : propiedades) {
        if (p->estaDisponible())
            cout << "  -- " << p->getNombre() << " ($" << p->getPrecioCompra() << ")\n";
    }
}

int Banco::calcularFortuna(Jugador* jugador) const {
    if (!jugador) return 0;
    int total = jugador->getDinero();
    for (auto* p : jugador->getPropiedades()) {
        total += p->getPrecioCompra();
        total += p->getNumCasas() * p->getPrecioCasa();
        total += p->getNumHoteles() * (p->getPrecioCasa() * 5);
    }
    return total;
}