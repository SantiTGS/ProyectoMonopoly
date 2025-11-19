#include "Propiedad.h"
#include "Jugador.h"
#include <iostream>

// Constructor para calles
Propiedad::Propiedad(string nombre, int posicion, int precio, int renta,
                     int color, int precioCasa)
    : Casilla(nombre, TIPO_PROPIEDAD, posicion),
      tipoPropiedad(PROP_CALLE), color(color), precioCompra(precio),
      rentaBase(renta), dueno(nullptr), hipotecada(false),
      numCasas(0), numHoteles(0), precioCasa(precioCasa) {
}

// Constructor para estaciones y servicios
Propiedad::Propiedad(string nombre, int posicion, int precio, int tipo)
    : Casilla(nombre, (tipo == PROP_ESTACION ? TIPO_ESTACION : TIPO_SERVICIO), posicion),
      tipoPropiedad(tipo), color(COLOR_SIN_COLOR), precioCompra(precio),
      rentaBase(25), dueno(nullptr), hipotecada(false),
      numCasas(0), numHoteles(0), precioCasa(0) {
}

Propiedad::~Propiedad() {
}

void Propiedad::accionAlCaer(Jugador* jugador) {
    cout << jugador->getNombre() << " cayo en " << nombre << endl;

    if (estaDisponible()) {
        cout << "Esta propiedad esta disponible por $" << precioCompra << endl;
        cout << "Usa el comando 'comprar' para adquirirla." << endl;
    } else if (dueno == jugador) {
        cout << "Esta es tu propiedad!" << endl;
    } else {
        if (hipotecada) {
            cout << "Esta propiedad esta hipotecada, no pagas renta." << endl;
        } else {
            int renta = calcularRenta();
            cout << "Debes pagar renta de $" << renta << " a " << dueno->getNombre() << endl;
            jugador->pagarRenta(renta, dueno);
        }
    }
}

// ========== COMPRA Y VENTA ==========
void Propiedad::comprar(Jugador* jugador) {
    if (!estaDisponible()) {
        cout << "Esta propiedad ya tiene dueno." << endl;
        return;
    }

    if (jugador->getDinero() < precioCompra) {
        cout << "No tienes suficiente dinero." << endl;
        return;
    }

    if (jugador->pagarDinero(precioCompra)) {
        dueno = jugador;
        jugador->agregarPropiedad(this);
        cout << jugador->getNombre() << " compro " << nombre << " por $" << precioCompra << endl;
    }
}

bool Propiedad::estaDisponible() const {
    return dueno == nullptr;
}

void Propiedad::liberar() {
    if (dueno != nullptr) {
        dueno->removerPropiedad(this);
        dueno = nullptr;
        numCasas = 0;
        numHoteles = 0;
        hipotecada = false;
    }
}

// ========== RENTA ==========
int Propiedad::calcularRenta() const {
    if (hipotecada) return 0;

    if (tipoPropiedad == PROP_CALLE) {
        if (numHoteles > 0) {
            return rentaBase * 50;
        } else if (numCasas > 0) {
            return rentaBase * (numCasas + 1);
        } else {
            return rentaBase;
        }
    } else {
        return rentaBase;
    }
}

// ========== CONSTRUCCION (SIMPLIFICADO) ==========
bool Propiedad::construirCasa() {
    if (tipoPropiedad != PROP_CALLE) {
        cout << "Solo se pueden construir casas en calles." << endl;
        return false;
    }

    if (numCasas >= 4) {
        cout << "Ya tienes 4 casas. Construye un hotel." << endl;
        return false;
    }

    if (hipotecada) {
        cout << "No puedes construir en propiedades hipotecadas." << endl;
        return false;
    }

    if (dueno->getDinero() < precioCasa) {
        cout << "No tienes suficiente dinero para construir una casa." << endl;
        return false;
    }

    if (dueno->pagarDinero(precioCasa)) {
        numCasas++;
        cout << "Casa construida en " << nombre << ". Total: " << numCasas << " casas." << endl;
        return true;
    }

    return false;
}

bool Propiedad::construirHotel() {
    if (tipoPropiedad != PROP_CALLE) {
        cout << "Solo se pueden construir hoteles en calles." << endl;
        return false;
    }

    if (numCasas < 4) {
        cout << "Necesitas 4 casas antes de construir un hotel." << endl;
        return false;
    }

    if (numHoteles >= 1) {
        cout << "Ya tienes un hotel en esta propiedad." << endl;
        return false;
    }

    int precioHotel = precioCasa * 5;
    
    if (dueno->getDinero() < precioHotel) {
        cout << "No tienes suficiente dinero para construir un hotel." << endl;
        return false;
    }

    if (dueno->pagarDinero(precioHotel)) {
        numCasas = 0;
        numHoteles = 1;
        cout << "Hotel construido en " << nombre << "!" << endl;
        return true;
    }

    return false;
}

// ========== HIPOTECA (SIMPLIFICADO) ==========
void Propiedad::hipotecar() {
    if (hipotecada) {
        cout << nombre << " ya esta hipotecada." << endl;
        return;
    }

    if (numCasas > 0 || numHoteles > 0) {
        cout << "Debes vender las construcciones antes de hipotecar." << endl;
        return;
    }

    hipotecada = true;
    int valorHipoteca = precioCompra / 2;
    dueno->recibirDinero(valorHipoteca);
    cout << nombre << " hipotecada por $" << valorHipoteca << endl;
}

bool Propiedad::estaHipotecada() const {
    return hipotecada;
}

// ========== GETTERS ==========
int Propiedad::getTipoPropiedad() const {
    return tipoPropiedad;
}

int Propiedad::getColor() const {
    return color;
}

int Propiedad::getPrecioCompra() const {
    return precioCompra;
}

int Propiedad::getRentaBase() const {
    return rentaBase;
}

Jugador* Propiedad::getDueno() const {
    return dueno;
}

int Propiedad::getNumCasas() const {
    return numCasas;
}

int Propiedad::getNumHoteles() const {
    return numHoteles;
}

int Propiedad::getPrecioCasa() const {
    return precioCasa;
}

string Propiedad::getNombreColor() const {
    if (color == COLOR_MARRON) return "Marron";
    if (color == COLOR_CELESTE) return "Celeste";
    if (color == COLOR_ROSA) return "Rosa";
    if (color == COLOR_NARANJA) return "Naranja";
    if (color == COLOR_ROJO) return "Rojo";
    if (color == COLOR_AMARILLO) return "Amarillo";
    if (color == COLOR_VERDE) return "Verde";
    if (color == COLOR_AZUL) return "Azul";
    return "Sin color";
}

void Propiedad::mostrarInfo() const {
    cout << "\n===== " << nombre << " =====" << endl;
    
    if (tipoPropiedad == PROP_CALLE) {
        cout << "Tipo: Calle" << endl;
        cout << "Color: " << getNombreColor() << endl;
    } else if (tipoPropiedad == PROP_ESTACION) {
        cout << "Tipo: Estacion" << endl;
    } else {
        cout << "Tipo: Servicio" << endl;
    }

    cout << "Precio: $" << precioCompra << endl;
    cout << "Renta base: $" << rentaBase << endl;

    if (dueno != nullptr) {
        cout << "Dueno: " << dueno->getNombre() << endl;
        
        if (numCasas > 0) {
            cout << "Casas: " << numCasas << endl;
        }
        
        if (numHoteles > 0) {
            cout << "Hoteles: " << numHoteles << endl;
        }
        
        if (hipotecada) {
            cout << "Estado: HIPOTECADA" << endl;
        }
    } else {
        cout << "Estado: Disponible para compra" << endl;
    }

    cout << "=======================" << endl;
}
