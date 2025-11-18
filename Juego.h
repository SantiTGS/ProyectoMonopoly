#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Tablero.h"
#include "Banco.h"
#include <vector>
#include <string>
using namespace std;


class Juego {
private:
    vector<Jugador*> jugadores;
    Tablero* tablero;
    Banco* banco;
   

    int turnoActual;
    bool juegoTerminado;

    int dado1;
    int dado2;

    
    //Pre: Ninguna
    //Pos: dado1 y dado2 tienen valores entre 1 y 6
     
    int lanzarDados();

public: 
    Juego();
    ~Juego(); //aqui  un destructor


    void inicializar();

    void agregarJugador(string nombre);

    void iniciar();

    void procesandoElTurno(); //procesa un turno

    void tirarDados();
    void comprarPropiedad();
    void construirCasa(string nombrePropiedad);
    void construirHotel(string nombrePropiedad);
    void hipotecar(string nombrePropiedad);
    void mostrarEstado();
    void mostrarPropiedades();
    void terminarTurno();
    bool haTerminado()

    Jugador* getJugadorActual();


    void mostrarGanador();
};

#endif