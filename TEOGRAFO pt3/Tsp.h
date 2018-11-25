
#include <stdio.h>

#include "Grafo.h"
#include <vector>

using namespace std;

struct TspNode
{
    int vertice;
    float peso;
    TspNode* pNext;
    TspNode* pPrev;
    
};


class Tsp :
public Grafo
{
public:
    Tsp(string path);
    TspNode * * m_pTsp;


    
    protected :
    
    float* x_axis;
    float* y_axis;
    
    void addAresta( int de, int para, float peso);
    void MST(int s);

    float * m_distancia;
    int* m_pai;
    int* m_conjuntoS;

    vector<int> m_circuit;
    long comprimento;
    int* percurso = new int[m_numero_de_vertices+1]();
    int* marcacao;
    
    
    
    void printCircuit(vector< vector<int> > adj);
    vector< vector<int> > adj1;
    
};
