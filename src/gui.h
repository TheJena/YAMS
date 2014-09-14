/**
 * @file
 * File che contiene l'interfaccia del modulo GUI. Qui vengono dichiarate le
 * funzioni che operano sulla parte grafica dell' applicazione.
 */
/*inizio header modulo_GUI*/

#include <gtk/gtk.h>

/**
 * Enumerato che identifica la disposizione delle tessere.
 */
#define enum_layout
/**
 * Enumerato che identifica il tipo di giocatore: umano principale, umano
 * secondario ed intelligenza artificiale.
 */
#define enum_p_player
#include "data_structures.h"
#undef enum_layout
#undef enum_p_player

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/

/**
 * Variabile che identifica la disposizione delle tessere scelta.
 */
extern layout level ;

/**
 * Variabile che identifica se il tasto undo e' utilizzabile o meno.
 */
extern bool lock_undo ;

/**
 * Variabile che identifica se il tasto mix e' utilizzabile o meno.
 */
extern bool lock_mix ;

/**
 * Variabile che identifica la prima delle ultime due tessere rimosse dal primo
 * giocatore.
 */
extern int last_removed_pl1_a ;

/**
 * Variabile che identifica la seconda delle ultime due tessere rimosse dal
 * primo giocatore.
 */
extern int last_removed_pl1_b ;

/**
 * Variabile che identifica la prima delle ultime due tessere rimosse dal
 * secondo giocatore.
 */
extern int last_removed_pl2_a ;

/**
 * Variabile che identifica la seconda delle ultime due tessere rimosse dal
 * secondo giocatore.
 */
extern int last_removed_pl2_b ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
/**
 * Funzione che chiama la funzione gtk_main.
 */
void call_gtk_main() ;

/**
 * Funzione che chiama la funzione gtk_main_quit.
 */
void call_gtk_main_quit() ;

/**
 * Funzione che chiama inizializza opportunamente le gtk.
 */
void initialize_gtk ( int argc, char * argv[] ) ;

/**
 * Funzione aggiorna le etichette riguardanti i turni.
 * @param[in] _switch booleano che indica il giocatore che deve rimuovere una
 * coppia.
 */
void refresh_turn_label ( bool _switch) ;

/**
 * Funzione aggiorna le etichette riguardanti i punteggi.
 * @param[in] score1, score2 punteggi da stampare a video.
 */
void refresh_scores_labels( const int &score1, const int &score2 ) ;

/**
 * Funzione aggiorna l' etichette riguardante il numero di coppie removibili
 * rimaste.
 * @param[in] couples coppie rimaste.
 */
void refresh_down_label ( const int & couples ) ;

/**
 * Funzione che chiede la ristampa a video di un dato widget.
 * @param[in] name nome del widget.
 */
void redraw_widget ( const char * name ) ;

/**
 * Funzione che pulisce le variabili che tengono memorizzate le ultime 4 tessere
 * rimosse.
 */
void clear_pair_removed () ;

/**
 * Funzione che setta le variabili che tengono memorizzate le ultime 2 tessere
 * rimosse da un giocatore.
 * @param[in] temp giocatore in questione.
 * @param[in] a, b interi identificativi delle due tessere.
 */
void refresh_pair_removed ( const p_player &temp, const int &a,
                            const int &b                        ) ;

/**
 * Funzione che chiede la stampa a video dello sfondo vuoto.
 */
void display_empty () ;

/**
 * Funzione che chiede la stampa a video delle tessere.
 */
void display_tiles () ;

/**
 * Funzione che chiede la stampa a video delle regole.
 */
void display_rules () ;

/**
 * Funzione che chiede la stampa a video del vincitore.
 */
void display_end () ;

/**
 * Funzione che chiede la stampa a video delle celle con riquadro di
 * evidenziazione.
 * @param[in] n indicatore di quale tra i due riquadri.
 * @param[in] x, y, z coordinate della cella.
 */
void set_highlighted_cell ( const int &n, const int &x, const int &y,
                            const int &z                              ) ;

/**
 * Funzione che rimuove i riquadri resettando le coordinate delle celle a valori
 * negativi.
 */
void reset_highlighted_cell () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_GUI*/
