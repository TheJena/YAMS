/**
 * @file
 * File che contiene l'interfaccia del modulo MOVEMENTS. Qui vengono dichiarate le
 * funzioni che operano sulla struttura dati unlocked.
 */
/*inizio header modulo_MOVEMENTS*/

/**
 * Enumerato che identifica la modalita' di AI.
 */
#define enum_computer
#include "data_structures.h"
#undef enum_computer

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/
/**
 * Costante che contiene il valore dei semi: bamboo, circle, cross.
 */
extern const int   SEEDVALUE ;

/**
 * Costante che contiene il valore dei venti.
 */
extern const int   WINDVALUE ;

/**
 * Costante che contiene il valore dei draghi.
 */
extern const int   DRAGONVALUE ;

/**
 * Costante che contiene il valore delle stagioni.
 */
extern const int   SEASONVALUE ;

/**
 * Costante che contiene il valore dei fiori.
 */
extern const int   FLOWERVALUE ;

/**
 * Variabile che indica quale modalita' di AI e' in uso.
 */
extern computer ai ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
/**
 * Funzione che visitando l' intero cubo, genera un array di puntatori alle
 * tessere removibili.
 */
void refresh_unlocked () ;

/**
 * Funzione che estrae a caso una coppia dall' array unlocked.
 * @param[out] first, second puntatori alle tessere estratte.
 * @param[in,out] exit booleano che indica il valore ritornato dalla funzione
 * ::check_pair, in conseguenza al calcolo della funzione
 * ::count_pairs_removable delle coppie ancora removibili.
 * @return se l' estrazione e' avvenuta con successo ( erano ancora presenti
 * tessere removibili) o meno ( erano finite, quindi il gioco e' terminato).
 */
bool airhead_extraction ( tile * &first, tile * &second, bool &exit ) ;

/**
 * Funzione che memorizza una coppia nella tabella mov.
 * @param[out] pair puntatore alla cella della tabella mov.
 * @return il valore di ritorno della funzione ::airhead_extraction.
 */
bool extract_pair ( couple *  pair ) ;

/**
 * Funzione che ordina per valori decrescenti i puntatori alle tessere
 * removibili
 * @return false se la funzione: ::count_pairs_removable ritorna -1.
 * Altrimenti true.
 */
bool sort_unlocked() ;

/**
 * Funzione che calcola il numero di coppie removibili.
 * @param[in] count contatore dell'eventuale ricorsione.
 * @return 0 se il numero di tessere removibili e' negativo, -1 se pari a zero,
 * altrimenti il numero delle stesse.
 */
int count_pairs_removable ( const int &count) ;

/**
 * Funzione che ritorna se un numero e' o meno compreso tra altri due.
 * @param[in] min, max estremi dell'intervallo.
 * @param[in] middle numero da controllare se rientra nell'intervallo o meno.
 * @return true se appartiene all' intervallo, false altrimenti.
 */
bool between ( const int &min, const int &middle, const int &max ) ;

/**
 * Funzione che ritorna il valore di una tessera passata in input.
 * @param[in] name_position numero identificativo della tessera.
 * @return il valore della tessera o -1 se essa e' sconosciuta.
 */
int tile_value (const int &name_position) ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_MOVEMENTS*/
