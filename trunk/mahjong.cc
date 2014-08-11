#include <iostream>
#include <gtk/gtk.h>

using namespace std ;

struct tile {
            bool    empty ;
            bool    lock ;
            char *  name ;
            } ;

const int dim_X = 12 ;
const int dim_Y = 8 ;
const int dim_Z = 4 ;
/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube ( tile *** cube )
{
    cube = new tile ** [dim_X] ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        cube[x] = new tile * [dim_Y] ;

        for ( int y = 0 ; y < dim_Y ; y++ )
            cube[x][y] = new tile [dim_Z] ;
    }
}

void initialize_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                cube[x][y][z].empty = true ;
                cube[x][y][z].lock  = true ;
                cube[x][y][z].name  = NULL ;
            }
}

void delete_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_X ; x++ )
    {
        for ( int y = 0 ; y < dim_Y ; y++ )
        {
            for ( int z = 0 ; z < dim_Z ; z++ )
            {
                if ( cube[x][y][z].name != NULL )
                    delete [] cube[x][y][z].name ;

                delete [] cube[x][y] ;
            }
        }

        delete [] cube[x] ;
    }

    delete [] cube ;
}

extern "C" gboolean handler_delete_event ( GtkWidget * widget, GdkEvent * event,
                                         gpointer user_data)  
{
    gtk_main_quit() ;
    cout<<"programma terminato regolarmente\n" ;
    return TRUE ;
}

int main ( int argc, char * argv[] )
{
    GtkBuilder * builder ;

    gtk_init ( &argc, &argv) ;

    builder = gtk_builder_new () ;

    gtk_builder_add_from_file ( builder, "./gui.glade", NULL ) ;

    gtk_builder_connect_signals(builder, NULL);

    gtk_main () ;
    /*
     * non inserire nulla qui in mezzo.
     * questa parte viene eseguita quando si chiude la finestra.
     */
    return 0 ;
}
















