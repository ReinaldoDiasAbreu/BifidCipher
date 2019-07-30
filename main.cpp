#include <iostream>
#include "bifid.h"
using namespace std;

int main(int argc, char *argv[])
{
    /* 
        ./bifid arq_in name_file_out def
        def: 
            true -> default: generate alphanumeric key and period automatically
            false-> request 32-digit alphanumeric key and period on entry.
    */

   switch(argc)
   {
        case 3:{
            Bifid cifra(argv[1], argv[2], true);
        break;
        }

        case 5:{
            cout << "request 32-digit alphanumeric key and period on entry." << endl;
        break;
        }
        case 2:{
            cout << "Help Program" << endl;
        break;
        }
    
        default:{
            cout << "Erro" << endl;
        break;
        }
   }

    return 0;
}