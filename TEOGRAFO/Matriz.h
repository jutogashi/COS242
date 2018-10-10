//
//  Matriz.hpp
//  Teo
//
//  Created by Julia on 03/09/18.
//  Copyright © 2018 Julia. All rights reserved.
//

#ifndef Matriz_hp
#define Matriz_hp

#include <stdio.h>

#include "Grafo.h"
#include <string>
#include <queue>
#include <stack>

#include <chrono>
using namespace std::chrono;

class Matriz :
public Grafo
{
public:
    Matriz(string path);
    ~Matriz();
    
private:
    void construtor();
    stack<Arvore*> pilha;
    bool** m_matriz;
    void addAresta(int i, int j);
    int *m_grau;
    queue<Arvore*> fila;
    int *m_vetor_de_marcacao;
    void BFS_Matriz(int s);
    void DFS_matriz(int s);
    Arvore * Parentesco(Arvore * v, int w);
    
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    
    
};


#endif /* Matriz_hp */
