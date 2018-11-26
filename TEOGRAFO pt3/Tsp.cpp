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
    //leitura do arquivo;
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
    
    // como diferentemente das outras partes, temos o arquivo com cordenadas, as guardaremos em vetores
    //correspondentes ao eixo x e y
    float* x_axis = new float[m_numero_de_vertices+1]();
    float* y_axis = new float[m_numero_de_vertices+1]();
    
    //variavel auxiliar para numerar os vertices, sendo que eles estão na ordem de leitura do arquivo, sendo o primeiro =1
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
                //calcula distancia euclidiana entre vertices, e adiciona as arestas na posição dos vetores 
                //na lista de adjacencia para ambos (grafo completo)
                float distancia = sqrt(((x_axis[i]) - (x_axis[n]))* ((x_axis[i]) - (x_axis[n])) + ((y_axis[i]) - (y_axis[n]))*((y_axis[i]) - (y_axis[n])));
                
                addAresta(n,i,distancia);
                addAresta(i,n, distancia);
            }
            
            //incrementa o numero do vertice
            n++;
            
        }
    }
    
    
    // constroi arvore geradora mínima desse garfo
    MST(1);
    
    //duplicam-se as arestas
    vector< vector<int> > adj1;
    adj1.resize(m_numero_de_vertices+1);
    // vetor para guardar circuito final
    vector<int> m_circuit;
    
    //cria nova lista de adjacencia para a arvore correspondente a MST com arestas direcionadas
    for (int i = 1; i<=m_numero_de_vertices; i++)
    {
        if (m_pai[i] != 0)
        {

            adj1[i].push_back(m_pai[i]);
            adj1[m_pai[i]].push_back(i);
            
        }
    }
    
    
    // algoritmo que encontrara um ciclo euleriano valido dado a arvore geradora miinima com arestas duplas direcionadas
    this->printCircuit(adj1);
   
    //gera arquivo de saida
     ofstream myTspFile;
     myTspFile.open(m_savePath + "/tsp.txt");
     for (int i = 0; i <= m_numero_de_vertices; i++)
     {
     //imprime caminho
     myTspFile << percurso[i] << " "  ;
        
    if (i != m_numero_de_vertices)
    {
    //calcula comprimento total do dado caminho
        comprimento += sqrt(((x_axis[percurso[i]]) - (x_axis[percurso[i+1]]))* ((x_axis[percurso[i]]) - (x_axis[percurso[i+1]])) + ((y_axis[percurso[i]]) - (y_axis[percurso[i+1]]))*((y_axis[percurso[i]]) - (y_axis[percurso[i+1]])));
     }
     }
    
    //imprime o comprimento
     myTspFile << endl << comprimento;

    
    return;

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
    // peso calculo da distancia euclidiana entre os dois pontos;
    no->peso = peso;
    this->m_pTsp[de] = no;
    
}

//semelhante a MST implementada em lista
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



//algoritmo para encontrar um circuito euleriano no grafo
void Tsp::printCircuit(vector< vector<int> > adj)
{
    //vetor auxiliar para construir circuito heuleriano
    int* marcacao = new int[m_numero_de_vertices+1]();


    // adj representa a lista de adjacencia do grafo direcionado
    // edge_count representa o numero de arestas saindo de um vertice

    unordered_map<int,int> edge_count;
    
    for (int i=0; i<adj.size(); i++)
    {
        //acha o numero de arestas para manter numero de arestas que ainda não foram utilizadas
        edge_count[i] = adj[i].size();
    }
    
    if (!adj.size())
        return; //grafo vazio
    
    // pilha para manter os vertices
    stack<int> curr_path;
    
    // começa de um vertice qualquer
    curr_path.push(1);
    int curr_v = 1; // Vertice atual
    
    while (!curr_path.empty())
    {
        // Se ainda há arestas sobrando
        if (edge_count[curr_v])
        {
            // bota o vertice na pilha
            curr_path.push(curr_v);
            
            // acha o proximo vertice usando uma aresta
            int next_v = adj[curr_v].back();
            
            // remove essa aresta
            edge_count[curr_v]--;
            adj[curr_v].pop_back();
            
            // passa para proximo vertice
            curr_v = next_v;
        }
        
        // volta para achar circuitos restantes
        else
        {
            m_circuit.push_back(curr_v);
            curr_v = curr_path.top();
            curr_path.pop();
        }
    }
    
   
    // variavel auxiliar para criar array com ciclo heuleriano
    int y = 0;
    //imprime circuito euleriano
    ofstream mycircuitoFile;
    mycircuitoFile.open(m_savePath + "/circuito.txt");

    for (long i = m_circuit.size() - 1; i >= 0; i--)
    {
        
        mycircuitoFile<< m_circuit[i] <<endl;
        if (marcacao[m_circuit[i]]==0)
        {
            //se ainda não passamos por esse vertice, adiciona ele no cilo hamiltoniano
            marcacao[m_circuit[i]]=1;
            percurso[y] = m_circuit[i];
            y++;
            
        }

    }
    //fecha o ciclo
     percurso[m_numero_de_vertices] = percurso[0];

    
}
