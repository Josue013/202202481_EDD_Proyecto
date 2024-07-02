#ifndef CIRCULAR_DOBLE_H
#define CIRCULAR_DOBLE_H
#include <iostream>
#include <string>
using namespace std;

template <typename type>
struct Nodo {
    type data;
    Nodo<type>* next;
    Nodo<type>* prev;

    Nodo(const type& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename type>
class DoubleCircularList {
private:
    Nodo<type>* first;
    Nodo<type>* last;
    int size;

public:
    DoubleCircularList() : first(nullptr), last(nullptr), size(0) {}

    ~DoubleCircularList() {
        clear();
    }

    void insert(const type& value) {
        Nodo<type>* nuevoNodo = new Nodo<type>(value);
        if (isEmpty()) {
            first = nuevoNodo;
            last = nuevoNodo;
            nuevoNodo->next = nuevoNodo;
            nuevoNodo->prev = nuevoNodo;
        } else {
            nuevoNodo->prev = last;
            nuevoNodo->next = first;
            last->next = nuevoNodo;
            first->prev = nuevoNodo;
            last = nuevoNodo;
        }
        size++;
    }

    void remove(const type& value) {
        if (isEmpty()) {
            return;
        }
        Nodo<type>* actual = first;
        do {
            if (actual->data == value) {
                if (actual == first && actual == last) {
                    first = nullptr;
                    last = nullptr;
                } else if (actual == first) {
                    first = actual->next;
                    last->next = first;
                    first->prev = last;
                } else if (actual == last) {
                    last = actual->prev;
                    last->next = first;
                    first->prev = last;
                } else {
                    actual->prev->next = actual->next;
                    actual->next->prev = actual->prev;
                }
                delete actual;
                size--;
                return;
            }
            actual = actual->next;
        } while (actual != first);
    }

    int getSize() const {
        return size;
    }

    type getElement(int indice) const {
        if (indice < 0 || indice >= size) {
            throw std::out_of_range("Fuera de rango");
        }
        Nodo<type>* actual = first;
        for (int i = 0; i < indice; i++) {
            actual = actual->next;
        }

        return actual->data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void clear() {
        if (!isEmpty()) {
            Nodo<type>* actual = first;
            do {
                Nodo<type>* temp = actual;
                actual = actual->next;
                delete temp;
            } while (actual != first);
            first = nullptr;
            last = nullptr;
            size = 0;
        }
    }

    void print() const {
        if (isEmpty()) {
            std::cout << "Esta vacia la lista" << std::endl;
            return;
        }
        Nodo<type>* actual = first;
        do {
            std::cout << actual->data << " ";
            actual = actual->next;
        } while (actual != first);
        std::cout << std::endl;
    }



    type* buscarPorNumeroDeRegistro(const string& numeroDeRegistro) const {
        if (isEmpty()) {
            return nullptr;
        }
        Nodo<type>* actual = first;
        do {
            if (actual->data.getNumeroRegistro() == numeroDeRegistro) {
                return &(actual->data);
            }
            actual = actual->next;
        } while (actual != first);
        return nullptr;
    }

    void eliminarPorNumeroDeRegistro(const string& numeroDeRegistro) {
        if (isEmpty()) {
            return;
        }
        Nodo<type>* actual = first;
        do {
            if (actual->data.getNumeroRegistro() == numeroDeRegistro) {
                if (actual == first && actual == last) {
                    first = nullptr;
                    last = nullptr;
                } else if (actual == first) {
                    first = actual->next;
                    last->next = first;
                    first->prev = last;
                } else if (actual == last) {
                    last = actual->prev;
                    last->next = first;
                    first->prev = last;
                } else {
                    actual->prev->next = actual->next;
                    actual->next->prev = actual->prev;
                }
                delete actual;
                size--;
                return;
            }
            actual = actual->next;
        } while (actual != first);
    }
};

#endif 