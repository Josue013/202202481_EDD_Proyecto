#ifndef RUTA_H
#define RUTA_H
#include <string>
using namespace std;


class Ruta {
public:
    Ruta(const string &origen, const string &destino, int distancia): origen(origen), destino(destino), distancia(distancia) {}

    string getOrigen() const { return origen; }
    string getDestino() const { return destino; }
    int getDistancia() const { return distancia; }

    ~Ruta() {}

private:
    string origen;
    string destino;
    int distancia;
};

#endif RUTA_H