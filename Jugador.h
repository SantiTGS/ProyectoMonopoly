#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>
using namespace std;


class Propiedad;

class Jugador {
private:
    string nombre;
    int dinero;
    int posicion;
    vector<Propiedad*> propiedades;
    bool enCarcel;
    int turnosEnCarcel;
    int turnosSinCarcel; // Turnos con pares para salir de carcel
    bool activo; // Si aun está en el juego (no en quiebra)
    
public:

    Jugador(string nombre, int dineroInicial = 1500);
    
  //deestrucutor se agrega por is algo 
    ~Jugador();
    
    // ========== MOVIMIENTO ==========
  
    void mover(int pasos);
    

    void moverA(int nuevaPosicion);
    
    // ========== DINERO ==========

    void recibirDinero(int monto);
    

    bool pagarDinero(int monto);
    
 
    void pagarRenta(int monto, Jugador* dueno);
    
    // ========== PROPIEDADES ==========
 
    void agregarPropiedad(Propiedad* prop);
    
  
    void removerPropiedad(Propiedad* prop);
    
  
    vector<Propiedad*> getPropiedades() const;
    
  
    int contarPropiedades() const;
    
    // ========== CÁRCEL ==========
  
    void irACarcel();
    void salirDeCarcel();
    void incrementarTurnosCarcel();
    bool estaEnCarcel() const;
    

    int getTurnosEnCarcel() const;
    
    // ========== QUIEBRA bancarrota ==========

    void declararQuiebra();
    bool estaActivo() const;
    
    // ========== GETTERS ==========
    string getNombre() const;
    int getDinero() const;
    int getPosicion() const;
    
    // ========== INFORMACION ==========

    void mostrarEstado() const;
};

#endif