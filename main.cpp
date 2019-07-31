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
        case 4:{
            Bifid cifra(argv[1], argv[2], atoi(argv[3]), 0, NULL);
        break;
        }

        case 6:{
            Bifid cifra(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), argv[5]);
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