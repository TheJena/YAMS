#include "game.h"

/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube () ;

/*
 * Funzione che inizializza tutte le celle del cubo
 */
void initialize_cube () ;

/*
 * Funzione che inserisce nelle celle del cubo le tessere secondo il livello
 * easy, medium, difficult
 */
void fill_cube ( ) ;

/*
 * Funzione che mischia le tessere del cubo
 * (in un numero random di scambi)
 */
void mix_cube () ;

/*
 * Funzione che aggiorna la removibilita' delle celle
 * controllando che almeno una delle due adiacenti sia libera
 */
void check_cube () ;

/*
 * Funzione che ricalcola l'array unlocked di puntatori alle tessere
 * rimuovibili (solo se il primo elemento e' a NULL), e ordina
 * quest'ultimo in ordine decrescente di valore.
 */
void refresh_unlocked () ;

void extract_pair ( couple *  pair ) ;

/*
 * Funzione che ordina alfabeticamente un sotto array
 */
void sort_sub_array ( tile ** out, int start, int end ) ;

/*
 * Funzione che ritorna true se la cella sinistra e' libera
 */
bool left_cell ( const int &x, const int &y, const int &z ) ;

/*
 * Funzione che ritorna true se la cella destra e' libera
 */
bool right_cell ( const int &x, const int &y, const int &z ) ;

bool check_pair ( const tile * const a, const tile * const b,
                  tile * &first, tile * &second ) ;

void extract_pair ( couple *  pair ) ;

void find_coord ( const int &num, int &_x, int &_y, int &_z ) ;

bool check_solvability ( int counter ) ;

bool between ( const int &min, const int &middle, const int &max ) ;

/*
 * Funzione che aggiorna la removibilita' di una singola tessera
 * visitando le celle destra e sinistra
 */
void check_cell ( const int &x, const int &y, const int &z ) ;





/*
 * Funzione che ordina l'array di puntatori a tessere rimuovibili "unlocked"
 * in ordine decrescente di valore
 */
void sort_unlocked() ;

/*
 * Funzione che scambia due tessere passate tramite le coordinate
 * x1, y1, z1 <--> x2, y2, z2
 */
void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i ) ;

/*
 * Funzione che genera due coordinate random
 */
void generate_random ( int &x1, int &y1, int &z1, int &x2, int &y2, int &z2 ) ;

/*
 * Funzione di popolamento del cubo con disposizione livello easy
 */
void fill_easy_layout ( int &last ) ;

/*
 * Funzione di popolamento del cubo con disposizione livello medium
 */
void fill_medium_layout ( int &last ) ;

/*
 * Funzione di popolamento del cubo con disposizione livello difficult
 */
void fill_difficult_layout ( int &last ) ;

/*
 * Funzione che passato in ingresso il numero della tessera
 * ritorna il valore della stessa
 */
int tile_value (const int &name_position) ;

/*
 * Funzione che prende in ingresso le coordinate x, y, z e il numero di tessera,
 * ed inserisce nella cella x, y, z la tessera corrispondente;
 * inoltre incrementa il numero di tessera di uno.
 */
void fill_cell ( const int &x, const int &y, const int &z, int &last) ;

/*
 * Funzione che riempie un rettangolo di celle tali che
 * x1 <= x <= x2 e y1 <= y <= y2 e altezza = z
 * con numeri di tessera progressivi
 */
void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last ) ;

/*
 * Funzione che dealloca lo spazio 3D (x, y, z) di celle per tessere
 */
void delete_cube ( ) ;

void reset_cell ( const int &x, const int &y, const int &z ) ;

void count_pairs_removable () ;

void airhead_extraction ( tile * &first, tile * &second, bool &exit ) ;



