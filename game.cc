/*inizio implementazione modulo_GAME*/
#include <iostream>
#include <cstring>
#include "debug_macros.h"
#include "game.h"      // include gia' le definizioni struct__string e enum_game
#include "io_file.h"                // include gia' la definizione struct_couple
#include "cube.h"                       //include gia la definizione struct_tile
#include "movements.h"                //include gia'le definizioni enum_computer
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player

using namespace std ;

/*inizio implementazione struttura dati pubblica*/
game mode = h_c ;
const int max_couple_row = TILES/4 ;
bool playing = false ;
/*fine implementazione struttura dati pubblica*/

/*inizio implementazione struttura dati privata*/
static int _score1 = 0 ;
static int _score2 = 0 ;
static int row = 0 ;
static int col = 0 ;
static couple ** mov ;
/*fine implementazione struttura dati privata*/

/*inizio prototipi funzioni private*/
static bool check_couple ( ) ;

static bool opponent_round () ;
/*fine prototipi funzioni private*/


/*inizio implementazione funzioni pubbliche*/
bool start_game ()
{
    D1(cerr<<"D1 start game\n")

    if (!playing)
    {
        playing = true ;
        initialize_cube () ;
        fill_cube () ;
        if ( !mix_cube () )
        {
            D3(cerr<<"start game\n")
            D7(cerr<<"mix cube returned false\n")
            return false ;
        }
        else
        {
            check_cube () ;
            refresh_unlocked () ;
            /*
             * qui non controllo il valore di ritorno di sort unlocked
             * perche' count pairs removable non puo ritornare -1 (dato
             * che la partita deve ancora iniziare) e forzare la sort
             * unlocked a ritornare false.
             */
            sort_unlocked () ;

            row = 0 ;
            col = 0 ;
            mov = new couple *[max_couple_row] ;
            for ( int x = 0 ; x < max_couple_row ; x++ )
                mov[x] = new couple[2] ;

            for ( int r = 0 ; r < max_couple_row ; r++ )
                for ( int c = 0 ; c < 2 ; c++ )
                {
                    mov[r][c].t1 = -1 ;
                    mov[r][c].t2 = -1 ;
                    mov[r][c].name1 = NULL ;
                    mov[r][c].name2 = NULL ;
                }

            reset_highlighted_cell() ;
            display_tiles() ;
            refresh_turn_label(false);
            refresh_scores() ;

            D10(cerr<<"D10 start game\n")

            return true ;
        }
    }
    else
    {
        D10(cerr<<"D10 start game returned false\n")

        return false ;
    }
}

bool save_game(char * filename )
{
    D1(cerr<<"D1 save game\n")
    D10(cerr<<"D10 save game\n")
    return save_game_on_file ( const_cast<const couple**>(mov), filename, row, col ) ;
}

bool load_game( char * filename )
{
    D1(cerr<<"D1 load game\n")
    D10(cerr<<"D10 load game\n")
    return load_game_from_file ( mov, filename, row, col ) ;
}


void undo_last_two_couples ()
{
    D1(cerr<<"D1 undo last two couples\n")

    if (( row >= 1 )&&(!lock_undo))
    {
        refresh_scores() ;
        row-- ;
        if ( mov[row][0].t1 != -1 )
            fill_cell(mov[row][0].x1, mov[row][0].y1, mov[row][0].z1, mov[row][0].t1);
        if ( mov[row][0].t2 != -1 )
            fill_cell(mov[row][0].x2, mov[row][0].y2, mov[row][0].z2, mov[row][0].t2);
        if ( mov[row][1].t1 != -1 )
            fill_cell(mov[row][1].x1, mov[row][1].y1, mov[row][1].z1, mov[row][1].t1);
        if ( mov[row][1].t2 != -1 )
            fill_cell(mov[row][1].x2, mov[row][1].y2, mov[row][1].z2, mov[row][1].t2);

        mov[row][0].t1 = -1 ;
        mov[row][0].name1 = NULL ;
        mov[row][0].t2 = -1 ;
        mov[row][0].name2 = NULL ;
        mov[row][1].t1 = -1 ;
        mov[row][1].name1 = NULL ;
        mov[row][1].t2 = -1 ;
        mov[row][1].name2 = NULL ;

        reset_highlighted_cell() ;

        col = 0 ;

        check_cube () ;
        refresh_unlocked () ;
        /*
         * non controllo il valore di ritorno della sort unlocked perche'
         * se ho appena inserito 4 tessere nel cubo, la count pairs removable,
         * non puo' ritornare -1 e forzare la sort unlocked a ritornare false,
         * quindi non bisogna controllare la fine del gioco in questo punto.
         */
        sort_unlocked () ;

        redraw_widget("playground") ;
    }

    D10(cerr<<"D10 undo last two couples\n")
}

void refresh_scores()
{
    D2(cerr<<"D2 refresh scores\n")

    _score1 = 0 ;
    _score2 = 0 ;
    for ( int c = 0 ; c < 2 ; c++ )
        for (int r = 0 ; r < max_couple_row ; r++ )
        {
            if ( ( c == 0 )&&( mov[r][c].t1 != -1 ) )
                _score1 += tile_value(mov[r][c].t1) ;

            if ( ( c == 1 )&&( mov[r][c].t1 != -1 ) )
                _score2 += tile_value(mov[r][c].t1) ;
        }

    refresh_scores_labels( _score1, _score2 ) ;

    D9(cerr<<"D9 refresh scores\n")
}

void reset_row()
{
    D1(cerr<<"D1 reset row\n")

    mov[row][col].t1 = -1 ;
    mov[row][col].t2 = -1 ;
    mov[row][col].name1 = NULL ;
    mov[row][col].name2 = NULL ;

    D10(cerr<<"D10 reset row\n")
}

bool insert_half_pair ( const int &num, const int &x, const int &y, const int &z )
{
    D1(cerr<<"D1 insert half pair\n")

/*debug se row troppo grande*/

    if ( mode == h_c )
        col = 0 ;

    if ( mov[row][col].t1 == -1)
    {
        mov[row][col].t1 = num ;
        mov[row][col].name1 = name[num].word ;
        mov[row][col].x1 = x ;
        mov[row][col].y1 = y ;
        mov[row][col].z1 = z ;

        set_highlighted_cell ( 1, x, y, z ) ;
        redraw_widget ( "playground" ) ;

        if ( !check_couple ( ) )
        {
            D3(cerr<<"insert half pair\n")
            D8(cerr<<"check couple returned false1")
            return false ;
        }
    }
    else if ( mov[row][col].t2 == -1 )
    {
        mov[row][col].t2 = num ;
        mov[row][col].name2 = name[num].word ;
        mov[row][col].x2 = x ;
        mov[row][col].y2 = y ;
        mov[row][col].z2 = z ;

        set_highlighted_cell ( 2, x, y, z ) ;
        redraw_widget ( "playground" ) ;

        if ( !check_couple ( ) )
        {
            D3(cerr<<"insert half pair\n")
            D8(cerr<<"check couple returned false2")
            return false ;
        }
    }

    D10(cerr<<"D10 insert half pair\n")

    return true ;
}

void end_game ()
{
    D1(cerr<<"D1 end game\n")

    if ( playing )
    {
        reset_highlighted_cell() ;
        clear_pair_removed () ;
        display_end () ;


        for ( int x = 0 ; x < max_couple_row ; x++ )
            delete [] mov[x] ;
        delete [] mov ;
        row = 0 ;
        col = 0 ;
        playing = false ;
        refresh_down_label(0) ;
    }

    D10(cerr<<"D10 end game\n")
}
/*fine implementazione funzioni pubbliche*/

/*inizio implementazione funzioni private*/
static bool check_couple ( )
{
    D2(cerr<<"D2 check couple\n")

    {
        if ( ( mov[row][col].t1 != mov[row][col].t2 )&&(mov[row][col].name1 != NULL)&&(mov[row][col].name2 != NULL)&&
             ( ( ( strcmp ( mov[row][col].name1, mov[row][col].name2 ) == 0 ) )
                     || ( ( between (136, mov[row][col].t1, 139 ) )
                          && ( between (136, mov[row][col].t2  , 139 ) ) )
                     || ( ( between (140, mov[row][col].t1, 143 ) )
                          && ( between (140, mov[row][col].t2  , 143 ) ) ) ) )
        {
            if ( ai != thoughtful )
            {
                lock_undo = false ;
                if ( ai == airhead )
                    lock_mix = false ;
            }

            refresh_scores() ;

            reset_cell ( mov[row][col].x1, mov[row][col].y1, mov[row][col].z1 ) ;
            reset_cell ( mov[row][col].x2, mov[row][col].y2, mov[row][col].z2 ) ;

            if ((mode == h_h)&&( col == 0 ))
            {
                refresh_turn_label(true) ;
                refresh_pair_removed( p_human1, mov[row][col].t1, mov[row][col].t2 ) ;
            }
            if ((mode == h_h)&&( col == 1 ))
            {
                refresh_turn_label(false) ;
                refresh_pair_removed( p_human2, mov[row][col].t1, mov[row][col].t2 ) ;
            }
            if ( mode == h_c )
                refresh_pair_removed( p_human1, mov[row][col].t1, mov[row][col].t2 ) ;
            check_cube () ;
            refresh_unlocked () ;
            if ( !sort_unlocked () )
            {
                D3(cerr<<"check couple\n")
                D6(cerr<<"sort unlocked returned false\n")
                return false ;
            }
            if ( count_pairs_removable(3) == -1 )
            {
                D3(cerr<<"check couple\n")
                D8(cerr<<"count pairs removable returned -1\n")
                return false ;
            }
            redraw_widget ( "playground" ) ;

            /*ora tocca all'avversario*/

            if ( !opponent_round () )
            {
                D3(cerr<<"check couple\n")
                D8(cerr<<"opponent round returned false\n")
                return false ;
            }
        }
        else
        {
            if ((mov[row][col].name1 != NULL)&&(mov[row][col].name2 != NULL))
            {
               mov[row][col].t1 = -1 ;
               mov[row][col].name1 = NULL ;
               mov[row][col].t2 = -1 ;
               mov[row][col].name2 = NULL ;
               reset_highlighted_cell() ;
            }
        }

    return true ;
    }

    D9(cerr<<"D9 check couple\n")

    return true ;
}

static bool opponent_round ()
{
    D1(cerr<<"D1 opponent round\n")

    static int dummy = TILES ;
    if ( mode == h_h )
    {
        col = (col+1)%2 ;
        if ( col == 0 )
            row++ ;

/*debug se row troppo grande*/
    }
    else if ( mode == h_c )
    {
        refresh_turn_label(true) ;
        if ( extract_pair ( &mov[row][1] ) )
        {
            refresh_pair_removed( p_ai, mov[row][1].t1, mov[row][1].t2 ) ;

            refresh_scores() ;

            reset_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1 ) ;
            reset_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2 ) ;

            dummy = TILES ;
            fill_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1, dummy ) ;
            fill_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2, dummy ) ;

            row++ ;

            if ( row == max_couple_row)
            {
                        end_game();
                            
            }
            else
            {
                refresh_turn_label(false);
                check_cube () ;
                refresh_unlocked () ;
                if ( !sort_unlocked () )
                {
                    D3(cerr<<"opponent round\n")
                    D6(cerr<<"sort unlocked returned false\n")
                    return false ;
                }
                if ( count_pairs_removable (3) == -1 )
                {
                    D3(cerr<<"opponent round\n")
                    D8(cerr<<"count pairs removable returned -1\n")
                    return false ;
                }
                redraw_widget ( "playground" ) ;
            }
        }
        else
        {
            end_game() ;
            D3(cerr<<"opponent round\n")
            D8(cerr<<"returned false\n")
            return false ;
        }
    }

    D10(cerr<<"D10 opponent round\n")

    return true ;
}
/*fine implementazione funzioni private*/

/*fine implementazione modulo_GAME*/
