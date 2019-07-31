#ifndef BIFID_H
#define BIFID_H

#include <cstdio>

class Bifid{

    private:
        char CARACTERES[37] = "abcdefghijklmnopqrstuvwxyz0123456789"; 
        int QUANT = 36;
        char table[6][6];
        long int periodo = 0;
        long int str_size = 0;
        
        // ######################################## Funçoes Comuns ########################################

        long int TamFile( char* arq_in_name);
        bool Read_file(char *str_dest, char *file, long int tam);
        void WriteOut(char *str, char *file_out);
        bool Search(char c, int *lin, int *col);
        void ParseToCode(char *str, int *line, int *col);
        void ParseToCord(char *str, int *line, int *col, long int tam);
        void PrintTable();
        bool CaracterPermitido(char c);

        // #################################### Funçoes de Criptografia ####################################

        void Create_Table();
        char Extract(char *str, int p);
        void Load_String(char* str_in, int* l, int*c);
        int* PeriodLine(int *line, int *col, int p);
        void GroupPairs(int *line, int *col, int*crypt);
        
        // ################################### Funçoes de Descriptografia ###################################

        void FillTable(char *key);
        int* UndoPairs(int *line, int *col);
        void UndoLinePeriod(int *line_crypt, int *line, int *col, int p);


    public:
        Bifid( char* arqin,  char* arqout, int def, int pin, char* key);

};

#endif