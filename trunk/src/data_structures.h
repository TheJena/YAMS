/**
 * @file
 * File che descrive tutte le strutture dati create ad_hoc per questo gioco.
 * Esse vengono poi incluse o meno nei rispettivi moduli attraverso delle macro,
 * ho scelto questo uso anomalo delle strutture dati perche' permette una
 * distinzione piu' precisa dei moduli, che altrimenti risultano difficilmente
 * scindibili, e portano ad errori di compilazione.
 */

#ifdef enum_computer
/**
 * Enumerato che identifica la modalita' di AI.
 */
    enum computer { airhead, greedy, thoughtful } ;
#endif

#ifdef enum_event_box
/**
 * Enumerato che identifica la schermata da stampare nell'area centrale della
 * finestra.
 */
    enum event_box { empty, rules, tiles, end } ;
#endif

#ifdef  enum_game
/**
 * Enumerato che identifica la modalita' di gioco, umano vs umano o umano vs AI.
 */
    enum game { h_c, h_h } ;
#endif

/**
 * Enumerato che identifica la disposizione delle tessere.
 */
#ifdef enum_layout
    enum layout { easy, medium, difficult } ;
#endif

#ifdef enum_p_player
/**
 * Enumerato che identifica il tipo di giocatore: umano principale, umano
 * secondario ed intelligenza artificiale.
 */
    enum p_player { p_human1, p_human2, p_ai } ;
#endif

#ifdef enum_tile_type

/**
 * Enumerato che identifica il tipo di tessera: numerica o dipinta.
 */
    enum tile_type { tt_number, tt_image } ;
#endif

#ifdef struct_couple
/**
 * Struttura che identifica una coppia di tessere candidate alla rimozione o al
 * salvataggio o alla verifica.
 */
    struct couple   {
                        int t1 ;
                        const char * name1 ;
                        int x1 ;
                        int y1 ;
                        int z1 ;

                        int t2 ;
                        const char * name2 ;
                        int x2 ;
                        int y2 ;
                        int z2 ;
                     } ;
#endif

#ifdef struct__string
/**
 * Struttura che permette una piu' semplice gestione di una matrice di
 * caratteri, altrimenti detta, elenco di parole disposte in righe.
 */
    struct _string  {
                        char * word ;
                     } ;
#endif

#ifdef struct_tile
/**
 * Struttura che identifica una cella del cubo (in pratica); a livello astratto
 * se il campo num e' compreso tra 0 e TILES-1 rappresenta invece una tessera.
 */
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
#endif
