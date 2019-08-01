#include <iostream>
#include "bifid.h"
using namespace std;

int main(int argc, char *argv[])
{
    int period = 0;
    char key[50] = "";
    switch(argc)
    {
        case 4:{
            if(atoi(argv[3]) == 1)
                Bifid cifra(argv[1], argv[2], atoi(argv[3]), 0, NULL);
            else 
            {
                cout << "Period: ";
                cin >> period;
                cout << "Key[49]: ";
                cin >> key;
                Bifid cifra(argv[1], argv[2], atoi(argv[3]), period, key);
            }
        break;
        }

        case 2:{
            cout << "           Help Bifid Cipher 0.1" << endl;
            cout << "   Comando: " << endl << endl;
            cout << "       ./bifid arq_in arq_out op" << endl << endl;
            cout << "   arq_in  -> Input File with extension if any" << endl;
            cout << "   arq_out -> Name Output File" << endl;
            cout << "   op      -> Operation Number (1 - Encrypt / 0 - Decrypt)" << endl << endl;
            cout << "   In the option Decrypt is necessary: " << endl << endl;
            cout << "   period  -> Period Number" << endl;
            cout << "   key     -> 36-character alphanumeric key" << endl << endl;

            cout << "   More: https://github.com/ReinaldoDiasAbreu/BifidCipher" << endl;
            cout << "   By Reinaldo Junio Dias de Abreu - 07/2019" << endl;

        break;
        }
    
        default:{
            cout << "Error: Invalid Syntax" << endl;
            cout << "Try ./bifid help" << endl;
        break;
        }
    }

    return 0;
}