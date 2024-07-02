#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "avion.h"
#include "piloto.h"

using namespace std;

class NodoMD {
public:
    string vuelo;
    string ciudad_destino;
    string numero_de_id;
    NodoMD* derecha;
    NodoMD* abajo;

    NodoMD(string vuelo, string ciudad_destino, string numero_de_id)
        : vuelo(vuelo), ciudad_destino(ciudad_destino), numero_de_id(numero_de_id), derecha(nullptr), abajo(nullptr) {}
};

class MatrizDispersa {
private:
    NodoMD* origen;

public:
    MatrizDispersa() : origen(nullptr) {}

    void insertarAvion(const Avion& avion) {
        NodoMD* fila = insertarFila(avion.getVuelo());
        NodoMD* nodo = new NodoMD(avion.getVuelo(), avion.getCiudadDestino(), "");
        
        // Insertar el nodo en la fila
        if (!fila->derecha) {
            fila->derecha = nodo;
        } else {
            NodoMD* actual = fila;
            while (actual->derecha) {
                actual = actual->derecha;
            }
            actual->derecha = nodo;
        }
    }

    void insertarPiloto(const Piloto& piloto) {
        NodoMD* fila = buscarFila(piloto.vuelo);
        if (fila) {
            NodoMD* actual = fila->derecha;
            while (actual) {
                if (actual->numero_de_id.empty()) {
                    actual->numero_de_id = piloto.numero_de_id;
                    return;
                }
                actual = actual->derecha;
            }
        }
    }

    void imprimirMatriz() {
        NodoMD* filaActual = origen;
        while (filaActual) {
            NodoMD* columnaActual = filaActual->derecha;
            while (columnaActual) {
                if (!columnaActual->numero_de_id.empty()) {
                    cout << "[" << filaActual->vuelo << ", " << columnaActual->ciudad_destino << ", " << columnaActual->numero_de_id << "] ";
                }
                columnaActual = columnaActual->derecha;
            }
            cout << endl;
            filaActual = filaActual->abajo;
        }
    }

    NodoMD* buscarPorNumeroDeID(const string& numero_de_id) {
        NodoMD* filaActual = origen;
        while (filaActual) {
            NodoMD* columnaActual = filaActual->derecha;
            while (columnaActual) {
                if (columnaActual->numero_de_id == numero_de_id) {
                    return columnaActual;
                }
                columnaActual = columnaActual->derecha;
            }
            filaActual = filaActual->abajo;
        }
        return nullptr; // Si no se encuentra el ID, retorna nullptr
    }

    void eliminarPorNumeroDeID(const string& numero_de_id) {
        NodoMD* filaActual = origen;
        NodoMD* nodoAEliminar = nullptr;

        while (filaActual) {
            NodoMD* columnaActual = filaActual->derecha;
            NodoMD* anterior = nullptr;
            while (columnaActual) {
                if (columnaActual->numero_de_id == numero_de_id) {
                    nodoAEliminar = columnaActual;
                    if (anterior) {
                        anterior->derecha = columnaActual->derecha;
                    } else {
                        filaActual->derecha = columnaActual->derecha;
                    }
                    break;
                }
                anterior = columnaActual;
                columnaActual = columnaActual->derecha;
            }

            if (nodoAEliminar) {
                delete nodoAEliminar;
                break;
            }
            filaActual = filaActual->abajo;
        }
    }

    void generarGraficoGraphviz() {
    ofstream archivo("Reportes/Matriz_dispersa_de_vuelos_y_ciudades.dot");
    archivo << "digraph G {\n";
    archivo << "node [shape=box, style=filled, fontname=\"Arial\", fontsize=12];\n"; // Estilo general de nodos
    archivo << "edge [color=\"#006400\", penwidth=1.5];\n"; // Estilo de conexiones (verde oscuro)

    archivo << "matriz [label=\"\", group=filas];\n"; // Nodo matriz invisible

    // Contar filas y columnas
    int contadorFila = 0;
    int contadorColumna = 0;
    NodoMD* filaActual = origen;
    NodoMD* columnaActual = origen;

    // Contar filas (vuelos)
    while (filaActual) {
        contadorFila++;
        filaActual = filaActual->abajo;
    }

    // Contar columnas (ciudades destino)
    filaActual = origen;
    while (filaActual) {
        columnaActual = filaActual;
        while (columnaActual) {
            contadorColumna++;
            columnaActual = columnaActual->derecha;
        }
        filaActual = filaActual->abajo;
    }

    // Nodos de filas (vuelos)
    filaActual = origen;
    for (int i = 1; i <= contadorFila && filaActual; i++) {
        archivo << "U" << i << "[label = \"" << filaActual->vuelo << "\" , group = filas, fillcolor=\"#f0fff0\"];\n";
        filaActual = filaActual->abajo;
    }

    // Conexiones entre filas
    archivo << "matriz -> U1 [dir=both] ;\n";
    for (int i = 1; i < contadorFila; i++) {
        archivo << "U" << i << " -> U" << (i + 1) << " [dir=both];\n";
    }

    // Nodos de columnas (ciudades destino)
    int colIndex = 0;
    filaActual = origen;
    while (filaActual) {
        columnaActual = filaActual;
        while (columnaActual) {
            archivo << "A" << colIndex << "[label = \"" << columnaActual->ciudad_destino << "\" , group = " << colIndex << ", fillcolor=\"#f0fff0\"];\n";
            columnaActual = columnaActual->derecha;
            colIndex++;
        }
        filaActual = filaActual->abajo;
    }

    // Conexiones entre columnas
    archivo << "matriz -> A1 [dir=both] ;\n";
    for (int i = 1; i < contadorColumna; i++) {
        archivo << "A" << i << " -> A" << (i + 1) << " [dir=both] ;\n";
    }

    // Rank same para matriz y columnas
    archivo << "{ rank = same; matriz;\n";
    for (int i = 1; i <= contadorColumna; i++) {
        archivo << "A" << i << ";\n";
    }
    archivo << "}\n";

    // Nodos de datos (número de ID) y conexiones
    filaActual = origen;
    int filaIndex = 1;
    while (filaActual) {
        columnaActual = filaActual;
        int colIndex = 1;
        while (columnaActual) {
            if (!columnaActual->numero_de_id.empty()) {
                archivo << "X" << colIndex << "Y" << filaIndex << " [ label = \"" << columnaActual->numero_de_id 
                        << "\", style=filled, fillcolor=\"#ffffcc\", group = " << colIndex << "] ;\n";
                
                // Conexiones con filas y columnas
                archivo << "U" << filaIndex << " -> X" << colIndex << "Y" << filaIndex << " ;\n";
                archivo << "X" << colIndex << "Y" << filaIndex << " -> U" << filaIndex << " ;\n";
                archivo << "A" << colIndex << " -> X" << colIndex << "Y" << filaIndex << " ;\n";
                archivo << "X" << colIndex << "Y" << filaIndex << " -> A" << colIndex << " ;\n";
            }
            columnaActual = columnaActual->derecha;
            colIndex++;
        }
        filaActual = filaActual->abajo;
        filaIndex++;
    }

    // Rank same para filas y nodos de datos
    filaActual = origen;
    filaIndex = 1;
    while (filaActual) {
        archivo << "{rank = same ; U" << filaIndex << " ;\n";
        columnaActual = filaActual;
        int colIndex = 1;
        while (columnaActual) {
            if (!columnaActual->numero_de_id.empty()) {
                archivo << "X" << colIndex << "Y" << filaIndex << " ;";
            }
            columnaActual = columnaActual->derecha;
            colIndex++;
        }
        archivo << "}\n";
        filaActual = filaActual->abajo;
        filaIndex++;
    }

    archivo << "}\n";
    archivo.close();
    
    // Generar la imagen
    system("dot -Tpng Reportes/Matriz_dispersa_de_vuelos_y_ciudades.dot -o Reportes/Matriz_dispersa_de_vuelos_y_ciudades.png");
    system("start Reportes/Matriz_dispersa_de_vuelos_y_ciudades.png");
    cout << "Gráfico generado exitosamente." << endl;
}

private:
    NodoMD* insertarFila(const string& vuelo) {
        if (!origen || vuelo < origen->vuelo) {
            NodoMD* nuevo = new NodoMD(vuelo, "", "");
            nuevo->abajo = origen;
            origen = nuevo;
            return nuevo;
        } else {
            NodoMD* actual = origen;
            while (actual->abajo && actual->abajo->vuelo <= vuelo) {
                actual = actual->abajo;
            }
            if (actual->vuelo != vuelo) {
                NodoMD* nuevo = new NodoMD(vuelo, "", "");
                nuevo->abajo = actual->abajo;
                actual->abajo = nuevo;
                return nuevo;
            }
            return actual;
        }
    }

    NodoMD* buscarFila(const string& vuelo) {
        NodoMD* actual = origen;
        while (actual && actual->vuelo != vuelo) {
            actual = actual->abajo;
        }
        return actual;
    }
};

#endif // MATRIZ_DISPERSA_H
