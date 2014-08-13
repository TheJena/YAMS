#include <iostream>

using namespace std ;

const int   TILES       = 144 ;
const int   SEEDVALUE   = 1 ;
const int   WINDVALUE   = 3 ;
const int   DRAGONVALUE = 5 ;
const int   SEASONVALUE = 12 ;
const int   FLOWERVALUE = 14 ;
const char  F_TILES     = "./tiles_names.dat" ;

enum layout { easy, medium, difficult } level ;

enum computer { airhead, greedy, thoughtful } ai;

struct tile {
                bool    empty ;
                bool    lock ;
                int     num ;
                int     value ;
            } ;

tile *** cube = NULL ;
tile *    unlocked[TILES] ;
struct string   {
                    char * word ;
                } ;

string * name = NULL ;






