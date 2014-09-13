/**
 * @file
 * File che contiene l'implementazione del modulo MOVEMENTS. Qui vengono
 * definite le funzioni che operano sulla struttura dati unlocked.
 */
/*inizio implementazione modulo_MOVEMENTS*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

/**
 * Struttura che identifica una coppia di tessere candidate alla rimozione o al
 * salvataggio o alla verifica.
 */
#define struct_couple           // perche' couple serve a movements.h che pero'
#include "data_structures.h"    // non la puo' contenere a causa di conflitti in
#undef struct_couple            // altri file in cui e' incluso ma couple e' gia
                                // dichiarata da altri header file

#include "debug_macros.h"
#include "cube.h"                       //include gia la definizione struct_tile
#include "movements.h"                //include gia'le definizioni enum_computer
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "game.h"      // include gia' le definizioni struct__string e enum_game

using namespace std ;

/*inizio implementazione struttura dati pubblica*/
const int   SEEDVALUE   = 1 ;
const int   WINDVALUE   = 3 ;
const int   DRAGONVALUE = 5 ;
const int   SEASONVALUE = 12 ;
const int   FLOWERVALUE = 14 ;

computer ai = airhead ;
/*fine implementazione struttura dati pubblica*/

/*inizio implementazione struttura dati privata*/
static const int FREE  = 12 * 8 ; // dim_X * dim_Y
static tile *    unlocked[FREE] ;
/*inizio implementazione struttura dati privata*/

/*inizio prototipi funzioni private*/
/**
 * Funzione privata che controlla se una coppia e' valida per essere rimossa,
 * o meglio salvata o meno.
 * @param[in] a, b puntatori alle tessere della coppia in input.
 * @param[out] first, second puntatori alle tessere a e b se la coppia e' valida
 * @return true se la coppia e valida o false altrimenti.
 */
static bool check_pair ( tile* a, tile* b, tile* &first, tile* &second ) ;

/**
 * Funzione privata che ordina in ordine alfabetico un sotto array passatole.
 * @param[in] out puntatore all'array
 * @param[in] start, end indici che delimitano il sotto array da ordinare.
 */
static void sort_sub_array ( tile ** out, int start, int end ) ;
/*fine prototipi funzioni private*/

/*inizio implementazione funzioni pubbliche*/
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
                        assert(unlocked[u]!=NULL) ;
                        u++ ;
                    }
/*debug se u > FREE*/

    D10(cerr<<"D10 refresh unlocked\n")
}

bool airhead_extraction ( tile * &first, tile * &second, bool &exit )
{
    D2(cerr<<"D2 airhead extraction\n")

    srand ( time(0) ) ;

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

        D9(cerr<<"D9 airhead extraction returned true\n")

        return true ;
    }
    else
    {
        end_game() ;

        D9(cerr<<"D9 airhead extraction returned false\n")

        return false ;
    }
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
                            {
                                D3(cerr<<"D3 extract pair"<<endl)
                                D8(cerr<<"D8 airhead extraction returned false1"<<endl)
                                return false ;
                            }
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
                                {
                                    D3(cerr<<"D3 extract pair"<<endl)
                                    D8(cerr<<"D8 airhead extraction returned false2"<<endl)
                                    return false ;
                                }
                            }
                            break ;
    } ;

    assert(pair!=NULL) ;
    assert(first!=NULL) ;
    assert(second!=NULL) ;

    pair->t1    = first->num ;
    pair->name1 = name[ first->num ].word ;
    find_coord ( first->num, pair->x1, pair->y1, pair->z1 ) ;

    pair->t2    = second->num ;
    pair->name2 = name[ second->num ].word ;
    find_coord ( second->num, pair->x2, pair->y2, pair->z2 ) ;

    set_highlighted_cell ( 1, pair->x1, pair->y1, pair->z1 ) ;
    set_highlighted_cell ( 2, pair->x2, pair->y2, pair->z2 ) ;

    redraw_widget("playground") ;

    D9(cerr<<"D9 extract pair\n")

    return true ;
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
        assert(unlocked[i]!=NULL) ;
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

        assert(unlocked[j]!=NULL) ;
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

    D9(cerr<<"D9 sort unlocked\n")

    if ( count_pairs_removable(0) == -1 )
    {
        D3(cerr<<"D3 sort unlocked"<<endl)
        D6(cerr<<"D6 returned false because"<<
                 " count pairs removable returned -1"<<endl)
        return false ;
    }
    else
        return true ;
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
            assert(unlocked[i-1]!= NULL) ;
            assert(unlocked[i]!= NULL) ;
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

        D9(cerr<<"D9 count pairs removable returned 0"<<
                 " because of negative couples\n")

        return 0 ;
        end_game() ;
    }
    else if ( couples == 0 )
    {
        if ( count == 0 )
        {
            end_game () ;

            D9(cerr<<"D9 count pairs removable returned -1\n")

            return -1 ;
        }
        else
        {
            mix_cube() ;
            check_cube() ;
            refresh_unlocked() ;
            sort_unlocked() ;

            D9(cerr<<"D9 count pairs removable call a recursion\n")

            return count_pairs_removable ( count-1 ) ;
        }
    }
    else
    {
        refresh_down_label ( couples ) ;

        D9(cerr<<"D9 count pairs removable returned "<<couples<<"\n")

        return couples ;
    }
}

bool between ( const int &min, const int &middle, const int &max )
{
    D2(cerr<<"D2 between\n")

    D9(cerr<<"D9 between\n")

    if ( min > max )
        return between( max, middle, min ) ;

    if ( ( min <= middle )&&( middle <= max ) )
        return true ;
    else
        return false ;
}

int tile_value (const int &name_position)
{
    D2(cerr<<"D2 tile value\n")

/*debug se name_position e' una posizione dell'array*/

    D9(cerr<<"D9 tile value\n")

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
}
/*fine implementazione funzioni pubbliche*/

/*inizio implementazione funzioni private*/
static bool check_pair ( tile* a, tile* b, tile* &first, tile* &second )
{
    D2(cerr<<"D2 check pair\n")

    assert(a!=NULL);
    assert(b!=NULL);

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
        D9(cerr<<"D9 check pair\n")

        return false ;
    }

    D9(cerr<<"D9 check pair\n")

    return false ;
}
static void sort_sub_array ( tile ** out, int start, int end )
{
    D2(cerr<<"D2 sort sub array\n")

    tile * temp = NULL ;

    for ( int i = start ; i<=end ; i++ )
        for ( int j = i+1 ; j<=end ; j++ )
        {
            assert(out[i]!=NULL) ;
            assert(out[j]!=NULL) ;

            if ( (out[i]!=NULL)&&(out[j]!=NULL)&&
                 (strcmp(name[out[i]->num].word, name[out[j]->num].word) > 0) )
            {
                temp = out[i] ; 
                out[i] = out[j] ;
                out[j] = temp ;
            }
        }

    D9(cerr<<"D9 sort sub array\n")
}
/*fine implementazione funzioni private*/

/*fine implementazione modulo_MOVEMENTS*/
