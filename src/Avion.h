#ifndef AVION_H
#define AVION_H
#include <string>
using namespace std;


class Avion {
public:
    // Constructor
    Avion(const string &vuelo, const string &numeroRegistro, const string &modelo, int capacidad, const string &aerolinea, const string &ciudad_destino, const string &estado) 
        : vuelo(vuelo), numeroRegistro(numeroRegistro), modelo(modelo), capacidad(capacidad), aerolinea(aerolinea), ciudad_destino(ciudad_destino), estado(estado) {}

    // Getters
    string getVuelo() const { return vuelo; }
    string getNumeroRegistro() const { return numeroRegistro; }
    string getModelo() const { return modelo; }
    int getCapacidad() const { return capacidad; }
    string getAerolinea() const { return aerolinea; }
    string getCiudadDestino() const { return ciudad_destino; }
    string getEstado() const { return estado; }

    ~Avion() {}

private:
    string vuelo;
    string numeroRegistro;
    string modelo;
    int capacidad;
    string aerolinea;
    string ciudad_destino;
    string estado;
};
#endif // AVION_H