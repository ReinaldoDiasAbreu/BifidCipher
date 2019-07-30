#include "bifid.h"
#include <iostream>
#include <ctime>
#include <cstdio>
using namespace std;

Bifid::Bifid( char* arqin, char* arqout, bool def)
{
    if(def)
    {
        Create_Table();
        PrintTable();
        long int tam = TamFile(arqin);
        srand(time(NULL));
        periodo = 2 + (rand() % 4);

        cout << tam << " " <<  periodo << endl;
    }
    else
    {

    }
}

void Bifid::Create_Table()
{
    char caracteres[36] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9'};
    srand(time(NULL));
    int val = 36;
    for(int i=0; i < 6; i++)
    {
        for(int j=0; j < 6; j++)
        {
            int p = rand() % val;
            table[i][j] = Extract(caracteres, val, p);
            val--;
        }
    }
}

char Bifid::Extract(char* c, int t, int p)
{
    char aux = c[p];
    for(int i=p; i < t; i++)
        c[i] = c[i+1];
    return aux;
}

long int Bifid::TamFile(const char* arq_in_name)
{
    FILE * arq = fopen(arq_in_name, "r");
    long int tam;
    if(arq != NULL)
    {
        fseek(arq, 0, SEEK_END);
        tam = ftell(arq);
    }
    else
    {
        tam = 0;
    }
    fclose(arq);
    return tam;
}




void Bifid::PrintTable()
{
    for(int i=0; i < 6; i++)
    {
        for(int j=0; j < 6; j++)
        {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}
