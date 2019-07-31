#include "bifid.h"
#include <iostream>
#include <ctime>
#include <cstring> 
#include <cctype>
using namespace std;

Bifid::Bifid( char* arqin, char* arqout, int def, int pin, char* key)
{
    if(def == 1)
    {
        Create_Table();
        long int tam = TamFile(arqin); 
        srand(time(NULL));
        periodo = 2 + (rand() % 4);
        char *str = new char[tam];
        
        if(Read_file(str, arqin, tam))
        {
            int *line = new int[tam];
            int *col = new int[tam];
            
            Load_String(str, line, col);
            
            int *Cryptline = PeriodLine(line, col, periodo);
            GroupPairs(line, col, Cryptline);
            ParseToCode(str, line, col);
            WriteOut(str, arqout);
            delete[] Cryptline;

            cout << periodo << " ";
            PrintTable();
            cout << " " << arqout << endl;

            delete[] line;
            delete[] col;
        }
        else
        {
            cout << "Input file not found!" << endl;
        }
        delete[] str;
    }
    else
    {
        if( (pin >= 2 && pin <=6) && (strlen(key) == 36) )
        {
          
        }
        else
        {
            cout << "Invalid Parameters!" << endl;
        }
        
    }
}

bool Bifid::Read_file(char *str_dest, char *file, long int tam)  // Ler o arquivo e copia para um vetor char
{
    FILE *arq = fopen(file, "r");
    
    if(arq != NULL)
    {
        for(long int i = 0; i < tam; i++)
        {
            fscanf(arq, "%c", &str_dest[i]);
        }
        fclose(arq);
        return true;
    }
    else
    {
        fclose(arq);
        return false;
    }
}

long int Bifid::TamFile(char* arq_in_name)  // Calcula a quantidade de caracteres no arquivo
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

// #################################### Funçoes de Criptografia ####################################

void Bifid::Create_Table()  // Cria tabela randominca de codificacao
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

void Bifid::Load_String(char* str_in, int* l, int*c) // Carrega o vetor com caracteres permitidos e linhas e colunas
{
    int linha, coluna;
    str_size = 0;

    for(long int i=0; i < strlen(str_in); i++)
    {
        char aux = tolower(str_in[i]);
        if(Search(aux, &linha, &coluna))
        {
            l[str_size] = linha;
            c[str_size] = coluna;
            str_size++;
        }
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

int* Bifid::PeriodLine(int *line, int *col, int p) // Retorna vetor com o agrupamento de coordendas de linha e coluna
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

void Bifid::GroupPairs(int *line, int *col, int*crypt) // Separa grupo de linha e coluna em pares correspondentes
{
    long int cline, ccol, ccrypt;
    cline = ccol = ccrypt = 0;
    for(long int i=0; i<str_size*2; i+=2)
    {
        line[cline++] = crypt[ccrypt++];
        col[ccol++] = crypt[ccrypt++];
    }
}

void Bifid::ParseToCode(char *str, int *line, int *col) // Passa linha e coluna para codificacao da tabela e salva em str
{
    for(int i=0; i < str_size; i++)
    {
        str[i] = table[line[i]][col[i]];
    }
}

void Bifid::WriteOut(char *str, char *file_out)  // Escreve a string no arq de saida tentando nao sobreescrever arquivo existente
{
    char nome[100];
    strcpy(nome, file_out);
    for(int i=0; i < 10000; i++) // Tenta criar um arquivo com um nome diferente caso exista
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
    fprintf(fout, "\n");
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


// #################################### Funçoes de Descriptografia ####################################


