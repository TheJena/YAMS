#include <iostream>
#include <cstdlib>
#include <cstring>
#include "data_structure.h"
#include "initializations_operations.h"
#include "gui.h"

using namespace std ;


couple ** mov ;

int row = 0 ;
int col = 0 ;

bool playing = false ;

void start_game ()
{
    if (!playing)
    {
        playing = true ;
        initialize_cube () ;
        fill_cube () ;
        mix_cube () ;
        check_cube () ;
        refresh_unlocked () ;
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
    }
    else
cerr<<"error game is alredy active\n";

}

void undo_last_two_couples ()
{
    if ( row >= 1 )
    {
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
        sort_unlocked () ;

        redraw_widget("playground") ;
    }
}



void check_couple ( )
{

    if ( ( mov[row][col].t1 != -1 )&&( mov[row][col].t2 != -1 ) )
    {
        if ( ( mov[row][col].t1 != mov[row][col].t2 ) &&
             ( ( ( strcmp ( mov[row][col].name1, mov[row][col].name2 ) == 0 ) )
                     || ( ( between (136, mov[row][col].t1, 139 ) )
                          && ( between (136, mov[row][col].t2  , 139 ) ) )
                     || ( ( between (140, mov[row][col].t1, 143 ) )
                          && ( between (140, mov[row][col].t2  , 143 ) ) ) ) )
        {
            reset_cell ( mov[row][col].x1, mov[row][col].y1, mov[row][col].z1 ) ;
            reset_cell ( mov[row][col].x2, mov[row][col].y2, mov[row][col].z2 ) ;

            if ((mode == h_h)&&( col == 0 ))
                refresh_turn_label(true) ;
            if ((mode == h_h)&&( col == 1 ))
                refresh_turn_label(false) ;
            check_cube () ;
            refresh_unlocked () ;
            sort_unlocked () ;
            check_solvability(0) ;
            redraw_widget ( "playground" ) ;

            /*ora tocca all'avversario*/

            opponent_round () ;
        }
        else
        {
            mov[row][col].t1 = -1 ;
            mov[row][col].t2 = -1 ;
            reset_highlighted_cell() ;
        }
    }
}

void opponent_round ()
{
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
        extract_pair ( &mov[row][1] ) ;
cerr<<mov[row][1].name1<<" "<<mov[row][1].t1<<" "<<mov[row][1].name2<<" "<<mov[row][1].t2<<endl;
        reset_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1 ) ;
        reset_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2 ) ;

        dummy = TILES ;
        fill_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1, dummy ) ;
        fill_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2, dummy ) ;



        row++ ;

        if ( row == TILES/4)
        {
                    end_game();
                    start_game();
        }
        else
        {
            refresh_turn_label(false);
            check_cube () ;
            refresh_unlocked () ;
            sort_unlocked () ;
            check_solvability(0) ;
            redraw_widget ( "playground" ) ;
        }
    }
}

void reset_row()
{
    mov[row][col].t1 = -1 ;
    mov[row][col].t2 = -1 ;
}

void insert_half_pair ( const int &num, const int &x, const int &y, const int &z )
{
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

        check_couple ( ) ;
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

        check_couple ( ) ;
    }
}

void end_game ()
{
    if ( playing )
    {
        reset_highlighted_cell() ;
        display_end () ;

        for ( int x = 0 ; x < TILES/4 ; x++ )
            delete [] mov[x] ;
        delete [] mov ;
        row = 0 ;
        col = 0 ;
        playing = false ;
    }
}
