
#include "Matriz.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


Matriz::Matriz(string path)
{
    ifstream myFile;
    
  
    
    myFile.open(path);
    cout << endl << "Lendo arquivo " << path << " Por favor aguarde." << endl;
    if (!myFile)
        //Se o arquivo n„o existir
    {
        cout << "Arquivo " << path << " Nao foi encontrado.";
        return;
    }
    
    myFile >> m_numero_de_vertices;        // Salva o numero de vertices totais do grafo
    
    
    
    this->construtor();       //chama a funÁ„o para construir a matriz
    
    
    // aloca espaÁo para o vetor de tamanho igual ao numero de vertices +1, para nı precisarmos
    //nos preocupar com o 0.
    
    m_numero_de_arestas = 0;  //inicializando numero de arestas como zero
    
    
    Grafo::m_grau = new int[m_numero_de_vertices + 1](); //criando array de graus
    
    string s;
    //guarda cada linha do arquivo
    while (getline(myFile, s))
    {
        if (s.empty() == false)        {
            istringstream tmp(s);
            int v1, v2;
            tmp >> v1 >> v2;
            this->addAresta(v1, v2);
            this->addAresta(v2, v1);
        }
    }
    
    
    cout << endl << "Leitura ocorreu com sucesso" << endl;
    
    m_vetor_de_marcacao = new int[m_numero_de_vertices + 1](); //inicializa vetor de marcaÁ„o
    
    // ordenando array de graus e obtendo infos
    Grafo::mergeSort(Grafo::m_grau, 1, m_numero_de_vertices);
    Grafo::Infos();
    
    
    ofstream myInfoFile;
    myInfoFile.open(m_savePath + "/info.txt");
    myInfoFile << "# n = " << m_numero_de_vertices << endl
    << "# m = " << (m_numero_de_arestas) << endl
    << "# g_max = " << m_grau_maximo << endl
    << "# g_min = " << m_grau_minimo << endl
    << "# g_med = " << m_grau_medio << endl
    << "# mediana = " << m_mediana_grau << endl;
    
    //TODO incluir informaÁıes sobre componentes conexas
    
    
    
    cout << "Analise salva em " << m_savePath << "/info.txt" << endl;
    
    /*float tempo =0;
    for (int i = 1; i <= 100; i++)
    {
        start = std::chrono::system_clock::now();
        DFS_matriz(1);
        end = std::chrono::system_clock::now();
        float elapse_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        tempo = tempo + elapse_seconds;
        m_vetor_de_marcacao = new int[m_numero_de_vertices + 1]();
    }
    tempo = tempo / 100;
    cout << "Tempo de Execucao: " << tempo << " Milissegundos" << endl;
    
    
    m_vetor_de_marcacao = new int[m_numero_de_vertices + 1](); //inicializa vetor de marcaÁ„o*/
    
    cout << endl << "Deseja rodar BFS ou DFS?" << endl;
    string escolha;
    cin >> escolha;
    if (escolha == "BFS")
    {
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando BFS..." << endl;
        BFS_Matriz(vertice);
        cout << "Processo concluido." << endl;
    }
    if (escolha == "DFS")
    {
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando DFS..." << endl;
        DFS_matriz(vertice);
        cout << "Processo concluido." << endl;
    }
    
    
}


void Matriz::addAresta(int linha, int coluna)
{
    
    m_matriz[linha][coluna] = true;
    m_numero_de_arestas++;
    Grafo::m_grau[linha]++;
    return;
}



void Matriz::construtor()
{
    
    m_matriz = new bool*[m_numero_de_vertices + 1];
    
    for (int i = 0; i < m_numero_de_vertices + 1; ++i)
        
        m_matriz[i] = new bool[m_numero_de_vertices];
    
    //iniciando ela com zero
    for (int i = 0; i <= m_numero_de_vertices; ++i)
        for (int j = 0; j <= m_numero_de_vertices; ++j)
            m_matriz[i][j] = false;
    
    return;
}




Matriz::~Matriz()
{
    for (int i = 0; i <= m_numero_de_vertices; ++i)
        delete[]m_matriz[i];
    
    delete[]m_matriz;
}




// BFS

void Matriz::BFS_Matriz(int s)
{
    
    while (!fila.empty())   //cofere que a fila estara vazia
    {
        fila.pop();
    }
    
    
    Arvore* raiz = new Arvore;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->ppai = NULL;
    
    ofstream myBFSFile;
    myBFSFile.open(m_savePath + "/matriz_BFS.txt");
    myBFSFile << "Vertice: " << raiz->vertice << ", Nivel: " << raiz->nivel
    << ", Pai: - " << endl;
    
    m_vetor_de_marcacao[s] = 1;    //marca vetor s
    fila.push(raiz);               //bota s na fila
    
    while (!fila.empty())
    {
        Arvore* v = fila.front();       //le elemento da fila
        fila.pop();                         // tira elemento da fila
        
        
        for (int w = 1; w <= m_numero_de_vertices; w++)
        {
            if (m_matriz[v->vertice][w] == true)
                if (m_vetor_de_marcacao[w] == 0)     //se w n„o estiver marcado
                {
                    m_vetor_de_marcacao[w] = 1;//marca w
                    
                    Arvore* x = this->Parentesco(v, w);  // cria relacionameneto entre os vertices
                    
                    //imprime informaÁıes sobre a arvore
                    myBFSFile << "Vertice: " << x->vertice << ", Nivel: " << x->nivel
                    << ", Pai: " << v->vertice << endl;
                    
                    fila.push(x);                   //coloca w na lista
                }
        }
    }
}




//DFS
void Matriz::DFS_matriz(int s)
{
    
    while (!pilha.empty())   //cofere que a pilha estara vazia
    {
        pilha.pop();
    }
    Arvore* raiz = new Arvore;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->ppai = NULL;
    
    ofstream myDFSFile;
    myDFSFile.open(m_savePath + "/matriz_DFS.txt");
    
    pilha.push(raiz);               //bota s na pilha
    while (!pilha.empty())
    {
        Arvore* u = pilha.top();       //le elemento da fila
        pilha.pop();  // tira elemento da fila
        if (m_vetor_de_marcacao[u->vertice] == 0)
        {
            //imprime informaÁıes sobre a arvore
            myDFSFile << "Vertice: " << u->vertice << ", Nivel: " << u->nivel;
            if (!(u == raiz)) myDFSFile << ", Pai: " << u->ppai->vertice << endl;
            else myDFSFile << ", Pai: - " << endl;
            
            
            m_vetor_de_marcacao[u->vertice] = 1;    //marca vetor s
            for (int w = 1; w <= m_numero_de_vertices; w++)
            {
                if (m_matriz[u->vertice][w] == true)
                {
                    Arvore* x = this->Parentesco(u, w);  // cria relacionameneto entre os vertices
                    pilha.push(x);
                }
            }
            
        }
    }
    
    return;
}




Arvore* Matriz::Parentesco(Arvore* v, int w)
{
    Arvore* filho = new Arvore;
    filho->vertice = w;
    int nivel_pai = v->nivel;
    filho->nivel = nivel_pai + 1;
    filho->ppai = v;
    
    
    return filho;
}


