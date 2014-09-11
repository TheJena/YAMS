#include <fstream>
#include "io_file.h"
#include "data_structure.h"

using namespace std ;

const char* F_TILES = "./tiles_names.dat" ;
const int MAXWORD = 20 ;

void import_tiles_names ()
{
    if ( name != NULL )
        delete[] name ;
    name = new _string [TILES] ;

    ifstream file ( F_TILES ) ;
        //debuggare se apertura fallita

    char temp ;
    while ( file.peek() != '@' )
        file.get ( temp ) ;

    file.get ( temp ) ;
    file.get ( temp ) ;

    char buffer[MAXWORD] ;

    for ( int i = 0, n = 0 ; i < TILES ; i++ )
    {
        file.get( buffer, MAXWORD, '\n' ) ;
        n = file.gcount() ;
        name[i].word = new char[n+1] ;
        for ( int j = 0 ; j < n+1 ; j++ )
            name[i].word[j] = buffer[j] ;
        //debuggare presenza terminatore

        while (file.peek() != '\n')
            file.get ( temp ) ;

        file.get ( temp ) ;
    }
    file.get ( temp ) ;
    file.close () ;
}

void delete_tiles_names ()
{
    for ( int i = 0 ; i < TILES ; i++ )
    {
        delete [] name[i].word ;
        name[i].word = NULL ;
    }

    delete [] name ;
    name = NULL ;
}
