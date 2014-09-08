#include <iostream>
#include <gtk/gtk.h>
#include "data_structure.h"
#include "initializations_operations.h"
#include "gui.h"
#include "io_file.h"

using namespace std ;

static const char* F_GLADE             = "./gui.glade" ;



int main ( int argc, char * argv[] )
{
    gtk_init ( &argc, &argv) ;
    builder = gtk_builder_new () ;
    gtk_builder_add_from_file ( builder, F_GLADE, NULL ) ;
    gtk_builder_connect_signals(builder, NULL);

    create_cube () ;
    initialize_cube () ;

    import_tiles_names () ;
    fill_cube () ; /* nota: chiama gia la mix_cube()*/
    check_cube () ;
    unlocked[0] = NULL ;
    refresh_unlocked () ; /* nota: chiama gia la sort_unlocked()*/
    start_game () ;

    gtk_main () ;
    /*
     * non inserire nulla qui in mezzo.
     * questa parte viene eseguita quando si chiude la finestra.
     */
    return 0 ;
}
