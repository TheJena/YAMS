#include <cstdlib>
#include "data_structure.h"

using namespace std ;

const int TILES = 144 ;

const int dim_X = 12 ;
const int dim_Y = 8 ;
const int dim_Z = 4 ;

const int   SEEDVALUE   = 1 ;
const int   WINDVALUE   = 3 ;
const int   DRAGONVALUE = 5 ;
const int   SEASONVALUE = 12 ;
const int   FLOWERVALUE = 14 ;

layout level = medium ;

computer ai = greedy ;

tile *** cube = NULL ;
tile *    unlocked[TILES] ;

_string * name = NULL ;