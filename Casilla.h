#ifndef CASILLA_H
#define CASILLA_H

#include <string>
using namespace std;

class Jugador;

//Tipos de casillas
const int TIPO_SALIDA = 0;
const int TIPO_PROPIEDAD = 1;
const int TIPO_ARCA_COMUNAL = 2;
const int TIPO_CASUALIDAD = 3;
const int TIPO_IMPUESTO = 4;
const int TIPO_ESTACION = 5;
const int TIPO_SERVICIO = 6;
const int TIPO_CARCEL = 7;
const int TIPO_PARKING_GRATIS = 8;
const int TIPO_IR_A_CARCEL = 9;
//para poder ubicar las casillas, creo creo que lo usaremos en el main, si no Recordatorio: modificar 

class Casilla {
protected:
    string nombre;
    int tipo;
    int posicion;

public:
    Casilla(string nombre, int tipo, int posicion);
    virtual ~Casilla();
    virtual void accionAlCaer(Jugador* jugador) = 0;

    string getNombre() const;
    int getTipo() const;
    int getPosicion() const;
};

// ========== Casillas especificas ==========

class CasillaSalida : public Casilla {
private:
    int bonoPasar;
    int bonoLlegar;
public:
    CasillaSalida(int posicion);
    void accionAlCaer(Jugador* jugador) override; //esto se usa con las clases, lo tenemos mientras terminaos la creacion del tood codigo peusoto que ayuda a verificar donde hay un error espcifoco
    int getBonoPasar() const;
};

class CasillaParking : public Casilla {
public:
    CasillaParking(int posicion);
    void accionAlCaer(Jugador* jugador) override;
};

class CasillaCarcel : public Casilla {
public:
    CasillaCarcel(int posicion);
    void accionAlCaer(Jugador* jugador) override;
};

class CasillaIrCarcel : public Casilla {
public:
    CasillaIrCarcel(int posicion);
    void accionAlCaer(Jugador* jugador) override;
};

class CasillaImpuesto : public Casilla {
private:
    int monto;
public:
    CasillaImpuesto(string nombre, int posicion, int monto);
    void accionAlCaer(Jugador* jugador) override;
    int getMonto() const;
};

class CasillaArcaComunal : public Casilla {
public:
    CasillaArcaComunal(int posicion);
    void accionAlCaer(Jugador* jugador) override;
};

class CasillaCasualidad : public Casilla {
public:
    CasillaCasualidad(int posicion);
    void accionAlCaer(Jugador* jugador) override;
};

#endif