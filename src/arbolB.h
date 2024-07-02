#ifndef ARBOLB_H
#define ARBOLB_H
#include "avion.h"
#include <iostream>
#include <string>
using namespace std;

const int Degree = 5;
int getNum(string numero_de_registro)
{
    return stoi(numero_de_registro.substr(1));
}

class NodoB
{
public:
    bool hoja;
    int n; 
    Avion *aviones[Degree - 1];
    NodoB *hijos[Degree];

    NodoB(bool _hoja)
    {
        hoja = _hoja;
        n = 0;
        for (int i = 0; i < Degree; i++)
            hijos[i] = nullptr;
    }

    void insertNoFull(Avion *avion)
    {
        int num_registro = getNum(avion->getNumeroRegistro());
        int i = n - 1;

        if (hoja)
        {
            while (i >= 0 && getNum(aviones[i]->getNumeroRegistro()) > num_registro)
            {
                aviones[i + 1] = aviones[i];
                i--;
            }

            aviones[i + 1] = avion;
            n = n + 1;
        }
        else
        {
            while (i >= 0 && getNum(aviones[i]->getNumeroRegistro()) > num_registro)
                i--;

            if (hijos[i + 1]->n == Degree - 1)
            {
                splitChild(i + 1, hijos[i + 1]);

                if (getNum(aviones[i + 1]->getNumeroRegistro()) < num_registro)
                    i++;
            }
            hijos[i + 1]->insertNoFull(avion);
        }
    }

    void splitChild(int i, NodoB *y)
    {
        NodoB *z = new NodoB(y->hoja);
        z->n = (Degree / 2) - 1;

        for (int j = 0; j < (Degree / 2) - 1; j++)
            z->aviones[j] = y->aviones[j + (Degree / 2)];

        if (!y->hoja)
        {
            for (int j = 0; j < Degree / 2; j++)
                z->hijos[j] = y->hijos[j + (Degree / 2)];
        }

        y->n = (Degree / 2) - 1;

        for (int j = n; j >= i + 1; j--)
            hijos[j + 1] = hijos[j];

        hijos[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            aviones[j + 1] = aviones[j];

        aviones[i] = y->aviones[(Degree / 2) - 1];

        n = n + 1;
    }

    void recorrer()
    {
        int i;
        for (i = 0; i < n; i++)
        {
            if (!hoja)
                hijos[i]->recorrer();
            cout << " " << aviones[i]->getNumeroRegistro();
        }

        if (!hoja)
            hijos[i]->recorrer();
    }

    NodoB *search(string numero_de_registro)
    {
        int num_registro = getNum(numero_de_registro);
        int i = 0;
        while (i < n && getNum(aviones[i]->getNumeroRegistro()) < num_registro)
            i++;

        if (i < n && getNum(aviones[i]->getNumeroRegistro()) == num_registro)
            return this;

        if (hoja)
            return nullptr;

        return hijos[i]->search(numero_de_registro);
    }

    void generarDot(ofstream &archivo, int &nodoId)
    {
        int miId = nodoId++;
        archivo << "    nodo" << miId << " [label=\"";

        // Separadores entre claves (opcional)
        for (int i = 0; i < n; i++)
        {
            if (i > 0)
                archivo << " - ";
            archivo << aviones[i]->getNumeroRegistro();
        }

        archivo << "\", ";

        // Estilos para los nodos
        archivo << "shape=box, style=filled, fillcolor=\"#e0f2e0\", "; // Fondo verde claro
        archivo << "fontname=\"Arial\", fontsize=12, ";                // Fuente Arial, tamaño 12
        archivo << "penwidth=2.0";                                     // Grosor de línea del borde

        archivo << "];\n";

        for (int i = 0; i <= n; i++)
        {
            if (hijos[i] != nullptr)
            {
                int hijoId = nodoId;
                hijos[i]->generarDot(archivo, nodoId);
                // Modificación para centrar el inicio de las flechas
                archivo << "    nodo" << miId << " -> nodo" << hijoId << ":n [color=\"#006400\"];\n"; // Conexiones desde el centro de miId
            }
        }
    }

    void deleteRecursive(string numero_de_registro)
    {
        int i = 0;
        while (i < n && getNum(aviones[i]->getNumeroRegistro()) < getNum(numero_de_registro))
            i++;
        if (i < n && aviones[i]->getNumeroRegistro() == numero_de_registro)
        { // Caso 1: El avión está en este nodo
            if (hoja)
            {
                // Eliminar el avión directamente si es una hoja
            }
            else
            {
                // Encontrar y eliminar el predecesor/sucesor si no es una hoja
            }
        }
        else
        {
            if (hoja)
            {
                // El avión no está en el árbol
                return;
            }
            // Descender al hijo adecuado, posiblemente después de ajustar el árbol
            bool flag = ((i == n) ? true : false);
            if (hijos[i]->n < (Degree / 2))
            {
                // Ajustar el árbol antes de descender
            }
            if (flag && i > n)
                hijos[i - 1]->deleteRecursive(numero_de_registro);
            else
                hijos[i]->deleteRecursive(numero_de_registro);
        }
        // Ajustar este nodo después de la eliminación si es necesario
    }

    Avion *searchRecursive(string numeroRegistro)
    {
        int num_registro = getNum(numeroRegistro);
        int i = 0;
        while (i < n && getNum(aviones[i]->getNumeroRegistro()) < num_registro)
            i++;

        if (i < n && getNum(aviones[i]->getNumeroRegistro()) == num_registro)
            return aviones[i];

        if (hoja)
            return nullptr;

        return hijos[i]->searchRecursive(numeroRegistro);
    }
};

class ArbolB
{
private:
    NodoB *raiz;

public:
    ArbolB() { raiz = nullptr; }

    void recorrer()
    {
        if (raiz != nullptr)
            raiz->recorrer();
    }

    NodoB *search(string numero_de_registro)
    {
        return (raiz == nullptr) ? nullptr : raiz->search(numero_de_registro);
    }

    void insert(Avion *avion)
    {
        if (raiz == nullptr)
        {
            raiz = new NodoB(true);
            raiz->aviones[0] = avion;
            raiz->n = 1;
        }
        else
        {
            if (raiz->n == Degree - 1)
            {
                NodoB *s = new NodoB(false);
                s->hijos[0] = raiz;

                s->splitChild(0, raiz);

                int i = 0;
                if (getNum(s->aviones[0]->getNumeroRegistro()) < getNum(avion->getNumeroRegistro()))
                    i++;
                s->hijos[i]->insertNoFull(avion);

                raiz = s;
            }
            else
            {
                raiz->insertNoFull(avion);
            }
        }
    }

    void generarDot()
    {
        ofstream archivo("Reportes/Arbolb_aviones_disponibles.dot");
        archivo << "digraph G {\n";
        archivo << "    node [shape=record];\n";
        archivo << "Titulo [fontname=\"Courier New\", color=red shape=box3d label=\"Árbol B con aviones disponibles\"];\n";

        int nodoId = 0;
        if (raiz != nullptr)
        {
            raiz->generarDot(archivo, nodoId);
        }

        archivo << "}\n";
        archivo.close();

        system("dot -Tpng Reportes/Arbolb_aviones_disponibles.dot -o Reportes/Arbolb_aviones_disponibles.png");
        system("start Reportes/Arbolb_aviones_disponibles.png");
    }

    void Deletear(string numero_de_registro)
    {
        if (raiz != nullptr)
        {
            raiz->deleteRecursive(numero_de_registro);
            if (raiz->n == 0)
            { // Si la raíz está vacía
                NodoB *tmp = raiz;
                if (raiz->hoja)
                    raiz = nullptr;
                else
                    raiz = raiz->hijos[0];
                delete tmp;
            }
        }
    }

    Avion *buscarAvion(string numeroRegistro)
    {
        if (raiz == nullptr)
            return nullptr;
        else
            return raiz->searchRecursive(numeroRegistro);
    }
};

#endif 
