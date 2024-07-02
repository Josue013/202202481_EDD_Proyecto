#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <fstream>
#include "piloto.h"
using namespace std;

class Hash {
private:
    int tableSize; 
    Piloto** table; 

    int h(string llave) {
        // Convertir el caracter en ascii
        int ascii = static_cast<int>(llave[0]);
        int sum = 0;
        for (size_t i = 1; i < llave.length(); ++i) {
            sum += llave[i] - '0'; // Convertir el caracter en número
        }
        // suma de los valores ascii de los caracteres de la llave
        sum += ascii;
        return sum % tableSize;
    }

public:
    Hash(int tam) : tableSize(tam) {
        table = new Piloto*[tableSize];
        for (int i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~Hash() {
        for (int i = 0; i < tableSize; ++i) {
            Piloto* actual = table[i];
            while (actual != nullptr) {
                Piloto* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }
        delete[] table;
    }

    void insert(Piloto* nuevo_piloto) {
        int indice = h(nuevo_piloto->numero_de_id);
        nuevo_piloto->siguiente = table[indice];
        table[indice] = nuevo_piloto;
    }

    Piloto* search(string numero_de_id) {
        int indice = h(numero_de_id);
        Piloto* actual = table[indice];
        while (actual != nullptr) {
            if (actual->numero_de_id == numero_de_id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void deletear(string numero_de_id) {
        int indice = h(numero_de_id);
        Piloto* actual = table[indice];
        Piloto* anterior = nullptr;
        
        while (actual != nullptr && actual->numero_de_id != numero_de_id) {
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
        
        delete actual;
    }

    void GenerarGraficoTablaHash() {
    std::string dotGraph = "digraph Hash {\n";
    dotGraph += "rankdir=TB;\n"; 

    // Estilos personalizados (mismos que antes)
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
        Piloto* actual = table[i];
        if (actual != nullptr) { // Solo si hay elementos en la lista
            dotGraph += std::to_string(i) + " -> " + actual->numero_de_id + ";\n"; // Enlace desde el índice al primer nodo
        }
        while (actual != nullptr) {
            std::string idPiloto = actual->numero_de_id;
            dotGraph += idPiloto + " [label=\"" + idPiloto + "\"];\n";
            if (actual->siguiente != nullptr) { // Enlace al siguiente nodo si existe
                dotGraph += idPiloto + " -> " + actual->siguiente->numero_de_id + ";\n";
            }
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