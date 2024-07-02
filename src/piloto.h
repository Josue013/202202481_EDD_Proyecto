#ifndef PILOTO_H
#define PILOTO_H
#include <string>
using namespace std;

class Piloto {
public:
    string nombre;
    string nacionalidad;
    string numero_de_id;
    string vuelo;
    int horas_de_vuelo;
    string tipo_de_licencia;

    Piloto* siguiente;

    Piloto(string nombre, string nacionalidad, string numero_de_id, string vuelo, int horas_de_vuelo, string tipo_de_licencia)
        : nombre(nombre), nacionalidad(nacionalidad), numero_de_id(numero_de_id), vuelo(vuelo), horas_de_vuelo(horas_de_vuelo), tipo_de_licencia(tipo_de_licencia) {}

    // Getters
    string getNombre() const { return nombre; }
    string getNacionalidad() const { return nacionalidad; }
    string getNumeroDeId() const { return numero_de_id; }
    string getVuelo() const { return vuelo; }
    int getHorasDeVuelo() const { return horas_de_vuelo; }
    string getTipoDeLicencia() const { return tipo_de_licencia; }
    
};

#endif // PILOTO_H