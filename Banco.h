#ifndef BANCO_H
#define BANCO_H

#include "Propiedad.h"
#include "Jugador.h"
#include <vector>
#include <string>
using namespace std;


class Banco {
private:
    int dineroTotal;
    int casasDisponibles;
    int hotelesDisponibles;
    int bonoPasarSalida;
    vector<Propiedad*> propiedades;

public:
    Banco(int dineroInicial = 1000000); //el banco tiene dinero ilimitado, por lo que aqui solo pusimos un dato grande para que el juego continue sin interrrupciones
    ~Banco();

    // el como se maneja el dinero
    void pagarAJugador(Jugador* jugador, int monto);
    void recibirDeJugador(Jugador* jugador, int monto);
    void pagarBonoPasarSalida(Jugador* jugador);//el dinero que el banco le paga al jugador al pasar por la casilla de salida
    void pagarBonoCaerSalida(Jugador* jugador);//si el jugador cae directamente en la casilla de salida tambien recibe el mismo bono

    // Propiedades
    bool venderPropiedad(Propiedad* propiedad, Jugador* jugador);
    bool comprarPropiedad(Propiedad* propiedad, Jugador* jugador);

    void registrarPropiedad(Propiedad* propiedad);
    Propiedad* buscarPropiedadPorNombre(string nombre);
    Propiedad* buscarPropiedadPorPosicion(int posicion);

    vector<Propiedad*> obtenerPropiedadesDe(Jugador* jugador);
    vector<Propiedad*> obtenerPropiedadesDisponibles();

    bool venderCasa(Jugador* jugador, Propiedad* propiedad);
    bool venderHotel(Jugador* jugador, Propiedad* propiedad);

    // Bancarrota y estado del jugagor 
    void gestionarBancarrota(Jugador* jugador, Jugador* prestador = nullptr); //si esta en quiebra pierde y debe dar todo lo que tiene:( 
//la cosa es que prestador--> si cae en alguna propiedad y no $ le debes al jugador-> prestador 
//Si quiebras, todo lo que tenías se le pasa al prestador este puede ser juagdor si no hay jugador -> banco:)

    int getDinero() const;
    int getCasasDisponibles() const;
    int getHotelesDisponibles() const;

    void mostrarEstado() const;
    void mostrarPropiedadesDisponibles() const;
    int calcularFortuna(Jugador* jugador) const;
};

#endif