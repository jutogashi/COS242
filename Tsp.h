#pragma once
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
	TspNode ** m_pTspMst;
	int *m_edge_count;

protected :

	float* x_axis;
	float* y_axis;

	void addAresta( int de, int para, float peso);
	void MST(int s);
	void addArestaMST(int de, int para, float peso);
	float * m_distancia;
	int* m_pai;
	int* m_conjuntoS;
	//void CicloEuleriano(TspNode** Grafo,int* graus);
	vector<int> m_circuit;
	float comprimento;
	int* percurso;
	int* marcacao;



	void printCircuit(vector< vector<int> > adj);
	vector< vector<int> > adj1;
	
};

