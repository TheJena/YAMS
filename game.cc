#include <iostream>
#include <cstdlib>
#include <cstring>
#include "gui.h"
#include "game.h"
#include "debug_macros.h"

using namespace std ;


couple ** mov ;

int row = 0 ;
int col = 0 ;

bool playing = false ;

int _score1 = 0 ;
int _score2 = 0 ;

bool lock_undo = false ;

bool start_game ()
{
    D1(cerr<<"D1 start game\n")

    if (!playing)
    {
        playing = true ;
        initialize_cube () ;
        fill_cube () ;
        if ( !mix_cube () )
            return false ;
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
            mov = new couple *[TILES/4] ;
            for ( int x = 0 ; x < TILES/4 ; x++ )
                mov[x] = new couple[2] ;

            for ( int r = 0 ; r < TILES/4 ; r++ )
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
            refresh_scores(_score1, _score2) ;
            return true ;
        }
    }
    else
cerr<<"error game is alredy active\n";
    return false ;


    D10(cerr<<"D10 start game\n")
}

void undo_last_two_couples ()
{
    D1(cerr<<"D1 undo last two couples\n")

    if (( row >= 1 )&&(!lock_undo))
    {
        refresh_scores(_score1, _score2) ;
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
        mov[row][0].t2 = -1 ;
        mov[row][1].t1 = -1 ;
        mov[row][1].t2 = -1 ;

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

void refresh_scores( int &score1, int &score2)
{
    D2(cerr<<"D2 refresh scores\n")

    score1 = 0 ;
    score2 = 0 ;
    for ( int c = 0 ; c < 2 ; c++ )
        for (int r = 0 ; r < TILES/4 ; r++ )
        {
            if ( ( c == 0 )&&( mov[r][c].t1 != -1 ) )
                score1 += tile_value(mov[r][c].t1) ;

            if ( ( c == 1 )&&( mov[r][c].t1 != -1 ) )
                score2 += tile_value(mov[r][c].t1) ;
        }

    refresh_scores_labels( score1, score2 ) ;

    D9(cerr<<"D9 refresh scores\n")
}



bool check_couple ( )
{
    D2(cerr<<"D2 check couple\n")

    if ( ( mov[row][col].t1 != -1 )&&( mov[row][col].t2 != -1 ) )
    {
        if ( ( mov[row][col].t1 != mov[row][col].t2 ) &&
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
            if ( ai )
            refresh_scores(_score1, _score2) ;

            reset_cell ( mov[row][col].x1, mov[row][col].y1, mov[row][col].z1 ) ;
            reset_cell ( mov[row][col].x2, mov[row][col].y2, mov[row][col].z2 ) ;

            if ((mode == h_h)&&( col == 0 ))
            {
                refresh_turn_label(true) ;
                refresh_pair_removed( p_human1, mov[row][1].t1, mov[row][1].t2 ) ;
            }
            if ((mode == h_h)&&( col == 1 ))
            {
                refresh_turn_label(false) ;
                refresh_pair_removed( p_human2, mov[row][1].t1, mov[row][1].t2 ) ;
            }
            if ( mode == h_c )
                refresh_pair_removed( p_human1, mov[row][col].t1, mov[row][col].t2 ) ;
            check_cube () ;
            refresh_unlocked () ;
            if ( !sort_unlocked () )
                return false ;
            if ( count_pairs_removable(3) == -1 )
                return false ;
            redraw_widget ( "playground" ) ;

            /*ora tocca all'avversario*/

            if ( !opponent_round () )
                return false ;
        }
        else
        {
            mov[row][col].t1 = -1 ;
            mov[row][col].t2 = -1 ;
            reset_highlighted_cell() ;
        }
    }
    return true ;

    D9(cerr<<"D9 check couple\n")
}

void clear_pair_removed ()
{
    D2(cerr<<"D2 clear pair removed\n")

    refresh_pair_removed( p_human1, -1, -1 ) ;
    refresh_pair_removed( p_ai, -1, -1 ) ;

    D9(cerr<<"D9 clear pair removed\n")
}

void refresh_pair_removed( const p_player &temp, const int &a, const int &b )
{
    D2(cerr<<"D2 refresh pair removed\n")

    if ( ( temp == p_ai ) || ( temp == p_human2 ) )
    {
        last_removed_pl2_a = a ;        // removed by ai or player 2
        last_removed_pl2_b = b ;        // removed by ai or player 2
    }
    else if ( temp == p_human1 )
    {
        last_removed_pl1_a = a ;        // removed by player 1
        last_removed_pl1_b = b ;        // removed by player 1
    }

    redraw_widget("drawingarea_right") ;

    D9(cerr<<"D9 refresh pair removed\n")
}

bool opponent_round ()
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

            refresh_scores(_score1, _score2) ;

            reset_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1 ) ;
            reset_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2 ) ;

            dummy = TILES ;
            fill_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1, dummy ) ;
            fill_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2, dummy ) ;

            row++ ;

            if ( row == TILES/4)
            {
                        end_game();
                            
            }
            else
            {
                refresh_turn_label(false);
                check_cube () ;
                refresh_unlocked () ;
                if ( sort_unlocked () )
                    return false ;
                if ( count_pairs_removable (0) == -1 )
                    return false ;
                redraw_widget ( "playground" ) ;
            }
        }
        else
        {
            end_game() ;
            return false ;
        }
    }
    return true ;

    D10(cerr<<"D10 opponent round\n")
}

void reset_row()
{
    D1(cerr<<"D1 reset row\n")

    mov[row][col].t1 = -1 ;
    mov[row][col].t2 = -1 ;

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
            return false ;
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
            return false ;
    }
    return true ;

    D10(cerr<<"D10 insert half pair\n")
}

void end_game ()
{
    D1(cerr<<"D1 end game\n")

    if ( playing )
    {
        reset_highlighted_cell() ;
        clear_pair_removed () ;
        display_end () ;

        for ( int x = 0 ; x < TILES/4 ; x++ )
            delete [] mov[x] ;
        delete [] mov ;
        row = 0 ;
        col = 0 ;
        playing = false ;
    }

    D10(cerr<<"D10 end game\n")
}
