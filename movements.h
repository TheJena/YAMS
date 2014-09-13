/*inizio header modulo_MOVEMENTS*/

#define enum_computer
#include "data_structures.h"
#undef enum_computer

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/
extern const int   SEEDVALUE ;
extern const int   WINDVALUE ;
extern const int   DRAGONVALUE ;
extern const int   SEASONVALUE ;
extern const int   FLOWERVALUE ;

extern computer ai ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
void refresh_unlocked () ;

bool airhead_extraction ( tile * &first, tile * &second, bool &exit ) ;

bool extract_pair ( couple *  pair ) ;

bool sort_unlocked() ;

int count_pairs_removable ( const int &count) ;

bool between ( const int &min, const int &middle, const int &max ) ;

int tile_value (const int &name_position) ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_MOVEMENTS*/
