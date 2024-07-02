#ifndef NODOBB_H
#define NODOBB_H

#include "Piloto.h"

class NodoBB {
private:
    Piloto piloto;
    NodoBB* izq; // Hijo izquierdo
    NodoBB* der; // Hijo derecho

public:
    NodoBB(Piloto piloto);
    void setPiloto(Piloto piloto);
    Piloto getPiloto();
    void setIzq(NodoBB* izq);
    NodoBB* getIzq();
    void setDer(NodoBB* der);
    NodoBB* getDer();
    ~NodoBB();
};

NodoBB::NodoBB(Piloto piloto) : piloto(piloto), izq(nullptr), der(nullptr) {}

void NodoBB::setPiloto(Piloto piloto) {
    this->piloto = piloto;
}

Piloto NodoBB::getPiloto() {
    return this->piloto;
}

void NodoBB::setIzq(NodoBB* izq) {
    this->izq = izq;
}

void NodoBB::setDer(NodoBB* der) {
    this->der = der;
}

NodoBB* NodoBB::getIzq() {
    return this->izq;
}

NodoBB* NodoBB::getDer() {
    return this->der;
}

NodoBB::~NodoBB() {}

#endif 
