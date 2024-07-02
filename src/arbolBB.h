#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H
#include <iostream>
#include <fstream>
#include "Piloto.h"
#include "NodoBB.h"
using namespace std;


class ArbolBB {
private:
    NodoBB* raiz;
    std::ofstream archivo;


public:
    ArbolBB();
    bool estaVacio();
    void insertar(Piloto piloto);
    NodoBB* eliminarNodoPorID(NodoBB* nodo, const string& numero_de_id);
    void eliminarPiloto(const string& numero_de_id);
    NodoBB* insertarNodo(Piloto piloto, NodoBB* nodoPtr);
    NodoBB* buscarPorId(const string& id);
    NodoBB* buscarPorId(const string& id, NodoBB* nodoPtr);
    void RecorridoPreOrden();
    void RecorridoInOrden();
    void RecorridoPostOrden();
    void RecorridoPreOrden(NodoBB* nodoPtr);
    void RecorridoInOrden(NodoBB* nodoPtr);
    void RecorridoPostOrden(NodoBB* nodoPtr);
    void generarReporte();
    void imprimirNodo(NodoBB* nodoPtr);
    ~ArbolBB();
};

ArbolBB::ArbolBB() : raiz(nullptr) {}

bool ArbolBB::estaVacio() {
    return (raiz == nullptr);
}

void ArbolBB::insertar(Piloto piloto) {
    raiz = insertarNodo(piloto, raiz);
}

NodoBB* ArbolBB::insertarNodo(Piloto piloto, NodoBB* nodoPtr) {
    if (nodoPtr == nullptr) {
        NodoBB* nuevo = new NodoBB(piloto);
        nodoPtr = nuevo;
    } else if (piloto.horas_de_vuelo < nodoPtr->getPiloto().horas_de_vuelo) {
        nodoPtr->setIzq(insertarNodo(piloto, nodoPtr->getIzq()));
    } else if (piloto.horas_de_vuelo > nodoPtr->getPiloto().horas_de_vuelo) {
        nodoPtr->setDer(insertarNodo(piloto, nodoPtr->getDer()));
    } else {
        std::cout << "Nodo duplicado\n";
    }
    return nodoPtr;
}

NodoBB* ArbolBB::buscarPorId(const string& id) {
    return buscarPorId(id, raiz);
}

NodoBB* ArbolBB::buscarPorId(const string& id, NodoBB* nodoPtr) {
    if (nodoPtr == nullptr) {
        return nullptr;
    }
    if (nodoPtr->getPiloto().getNumeroDeId() == id) {
        return nodoPtr;
    }
    NodoBB* result = buscarPorId(id, nodoPtr->getIzq());
    if (result == nullptr) {
        result = buscarPorId(id, nodoPtr->getDer());
    }
    return result;
}


void ArbolBB::RecorridoPreOrden() {
    RecorridoPreOrden(raiz);
}

void ArbolBB::RecorridoInOrden() {
    RecorridoInOrden(raiz);
}

void ArbolBB::RecorridoPostOrden() {
    RecorridoPostOrden(raiz);
}


void ArbolBB::RecorridoPreOrden(NodoBB* nodoPtr) {
    if (nodoPtr != nullptr) {
        std::cout << " (" << nodoPtr->getPiloto().horas_de_vuelo << " horas)" << std::endl;
        std::cout << "      ↓       " << std::endl;
        RecorridoPreOrden(nodoPtr->getIzq());
        RecorridoPreOrden(nodoPtr->getDer());
    }
}

void ArbolBB::RecorridoInOrden(NodoBB* nodoPtr) {
    if (nodoPtr != nullptr) {
        RecorridoInOrden(nodoPtr->getIzq());
        std::cout << " (" << nodoPtr->getPiloto().horas_de_vuelo << " horas)" << std::endl;
        std::cout << "      ↓       " << std::endl;
        RecorridoInOrden(nodoPtr->getDer());
    }
}

void ArbolBB::RecorridoPostOrden(NodoBB* nodoPtr) {
    if (nodoPtr != nullptr) {
        RecorridoPostOrden(nodoPtr->getIzq());
        RecorridoPostOrden(nodoPtr->getDer());
        std::cout <<  " (" << nodoPtr->getPiloto().horas_de_vuelo << " horas)" << std::endl;
        std::cout << "      ↓       " << std::endl;
    }
}


void ArbolBB::generarReporte() {
    if (estaVacio()) {
        return;
    } else {
        archivo.open("Reportes/ArbolBB_horas_de_vuelo.dot", std::ios::out);
        archivo << "digraph G {\n";
        archivo << "node [shape=doublecircle, fontname=\"Courier New\"];\n";
        archivo << "edge [fontname=\"Courier New\"];\n";
        archivo << "Titulo [fontname=\"Courier New\", color=red shape=box3d label=\"Arbol Binario De Busqueda Horas De Vuelo\"];\n";
        
        // Color de la raíz
        archivo << raiz->getPiloto().horas_de_vuelo << " [style=filled, fillcolor=forestgreen];\n";

        imprimirNodo(raiz);
        archivo << "}\n";
        archivo.close();
        system("dot -Tpng Reportes/ArbolBB_horas_de_vuelo.dot -o Reportes/ArbolBB_horas_de_vuelo.png");
        system("start Reportes/ArbolBB_horas_de_vuelo.png");
    }
}

void ArbolBB::imprimirNodo(NodoBB* nodoPtr) {
    if (nodoPtr == nullptr) {
        return;
    }

    // Color de los nodos que no son la raíz
    if (nodoPtr != raiz) {
        archivo << nodoPtr->getPiloto().horas_de_vuelo << " [style=filled, fillcolor=lightgreen];\n";
    }

    if (nodoPtr->getIzq() != nullptr) {
        archivo << nodoPtr->getPiloto().horas_de_vuelo << " -> " 
                << nodoPtr->getIzq()->getPiloto().horas_de_vuelo << " [label=\"L\"];\n";
    }
    imprimirNodo(nodoPtr->getIzq());

    if (nodoPtr->getDer() != nullptr) {
        archivo << nodoPtr->getPiloto().horas_de_vuelo << " -> " 
                << nodoPtr->getDer()->getPiloto().horas_de_vuelo << " [label=\"R\"];\n";
    }
    imprimirNodo(nodoPtr->getDer());
}

NodoBB* ArbolBB::eliminarNodoPorID(NodoBB* nodo, const string& numero_de_id) {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (nodo->getPiloto().getNumeroDeId() == numero_de_id) {
        // Caso 1: Nodo hoja (sin hijos)
        if (nodo->getIzq() == nullptr && nodo->getDer() == nullptr) {
            delete nodo;
            return nullptr;
        }
        // Caso 2: Nodo con un hijo
        else if (nodo->getIzq() == nullptr) {
            NodoBB* temp = nodo->getDer();
            delete nodo;
            return temp;
        }
        else if (nodo->getDer() == nullptr) {
            NodoBB* temp = nodo->getIzq();
            delete nodo;
            return temp;
        }
        // Caso 3: Nodo con dos hijos
        else {
            NodoBB* sucesor = nodo->getDer();
            NodoBB* padreSucesor = nodo;
            while (sucesor->getIzq() != nullptr) {
                padreSucesor = sucesor;
                sucesor = sucesor->getIzq();
            }
            nodo->setPiloto(sucesor->getPiloto());
            if (padreSucesor == nodo) {
                nodo->setDer(sucesor->getDer());
            } else {
                padreSucesor->setIzq(sucesor->getDer());
            }
            delete sucesor;
            return nodo;
        }
    } else {
        nodo->setIzq(eliminarNodoPorID(nodo->getIzq(), numero_de_id));
        nodo->setDer(eliminarNodoPorID(nodo->getDer(), numero_de_id));
    }

    return nodo;
}

void ArbolBB::eliminarPiloto(const string& numero_de_id) {
    raiz = eliminarNodoPorID(raiz, numero_de_id);
}

ArbolBB::~ArbolBB() {}
#endif 
