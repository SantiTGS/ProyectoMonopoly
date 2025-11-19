#ifndef BANCO_H
#define BANCO_H

#include "Propiedad.h"
#include "Jugador.h"
#include <vector>
#include <string>
using namespace std;

/**
 * Clase Banco - Gestiona el dinero y las propiedades del juego
 *
 * Responsabilidades:
 * - Pagar y recibir dinero de jugadores
 * - Controlar la compra/venta de propiedades
 * - Gestionar casas y hoteles disponibles
 * - Manejar bancarrotas
 */
class Banco {
private:
    int dineroTotal;          // Dinero del banco (ilimitado en la práctica)
    int casasDisponibles;     // Máximo 32 casas
    int hotelesDisponibles;   // Máximo 12 hoteles
    int bonoPasarSalida;      // Bono al pasar por Salida: $200

    vector<Propiedad*> propiedades;  // Todas las propiedades del juego

public:
    // Constructor con dinero inicial del banco
    //pre: dineroInicial >= 0
    //post: Banco creado con dineroInicial, 32 casas, 12 hoteles, bonoPasarSalida = 200
    Banco(int dineroInicial = 1000000);

    //pre: Banco existe
    //post: Memoria liberada, vector de propiedades vaciado
    ~Banco();

    // ===== TRANSACCIONES DE DINERO =====
    //pre: jugador != nullptr, monto > 0
    //post: dineroTotal -= monto, jugador recibe monto
    void pagarAJugador(Jugador* jugador, int monto);

    //pre: jugador != nullptr, monto > 0
    //post: si jugador pudo pagar, dineroTotal += monto
    void recibirDeJugador(Jugador* jugador, int monto);

    //pre: jugador != nullptr
    //post: jugador recibe bonoPasarSalida ($200)
    void pagarBonoPasarSalida(Jugador* jugador);

    //pre: jugador != nullptr
    //post: jugador recibe bonoPasarSalida * 2 ($400)
    void pagarBonoCaerSalida(Jugador* jugador);

    // ===== GESTIÓN DE PROPIEDADES =====
    //pre: propiedad != nullptr
    //post: propiedad agregada al vector de propiedades
    void registrarPropiedad(Propiedad* propiedad);

    //pre: nombre es un string válido
    //post: retorna puntero a propiedad con ese nombre, o nullptr si no existe
    Propiedad* buscarPropiedadPorNombre(string nombre);

    //pre: 0 <= posicion < 40
    //post: retorna puntero a propiedad en esa posición, o nullptr si no existe
    Propiedad* buscarPropiedadPorPosicion(int posicion);

    // Compra/venta (jugador <-> banco)
    //pre: propiedad != nullptr, jugador != nullptr, propiedad disponible, jugador tiene dinero suficiente
    //post: retorna true si venta exitosa, jugador paga precio, propiedad asignada a jugador, dineroTotal aumenta
    bool venderPropiedad(Propiedad* propiedad, Jugador* jugador);

    //pre: propiedad != nullptr, jugador != nullptr, jugador es dueño de propiedad
    //post: retorna true, banco paga valorHipoteca (precio/2) a jugador, propiedad liberada
    bool comprarPropiedad(Propiedad* propiedad, Jugador* jugador);

    // Consultas
    //pre: jugador != nullptr
    //post: retorna vector con todas las propiedades donde jugador es dueño
    vector<Propiedad*> obtenerPropiedadesDe(Jugador* jugador);

    //pre: ninguna
    //post: retorna vector con todas las propiedades sin dueño
    vector<Propiedad*> obtenerPropiedadesDisponibles();

    // ===== CONSTRUCCIÓN =====
    //pre: jugador != nullptr, propiedad != nullptr, jugador es dueño, casasDisponibles > 0
    //post: retorna true si construcción exitosa, casasDisponibles--, jugador paga precio de casa
    bool venderCasa(Jugador* jugador, Propiedad* propiedad);

    //pre: jugador != nullptr, propiedad != nullptr, jugador es dueño, hotelesDisponibles > 0
    //post: retorna true si construcción exitosa, hotelesDisponibles--, casasDisponibles += 4, jugador paga precio de hotel
    bool venderHotel(Jugador* jugador, Propiedad* propiedad);

    // ===== BANCARROTA =====
    //pre: jugador != nullptr
    //post: si prestador == nullptr, propiedades liberadas al banco; si prestador != nullptr, propiedades transferidas a prestador; jugador declarado en quiebra
    void gestionarBancarrota(Jugador* jugador, Jugador* prestador = nullptr);

    // ===== GETTERS =====
    //pre: ninguna
    //post: retorna dineroTotal
    int getDinero() const;

    //pre: ninguna
    //post: retorna casasDisponibles
    int getCasasDisponibles() const;

    //pre: ninguna
    //post: retorna hotelesDisponibles
    int getHotelesDisponibles() const;

    // ===== INFORMACIÓN =====
    //pre: ninguna
    //post: imprime estado del banco (dinero, casas, hoteles)
    void mostrarEstado() const;

    //pre: ninguna
    //post: imprime lista de propiedades sin dueño
    void mostrarPropiedadesDisponibles() const;

    //pre: jugador != nullptr
    //post: retorna fortuna total (dinero + valor de propiedades + construcciones)
    int calcularFortuna(Jugador* jugador) const;
};

#endif