#include <fstream>
#include "io_file.h"
#include "data_structure.h"
#include "debug_macros.h"

using namespace std ;

const char* F_TILES = "./tiles_names.dat" ;
const int MAXWORD = 20 ;

void import_tiles_names ()
{
    D1(cerr<<"D1 import tiles names\n")

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

    D10(cerr<<"D10 import tiles names\n")
}

void delete_tiles_names ()
{
    D1(cerr<<"D1 delete tiles names\n")

    for ( int i = 0 ; i < TILES ; i++ )
    {
        delete [] name[i].word ;
        name[i].word = NULL ;
    }

    delete [] name ;
    name = NULL ;

    D10(cerr<<"D10 delete tiles names\n")
}
