#include <cstdlib>

using namespace std ;

extern unsigned int MASK ;

extern const int TILES ;

extern const int dim_X ;
extern const int dim_Y ;
extern const int dim_Z ;

extern const int   SEEDVALUE ;
extern const int   WINDVALUE ;
extern const int   DRAGONVALUE ;
extern const int   SEASONVALUE ;
extern const int   FLOWERVALUE ;

enum layout { easy, medium, difficult } ;
enum computer { airhead, greedy, thoughtful } ;
enum game { h_c, h_h } ;

extern layout level ;
extern computer ai ;
extern game mode ;

struct tile {
                bool    empty ;
                bool    lock ;
                int     num ;
                int     value ;

                int     x1 ;
                int     y1 ;

                int     x2 ;
                int     y2 ;
            } ;

extern tile *** cube ;
extern const int FREE ;
extern tile *    unlocked[] ;

struct _string  {
                    char * word ;
                } ;
extern _string * name ;
