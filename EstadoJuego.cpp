#include "EstadoJuego.h"
#include "Jugador.h"
#include "Propiedad.h"
#include <iostream>
using namespace std;

// ========== IMPLEMENTACION DE ESTADOJUGADOR ==========

EstadoJugador::EstadoJugador() 
    : nombre(""), dinero(0), posicion(0), enCarcel(false), 
      turnosEnCarcel(0), activo(true) {
}

EstadoJugador::EstadoJugador(Jugador* j) {
    if (j == nullptr) {
        nombre = "";
        dinero = 0;
        posicion = 0;
        enCarcel = false;
        turnosEnCarcel = 0;
        activo = false;
        return;
    }
    
    // Copiar datos del jugador
    nombre = j->getNombre();
    dinero = j->getDinero();
    posicion = j->getPosicion();
    enCarcel = j->estaEnCarcel();
    turnosEnCarcel = j->getTurnosEnCarcel();
    activo = j->estaActivo();
    
    // Guardar solo nombres de propiedades (para simplificar)
    vector<Propiedad*> props = j->getPropiedades();
    for (Propiedad* p : props) {
        nombresPropiedades.push_back(p->getNombre());
    }
}

// ========== IMPLEMENTACION DE ESTADOJUEGO ==========

EstadoJuego::EstadoJuego() 
    : turnoActual(0), casasDisponibles(32), hotelesDisponibles(12) {
}

EstadoJuego::EstadoJuego(const vector<Jugador*>& jugadores, int turno, int casas, int hoteles)
    : turnoActual(turno), casasDisponibles(casas), hotelesDisponibles(hoteles) {
    
    // Guardar estado de cada jugador
    for (Jugador* j : jugadores) {
        estadosJugadores.push_back(EstadoJugador(j));
    }
}

EstadoJuego::~EstadoJuego() {
    estadosJugadores.clear();
}

vector<EstadoJugador> EstadoJuego::getEstadosJugadores() const {
    return estadosJugadores;
}

int EstadoJuego::getTurnoActual() const {
    return turnoActual;
}

int EstadoJuego::getCasasDisponibles() const {
    return casasDisponibles;
}

int EstadoJuego::getHotelesDisponibles() const {
    return hotelesDisponibles;
}

// ========== IMPLEMENTACION DE PILAESTADOS ==========

PilaEstados::PilaEstados(int max) : maxEstados(max) {
    cout << "[PILA] Inicializada con limite de " << maxEstados << " estados" << endl;
}

PilaEstados::~PilaEstados() {
    limpiar();
}

void PilaEstados::guardarEstado(const EstadoJuego& estado) {
    // OPCIÓN CON LÍMITE: Mantiene los últimos maxEstados
    // Cuando se llena, elimina el más antiguo automáticamente
    
    if (estados.size() >= maxEstados) {
        // Crear pila temporal para reorganizar
        stack<EstadoJuego> temp;
        
        // Mover todos excepto el primero (más antiguo) a temp
        int count = estados.size() - 1;
        for (int i = 0; i < count; i++) {
            temp.push(estados.top());
            estados.pop();
        }
        
        // Eliminar el más antiguo
        estados.pop();
        
        // Restaurar los estados
        while (!temp.empty()) {
            estados.push(temp.top());
            temp.pop();
        }
        
        cout << "[PILA] Limite alcanzado, estado mas antiguo eliminado" << endl;
    }
    
    estados.push(estado);
    cout << "[PILA] Estado guardado (" << estados.size() << "/" << maxEstados << ")" << endl;
}

EstadoJuego PilaEstados::restaurarEstado() {
    if (estados.empty()) {
        cout << "[PILA] No hay estados para restaurar" << endl;
        return EstadoJuego();  // Retornar estado vacío
    }
    
    EstadoJuego estado = estados.top();
    estados.pop();
    
    cout << "[PILA] Estado restaurado (quedan " << estados.size() << " estados)" << endl;
    return estado;
}

bool PilaEstados::estaVacia() const {
    return estados.empty();
}

int PilaEstados::size() const {
    return estados.size();
}

void PilaEstados::limpiar() {
    while (!estados.empty()) {
        estados.pop();
    }
    cout << "[PILA] Estados limpiados" << endl;
}
