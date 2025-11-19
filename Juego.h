#ifndef JUEGO_H
#define JUEGO_H

#include "Jugador.h"
#include "Tablero.h"
#include "Banco.h"
#include "MazoCartas.h"
#include <vector>
#include <string>
using namespace std;


//Controlador principal del juego Monopoly

class Juego {
private:
    vector<Jugador*> jugadores;
    Tablero* tablero;
    Banco* banco;
    MazoCartas* arcaComunal;
    MazoCartas* casualidad;

    int turnoActual;
    bool juegoTerminado;

    int dado1;
    int dado2;


     //Lanza los dados
     //Suma de ambos dados
     //Precondicion: Ninguna
     //Postcondicion: dado1 y dado2 tienen valores entre 1 y 6

    int lanzarDados();

public:
    //Constructor
    Juego();


 //Destructor
    ~Juego();

    //Inicializa el juego
    void inicializar();


    //Agrega un jugador al juego
    void agregarJugador(string nombre);


//Inicia el juego
    void iniciar();


//Procesa un turno completo
    void procesarTurno();


//Lanza los dados y mueve al jugador
    void tirarDados();


//Compra la propiedad actual
    void comprarPropiedad();

    //Construye casa en una propiedad
    void construirCasa(string nombrePropiedad);


    //Construye hotel en una propiedad
    void construirHotel(string nombrePropiedad);


 //Hipoteca una propiedad
    void hipotecar(string nombrePropiedad);


 //Muestra el estado del juego
    void mostrarEstado();


    //Muestra propiedades del jugador actual
    void mostrarPropiedades();


     //Termina el turno actual
    void terminarTurno();


//Verifica si el juego termino
    bool haTerminado();


     //Obtiene el jugador actual
    Jugador* getJugadorActual();


//Muestra el ganador
    void mostrarGanador();
};

#endif