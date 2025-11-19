#include "Propiedad.h"
#include "Jugador.h"
#include <iostream>
using namespace std;

// ========== CONSTRUCTORES ==========

// Constructor para CALLES (con color y posibilidad de construcción)
Propiedad::Propiedad(string nombre, int posicion, int precio, int renta,
                     int color, int precioCasa)
    : Casilla(nombre, TIPO_PROPIEDAD, posicion),
      tipoPropiedad(PROP_CALLE),
      color(color),
      precioCompra(precio),
      rentaBase(renta),
      dueno(nullptr),
      hipotecada(false),
      numCasas(0),
      numHoteles(0),
      precioCasa(precioCasa) {
}

// Constructor para ESTACIONES y SERVICIOS
Propiedad::Propiedad(string nombre, int posicion, int precio, int tipo)
    : Casilla(nombre, (tipo == PROP_ESTACION ? TIPO_ESTACION : TIPO_SERVICIO), posicion),
      tipoPropiedad(tipo),
      color(COLOR_SIN_COLOR),
      precioCompra(precio),
      rentaBase(25),
      dueno(nullptr),
      hipotecada(false),
      numCasas(0),
      numHoteles(0),
      precioCasa(0) {
}

Propiedad::~Propiedad() {
}

// ========== ACCIÓN AL CAER ==========

void Propiedad::accionAlCaer(Jugador* jugador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }

    cout << "\n>>> " << jugador->getNombre()
         << " cayo en " << nombre << endl;

    // Caso 1: Propiedad disponible
    if (estaDisponible()) {
        cout << "    Disponible por $" << precioCompra << endl;
        cout << "    Usa 'comprar' para adquirirla" << endl;
        return;
    }

    // Caso 2: Es del jugador
    if (dueno == jugador) {
        cout << "    Esta es tu propiedad!" << endl;
        return;
    }

    // Caso 3: Es de otro jugador
    if (hipotecada) {
        cout << "    Esta hipotecada, no pagas renta" << endl;
        return;
    }

    // Cobrar renta
    int renta = calcularRenta();
    cout << "    Debes pagar renta de $" << renta
         << " a " << dueno->getNombre() << endl;

    jugador->pagarRenta(renta, dueno);
}

// ========== COMPRA Y VENTA ==========

void Propiedad::asignarDueno(Jugador* jugador) {
    // SOLO asignar dueño, NO cobrar dinero
    // El cobro lo hace el Banco

    if (jugador == nullptr) {
        return;
    }

    // Asignar dueño
    dueno = jugador;

    // Agregar propiedad al jugador
    jugador->agregarPropiedad(this);
}

bool Propiedad::estaDisponible() const {
    return (dueno == nullptr);
}

void Propiedad::liberar() {
    // Si tiene dueño, remover la propiedad de su lista
    if (dueno != nullptr) {
        dueno->removerPropiedad(this);
        dueno = nullptr;
    }

    // Limpiar construcciones
    numCasas = 0;
    numHoteles = 0;
    hipotecada = false;
}

// ========== RENTA ==========

int Propiedad::calcularRenta() const {
    // Si está hipotecada, no cobra
    if (hipotecada) {
        return 0;
    }

    // Solo las calles tienen construcciones
    if (tipoPropiedad != PROP_CALLE) {
        return rentaBase;
    }

    // Calcular según construcciones
    if (numHoteles > 0) {
        // Hotel multiplica x50
        return rentaBase * 50;
    }

    if (numCasas > 0) {
        // Cada casa aumenta la renta
        return rentaBase * (numCasas + 1);
    }

    // Sin construcciones
    return rentaBase;
}

// ========== CONSTRUCCIÓN ==========

bool Propiedad::construirCasa() {
    // Validar que sea una calle
    if (tipoPropiedad != PROP_CALLE) {
        cout << "Solo se pueden construir casas en calles" << endl;
        return false;
    }

    // Validar que no esté hipotecada
    if (hipotecada) {
        cout << "No puedes construir en propiedades hipotecadas" << endl;
        return false;
    }

    // Validar límite de casas
    if (numCasas >= 4) {
        cout << "Ya tienes 4 casas. Construye un hotel" << endl;
        return false;
    }

    // Validar que el dueño tenga dinero
    if (dueno == nullptr) {
        return false;
    }

    int dineroDisponible = dueno->getDinero();

    if (dineroDisponible < precioCasa) {
        cout << "No tienes suficiente dinero para construir" << endl;
        cout << "Necesitas: $" << precioCasa
             << " | Tienes: $" << dineroDisponible << endl;
        return false;
    }

    // Construir casa
    bool pudoPagar = dueno->pagarDinero(precioCasa);

    if (pudoPagar) {
        numCasas++;
        cout << "Casa construida en " << nombre
             << ". Total: " << numCasas << " casa(s)" << endl;
        return true;
    }

    return false;
}

bool Propiedad::construirHotel() {
    // Validar que sea una calle
    if (tipoPropiedad != PROP_CALLE) {
        cout << "Solo se pueden construir hoteles en calles" << endl;
        return false;
    }

    // Validar que no esté hipotecada
    if (hipotecada) {
        cout << "No puedes construir en propiedades hipotecadas" << endl;
        return false;
    }

    // Validar que tenga 4 casas
    if (numCasas < 4) {
        cout << "Necesitas 4 casas antes de construir un hotel" << endl;
        return false;
    }

    // Validar que no tenga hotel ya
    if (numHoteles >= 1) {
        cout << "Ya tienes un hotel en esta propiedad" << endl;
        return false;
    }

    // Validar dinero (hotel cuesta 5x el precio de una casa)
    if (dueno == nullptr) {
        return false;
    }

    int precioHotel = precioCasa * 5;
    int dineroDisponible = dueno->getDinero();

    if (dineroDisponible < precioHotel) {
        cout << "No tienes suficiente dinero para el hotel" << endl;
        cout << "Necesitas: $" << precioHotel
             << " | Tienes: $" << dineroDisponible << endl;
        return false;
    }

    // Construir hotel
    bool pudoPagar = dueno->pagarDinero(precioHotel);

    if (pudoPagar) {
        numCasas = 0;      // Las casas se convierten en hotel
        numHoteles = 1;
        cout << "Hotel construido en " << nombre << "!" << endl;
        return true;
    }

    return false;
}

// ========== HIPOTECA ==========

void Propiedad::hipotecar() {
    // Validar que no esté ya hipotecada
    if (hipotecada) {
        cout << nombre << " ya esta hipotecada" << endl;
        return;
    }

    // Validar que no tenga construcciones
    if (numCasas > 0 || numHoteles > 0) {
        cout << "Debes vender las construcciones antes de hipotecar" << endl;
        return;
    }

    // Hipotecar (vale la mitad del precio)
    hipotecada = true;
    int valorHipoteca = precioCompra / 2;

    if (dueno != nullptr) {
        dueno->recibirDinero(valorHipoteca);
    }

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

// ========== INFORMACIÓN ==========

void Propiedad::mostrarInfo() const {
    cout << "\n========================================" << endl;
    cout << "  " << nombre << endl;
    cout << "========================================" << endl;

    // Tipo
    if (tipoPropiedad == PROP_CALLE) {
        cout << "Tipo:   Calle " << getNombreColor() << endl;
    } else if (tipoPropiedad == PROP_ESTACION) {
        cout << "Tipo:   Estacion de Tren" << endl;
    } else {
        cout << "Tipo:   Servicio Publico" << endl;
    }

    // Precio
    cout << "Precio: $" << precioCompra << endl;
    cout << "Renta:  $" << rentaBase;

    if (numCasas > 0 || numHoteles > 0) {
        int rentaActual = calcularRenta();
        cout << " (Actual: $" << rentaActual << ")";
    }
    cout << endl;

    // Dueño y estado
    if (dueno != nullptr) {
        cout << "\nDueno:  " << dueno->getNombre() << endl;

        if (numCasas > 0) {
            cout << "Casas:  " << numCasas << " casa(s)" << endl;
        }

        if (numHoteles > 0) {
            cout << "Hotel:  SI" << endl;
        }

        if (hipotecada) {
            cout << "\n*** HIPOTECADA ***" << endl;
        }
    } else {
        cout << "\nEstado: DISPONIBLE PARA COMPRA" << endl;
    }

    cout << "========================================\n" << endl;
}