#include <gtk/gtk.h>

using namespace std ;

extern GtkBuilder * builder ;

struct colour {
                double r ;
                double g ;
                double b ;
              } ;

extern int h_x1 ;
extern int h_y1 ;
extern int h_z1 ;

extern int h_x2 ;
extern int h_y2 ;
extern int h_z2 ;

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

void set_coor_tile ( tile * punt, const int &x, const int &y ) ;

void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z ) ;

void reset_highlighted_cell () ;
