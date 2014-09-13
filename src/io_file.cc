/**
 * @file
 * File che contiene l'implementazione del modulo IOFILE. Qui vengono definite
 * le funzioni che operano su tale struttura dati.
 */
/*inizio implementazione modulo_IOFILE*/
#include <fstream>
#include <iostream>
#include "debug_macros.h"
#include "io_file.h"                // include gia' la definizione struct_couple
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "cube.h"                       //include gia la definizione struct_tile
#include "game.h"      // include gia' le definizioni struct__string e enum_game
#include "movements.h"                //include gia'le definizioni enum_computer


using namespace std ;

/*inizio implementazione struttura dati pubblica*/
_string * name = NULL ;
/*fine implementazione struttura dati pubblica*/

/*inizio implementazione struttura dati privata*/
static const char* F_TILES = "./tiles_names.dat" ;
static const int MAXWORD = 20 ;
/*fine implementazione struttura dati privata*/

/*inizio prototipi funzioni private*/
/**
 * Funzione privata che controlla la validita' del nome del file binario su cui
 * si vuole salvare la partita.
 * @param[in,out] filename nome eventualmente aggiustato se non conforme a
 * quanto aspettato.
 */
static void check_filename ( char * filename ) ;
/*fine prototipi funzioni private*/

/*inizio implementazione funzioni pubbliche*/
bool save_game_on_file ( const couple** mov, char * filename,
                         const int &row, const int &col )
{
    D1(cerr<<"D1 save game on file\n")

    check_filename( filename ) ;
    remove_dummies() ;

    ofstream out_file( filename ) ;
    if ( !out_file )
    {
        D3(cerr<<"D3 save game on file"<<endl)
        D8(cerr<<"D8 returned false on file opening"<<endl)
        return false ;
    }


    const int intsize = sizeof(int) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&intsize)), intsize ) ;

    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&dim_X)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&dim_Y)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&dim_Z)), intsize ) ;

    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&SEEDVALUE)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&WINDVALUE)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&DRAGONVALUE)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&SEASONVALUE)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&FLOWERVALUE)), intsize ) ;

    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&last_removed_pl1_a)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&last_removed_pl1_b)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&last_removed_pl2_a)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&last_removed_pl2_b)), intsize ) ;

    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&row)), intsize ) ;
    out_file.write( reinterpret_cast<char*>(const_cast<int*>(&col)), intsize ) ;

    out_file.write( reinterpret_cast<char*>(&level),     sizeof(layout) ) ;
    out_file.write( reinterpret_cast<char*>(&ai),        sizeof(computer) ) ;
    out_file.write( reinterpret_cast<char*>(&mode),      sizeof(game) ) ;

    for ( int x = 0 ; x < dim_X ; x++ )
        for ( int y = 0 ; y < dim_Y ; y++ )
            for ( int z = 0 ; z < dim_Z ; z++ )
                    out_file.write( reinterpret_cast<char*>(&cube[x][y][z]),
                                    sizeof(tile) ) ;

    for ( int x = 0 ; x < max_couple_row ; x++ )
        for ( int y = 0 ; y < 2 ; y++ )
            out_file.write(reinterpret_cast<char*>(const_cast<couple*>(&mov[x][y])), sizeof(couple) ) ;

    out_file.close();

    D10(cerr<<"D10 save game on file\n")

    return true ;
}

bool load_game_from_file ( couple** &mov, const char * filename, int &row, int &col )
{
    D1(cerr<<"D1 load game from file\n")

    end_game() ;
    if ( !start_game() )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 start game returned false"<<endl)
        end_game() ;
        return false ;
    }

    ifstream in_file ( filename ) ;
    if ( !in_file )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false on file opening"<<endl)
        return false ;
    }

    const int intsize = sizeof(int) ;
    char buffer[intsize] ;

    /*
     * controllo che la dimensione degli interi sulla macchina
     * sia la stessa usata per scrivere il file binario
     */
    in_file.read( buffer, intsize ) ;
    int * file_intsize = reinterpret_cast<int*>(buffer) ;
    assert( file_intsize != NULL ) ;
    if ( intsize != (*file_intsize))
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different size of int"<<endl)
        return false ;
    }

    /*
     * controllo che le dimensioni dello spazio tridimensionale
     * siano le stesse usate per scrivere il file binario
     */
    in_file.read ( buffer, intsize ) ;
    int * file_dim_X = reinterpret_cast<int*>(buffer) ;
    assert( file_dim_X != NULL ) ;
    if ( dim_X != (*file_dim_X) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different dim_X"<<endl)
        return false ;
    }
    in_file.read ( buffer, intsize ) ;
    int * file_dim_Y = reinterpret_cast<int*>(buffer) ;
    assert( file_dim_Y != NULL ) ;
    if ( dim_Y != (*file_dim_Y) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different dim_Y"<<endl)
        return false ;
    }
    in_file.read ( buffer, intsize ) ;
    int * file_dim_Z = reinterpret_cast<int*>(buffer) ;
    assert( file_dim_Z != NULL ) ;
    if ( dim_Z != (*file_dim_Z) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different dim_Z"<<endl)
        return false ;
    }

    /*
     * controllo che i valori delle tessere
     * siano gli stessi usati per scrivere il file binario
     */
    in_file.read ( buffer, intsize ) ;
    int * file_SEEDVALUE = reinterpret_cast<int*>(buffer) ;
    assert( file_SEEDVALUE != NULL ) ;
    if ( SEEDVALUE != (*file_SEEDVALUE) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different SEEDVALUE"<<endl)
        return false ;
    }
    in_file.read ( buffer, intsize ) ;
    int * file_WINDVALUE = reinterpret_cast<int*>(buffer) ;
    assert( file_WINDVALUE != NULL ) ;
    if ( WINDVALUE != (*file_WINDVALUE) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different WINDVALUE"<<endl)
        return false ;
    }
    in_file.read ( buffer, intsize ) ;
    int * file_DRAGONVALUE = reinterpret_cast<int*>(buffer) ;
    assert( file_DRAGONVALUE != NULL ) ;
    if ( DRAGONVALUE != (*file_DRAGONVALUE) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different DRAGONVALUE"<<endl)
        return false ;
    };
    in_file.read ( buffer, intsize ) ;
    int * file_SEASONVALUE = reinterpret_cast<int*>(buffer) ;
    assert( file_SEASONVALUE != NULL ) ;
    if ( SEASONVALUE != (*file_SEASONVALUE) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different SEASONVALUE"<<endl)
        return false ;
    }
    in_file.read ( buffer, intsize ) ;
    int * file_FLOWERVALUE = reinterpret_cast<int*>(buffer) ;
    assert( file_FLOWERVALUE != NULL ) ;
    if ( FLOWERVALUE != (*file_FLOWERVALUE) )
    {
        D3(cerr<<"D3 load game from file"<<endl)
        D8(cerr<<"D8 returned false because of different FLOWERVALUE"<<endl)
        return false ;
    }

    /*superati i controlli soprastanti si puo' procedere col caricamento*/
    in_file.read ( buffer, intsize ) ;
    int * file_last_removed_pl1_a = reinterpret_cast<int*>(buffer) ;
    assert( file_last_removed_pl1_a != NULL ) ;
    last_removed_pl1_a = *(file_last_removed_pl1_a) ;
    in_file.read ( buffer, intsize ) ;
    int * file_last_removed_pl1_b = reinterpret_cast<int*>(buffer) ;
    assert( file_last_removed_pl1_b != NULL ) ;
    last_removed_pl1_b = *(file_last_removed_pl1_b) ;
    in_file.read ( buffer, intsize ) ;
    int * file_last_removed_pl2_a = reinterpret_cast<int*>(buffer) ;
    assert( file_last_removed_pl2_a != NULL ) ;
    last_removed_pl2_a = *(file_last_removed_pl2_a) ;
    in_file.read ( buffer, intsize ) ;
    int * file_last_removed_pl2_b = reinterpret_cast<int*>(buffer) ;
    assert( file_last_removed_pl2_b != NULL ) ;
    last_removed_pl2_b = *(file_last_removed_pl2_b) ;

    in_file.read ( buffer, intsize ) ;
    int * file_row = reinterpret_cast<int*>(buffer) ;
    assert( file_row != NULL ) ;
    row = *(file_row) ;
    in_file.read ( buffer, intsize ) ;
    int * file_col = reinterpret_cast<int*>(buffer) ;
    assert( file_col != NULL ) ;
    col = *(file_col) ;

    char layout_buffer[sizeof(layout)] ;
    in_file.read ( layout_buffer, sizeof(layout)) ;
    layout * file_level = reinterpret_cast<layout*>(layout_buffer) ;
    assert( file_level != NULL ) ;
    level = *(file_level) ;
    char computer_buffer[sizeof(computer)] ;
    in_file.read ( computer_buffer, sizeof(computer)) ;
    computer * file_ai = reinterpret_cast<computer*>(computer_buffer) ;
    assert( file_ai != NULL ) ;
    ai = *(file_ai) ;
    char game_buffer[sizeof(game)] ;
    in_file.read ( game_buffer, sizeof(game)) ;
    game * file_mode = reinterpret_cast<game*>(game_buffer) ;
    assert( file_mode != NULL ) ;
    mode = *(file_mode) ;

    char tile_buffer[sizeof(tile)] ;
    tile * file_tile = NULL ;
    for ( int x = 0 ; x < dim_X ; x++ )
        for ( int y = 0 ; y < dim_Y ; y++ )
            for ( int z = 0 ; z < dim_Z ; z++ )
            {
                in_file.read( tile_buffer, sizeof(tile)) ;
                file_tile = reinterpret_cast<tile*>(tile_buffer) ;
                assert( file_tile != NULL ) ;
                cube[x][y][z] = *(file_tile) ;
            }
    check_cube() ;
    refresh_unlocked() ;
    sort_unlocked() ;

    char couple_buffer[sizeof(couple)] ;
    couple * file_couple = NULL ;
    for ( int x = 0 ; x < max_couple_row ; x++ )
        for ( int y = 0 ; y < 2 ; y++ )
        {
            in_file.read( couple_buffer, sizeof(couple)) ;
            file_couple = reinterpret_cast<couple*>(couple_buffer) ;
            assert( file_couple != NULL ) ;
            mov[x][y] = *(file_couple) ;
        }
    reset_highlighted_cell() ;
    display_tiles() ;
    refresh_scores() ;
    if ( col == 1 )
        refresh_turn_label( true ) ;
    else if ( col == 0 )
        refresh_turn_label( false ) ;


    in_file.close() ;

    D10(cerr<<"D10 load game from file\n")

    return true ;
}

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
/*fine implementazione funzioni pubbliche*/

/*inizio implementazione funzioni private*/
static void check_filename ( char * filename )
{
    D1(cerr<<"D1 check filename\n")
    int x ;
    for ( x = 0 ; x<MAXWORD ; x++ )
    {
        if (( filename[x] == '.' ) || ( filename[x] == '\0' ) )
            break ;
    }
    if ( x < MAXWORD-5 )
    {
        filename[x]   = '.' ;
        filename[x+1] = 'd' ;
        filename[x+2] = 'a' ;
        filename[x+3] = 't' ;
        filename[x+4] = '\0' ;
    }
    else
    {
        filename[MAXWORD-5] = '.' ;
        filename[MAXWORD-4] = 'd' ;
        filename[MAXWORD-3] = 'a' ;
        filename[MAXWORD-2] = 't' ;
        filename[MAXWORD-1] = '\0' ;
    }
    D10(cerr<<"D10 check filename\n")
}
/*fine implementazione funzioni private*/

/*fine implementazione modulo_IOFILE*/
