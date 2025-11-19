#ifndef MAZOCARTAS_H
#define MAZOCARTAS_H

#include <vector>
#include <string>
using namespace std;

class Jugador;

// Tipos de accion de las cartas
const int ACCION_RECIBIR_DINERO = 0;
const int ACCION_PAGAR_DINERO = 1;
const int ACCION_MOVER_A_POSICION = 2;
const int ACCION_IR_CARCEL = 3;

/**
 * @brief Carta del juego
 */
class Carta {
private:
    string descripcion;
    int tipoAccion;
    int valor;

public:
    Carta(string desc, int tipo, int val = 0);
    ~Carta();
    
    void ejecutar(Jugador* jugador);
    
    string getDescripcion() const;
    int getTipoAccion() const;
    int getValor() const;
};

/**
 * @brief TAD MazoCartas - Vector Simple
 * Gestiona las cartas de Arca Comunal o Casualidad.
 */
class MazoCartas {
private:
    vector<Carta*> cartas;
    string tipo;
    int indiceActual;

public:
    MazoCartas(string tipoDeMazo);
    ~MazoCartas();
    
    void agregarCarta(Carta* carta);
    Carta* sacarCarta();
    void barajar();
    string getTipo() const;
    
    void crearMazoArcaComunal();
    void crearMazoCasualidad();
};

#endif
