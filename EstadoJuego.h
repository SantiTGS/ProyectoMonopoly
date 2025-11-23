#ifndef ESTADOJUEGO_H
#define ESTADOJUEGO_H

#include <stack>
#include <vector>
#include <string>
using namespace std;

// Forward declarations
class Jugador;
class Propiedad;

/**
 * Estructura para guardar el estado de un jugador
 */
struct EstadoJugador {
    string nombre;
    int dinero;
    int posicion;
    bool enCarcel;
    int turnosEnCarcel;
    bool activo;
    vector<string> nombresPropiedades;  // Solo nombres para simplificar

    //pre: ninguna
    //post: EstadoJugador creado con valores por defecto
    EstadoJugador();
    
    //pre: j != nullptr
    //post: EstadoJugador creado con datos copiados del jugador j
    EstadoJugador(Jugador* j);
};

/**
 * Clase para representar el estado completo del juego en un momento dado
 */
class EstadoJuego {
private:
    vector<EstadoJugador> estadosJugadores;  // Estado de cada jugador
    int turnoActual;                         // Índice del jugador actual
    int casasDisponibles;                    // Casas disponibles en el banco
    int hotelesDisponibles;                  // Hoteles disponibles en el banco
    
public:
    //pre: ninguna
    //post: EstadoJuego creado vacío
    EstadoJuego();
    
    //pre: jugadores no vacío, turno >= 0, casas >= 0, hoteles >= 0
    //post: EstadoJuego creado con snapshot del estado actual del juego
    EstadoJuego(const vector<Jugador*>& jugadores, int turno, int casas, int hoteles);
    
    //pre: EstadoJuego existe
    //post: Memoria liberada
    ~EstadoJuego();
    
    // Getters
    //pre: ninguna
    //post: retorna vector con estados de jugadores
    vector<EstadoJugador> getEstadosJugadores() const;
    
    //pre: ninguna
    //post: retorna turnoActual
    int getTurnoActual() const;
    
    //pre: ninguna
    //post: retorna casasDisponibles
    int getCasasDisponibles() const;
    
    //pre: ninguna
    //post: retorna hotelesDisponibles
    int getHotelesDisponibles() const;
};

/**
 * TAD PilaEstados - Pila (Stack) de estados del juego
 * Permite deshacer jugadas guardando el estado anterior
 */
class PilaEstados {
private:
    stack<EstadoJuego> estados;  // Pila de estados
    int maxEstados;              // Límite de estados guardados
    
public:
    //pre: max > 0
    //post: PilaEstados creada vacía con maxEstados = max
    PilaEstados(int max = 10);
    
    //pre: PilaEstados existe
    //post: pila vaciada, memoria liberada
    ~PilaEstados();
    
    //pre: ninguna
    //post: estado agregado al tope de la pila, si size() > maxEstados se elimina el más antiguo
    void guardarEstado(const EstadoJuego& estado);
    
    //pre: !estados.empty()
    //post: retorna estado del tope de la pila, estado removido del tope
    EstadoJuego restaurarEstado();
    
    //pre: ninguna
    //post: retorna true si pila vacía, false en caso contrario
    bool estaVacia() const;
    
    //pre: ninguna
    //post: retorna cantidad de estados en la pila
    int size() const;
    
    //pre: ninguna
    //post: pila vaciada completamente
    void limpiar();
};

#endif
