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


void start_game ()
{
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
}

void check_couple ( int &row )
{

    if ( ( strcmp ( mov[row][col].name1, mov[row][col].name2 ) == 0 )
         && ( mov[row][col].t1 != mov[row][col].t2 ) )
    {
        reset_cell ( mov[row][col].x1, mov[row][col].y1, mov[row][col].z1 ) ;
        reset_cell ( mov[row][col].x2, mov[row][col].y2, mov[row][col].z2 ) ;
        check_cube () ;
        check_solvability(0) ;

        /*ora tocca all'avversario*/
        opponent_round () ;
    }
    else
    {
        mov[row][col].t1 = -1 ;
        mov[row][col].t2 = -1 ;
    }

}

void opponent_round ()
{
    if ( mode == h_h )
    {
        col = (col+1)%2 ;
        if ( col == 0 )
            row++ ;
/*debug se row troppo grande*/
    }
    else if ( mode == h_c )
    {
        extract_pair ( &mov[row][1] ) ;
cerr<<mov[row][1].name1<<" "<<mov[row][1].t1<<" "<<mov[row][1].name2<<" "<<mov[row][1].t2<<endl;
        reset_cell ( mov[row][1].x1, mov[row][1].y1, mov[row][1].z1 ) ;
        reset_cell ( mov[row][1].x2, mov[row][1].y2, mov[row][1].z2 ) ;

        row++ ;
        check_cube () ;
        check_solvability(0) ;
        redraw_widget ( "playground" ) ;
    }
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
    }
    else if ( mov[row][col].t2 == -1 )
    {
        mov[row][col].t2 = num ;
        mov[row][col].name2 = name[num].word ;
        mov[row][col].x2 = x ;
        mov[row][col].y2 = y ;
        mov[row][col].z2 = z ;

        check_couple ( row ) ;
    }
}

void end_game ()
{
    for ( int x = 0 ; x < TILES/4 ; x++ )
        delete [] mov[x] ;
    delete [] mov ;
}
