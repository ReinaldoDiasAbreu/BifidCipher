#ifndef BIFID_H
#define BIFID_H

#include <cstdio>

class Bifid{

    private:
        char table[6][6];
        long int periodo = 0;
        long int str_size = 0;

        void Create_Table();
        char Extract(char* c, int t, int p);
        long int TamFile(const char* arq_in_name);
        bool Load_String(char* file_name, char* str, int* l, int*c, long int tam);
        bool Search(char c, int *lin, int *col);
        int* PeriodLine(int *line, int *col, int p);
        void GroupPairs(int *line, int *col, int*crypt);
        void ParseToCode(char *str, int *line, int *col);
        void WriteOut(char *str, char *file_out);
        void PrintTable();


    public:
        Bifid( char* arqin,  char* arqout, bool def);

};

#endif