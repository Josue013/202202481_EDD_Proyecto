#ifndef AdjacencyList_H
#define AdjacencyList_H
#include <iostream>
#include "fstream"
#include <sstream> 
#include <vector>
#include <string>
using namespace std;

const int infinityNum = 1e9; 

struct Edge {
    string destino;
    int distancia;
    Edge(string dest, int dist) : destino(dest), distancia(dist) {}
};
struct NODOAL {
    string origen;
    vector<Edge> edges;
    NODOAL(string orig) : origen(orig) {}
};

class AdjacencyList {
private:
    vector<NODOAL> nodos;

    NODOAL* findNODOAL(const string& origen) {
        for (auto& nodo : nodos) {
            if (nodo.origen == origen) {
                return &nodo;
            }
        }
        return nullptr;
    }

    int findIndexNODOAL(const string& nombre) const {
        for (int i = 0; i < nodos.size(); ++i) {
            if (nodos[i].origen == nombre) {
                return i;
            }
        }
        return -1; 
    }

    pair<vector<string>, int> findShortestPath(int indiceActual, int indiceDestino, int distanciaAcumulada, vector<int>& distanciasMinimas, vector<string>& rutaActual) const {
        if (indiceActual == indiceDestino) {
            rutaActual.push_back(nodos[indiceActual].origen);
            return {rutaActual, distanciaAcumulada};
        }
        if (distanciaAcumulada >= distanciasMinimas[indiceActual]) {
            return {{}, infinityNum};
        }
        distanciasMinimas[indiceActual] = distanciaAcumulada;
        pair<vector<string>, int> mejorRuta = {{}, infinityNum};
        for (const auto& edge : nodos[indiceActual].edges) {
            int indiceDestinoArista = findIndexNODOAL(edge.destino);
            if (indiceDestinoArista != -1) {
                vector<string> rutaSiguiente = rutaActual;
                rutaSiguiente.push_back(nodos[indiceActual].origen);
                auto resultado = findShortestPath(indiceDestinoArista, indiceDestino, distanciaAcumulada + edge.distancia, distanciasMinimas, rutaSiguiente);
                if (resultado.second < mejorRuta.second) {
                    mejorRuta = resultado;
                }
            }
        }
        return mejorRuta;
    }

public:
    void addEdge(const string& origen, const string& destino, int distancia) {
        NODOAL* nodoOrigen = findNODOAL(origen);
        if (nodoOrigen == nullptr) {
            nodos.push_back(NODOAL(origen));
            nodoOrigen = &nodos.back();
        }
        nodoOrigen->edges.emplace_back(destino, distancia);
    }

    void displayList() const {
        for (const auto& nodo : nodos) {
            cout << "Origen: " << nodo.origen << " -> ";
            for (const auto& edge : nodo.edges) {
                cout << "(Destino: " << edge.destino << ", Distancia: " << edge.distancia << ") ";
            }
            cout << endl;
        }
    }

    pair<vector<string>, int> shortestPath(const string& origen, const string& destino) const {
        int indiceOrigen = findIndexNODOAL(origen);
        int indiceDestino = findIndexNODOAL(destino);
        if (indiceOrigen == -1 || indiceDestino == -1) return {{}, -1};
        vector<int> distanciasMinimas(nodos.size(), infinityNum);
        vector<string> ruta;
        auto resultado = findShortestPath(indiceOrigen, indiceDestino, 0, distanciasMinimas, ruta);
        if (resultado.second == infinityNum) return {{}, -1};
        return resultado;
    }

    void generarGrafoGraphviz() const {
    ofstream archivo("Reportes/Grafo_dirigido_con_las_rutas.dot");
    archivo << "digraph G {" << endl;
    archivo << "  node [shape=note, style=filled, color=lightgrey, fontname=Arial, penwidth=2];" << endl; // penwidth=2 para el borde del nodo
    archivo << "  edge [fontname=Arial, fontsize=10, color=black, arrowhead=vee];" << endl; // fontsize=10 para hacer la fuente más pequeña y arrowhead=vee para cambiar el estilo de las flechas
    archivo << "Titulo [fontname=\"Courier New\", color=red shape=box3d label=\"Grafo dirigido de rutas\"];\n";
    // Recorrer nodos y edges
    for (const auto& nodo : nodos) {
        for (const auto& edge : nodo.edges) {
            archivo << "    \"" << nodo.origen << "\" -> \"" << edge.destino << "\" [label=\"" << edge.distancia << "\", fontcolor=blue];" << endl;
        }
    }
    archivo << "}" << endl;
    archivo.close();
    system("dot -Tpng Reportes/Grafo_dirigido_con_las_rutas.dot -o Reportes/Grafo_dirigido_con_las_rutas.png");
    system("start Reportes/Grafo_dirigido_con_las_rutas.png");
}

};

#endif 