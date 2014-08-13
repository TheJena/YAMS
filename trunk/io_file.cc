#include <fstream>
#include "data_structure.h"
using namespace std ;

const int MAXWORD = 20 ;

/*deallocare correttamente a chiusura applicazione
  array 2d con nomi tessere*/

void import_tiles_names (string * name)
{
    if ( name != NULL )
        delete[] name ;
    name = new string [TILES] ;

    ifstream file ( F_TILES ) ;
        //debuggare se apertura fallita

    char temp ;
    while (file.peek != '@')
        temp = file.get() ;

    temp = file.get() ;
    temp = file.get() ;

    char buffer[MAXWORD] ;

    for ( int i = 0, n = 0 ; i < TILES ; i++ )
    {
        file.get( buffer, MAXWORD, '\n' ) ;
        n = file.gcount() ;
        name[i].word = new char[n+1] ;
        for ( int j = 0 ; j < n+1 ; j++ )
            name[i].word[j] = buffer[j] ;
        //debuggare presenza terminatore

        while (file.peek != '\n')
            temp = file.get() ;

        temp = file.get() ;
    }
    temp = file.get() ;
    file.close () ;
}


