#include "gui.h"
#include "io_file.h"
#include "debug_macros.h"

using namespace std ;

static const char* F_GLADE             = "./gui.glade" ;



int main ( int argc, char * argv[] )
{
    D1(cerr<<"D1 main\n")

    gtk_init ( &argc, &argv) ;
    builder = gtk_builder_new () ;
    gtk_builder_add_from_file ( builder, F_GLADE, NULL ) ;
    gtk_builder_connect_signals(builder, NULL);

    create_cube () ;
    import_tiles_names () ;

    display_rules () ;

    gtk_main () ;
    /*
     * non inserire nulla qui in mezzo.
     * questa parte viene eseguita quando si chiude la finestra.
     */

    D10(cerr<<"D10 main\n")
    return 0 ;
}
