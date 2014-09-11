#include "data_structure.h"

/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube () ;

/*
 * Funzione che dealloca lo spazio 3D (x, y, z) di celle per tessere
 */
void delete_cube ( ) ;

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
 * Funzione che genera due coordinate random
 */
void generate_random ( int &x1, int &y1, int &z1, int &x2, int &y2, int &z2 ) ;

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

bool check_pair ( const tile * const a, const tile * const b,
                  tile * &first, tile * &second ) ;

void initialize_neighbor ( tile * &left_a, tile * &right_a, tile * &under_a,
                           tile * &left_b, tile * &right_b, tile * &under_b,
                           const int &xa, const int &ya, const int &za,
                           const int &xb, const int &yb, const int &zb,
                           tile * &a, tile * &b ) ;

void check_convenience ( tile * a, tile * b, bool &exit ) ;

void airhead_extraction ( tile * &first, tile * &second, bool &exit ) ;

void extract_pair ( couple *  pair ) ;

void find_coord ( const int &num, int &_x, int &_y, int &_z ) ;

bool between ( const int &min, const int &middle, const int &max ) ;

/*
 * Funzione che aggiorna la removibilita' di una singola tessera
 * visitando le celle destra e sinistra
 */
void check_cell ( const int &x, const int &y, const int &z ) ;

/*
 * Funzione che ritorna true se la cella sinistra e' libera
 */
bool left_cell ( const int &x, const int &y, const int &z ) ;

/*
 * Funzione che ritorna true se la cella destra e' libera
 */
bool right_cell ( const int &x, const int &y, const int &z ) ;

/*
 * Funzione che ordina l'array di puntatori a tessere rimuovibili "unlocked"
 * in ordine decrescente di valore
 */
void sort_unlocked() ;

int count_pairs_removable (const int &counter) ;

/*
 * Funzione che ordina alfabeticamente un sotto array
 */
void sort_sub_array ( tile ** out, int start, int end ) ;

/*
 * Funzione che scambia due tessere passate tramite le coordinate
 * x1, y1, z1 <--> x2, y2, z2
 */
void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i ) ;

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

void reset_cell ( const int &x, const int &y, const int &z ) ;

void remove_dummies() ;
