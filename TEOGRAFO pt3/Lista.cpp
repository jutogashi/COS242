
#include "Lista.h"
#include "Heap.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdlib.h>
#include <time.h>


using namespace std;


Lista::Lista(string path)
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
    m_pLista = new ListNode*[m_numero_de_vertices + 1]();
    
    // aloca espaço para o vetor de tamanho igual ao numero de vertices +1, para nõ precisarmos
    //nos preocupar com o 0.
    m_numero_de_arestas = 0;
    
    string s;
    //guarda cada linha do arquivo
    while (getline(myFile, s))
    {
        if (s.empty() == false)
        {
            istringstream tmp(s);
            int v1, v2;
            float v3;
            tmp >> v1 >> v2 >>v3;
            
            if (v1<= m_numero_de_vertices && v2 <= m_numero_de_vertices && v3>=0)
            {
                this->addAresta(v1, v2, v3);
                this->addAresta(v2, v1, v3);
            }
            
            
            if (v1<= m_numero_de_vertices && v2 <= m_numero_de_vertices && v3 <0)
            {
                this->addAresta(v1, v2, 1);
                this->addAresta(v2, v1, 1);
            }
        }
    }
    
    
    //criando array de graus e ordenando e obtendo infos
    Grafo::m_grau = new int[m_numero_de_vertices + 1]();
    this->Grau();
    
    Grafo::mergeSort(Grafo::m_grau, 1, m_numero_de_vertices);
    Grafo::Infos();
    
    
    cout << endl << "Leitura ocorreu com sucesso" << endl;
    
    
    
    
    m_vetor_de_marcacao = new int[m_numero_de_vertices + 1](); //inicializa vetor de marcação
    
    cout << "Por favor Aguarde" << endl;
    
    //FComponentes_conexas();  //chama função para determinar informações sobre componentes conexas
    
    
    
    ofstream myInfoFile;
    myInfoFile.open(m_savePath + "/info.txt");
    myInfoFile << "# n = " << m_numero_de_vertices << endl
    << "# m = " << (m_numero_de_arestas) << endl
    << "# g_max = " << m_grau_maximo << endl
    << "# g_min = " << m_grau_minimo << endl
    << "# g_med = " << m_grau_medio << endl
    << "# mediana = " << m_mediana_grau << endl
    << "# numero de componentes conexas: " << m_numero_de_componentes_conexas << endl;
    
    int maior = m_tamanho_da_componente_conexa;
    int k = m_i;
    for (int i = 1; i <= m_numero_de_componentes_conexas; i++)
    {
        for (int i = 1; i <= m_numero_de_componentes_conexas; i++)
        {
            if (m_componentes_conexos[i] > maior)
            {
                maior = m_componentes_conexos[i];
                k = i;
                
            }
            
            
        }
        
        m_componentes_conexos[k] = 0;
        myInfoFile << "#numero de vertices da componete conexa: " << maior
        << endl << "#vetores que fazem parte dessa componente: ";
        for (int i = 1; i <= m_numero_de_vertices; i++)
        {
            if (m_vetor_de_marcacao[i] == k)
            {
                myInfoFile << i << ", ";
            }
        }
    }
    myInfoFile << endl << endl;
    
    m_vetor_de_marcacao = new int[m_numero_de_vertices + 1](); //inicializa vetor de marcação
    
    
    cout << "Analise salva em " << m_savePath << "/info.txt" << endl;
    
    
    
    /* CALCULO DA MEDIA DA EXCENTRICIDADE
     
     
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    srand(time(NULL));
    for( int i =0; i <100; i++)
    {
        Excentricidade(rand()%m_numero_de_vertices);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(t2 - t1).count();
    cout << (duration1/100) ;
    
    //float a = Distancia_media();
    //cout << a;
    */
    

    cout << endl << "Deseja rodar BFS ou DFS ou Dijkstra ou MST?" << endl;
    string escolha;
    cin >> escolha;
    
    if (escolha == "BFS")
    {
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando BFS..." << endl;
        BFS_lista(vertice);
        cout << "Processo concluido." << endl;
    }
    if (escolha == "DFS")
    {
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando DFS..." << endl;
        DFS_lista(vertice);
        cout << "Processo concluido." << endl;
    }
    if (escolha == "Dijkstra")
    {
        print_dijkstra=true;
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando Dijkstra..." << endl;
        Dijkstra(vertice);
        cout << "Processo concluido." << endl;
        print_dijkstra=false;
    }
    
    if (escolha == "MST")
    {
        int vertice;
        cout << "Escolha um vertice:" << endl;
        cin >> vertice;
        cout << "Rodando MST..." << endl;
        MST(vertice);
        cout << "Processo concluido." << endl;
    }
    
 
    
    
}



//constroi uma aresta
void Lista::addAresta(int de, int para, float peso)
{
    ListNode* no = new ListNode;
    no->vertice = para;
    if (m_pLista[de] != NULL)
    {
        m_pLista[de]->pPrev = no;
    }
    no->pNext = m_pLista[de];
    no->pPrev = NULL;
    no->peso = peso;
    this->m_pLista[de] = no;
    
    m_numero_de_arestas++;
}




void Lista::Grau()
{
    
    int grau = 0;
    ListNode* temp;
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        for (ListNode* neigh = m_pLista[i]; neigh != NULL;)
        {
            temp = neigh;
            neigh = neigh->pNext;
            grau++;
            
        }
        
        Grafo::m_grau[i] = grau;
        grau = 0;
    }
    
    return;
}



Lista::~Lista()
{
    ListNode* temp;
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        for (ListNode* neigh = m_pLista[i]; neigh != NULL;)
        {
            temp = neigh;
            neigh = neigh->pNext;
            delete temp;
            
        }
    }
    delete[] m_pLista;
}



// BFS

void Lista::BFS_lista(int s)
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
    myBFSFile.open(m_savePath + "/lista_BFS.txt");
    myBFSFile << "Vertice: " << raiz->vertice << ", Nivel: " << raiz->nivel
    << ", Pai: - " << endl;
    
    m_vetor_de_marcacao[s] = 1;    //marca vetor s
    fila.push(raiz);               //bota s na fila
    
    while (!fila.empty())
    {
        Arvore* v = fila.front();       //le elemento da fila
        fila.pop();                         // tira elemento da fila
        
        m_tamanho_da_componente_conexa++;
        
        
        int w;
        for (ListNode* neigh = m_pLista[v->vertice]; neigh != NULL;)
        {
            w = neigh->vertice;
            neigh = neigh->pNext;
            if (m_vetor_de_marcacao[w] == 0)     //se w não estiver marcado
            {
                m_vetor_de_marcacao[w] = 1;//marca w
                
                Arvore* x = this->Parentesco(v, w);  // cria relacionameneto entre os vertices
                
                //imprime informações sobre a arvore
                myBFSFile << "Vertice: " << x->vertice << ", Nivel: " << x->nivel
                << ", Pai: " << v->vertice << endl;
                
                fila.push(x);                   //coloca w na lista
                
            }
        }
    }
    
    return;
}



//DFS
void Lista::DFS_lista(int s)
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
    myDFSFile.open(m_savePath + "/lista_DFS.txt");
    
    pilha.push(raiz);               //bota s na pilha
    while (!pilha.empty())
    {
        Arvore* u = pilha.top();       //le elemento da fila
        pilha.pop();  // tira elemento da fila
        if (m_vetor_de_marcacao[u->vertice] == 0)
        {
            //imprime informações sobre a arvore
            myDFSFile << "Vertice: " << u->vertice << ", Nivel: " << u->nivel;
            
            if (!(u == raiz)) myDFSFile << ", Pai: " << u->ppai->vertice << endl;
            else myDFSFile << ", Pai: - " << endl;
            
            
            m_vetor_de_marcacao[u->vertice] = 1;    //marca vetor s
            for (ListNode* neigh = m_pLista[u->vertice]; neigh != NULL;)
            {
                int v;
                v = neigh->vertice;
                neigh = neigh->pNext;
                Arvore* x = this->Parentesco(u, v);  // cria relacionameneto entre os vertices
                pilha.push(x);
            }
            
        }
    }
    
    return;
}



Arvore* Lista::Parentesco(Arvore* v, int w)
{
    Arvore* filho = new Arvore;
    filho->vertice = w;
    int nivel_pai = v->nivel;
    filho->nivel = nivel_pai + 1;
    filho->ppai = v;
    
    
    return filho;
}




void Lista::FComponentes_conexas()
{
    m_componentes_conexos = new int[m_numero_de_vertices + 1];
    m_i = 1;
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        if (m_vetor_de_marcacao[i] == 0)
        {
            
            m_numero_de_componentes_conexas++;
            BFS_lista(i);
            
            m_componentes_conexos[i] = m_tamanho_da_componente_conexa;
            m_i++;
            
        }
        
    }
    
    
    m_i = 1;
    return;
}



void Lista::Dijkstra(int s)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    // inicializa heap e vetores auxiliares
    
    Heap Dijkstra_heap;
    m_distancia = new float[m_numero_de_vertices + 1]();
    m_pai = new int[m_numero_de_vertices + 1]();
    m_conjuntoS = new int[m_numero_de_vertices + 1]();
    
    // find eé usado para podermos acessar um nó em O(1) ja que a funcao find implementada no heap é em O(n),
    //o que é importante pois o nóé necessário para fazer a operaçao de alteração de uma chave do heap
    node **find = new node*[m_numero_de_vertices + 1]();
    
    //define infinito
    float infinito = std::numeric_limits<float>::max();
    
    //inicializa vetor distancia como infinito, menos para o vertice s que é 0, e add o vertice s no heap
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        if (i != s)
        {
            m_distancia[i] = infinito;
        }
        else Dijkstra_heap.insert(0, s);
    }
    
    while (Dijkstra_heap.isEmpty() == false)
    {
        m_distancia_parcial = 0;
        node* v = Dijkstra_heap.getMinimum(); //pega valor minimo do heap
        int vertice = v->Vertice;
        m_conjuntoS[vertice] = 1;
        Dijkstra_heap.removeMinimum();  // remove minimo do heap
        
        for (ListNode* w = m_pLista[vertice]; w != NULL;) //percorre os vizinhos desse vertice tirado do heap
        {
            // se o vertice ainda nao foi tirado do heap e a distancia de w > distancia de v + peso de w
            if ((m_distancia[w->vertice] > m_distancia[vertice] + w->peso) && (m_conjuntoS[w->vertice] == 0))
            {
                //se esse vertice ainda nao foi adicionado no heap, adiciona e atualiza vetores auxiliares
                if ((m_distancia[w->vertice] = !infinito))
                {
                    find[w->vertice] = Dijkstra_heap.decreaseKey(find[w->vertice], m_distancia[vertice] + w->peso);
                    m_distancia[w->vertice] = m_distancia[vertice] + w->peso;
                    m_pai[w->vertice] = vertice;
                }
                //se esse vertice ja foi adicionado no heap, atualiza e atualiza vetores auxiliares
                else
                {
                    find[w->vertice]=Dijkstra_heap.insert(m_distancia[vertice] + w->peso, w->vertice);
                    m_distancia[w->vertice] = m_distancia[vertice] + w->peso;
                    m_pai[w->vertice] = vertice;
                }
            }
            w = w->pNext;
        }
    }
    
    //gera arquivo com tempo de duracao de dijkstra
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(t2 - t1).count();
    ofstream myTimeFile;
    myTimeFile.open(m_savePath + "/Dijskstra_time.txt");
    myTimeFile << duration1 <<" microsegundos" <<endl;
    
    
    //utilizado para saber soma das distancias de um vertice para a funcao de distancia media
    if (dist_media)
    {
        for (int i = s; i <= m_numero_de_vertices; i++)
        {
            if (m_distancia[i] != infinito) m_distancia_parcial = m_distancia_parcial + m_distancia[i];
            else m_pares_invalidos = m_pares_invalidos + 1;
        }
    }
    
    
    //imprime arquivo de saida
    ofstream myDijkstraFile;
    myDijkstraFile.open(m_savePath + "/lista_Dijskstra.txt");
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        if (print_dijkstra)
        {
            myDijkstraFile << "Vertice: " << i << ", Distancia: " << m_distancia[i] << ", Caminho: " << i <<", ";
            int x = i;
            while (m_pai[x] != 0)
            {
                myDijkstraFile << m_pai[x] << ", ";
                x = m_pai[x];
                
            }
            
        }
        myDijkstraFile << endl;
    }
}

void Lista::MST(int s)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
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
        
        for (ListNode* w = m_pLista[vertice]; w != NULL;)
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
    
    
    
    //imprime arquivo de saida
    ofstream myMSTFile;
    myMSTFile.open(m_savePath + "/lista_MST.txt");
    float peso_total = 0;
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        peso_total += m_distancia[i];
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(t2 - t1).count();
    cout << duration1<<endl <<peso_total;
    
    myMSTFile << "Peso total: " << peso_total <<endl ;
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        myMSTFile << i << "   " << m_pai[i] << endl;
    }
    
}


//calcula excentricidade do grafon para vertice s
int Lista::Excentricidade(int s)
{
 
    int excentricidade=0;
    Lista::Dijkstra(s);
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        if (excentricidade < m_distancia[i]) excentricidade = m_distancia[i];
    }

    return excentricidade;
}

//calcula distancia media
float Lista::Distancia_media()
{
    dist_media = true;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    m_pares_invalidos = 0;
    float Distancia_media = 0;
 
    for (int i = 1; i <= m_numero_de_vertices; i++)
    {
        Dijkstra(i);
        Distancia_media = Distancia_media + m_distancia_parcial;
    }
    
    //numero de combinacoes validas que podemos ter
    int combinacoes = ((m_numero_de_vertices - 1)* m_numero_de_vertices) / 2 ;
    combinacoes = combinacoes - m_pares_invalidos;
    
    Distancia_media = Distancia_media / combinacoes;
    
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(t2 - t1).count();
    cout << duration1 << endl;
    dist_media = false;
    
    return Distancia_media;
}


