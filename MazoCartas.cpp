#include "MazoCartas.h"
#include "Jugador.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// ========== IMPLEMENTACION DE CARTA ==========

Carta::Carta(string desc, int tipo, int val)
    : descripcion(desc), tipoAccion(tipo), valor(val) {
}

Carta::~Carta() {
}

void Carta::ejecutar(Jugador* jugador) {
    cout << "\n>>> CARTA <<<" << endl;
    cout << descripcion << endl;

    if (tipoAccion == ACCION_RECIBIR_DINERO) {
        jugador->recibirDinero(valor);
        
    } else if (tipoAccion == ACCION_PAGAR_DINERO) {
        jugador->pagarDinero(valor);
        
    } else if (tipoAccion == ACCION_MOVER_A_POSICION) {
        cout << jugador->getNombre() << " se mueve a la posicion " << valor << endl;
        jugador->moverA(valor);
        
    } else if (tipoAccion == ACCION_IR_CARCEL) {
        cout << jugador->getNombre() << " va a la carcel!" << endl;
        jugador->irACarcel();
    }
}

string Carta::getDescripcion() const {
    return descripcion;
}

int Carta::getTipoAccion() const {
    return tipoAccion;
}

int Carta::getValor() const {
    return valor;
}

// ========== IMPLEMENTACION DE MAZOCARTAS ==========

MazoCartas::MazoCartas(string tipoDeMazo)
    : tipo(tipoDeMazo), indiceActual(0) {
}

MazoCartas::~MazoCartas() {
    for (Carta* c : cartas) {
        delete c;
    }
    cartas.clear();
}

void MazoCartas::agregarCarta(Carta* carta) {
    if (carta == nullptr) return;
    cartas.push_back(carta);
}

Carta* MazoCartas::sacarCarta() {
    if (cartas.empty()) {
        cout << "No hay cartas!" << endl;
        return nullptr;
    }

    // Si llegamos al final, rebarajar
    if (indiceActual >= cartas.size()) {
        cout << "Rebarajando " << tipo << "..." << endl;
        barajar();
        indiceActual = 0;
    }

    Carta* carta = cartas[indiceActual];
    indiceActual++;

    return carta;
}

void MazoCartas::barajar() {
    if (cartas.empty()) return;

    random_shuffle(cartas.begin(), cartas.end());
    indiceActual = 0;

    cout << "Mazo '" << tipo << "' barajado" << endl;
}

string MazoCartas::getTipo() const {
    return tipo;
}

void MazoCartas::crearMazoArcaComunal() {
    cout << "Creando mazo de Arca Comunal..." << endl;

    agregarCarta(new Carta("Avanza hasta la Salida. Cobra $200", ACCION_MOVER_A_POSICION, 0));
    agregarCarta(new Carta("Error bancario a tu favor. Recibe $200", ACCION_RECIBIR_DINERO, 200));
    agregarCarta(new Carta("Paga gastos medicos de $50", ACCION_PAGAR_DINERO, 50));
    agregarCarta(new Carta("Venta de acciones. Recibe $50", ACCION_RECIBIR_DINERO, 50));
    agregarCarta(new Carta("Fondo de seguros. Recibe $100", ACCION_RECIBIR_DINERO, 100));
    agregarCarta(new Carta("Paga gastos hospitalarios de $100", ACCION_PAGAR_DINERO, 100));
    agregarCarta(new Carta("Paga seguro de $50", ACCION_PAGAR_DINERO, 50));
    agregarCarta(new Carta("Recibe $25 por servicios", ACCION_RECIBIR_DINERO, 25));
    agregarCarta(new Carta("Recibe herencia de $100", ACCION_RECIBIR_DINERO, 100));
    agregarCarta(new Carta("Reembolso de impuestos. Recibe $20", ACCION_RECIBIR_DINERO, 20));
    agregarCarta(new Carta("Es tu cumpleanos! Recibe $50", ACCION_RECIBIR_DINERO, 50));
    agregarCarta(new Carta("Ve directamente a la carcel", ACCION_IR_CARCEL, 0));
    agregarCarta(new Carta("Paga tasas escolares de $150", ACCION_PAGAR_DINERO, 150));
    agregarCarta(new Carta("Recibe consultoria. Cobra $25", ACCION_RECIBIR_DINERO, 25));
    agregarCarta(new Carta("Recibe premio. Cobra $100", ACCION_RECIBIR_DINERO, 100));
    agregarCarta(new Carta("Paga multa de $75", ACCION_PAGAR_DINERO, 75));

    barajar();
}

void MazoCartas::crearMazoCasualidad() {
    cout << "Creando mazo de Casualidad..." << endl;

    agregarCarta(new Carta("Avanza hasta la Salida. Cobra $200", ACCION_MOVER_A_POSICION, 0));
    agregarCarta(new Carta("Avanza hasta Boardwalk", ACCION_MOVER_A_POSICION, 39));
    agregarCarta(new Carta("Avanza hasta Reading Railroad", ACCION_MOVER_A_POSICION, 5));
    agregarCarta(new Carta("El banco te paga dividendo de $50", ACCION_RECIBIR_DINERO, 50));
    agregarCarta(new Carta("Ve directamente a la carcel", ACCION_IR_CARCEL, 0));
    agregarCarta(new Carta("Paga impuestos de $15", ACCION_PAGAR_DINERO, 15));
    agregarCarta(new Carta("Has sido elegido presidente. Paga $50", ACCION_PAGAR_DINERO, 50));
    agregarCarta(new Carta("Tu edificio madura. Recibe $150", ACCION_RECIBIR_DINERO, 150));
    agregarCarta(new Carta("Ganas concurso. Recibe $10", ACCION_RECIBIR_DINERO, 10));
    agregarCarta(new Carta("Banco te presta. Recibe $100", ACCION_RECIBIR_DINERO, 100));
    agregarCarta(new Carta("Paga reparaciones de $50", ACCION_PAGAR_DINERO, 50));
    agregarCarta(new Carta("Avanza hasta St. Charles Place", ACCION_MOVER_A_POSICION, 11));
    agregarCarta(new Carta("Recibe intereses. Cobra $75", ACCION_RECIBIR_DINERO, 75));
    agregarCarta(new Carta("Multa de trafico. Paga $25", ACCION_PAGAR_DINERO, 25));
    agregarCarta(new Carta("Vendes acciones. Recibe $125", ACCION_RECIBIR_DINERO, 125));
    agregarCarta(new Carta("Avanza hasta Illinois Avenue", ACCION_MOVER_A_POSICION, 24));

    barajar();
}
