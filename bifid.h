#ifndef BIFID_H
#define BIFID_H

class Bifid{

    private:
        char CARACTERES[50] = "abcdefghijklmnopqrstuvwxyz0123456789.,:_()=!?/-+*"; // Accepted Characters
        int QUANT = 49;
        int TAM_TABLE = 7;
        char table[7][7];
        long int periodo = 0;
        long int str_size = 0;
        
        // #################################### Common Functions ####################################

        long int TamFile( char* arq_in_name);
        bool Read_file(char *str_dest, char *file, long int tam);
        void WriteOut(char *str, char *file_out);
        bool Search(char c, int *lin, int *col);
        void ParseToCode(char *str, int *line, int *col);
        void ParseToCord(char *str, int *line, int *col, long int tam);
        void PrintTable();
        bool CaracterPermitido(char c);

       // #################################### Encryption Functions ####################################

        void Create_Table();
        char Extract(char *str, int p);
        void Load_String(char* str_in, int* l, int*c);
        int* PeriodLine(int *line, int *col, int p);
        void GroupPairs(int *line, int *col, int*crypt);
        
        // #################################### Decryption Functions ####################################

        void FillTable(char *key);
        int* UndoPairs(int *line, int *col);
        void UndoLinePeriod(int *line_crypt, int *line, int *col, int p);


    public:
        Bifid( char* arqin,  char* arqout, int def, int pin, char* key);

};

#endif