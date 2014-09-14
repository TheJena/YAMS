/**
 * @file
 * File che contiene l'interfaccia del modulo CUBE. Qui vengono dichiarate le
 * funzioni che operano su tale struttura dati, appunto la cube.
 */
/*inizio header modulo_CUBE*/

/**
 * Struttura che identifica una cella del cubo (in pratica); a livello astratto
 * se il campo num e' compreso tra 0 e TILES-1 rappresenta invece una tessera.
 */
#define struct_tile
#include "data_structures.h"
#undef struct_tile

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/

/**
 * Numero di tessere massimo.
 */
extern const int TILES ;
/**
 * Limite in larghezza al numero di tessere del piano di gioco.
 */
extern const int dim_X ;
/**
 * Limite in profondita' al numero di tessere del piano di gioco.
 */
extern const int dim_Y ;
/**
 * Limite in altezza al numero di tessere del piano di gioco.
 */
extern const int dim_Z ;

/**
 * Core della struttura dati tridimensionale per memorizzare le tessere.
 */
extern tile *** cube ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
/**
 * Funzione incaricata ad allocare memoria per la struttura dati 3d.
 */
void create_cube () ;

/**
 * Funzione incaricata a deallocare la memoria della struttura dati 3d.
 */
void delete_cube () ;

/**
 * Funzione incaricata di mescolare le tessere ancora presenti in gioco.
 * @return false se non ci sono piu' tessere o se sono occorse
 * complicazioni durante l' esecuzione della funzione ::remove_dummies.
 */
bool mix_cube () ;

/**
 * Funzione incaricata di aggiornare i campi delle tessere ancora in gioco.
 */
void check_cube () ;

/**
 * Funzione incaricata di valutare la convenienza di una mossa compiuta dal AI.
 * @param[in] a     puntatore alla prima tessera selezionata.
 * @param[in] b     puntatore alla seconda tessera selezionata.
 * @param[in,out] exit booleano che conferma / scarta la coppia proposta.
 */
void check_convenience ( tile * a, tile * b, bool &exit ) ;

/**
 * Funzione incaricata a ritornare le coordinate di una tessera passata
 * tramite il suo campo num.
 * @param[in] num   intero che identifica univocamente la tessera.
 * @param[out] _x, _y, _z coordinate della tessera.
 */
void find_coord ( const int &num, int &_x, int &_y, int &_z ) ;

/**
 * Funzione incaricata a settare una cella, affinche contenga una certa tessera.
 * @param[in] x, y, z interi che identificano la cella da riempire per
 * coordinate.
 * @param[in,out] last intero che setta la cella a contenere la tessera
 * identificata da tale numero, il parametro last viene inoltre incrementato.
 */
void fill_cell ( const int &x, const int &y, const int &z, int &last ) ;

/**
 * Funzione incaricata a pulire completamente ogni campo della cella indicata.
 * @param[in] x, y, z interi che identificano la cella per coordinate.
 */
void reset_cell ( const int &x, const int &y, const int &z ) ;

/**
 * Funzione incaricata alla rimozione delle due tessere finte, usate per
 * indicare la mossa compiuta dal AI.
 * @return false se sono occorse complicazioni durante l'esecuzione delle
 * funzioni: ::sort_unlocked e ::count_pairs_removable.
 */
bool remove_dummies() ;

/**
 * Funzione incaricata a pulire completamente tutte le celle.
 */
void initialize_cube () ;

/**
 * Funzione incaricata a disporre nel cubo le tessere secondo il layout scelto
 * dall'utente.
 */
void fill_cube () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_CUBE*/
