#include <cstdlib>

using namespace std ;

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

extern layout level ;

extern computer ai ;

struct tile {
                bool    empty ;
                bool    lock ;
                int     num ;
                int     value ;
            } ;

extern tile *** cube ;
extern tile *    unlocked[] ;

struct _string  {
                    char * word ;
                } ;
extern _string * name ;
