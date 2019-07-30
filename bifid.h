#ifndef BIFID_H
#define BIFID_H

#include <cstdio>

class Bifid{

    private:
        char table[6][6];
        long int periodo = 0;

        void Create_Table();
        char Extract(char* c, int t, int p);
        long int TamFile(const char* arq_in_name);
        void PrintTable();


    public:
        Bifid( char* arqin,  char* arqout, bool def);

};

#endif