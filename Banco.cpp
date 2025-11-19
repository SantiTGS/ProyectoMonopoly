#include "Banco.h"
#include "Propiedad.h"
#include <iostream>
using namespace std;

// ========== CONSTRUCTOR Y DESTRUCTOR ==========

Banco::Banco(int dineroInicial)
    : dineroTotal(dineroInicial),
      casasDisponibles(32),
      hotelesDisponibles(12),
      bonoPasarSalida(200) {
    cout << "\n[BANCO] Inicializado con $" << dineroTotal << endl;
    cout << "[BANCO] Casas disponibles: 32 | Hoteles disponibles: 12\n" << endl;
}

Banco::~Banco() {
    propiedades.clear();
}

// ========== TRANSACCIONES DE DINERO ==========

void Banco::pagarAJugador(Jugador* jugador, int monto) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }

    // Validar monto
    if (monto <= 0) {
        return;
    }

    // El banco paga (resta su dinero, suma al jugador)
    dineroTotal -= monto;
    jugador->recibirDinero(monto);
    cout << "[BANCO] Paga $" << monto << " a " << jugador->getNombre() << endl;
}

void Banco::recibirDeJugador(Jugador* jugador, int monto) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }

    // Validar monto
    if (monto <= 0) {
        return;
    }

    // El jugador intenta pagar al banco
    bool pudoPagar = jugador->pagarDinero(monto);

    if (pudoPagar) {
        // Si pudo pagar, el banco recibe el dinero
        dineroTotal += monto;
        cout << "[BANCO] Recibe $" << monto << " de "
             << jugador->getNombre() << endl;
    } else {
        // No pudo pagar
        cout << "[BANCO] " << jugador->getNombre()
             << " no pudo pagar!" << endl;
    }
}

void Banco::pagarBonoPasarSalida(Jugador* jugador) {
    if (jugador != nullptr) {
        pagarAJugador(jugador, bonoPasarSalida);  // $200
    }
}

void Banco::pagarBonoCaerSalida(Jugador* jugador) {
    if (jugador != nullptr) {
        pagarAJugador(jugador, bonoPasarSalida * 2);  // $400
    }
}

// ========== GESTIÓN DE PROPIEDADES ==========

void Banco::registrarPropiedad(Propiedad* propiedad) {
    // Validar que la propiedad exista
    if (propiedad == nullptr) {
        return;
    }

    // Agregar a la lista de propiedades del banco
    propiedades.push_back(propiedad);
}

Propiedad* Banco::buscarPropiedadPorNombre(string nombre) {
    // Buscar en todas las propiedades
    for (int i = 0; i < propiedades.size(); i++) {
        Propiedad* prop = propiedades[i];

        // ¿El nombre coincide?
        if (prop->getNombre() == nombre) {
            return prop;  // Encontrada!
        }
    }

    // No se encontró
    return nullptr;
}

Propiedad* Banco::buscarPropiedadPorPosicion(int posicion) {
    // Buscar en todas las propiedades
    for (int i = 0; i < propiedades.size(); i++) {
        Propiedad* prop = propiedades[i];

        // ¿La posición coincide?
        if (prop->getPosicion() == posicion) {
            return prop;  // Encontrada!
        }
    }

    // No se encontró
    return nullptr;
}

bool Banco::venderPropiedad(Propiedad* propiedad, Jugador* jugador) {
    // 1. Validar parámetros
    if (propiedad == nullptr) {
        cout << "[BANCO] Propiedad invalida" << endl;
        return false;
    }

    if (jugador == nullptr) {
        cout << "[BANCO] Jugador invalido" << endl;
        return false;
    }

    // 2. ¿Ya tiene dueño?
    if (!propiedad->estaDisponible()) {
        cout << "[BANCO] " << propiedad->getNombre()
             << " ya tiene dueno" << endl;
        return false;
    }

    // 3. ¿Tiene suficiente dinero?
    int precio = propiedad->getPrecioCompra();
    int dineroJugador = jugador->getDinero();

    if (dineroJugador < precio) {
        cout << "[BANCO] " << jugador->getNombre()
             << " no tiene suficiente dinero" << endl;
        cout << "[BANCO] Necesita: $" << precio
             << " | Tiene: $" << dineroJugador << endl;
        return false;
    }

    // 4. Realizar la venta
    bool pudoPagar = jugador->pagarDinero(precio);

    if (pudoPagar) {
        // El banco recibe el dinero
        dineroTotal += precio;

        // Asignar propiedad al jugador (SIN cobrar de nuevo)
        propiedad->asignarDueno(jugador);

        cout << "[BANCO] " << jugador->getNombre() << " compra "
             << propiedad->getNombre() << " por $" << precio << endl;
        return true;
    }

    return false;
}

bool Banco::comprarPropiedad(Propiedad* propiedad, Jugador* jugador) {
    // 1. Validar parámetros
    if (propiedad == nullptr) {
        return false;
    }

    if (jugador == nullptr) {
        return false;
    }

    // 2. ¿Es el dueño?
    Jugador* dueno = propiedad->getDueno();

    if (dueno != jugador) {
        cout << "[BANCO] " << jugador->getNombre()
             << " no es dueno de " << propiedad->getNombre() << endl;
        return false;
    }

    // 3. Calcular valor de hipoteca (50% del precio)
    int precioCompra = propiedad->getPrecioCompra();
    int valorHipoteca = precioCompra / 2;

    // 4. El banco compra la propiedad
    pagarAJugador(jugador, valorHipoteca);
    propiedad->liberar();

    cout << "[BANCO] Compra " << propiedad->getNombre()
         << " por $" << valorHipoteca << " (hipoteca)" << endl;

    return true;
}

vector<Propiedad*> Banco::obtenerPropiedadesDe(Jugador* jugador) {
    vector<Propiedad*> resultado;

    // Validar jugador
    if (jugador == nullptr) {
        return resultado;  // Vector vacío
    }

    // Buscar todas las propiedades de este jugador
    for (int i = 0; i < propiedades.size(); i++) {
        Propiedad* prop = propiedades[i];
        Jugador* dueno = prop->getDueno();

        // ¿Esta propiedad es del jugador?
        if (dueno == jugador) {
            resultado.push_back(prop);
        }
    }

    return resultado;
}

vector<Propiedad*> Banco::obtenerPropiedadesDisponibles() {
    vector<Propiedad*> resultado;

    // Buscar propiedades sin dueño
    for (int i = 0; i < propiedades.size(); i++) {
        Propiedad* prop = propiedades[i];

        // ¿Está disponible?
        if (prop->estaDisponible()) {
            resultado.push_back(prop);
        }
    }

    return resultado;
}

// ========== CONSTRUCCIÓN ==========

bool Banco::venderCasa(Jugador* jugador, Propiedad* propiedad) {
    // 1. Validar parámetros
    if (jugador == nullptr || propiedad == nullptr) {
        return false;
    }

    // 2. ¿Hay casas disponibles?
    if (casasDisponibles <= 0) {
        cout << "[BANCO] No hay casas disponibles" << endl;
        return false;
    }

    // 3. ¿Es el dueño?
    if (propiedad->getDueno() != jugador) {
        cout << "[BANCO] No eres dueno de " << propiedad->getNombre() << endl;
        return false;
    }

    // 4. Intentar construir (la propiedad valida dinero, límites, etc.)
    bool construido = propiedad->construirCasa();

    if (construido) {
        // Reducir inventario de casas
        casasDisponibles--;
        cout << "[BANCO] Casa vendida! Quedan: "
             << casasDisponibles << " casas" << endl;
        return true;
    }

    return false;
}

bool Banco::venderHotel(Jugador* jugador, Propiedad* propiedad) {
    // 1. Validar parámetros
    if (jugador == nullptr || propiedad == nullptr) {
        return false;
    }

    // 2. ¿Hay hoteles disponibles?
    if (hotelesDisponibles <= 0) {
        cout << "[BANCO] No hay hoteles disponibles" << endl;
        return false;
    }

    // 3. ¿Es el dueño?
    if (propiedad->getDueno() != jugador) {
        cout << "[BANCO] No eres dueno de " << propiedad->getNombre() << endl;
        return false;
    }

    // 4. Intentar construir hotel
    bool construido = propiedad->construirHotel();

    if (construido) {
        // Reducir hoteles y devolver 4 casas al banco
        hotelesDisponibles--;
        casasDisponibles += 4;

        cout << "[BANCO] Hotel vendido! Quedan: "
             << hotelesDisponibles << " hoteles" << endl;
        return true;
    }

    return false;
}

// ========== BANCARROTA ==========

void Banco::gestionarBancarrota(Jugador* jugador, Jugador* prestador) {
    // Validar jugador
    if (jugador == nullptr) {
        return;
    }

    cout << "\n*** " << jugador->getNombre() << " ESTA EN BANCARROTA ***" << endl;

    // Obtener todas las propiedades del jugador
    vector<Propiedad*> propiedadesJugador = jugador->getPropiedades();

    // ¿Quiebra contra el banco o contra otro jugador?
    if (prestador == nullptr) {
        // Caso 1: Quiebra contra el banco
        cout << "Todas sus propiedades vuelven al banco" << endl;

        // Liberar cada propiedad
        for (int i = 0; i < propiedadesJugador.size(); i++) {
            Propiedad* prop = propiedadesJugador[i];
            prop->liberar();
        }

    } else {
        // Caso 2: Quiebra contra otro jugador
        cout << "Todas sus propiedades van a "
             << prestador->getNombre() << endl;

        // Transferir cada propiedad
        for (int i = 0; i < propiedadesJugador.size(); i++) {
            Propiedad* prop = propiedadesJugador[i];

            // Primero liberar del dueño anterior
            prop->liberar();

            // Luego asignar al nuevo dueño (SIN cobrar)
            prop->asignarDueno(prestador);
        }
    }

    // Declarar al jugador en quiebra
    jugador->declararQuiebra();
}

// ========== GETTERS ==========

int Banco::getDinero() const {
    return dineroTotal;
}

int Banco::getCasasDisponibles() const {
    return casasDisponibles;
}

int Banco::getHotelesDisponibles() const {
    return hotelesDisponibles;
}

// ========== INFORMACIÓN ==========

void Banco::mostrarEstado() const {
    cout << "\n===== ESTADO DEL BANCO =====" << endl;
    cout << "Dinero:   $" << dineroTotal << endl;
    cout << "Casas:    " << casasDisponibles << endl;
    cout << "Hoteles:  " << hotelesDisponibles << endl;
    cout << "============================" << endl;
}

void Banco::mostrarPropiedadesDisponibles() const {
    cout << "\n----- PROPIEDADES DISPONIBLES -----" << endl;

    int contador = 0;

    // Mostrar cada propiedad disponible
    for (int i = 0; i < propiedades.size(); i++) {
        Propiedad* prop = propiedades[i];

        // ¿Está disponible?
        if (prop->estaDisponible()) {
            cout << "  * " << prop->getNombre()
                 << " - $" << prop->getPrecioCompra() << endl;
            contador++;
        }
    }

    // Si no hay ninguna disponible
    if (contador == 0) {
        cout << "  (Todas las propiedades tienen dueno)" << endl;
    }

    cout << "-----------------------------------" << endl;
}

int Banco::calcularFortuna(Jugador* jugador) const {
    // Validar jugador
    if (jugador == nullptr) {
        return 0;
    }

    // 1. Dinero en efectivo
    int total = jugador->getDinero();

    // 2. Valor de propiedades y construcciones
    vector<Propiedad*> props = jugador->getPropiedades();

    for (int i = 0; i < props.size(); i++) {
        Propiedad* prop = props[i];

        // Valor de la propiedad
        int valorPropiedad = prop->getPrecioCompra();
        total += valorPropiedad;

        // Valor de casas
        int numCasas = prop->getNumCasas();
        int precioCasa = prop->getPrecioCasa();
        int valorCasas = numCasas * precioCasa;
        total += valorCasas;

        // Valor de hoteles (vale como 5 casas)
        int numHoteles = prop->getNumHoteles();
        int valorHoteles = numHoteles * (precioCasa * 5);
        total += valorHoteles;
    }

    return total;
}