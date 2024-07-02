#ifndef NODO_H
#define NODO_H

#include <string>

class NodoMD {
public:
    std::string vuelo;
    std::string ciudad_destino;
    std::string numero_de_id;
    NodoMD* derecha;
    NodoMD* izquierda;
    NodoMD* abajo;
    NodoMD* arriba;

    NodoMD(std::string vuelo, std::string ciudad_destino, std::string numero_de_id)
        : vuelo(vuelo), ciudad_destino(ciudad_destino), numero_de_id(numero_de_id), 
          derecha(nullptr), izquierda(nullptr), abajo(nullptr), arriba(nullptr) {}
};

#endif 