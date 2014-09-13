/*inizio header modulo_GUI*/

#include <gtk/gtk.h>

#define enum_layout
#define enum_p_player
#include "data_structures.h"
#undef enum_layout
#undef enum_p_player

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/


extern layout level ;
extern bool lock_undo ;
extern bool lock_mix ;
extern int last_removed_pl1_a ;
extern int last_removed_pl1_b ;
extern int last_removed_pl2_a ;
extern int last_removed_pl2_b ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
void call_gtk_main() ;

void call_gtk_main_quit() ;

void initialize_gtk ( int argc, char * argv[] ) ;

void refresh_turn_label ( bool _switch) ;

void refresh_scores_labels( const int &score1, const int &score2 ) ;

void refresh_down_label ( const int & couples ) ;

void redraw_widget ( const char * name ) ;

void clear_pair_removed () ;

void refresh_pair_removed( const p_player &temp, const int &a,
                           const int &b ) ;

void display_empty () ;

void display_tiles () ;

void display_rules () ;

void display_end () ;

void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z ) ;

void reset_highlighted_cell () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_GUI*/
