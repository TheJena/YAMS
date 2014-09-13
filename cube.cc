/*inizio implementazione modulo_CUBE*/
#include <iostream>
#include <cstdlib>
#include <ctime>

#define struct_couple           // perche' couple serve a movements.h che pero'
#include "data_structures.h"    // non la puo' contenere a causa di conflitti in
#undef struct_couple            // altri file in cui e' incluso ma couple e' gia
                                // dichiarata da altri header file

#include "debug_macros.h"
#include "cube.h"                       //include gia la definizione struct_tile
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "game.h"      // include gia' le definizioni struct__string e enum_game
#include "movements.h"                //include gia'le definizioni enum_computer

using namespace std ;

/*inizio implementazione struttura dati pubblica*/
const int TILES = 144 ;
const int dim_X = 12 ;
const int dim_Y = 8 ;
const int dim_Z = 4 ;

tile *** cube = NULL ;
/*fine implementazione struttura dati pubblica*/

/*inizio prototipi funzioni private*/
static void initialize_neighbor ( tile * &left_a, tile * &right_a, tile * &under_a,
                           tile * &left_b, tile * &right_b, tile * &under_b,
                           const int &xa, const int &ya, const int &za,
                           const int &xb, const int &yb, const int &zb,
                           tile * &a, tile * &b ) ;

static void check_cell ( const int &x, const int &y, const int &z ) ;

static bool left_cell ( const int &x, const int &y, const int &z ) ;

static bool right_cell ( const int &x, const int &y, const int &z ) ;

static void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i ) ;

static void fill_easy_layout ( int &last ) ;

static void fill_medium_layout ( int &last ) ;

static void fill_difficult_layout ( int &last ) ;

static void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last ) ;
/*fine  prototipi funzioni private*/

/*inizio implementazione funzioni pubbliche*/
void create_cube ()
{
    D1(cerr<<"D1 create cube\n")
    cube = new tile ** [dim_X] ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        cube[x] = new tile * [dim_Y] ;

        for ( int y = 0 ; y < dim_Y ; y++ )
            cube[x][y] = new tile [dim_Z] ;
    }

    assert (cube != NULL) ;

    D10(cerr<<"D10 create cube\n")
}

void delete_cube ()
{
    D1(cerr<<"D1 delete cube\n")

    assert (cube != NULL) ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        for ( int y = 0 ; y < dim_Y ; y++ )
            delete [] cube[x][y] ;

        delete [] cube[x] ;
    }
    delete [] cube ;
    cube = NULL ;

    D10(cerr<<"D10 delete cube\n")
}

bool mix_cube ()
{
    D1(cerr<<"D1 mix cube\n")

    if ( !remove_dummies() )
    {
        D3(cerr<<"mix cube"<<endl)
        D8(cerr<<"remove dummies returned false"<<endl)
        return false ;
    }

    srand ( time(0) ) ;

    int n_tile = 0 ;

    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                if ( !cube[x][y][z].empty )
                    n_tile++ ;
            }

    if ( n_tile > 0 )
    {
        tile ** aus = new tile * [n_tile] ;

        int i = 0 ;
        for ( int x = 0 ; x < dim_X ; x++)
            for ( int y = 0 ; y < dim_Y ; y++)
                for ( int z = 0 ; z < dim_Z ; z++)
                {
                    if ( !cube[x][y][z].empty )
                    {
                        aus[i] = &(cube[x][y][z]) ;
                        i++ ;
                    }
                }
/* debug se i > n_tile */

        const int min = n_tile * 4 ;
        const int max = n_tile * 7 ;
        const int swaps = min + rand()%max ;

        int x1, x2, y1, y2, z1, z2 ;
        int index_a, index_b ;
        for ( int j = 0 ; j < swaps ; j++ )
        {
            index_a = rand()%n_tile ;
            do
            {
                index_b = rand()%n_tile ;
            }
            while ( index_a == index_b ) ;

            assert(aus[index_a] != NULL) ;
            assert(aus[index_b] != NULL) ;

            find_coord ( aus[index_a]->num, x1, y1, z1 ) ;
            find_coord ( aus[index_b]->num, x2, y2, z2 ) ;

            swap_tiles ( x1, y1, z1, x2, y2, z2, i ) ;

        }

        delete [] aus ;

        D10(cerr<<"D10 mix cube\n")

        return true ;
    }
    else
    {
        end_game() ;

    D10(cerr<<"D10 mix cube\n")

        return false ;
    }
}

void check_cube ()
{
    D1(cerr<<"D1 check cube\n")

    bool visited[dim_X][dim_Y] ;
    for ( int x = 0 ; x < dim_X ; x++ )
        for ( int y = 0 ; y < dim_Y ; y++ )
            visited[x][y] = false ;

     for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = dim_Z-1 ; z >= 0 ; z--)
            {

                if ( ( ! cube[x][y][z].empty ) && ( ! visited[x][y] )&&( cube[x][y][z].num < TILES ) )
                {
                    visited[x][y] = true ;
                    check_cell ( x,y,z ) ;
                }
                else
                    cube[x][y][z].lock = true ;
            }

    D10(cerr<<"D10 check cube\n")
}

void check_convenience ( tile * a, tile * b, bool &exit )
{
    D2(cerr<<"D2 check convenience\n")

    assert(a!=NULL) ;
    assert(b!=NULL) ;

    int xa, ya, za, xb, yb, zb ;
    find_coord ( a->num, xa, ya, za ) ;
    find_coord ( b->num, xb, yb, zb ) ;

    tile * left_a=NULL;
    tile * right_a=NULL;
    tile * under_a=NULL;
    tile * left_b=NULL;
    tile * right_b=NULL;
    tile * under_b=NULL ;

    initialize_neighbor ( left_a, right_a, under_a,
                          left_b, right_b, under_b,
                          xa, ya, za, xb, yb, zb, a, b    ) ;

    int temp_x, temp_y, temp_z ;

    if ( (exit)&&( left_a != NULL )&&( left_a->value > a->value ) )
    {
        find_coord( left_a->num, temp_x, temp_y, temp_z ) ;
        if ( ( temp_x-1 >= 0 )&&( cube[temp_x-1][temp_y][temp_z].empty ) )
            exit = false ;
    }
    if ( (exit)&&( right_a != NULL )&&( right_a->value > a->value ) )
    {
        find_coord( right_a->num, temp_x, temp_y, temp_z ) ;
        if ( ( temp_x+1 < dim_X )&&( cube[temp_x+1][temp_y][temp_z].empty ) )
            exit = false ;
    }
    if ( (exit)&&( under_a != NULL )&&( under_a->value > a->value) )
    {
        find_coord( under_a->num, temp_x, temp_y, temp_z ) ;
        if ( ( ( temp_x-1 >= 0 )&&( cube[temp_x-1][temp_y][temp_z].empty) ) ||
             ( ( temp_x+1 < dim_X)&&(cube[temp_x+1][temp_y][temp_z].empty) )  )
            exit = false ;
    }
    if ( (exit)&&( left_b != NULL )&&( left_b->value > b->value ) )
    {
        find_coord( left_b->num, temp_x, temp_y, temp_z ) ;
        if ( ( temp_x-1 >= 0 )&&( cube[temp_x-1][temp_y][temp_z].empty ) )
            exit = false ;
    }
    if ( (exit)&&( right_b != NULL )&&( right_b->value > b->value ) )
    {
        find_coord( right_b->num, temp_x, temp_y, temp_z ) ;
        if ( ( temp_x+1 < dim_X )&&( cube[temp_x+1][temp_y][temp_z].empty ) )
            exit = false ;
    }
    if ( (exit)&&( under_b != NULL )&&( under_b->value > b->value) )
    {
        find_coord( under_b->num, temp_x, temp_y, temp_z ) ;
        if ( ( ( temp_x-1 >= 0 )&&( cube[temp_x-1][temp_y][temp_z].empty) ) ||
             ( ( temp_x+1 < dim_X)&&(cube[temp_x+1][temp_y][temp_z].empty) )  )
            exit = false ;
    }

    D9(cerr<<"D9 check convenience\n")
}

void find_coord ( const int &num, int &_x, int &_y, int &_z )
{
    D2(cerr<<"D2 find coord\n")

    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                if ( cube[x][y][z].empty )
                    continue ;

                if ( cube[x][y][z].num == num )
                {
                    _x = x ;
                    _y = y ;
                    _z = z ;
                }
            }

    D9(cerr<<"D9 find coord\n")
}

void fill_cell ( const int &x, const int &y, const int &z, int &last)
{
    D1(cerr<<"D1 fill cell\n")

/*debug se valori x, y, z consoni*/
    cube[x][y][z].empty = false ;
    cube[x][y][z].num = last ;
    if ( last < TILES )
        cube[x][y][z].value = tile_value (last) ;
    else
        cube[x][y][z].value = 0 ;

    last++ ;

    D10(cerr<<"D10 fill cell\n")
}

void reset_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 reset cell\n")

    cube[x][y][z].empty = true ;
    cube[x][y][z].lock  = true ;
    cube[x][y][z].num   = -1 ;
    cube[x][y][z].value =  0 ;
    cube[x][y][z].x1    = -1 ;
    cube[x][y][z].y1    = -1 ;
    cube[x][y][z].x2    = -1 ;
    cube[x][y][z].y2    = -1 ;

    D9(cerr<<"D9 reset cell\n")
}

bool remove_dummies()
{
    D1(cerr<<"D1 remove dummies\n")

    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                if ( cube[x][y][z].num >= TILES )
                    reset_cell ( x, y, z ) ;
            }
    reset_highlighted_cell() ;
    check_cube () ;
    refresh_unlocked () ;
    if ( !sort_unlocked () )
    {
        D3(cerr<<"remove_dummies"<<endl)
        D8(cerr<<"sort unlocked returned false"<<endl)
        return false ;
    }
    if ( count_pairs_removable(0) == -1 )
    {
        D3(cerr<<"remove_dummies"<<endl)
        D8(cerr<<"count pairs removable returned -1"<<endl)
        return false ;
    }
    redraw_widget("playground") ;

    D10(cerr<<"D10 remove dummies\n")

    return true ;
}

void initialize_cube ()
{
    D1(cerr<<"D1 initialize cube\n")

    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
                reset_cell ( x, y, z ) ;

    D10(cerr<<"D10 initialize cube\n")
}

void fill_cube ()
{
    D1(cerr<<"D1 fill cube\n")

    int last = 0 ;

    switch ( level )
    {
        case easy:
                        fill_easy_layout( last ) ;                        
                        break ;
        case medium:
                        fill_medium_layout( last ) ;
                        break ;
        case difficult:
                        fill_difficult_layout( last ) ;
                        break ;
    /*debug se last != 144*/
    }

    D10(cerr<<"D10 fill cube\n")
}
/*fine implementazione funzioni pubbliche*/

/*inizio implementazione funzioni private*/

static void initialize_neighbor ( tile * &left_a, tile * &right_a, tile * &under_a,
                           tile * &left_b, tile * &right_b, tile * &under_b,
                           const int &xa, const int &ya, const int &za,
                           const int &xb, const int &yb, const int &zb,
                           tile * &a, tile * &b )
{
    D2(cerr<<"D2 initialize neighbor\n")

    if ((xa-1 >= 0)&&( !cube[xa-1][ya][za].empty ))
        left_a = &(cube[xa-1][ya][za]) ;
    if ((xa+1 < dim_X)&&( !cube[xa+1][ya][za].empty ))
        right_a = &(cube[xa+1][ya][za]) ;
    if ((za-1 >= 0)&&( !cube[xa][ya][za-1].empty ))
        under_a = &(cube[xa][ya][za-1]) ;

    if ((xb-1 >= 0)&&( !cube[xb-1][yb][zb].empty ))
        left_b = &(cube[xb-1][yb][zb]) ;
    if ((xb+1 < dim_X)&&( !cube[xb+1][yb][zb].empty ))
        right_b = &(cube[xb+1][yb][zb]) ;
    if ((zb-1 >= 0)&&( !cube[xb][yb][zb-1].empty ))
        under_b = &(cube[xb][yb][zb-1]) ;
    /*controllo i casi particolari ed escludo le tessere comuni*/
    if (( left_a == right_b )||( left_b == right_a ))
    {
        if ( left_b == right_a )
        {
            tile * temp = a ;
            a = b ;
            b = temp ;
        }
        right_b = NULL ;
    } else 
    if (( left_a == b )||(left_b == a))
    {
        if ( left_b ==a )
        {
            tile * temp = a ;
            a = b ;
            b = temp ;
        }
        left_a  = NULL ;
        right_b = NULL ;
    } else
    if ( ( under_a == right_b )||( under_b == right_a ) )
    {
        if ( under_b == left_a )
        {
            tile * temp = a ;
            a = b ;
            b = temp ;
        }
        left_a  = NULL ;
        right_b = NULL ;
    } else
    if ( ( under_a == left_b )||( under_b == left_a ) )
    {
        if ( under_b == left_a )
        {
            tile * temp = a ;
            a = b ;
            b = temp ;
        }
        right_a = NULL ;
        left_a  = NULL ;
    }

    D9(cerr<<"D9 initialize neighbor\n")
}

static void check_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 check cell\n")

    if ( left_cell ( x,y,z ) || right_cell ( x,y,z ) )
        cube[x][y][z].lock = false ;
    else
        cube[x][y][z].lock = true ;

    D9(cerr<<"D9 check cell\n")
}

static bool left_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 left cell\n")

    D9(cerr<<"D9 left cell\n")

    if ( x == 0 )
        return true ;
    else
        return cube[x-1][y][z].empty ;
}

static bool right_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 right cell\n")

    D9(cerr<<"D9 right cell\n")

    if ( x == ( dim_X-1 ) )
        return true ;
    else
        return cube[x+1][y][z].empty ;
}

static void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i )
{
    D2(cerr<<"D2 swap tiles\n")

    tile &rif_a = cube[x1][y1][z1] ;
    tile &rif_b = cube[x2][y2][z2] ;

    assert(&rif_a != NULL ) ;
    assert(&rif_b != NULL ) ;

    if ( (rif_a.empty)||(rif_b.empty)||(rif_a.num==rif_b.num)||(rif_a.num >= TILES)||(rif_b.num >= TILES ) )
        i-- ;
    else
    {
        /*swap empty informations*/
        rif_a.empty = rif_a.empty ^ rif_b.empty ;
        rif_b.empty = rif_a.empty ^ rif_b.empty ;
        rif_a.empty = rif_a.empty ^ rif_b.empty ;
        /*swap lock informations*/
        rif_a.lock = rif_a.lock ^ rif_b.lock ;
        rif_b.lock = rif_a.lock ^ rif_b.lock ;
        rif_a.lock = rif_a.lock ^ rif_b.lock ;
        /*swap num informations*/
        rif_a.num = rif_a.num ^ rif_b.num ;
        rif_b.num = rif_a.num ^ rif_b.num ;
        rif_a.num = rif_a.num ^ rif_b.num ;
        /*swap value informations*/
        rif_a.value = rif_a.value ^ rif_b.value ;
        rif_b.value = rif_a.value ^ rif_b.value ;
        rif_a.value = rif_a.value ^ rif_b.value ;
    }

    D9(cerr<<"D9 swap tiles\n")
}

static void fill_easy_layout ( int &last )
{
    D1(cerr<<"D1 fill easy layout\n")

    /*primo strato*/
	fill_floor ( 2, 9, 1, 6, 0, last ) ;
	fill_floor ( 0,11, 0, 0, 0, last ) ;
	fill_floor ( 0,11, 7, 7, 0, last ) ;
	fill_floor ( 0, 1, 3, 4, 0, last ) ;
	fill_floor (10,11, 3, 4, 0, last ) ;
	fill_cell  ( 1, 2, 0, last ) ;
	fill_cell  ( 1, 5, 0, last ) ;
	fill_cell  (10, 2, 0, last ) ;
	fill_cell  (10, 5, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 3, 8, 1, 6, 1, last ) ;
	fill_cell  ( 0, 0, 1, last ) ;
	fill_cell  ( 0, 7, 1, last ) ;
	fill_cell  (11, 0, 1, last ) ;
	fill_cell  (11, 7, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 4, 7, 2, 5, 2, last ) ;
    /*quarto strato*/
	fill_floor ( 5, 6, 3, 4, 3, last ) ;

    D10(cerr<<"D10 fill easy layout\n")
}

static void fill_medium_layout ( int &last )
{
    D1(cerr<<"D1 fill medium layout\n")

    /*primo strato*/
	fill_floor ( 0,11, 0, 5, 0, last ) ;
	fill_floor ( 1,10, 7, 7, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 0, 0, 0, 4, 1, last ) ;
	fill_floor ( 2, 2, 0, 4, 1, last ) ;
	fill_floor ( 4, 4, 0, 4, 1, last ) ;
	fill_floor ( 6, 6, 0, 4, 1, last ) ;
	fill_floor ( 8, 8, 0, 4, 1, last ) ;
	fill_floor (10,10, 0, 4, 1, last ) ;
	fill_floor ( 2, 9, 7, 7, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 0, 0, 0, 3, 2, last ) ;
	fill_floor ( 2, 2, 0, 3, 2, last ) ;
	fill_floor ( 4, 4, 0, 3, 2, last ) ;
	fill_floor ( 6, 6, 0, 3, 2, last ) ;
	fill_floor ( 8, 8, 0, 3, 2, last ) ;
	fill_floor (10,10, 0, 3, 2, last ) ;
    /*quarto strato VUOTO volutamente*/

    D10(cerr<<"D10 fill medium layout\n")
}

static void fill_difficult_layout ( int &last )
{
    D1(cerr<<"D1 fill difficult layout\n")

    /*primo strato*/
	fill_floor ( 0, 4, 4, 7, 0, last ) ;
	fill_floor ( 7,11, 4, 7, 0, last ) ;
	fill_floor ( 0, 2, 0, 2, 0, last ) ;
	fill_floor ( 9,11, 0, 2, 0, last ) ;
	fill_floor ( 4, 7, 0, 3, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 1, 3, 5, 6, 1, last ) ;
	fill_floor ( 8,10, 5, 6, 1, last ) ;
	fill_floor ( 0, 1, 0, 1, 1, last ) ;
	fill_cell  ( 0, 2, 1, last ) ;
	fill_cell  ( 2, 0, 1, last ) ;
	fill_floor (10,11, 0, 1, 1, last ) ;
	fill_cell  ( 9, 0, 1, last ) ;
	fill_cell  (11, 2, 1, last ) ;
	fill_floor ( 4, 7, 0, 2, 1, last ) ;
	fill_cell  ( 4, 3, 1, last ) ;
	fill_cell  ( 7, 3, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 1, 2, 5, 6, 2, last ) ;
	fill_floor ( 9,10, 5, 6, 2, last ) ;
	fill_floor ( 0, 1, 0, 0, 2, last ) ;
	fill_cell  ( 0, 1, 2, last ) ;
	fill_floor (10,11, 0, 0, 2, last ) ;
	fill_cell  (11, 1, 2, last ) ;
	fill_floor ( 4, 7, 0, 1, 2, last ) ;
	fill_cell  ( 4, 2, 2, last ) ;
	fill_cell  ( 7, 2, 2, last ) ;
    /*quarto strato*/
	fill_cell  ( 0, 0, 3, last ) ;
	fill_cell  (11, 0, 3, last ) ;
	fill_cell  ( 4, 1, 3, last ) ;
	fill_cell  ( 7, 1, 3, last ) ;
	fill_floor ( 4, 7, 0, 0, 3, last ) ;

    D10(cerr<<"D10 fill difficult layout\n")
}

static void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last )
{
    D1(cerr<<"D1 fill floor\n")

/*debug se valori x, y, z consoni*/
    for ( int x = x1 ; x <= x2 ; x++ )
        for ( int y = y1 ; y <=y2 ; y++ )
            fill_cell ( x, y, z, last ) ;

    D10(cerr<<"D10 fill floor\n")
}
/*fine implementazione funzioni private*/

/*fine implementazione modulo_CUBE*/
