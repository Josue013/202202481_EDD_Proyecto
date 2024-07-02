#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <fstream>
#include "piloto.h"
using namespace std;


class Hash {
private:
    int tableSize;
    struct Nodo {
        Piloto* piloto;
        Nodo* siguiente;

        Nodo(Piloto* p) : piloto(p), siguiente(nullptr) {}
    };
    Nodo** table; // Arreglo de punteros a Nodo

    int h(string llave) {
        // Función de hash simple, suma de valores ASCII
        int ascii = static_cast<int>(llave[0]);
        int sum = 0;
        for (size_t i = 1; i < llave.length(); ++i) {
            sum += llave[i] - '0'; // Convertir el caracter en número
        }
        sum += ascii;
        return sum % tableSize;
    }

public:
    Hash(int tam) : tableSize(tam) {
        table = new Nodo*[tableSize];
        for (int i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~Hash() {
        for (int i = 0; i < tableSize; ++i) {
            Nodo* actual = table[i];
            while (actual != nullptr) {
                Nodo* temp = actual;
                actual = actual->siguiente;
                delete temp->piloto;
                delete temp; // Liberar memoria de Nodo y Piloto
            }
        }
        delete[] table; // Liberar el arreglo de punteros a Nodo
    }

    void insert(Piloto* nuevo_piloto) {
        int indice = h(nuevo_piloto->numero_de_id);
        Nodo* nuevo_nodo = new Nodo(nuevo_piloto);
        nuevo_nodo->siguiente = table[indice];
        table[indice] = nuevo_nodo;
    }

    Piloto* search(string numero_de_id) {
        int indice = h(numero_de_id);
        Nodo* actual = table[indice];
        while (actual != nullptr) {
            if (actual->piloto->numero_de_id == numero_de_id) {
                return actual->piloto;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void deletear(string numero_de_id) {
        int indice = h(numero_de_id);
        Nodo* actual = table[indice];
        Nodo* anterior = nullptr;

        while (actual != nullptr && actual->piloto->numero_de_id != numero_de_id) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            return; // El piloto no se encontró
        }

        if (anterior == nullptr) {
            table[indice] = actual->siguiente; // El piloto a eliminar es el primero de la lista
        } else {
            anterior->siguiente = actual->siguiente;
        }

        delete actual->piloto;
        delete actual; // Liberar memoria de Nodo y Piloto
    }

    void GenerarGraficoTablaHash() {
    std::string dotGraph = "digraph Hash {\n";
    dotGraph += "rankdir=TB;\n"; 

    // Estilos personalizados
    dotGraph += "node [shape=box, style=filled, fillcolor=\"#f0f0f5\", fontname=\"Arial\", penwidth=2, color=\"#4CAF50\"];\n";
    dotGraph += "edge [color=\"#2196F3\", penwidth=1.5, arrowhead=vee];\n";
    dotGraph += "Titulo [fontname=\"Courier New\", color=red shape=box3d label=\"Tabla hash de pilotos\"];\n";

    // Subgrafo para los índices (cuadros amarillos)
    dotGraph += "subgraph indices {\n";
    dotGraph += "rank=same; "; // Forzar a los índices a estar en el mismo nivel (arriba)
    for (int i = 0; i < tableSize; ++i) {
        dotGraph += std::to_string(i) + " [label=\"" + std::to_string(i) + "\", style=filled, fillcolor=\"#FFC107\"];\n";
    }
    dotGraph += "}\n"; // Cerrar el subgrafo de índices

    // Nodos con IDs y enlaces
    for (int i = 0; i < tableSize; ++i) {
        Nodo* actual = table[i];
        if (actual != nullptr) {
            dotGraph += std::to_string(i) + " -> " + actual->piloto->numero_de_id + ";\n"; // Enlace desde el índice al primer nodo
        }
        while (actual != nullptr && actual->siguiente != nullptr) {
            dotGraph += actual->piloto->numero_de_id + " -> " + actual->siguiente->piloto->numero_de_id + ";\n";
            actual = actual->siguiente;
        }
    }

    dotGraph += "}\n";

    std::ofstream outFile("Reportes/tabla_hash_visualizacion.dot");
    if (outFile.is_open()) {
        outFile << dotGraph;
        outFile.close();
    }

    system("dot -Tpng -o Reportes/tabla_hash_visualizacion.png Reportes/tabla_hash_visualizacion.dot");
    system("start Reportes/tabla_hash_visualizacion.png");
}
};

#endif