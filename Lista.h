#pragma once
#include "Grafo.h"
#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <chrono>

using namespace std::chrono;

using namespace std;



struct ListNode
{
	int vertice;
	int peso;
	ListNode* pNext;
	ListNode* pPrev;

};


class Lista :
	public Grafo
{
public:

	Lista(string path);
	~Lista();


protected:

	stack<Arvore*> pilha; //pilha usada na DFS
	queue<Arvore*> fila;   //fila usada na BFS
	int *m_vetor_de_marcacao;   //vetor de marcação dos vertices usado na BFS e na DFS
	void addAresta(int de, int para, float peso);   //cria aresta na lista de adjacencia
	ListNode** m_pLista;
	void Grau();
	void BFS_lista(int s);

	void DFS_lista(int s);

	int m_tamanho_da_componente_conexa;
	int m_numero_de_componentes_conexas = 0;
	int* m_componentes_conexos;
	int m_x;
	int m_maior_tamanho = 0;
	bool etapa;
	int m_i;
	Arvore* Parentesco(Arvore* v, int w);


	void FComponentes_conexas();


	std::chrono::time_point<std::chrono::system_clock> start, end;



	void Dijkstra(int s);

};

