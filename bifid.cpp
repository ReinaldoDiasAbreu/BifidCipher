#include "bifid.h"
#include <iostream>
#include <ctime>
#include <cstring> 
#include <cctype>
using namespace std;

Bifid::Bifid( char* arqin, char* arqout, bool def)
{
    if(def)
    {
        Create_Table();
        long int tam = TamFile(arqin);
        srand(time(NULL));
        periodo = 2 + (rand() % 4);

        char *str = new char[tam];
        int *line = new int[tam];
        int *col = new int[tam];

        if(Load_String(arqin, str, line, col, tam))
        {
            int *Cryptline = PeriodLine(line, col, periodo);
            GroupPairs(line, col, Cryptline);
            ParseToCode(str, line, col);
            WriteOut(str, arqout);
            delete[] Cryptline;

            cout << periodo << " ";
            PrintTable();
            cout << " " << arqout << endl;
        }
        else
        {
            cout << "File not fonud!" << endl;
        }

        delete[] str;
        delete[] line;
        delete[] col;

    }
    else
    {

    }
}

void Bifid::Create_Table()  // Cria tablela randominca de codificacao
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

char Bifid::Extract(char* c, int t, int p) // Retira e retorna um caracter selecionado no vetor
{
    char aux = c[p];
    for(int i=p; i < t; i++)
        c[i] = c[i+1];
    return aux;
}

long int Bifid::TamFile(const char* arq_in_name)  // Calcula a quantidade de caracteres no arquivo
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

bool Bifid::Load_String(char* file_name, char* str, int* l, int*c, long int tam) // Carrega o vetor com caracteres permitidos
{
    FILE *arq_in = fopen(file_name, "r");
    char aux;
    int linha, coluna;
    str_size = 0;
    if(arq_in != NULL)
    {
        for(long int i=0; i < tam; i++)
        {
            fscanf(arq_in, "%c", &aux);
            aux = tolower(aux);
            if(Search(aux, &linha, &coluna))
            {
                str[str_size] = aux;
                l[str_size] = linha;
                c[str_size] = coluna;
                str_size++;
            }
        }
        fclose(arq_in);
        return true;
    }
    else
    {
        fclose(arq_in);
        return false;
    }
}

bool Bifid::Search(char c, int *lin, int *col)  // Busca caracter na table e repassa linha e coluna se existir
{
    for(int i=0; i < 6; i++)
        for(int j=0; j < 6; j++)
            if(c == table[i][j])
            {
                *lin = i;
                *col = j;
                return true;
            }
    return false;
}

int* Bifid::PeriodLine(int *line, int *col, int p) // Agrupa linhas e colunas com período passado em um vetor
{
    int* PeriodLineCrypt = new int[str_size*2];
    long int pcol, pline, pcrypt;
    pcol = pline = pcrypt= 0;

    for(long int i=0; i<str_size*2; i+=p*2)
    {
        for(long int l=0; l < p; l++)
        {
            if(pline < str_size)
            {
                PeriodLineCrypt[pcrypt] = line[pline++];
                pcrypt++;
            }
        }
        for(long int c=0; c < p; c++)
        {
            if(pcol < str_size)
            {
            PeriodLineCrypt[pcrypt] = col[pcol++];
            pcrypt++;
            }
        }
    }
    return PeriodLineCrypt;
}

void Bifid::GroupPairs(int *line, int *col, int*crypt) // Separa grupo de linha e coluna
{
    long int cline, ccol, ccrypt;
    cline = ccol = ccrypt = 0;
    for(long int i=0; i<str_size*2; i+=2)
    {
        line[cline++] = crypt[ccrypt++];
        col[ccol++] = crypt[ccrypt++];
    }
}

void Bifid::ParseToCode(char *str, int *line, int *col) // Passa linha e coluna para codificacao da tabela
{
    for(int i=0; i < str_size; i++)
    {
        str[i] = table[line[i]][col[i]];
    }
}

void Bifid::WriteOut(char *str, char *file_out)  // Escreve a string no arq de saida tentando nao sobreescrever se existente
{
    char nome[100];
    strcpy(nome, file_out);
    for(int i=0; i < 10000; i++)
    {
        FILE *tfile = fopen(nome, "r");
        if( tfile != NULL)
        {
            char aux[10];
            sprintf(aux, "%i", i);
            strcpy(nome, file_out);
            strcat(nome, aux);
            fclose(tfile);
        }
        else
        {
            strcpy(file_out, nome);
            fclose(tfile);
            break;
        }
    }

    FILE *fout = fopen(file_out, "w");
    for(long int i=0; i < str_size; i++)
        fprintf(fout, "%c", str[i]);
    fclose(fout);
}

void Bifid::PrintTable() // Imprime tabela de codificacao
{
    for(int i=0; i < 6; i++)
    {
        for(int j=0; j < 6; j++)
        {
            cout << table[i][j];
        }
    }
}
