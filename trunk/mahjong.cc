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

static GtkBuilder * builder ;

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

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data )
{
    /*
     * widget       -> the object which received the signal
     * event        -> the GdkEventButton which triggered this signal.
     * user_data    -> user data set when the signal handler was connected.
     */
    if      ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "new" )))
    {    cerr<<"new pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "mix" )))
    {    cerr<<"mix pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "undo" )))
    {    cerr<<"undo pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "redo" )))
    {    cerr<<"redo pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "load" )))
    {    cerr<<"load pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "save" )))
    {    cerr<<"save pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "exit" )))
    {    cerr<<"exit pressed"<<endl ; }

    return TRUE ;
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget, CairoContext * cr,
                                       gpointer user_data )
{


    return TRUE ;
}

int main ( int argc, char * argv[] )
{

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
