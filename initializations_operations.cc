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
            {
                cube[x][y][z].empty = true ;
                cube[x][y][z].lock  = true ;
                cube[x][y][z].num   = -1 ;
                cube[x][y][z].value = 0 ;
            }
    unlocked[0] = NULL ;
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
     * compreso tra 1439 e 4493
     * nota: 1439 e 4493 sono numeri primi, mentre 3055 non lo e'.
     */
    int max = 1439 + ( rand() % 3055 ) ;

    int x1, x2, y1, y2, z1, z2 ;
    for ( int i = 0 ; i < max ; i++ )
    {
        generate_random ( x1, y1, z1, x2, y2, z2 ) ;
        swap_tiles( x1, y1, z1, x2, y2, z2, i ) ;
    }
}

/*
 * Funzione che genera due coordinate random
 */
void generate_random ( int &x1, int &y1, int &z1, int &x2, int &y2, int &z2 )
{
    do
    {
        x1 = rand()%dim_X ;
        x2 = rand()%dim_X ;
    }
    while ( x1 != x2 ) ;
    do
    {
        y1 = rand()%dim_Y ;
        y2 = rand()%dim_Y ;
    }
    while ( x1 != x2 ) ;
    do
    {
        z1 = rand()%dim_Z ;
        z2 = rand()%dim_Z ;
    }
    while ( x1 != x2 ) ;

    srand ( rand() ) ;
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
}

/*
 * Funzione che ricalcola l'array unlocked di puntatori alle tessere
 * rimuovibili (solo se il primo elemento e' a NULL), e ordina
 * quest'ultimo in ordine decrescente di valore.
 */
void refresh_unlocked ()
{
    if ( unlocked[0] == NULL )
    {
        int u = 0 ;
        for ( int x = 0 ; x < dim_X ; x++)
            for ( int y = 0 ; y < dim_Y ; y++)
                for ( int z = 0 ; z < dim_Z ; z++)
                    if ( ( !cube[x][y][z].empty ) && ( !cube[x][y][z].lock ) )
                    {
                        unlocked[u] = &(cube[x][y][z]) ;
                        u++ ;
                    }
/*debug se u > 144*/
    }
    sort_unlocked() ;
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
    int seed = 0 ;
    int wind = 0 ;
    int dragon = 0 ;
    int season = 0 ;
    int flower = 0 ;
    seed=0; wind=0; dragon=0; season=0; flower=0;
    for ( int i = 0 ; i < TILES ; i++ )
    {
        if ( unlocked[i] == NULL )
            continue ;
        else
        {
            if      ( unlocked[i]->value == SEEDVALUE )
                seed++;
            else if ( unlocked[i]->value == WINDVALUE )
                wind++;
            else if ( unlocked[i]->value == DRAGONVALUE )
                dragon++;
            else if ( unlocked[i]->value == SEASONVALUE )
                season++;
            else if ( unlocked[i]->value == FLOWERVALUE )
                flower++;
        }
    }

    const int end_seed      = seed ;
    const int end_wind      = seed + wind ;
    const int end_dragon    = seed + wind + dragon ;
    const int end_season    = seed + wind + dragon + season ;
    const int end_flower    = seed + wind + dragon + season + flower ;
    const int removable = seed+wind+dragon+season+flower ;
    tile ** _output = new tile* [removable] ;
    for ( int   a=0,
                b=a+seed,
                c=b+wind,
                d=c+dragon,
                e=d+season,
                j=0         ;   (j < TILES)     &&
                                (a<=end_seed)   &&
                                (b<=end_wind)   &&
                                (c<=end_dragon) &&
                                (d<=end_season) &&
                                (e<=end_flower)     ; j++ )
    {
        if (unlocked[j] == NULL )
            continue ;
        else
        {
            if      ( unlocked[j]->value == SEEDVALUE )
            {    _output[a] = unlocked[j] ;
                a++ ;
            }
            else if ( unlocked[j]->value == WINDVALUE )
            {    _output[b] = unlocked[j] ;
                b++ ;
            }
            else if ( unlocked[j]->value == DRAGONVALUE )
            {    _output[c] = unlocked[j] ;
                c++ ;
            }
             else if ( unlocked[j]->value == SEASONVALUE )
            {    _output[d] = unlocked[j] ;
                d++ ;
            }
            else if ( unlocked[j]->value == FLOWERVALUE )
            {    _output[e] = unlocked[j] ;
                e++ ;
            }
        }
    }

    sort_sub_array ( _output, 0,            end_seed-1 ) ;

    sort_sub_array ( _output, end_seed,   end_wind-1 ) ;

    sort_sub_array ( _output, end_wind,   end_dragon-1 ) ;

    sort_sub_array ( _output, end_dragon, end_season-1 ) ;

    sort_sub_array ( _output, end_season, removable-1 ) ;


    for ( int a = 0, b = removable-1 ; ((a<removable)&&(b>=0)) ; a++, b-- )
        unlocked[a] = _output[b] ;
    for ( int z = removable ; z < TILES ; z++ )
        unlocked[z] = NULL ;

    delete [] _output ;

}

/*
 * Funzione che ordina alfabeticamente un sotto array
 */
void sort_sub_array ( tile ** out, int start, int end )
{

for (int x= start ; x<=end ; x++)
    if ( start < end )
    {
        for ( int i = start ; i<=end ; i++ )
            for ( int j = i+1 ; j<=end ; j++ )
            {
                if (strcmp(name[out[i]->num].word, name[out[j]->num].word) > 0)
                {
                    tile * temp = out[i] ; 
                    out[i] = out[j] ;
                    out[j] = temp ;
                }
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

