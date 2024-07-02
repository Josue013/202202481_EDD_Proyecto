#ifndef PILOTO_H
#define PILOTO_H

#include <string>

class Piloto {
public:
    std::string nombre;
    std::string nacionalidad;
    std::string numero_de_id;
    std::string vuelo;
    int horas_de_vuelo;
    std::string tipo_de_licencia;

    Piloto* siguiente;

    Piloto(std::string nombre, std::string nacionalidad, std::string numero_de_id, std::string vuelo, int horas_de_vuelo, std::string tipo_de_licencia)
        : nombre(nombre), nacionalidad(nacionalidad), numero_de_id(numero_de_id), vuelo(vuelo), horas_de_vuelo(horas_de_vuelo), tipo_de_licencia(tipo_de_licencia) {}

    // Getters
    std::string getNombre() const { return nombre; }
    std::string getNacionalidad() const { return nacionalidad; }
    std::string getNumeroDeId() const { return numero_de_id; }
    std::string getVuelo() const { return vuelo; }
    int getHorasDeVuelo() const { return horas_de_vuelo; }
    std::string getTipoDeLicencia() const { return tipo_de_licencia; }
    
};

#endif 