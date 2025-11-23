#include "Juego.h"
#include "Propiedad.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Juego::Juego() : turnoActual(0), juegoTerminado(false), dado1(0), dado2(0) {
    tablero = nullptr;
    banco = nullptr;
    arcaComunal = nullptr;
    casualidad = nullptr;
    pilaEstados = nullptr;
}

Juego::~Juego() {
    for (Jugador* j : jugadores) {
        delete j;
    }
    delete tablero;
    delete banco;
    delete arcaComunal;
    delete casualidad;
    delete pilaEstados;
}

void Juego::inicializar() {
    srand(time(0));

    cout << "Inicializando Monopoly..." << endl;

    tablero = new Tablero();
    banco = new Banco();
    pilaEstados = new PilaEstados(20);

    arcaComunal = new MazoCartas("Arca Comunal");
    arcaComunal->crearMazoArcaComunal();

    casualidad = new MazoCartas("Casualidad");
    casualidad->crearMazoCasualidad();

    // Registrar propiedades del tablero en el banco (TABLA HASH)
    for (int i = 0; i < 40; i++) {
        Casilla* c = tablero->obtenerCasilla(i);
        int tipo = c->getTipo();
        
        if (tipo == TIPO_PROPIEDAD || tipo == TIPO_ESTACION || tipo == TIPO_SERVICIO) {
            Propiedad* prop = dynamic_cast<Propiedad*>(c);
            if (prop) {
                banco->registrarPropiedad(prop);
            }
        }
    }

    cout << "Juego listo!" << endl;
}

void Juego::agregarJugador(string nombre) {
    Jugador* j = new Jugador(nombre, 60);
    jugadores.push_back(j);
    cout << nombre << " se unio al juego" << endl;
}

void Juego::iniciar() {
    cout << "\nInicia el juego con " << jugadores.size() << " jugadores!" << endl;
    turnoActual = 0;
    
//guardar estado inicial
    guardarEstadoActual();
}

void Juego::procesarTurno() {
    Jugador* actual = getJugadorActual();

    if (!actual->estaActivo()) {
        terminarTurno();
        return;
    }

    cout << "\n=== Turno de " << actual->getNombre() << " ===" << endl;
    cout << "Posicion: " << actual->getPosicion() << endl;
    cout << "Dinero: $" << actual->getDinero() << endl;

    if (actual->estaEnCarcel()) {
        cout << actual->getNombre() << " esta en carcel (turno "
             << actual->getTurnosEnCarcel() << "/3)" << endl;

        actual->incrementarTurnosCarcel();

        if (actual->getTurnosEnCarcel() >= 3) {
            cout << actual->getNombre() << " sale de la carcel" << endl;
            actual->salirDeCarcel();
        }
    }
}

int Juego::lanzarDados() {
    dado1 = 1;
    dado2 = 2;
    return dado1 + dado2;
}

void Juego::tirarDados() {
    Jugador* actual = getJugadorActual();

    if (actual->estaEnCarcel()) {
        cout << "No puedes tirar, estas en carcel" << endl;
        return;
    }

    // estado antes de tirar
    guardarEstadoActual();

    int total = lanzarDados();
    cout << "Dados: [" << dado1 << "] [" << dado2 << "] = " << total << endl;

    if (dado1 == dado2) {
        cout << "Dobles!" << endl;
    }

    actual->mover(total);

    Casilla* casilla = tablero->obtenerCasilla(actual->getPosicion());
    cout << "Caiste en: " << casilla->getNombre() << endl;

    // Ejecutar accion de la casilla
    int tipo = casilla->getTipo();

    if (tipo == TIPO_PROPIEDAD || tipo == TIPO_ESTACION || tipo == TIPO_SERVICIO) {
        Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
        if (prop) {
            if (prop->estaDisponible()) {
                cout << "Disponible por $" << prop->getPrecioCompra() << endl;
                cout << "Usa 'comprar' para adquirirla" << endl;
            } else if (prop->getDueno() != actual) {
                int renta = prop->calcularRenta();
                cout << "Pagas renta de $" << renta << " a "
                     << prop->getDueno()->getNombre() << endl;
                actual->pagarRenta(renta, prop->getDueno());
            } else {
                cout << "Es tu propiedad" << endl;
            }
        }
    } else if (tipo == TIPO_ARCA_COMUNAL) {
        Carta* carta = arcaComunal->sacarCarta();
        if (carta) {
            cout << "Carta Arca Comunal: " << carta->getDescripcion() << endl;
            carta->ejecutar(actual);
        }
    } else if (tipo == TIPO_CASUALIDAD) {
        Carta* carta = casualidad->sacarCarta();
        if (carta) {
            cout << "Carta Casualidad: " << carta->getDescripcion() << endl;
            carta->ejecutar(actual);
        }
    } else {
        casilla->accionAlCaer(actual);
    }
}

void Juego::comprarPropiedad() {
    //Guardar estado antes de comprar
    guardarEstadoActual();
    
    Jugador* actual = getJugadorActual();
    Casilla* casilla = tablero->obtenerCasilla(actual->getPosicion());
    
    int tipo = casilla->getTipo();

    if (tipo != TIPO_PROPIEDAD && tipo != TIPO_ESTACION && tipo != TIPO_SERVICIO) {
        cout << "No es una propiedad" << endl;
        return;
    }

    Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
    if (!prop) return;

    if (!prop->estaDisponible()) {
        cout << "Ya tiene dueno" << endl;
        return;
    }

    banco->venderPropiedad(prop, actual);
}

void Juego::construirCasa(string nombrePropiedad) {
    // Guardar estado antes de construir
    guardarEstadoActual();
    
    Jugador* actual = getJugadorActual();
    
    // Búsqueda O(1) en tabla hash
    Propiedad* prop = banco->buscarPropiedadPorNombre(nombrePropiedad);

    if (!prop) {
        cout << "Propiedad no encontrada" << endl;
        return;
    }

    if (prop->getDueno() != actual) {
        cout << "No eres dueno" << endl;
        return;
    }

    banco->venderCasa(actual, prop);
}

void Juego::construirHotel(string nombrePropiedad) {
    // Guardar estado antes de construir
    guardarEstadoActual();
    
    Jugador* actual = getJugadorActual();
    
    // Búsqueda O(1) en tabla hash
    Propiedad* prop = banco->buscarPropiedadPorNombre(nombrePropiedad);

    if (!prop) {
        cout << "Propiedad no encontrada" << endl;
        return;
    }

    if (prop->getDueno() != actual) {
        cout << "No eres dueno" << endl;
        return;
    }

    banco->venderHotel(actual, prop);
}

void Juego::hipotecar(string nombrePropiedad) {
    // Guardar estado antes de hipotecar
    guardarEstadoActual();
    
    Jugador* actual = getJugadorActual();
    
    // Búsqueda O(1) en tabla hash
    Propiedad* prop = banco->buscarPropiedadPorNombre(nombrePropiedad);

    if (!prop) {
        cout << "Propiedad no encontrada" << endl;
        return;
    }

    if (prop->getDueno() != actual) {
        cout << "No eres dueno" << endl;
        return;
    }

    prop->hipotecar();
}

//Nueva función para deshacer jugada
void Juego::deshacerJugada() {
    if (pilaEstados->estaVacia()) {
        cout << "\n[DESHACER] No hay jugadas para deshacer" << endl;
        return;
    }
    
    cout << "\n[DESHACER] Deshaciendo ultima jugada..." << endl;
    
    EstadoJuego estadoAnterior = pilaEstados->restaurarEstado();
    restaurarEstado(estadoAnterior);
    
    cout << "[DESHACER] Jugada deshecha exitosamente!" << endl;
}

void Juego::mostrarEstado() {
    cout << "\n=== ESTADO DEL JUEGO ===" << endl;

    for (Jugador* j : jugadores) {
        if (j->estaActivo()) {
            cout << j->getNombre() << ": $" << j->getDinero()
                 << " | Pos:" << j->getPosicion()
                 << " | Props:" << j->contarPropiedades() << endl;
        }
    }

    cout << "\nTurno de: " << getJugadorActual()->getNombre() << endl;
    cout << "Estados guardados: " << pilaEstados->size() << "/10" << endl;  // CAMBIO: Mostrar pila
}

void Juego::mostrarPropiedades() {
    Jugador* actual = getJugadorActual();
    vector<Propiedad*> props = actual->getPropiedades();

    if (props.empty()) {
        cout << "No tienes propiedades" << endl;
        return;
    }

    cout << "\nTus propiedades:" << endl;
    for (Propiedad* p : props) {
        cout << "  - " << p->getNombre() << " | Casas:" << p->getNumCasas()
             << " | Hoteles:" << p->getNumHoteles() << endl;
    }
}

void Juego::terminarTurno() {
    turnoActual = (turnoActual + 1) % jugadores.size();

    // Verificar si solo queda un jugador activo
    int activos = 0;
    for (Jugador* j : jugadores) {
        if (j->estaActivo()) activos++;
    }

    if (activos <= 1) {
        juegoTerminado = true;
    }
}

bool Juego::haTerminado() {
    return juegoTerminado;
}

Jugador* Juego::getJugadorActual() {
    return jugadores[turnoActual];
}

void Juego::mostrarGanador() {
    int maxPatrimonio = 0;
    Jugador* ganador = nullptr;

    for (Jugador* j : jugadores) {
        if (j->estaActivo()) {
            int patrimonio = banco->calcularFortuna(j);
            if (patrimonio > maxPatrimonio) {
                maxPatrimonio = patrimonio;
                ganador = j;
            }
        }
    }

    if (ganador) {
        cout << "\n=== GANADOR ===" << endl;
        cout << ganador->getNombre() << " gana con $" << maxPatrimonio << endl;
    }
}

// ========== FUNCIONES PRIVADAS PARA MANEJO DE ESTADO ==========

void Juego::guardarEstadoActual() {
    EstadoJuego estadoActual(jugadores, turnoActual, 
                             banco->getCasasDisponibles(), 
                             banco->getHotelesDisponibles());
    pilaEstados->guardarEstado(estadoActual);
}

void Juego::restaurarEstado(const EstadoJuego& estado) {
    // Restaurar turno
    turnoActual = estado.getTurnoActual();
    
    // Restaurar estado de jugadores
    vector<EstadoJugador> estadosJugadores = estado.getEstadosJugadores();
    
    for (int i = 0; i < jugadores.size() && i < estadosJugadores.size(); i++) {
        Jugador* jugador = jugadores[i];
        EstadoJugador& estadoJug = estadosJugadores[i];
        
        // APLICAR los cambios al jugador
        jugador->setDinero(estadoJug.dinero);
        jugador->setPosicion(estadoJug.posicion);
        jugador->setEstadoCarcel(estadoJug.enCarcel, estadoJug.turnosEnCarcel);
        jugador->setActivo(estadoJug.activo);
        
        cout << "[RESTAURAR] " << jugador->getNombre() 
             << " restaurado a posicion " << estadoJug.posicion 
             << " con $" << estadoJug.dinero << endl;
    }

}
