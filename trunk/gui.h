#include <gtk/gtk.h>
#include "initializations_operations.h"

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

extern int last_removed_pl1_a ;
extern int last_removed_pl1_b ;
extern int last_removed_pl2_a ;
extern int last_removed_pl2_b ;

void display_end () ;

void display_empty () ;

void display_tiles () ;

void display_rules () ;

void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z ) ;

void reset_highlighted_cell () ;

bool check_position ( const tile * punt, const int &x, const int &y ) ;

extern "C" gboolean handler_click_on_widget (GtkWidget *widget,
               GdkEventButton  *event,
               gpointer   user_data) ;

extern "C" gboolean handler_delete_event ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data) ;

extern "C" gboolean handler_hide_window ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data) ;

extern "C" gboolean handler_set_new_game ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data) ;

void refresh_turn_label ( bool _switch) ;

void refresh_scores_labels( const int &score1, const int &score2 ) ;

void refresh_down_label ( const int & couples ) ;

/*
 * Funzione che ritorna un puntatore a label
 * dato in input il nome della label
 */
GtkLabel * label_from_name ( const char * name ) ;

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

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data ) ;

/*
 * Funzione che ridisegna il widget passato per nome
 */
void redraw_widget ( const char * name ) ;

void draw_number_on_tile ( cairo_t * &cr_tile,
                           cairo_surface_t * &_obj,
                           const int &num ) ;

cairo_surface_t * number_on_tile (  cairo_surface_t * _obj,
                                    const int &number ) ;

int number_from_string ( const char * word ) ;

cairo_surface_t * paint_tile (  const int &num,
                                const int &x,
                                const int &y,
                                const int &z ) ;

void calculate_coor_x_y ( const int &x,
                          const int &y,
                          const int &z,
                          int &_dx,
                          int &_dy ) ;

extern "C" gboolean draw_removed_tiles  ( GtkWidget * widget,
                                          cairo_t * cr,
                                          gpointer user_data ) ;

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data ) ;

void set_coor_tile ( tile * punt, const int &x, const int &y ) ;
