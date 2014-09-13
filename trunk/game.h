/*inizio header modulo_GAME*/

#define struct__string
#define enum_game
#include "data_structures.h"
#undef struct__string
#undef enum_game

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/
extern _string * name ;

extern game mode ;

extern const int max_couple_row ;

extern bool playing ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
bool start_game () ;

bool save_game(char * filename ) ;

bool load_game( char * filename ) ;

void undo_last_two_couples () ;

void refresh_scores() ;

void reset_row() ;

bool insert_half_pair ( const int &num, const int &x, const int &y, const int &z ) ;

void end_game () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_GAME*/
