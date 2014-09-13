/**
 * @file
 * File che contiene l'interfaccia del modulo GAME. Qui vengono dichiarate le
 * funzioni che operano sulla struttura dati mov.
 */
/*inizio header modulo_GAME*/

/**
 * Struttura che permette una piu' semplice gestione di una matrice di caratteri,
 * altrimenti detta, elenco di parole disposte in righe.
 */
#define struct__string
/**
 * Enumerato che identifica la modalita' di gioco, umano vs umano o umano vs AI.
 */
#define enum_game
#include "data_structures.h"
#undef struct__string
#undef enum_game

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/
extern _string * name ;

/**
 * Variabile che indica la modalita' di gioco: umano vs umano o umano vs ai.
 */
extern game mode ;

/**
 * Costante che indica il numero massimo di righe nella tabella mov.
 */
extern const int max_couple_row ;

/**
 * Booleano che indica se si sta' giocando o meno.
 */
extern bool playing ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
/**
 * Funzione che predispone l'avvio della partita.
 * @return false se occorrono complicazioni durante l'esecuzione della funzione:
 * ::mix_cube, o se e' gia in corso una partita non ancora terminata. Altrimenti
 * true.
 */
bool start_game () ;

/**
 * Funzione che passa al modulo io_file, i dati necessari per salvare la
 * partita.
 * @return il valore di ritorno della funzione ::save_game_on_file.
 */
bool save_game(char * filename ) ;

/**
 * Funzione che passa al modulo io_file, i dati necessari per caricare una
 * partita.
 * @return il valore di ritorno della funzione ::load_game_from_file.
 */
bool load_game( char * filename ) ;

/**
 * Funzione che rimette in gioco le ultime 4 tessere rimosse e reinizializza
 * la zona di struttura dati dove erano memorizzate.
 */
void undo_last_two_couples () ;

/**
 * Funzione che visitando l'intera partita mossa per mossa calcola il punteggio
 * di ogni giocatore e lo pone in due variabili statiche al file che pero'
 * passa per riferimento costante alla funzione ::refresh_scores_labels, per
 * stamparle a video.
 */
void refresh_scores() ;

/**
 * Funzione che pulisce la riga della tabella mov all' indice di riga statico
 * globale.
 */
void reset_row() ;

/**
 * Funzione che inserice nella tabella mov, la tessera passata in input.
 * @param[in] num, x, y, z valore numerico identificativo della tessera e
 * coordinate della medesima.
 * @return il valore di ritorno della ::check_couple.
 */
bool insert_half_pair ( const int &num, const int &x, const int &y, const int &z ) ;

/**
 * Funzione che predispone il termine della partita.
 */
void end_game () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_GAME*/
