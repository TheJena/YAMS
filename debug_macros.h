#include <iostream>

#ifndef DEBUG_MODE
    #define NDEBUG
#endif

#include <cassert>



#ifdef DEBUG_MODE
    #define DBG( a , b ) { if ( ( a ) & MASK ) { b ; } }
#else
    #define DBG( a , b )
#endif


#define D1(a)   DBG(1,a)        // per inizio funzioni "rare"
#define D2(a)   DBG(2,a)        // per inizio funzioni frequenti
#define D3(a)   DBG(3,a)
#define D4(a)   DBG(4,a)
#define D5(a)   DBG(5,a)
#define D6(a)   DBG(6,a)
#define D7(a)   DBG(7,a)
#define D8(a)   DBG(8,a)
#define D9(a)   DBG(9,a)        // per uscita funzioni frequenti
#define D10(a)  DBG(10,a)       // per uscita funzioni "rare"
