#include "bifid.h"
#include <iostream>
#include <ctime>
#include <cstring> 

using namespace std;

Bifid::Bifid( char* arqin, char* arqout, int def, int pin, char* key)
{
    if(def == 1) // Encryption
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
    else // Decryption
    {
        if( (pin >= 2 && pin <= 6) && (strlen(key) == QUANT) )
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

// #################################### Common Functions ####################################

bool Bifid::Read_file(char *str_dest, char *file, long int tam)  /// Read file and copy for a vector
{
    FILE *arq = fopen(file, "r");
    char aux;
    if(arq != NULL)
    {   
        for(long int i= 0; i < tam; i++)
        {
            fscanf(arq, "%c", &aux);
            if(aux == ' ') aux = '_';
            else aux = tolower(aux);
            
            if(CaracterPermitido(aux) == true)
                str_dest[str_size++] = aux;
            
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

void Bifid::WriteOut(char *str, char *file_out)  /// Write string to output file trying not to overwrite existing file
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
    {
        if(str[i] == '_') str[i] = ' ';
        fprintf(fout, "%c", str[i]);
    }
    fprintf(fout, "\n");
    fclose(fout);
}

long int Bifid::TamFile(char* arq_in_name)  /// Calculate the amount of characters in the file
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

bool Bifid::Search(char c, int *lin, int *col)  /// Search character in table and pass row and column if any
{
    for(int i=0; i < TAM_TABLE; i++)
        for(int j=0; j < TAM_TABLE; j++)
            if(c == table[i][j])
            {
                *lin = i;
                *col = j;
                return true;
            }
    return false;
}

void Bifid::ParseToCode(char *str, int *line, int *col) /// Pass row and column through table encoding to save to str
{
    for(int i=0; i < str_size; i++)
    {
        str[i] = table[line[i]][col[i]];
    }
}

void Bifid::ParseToCord(char *str, int *line, int *col, long int tam)  /// Pass characters to row and column code
{
    for(long int i=0; i < tam; i++)
        Search(str[i], &line[i], &col[i]);
}

void Bifid::PrintTable() /// Print inline coding table
{
  
    for(int i=0; i < TAM_TABLE; i++)
    {
        for(int j=0; j < TAM_TABLE; j++)
        {
            cout << table[i][j];
        }
    }
    cout << endl;
}

bool Bifid::CaracterPermitido(char c) /// Checks if the character is allowed
{
    for(int i=0; i < QUANT; i++)
        if(c == CARACTERES[i])
            return true;
    return false;
}


// #################################### Encryption Functions ####################################

void Bifid::Create_Table()  /// Create random coding table
{
    srand(time(NULL));
    int val = QUANT;
    char *str = new char[QUANT+1];
    strcpy(str, CARACTERES);
    for(int i=0; i < TAM_TABLE; i++)
    {
        for(int j=0; j < TAM_TABLE; j++)
        {
            int p = rand() % val;
            table[i][j] = Extract(str, p);
            val--;
        }
    }
}

char Bifid::Extract(char *str, int p) /// Removes and returns a selected character in the vector.
{
    char aux = str[p];
    for(int i=p; i < QUANT; i++)
        str[i] = str[i+1];
    return aux;
}

void Bifid::Load_String(char* str_in, int* l, int*c) /// Load vector with allowed characters and rows and columns
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

int* Bifid::PeriodLine(int *line, int *col, int p) /// Returns vector with row and column coordinate grouping
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

void Bifid::GroupPairs(int *line, int *col, int*crypt) /// Separate row and column group into matching pairs
{
    long int cline, ccol, ccrypt;
    cline = ccol = ccrypt = 0;
    for(long int i=0; i<str_size*2; i+=2)
    {
        line[cline++] = crypt[ccrypt++];
        col[ccol++] = crypt[ccrypt++];
    }
}

// #################################### Decryption Functions ####################################


void Bifid::FillTable(char *key)  /// Load key into coding table
{
    int cont = 0;
    for(int i=0; i<TAM_TABLE; i++)
        for(int j=0; j<TAM_TABLE; j++)
            table[i][j] = key[cont++];
}

int* Bifid::UndoPairs(int *line, int *col) /// Undo row and column pairs and return a vector in sequence
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

void Bifid::UndoLinePeriod(int *line_crypt, int *line, int *col, int p) /// Remove period shuffle and return original row and columns
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