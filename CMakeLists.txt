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
        Juego.cpp
)

add_executable(monopoly ${SOURCE_FILES})