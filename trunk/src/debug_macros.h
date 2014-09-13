/**
 * @file
 * File che contiene le macro e gli strumenti finalizzati al debug.
 */
/*inizio header modulo_MACRO*/
#include <iostream>

/*inizio interfaccia modulo_MACRO*/
#ifndef DEBUG_MODE
/**
 * se definita, NDEBUG disattiva le asserzioni,
 * se non definita le attiva
 */
    #define NDEBUG
#endif

#include <cassert>

/*inizio struttura dati pubblica*/
/**
 * intero senza segno che opportunamente settato attiva / disattiva le macro di
 * debug (in particolare per il tracing).
 * E' sufficiente porlo pari all' OR bit a bit dei numeri identificanti le macro,
 * per attivare le stesse.
 */
extern unsigned int MASK ;
/*fine struttura dati pubblica*/

#ifdef DEBUG_MODE
    #define DBG( a , b ) { if ( ( a ) & MASK ) { b ; } }
#else
    #define DBG( a , b )
#endif


#define D1(a)   DBG(1,a)    /**< per inizio funzioni "rare"*/
#define D2(a)   DBG(2,a)    /**< per inizio funzioni frequenti*/
#define D3(a)   DBG(3,a)    /**< per indicare quale funzione ha generato un D?*/
#define D4(a)   DBG(4,a)    /**< per parametri cruciali di alcune funzioni*/
#define D5(a)   DBG(5,a)    /**< per le funzioni inerenti i/o da file*/
#define D6(a)   DBG(6,a)    /**< per return false di sort unlocked*/
#define D7(a)   DBG(7,a)    /**< per return false di mix cube*/
#define D8(a)   DBG(8,a)    /**< per altre return false*/
#define D9(a)   DBG(9,a)    /**< per uscita funzioni frequenti*/
#define D10(a)  DBG(10,a)   /**< per uscita funzioni "rare"*/

/*fine interfaccia modulo_MACRO*/

/*fine header modulo_MACRO*/
