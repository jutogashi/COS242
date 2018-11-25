
#include "Tsp.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "Heap.h"
#include <stack>
#include <vector>
#include <limits>
#include <unordered_map>


using namespace std;


Tsp::Tsp(string path)
{
    ifstream myFile;
    
    
    myFile.open(path);
    cout << endl << "Lendo arquivo " << path << " Por favor aguarde." << endl;
    if (!myFile)
        //Se o arquivo não existir
    {
        cout << "Arquivo " << path << " Nao foi encontrado.";
        return;
    }
    
    myFile >> m_numero_de_vertices;
    // Salva o numero de vertices totais do grafo
    m_pTsp = new TspNode*[m_numero_de_vertices+1]();
    
    // aloca espaço para o vetor de tamanho igual ao numero de vertices +1, para nõ precisarmos
    //nos preocupar com o 0.
    float* x_axis = new float[m_numero_de_vertices+1]();
    float* y_axis = new float[m_numero_de_vertices+1]();
    
    int n = 1;
    string s;
    //guarda cada linha do arquivo
    while (getline(myFile, s))
    {
        if (s.empty() == false)
        {
            
            istringstream tmp(s);
            float v1, v2;
            
            tmp >> v1 >> v2;
            // define para cada vertice suas coordenadas x e y retiradas do arquivo
            x_axis[n] = v1;
            y_axis[n] = v2;
            
            // cria arestas entre o novo vertice, e todos os ja anteriormente computados
            for( int i=1 ; i<n; i++ )
            {
                float distancia = sqrt(((x_axis[i]) - (x_axis[n]))* ((x_axis[i]) - (x_axis[n])) + ((y_axis[i]) - (y_axis[n]))*((y_axis[i]) - (y_axis[n])));
                
                addAresta(n,i,distancia);
                addAresta(i,n, distancia);
            }
            
            n++;
            
        }
    }
    
    
    // constroi arvore geradora mínima desse garfo
    MST(1);
    
    //duplicam-se as arestas
    vector< vector<int> > adj1;
    adj1.resize(m_numero_de_vertices+1);
    // vector to store final circuit
    vector<int> m_circuit;
    
    for (int i = 1; i<=m_numero_de_vertices; i++)
    {
        if (m_pai[i] != 0)
        {

            adj1[i].push_back(m_pai[i]);
            adj1[m_pai[i]].push_back(i);
            
        }
    }
    
    
    // algoritmo que encontrara um ciclo euleriano valido dado a arvore geradora miinima com arestas duplas direcionadas

    printCircuit(adj1);
    

    
    
     comprimento = 0;
     int* marcacao = new int[m_numero_de_vertices+1]();
     int* percurso = new int[m_numero_de_vertices+1]();
     int y = 0;
     for (long i = m_circuit.size() - 1; i >= 0; i--)
     {
     int x = m_circuit[i];
     cout << m_circuit[i] << ", ";
     if (marcacao[x] == 0)
     {
     marcacao[x] = 1;
     percurso[y] = x;
     y++;
     }
     }
     percurso[m_numero_de_vertices + 1] = percurso[0];
    
    
     ofstream myTspFile;
     myTspFile.open(m_savePath + "/tsp.txt");
     for (int i = 0; i <= m_numero_de_vertices; i++)
     {
     myTspFile <<"(" << x_axis[percurso[i]] <<", " << y_axis[percurso[i]] << ") " ;
     comprimento += sqrt(((x_axis[i]) - (x_axis[i+1]))* ((x_axis[i]) - (x_axis[i+1])) + ((y_axis[i]) - (y_axis[i+1]))*((y_axis[i]) - (y_axis[i+1])));
    
    if (i != m_numero_de_vertices) myTspFile << " -> ";
     }
     myTspFile << endl << comprimento;
    


}




void Tsp::addAresta(int de, int para, float peso)
{
    TspNode* no = new TspNode;
    no->vertice = para;
    if (m_pTsp[de] != NULL)
    {
        m_pTsp[de]->pPrev = no;
    }
    no->pNext = m_pTsp[de];
    no->pPrev = NULL;
    // peso calcula a distancia euclidiana entre os dois pontos;
    no->peso = peso;
    this->m_pTsp[de] = no;
    
}

void Tsp::MST(int s)
{

    
    // inicializa heap e vetores auxiliares
    Heap MST_heap;
    m_distancia = new float[m_numero_de_vertices + 1]();
    m_pai = new int[m_numero_de_vertices + 1]();
    m_conjuntoS = new int[m_numero_de_vertices + 1]();
    
    //define infinito
    float infinito = std::numeric_limits<float>::max();
    
    // find eé usado para podermos acessar um nó em O(1) ja que a funcao find implementada no heap é em O(n),
    //o que é importante pois o nóé necessário para fazer a operaçao de alteração de uma chave do heap
    node **find = new node*[m_numero_de_vertices + 1]();
    
    //inicializa vetor distancia como infinito, menos para o vertice s que é 0, e add o vertice s no heap
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        if (i != s)
        {
            m_distancia[i] = infinito;
        }
        else MST_heap.insert(0, s);
    }
    
    while (MST_heap.isEmpty() == false)
    {
        node* v = MST_heap.getMinimum();  //pega valor minimo do heap
        int vertice = v->Vertice;
        m_conjuntoS[vertice] = 1;
        
        MST_heap.removeMinimum();  // remove minimo do heap
        
        for (TspNode* w = m_pTsp[vertice]; w != NULL;)
        {
            //percorre os vizinhos desse vertice tirado do heap
            if ((m_distancia[w->vertice] > w->peso) && (m_conjuntoS[w->vertice] == 0))
            {
                //se esse vertice ainda nao foi adicionado no heap, adiciona e atualiza vetores auxiliares
                if ((m_distancia[w->vertice] = !infinito))
                {
                    find[w->vertice] = MST_heap.decreaseKey(find[w->vertice],  w->peso);
                    m_distancia[w->vertice] = w->peso;
                    m_pai[w->vertice] = vertice;
                }
                //se esse vertice ja foi adicionado no heap, atualiza e atualiza vetores auxiliares
                else
                {
                    find[w->vertice]=MST_heap.insert( w->peso, w->vertice);
                    m_distancia[w->vertice] =  w->peso;
                    m_pai[w->vertice] = vertice;
                }
            }
            w = w->pNext;
        }
    }
}




void Tsp::printCircuit(vector< vector<int> > adj)
{
    

    
    // adj represents the adjacency list of
    // the directed graph
    // edge_count represents the number of edges
    // emerging from a vertex
    unordered_map<int,int> edge_count;
    
    for (int i=0; i<adj.size(); i++)
    {
        //find the count of edges to keep track
        //of unused edges
        edge_count[i] = adj[i].size();
    }
    
    if (!adj.size())
        return; //empty graph
    
    // Maintain a stack to keep vertices
    stack<int> curr_path;
    
    
    
    // start from any vertex
    curr_path.push(1);
    int curr_v = 1; // Current vertex
    
    while (!curr_path.empty())
    {
        // If there's remaining edge
        if (edge_count[curr_v])
        {
            // Push the vertex
            curr_path.push(curr_v);
            
            // Find the next vertex using an edge
            int next_v = adj[curr_v].back();
            
            // and remove that edge
            edge_count[curr_v]--;
            adj[curr_v].pop_back();
            
            // Move to next vertex
            curr_v = next_v;
        }
        
        // back-track to find remaining circuit
        else
        {
            m_circuit.push_back(curr_v);

            // Back-tracking
            curr_v = curr_path.top();
            curr_path.pop();
        }
    }
    
    
    
    ofstream mycircuitoFile;
    mycircuitoFile.open(m_savePath + "/circuito.txt");
    
    // we've got the circuit, now print it in reverse
    for (long i = m_circuit.size() - 1; i >= 0; i--)
    {
        
        mycircuitoFile<< m_circuit[i] <<endl;
        
    }
    
    
    
    
    
    
    
    
}
