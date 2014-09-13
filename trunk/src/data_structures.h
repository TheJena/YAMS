#ifdef enum_computer
    enum computer { airhead, greedy, thoughtful } ;
#endif

#ifdef enum_event_box
    enum event_box { empty, rules, tiles, end } ;
#endif

#ifdef  enum_game
    enum game { h_c, h_h } ;
#endif

#ifdef enum_layout
    enum layout { easy, medium, difficult } ;
#endif

#ifdef enum_p_player
    enum p_player { p_human1, p_human2, p_ai } ;
#endif

#ifdef enum_tile_type
    enum tile_type { tt_number, tt_image } ;
#endif

#ifdef struct_couple
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
    struct _string  {
                        char * word ;
                     } ;
#endif

#ifdef struct_tile
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
