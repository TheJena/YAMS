/**
 * @mainpage Mahjong
 *
 * Semplice gioco da tavolo, leggermente modificato rispetto alla versione
 * tradizionale. La novita' infatti e' la modalita' di gioco, ovvero non piu' il
 * solitario, bensi' un gioco per due: a turno i giocatori devono rimuovere una
 * coppia di tessere, col fine di massimizzare il punteggio totale, e vincere
 * contro l'avversario, quest'ultimo puo' essere o il computer stesso o una 
 * seconda persona. Sono inoltre disponibili tre livelli di intelligenza
 * artificiale contro cui confrontarsi; tre possibili disposizioni delle tessere
 * e comandi per salvare e ricaricare le partite.
 *
 * Per ulteriori dettagli sull'implementazione si rimanda alla documentazione
 * della funzione ::main.
 *
 * La divisione in moduli del programma e' la seguente:
 * - cube.cc    fornisce la struttura 3d di celle nelle quali vengono
 *  memorizzate le tessere, e le funzioni ad essa applicate.
 * - game.cc    fornisce la struttura dati che permette di giocare e tenere in
 * memoria la partita in corso.
 * - gui.cc     collega gli input grafici dell' utente al resto del programma,
 * inoltre implementa tutta la parte grafica del gioco.
 * - io_file.cc si occupa di ogni lettura e scrittura da / su file.
 * - movements.cc contiene la struttura dati necessaria per verificare le mosse
 * consentite, controllare quali tessere sono removibili e calcolare i punteggi.
 *
 * @author Federico Motta
 */

/**
 * @file
 * File che contiene il main del programma. Qui vengono solamente chiamate le
 * funzioni necessarie per avviare il programma ed aspettare input dall' utente.
 */
#include <iostream>
#include "debug_macros.h"
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "cube.h"                       //include gia la definizione struct_tile
#include "io_file.h"                // include gia' la definizione struct_couple

/*inizio implementazione modulo_MACRO*/
#ifdef DEBUG_MODE
    /*inizio implementazione struttura dati pubblica*/
    unsigned int MASK = 1|2|3|4|5|6|7|8|9|10 ;
    /*fine implementazione struttura dati pubblica*/
#endif
/*fine implementazione modulo_MACRO*/

using namespace std ;

int main ( int argc, char * argv[] )
{
    D1(cerr<<"D1 main\n")

    initialize_gtk(argc, argv) ;

    create_cube () ;
    import_tiles_names () ;

    display_rules () ;

    call_gtk_main() ;

    D10(cerr<<"D10 main\n")
    return 0 ;
}
