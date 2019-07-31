#ifndef BIFID_H
#define BIFID_H

#include <cstdio>

class Bifid{

    private:
        char table[6][6];
        long int periodo = 0;
        long int str_size = 0;
        
        long int TamFile( char* arq_in_name);
        bool Read_file(char *str_dest, char *file, long int tam);

        // #################################### Funçoes de Criptografia ####################################
        void Create_Table();
        char Extract(char* c, int t, int p);
        void Load_String(char* str_in, int* l, int*c);
        bool Search(char c, int *lin, int *col);
        int* PeriodLine(int *line, int *col, int p);
        void GroupPairs(int *line, int *col, int*crypt);
        void ParseToCode(char *str, int *line, int *col);
        void WriteOut(char *str, char *file_out);
        void PrintTable();
        // #################################### Funçoes de Descriptografia ####################################



    public:
        Bifid( char* arqin,  char* arqout, int def, int pin, char* key);

};

#endif