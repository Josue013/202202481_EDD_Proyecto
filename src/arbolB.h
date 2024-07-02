#ifndef B_TREE_H_CUSTOM
#define B_TREE_H_CUSTOM
#include <iostream>
#include <cstring>
#include <string>
#include "avion.h"
#include <fstream>
#include <cstdlib>
using namespace std;


const int Degree = 3; 
const int MaxDegree = 2 * Degree - 1; // Maximum degree of B-tree = 5
const int MaxChildren = 2 * Degree;

struct BTreeNode {
    Avion* keys[MaxDegree];
    BTreeNode* children[MaxChildren];
    int n; 
    bool leaf;

    BTreeNode(bool _leaf = true) : n(0), leaf(_leaf) {
        memset(keys, 0, sizeof(keys));
        memset(children, 0, sizeof(children));
    }
};

class BTree {
private:
    BTreeNode* root;

    void splitChild(BTreeNode* parent, int i, BTreeNode* child);
    void insertNonFull(BTreeNode* node, Avion* key);
    Avion* search(BTreeNode* node, const string& registration_number);
    void remove(BTreeNode* node, const string& registration_number);
    void removeFromLeaf(BTreeNode* node, int id);
    void removeFromNonLeaf(BTreeNode* node, int id);
    void mergeNodes(BTreeNode* node, int id);
    void borrowFromPrev(BTreeNode* node, int id);
    void borrowFromNext(BTreeNode* node, int id);
    void fillNode(BTreeNode* node, int id);
    Avion* getPredecessor(BTreeNode* node, int id);
    Avion* getSuccessor(BTreeNode* node, int id);
    void generateRecursiveDOT(BTreeNode* node, string& content, int& nodeId);

public:
    BTreeNode* getRoot() const {
        return root;
    }

    BTree() : root(nullptr) {}

    void insert(Avion* key);
    Avion* search(const string& registration_number);
    void remove(const string& registration_number);
    void printPlanes();
    void generateAvailablePlanesReport();
};

void BTree::insert(Avion* key) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys[0] = key;
        root->n = 1;
    } else {
        if (root->n == MaxDegree) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;
            splitChild(s, 0, root);
            int i = 0;
            if (s->keys[0]->getNumeroRegistro() < key->getNumeroRegistro())
                i++;
            insertNonFull(s->children[i], key);
            root = s;
        } else {
            insertNonFull(root, key);
        }
    }
}

void BTree::insertNonFull(BTreeNode* node, Avion* key) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && node->keys[i]->getNumeroRegistro() > key->getNumeroRegistro()) {
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = key;
        node->n = node->n + 1;
    } else {
        while (i >= 0 && node->keys[i]->getNumeroRegistro() > key->getNumeroRegistro())
            i--;
        i++;
        if (node->children[i]->n == MaxDegree) {
            splitChild(node, i, node->children[i]);
            if (node->keys[i]->getNumeroRegistro() < key->getNumeroRegistro())
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    BTreeNode* z = new BTreeNode(child->leaf);
    z->n = Degree - 1;

    for (int j = 0; j < Degree - 1; j++)
        z->keys[j] = child->keys[j + Degree];

    if (!child->leaf) {
        for (int j = 0; j < Degree; j++)
            z->children[j] = child->children[j + Degree];
    }

    child->n = Degree - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = z;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[Degree - 1];
    parent->n = parent->n + 1;
}

Avion* BTree::search(const string& registration_number) {
    return (root == nullptr) ? nullptr : search(root, registration_number);
}

Avion* BTree::search(BTreeNode* node, const string& registration_number) {
    int i = 0;
    while (i < node->n && registration_number > node->keys[i]->getNumeroRegistro())
        i++;

    if (i < node->n && registration_number == node->keys[i]->getNumeroRegistro())
        return node->keys[i];

    if (node->leaf)
        return nullptr;

    return search(node->children[i], registration_number);
}

void BTree::remove(const string& registration_number) {
    if (!root) {
        return;
    }
    remove(root, registration_number);
    if (root->n == 0) {
        BTreeNode* tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];
        delete tmp;
    }
}

void BTree::remove(BTreeNode* node, const string& registration_number) {
    int id = 0;
    while (id < node->n && registration_number > node->keys[id]->getNumeroRegistro())
        ++id;

    if (id < node->n && registration_number == node->keys[id]->getNumeroRegistro()) {
        if (node->leaf)
            removeFromLeaf(node, id);
        else
            removeFromNonLeaf(node, id);
    }
    else {
        if (node->leaf) {
            cout << "The key " << registration_number << " does not exist in the tree\n";
            return;
        }

        bool flag = ((id == node->n) ? true : false);

        if (node->children[id]->n < Degree)
            fillNode(node, id);

        if (flag && id > node->n)
            remove(node->children[id - 1], registration_number);
        else
            remove(node->children[id], registration_number);
    }
}

void BTree::removeFromLeaf(BTreeNode* node, int id) {
    for (int i = id + 1; i < node->n; ++i)
        node->keys[i - 1] = node->keys[i];
    node->n--;
}

void BTree::removeFromNonLeaf(BTreeNode* node, int id) {
    Avion* k = node->keys[id];

    if (node->children[id]->n >= Degree) {
        Avion* pred = getPredecessor(node, id);
        node->keys[id] = pred;
        remove(node->children[id], pred->getNumeroRegistro());
    }
    else if (node->children[id + 1]->n >= Degree) {
        Avion* succ = getSuccessor(node, id);
        node->keys[id] = succ;
        remove(node->children[id + 1], succ->getNumeroRegistro());
    }
    else {
        mergeNodes(node, id);
        remove(node->children[id], k->getNumeroRegistro());
    }
}

void BTree::fillNode(BTreeNode* node, int id) {
    if (id != 0 && node->children[id - 1]->n >= Degree)
        borrowFromPrev(node, id);
    else if (id != node->n && node->children[id + 1]->n >= Degree)
        borrowFromNext(node, id);
    else {
        if (id != node->n)
            mergeNodes(node, id);
        else
            mergeNodes(node, id - 1);
    }
}

void BTree::borrowFromPrev(BTreeNode* node, int id) {
    BTreeNode* child = node->children[id];
    BTreeNode* sibling = node->children[id - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[id - 1];

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];

    node->keys[id - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void BTree::borrowFromNext(BTreeNode* node, int id) {
    BTreeNode* child = node->children[id];
    BTreeNode* sibling = node->children[id + 1];

    child->keys[(child->n)] = node->keys[id];

    if (!(child->leaf))
        child->children[(child->n) + 1] = sibling->children[0];

    node->keys[id] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

void BTree::mergeNodes(BTreeNode* node, int id) {
    BTreeNode* child = node->children[id];
    BTreeNode* sibling = node->children[id + 1];

    child->keys[Degree - 1] = node->keys[id];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + Degree] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->children[i + Degree] = sibling->children[i];
    }

    for (int i = id + 1; i < node->n; ++i)
        node->keys[i - 1] = node->keys[i];

    for (int i = id + 2; i <= node->n; ++i)
        node->children[i - 1] = node->children[i];

    child->n += sibling->n + 1;
    node->n--;
}

Avion* BTree::getPredecessor(BTreeNode* node, int id) {
    BTreeNode* current = node->children[id];
    while (!current->leaf)
        current = current->children[current->n];
    return current->keys[current->n - 1];
}

Avion* BTree::getSuccessor(BTreeNode* node, int id) {
    BTreeNode* current = node->children[id + 1];
    while (!current->leaf)
        current = current->children[0];
    return current->keys[0];
}




void BTree::generateAvailablePlanesReport() {
    string dotContent = "digraph G {\n";
    dotContent += "    node [shape=record];\n";
    dotContent += "    Titulo [fontname=\"Courier New\", color=red, shape=box3d, label=\"Árbol B con aviones disponibles\"];\n";
    
    int nodeId = 0;
    generateRecursiveDOT(root, dotContent, nodeId);
    
    dotContent += "}\n";

    ofstream out("Reportes/Arbolb_aviones_disponibles.dot");
    out << dotContent;
    out.close();

    system("dot -Tpng Reportes/Arbolb_aviones_disponibles.dot -o Reportes/Arbolb_aviones_disponibles.png");
    system("start Reportes/Arbolb_aviones_disponibles.png");
}

void BTree::generateRecursiveDOT(BTreeNode* node, string& content, int& nodeId) {
    if (node == nullptr)
        return;

    int currentId = nodeId++;
    content += "    " + to_string(currentId) + "[label=\"";
    
    // Separadores entre claves (opcional)
    for (int i = 0; i < node->n; ++i) {
        if (i > 0)
            content += " - ";
        content += node->keys[i]->getNumeroRegistro();
    }

    content += "\", ";

    // Establecer estilo y color para el nodo
    content += "shape=record, style=filled, fillcolor=\"#e0f2e0\", fontname=\"Arial\", fontsize=12, penwidth=2.0";

    content += "];\n";

    if (!node->leaf) {
        for (int i = 0; i <= node->n; ++i) {
            if (node->children[i] != nullptr) {
                int childId = nodeId;
                content += "    " + to_string(currentId) + " -> " + to_string(childId) + " [color=\"#006400\", headport=\"n\"];\n"; // Cambio de color de flecha a verde obscuro y conexión desde el centro
                generateRecursiveDOT(node->children[i], content, nodeId);
            }
        }
    }
}


#endif  // B_TREE_H_CUSTOM
