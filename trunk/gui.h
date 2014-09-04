#include <gtk/gtk.h>

using namespace std ;

extern GtkBuilder * builder ;

struct colour {
                double r ;
                double g ;
                double b ;
              } ;

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data ) ;
/*
 * Funzione che ridisegna il widget passato per nome
 */
void redraw_widget ( const char * name ) ;

/*
 * Funzione che ritorna un puntatore a widget
 * dato in input il nome del widget
 */
GtkWidget * widget_from_name ( const char * name ) ;

/*
 * Funzione che ritorna un puntatore a toggle button
 * dato in input il nome del togglebutton
 */
GtkToggleButton * tb_from_name ( const char * name ) ;
