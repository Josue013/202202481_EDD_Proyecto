#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "avion.h"
#include "piloto.h"
#include "NodoMatriz.h"

using namespace std;

class MatrizDispersa {
private:
    NodoMD* origen;

    NodoMD* insertarFila(const string& vuelo);
    NodoMD* insertarColumna(const string& ciudad_destino);
    void insertarNodoEnFila(const string& vuelo, const string& ciudad_destino);
    NodoMD* buscarFila(const string& vuelo);
    void insertarNodo(const string& vuelo, const string& ciudad_destino, const string& numero_de_id);
    NodoMD* buscarNodo(const string& vuelo, const string& ciudad_destino);
    NodoMD* buscarNodoVacioEnFila(NodoMD* fila);

public:
    MatrizDispersa() : origen(nullptr) {}

    void insertarAvion(const Avion& avion);
    void insertarPiloto(const Piloto& piloto);
    void imprimirMatriz();
    NodoMD* buscarPorNumeroDeID(const string& numero_de_id);
    void eliminarPorNumeroDeID(const string& numero_de_id);
    void generarGraficoGraphviz();
};

NodoMD* MatrizDispersa::insertarFila(const string& vuelo) {
    if (!origen || vuelo < origen->vuelo) {
        NodoMD* nuevo = new NodoMD(vuelo, "", "");
        nuevo->abajo = origen;
        if (origen) {
            origen->arriba = nuevo;
        }
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
            if (actual->abajo) {
                actual->abajo->arriba = nuevo;
            }
            actual->abajo = nuevo;
            nuevo->arriba = actual;
            return nuevo;
        }
        return actual;
    }
}

NodoMD* MatrizDispersa::insertarColumna(const string& ciudad_destino) {
    if (!origen || ciudad_destino < origen->ciudad_destino) {
        NodoMD* nuevo = new NodoMD("", ciudad_destino, "");
        nuevo->derecha = origen;
        if (origen) {
            origen->izquierda = nuevo;
        }
        origen = nuevo;
        return nuevo;
    } else {
        NodoMD* actual = origen;
        while (actual->derecha && actual->derecha->ciudad_destino <= ciudad_destino) {
            actual = actual->derecha;
        }
        if (actual->ciudad_destino != ciudad_destino) {
            NodoMD* nuevo = new NodoMD("", ciudad_destino, "");
            nuevo->derecha = actual->derecha;
            if (actual->derecha) {
                actual->derecha->izquierda = nuevo;
            }
            actual->derecha = nuevo;
            nuevo->izquierda = actual;
            return nuevo;
        }
        return actual;
    }
}

void MatrizDispersa::insertarNodoEnFila(const string& vuelo, const string& ciudad_destino) {
    NodoMD* fila = buscarFila(vuelo);
    if (!fila) return;

    NodoMD* actual = fila;
    while (actual->derecha && actual->derecha->ciudad_destino < ciudad_destino) {
        actual = actual->derecha;
    }

    if (actual->ciudad_destino != ciudad_destino) {
        NodoMD* nuevo = new NodoMD(vuelo, ciudad_destino, "");
        nuevo->derecha = actual->derecha;
        if (actual->derecha) {
            actual->derecha->izquierda = nuevo;
        }
        actual->derecha = nuevo;
        nuevo->izquierda = actual;
    }
}

NodoMD* MatrizDispersa::buscarFila(const string& vuelo) {
    NodoMD* actual = origen;
    while (actual && actual->vuelo < vuelo) {
        actual = actual->abajo;
    }
    return (actual && actual->vuelo == vuelo) ? actual : nullptr;
}

void MatrizDispersa::insertarNodo(const string& vuelo, const string& ciudad_destino, const string& numero_de_id) {
    NodoMD* fila = insertarFila(vuelo);
    NodoMD* columna = insertarColumna(ciudad_destino);

    NodoMD* nuevo = new NodoMD(vuelo, ciudad_destino, numero_de_id);

    // Insertar en la fila
    NodoMD* actual = fila;
    while (actual->derecha && actual->derecha->ciudad_destino < ciudad_destino) {
        actual = actual->derecha;
    }
    nuevo->derecha = actual->derecha;
    if (actual->derecha) {
        actual->derecha->izquierda = nuevo;
    }
    actual->derecha = nuevo;
    nuevo->izquierda = actual;

    // Insertar en la columna
    actual = columna;
    while (actual->abajo && actual->abajo->vuelo < vuelo) {
        actual = actual->abajo;
    }
    nuevo->abajo = actual->abajo;
    if (actual->abajo) {
        actual->abajo->arriba = nuevo;
    }
    actual->abajo = nuevo;
    nuevo->arriba = actual;
}

NodoMD* MatrizDispersa::buscarNodo(const string& vuelo, const string& ciudad_destino) {
    NodoMD* fila = buscarFila(vuelo);
    if (!fila) return nullptr;

    NodoMD* actual = fila;
    while (actual && actual->ciudad_destino != ciudad_destino) {
        actual = actual->derecha;
    }
    return actual;
}

NodoMD* MatrizDispersa::buscarNodoVacioEnFila(NodoMD* fila) {
    NodoMD* actual = fila;
    while (actual) {
        if (actual->numero_de_id.empty()) {
            return actual;
        }
        actual = actual->derecha;
    }
    return nullptr;
}

void MatrizDispersa::insertarAvion(const Avion& avion) {
    insertarNodo(avion.getVuelo(), avion.getCiudadDestino(), "");
}

void MatrizDispersa::insertarPiloto(const Piloto& piloto) {
    NodoMD* fila = buscarFila(piloto.vuelo);
    if (fila) {
        NodoMD* nodoVacio = buscarNodoVacioEnFila(fila);
        if (nodoVacio) {
            nodoVacio->numero_de_id = piloto.numero_de_id;
        } else {
            cout << "No hay espacios vacíos en el vuelo " << piloto.vuelo << " para el piloto " << piloto.numero_de_id << endl;
        }
    } else {
        cout << "No se encontró el vuelo " << piloto.vuelo << " para el piloto " << piloto.numero_de_id << endl;
    }
}

NodoMD* MatrizDispersa::buscarPorNumeroDeID(const string& numero_de_id) {
    NodoMD* filaActual = origen;
    while (filaActual) {
        NodoMD* columnaActual = filaActual;
        while (columnaActual) {
            if (columnaActual->numero_de_id == numero_de_id) {
                return columnaActual;
            }
            columnaActual = columnaActual->derecha;
        }
        filaActual = filaActual->abajo;
    }
    return nullptr;
}

void MatrizDispersa::eliminarPorNumeroDeID(const string& numero_de_id) {
    NodoMD* nodo = buscarPorNumeroDeID(numero_de_id);
    if (nodo) {
        nodo->numero_de_id = "";
    }
}

void MatrizDispersa::imprimirMatriz() {
    NodoMD* filaActual = origen;
    while (filaActual) {
        NodoMD* columnaActual = filaActual;
        while (columnaActual) {
            cout << "[" << columnaActual->vuelo << ", " << columnaActual->ciudad_destino << ", " << columnaActual->numero_de_id << "] ";
            columnaActual = columnaActual->derecha;
        }
        cout << endl;
        filaActual = filaActual->abajo;
    }
}


void MatrizDispersa::generarGraficoGraphviz() {
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

    while (filaActual) {
        contadorFila++;
        filaActual = filaActual->abajo;
    }
    while (columnaActual) {
        contadorColumna++;
        columnaActual = columnaActual->derecha;
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
    columnaActual = origen;
    for (int i = 1; i <= contadorColumna && columnaActual; i++) {
        archivo << "A" << i << "[label = \"" << columnaActual->ciudad_destino << "\" , group = " << i << ", fillcolor=\"#f0fff0\"];\n";
        columnaActual = columnaActual->derecha;
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
    for (int i = 1; i <= contadorFila && filaActual; i++) {
        columnaActual = filaActual;
        for (int j = 1; j <= contadorColumna && columnaActual; j++) {
            if (!columnaActual->numero_de_id.empty()) {
                archivo << "X" << j << "Y" << i << " [ label = \"" << columnaActual->numero_de_id 
                        << "\", style=filled, fillcolor=\"#ffffcc\", group = " << j << "] ;\n";
                
                // Conexiones con filas y columnas
                archivo << "U" << i << " -> X" << j << "Y" << i << " ;\n";
                archivo << "X" << j << "Y" << i << " -> U" << i << " ;\n";
                archivo << "A" << j << " -> X" << j << "Y" << i << " ;\n";
                archivo << "X" << j << "Y" << i << " -> A" << j << " ;\n";

                // Conexiones horizontales
                if (j > 1 && columnaActual->izquierda && !columnaActual->izquierda->numero_de_id.empty()) {
                    archivo << "X" << (j-1) << "Y" << i << " -> X" << j << "Y" << i << " [dir=both] ;\n";
                }
            }
            columnaActual = columnaActual->derecha;
        }
        filaActual = filaActual->abajo;
    }

    // Rank same para filas y nodos de datos
    filaActual = origen;
    for (int i = 1; i <= contadorFila && filaActual; i++) {
        archivo << "{rank = same ; U" << i << " ;\n";
        columnaActual = filaActual;
        for (int j = 1; j <= contadorColumna && columnaActual; j++) {
            if (!columnaActual->numero_de_id.empty()) {
                archivo << "X" << j << "Y" << i << " ;";
            }
            columnaActual = columnaActual->derecha;
        }
        archivo << "}\n";
        filaActual = filaActual->abajo;
    }

    archivo << "}\n";
    archivo.close();
    
    // Generar la imagen
    system("dot -Tpng Reportes/Matriz_dispersa_de_vuelos_y_ciudades.dot -o Reportes/Matriz_dispersa_de_vuelos_y_ciudades.png");
    system("start Reportes/Matriz_dispersa_de_vuelos_y_ciudades.png");
    cout << "Gráfico generado exitosamente." << endl;
}



#endif
