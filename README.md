# Monopoly - Implementación con Estructuras de Datos

Un juego de Monopoly implementado en C++ que demuestra el uso práctico de múltiples estructuras de datos (TADs).

## 📋 Descripción

Este proyecto es una implementación completa del juego de mesa Monopoly, desarrollado como proyecto académico para la materia de Estructuras de Datos. El enfoque principal está en demostrar la aplicación práctica de diferentes Tipos Abstractos de Datos (TADs) en un sistema real de juego.

El juego incluye todas las mecánicas clásicas del Monopoly: compra de propiedades, construcción de casas y hoteles, cartas de Arca Comunal y Casualidad, cárcel, impuestos, y un innovador sistema de deshacer jugadas implementado con una pila.

## 🎯 Estructuras de Datos Implementadas

### 1. **Lista Circular Doblemente Enlazada** - Tablero del Juego
- **Ubicación**: `Tablero.h` / `Tablero.cpp`
- **Propósito**: Representar las 40 casillas del tablero de Monopoly
- **Características**:
  - Cada nodo contiene una casilla y punteros al siguiente y anterior
  - El último nodo conecta con el primero (circularidad)
  - El primer nodo conecta con el último (doble enlace)
  - Permite movimiento infinito en ambas direcciones
  
**Ventajas de esta estructura**:
- Simula perfectamente el tablero circular del Monopoly
- Permite avanzar y retroceder sin límites
- No hay "inicio" ni "fin" real, ideal para un juego de mesa circular
- Acceso bidireccional eficiente

**Implementación**:
```cpp
struct NodoCasilla {
    Casilla* casilla;
    NodoCasilla* siguiente;  // Apunta a la siguiente casilla
    NodoCasilla* anterior;   // Apunta a la casilla anterior
};

// El último nodo tiene siguiente apuntando al primero
// El primer nodo tiene anterior apuntando al último
```

**Optimización adicional**:
- Se incluye un `map<int, NodoCasilla*>` como índice
- Permite acceso directo O(log n) a cualquier posición
- Combina lo mejor de listas enlazadas y acceso aleatorio

### 2. **Tabla Hash (unordered_map)** - Banco de Propiedades
- **Ubicación**: `Banco.h` / `Banco.cpp`
- **Propósito**: Registro y búsqueda rápida de propiedades del juego
- **Complejidad**: O(1) promedio para búsquedas e inserciones

**¿Por qué Tabla Hash?**
- Búsqueda instantánea de propiedades por nombre
- Búsqueda instantánea de propiedades por posición
- Alternativa superior a búsqueda lineal O(n)
- Ideal para un juego con 28 propiedades distintas

**Implementación**:
```cpp
// Dos tablas hash para búsqueda dual
unordered_map<string, Propiedad*> propiedadesPorNombre;
unordered_map<int, Propiedad*> propiedadesPorPosicion;

// Búsqueda O(1) por nombre
Propiedad* buscarPropiedadPorNombre(string nombre) {
    auto it = propiedadesPorNombre.find(nombre);
    if (it != propiedadesPorNombre.end()) {
        return it->second;
    }
    return nullptr;
}

// Búsqueda O(1) por posición
Propiedad* buscarPropiedadPorPosicion(int posicion) {
    auto it = propiedadesPorPosicion.find(posicion);
    if (it != propiedadesPorPosicion.end()) {
        return it->second;
    }
    return nullptr;
}
```

**Casos de uso en el juego**:
- Buscar propiedad cuando un jugador cae en una casilla
- Encontrar propiedades para construir casas/hoteles
- Listar propiedades disponibles
- Calcular fortuna de jugadores

### 3. **Cola (Queue)** - Mazos de Cartas
- **Ubicación**: `MazoCartas.h` / `MazoCartas.cpp`
- **Propósito**: Gestionar cartas de Arca Comunal (16 cartas) y Casualidad (16 cartas)
- **Comportamiento**: FIFO (First In, First Out) con circularidad

**¿Por qué Cola?**
- Las cartas en Monopoly siguen comportamiento de cola
- Carta sacada del frente se devuelve al final
- Simula perfectamente el comportamiento del juego real
- Mezcla inicial con `random_shuffle`

**Implementación**:
```cpp
queue<Carta*> cartas;  // Cola de cartas

Carta* sacarCarta() {
    if (cartas.empty()) return nullptr;
    
    // Tomar carta del frente (FIFO)
    Carta* carta = cartas.front();
    cartas.pop();
    
    // Devolver al final (comportamiento circular)
    cartas.push(carta);
    
    return carta;
}

void barajar() {
    // Convertir a vector temporalmente
    vector<Carta*> temp;
    while (!cartas.empty()) {
        temp.push_back(cartas.front());
        cartas.pop();
    }
    
    // Mezclar aleatoriamente
    random_shuffle(temp.begin(), temp.end());
    
    // Volver a la cola
    for (Carta* c : temp) {
        cartas.push(c);
    }
}
```

**Tipos de cartas**:
- `ACCION_RECIBIR_DINERO`: El jugador recibe dinero del banco
- `ACCION_PAGAR_DINERO`: El jugador paga al banco
- `ACCION_MOVER_A_POSICION`: El jugador se mueve a una casilla específica
- `ACCION_IR_CARCEL`: El jugador va directamente a la cárcel

### 4. **Pila (Stack)** - Sistema de Deshacer Jugadas
- **Ubicación**: `EstadoJuego.h` / `EstadoJuego.cpp`
- **Propósito**: Guardar estados del juego para permitir deshacer jugadas
- **Comportamiento**: LIFO (Last In, First Out)

**¿Por qué Pila?**
- Deshacer requiere acceso al estado más reciente primero
- LIFO es perfecto para operaciones de undo/redo
- Limita memoria guardando solo últimos 10-20 estados
- Comportamiento natural e intuitivo para usuarios

**Implementación**:
```cpp
stack<EstadoJuego> estados;
int maxEstados = 10;  // Límite configurable

void guardarEstado(const EstadoJuego& estado) {
    // Si llegamos al límite, eliminar el más antiguo
    if (estados.size() >= maxEstados) {
        // Reorganizar para eliminar el fondo
        stack<EstadoJuego> temp;
        int count = estados.size() - 1;
        for (int i = 0; i < count; i++) {
            temp.push(estados.top());
            estados.pop();
        }
        estados.pop();  // Eliminar más antiguo
        
        // Restaurar
        while (!temp.empty()) {
            estados.push(temp.top());
            temp.pop();
        }
    }
    
    estados.push(estado);
}

EstadoJuego restaurarEstado() {
    if (estados.empty()) {
        return EstadoJuego();  // Estado vacío
    }
    
    EstadoJuego estado = estados.top();
    estados.pop();
    return estado;
}
```

**¿Qué se guarda en cada estado?**
```cpp
struct EstadoJugador {
    string nombre;
    int dinero;
    int posicion;
    bool enCarcel;
    int turnosEnCarcel;
    bool activo;
    vector<string> nombresPropiedades;
};

class EstadoJuego {
    vector<EstadoJugador> estadosJugadores;
    int turnoActual;
    int casasDisponibles;
    int hotelesDisponibles;
};
```

**Cuándo se guarda un estado**:
- Antes de tirar los dados
- Antes de comprar una propiedad
- Antes de construir casa o hotel
- Antes de hipotecar
- ¡El jugador puede deshacer todas estas acciones!

### 5. **Vector (STL)** - Colecciones Dinámicas
- **Ubicación**: Múltiples clases
- **Propósito**: Almacenamiento flexible de elementos con tamaño dinámico

**Usos principales**:
```cpp
// En Juego.h - Lista de jugadores
vector<Jugador*> jugadores;

// En Jugador.h - Propiedades de cada jugador
vector<Propiedad*> propiedades;

// En EstadoJuego.h - Snapshot de jugadores
vector<EstadoJugador> estadosJugadores;
```

**Ventajas del Vector**:
- Tamaño dinámico (2-4 jugadores según partida)
- Acceso aleatorio O(1) por índice
- Iteración eficiente
- Gestión automática de memoria

## 🏗️ Arquitectura del Proyecto

### Diagrama de Clases Principal

```
                          ┌─────────────┐
                          │    Juego    │
                          │ (Controller)│
                          └──────┬──────┘
                                 │
                 ┌───────────────┼───────────────┐
                 │               │               │
           ┌─────▼─────┐   ┌────▼────┐   ┌─────▼─────┐
           │  Tablero  │   │  Banco  │   │MazoCartas │
           │  (Lista)  │   │ (Hash)  │   │  (Cola)   │
           └─────┬─────┘   └────┬────┘   └───────────┘
                 │               │
           ┌─────▼─────┐   ┌────▼────┐
           │  Casilla  │   │Propiedad│
           │  (Base)   │   │         │
           └───────────┘   └─────────┘
                 
           ┌─────────────┐   ┌────────────┐
           │  Jugador    │   │PilaEstados │
           │             │   │  (Stack)   │
           └─────────────┘   └────────────┘
```

### Clases Principales

#### `Juego` - Controlador Central
**Responsabilidades**:
- Coordinar todos los componentes del juego
- Gestionar turnos de jugadores
- Procesar comandos del usuario
- Mantener pila de estados para deshacer
- Determinar condiciones de victoria/derrota

**Métodos clave**:
```cpp
void inicializar();           // Crea tablero, banco, mazos
void procesarTurno();         // Ejecuta turno del jugador actual
void tirarDados();            // Lanza dados y mueve jugador
void comprarPropiedad();      // Compra propiedad actual
void construirCasa(string);   // Construir casa en propiedad
void construirHotel(string);  // Construir hotel en propiedad
void deshacerJugada();        // Deshacer con pila (NUEVO)
void terminarTurno();         // Avanzar al siguiente jugador
```

#### `Tablero` - TAD Lista Circular Doblemente Enlazada
**Responsabilidades**:
- Mantener las 40 casillas del juego enlazadas circularmente
- Permitir navegación bidireccional infinita
- Proveer acceso rápido a casillas específicas

**Estructura interna**:
```cpp
NodoCasilla* inicio;          // Apunta a casilla 0 (Salida)
int numCasillas;              // Siempre 40 en Monopoly
map<int, NodoCasilla*> indice; // Acceso rápido O(log n)
```

**Métodos clave**:
```cpp
void crearTablero();                    // Crea las 40 casillas
Casilla* obtenerCasilla(int pos);      // Acceso por posición
Casilla* avanzarDesde(int pos, int n); // Avanzar n casillas
Casilla* retrocederDesde(int pos, int n); // Retroceder n
void mostrarTablero();                  // Visualizar tablero
```

#### `Banco` - TAD Tabla Hash
**Responsabilidades**:
- Gestionar dinero del banco (inicialmente $100,000)
- Controlar transacciones con jugadores
- Registrar y buscar propiedades (O(1) con hash)
- Gestionar inventario de casas (32) y hoteles (12)
- Manejar bancarrotas y transferencias

**Estructura interna**:
```cpp
int dineroTotal;                    // Dinero del banco
int casasDisponibles;               // Máximo 32
int hotelesDisponibles;             // Máximo 12
int bonoPasarSalida;                // $200

// TABLAS HASH para búsqueda O(1)
unordered_map<string, Propiedad*> propiedadesPorNombre;
unordered_map<int, Propiedad*> propiedadesPorPosicion;
```

**Métodos clave**:
```cpp
void pagarAJugador(Jugador*, int);     // Banco → Jugador
void recibirDeJugador(Jugador*, int);  // Jugador → Banco
void registrarPropiedad(Propiedad*);   // Agregar a hash
Propiedad* buscarPropiedadPorNombre(string); // O(1)
Propiedad* buscarPropiedadPorPosicion(int);  // O(1)
bool venderPropiedad(Propiedad*, Jugador*);
bool venderCasa(Jugador*, Propiedad*);
bool venderHotel(Jugador*, Propiedad*);
void gestionarBancarrota(Jugador*, Jugador*);
```

#### `MazoCartas` - TAD Cola (Queue)
**Responsabilidades**:
- Gestionar 16 cartas de Arca Comunal
- Gestionar 16 cartas de Casualidad
- Mezclar cartas aleatoriamente
- Comportamiento circular (carta usada va al final)

**Estructura interna**:
```cpp
queue<Carta*> cartas;  // Cola FIFO
string tipo;           // "Arca Comunal" o "Casualidad"
```

**Métodos clave**:
```cpp
void agregarCarta(Carta*);        // Agregar al final
Carta* sacarCarta();              // Sacar del frente
void barajar();                   // Mezclar aleatoriamente
void crearMazoArcaComunal();      // Crear 16 cartas
void crearMazoCasualidad();       // Crear 16 cartas
```

#### `PilaEstados` - TAD Pila (Stack)
**Responsabilidades**:
- Guardar snapshots del estado del juego
- Permitir deshacer jugadas (hasta 10 estados)
- Gestionar límite de memoria

**Estructura interna**:
```cpp
stack<EstadoJuego> estados;  // Pila LIFO
int maxEstados;              // Límite (default: 10)
```

**Métodos clave**:
```cpp
void guardarEstado(const EstadoJuego&);  // Push con límite
EstadoJuego restaurarEstado();            // Pop
bool estaVacia();
int size();
void limpiar();
```

#### `Jugador`
**Responsabilidades**:
- Gestionar dinero del jugador (inicia con $1500)
- Controlar posición en el tablero (0-39)
- Mantener lista de propiedades
- Gestionar estado de cárcel
- Detectar bancarrota

**Estructura interna**:
```cpp
string nombre;
int dinero;              // Inicialmente $1500
int posicion;            // 0-39 en el tablero
vector<Propiedad*> propiedades;
bool enCarcel;
int turnosEnCarcel;      // Máximo 3 turnos
bool activo;             // false si en quiebra
```

**Métodos clave**:
```cpp
void mover(int pasos);              // Mover y cobrar por Salida
void recibirDinero(int);            // Aumentar dinero
bool pagarDinero(int);              // Pagar si es posible
void pagarRenta(int, Jugador*);     // Pagar a otro jugador
void agregarPropiedad(Propiedad*);  // Agregar a vector
void irACarcel();                   // Enviar a cárcel
void salirDeCarcel();               // Liberar de cárcel
void declararQuiebra();             // Bancarrota
```

#### `Propiedad` (hereda de `Casilla`)
**Responsabilidades**:
- Representar propiedades comprables
- Calcular renta según construcciones
- Gestionar construcción de casas/hoteles
- Manejar hipotecas

**Tipos de propiedades**:
- **Calles** (22): Pueden tener casas y hoteles
- **Estaciones** (4): Renta fija
- **Servicios** (2): Renta según dados

**Estructura interna**:
```cpp
int tipoPropiedad;  // CALLE, ESTACION, SERVICIO
int color;          // Para agrupar calles
int precioCompra;   // Precio de compra
int rentaBase;      // Renta sin construcciones
Jugador* dueno;     // nullptr = disponible
bool hipotecada;
int numCasas;       // 0-4
int numHoteles;     // 0-1
int precioCasa;     // Costo por casa
```

**Métodos clave**:
```cpp
void asignarDueno(Jugador*);      // Asignar propiedad
bool estaDisponible();             // Verificar si tiene dueño
void liberar();                    // Volver al banco
int calcularRenta();               // Según construcciones
bool construirCasa();              // Construir casa
bool construirHotel();             // Construir hotel
void hipotecar();                  // Hipotecar al banco
```

**Cálculo de renta**:
```cpp
// Sin construcciones: rentaBase
// Con 1 casa: rentaBase * 2
// Con 2 casas: rentaBase * 3
// Con 3 casas: rentaBase * 4
// Con 4 casas: rentaBase * 5
// Con hotel: rentaBase * 50
```

#### `Casilla` (Clase Base Abstracta)
**Jerarquía de herencia**:
```
         Casilla (abstracta)
              │
    ┌─────────┼─────────────────┐
    │         │                 │
Propiedad  CasillaSalida  CasillaCarcel
            │                 │
    CasillaParking    CasillaIrCarcel
            │                 │
    CasillaImpuesto   CasillaArcaComunal
                            │
                    CasillaCasualidad
```

**Tipos de casillas especiales**:
- `CasillaSalida`: Bono de $200 al pasar, $400 al caer
- `CasillaParking`: Parking gratis (descanso)
- `CasillaCarcel`: Visita o prisión
- `CasillaIrCarcel`: Envía directamente a cárcel
- `CasillaImpuesto`: Paga monto fijo al banco
- `CasillaArcaComunal`: Saca carta de Arca
- `CasillaCasualidad`: Saca carta de Casualidad

## 🎮 Características del Juego

### Mecánicas Implementadas

#### ✅ Sistema de Turnos
- 2-4 jugadores por partida
- Rotación automática de turnos
- Validación de jugadores activos

#### ✅ Movimiento y Tablero
- 40 casillas del Monopoly clásico
- Movimiento circular infinito
- Bono de $200 al pasar por Salida
- Bono de $400 al caer en Salida

#### ✅ Propiedades
- 22 calles divididas en 8 grupos de color
- 4 estaciones de tren
- 2 servicios públicos (Eléctrica y Agua)
- Sistema de compra/venta

#### ✅ Construcción
- Hasta 4 casas por propiedad
- 1 hotel por propiedad (reemplaza 4 casas)
- Inventario limitado: 32 casas, 12 hoteles
- Construcción requiere monopolio del color

#### ✅ Renta
- Cálculo dinámico según construcciones
- Multiplicadores por casas (x2, x3, x4, x5)
- Multiplicador por hotel (x50)
- Rentas especiales para estaciones y servicios

#### ✅ Cartas
- 16 cartas de Arca Comunal
- 16 cartas de Casualidad
- Acciones: recibir dinero, pagar, mover, ir a cárcel
- Sistema de cola circular

#### ✅ Cárcel
- Casilla especial en posición 10
- Máximo 3 turnos en prisión
- Liberación automática después de 3 turnos
- Casilla "Ve a la Cárcel" en posición 30

#### ✅ Impuestos
- Impuesto sobre la Renta: $200 (posición 4)
- Impuesto de Lujo: $100 (posición 38)

#### ✅ Hipotecas
- Valor de hipoteca = 50% del precio de compra
- No se puede hipotecar con construcciones
- No se cobra renta en propiedades hipotecadas

#### ✅ Sistema de Deshacer (INNOVACIÓN)
- Pila de hasta 10 estados
- Deshacer: tiradas de dados, compras, construcciones, hipotecas
- Restauración completa del estado del juego

#### ✅ Bancarrota
- Detección automática de quiebra
- Transferencia de propiedades al acreedor
- Liberación de propiedades si quiebra contra el banco

## 🚀 Compilación y Ejecución

### Requisitos del Sistema
- **Compilador**: C++14 o superior
- **Build System**: CMake 3.26 o superior
- **Sistema Operativo**: Linux, macOS, o Windows con MinGW

### Estructura de Archivos

```
monopoly/
├── CMakeLists.txt          # Configuración de CMake
├── main.cpp                # Punto de entrada del programa
├── Juego.h / Juego.cpp     # Controlador principal
├── Tablero.h / Tablero.cpp # Lista circular doblemente enlazada
├── Banco.h / Banco.cpp     # Tabla hash de propiedades
├── MazoCartas.h / MazoCartas.cpp  # Cola de cartas
├── EstadoJuego.h / EstadoJuego.cpp  # Pila de estados
├── Jugador.h / Jugador.cpp # Gestión de jugadores
├── Propiedad.h / Propiedad.cpp  # Propiedades del juego
└── Casilla.h / Casilla.cpp # Casillas del tablero
```

### Pasos de Compilación

#### Método 1: Con CMake (Recomendado)

```bash
# 1. Crear directorio de compilación
mkdir build
cd build

# 2. Generar archivos de build
cmake ..

# 3. Compilar el proyecto
make

# 4. Ejecutar el juego
./monopoly
```

#### Método 2: Compilación Manual

```bash
g++ -std=c++14 -o monopoly \
    main.cpp \
    Juego.cpp \
    Tablero.cpp \
    Banco.cpp \
    MazoCartas.cpp \
    EstadoJuego.cpp \
    Jugador.cpp \
    Propiedad.cpp \
    Casilla.cpp

./monopoly
```

### Configuración de CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.26)
project(monopoly)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    main.cpp
    Jugador.cpp
    Casilla.cpp
    Tablero.cpp
    Propiedad.cpp
    MazoCartas.cpp
    Banco.cpp
    EstadoJuego.cpp
    Juego.cpp
)

add_executable(monopoly ${SOURCE_FILES})
```

## 🎯 Guía de Uso

### Inicio del Juego

```
========================================
         MONOPOLY - ESTRUCTURAS DE DATOS
========================================

Estructuras de datos implementadas:
  * Lista Circular Doblemente Enlazada (Tablero)
  * Tabla Hash / unordered_map (Propiedades)
  * Cola / queue (Mazos de Cartas)
  * Pila / stack (Estados del Juego)
  * Vector / STL (Jugadores)
========================================

Cuantos jugadores? (2-4): 3
Nombre del jugador 1: Alice
Nombre del jugador 2: Bob
Nombre del jugador 3: Charlie
```

### Comandos Disponibles

| Comando | Descripción | Ejemplo |
|---------|-------------|---------|
| `tirar` | Lanzar dados y mover | `tirar` |
| `comprar` | Comprar propiedad donde estás | `comprar` |
| `propiedades` | Ver tus propiedades | `propiedades` |
| `casa [nombre]` | Construir casa | `casa Baltic Avenue` |
| `hotel [nombre]` | Construir hotel | `hotel Park Place` |
| `hipotecar [nombre]` | Hipotecar propiedad | `hipotecar Boardwalk` |
| `deshacer` | **Deshacer última jugada** | `deshacer` |
| `estado` | Ver estado del juego | `estado` |
| `terminar` | Terminar tu turno | `terminar` |
| `ayuda` | Mostrar comandos | `ayuda` |
| `salir` | Salir del juego | `salir` |

### Ejemplo de Partida

```
=== Turno de Alice ===
Posicion: 0
Dinero: $1500

> tirar
Dados: [3] [4] = 7
Alice paso por la SALIDA! +$200
Caiste en: Casualidad

>>> Alice saco una carta de CASUALIDAD!
Carta Casualidad: Avanza hasta Boardwalk
Alice se mueve a la posicion 39

>>> Alice cayo en Boardwalk
    Disponible por $400
    Usa 'comprar' para adquirirla

> comprar
[BANCO] Alice compra Boardwalk por $400

> estado
=== ESTADO DEL JUEGO ===
Alice: $1300 | Pos:39 | Props:1
Bob: $1500 | Pos:0 | Props:0
Charlie: $1500 | Pos:0 | Props:0

Turno de: Alice
Estados guardados: 2/10

> terminar

=== Turno de Bob ===
...
```

### Uso del Sistema de Deshacer

```
> tirar
Dados: [5] [2] = 7
Caiste en: Park Place
Disponible por $350

> comprar
[BANCO] Bob compra Park Place por $350

> estado
Bob: $1150 | Pos:37 | Props:1

> deshacer
[DESHACER] Deshaciendo ultima jugada...
[RESTAURAR] Bob restaurado a posicion 30 con $1500
[DESHACER] Jugada deshecha exitosamente!

> estado
Bob: $1500 | Pos:30 | Props:0
```

## 📊 Análisis de Complejidad

### Tabla de Complejidades

| Operación | Estructura Usada | Complejidad Temporal | Complejidad Espacial |
|-----------|-----------------|---------------------|---------------------|
| Buscar propiedad por nombre | Tabla Hash | **O(1)** promedio | O(n) |
| Buscar propiedad por posición | Tabla Hash | **O(1)** promedio | O(n) |
| Avanzar n casillas en tablero | Lista Circular | **O(n)** | O(1) |
| Acceder casilla por índice | Map (índice adicional) | **O(log n)** | O(n) |
| Sacar carta del mazo | Cola | **O(1)** | O(n) |
| Guardar estado del juego | Pila | **O(k)** donde k=jugadores | O(m*k) donde m=estados |
| Deshacer jugada | Pila | **O(k)** donde k=jugadores | O(1) |
| Agregar jugador | Vector | **O(1)** amortizado | O(n) |
| Listar propiedades de jugador | Vector | **O(n)** | O(1) |
| Barajar mazo | Cola + Vector | **O(n log n)** | O(n) |

### Justificación de Complejidades

#### Tabla Hash - O(1)
```cpp
// Operación de búsqueda
Propiedad* prop = propiedadesPorNombre["Boardwalk"];
// Complejidad: O(1) promedio
// Mejor que búsqueda lineal O(n) en vector
```

**Ventaja**: En un juego con 28 propiedades, la diferencia es:
- Tabla Hash: 1 operación
- Búsqueda lineal: hasta 28 operaciones

#### Lista Circular - O(n) para avanzar
```cpp
// Avanzar 7 casillas
for (int i = 0; i < 7; i++) {
    actual = actual->siguiente;
}
// Complejidad: O(7) = O(n)
```

**Aceptable porque**:
- Máximo 12 en un lanzamiento de dados (6+6)
- Promedio: 7 casillas por turno
- Trade-off por beneficios de estructura circular

#### Pila - O(k) para guardar/restaurar
```cpp
// Guardar estado de k jugadores
for (Jugador* j : jugadores) {  // k iteraciones
    estadosJugadores.push_back(EstadoJugador(j));
}
// Complejidad: O(k) donde k es número de jugadores (2-4)
```

**Eficiente porque**:
- k es pequeño (máximo 4 jugadores)
- Operación no es frecuente (1 vez por acción)
- Espacio limitado (solo 10 estados)

## 📝 Precondiciones y Postcondiciones

Todas las funciones incluyen documentación formal con especificación de contratos:

### Ejemplo: Jugador

```cpp
//pre: nombre no vacío, dineroInicial >= 0
//post: Jugador creado con nombre, dineroInicial, posicion = 0, 
//      enCarcel = false, activo = true
Jugador(string nombre, int dineroInicial = 1500);

//pre: pasos >= 0
//post: posicion = (posicion + pasos) % 40, 
//      si pasó por Salida recibe $200
void mover(int pasos);

//pre: monto > 0
//post: si dinero >= monto, dinero -= monto y retorna true; 
//      sino retorna false
bool pagarDinero(int monto);
```

### Ejemplo: Banco

```cpp
//pre: propiedad != nullptr
//post: propiedad agregada a ambas tablas hash 
//      (por nombre y por posición)
void registrarPropiedad(Propiedad* propiedad);

//pre: propiedad != nullptr, jugador != nullptr, 
//     propiedad disponible, jugador tiene dinero suficiente
//post: retorna true si venta exitosa, jugador paga precio, 
//      propiedad asignada a jugador, dineroTotal aumenta
bool venderPropiedad(Propiedad* propiedad, Jugador* jugador);
```

### Ejemplo: Pila de Estados

```cpp
//pre: ninguna
//post: estado agregado al tope de la pila, 
//      si size() > maxEstados se elimina el más antiguo
void guardarEstado(const EstadoJuego& estado);

//pre: !estados.empty()
//post: retorna estado del tope de la pila, 
//      estado removido del tope
EstadoJuego restaurarEstado();
```

## 🎓 Conceptos de Estructuras de Datos Demostrados

### 1. Lista Circular Doblemente Enlazada

**Concepto**: Estructura lineal donde cada nodo apunta al siguiente y al anterior, y el último nodo conecta con el primero.

**Aplicación en Monopoly**:
- Simula el tablero circular del juego
- Permite movimiento infinito sin condiciones especiales
- Navegación bidireccional (adelante y atrás)

**Ventajas sobre otras estructuras**:
- Array: No permite circularidad natural
- Lista simple: No permite retroceso eficiente
- Lista no circular: Requiere lógica adicional para dar la vuelta

### 2. Tabla Hash (Hash Table)

**Concepto**: Estructura que mapea claves a valores usando una función hash, permitiendo acceso O(1).

**Aplicación en Monopoly**:
- Búsqueda rápida de propiedades por nombre
- Búsqueda rápida por posición en tablero
- Evita búsquedas lineales costosas

**Función hash utilizada**:
```cpp
// std::hash para strings y enteros
unordered_map usa internamente:
hash<string>()("Boardwalk") → índice en tabla
hash<int>()(39) → índice en tabla
```

### 3. Cola (Queue)

**Concepto**: Estructura FIFO (First In, First Out) - primero en entrar, primero en salir.

**Aplicación en Monopoly**:
- Gestión de mazos de cartas
- Carta del frente se saca y se devuelve al final
- Simula comportamiento real del juego de mesa

**Comportamiento circular**:
```cpp
// Ciclo infinito de cartas
Sacar → Usar → Devolver al final → Sacar → ...
```

### 4. Pila (Stack)

**Concepto**: Estructura LIFO (Last In, First Out) - último en entrar, primero en salir.

**Aplicación en Monopoly**:
- Sistema de deshacer (undo)
- Estado más reciente se restaura primero
- Historial limitado para gestión de memoria

**Patrón de uso**:
```cpp
// Antes de acción
Estado e1 = capturarEstado();
pila.push(e1);

// Realizar acción
comprarPropiedad();

// Si quiere deshacer
Estado anterior = pila.pop();
restaurarEstado(anterior);
```

### 5. Vector Dinámico

**Concepto**: Array de tamaño variable que crece automáticamente.

**Aplicación en Monopoly**:
- Lista de jugadores (2-4, variable)
- Propiedades de cada jugador (crece al comprar)
- Snapshots de estado

**Ventaja sobre arrays estáticos**:
```cpp
// Flexible
vector<Jugador*> jugadores;  // Tamaño dinámico

// vs Array estático
Jugador* jugadores[4];  // Tamaño fijo, desperdicia memoria
```

## 🔬 Casos de Prueba

### Test 1: Sistema de Deshacer

```
Acción: Tirar dados, comprar propiedad
Esperado: Poder deshacer y volver al estado anterior

> tirar
Dados: [2] [3] = 5
Caiste en: Reading Railroad ($200)

> comprar
Comprado. Dinero: $1300

> deshacer
Restaurado. Dinero: $1500, Posición: 0
```

### Test 2: Búsqueda en Tabla Hash

```
Acción: Buscar propiedad por nombre
Complejidad esperada: O(1)

> casa Boardwalk
Buscando "Boardwalk" en tabla hash...
Encontrado en 1 operación (O(1))
Casa construida exitosamente.
```

### Test 3: Circularidad del Tablero

```
Acción: Avanzar desde posición 38 con dados 5
Esperado: Llegar a posición 3 y cobrar $200

Posición: 38
> tirar
Dados: [2] [3] = 5
Posición nueva: (38 + 5) % 40 = 3
>>> Paso por la SALIDA! +$200
Caiste en: Baltic Avenue
```

### Test 4: Cola de Cartas Circular

```
Acción: Sacar 17 cartas del mazo (16 + 1)
Esperado: Carta 17 = Carta 1 (circularidad)

Carta 1: "Error bancario a tu favor"
Carta 2: "Paga gastos médicos"
...
Carta 16: "Multa de $75"
Carta 17: "Error bancario a tu favor" ← Misma que carta 1
```

### Test 5: Límite de Pila

```
Acción: Guardar 12 estados (límite: 10)
Esperado: Eliminar los 2 más antiguos

Estado 1: guardado
Estado 2: guardado
...
Estado 10: guardado
Estado 11: guardado, Estado 1 eliminado
Estado 12: guardado, Estado 2 eliminado

Estados en pila: [3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
```

## 🐛 Manejo de Errores

### Validaciones Implementadas

```cpp
// 1. Validación de punteros nulos
if (jugador == nullptr) {
    return;  // Evita segmentation fault
}

// 2. Validación de dinero suficiente
if (dinero < monto) {
    cout << "Dinero insuficiente" << endl;
    return false;
}

// 3. Validación de propiedades
if (!propiedad->estaDisponible()) {
    cout << "Propiedad no disponible" << endl;
    return false;
}

// 4. Validación de inventario
if (casasDisponibles <= 0) {
    cout << "No hay casas disponibles" << endl;
    return false;
}

// 5. Validación de pila vacía
if (estados.empty()) {
    cout << "No hay estados para deshacer" << endl;
    return EstadoJuego();
}
```
## 👥 Información del Proyecto

### Contexto Académico

- **Materia**: Estructuras de Datos
- **Profesor**: Gerardo M. Sarria M.
- **Institución**: Universidad Javeriana
- **Semestre**: 2024-2

### Objetivos de Aprendizaje Cumplidos

✅ Implementar Lista Circular Doblemente Enlazada desde cero
✅ Utilizar Tabla Hash (unordered_map) de STL eficientemente
✅ Implementar Cola (Queue) con comportamiento circular
✅ Implementar Pila (Stack) para sistema de deshacer
✅ Integrar múltiples TADs en un sistema cohesivo
✅ Documentar con precondiciones y postcondiciones
✅ Análisis de complejidad temporal y espacial
✅ Gestión de memoria dinámica (punteros)

### Criterios de Evaluación

| Criterio | Peso | Cumplimiento |
|----------|------|--------------|
| Lista Circular Doblemente Enlazada | 25% | ✅ Implementada |
| Tabla Hash para búsqueda O(1) | 20% | ✅ Implementada |
| Cola para mazos de cartas | 15% | ✅ Implementada |
| Pila para sistema de deshacer | 15% | ✅ Implementada |
| Integración de estructuras | 15% | ✅ Completa |
| Documentación y comentarios | 10% | ✅ Completa |

## 📚 Referencias y Recurso

### Documentación STL
- [cppreference.com - unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)
- [cppreference.com - queue](https://en.cppreference.com/w/cpp/container/queue)
- [cppreference.com - stack](https://en.cppreference.com/w/cpp/container/stack)
- [cppreference.com - vector](https://en.cppreference.com/w/cpp/container/vector)

### Reglas de Monopoly
- Hasbro Official Monopoly Rules (Versión clásica)

## 📄 Licencia

Este proyecto es de código abierto para fines educativos.

---

**Monopoly - Estructuras de Datos**  
*Desarrollado con fines académicos*  

**Este .readme fue elaborado con ayuda de inteligencia artificial.
