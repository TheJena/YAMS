/**
 * @file
 * File che contiene l'interfaccia del modulo IOFILE. Qui vengono dichiarate
 * le funzioni che operano su tale struttura dati.
 */
/*inizio header modulo_IOFILE*/

/**
 * Struttura che identifica una coppia di tessere candidate alla rimozione o al
 * salvataggio o alla verifica.
 */
#define struct_couple
#include "data_structures.h"
#undef struct_couple

/*inizio interfaccia*/

/*inizio prototipi funzioni pubbliche*/
/**
 * Funzione che salva su file binario una partita.
 * @param[in] mov tabella da salvare
 * @param[in] filename nome del file binario
 * @param[in] row, col limite della tabella della parte gia' memorizzata di
 * partita.
 * @return false se fallisce il salvataggio, true altrimenti.
 */
bool save_game_on_file ( const couple** mov, char * filename,
                         const int &row, const int &col       ) ;

/**
 * Funzione che carica da file binario una partita.
 * @param[in] mov tabella da caricare
 * @param[in] filename nome del file binario
 * @param[in] row, col limite della tabella della parte gia' memorizzata di
 * partita.
 * @return false se fallisce il caricamento o se la partita che si vuole
 * importare non e' coerente coi parametri attuali di gioco, true altrimenti.
 */
bool load_game_from_file ( couple** &mov, const char * filename,
                           int &row, int &col                    ) ;

/**
 * Funzione che carica da file binario l' elenco dei nomi delle tessere.
 * Per poi memorizzarlo in un array.
 */
void import_tiles_names () ;

/**
 * Funzione che dealloca l'array dinamico coi nomi delle tessere.
 */
void delete_tiles_names () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_IOFILE*/
