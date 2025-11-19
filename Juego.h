#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Tablero.h"
#include "Banco.h"
#include "MazoCartas.h"
#include "EstadoJuego.h"  // CAMBIO: Incluir pila de estados
#include <vector>
#include <string>
using namespace std;

/**
 * Controlador principal del juego Monopoly
 */
class Juego {
private:
    vector<Jugador*> jugadores;
    Tablero* tablero;
    Banco* banco;
    MazoCartas* arcaComunal;
    MazoCartas* casualidad;
    PilaEstados* pilaEstados;  // CAMBIO: Pila para deshacer jugadas

    int turnoActual;
    bool juegoTerminado;

    int dado1;
    int dado2;

    //Lanza los dados
    //pre: ninguna
    //post: dado1 y dado2 tienen valores entre 1 y 6, retorna suma de ambos dados
    int lanzarDados();

public:
    //pre: ninguna
    //post: Juego creado con turnoActual = 0, juegoTerminado = false, punteros inicializados en nullptr, pilaEstados inicializada
    Juego();

    //pre: Juego existe
    //post: Memoria de jugadores, tablero, banco, mazos y pilaEstados liberada
    ~Juego();

    //pre: Juego creado
    //post: tablero, banco, mazos de cartas y pilaEstados inicializados, propiedades registradas en banco (tabla hash)
    void inicializar();

    //pre: nombre no vacío, jugadores.size() < 8
    //post: nuevo jugador agregado al vector jugadores con $1500
    void agregarJugador(string nombre);

    //pre: jugadores.size() >= 2, inicializar() ejecutado
    //post: turnoActual = 0, mensaje de inicio mostrado, estado inicial guardado en pila
    void iniciar();

    //pre: !juegoTerminado
    //post: turno del jugador actual procesado (cárcel verificada, mensaje mostrado)
    void procesarTurno();

    //pre: jugador actual no está en cárcel
    //post: dados lanzados, estado guardado en pila, jugador movido, acción de casilla ejecutada
    void tirarDados();

    //pre: jugador actual en casilla de tipo propiedad disponible, jugador tiene dinero suficiente
    //post: si exitoso, estado guardado en pila, propiedad comprada y asignada a jugador, dinero deducido
    void comprarPropiedad();

    //pre: nombrePropiedad existe en tabla hash, jugador actual es dueño, casasDisponibles > 0
    //post: si exitoso, estado guardado en pila, casa construida en propiedad, dinero deducido, casasDisponibles--
    void construirCasa(string nombrePropiedad);

    //pre: nombrePropiedad existe en tabla hash, jugador actual es dueño, hotelesDisponibles > 0
    //post: si exitoso, estado guardado en pila, hotel construido en propiedad, dinero deducido, hotelesDisponibles--
    void construirHotel(string nombrePropiedad);

    //pre: nombrePropiedad existe en tabla hash, jugador actual es dueño, propiedad sin construcciones
    //post: estado guardado en pila, propiedad hipotecada, jugador recibe valorHipoteca
    void hipotecar(string nombrePropiedad);

    // CAMBIO: Nueva función para deshacer jugada
    //pre: pilaEstados no está vacía
    //post: estado anterior restaurado desde la pila, jugadores y banco actualizados
    void deshacerJugada();

    //pre: ninguna
    //post: estado de todos los jugadores activos e información del turno mostrada
    void mostrarEstado();

    //pre: jugador actual tiene propiedades
    //post: lista de propiedades del jugador actual mostrada
    void mostrarPropiedades();

    //pre: ninguna
    //post: turnoActual avanza al siguiente jugador, si solo queda 1 activo entonces juegoTerminado = true
    void terminarTurno();

    //pre: ninguna
    //post: retorna true si juegoTerminado == true, false en caso contrario
    bool haTerminado();

    //pre: jugadores.size() > 0
    //post: retorna puntero al jugador en turnoActual
    Jugador* getJugadorActual();

    //pre: juegoTerminado == true
    //post: ganador con mayor fortuna mostrado
    void mostrarGanador();

private:
    // CAMBIO: Funciones auxiliares para la pila de estados
    //pre: ninguna
    //post: estado actual del juego guardado en pilaEstados
    void guardarEstadoActual();
    
    //pre: estado válido
    //post: jugadores y banco restaurados al estado dado
    void restaurarEstado(const EstadoJuego& estado);
};

#endif
