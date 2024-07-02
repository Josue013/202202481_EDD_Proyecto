#ifndef NODO_H
#define NODO_H

#include <string>
using namespace std;
class NodoMD {
public:
    string vuelo;
    string ciudad_destino;
    string numero_de_id;
    NodoMD* derecha;
    NodoMD* izquierda;
    NodoMD* abajo;
    NodoMD* arriba;

    NodoMD(string vuelo, string ciudad_destino, string numero_de_id)
        : vuelo(vuelo), ciudad_destino(ciudad_destino), numero_de_id(numero_de_id), 
          derecha(nullptr), izquierda(nullptr), abajo(nullptr), arriba(nullptr) {}
};

#endif // NODO_H