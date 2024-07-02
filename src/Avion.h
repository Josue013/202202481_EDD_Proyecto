#ifndef AVION_H
#define AVION_H
#include <string>

class Avion {
public:
    // Constructor
    Avion(const std::string &vuelo, const std::string &numeroRegistro, const std::string &modelo, int capacidad, const std::string &aerolinea, const std::string &ciudad_destino, const std::string &estado) 
        : vuelo(vuelo), numeroRegistro(numeroRegistro), modelo(modelo), capacidad(capacidad), aerolinea(aerolinea), ciudad_destino(ciudad_destino), estado(estado) {}

    // Getters
    std::string getVuelo() const { return vuelo; }
    std::string getNumeroRegistro() const { return numeroRegistro; }
    std::string getModelo() const { return modelo; }
    int getCapacidad() const { return capacidad; }
    std::string getAerolinea() const { return aerolinea; }
    std::string getCiudadDestino() const { return ciudad_destino; }
    std::string getEstado() const { return estado; }

    ~Avion() {}

private:
    std::string vuelo;
    std::string numeroRegistro;
    std::string modelo;
    int capacidad;
    std::string aerolinea;
    std::string ciudad_destino;
    std::string estado;
};

#endif 