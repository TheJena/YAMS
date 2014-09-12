#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "gui.h"
#include "game.h"
#include "debug_macros.h"

using namespace std ;

bool lock_mix = false ;

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

    D10(cerr<<"D10 create cube\n")
}

void delete_cube ()
{
    D1(cerr<<"D1 delete cube\n")

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

bool mix_cube ()
{
    D1(cerr<<"D1 mixcube\n")

    if ( !remove_dummies() )
        return false ;

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

            find_coord ( aus[index_a]->num, x1, y1, z1 ) ;
            find_coord ( aus[index_b]->num, x2, y2, z2 ) ;

            swap_tiles ( x1, y1, z1, x2, y2, z2, i ) ;

        }

        delete [] aus ;

        return true ;
    }
    else
    {
        end_game() ;
        return false ;
    }

    D10(cerr<<"D10 mix cube\n")
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

void refresh_unlocked ()
{
    D1(cerr<<"D1 refresh unlocked\n")

    for ( int x = 0 ; x < FREE ; x++ )
        unlocked[x] = NULL ;


        int u = 0 ;
        for ( int x = 0 ; x < dim_X ; x++)
            for ( int y = 0 ; y < dim_Y ; y++)
                for ( int z = 0 ; z < dim_Z ; z++)
                    if ( ( !cube[x][y][z].empty ) && ( !cube[x][y][z].lock ) )
                    {
                        unlocked[u] = &(cube[x][y][z]) ;
                        u++ ;
                    }
/*debug se u > FREE*/

    D10(cerr<<"D10 refresh unlocked\n")
}

bool check_pair ( tile* a, tile* b, tile* &first, tile* &second )
{
    D2(cerr<<"D2 check pair\n")

    if ( a->num == b->num )
        return false ;
    else if ( 0 == strcmp ( name[a->num].word, name[b->num].word ) )
    {
        first = a ;
        second = b ;
        return true ;
    } else
    if ( ( between (136, a->num, 139 ) ) && ( between (136, b->num  , 139 ) ) )
    {
        first = a ;
        second = b ;
        return true ;
    } else
    if ( ( between (140, a->num, 143 ) ) && ( between (140, b->num  , 143 ) ) )
    {
        first = a ;
        second = b ;
        return true ;
    } else
    {
        return false ;
    }

    D9(cerr<<"D9 check pair\n")
}

void initialize_neighbor ( tile * &left_a, tile * &right_a, tile * &under_a,
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


void check_convenience ( tile * a, tile * b, bool &exit )
{
    D2(cerr<<"D2 check convenience\n")

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

bool airhead_extraction ( tile * &first, tile * &second, bool &exit )
{
    D2(cerr<<"D2 airhead extraction\n")

    if ( count_pairs_removable(0) >= 1)
    {
        int temp = 0 ;
        do
        {
            do
            {   /*
                 * genero un random tra 1 e 142 cosi'
                 * evito di dover controllare che le
                 * celle attigue non siano fuori dall'array
                 */
                temp = 1 + rand()%(FREE-2) ;
            } while ( unlocked[temp] == NULL ) ;
            if ( unlocked[temp-1] != NULL )
                exit = check_pair(unlocked[temp], unlocked[temp-1], first, second);
            if ( ( !exit ) && ( unlocked[temp+1] != NULL ) )
                exit = check_pair(unlocked[temp], unlocked[temp+1], first, second);
        } while ( ! exit ) ;
        return true ;
    }
    else
    {
        end_game() ;
        return false ;
    }

    D9(cerr<<"D9 airhead extraction\n")
}

bool extract_pair ( couple *  pair )
{
    D2(cerr<<"D2 extract pair\n")

    bool exit = false ;
    tile * first = NULL ;
    tile * second = NULL ;
    switch ( ai )
    {
        case airhead :      if ( !airhead_extraction( first, second, exit ) )
                                return false ;
                            break ;

        case greedy :       for ( int h = 1 ; ((h<FREE)&&(!exit)) ; h++ )
                            {
                                if ((unlocked[h-1]!=NULL )&&(unlocked[h]!=NULL))
                                    exit = check_pair( unlocked[h-1],
                                                       unlocked[h], first,
                                                       second ) ;
                            }
                            if ( !exit )
                                end_game() ;
                            break ;

        case thoughtful :   for ( int h = 1 ; ((h<FREE)&&(!exit)) ; h++ )
                            {
                                if ((unlocked[h-1]!=NULL )&&(unlocked[h]!=NULL))
                                    exit = check_pair( unlocked[h-1],
                                                       unlocked[h], first,
                                                       second ) ;
                                if (exit)
                                    check_convenience ( first, second, exit ) ;
                            }
                            if ( !exit )
                            {
                                if ( !airhead_extraction( first, second, exit ) )
                                    return false ;
cerr<<"a casoooooooooo\n";

                            }
                            break ;
    } ;

    pair->t1    = first->num ;
    pair->name1 = name[ first->num ].word ;
    find_coord ( first->num, pair->x1, pair->y1, pair->z1 ) ;

    pair->t2    = second->num ;
    pair->name2 = name[ second->num ].word ;
    find_coord ( second->num, pair->x2, pair->y2, pair->z2 ) ;

    set_highlighted_cell ( 1, pair->x1, pair->y1, pair->z1 ) ;
    set_highlighted_cell ( 2, pair->x2, pair->y2, pair->z2 ) ;

    redraw_widget("playground") ;

    return true ;

    D9(cerr<<"D9 extract pair\n")
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

bool between ( const int &min, const int &middle, const int &max )
{
    D2(cerr<<"D2 between\n")

    if ( min > max )
        return between( max, middle, min ) ;

    if ( ( min <= middle )&&( middle <= max ) )
        return true ;
    else
        return false ;

    D9(cerr<<"D9 between\n")
}

void check_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 check cell\n")

    if ( left_cell ( x,y,z ) || right_cell ( x,y,z ) )
        cube[x][y][z].lock = false ;
    else
        cube[x][y][z].lock = true ;

    D9(cerr<<"D9 check cell\n")
}

bool left_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 left cell\n")

    if ( x == 0 )
        return true ;
    else
        return cube[x-1][y][z].empty ;

    D9(cerr<<"D9 left cell\n")
}

bool right_cell ( const int &x, const int &y, const int &z )
{
    D2(cerr<<"D2 right cell\n")

    if ( x == ( dim_X-1 ) )
        return true ;
    else
        return cube[x+1][y][z].empty ;

    D9(cerr<<"D9 right cell\n")
}

bool sort_unlocked()
{
    D2(cerr<<"D2 sort unlocked\n")

    int n_seed      = 0 ;
    int n_wind      = 0 ;
    int n_dragon    = 0 ;
    int n_season    = 0 ;
    int n_flower    = 0 ;

    int temp = 0 ;

    for ( int i = 0 ; i < FREE ; i++ )
    {
        if ( unlocked[i] == NULL )
        {
            continue ;
        }
        else
        {
        temp = unlocked[i]->value ;

        if      ( temp == SEEDVALUE )
            n_seed++ ;
        else if ( temp == WINDVALUE )
            n_wind++ ;
        else if ( temp == DRAGONVALUE )
            n_dragon++ ;
        else if ( temp == SEASONVALUE )
            n_season++ ;
        else if ( temp == FLOWERVALUE )
            n_flower++ ;
        else
            ; /*debuggare questo caso*/     
        }
    }

    const int n_out = n_seed + n_wind + n_dragon + n_season + n_flower ;

    tile ** _output = new tile * [n_out] ;

    const int max_seed      = n_seed     - 1 ;
    const int max_wind      = max_seed   + n_wind ;
    const int max_dragon    = max_wind   + n_dragon ;
    const int max_season    = max_dragon + n_season ;
    const int max_flower    = max_season + n_flower ;

    n_seed      =              0 ;
    n_wind      = max_seed   + 1 ;
    n_dragon    = max_wind   + 1 ;
    n_season    = max_dragon + 1 ;
    n_flower    = max_season + 1 ;

    for ( int j = 0 ; j < FREE ; j++ )
    {
        if ( unlocked[j] == NULL )
            continue ;

        temp = unlocked[j]->value ;

        if      ( ( temp == SEEDVALUE )&&( n_seed <= max_seed ) )
        {
            _output[n_seed] = unlocked[j] ;
            n_seed++ ;
        }
        else if ( ( temp == WINDVALUE )&&( n_wind <= max_wind ) )
        {
            _output[n_wind] = unlocked[j] ;
            n_wind++ ;
        }
        else if ( ( temp == DRAGONVALUE )&&( n_dragon <= max_dragon ) )
        {
            _output[n_dragon] = unlocked[j] ;
            n_dragon++ ;
        }
        else if ( ( temp == SEASONVALUE )&&(n_season <= max_season ) )
        {
            _output[n_season] = unlocked[j] ;
            n_season++ ;
        }
        else if ( ( temp == FLOWERVALUE )&&(n_flower <= max_flower ) )
        {
            _output[n_flower] = unlocked[j] ;
            n_flower++ ;
        }
        else
            ; /*debuggare questo caso*/

        unlocked[j] = NULL ;
    }

    sort_sub_array( _output, 0,              max_seed   ) ;
    sort_sub_array( _output, max_seed   + 1, max_wind   ) ;
    sort_sub_array( _output, max_wind   + 1, max_dragon ) ;
    sort_sub_array( _output, max_dragon + 1, max_season ) ;
    sort_sub_array( _output, max_season + 1, max_flower ) ;

    int q = 0 ;
    for ( int i = max_flower ; i >= 0 ; i--, q++ )
        unlocked[q] = _output[i] ;

    for ( ; q < FREE ; q++ )
        unlocked[q] = NULL ;

    delete [] _output ;

    if ( count_pairs_removable(0) == -1 )
        return false ;
    else
        return true ;

    D9(cerr<<"D9 sort unlocked\n")
}

int count_pairs_removable ( const int &count)
{
    D2(cerr<<"D2 count pairs removable\n")

    int couples = 0 ;
    for ( int i = 1 ; i < FREE ; i++ )
    {
        if ( ( unlocked[i-1] != NULL )&&( unlocked[i] != NULL )&&
             (unlocked[i-1]->num != unlocked[i]->num )            )
        {
            if ( ( 0 == strcmp( name[unlocked[i-1]->num].word,
                                name[unlocked[i]->num].word))    ||
                 ( ( between (136, unlocked[i-1]->num, 139 ) ) &&
                   ( between (136, unlocked[i]->num  , 139 ) ) ) ||
                 ( ( between (140, unlocked[i-1]->num, 143 ) ) &&
                   ( between (140, unlocked[i]->num  , 143 ) ) ) )
            {
                couples++;
                i++;
            }
        }
    }

    if ( couples < 0)
    {
        /*debug here*/ ;
        return 0 ;
cerr<<"error in count pairs removable, couples negative\n" ;
        end_game() ;
    }
    else if ( couples == 0 )
    {
        if ( count == 0 )
        {
            cerr<<"mosse finite\n" ;
            end_game () ;
            return -1 ;
        }
        else
        {
            mix_cube() ;
            check_cube() ;
            refresh_unlocked() ;
            sort_unlocked() ;
            return count_pairs_removable ( count-1 ) ;
        }
    }
    else
    {
        refresh_down_label ( couples ) ;
        return couples ;
    }

    D9(cerr<<"D9 count pairs removable\n")
}

void sort_sub_array ( tile ** out, int start, int end )
{
    D2(cerr<<"D2 sort sub array\n")

    tile * temp = NULL ;

    for ( int i = start ; i<=end ; i++ )
        for ( int j = i+1 ; j<=end ; j++ )
        {
            if (strcmp(name[out[i]->num].word, name[out[j]->num].word) > 0)
            {
                temp = out[i] ; 
                out[i] = out[j] ;
                out[j] = temp ;
            }
        }

    D9(cerr<<"D9 sort sub array\n")
}

void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i )
{
    D2(cerr<<"D2 swap tiles\n")

    tile &rif_a = cube[x1][y1][z1] ;
    tile &rif_b = cube[x2][y2][z2] ;
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

void fill_easy_layout ( int &last )
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

void fill_medium_layout ( int &last )
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

void fill_difficult_layout ( int &last )
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

int tile_value (const int &name_position)
{
    D2(cerr<<"D2 tile value\n")

/*debug se name_position e' una posizione dell'array*/

    /* si tratta delle prime 108 tessere:
     * circle, bamboo, cross, numerati da 1 a 9
     */
    if ( ( name_position >= 0 ) && ( name_position <= 107 ) )
        return SEEDVALUE ;
    else
    /* si tratta delle 16 tessere dei venti:
     * east, sud, west, north
     */
    if ( name_position <= 123 )
        return WINDVALUE ;
    else
    /* si tratta delle 12 tessere dei draghi:
     * red, green, white dragon
     */
    if ( name_position <= 135 )
        return DRAGONVALUE ;
    else
    /* si tratta delle 4 tessere delle stagioni:
     * spring, summer, autumn, winter
     */
    if ( name_position <= 139 )
        return SEASONVALUE ;
    else
    /* si tratta delle 4 tessere dei fiori:
     * plumb, orchid, chrysanthemum, bamboo
     */
    if ( name_position <= 143 )
        return FLOWERVALUE ;
    else
    /* attenzione errore di gestione della memoria
     */
/*debug se name_position eccede l'array*/
        return -1 ;

    D9(cerr<<"D9 tile value\n")
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

void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last )
{
    D1(cerr<<"D1 fill floor\n")

/*debug se valori x, y, z consoni*/
    for ( int x = x1 ; x <= x2 ; x++ )
        for ( int y = y1 ; y <=y2 ; y++ )
            fill_cell ( x, y, z, last ) ;

    D10(cerr<<"D10 fill floor\n")
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
        return false ;
    if ( count_pairs_removable(0) == -1 )
        return false ;
    redraw_widget("playground") ;

    D10(cerr<<"D10 remove dummies\n")
    return true ;
}
