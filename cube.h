/*inizio header modulo_CUBE*/

#define struct_tile
#include "data_structures.h"
#undef struct_tile

/*inizio interfaccia*/

/*inizio struttura dati pubblica*/


extern const int TILES ;
extern const int dim_X ;
extern const int dim_Y ;
extern const int dim_Z ;

extern tile *** cube ;
/*fine struttura dati pubblica*/

/*inizio prototipi funzioni pubbliche*/
void create_cube () ;

void delete_cube () ;

bool mix_cube () ;

void check_cube () ;

void check_convenience ( tile * a, tile * b, bool &exit ) ;

void find_coord ( const int &num, int &_x, int &_y, int &_z ) ;

void fill_cell ( const int &x, const int &y, const int &z, int &last) ;

void reset_cell ( const int &x, const int &y, const int &z ) ;

bool remove_dummies() ;

void initialize_cube () ;

void fill_cube () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_CUBE*/
