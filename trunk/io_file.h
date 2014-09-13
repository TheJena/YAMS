/*inizio header modulo_IOFILE*/

#define struct_couple
#include "data_structures.h"
#undef struct_couple

/*inizio interfaccia*/

/*inizio prototipi funzioni pubbliche*/

bool save_game_on_file ( const couple** mov, char * filename,
                         const int &row, const int &col ) ;

bool load_game_from_file ( couple** &mov, const char * filename, int &row, int &col ) ;

void import_tiles_names () ;

void delete_tiles_names () ;
/*fine prototipi funzioni pubbliche*/

/*fine interfaccia*/

/*fine header modulo_IOFILE*/
