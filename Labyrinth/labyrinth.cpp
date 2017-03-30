//
//  main.cpp
//  Labyrint1.2
//
//  Created by Adam Zvada on 27.10.16.
//  Copyright © 2016 Adam Zvada. All rights reserved.
//


/*
 / Getting shortest path in graph. Some verticies require acces card {a,b,c}.
 / Access card can be picked on nodes.
 / Using BFS...
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>


#define possibleCardCombinations 8

using namespace std;

enum States {FRESH, OPEN, BLOCKED, CLOSED};

enum Cards {A = 'a', B = 'b', C = 'c', X = 'x'};

class Node;
class NodeInfo;
class Vertex;



//
//---------------------------*FIFO*------------------------------
//


class NodeList {
public:
    Node * val;
    NodeList * nextNode;
};

class Queue {
    NodeList * root;
    NodeList * last;
    int size;
    
public:
    Queue();
    Node* dequeue();
    void enqueue(Node* val);
    int getSize();
    bool isEmpty();
};

Queue::Queue() {
    this->size = 0;
    this->root = NULL;
    this->last = NULL;
}

Node* Queue::dequeue() {
    
    if (this->isEmpty()) {
        cout << "Queue is empty" << endl;
        return NULL;
    }
    
    Node * retVal = this->root->val;
    NodeList * curRoot = this->root;
    
    this->root = curRoot->nextNode;
    this->size--;
    delete curRoot;
    
    return retVal;
}

void Queue::enqueue(Node* val) {
    NodeList * newNode = new NodeList();
    newNode->val = val;
    newNode->nextNode = NULL;
    
    if (!this->isEmpty()) {
        this->last->nextNode = newNode;
        this->last = newNode;
    } else {
        this->last = newNode;
        this->root = newNode;
    }
    
    this->size++;
}

bool Queue::isEmpty() {
    if (this->size == 0) {
        return true;
    }
    return false;
}


//
//---------------------------*NODE*------------------------------
//

class Node {
public:
    Vertex * verticies;
    int numVertex;
    
    int value;
    
    int  distance;
    States state;
    Node * predecessor;
    
    bool accessCardToPick[3];
    
    pair<int, int> nodeVal;             //pair of node value and its access cards
    
    Node();
    
    void setValue(int value);
    void setCombCard(int comb);
    void setValueAndCardComb(int value, int comb);
    int getValueFromBinary(bool * comb);
private:
    int allocSizeVertex;
    
};

class Vertex {
public:
    Node * vertex;                      //shelf copy
    Cards authorizationCard;
private:
    
};

int getValueFromBinary(bool * comb) {
    return 4*comb[2] + 2*comb[1] + comb[0];
}

Node::Node() {
    
    state = FRESH;
    predecessor = NULL;
    
    distance = INT16_MAX;
    
    accessCardToPick[0] = false;
    accessCardToPick[1] = false;
    accessCardToPick[2] = false;
    
    verticies = NULL;
    numVertex = 0;
    
}


void Node::setValueAndCardComb(int value, int comb) {
    this->value = value;
    
    this->nodeVal.first = value;
    this->nodeVal.second = comb;
}

//
//---------------------------*GRAPH*------------------------------
//

class Graph {
public:
    Node * nodes;           //nodes*(2^3)
    int numNodes;
    int allocatedNodes;
    
    Graph(int numNodes);
    void BFS(int startIndex, int endNode);
    int pathReconstruction(int indexStart, int indexEnd);
    
    void addVertex(int indexParent, int indexChild, int count, int numVerticies, Cards accessCardRequired);
    void addAccessCardToNode(int indexNode, Cards cardForNode);
    void printGraph();
    bool mayAccessNode(char card, Node * fromNode);
    Node * checkForDroppedCards(Node * currentNode);
private:
    int initNumOfVerticies(int indexParent, int numVertex);
    
};

Graph::Graph(int numNodes) {
    
    this->numNodes = numNodes;
    this->allocatedNodes = numNodes*possibleCardCombinations;
    
    nodes = new Node[numNodes*possibleCardCombinations];
    
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < possibleCardCombinations; j++) {
            nodes[i*possibleCardCombinations+j].setValueAndCardComb(i, j);
        }
    }
}


void Graph::BFS(int startIndex, int endIndex) {
    
    Queue * queue = new Queue();
    
    
    Node * startNode = &nodes[startIndex*possibleCardCombinations];
    
    //Check if there is access card to pick, if so, change the node to its right node combination
    startNode = this->checkForDroppedCards(&nodes[startIndex*possibleCardCombinations]);
    
    startNode->distance = 0;
    queue->enqueue(startNode);
    startNode->state = OPEN;
    
    
    
    while (!queue->isEmpty()) {
        Node * curNode = queue->dequeue();
        //cout << "DEQUEUE: " << curNode->value << " " << curNode->nodeVal.second << endl;
        
        if (curNode->value == this->nodes[endIndex*possibleCardCombinations].value) {
            break;
        }
        
        int numOfAdjacent = curNode->numVertex;
        for (int i = 0; i < numOfAdjacent; i++) {
            Node * adjacentNode = curNode->verticies[i].vertex;
            //cout << "----- AdjNode: " << adjacentNode->value << " " << adjacentNode->nodeVal.second << endl;
            
            char cardRequired = curNode->verticies[i].authorizationCard;
            
            
            if(mayAccessNode(cardRequired, curNode)) {
                //Check if there is access card to pick, if so, change the node to its right node combination
                adjacentNode = this->checkForDroppedCards(adjacentNode);
                
                if (adjacentNode->state == FRESH) {
                    adjacentNode->state = OPEN;
                    adjacentNode->distance = curNode->distance + 1;
                    adjacentNode->predecessor = curNode;
                    
                    queue->enqueue(adjacentNode);
                    
                }
            }
        }
    
    }
}

int Graph::pathReconstruction(int indexStart, int indexEnd) {
    
    int distance = INT16_MAX;
    int indexComb = -1;
    
    for (int i = 0; i < possibleCardCombinations; i++) {
        Node * endNode = &this->nodes[indexEnd*possibleCardCombinations + i];
        if (endNode->distance < distance) {
            distance = endNode->distance;
            indexComb = i;
        }
    }
    
    if (indexComb == -1 ) {
        cout << "No solution" << endl;
        return 1;
    }
    
    Node * curNode = &this->nodes[indexEnd*possibleCardCombinations + indexComb];
    Node * startNode = this->checkForDroppedCards(&this->nodes[indexStart*possibleCardCombinations]);

    int * path = new int[distance+1];
    
    ostringstream oss;
    
    int i = 0;
    while (startNode != curNode) {
        path[i] = curNode->value;
        curNode = curNode->predecessor;
        i++;
    }
    path[distance] = curNode->value;
    
    
    cout << distance << endl;
    for (int i = 0; i < distance+1; i++) {
        cout << path[distance-i] << " ";
    }
    cout << endl;

    delete [] path;
    
    return 0;
}


int Graph::initNumOfVerticies(int indexParent, int numVertex) {
    
    this->nodes[indexParent].numVertex = numVertex;
    
    this->nodes[indexParent].verticies = new Vertex[numVertex];
    
    return 0;
}


void Graph::addVertex(int indexParent, int indexChild, int count, int numVerticies, Cards accessCardRequired) {
    
    for (int i = 0; i < possibleCardCombinations; i++) {
        int scaleIndexParent = indexParent*possibleCardCombinations + i;
        int scaleIndexChild = indexChild*possibleCardCombinations + i;
        
        if (this->nodes[scaleIndexParent].numVertex == 0) {
            initNumOfVerticies(scaleIndexParent, numVerticies);
        }
    
        this->nodes[scaleIndexParent].verticies[count].vertex = &this->nodes[scaleIndexChild];
        this->nodes[scaleIndexParent].verticies[count].authorizationCard = accessCardRequired;
    }
}


void Graph::addAccessCardToNode(int indexNode, Cards cardForNode) {
    
    if (cardForNode == 'a') {
        for(int i = 0; i < possibleCardCombinations; i++) {
            this->nodes[indexNode*possibleCardCombinations + i].accessCardToPick[2] = true;
        }
    } else if (cardForNode == 'b') {
        for(int i = 0; i < possibleCardCombinations; i++) {
            this->nodes[indexNode*possibleCardCombinations + i].accessCardToPick[1] = true;
        }
    } else if (cardForNode == 'c') {
        for(int i = 0; i < possibleCardCombinations; i++) {
            this->nodes[indexNode*possibleCardCombinations + i].accessCardToPick[0] = true;
        }
    }
}
    
bool Graph::mayAccessNode(char card, Node * curNode) {
    
    if (card == 'x') {
        return true;
    } else if (card == 'a' && (curNode->nodeVal.second & 4)) {
        return  true;
    } else if (card == 'b' && (curNode->nodeVal.second & 2)) {
        return true;
    } else if (card == 'c' && (curNode->nodeVal.second & 1)) {
        return true;
    }
    
    return false;
}

Node* Graph::checkForDroppedCards(Node * currentNode) {
    
    int myCardComb = currentNode->nodeVal.second;
    
    //bool hasMyCardA = (myCardComb & 0b100);
    //bool hasMyCardB = (myCardComb & 0b010);
    //bool hasMyCardC = (myCardComb & 0b001);
    
    bool hasMyCardA = (myCardComb & 4);
    bool hasMyCardB = (myCardComb & 2);
    bool hasMyCardC = (myCardComb & 1);
    
    if (hasMyCardA == false && currentNode->accessCardToPick[2] == true) {
        hasMyCardA = true;
    }
    
    if (hasMyCardB == false && currentNode->accessCardToPick[1] == true) {
        hasMyCardB = true;
    }
    
    if (hasMyCardC == false && currentNode->accessCardToPick[0] == true) {
        hasMyCardC = true;
    }
    
    int myNewCardComb = 4*hasMyCardA + 2*hasMyCardB + hasMyCardC;
    
//    cout << "CHANGING: " << currentNode->nodeVal.first << "." << currentNode->nodeVal.second << " - " << this->nodes[currentNode->nodeVal.first*possibleCardCombinations + myNewCardComb].nodeVal.first << "." << this->nodes[currentNode->nodeVal.first*possibleCardCombinations + myNewCardComb].nodeVal.second << endl;
//    
    
    return &this->nodes[currentNode->nodeVal.first*possibleCardCombinations + myNewCardComb];

}


void Graph::printGraph() {
    
    for (int i = 0; i < this->numNodes; i++) {
        
        //Node * thisNode = &this->nodes[i];
        
        cout << "{ " << this->nodes[i*possibleCardCombinations].value << " ";
        for (int j = 0; j < 3; j++) {
            cout << this->nodes[i*possibleCardCombinations].accessCardToPick[j];
        }
        cout << " } " << " -> ";
        
        for (int j = 0; j < this->nodes[i*possibleCardCombinations].numVertex; j++) {
            cout << this->nodes[i*possibleCardCombinations].verticies[j].vertex->value << " + ";
            cout << (char)this->nodes[i*possibleCardCombinations].verticies[j].authorizationCard << " -> ";
        }
        
        cout << endl;
        
    }
}


int main(int argc, const char * argv[]) {
    
    int numNodes;
    int startNode;
    int finalNode;
    
    //add verification of input
    if(scanf("%d %d %d", &numNodes, &startNode, &finalNode) != 3 || numNodes <= startNode || numNodes <= finalNode) {
        cout << "Nespravny vstup!" << endl;
        return 1;
    }
    
    Graph * graph = new Graph(numNodes);
    
    for (int i = 0; i < numNodes; i++) {
        int numVerticies;
        scanf("%d", &numVerticies);
        
        for (int j = 0; j < numVerticies; j++) {
            int indexChild;
            char accesCardNeeded;
            scanf("%d %c", &indexChild, &accesCardNeeded);
            
            graph->addVertex(i, indexChild, j, numVerticies, (Cards)accesCardNeeded);
        }
        
    }
    int numOfAccessCards;
    scanf("%d", &numOfAccessCards);
    
    for (int i = 0; i < numOfAccessCards; i++) {
        int indexNode;
        char accessCard;
        scanf("%d %c", &indexNode, &accessCard);
        
        graph->addAccessCardToNode(indexNode, (Cards)accessCard);
    }
    
    //graph->printGraph();
    
    graph->BFS(startNode, finalNode);
    
    graph->pathReconstruction(startNode, finalNode);
    
    /*
    graph->BFS(startNode);
    
    for (int i = 0; i < numNodes; i++) {
        if (-1 != startNode) {
            cout << "(" << i << ") " << graph->nodes[i].predecesor->value << endl;
        }
    }
    
    graph->pathReconstruction(startNode, finalNode);
    */
    
    delete  graph;
    
    
    return 0;
}
