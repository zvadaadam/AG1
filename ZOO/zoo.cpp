//
//  main.cpp
//  ZOO_krmeni
//
//  Created by Adam Zvada on 07.11.16.
//  Copyright © 2016 Adam Zvada. All rights reserved.
//


/*
 / Information system for ZOO, has some instruction to feed animals :)
 / Using heap max & min
 /
*/

#include <iostream>

#define MAX_VANS 10002

using namespace std;


//-----------------NODE-----------------

struct Node {
    uint32_t animalID;
    uint32_t animalPriority;
    bool * isDeleted;
    
    Node * parentNode;
    Node * leftChild;
    Node * rightChild;
};

//-----------------VECTOR-----------------

template <class T>
class Vector {
public:
    T * data;
    uint32_t numElemets;
    
    Vector();
    ~Vector();
    
    void push_back(T & data);
private:
    uint32_t count;
    void resize();
};

template <class T>
Vector<T>::Vector() {
    data = new T[5];
    count = 5;
    
    numElemets = 1;
}

template <class T>
void Vector<T>::push_back(T & data) {
    
    if (numElemets >= count) {
        resize();
    }
    
    this->data[numElemets] = data;
    numElemets++;
}

template <class T>
void Vector<T>::resize() {
    
    T * newNode = new T[count*2];
    count *= 2;
    
    for (uint32_t i = 0; i < numElemets; i++) {
        newNode[i] = data[i];
    }
    
    delete [] data;
    
    data = newNode;
}

template <class T>
Vector<T>::~Vector() {
    delete [] data;
}


//-----------------BINOMIAL_HEAP-----------------

class BinomialHeap {
public:
    Node * heaps;
    
    BinomialHeap();

private:
};

//-----------------HEAP-----------------

class Heap {
public:
    //Node * nodes;
    uint32_t heapSizeMin;
    uint32_t heapSizeMax;
    
    uint32_t realHeapSize;
    
    Vector<Node> * nodesMin;
    Vector<Node> * nodesMax;
    
    Heap(Vector<Node> * nodes);
    Heap();
    
    uint32_t extract();
    uint32_t extractMin();
    uint32_t extractMax();
    
    void setMaxDelete();
    void setMinDelete();
    
    void insert(Node * node);
    void insertMax(Node * node);
    void insertMin(Node * node);
    
    void deleteMax(uint32_t index);
    void deleteMin(uint32_t index);
    
    void buildHeapMax();
    void buildHeapMin();
    
    void printHeap();
private:
    void heapifyMax(uint32_t i);
    void heapifyMin(uint32_t i);
    void swap(Node & n1, Node & n2);
};


Heap::Heap(Vector<Node> * nodes) {

    for (uint32_t i = 0; i < nodes->numElemets; i++) {
        nodes->data[i].isDeleted = new bool(false);
    }

    
    this->nodesMax = nodes;
    this->nodesMin = nodes;
    
    heapSizeMin = nodes->numElemets-1;
    heapSizeMax = nodes->numElemets-1;
    
    realHeapSize = nodes->numElemets-1;
}

Heap::Heap() {
    heapSizeMin = 0;
    heapSizeMax = 0;
    
    realHeapSize = 0;
    
    nodesMin = new Vector<Node>();
    nodesMax = new Vector<Node>();
}

void Heap::insert(Node * node) {
    
    //memory leak, same field add
    node->isDeleted = new bool;
    *node->isDeleted = false;
    
    realHeapSize++;
    
    insertMax(node);
    
    insertMin(node);
}

void Heap::insertMax(Node * node) {
    
    heapSizeMax++;
    nodesMax->push_back(*node);
    
    uint32_t indexParent = heapSizeMax/2;
    uint32_t indexChild = heapSizeMax;
    
    while (indexParent > 0 && nodesMax->data[indexParent].animalPriority <= node->animalPriority) {
        
        if (nodesMax->data[indexParent].animalPriority == node->animalPriority &&
            nodesMax->data[indexParent].animalID > node->animalID) {
            swap(nodesMax->data[indexParent], nodesMax->data[indexChild]);
        } else if (nodesMax->data[indexParent].animalPriority != node->animalPriority) {
            swap(nodesMax->data[indexParent], nodesMax->data[indexChild]);
        }
        
        indexChild = indexParent;
        indexParent = indexParent/2;
    }
}

void Heap::insertMin(Node * node) {
    heapSizeMin++;
    
    nodesMin->push_back(*node);
    
    uint32_t indexParent = heapSizeMin/2;
    uint32_t indexChild = heapSizeMin;
    
    while (indexParent > 0 && nodesMin->data[indexParent].animalPriority >= node->animalPriority) {
        
        if (nodesMin->data[indexParent].animalPriority == node->animalPriority &&
            nodesMin->data[indexParent].animalID > node->animalID) {
            swap(nodesMin->data[indexParent], nodesMin->data[indexChild]);
        } else if (nodesMin->data[indexParent].animalPriority != node->animalPriority) {
            swap(nodesMin->data[indexParent], nodesMin->data[indexChild]);
        }
        
        indexChild = indexParent;
        indexParent = indexParent/2;
    }
}

void Heap::setMaxDelete() {
    *nodesMax->data[1].isDeleted = true;
}

void Heap::setMinDelete() {
    *nodesMin->data[1].isDeleted = true;
}


//Returns Animal ID with max priority
uint32_t Heap::extractMax() {
    if (realHeapSize == 0) {
        cout << "empty" << endl;
        return -1;
    }
    
    while (*nodesMax->data[1].isDeleted == true) {
        this->deleteMax(1);
    }

    setMaxDelete();
    
    uint32_t max = nodesMax->data[1].animalID;
    
    nodesMax->data[1] = nodesMax->data[heapSizeMax];
    nodesMax->numElemets--;
    heapSizeMax--;
    
    realHeapSize--;
    
    heapifyMax(1);
    
    
    cout << max << endl;
    
    return max;
}

uint32_t Heap::extractMin() {
    if (realHeapSize == 0) {
        cout << "empty" << endl;
        return -1;
    }
    
    while (*nodesMin->data[1].isDeleted == true) {
        this->deleteMin(1);
    }

    
    setMinDelete();
    
    uint32_t min = nodesMin->data[1].animalID;
    
    nodesMin->data[1] = nodesMin->data[heapSizeMin];
    nodesMin->numElemets--;
    heapSizeMin--;
    
    realHeapSize--;
    
    heapifyMin(1);
    
    cout << min << endl;
    
    return min;
}

void Heap::deleteMax(uint32_t index) {
    nodesMax->data[index] = nodesMax->data[heapSizeMax];
    nodesMax->numElemets--;
    heapSizeMax--;
    heapifyMax(index);
}

void Heap::deleteMin(uint32_t index) {
    nodesMin->data[index] = nodesMin->data[heapSizeMin];
    nodesMin->numElemets--;
    heapSizeMin--;
    heapifyMin(index);
}

void Heap::buildHeapMax() {
    for (uint32_t i = heapSizeMax/2; i > 0; i--) {
        heapifyMax(i);
    }
}

void Heap::buildHeapMin() {
    for (uint32_t i = heapSizeMin/2; i > 0; i--) {
        heapifyMin(i);
    }
}

void Heap::heapifyMax(uint32_t i) {
    uint32_t indexLeft = 2*i;
    uint32_t indexRight = 2*i + 1;
    uint32_t largest;
    
    if (indexLeft <= heapSizeMax && (nodesMax->data[indexLeft].animalPriority > nodesMax->data[i].animalPriority
                || (nodesMax->data[indexLeft].animalPriority == nodesMax->data[i].animalPriority
                && nodesMax->data[indexLeft].animalID < nodesMax->data[i].animalID))) {
        largest = indexLeft;
    } else {
        largest = i;
    }
    
    if (indexRight <= heapSizeMax && (nodesMax->data[indexRight].animalPriority > nodesMax->data[largest].animalPriority
            || (nodesMax->data[indexRight].animalPriority == nodesMax->data[largest].animalPriority
            && nodesMax->data[indexRight].animalID < nodesMax->data[largest].animalID))) {
        largest = indexRight;
    }
    
    if (largest != i) {
        swap(nodesMax->data[i], nodesMax->data[largest]);
        heapifyMax(largest);
    }
}

void Heap::heapifyMin(uint32_t i) {
    uint32_t indexLeft = 2*i;
    uint32_t indexRight = 2*i + 1;
    uint32_t largest;
    
    if (indexLeft <= heapSizeMin && (nodesMin->data[indexLeft].animalPriority < nodesMin->data[i].animalPriority
            || (nodesMin->data[indexLeft].animalPriority == nodesMin->data[i].animalPriority
            && nodesMin->data[indexLeft].animalID < nodesMin->data[i].animalID))) {
        largest = indexLeft;
    } else {
        largest = i;
    }
    
    if (indexRight <= heapSizeMin && (nodesMin->data[indexRight].animalPriority < nodesMin->data[largest].animalPriority
            || (nodesMin->data[indexRight].animalPriority == nodesMin->data[largest].animalPriority
            && nodesMin->data[indexRight].animalID < nodesMin->data[largest].animalID))) {
        largest = indexRight;
    }
    
    if (largest != i) {
        swap(nodesMin->data[i], nodesMin->data[largest]);
        heapifyMin(largest);
    }
}


void Heap::swap(Node & n1, Node & n2) {
    Node tmp = n1;
    n1 = n2;
    n2 = tmp;
}

void Heap::printHeap() {
    cout << "MAX: ";
    for (uint32_t i = 1; i <= heapSizeMax; i++) {
        if (*nodesMax->data[i].isDeleted == true) {
            cout << "(" << nodesMax->data[i].animalID << ")";
        } else {
            cout << nodesMax->data[i].animalID << " ";
        }
    }
    cout << endl;
    
    cout << "MIN: ";
    for (uint32_t i = 1; i <= heapSizeMin; i++) {
        if (*nodesMin->data[i].isDeleted == true) {
            cout << "(" << nodesMin->data[i].animalID << ")";
        } else {
            cout << nodesMin->data[i].animalID << " ";
        }
    }
    cout << endl;

}


//-----------------MAIN-----------------

int main(int argc, const char * argv[]) {
    

    Heap * heapArr = new Heap[MAX_VANS];
    
    int instruction;
    
    do {
        
        scanf("%d", &instruction);
        
        uint16_t van;
        uint32_t animalID;
        uint32_t animalPriority;
        
        switch (instruction) {
            case 0:
            {
                //"K ID S" jsou přirozená čísla. Tento příkaz říká přidej náklaďáku K do seznamu zvířátko ID se silou S.
                
                scanf("%hu %u %u", &van, &animalID, &animalPriority);
                Node * node = new Node;
                node->animalID = animalID;
                node->animalPriority = animalPriority;
                
                heapArr[van].insert(node);

                break;
            }
            case 1:
            {
                //"K" kde K je přirozené číslo. Tento příkaz znamená nakrm (a odstraň) od náklaďáku K nejsilnější zvířátko a vypiš jeho ID na výstup na samostatný řádek.
                scanf("%hu", &van);
                heapArr[van].extractMax();
                break;
            }
            case 2:
            {
                //"K" kde K je přirozené číslo Tento příkaz znamená nakrm (a odstraň) od náklaďáku K nejslabší zvířátko a vypiš jeho ID na výstup na samostatný řádek.
                scanf("%hu", &van);
                heapArr[van].extractMin();
                break;
            }
            case 3:
                //"K1 K2" kde K1, K2 jsou přirozená čísla (může se stát ze K1 = K2) Tento příkaz znamená přidej zvířátka od náklaďáku K2 k náklaďáku K1.
                uint16_t vanFrom;
                scanf("%hu %hu", &van, &vanFrom);
                
                //Merge Vans, TODO
                
                break;
            case 4:
                //Tento příkaz znamená vypni informační systém; je zaručeně jako poslední na vstupu
                break;
            default:
                cout << "Nespravny vstup." << endl;
                return 1;
                break;
        }
    } while (instruction != 4);
    
    return 0;
}
