#include <iostream>
#include <gtk/gtk.h>
using namespace std ;
static GtkBuilder * builder ;
int main ( int argc, char * argv[] )
{
/*                                    CONTROLLARE CON FILE HEADER
    gtk_init ( &argc, &argv) ;
    builder = gtk_builder_new () ;
    gtk_builder_add_from_file ( builder, "./gui.glade", NULL ) ;
    gtk_builder_connect_signals(builder, NULL);

    gtk_main () ;                                                       */
    /*
     * non inserire nulla qui in mezzo.
     * questa parte viene eseguita quando si chiude la finestra.
     */
    return 0 ;
}
