#include <iostream>
#include "debug_macros.h"
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "cube.h"                       //include gia la definizione struct_tile
#include "io_file.h"                // include gia' la definizione struct_couple

/*inizio implementazione modulo_MACRO*/
#ifdef DEBUG_MODE
    /*inizio implementazione struttura dati pubblica*/
    unsigned int MASK = 1|2|3|4|5|6|7|8|9|10 ;
    /*fine implementazione struttura dati pubblica*/
#endif
/*fine implementazione modulo_MACRO*/

using namespace std ;

int main ( int argc, char * argv[] )
{
    D1(cerr<<"D1 main\n")

    initialize_gtk(argc, argv) ;

    create_cube () ;
    import_tiles_names () ;

    display_rules () ;

    call_gtk_main() ;
    /*
     * non inserire nulla qui in mezzo.
     * questa parte viene eseguita quando si chiude la finestra.
     */

    D10(cerr<<"D10 main\n")
    return 0 ;
}
