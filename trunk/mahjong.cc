#include <iostream>

using namespace std ;

struct tile {
            bool    empty ;
            bool    lock ;
            char *  name ;
            }

const int dim_X = 12 ;
const int dim_Y = 8 ;
const int dim_Z = 4 ;
/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube ( tile *** cube )
{
    cube = new tile ** [dim_X] ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        cube[x] = new tile * [dim_Y] ;

        for ( int y = 0 ; y < dim_Y ; y++ )
            cube[x][y] = new tile [dim_Z] ;
    }
}

void initialize_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                cube[x][y][z].empty = true ;
                cube[x][y][z].lock  = true ;
                cube[x][y][z].name  = NULL ;
            }
}

void delete_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_x ; x++ )
    {
        for ( int y = 0 ; y < dim_Y ; y++ )
        {
            for ( int z = 0 ; z < dim_Z ; z++ )
            {
                if ( cube[x][y][z].name != NULL )
                    delete [] cube[x][y][z].name ;

                delete [] cube[x][y] ;
            }
        }

        delete [] cube[x] ;
    }

    delete [] cube ;
}
int main ()
{

    return 0 ;
}
