
#include "Lista.h"
#include <iostream>
#include "Grafo.h"
#include "Matriz.h"
#include <string>
using namespace std;


int main()
{
    string arquivo = "";
    char escolha;
    cout << "Por favor, indique o arquivo: " << endl;
    getline(cin, arquivo);
    cout << "Escolha: Matriz de Adjacencia(M)/ Lista de adjacencia (L): " << endl;
    cin >> escolha;
    if (escolha == 'M')
    {
        Matriz Matriz_de_adjacencia = Matriz(arquivo);
    }
    else if (escolha == 'L')
    {
        Lista Lista_de_Adjacencia = Lista(arquivo);
        //TODO notes
        
        
    }
    else
    {
        cout << "Opcao invalida.";
    }
    
    return 0;
}

