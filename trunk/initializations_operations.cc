#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "data_structure.h"
#include "initializations_operations.h"

using namespace std ;


/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube ()
{
    cube = new tile ** [dim_X] ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        cube[x] = new tile * [dim_Y] ;

        for ( int y = 0 ; y < dim_Y ; y++ )
            cube[x][y] = new tile [dim_Z] ;
    }
}

/*
 * Funzione che dealloca lo spazio 3D (x, y, z) di celle per tessere
 */
void delete_cube ()
{
    for ( int x = 0 ; x < dim_X ; x++ )
    {
        for ( int y = 0 ; y < dim_Y ; y++ )
            delete [] cube[x][y] ;

        delete [] cube[x] ;
    }
    delete [] cube ;
    cube = NULL ;
}

/*
 * Funzione che inizializza tutte le celle del cubo
 */
void initialize_cube ()
{
    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
                reset_cell ( x, y, z ) ;

}

/*
 * Funzione che inserisce nelle celle del cubo le tessere secondo il livello
 * easy, medium, difficult
 */
void fill_cube ()
{
    /*debug controllo array nomi tessere non vuoto*/

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

    mix_cube () ;

}

/*
 * Funzione che mischia le tessere del cubo
 * (in un numero random di scambi)
 */
void mix_cube ()
{
    srand ( time(0) ) ;

    /* genero un numero massimo di swap tra le tessere
     * compreso tra 719 e 1009
     * nota: 719 e 1009 sono numeri primi, mentre 291 non lo e'.
     * in media ci saranno tra i 5 e i 7 swap per tessera.
     */
    int max = 719 + ( rand() % 291 ) ;

    int x1, x2, y1, y2, z1, z2 ;
    for ( int i = 0 ; i < max ; i++ )
    {
        generate_random ( x1, y1, z1, x2, y2, z2 ) ;
        swap_tiles( x1, y1, z1, x2, y2, z2, i ) ;
    }
    check_cube() ;
}

/*
 * Funzione che genera due coordinate random
 */
void generate_random ( int &x1, int &y1, int &z1, int &x2, int &y2, int &z2 )
{
    x1 = rand()%dim_X ;
    y1 = rand()%dim_Y ;
    z1 = rand()%dim_Z ;

    x2 = rand()%dim_X ;
    y2 = rand()%dim_Y ;
    z2 = rand()%dim_Z ;
}

/*
 * Funzione che aggiorna la removibilita' delle celle
 * controllando che almeno una delle due adiacenti sia libera
 */
void check_cube ()
{
    bool visited[dim_X][dim_Y] ;
    for ( int x = 0 ; x < dim_X ; x++ )
        for ( int y = 0 ; y < dim_Y ; y++ )
            visited[x][y] = false ;

     for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = dim_Z-1 ; z >= 0 ; z--)
            {

                if ( ( ! cube[x][y][z].empty ) && ( ! visited[x][y] ) )
                {
                    visited[x][y] = true ;
                    check_cell ( x,y,z ) ;
                }
                else
                    cube[x][y][z].lock = true ;
            }
    refresh_unlocked () ;
}

/*
 * Funzione che ricalcola l'array unlocked di puntatori alle tessere
 * rimuovibili (solo se il primo elemento e' a NULL), e ordina
 * quest'ultimo in ordine decrescente di valore.
 */
void refresh_unlocked ()
{
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

    sort_unlocked() ;
}

bool check_pair ( tile* a, tile* b, tile* &first, tile* &second )
{

    if ( 0 == strcmp ( name[a->num].word, name[b->num].word ) )
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
}

void extract_pair ( couple *  pair )
{
    int temp = 0 ;
    bool exit = false ;
    tile * first = NULL ;
    tile * second = NULL ;
    switch ( ai )
    {
        case airhead :      do
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
                                    exit = check_pair ( unlocked[temp],
                                                        unlocked[temp-1],
                                                        first,
                                                        second ) ;
                                if ( ( !exit ) && ( unlocked[temp+1] != NULL ) )
                                    exit = check_pair ( unlocked[temp],
                                                        unlocked[temp+1],
                                                        first,
                                                        second ) ;
                            } while ( ! exit ) ;
                            break ;

        case greedy :       for ( int h = 1 ; ((h<FREE)&&(!exit)) ; h++ )
                            {
                                exit = check_pair( unlocked[h-1], unlocked[h],
                                                   first, second ) ;
                            }
                            if ( !exit )
                                cout<<"errore_in_extract_pair\n" ;
                            break ;
        case thoughtful :   
                            break ;
    } ;

    pair->t1    = first->num ;
    pair->name1 = name[ first->num ].word ;
    find_coord ( first->num, pair->x1, pair->y1, pair->z1 ) ;

    pair->t2    = second->num ;
    pair->name2 = name[ second->num ].word ;
    find_coord ( second->num, pair->x2, pair->y2, pair->z2 ) ;
}

void find_coord ( const int &num, int &_x, int &_y, int &_z )
{
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
}

bool check_solvability ( int counter )
{
    if ( counter == 5 )
    {
/*debug se partita impossibile*/
cerr<<"mosse finite" ;
    return false ;
    }

    bool ret = false ;
    for ( int i = 1 ; ((i < FREE)&&(!ret)) ; i++ )
    {
        if ( ( unlocked[i] == NULL ) || ( unlocked[i-1] == NULL ) )
            continue ;
        else
        {
            if ( ( 0 == strcmp ( name[unlocked[i-1]->num].word,
                                 name[unlocked[i]->num].word    )   ) )
            {
                ret = true ;
            }
            else if ( between( 136, unlocked[i-1]->num, 139 ) &&
                      between( 136, unlocked[i]->num  , 139 )    )
            {
                ret = true ;
            }
            else if ( between( 140, unlocked[i-1]->num, 143 ) &&
                      between( 140, unlocked[i]->num  , 143 )    )
            {
                ret = true ;
            }

        }
    }

    if ( ret == false )
    {
        mix_cube() ;
        return check_solvability( counter+1 ) ;
    }
    else
        return ret ;
}

bool between ( const int &min, const int &middle, const int &max )
{
    if ( min > max )
        return between( max, middle, min ) ;

    if ( ( min <= middle )&&( middle <= max ) )
        return true ;
    else
        return false ;
}

/*
 * Funzione che aggiorna la removibilita' di una singola tessera
 * visitando le celle destra e sinistra
 */
void check_cell ( const int &x, const int &y, const int &z )
{
    if ( left_cell ( x,y,z ) || right_cell ( x,y,z ) )
        cube[x][y][z].lock = false ;
    else
        cube[x][y][z].lock = true ;
}

/*
 * Funzione che ritorna true se la cella sinistra e' libera
 */
bool left_cell ( const int &x, const int &y, const int &z )
{
    if ( x == 0 )
        return true ;
    else
        return cube[x-1][y][z].empty ;
}

/*
 * Funzione che ritorna true se la cella destra e' libera
 */
bool right_cell ( const int &x, const int &y, const int &z )
{
    if ( x == ( dim_X-1 ) )
        return true ;
    else
        return cube[x+1][y][z].empty ;
}

/*
 * Funzione che ordina l'array di puntatori a tessere rimuovibili "unlocked"
 * in ordine decrescente di valore
 */
void sort_unlocked()
{
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
}

/*
 * Funzione che ordina alfabeticamente un sotto array
 */
void sort_sub_array ( tile ** out, int start, int end )
{
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
}

/*
 * Funzione che scambia due tessere passate tramite le coordinate
 * x1, y1, z1 <--> x2, y2, z2
 */
void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i )
{
    tile &rif_a = cube[x1][y1][z1] ;
    tile &rif_b = cube[x2][y2][z2] ;
    if ( (rif_a.empty)||(rif_b.empty)||(rif_a.num==rif_b.num) )
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
}



/*
 * Funzione di popolamento del cubo con disposizione livello easy
 */
void fill_easy_layout ( int &last )
{
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
}

/*
 * Funzione di popolamento del cubo con disposizione livello medium
 */
void fill_medium_layout ( int &last )
{
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
}

/*
 * Funzione di popolamento del cubo con disposizione livello difficult
 */
void fill_difficult_layout ( int &last )
{
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
}

/*
 * Funzione che passato in ingresso il numero della tessera
 * ritorna il valore della stessa
 */
int tile_value (const int &name_position)
{
/*debug se name_position e' una posizione dell'array*/

    /* si tratta delle prime 108 tessere:
     * circle, bamboo, cross, numerati da 1 a 9
     */
    if      ( name_position <= 107 )
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
        
}

/*
 * Funzione che prende in ingresso le coordinate x, y, z e il numero di tessera,
 * ed inserisce nella cella x, y, z la tessera corrispondente;
 * inoltre incrementa il numero di tessera di uno.
 */
void fill_cell ( const int &x, const int &y, const int &z, int &last)
{
/*debug se valori x, y, z consoni*/
    cube[x][y][z].empty = false ;
    cube[x][y][z].num = last ;
    cube[x][y][z].value = tile_value (last) ;

    last++ ;

}

/*
 * Funzione che riempie un rettangolo di celle tali che
 * x1 <= x <= x2 e y1 <= y <= y2 e altezza = z
 * con numeri di tessera progressivi
 */
void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last )
{
/*debug se valori x, y, z consoni*/
    for ( int x = x1 ; x <= x2 ; x++ )
        for ( int y = y1 ; y <=y2 ; y++ )
            fill_cell ( x, y, z, last ) ;
}

void reset_cell ( const int &x, const int &y, const int &z )
{
    cube[x][y][z].empty = true ;
    cube[x][y][z].lock  = true ;
    cube[x][y][z].num   = -1 ;
    cube[x][y][z].value =  0 ;
    cube[x][y][z].x1    = -1 ;
    cube[x][y][z].y1    = -1 ;
    cube[x][y][z].x2    = -1 ;
    cube[x][y][z].y2    = -1 ;
}





