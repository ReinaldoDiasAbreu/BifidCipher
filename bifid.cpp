#include "bifid.h"
#include <iostream>
#include <ctime>
#include <cstring> 
#include <cctype>

using namespace std;

Bifid::Bifid( char* arqin, char* arqout, int def, int pin, char* key)
{
    if(def == 1) // Criptografia
    {
        Create_Table();
        long int tam = TamFile(arqin); 
        srand(time(NULL));
        periodo = 2 + (rand() % 4);
        char *str = new char[tam];

        if(Read_file(str, arqin, tam))
        {
            int *line = new int[str_size];
            int *col = new int[str_size];
            
            Load_String(str, line, col);
            
            int *Cryptline = PeriodLine(line, col, periodo);
            GroupPairs(line, col, Cryptline);
            ParseToCode(str, line, col);
            WriteOut(str, arqout);
            delete[] Cryptline;

            cout << arqout << " << " << periodo << " ";
            PrintTable();

            delete[] line;
            delete[] col;
        }
        else
        {
            cout << "Input file not found!" << endl;
        }
        delete[] str;
    }
    else // Descriptografia
    {
        if( (pin >= 2 && pin <=6) && (strlen(key) == QUANT) )
        { 
            long int tam = TamFile(arqin); 
            char *str = new char[tam];
        
            if(Read_file(str, arqin, tam))
            {
                int *line = new int[str_size];
                int *col = new int[str_size];
                FillTable(key);
                ParseToCord(str, line, col, str_size);
                int *cryptline = UndoPairs(line, col);
                UndoLinePeriod(cryptline, line, col, pin);
                ParseToCode(str, line, col);
                WriteOut(str, arqout);
                cout << arqout << " << " << pin << " " << key << endl;
                delete[] line;
                delete[] col;
            }
            else
            {
                cout << "Input file not found!" << endl;
            }
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
    char aux;
    if(arq != NULL)
    {   
        for(long int i= 0; i < tam; i++)
        {
            fscanf(arq, "%c", &aux);
            aux = tolower(aux);
            if(CaracterPermitido(aux) == true)
            {
                str_dest[str_size++] = aux;
            }
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

void Bifid::ParseToCode(char *str, int *line, int *col) // Passa linha e coluna para codificacao da tabela e salva em str
{
    for(int i=0; i < str_size; i++)
    {
        str[i] = table[line[i]][col[i]];
    }
}

void Bifid::ParseToCord(char *str, int *line, int *col, long int tam)  // Passa caracteres para codigo em linha e coluna
{
    for(long int i=0; i < tam; i++)
        Search(str[i], &line[i], &col[i]);
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
    cout << endl;
}

bool Bifid::CaracterPermitido(char c)
{
    for(int i=0; i < QUANT; i++)
        if(c == CARACTERES[i])
            return true;
    return false;
}


// #################################### Funçoes de Criptografia ####################################

void Bifid::Create_Table()  // Cria tabela randominca de codificacao
{
    srand(time(NULL));
    int val = QUANT;
    char *str = new char[QUANT+1];
    strcpy(str, CARACTERES);
    for(int i=0; i < 6; i++)
    {
        for(int j=0; j < 6; j++)
        {
            int p = rand() % val;
            table[i][j] = Extract(str, p);
            val--;
        }
    }
}

char Bifid::Extract(char *str, int p) // Retira e retorna um caracter selecionado no vetor
{
    char aux = str[p];
    for(int i=p; i < QUANT; i++)
        str[i] = str[i+1];
    return aux;
}

void Bifid::Load_String(char* str_in, int* l, int*c) // Carrega o vetor com caracteres permitidos e linhas e colunas
{
    int linha, coluna;
    for(long int i=0; i < str_size; i++)
    {
        if(Search(str_in[i], &linha, &coluna))
        {
            l[i] = linha;
            c[i] = coluna;
        }
    }
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

// #################################### Funçoes de Descriptografia ####################################


void Bifid::FillTable(char *key)  // Carrega a chave na tabela de codificacao
{
    int cont = 0;
    for(int i=0; i<6; i++)
        for(int j=0; j<6; j++)
            table[i][j] = key[cont++];
}

int* Bifid::UndoPairs(int *line, int *col) // Defaz pares de linha e colina e retorna um vetor na sequencia
{
   int *crypt = new int[str_size*2];
   long int c = 0;
   for(long int i=0; i < str_size; i++)
   {
       crypt[c++] = line[i];
       crypt[c++] = col[i];
   }
   return crypt;
}

void Bifid::UndoLinePeriod(int *line_crypt, int *line, int *col, int p) // Remove o embaralhamento por periodo e retorna a linha e colunas originais
{
    long int pcol, pline, pcrypt, cont;
    pcol = pline = pcrypt = 0;
    cont = str_size*2;

    while(cont > 0)
    {
        if(!(cont >= p*2))
            p = cont/2;
        
        for(long int l=0; l < p; l++)
            line[pline++] = line_crypt[pcrypt++];
        for(long int c=0; c < p; c++)
            col[pcol++] = line_crypt[pcrypt++];
        
        cont -= (p*2);
    }

}