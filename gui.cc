#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "gui.h"
#include "game.h"
#include "io_file.h"
#include "debug_macros.h"



using namespace std ;

const char* I_AUTUMN            = "./autumn.png" ;
const char* I_BAMBOO            = "./bamboo.png" ;
const char* I_BAMBOO_FOREST     = "./bamboo_forest.png" ;
const char* I_CHRYSANTEMUM      = "./chrysantemum.png" ;
const char* I_CIRCLE            = "./circle.png" ;
const char* I_CROSS             = "./cross.png" ;
const char* I_EAST              = "./east.png" ;
const char* I_GREEN_DRAGON      = "./green_dragon.png" ;
const char* I_NORTH             = "./north.png" ;
const char* I_ORCHID            = "./orchid.png" ;
const char* I_PLUMB              = "./plumb.png" ;
const char* I_RED_DRAGON        = "./red_dragon.png" ;
const char* I_SPRING            = "./spring.png" ;
const char* I_SUD               = "./sud.png" ;
const char* I_SUMMER            = "./summer.png" ;
const char* I_TILE              = "./tile_shape.png" ;
const char* I_WEST              = "./west.png" ;
const char* I_WHITE_DRAGON      = "./white_dragon.png" ;
const char* I_WINTER            = "./winter.png" ;
const char* I_DUMMY             = "./dummy.png" ;

const char* W_newgame           = "window_new_game" ;

const int MAXLUN = 100 ;

enum event_box { empty, rules, tiles, end } play_ground ;



GtkBuilder * builder ;

colour black = { 0.0/255, 0.0/255, 0.0/255 } ;
colour blue_persia = { 28.0/255, 57.0/255, 187.0/255 } ;
colour carmine = { 150.0/255, 0.0/255, 24.0/255 } ;
colour coffe = { 111.0/255, 78.0/255, 55.0/255 } ;
colour ferrari = { 204.0/255, 0.0/255, 0.0/255 } ;
colour gold = { 255.0/255, 215.0/255, 0.0/255 } ;
colour green_forest = { 34.0/255, 139.0/255, 34.0/255 } ;
colour lapislazuli = { 38.0/255, 97.0/255, 156.0/255 } ;
colour light_blue = { 0.0/255, 127.0/255, 255.0/255 } ;
colour light_orange = { 255.0/255, 153.0/255, 0.0/255 } ;
colour peach = { 255.0/255, 229.0/255, 180.0/255 } ;
colour sand = { 244.0/255, 164.0/255, 96.0/255 } ;
colour white = { 255.0/255, 255.0/255, 255.0/255 } ;
colour yellow = { 255.0/255, 255.0/255, 0.0/255 } ;


int h_x1 = -1 ;
int h_y1 = -1 ;
int h_z1 = -1 ;

int h_x2 = -1 ;
int h_y2 = -1 ;
int h_z2 = -1 ;

int last_removed_pl1_a = -1 ;     // removed by player 1
int last_removed_pl1_b = -1 ;     // removed by player 1
int last_removed_pl2_a = -1 ;     // removed by ai or player 2
int last_removed_pl2_b = -1 ;     // removed by ai or player 2

void display_end ()
{
    D1(cerr<<"D1 display end\n")

    refresh_scores(_score1, _score2) ;
    play_ground = end ;
    redraw_widget ("playground") ;

    D10(cerr<<"D10 display end\n")
}

void display_empty ()
{
    D1(cerr<<"D1 display empty\n")

    play_ground = empty ;
    redraw_widget ("playground") ;

    D10(cerr<<"D10 display empty\n")
}

void display_tiles ()
{
    D1(cerr<<"D1 display tiles\n")

    play_ground = tiles ;
    redraw_widget ("playground") ;

    D10(cerr<<"D10 display tiles\n")
}

void display_rules ()
{
    D1(cerr<<"D1 display rules\n")

    play_ground = rules ;
    redraw_widget ("playground") ;

    D10(cerr<<"D10 display rules\n")
}

void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z )
{
    D1(cerr<<"D1 set highlighted cell\n")

    if ( n == 1 )
    {
        h_x1 = x ;
        h_y1 = y ;
        h_z1 = z ;
    } else
    if ( n == 2 )
    {
        h_x2 = x ;
        h_y2 = y ;
        h_z2 = z ;
    } else { /*debug this case*/ ; }

    D10(cerr<<"D10 set highlighted cell\n")
}

void reset_highlighted_cell ()
{
    D1(cerr<<"D1 reset highlighted cell\n")

    h_x1 = -1 ;
    h_y1 = -1 ;
    h_z1 = -1 ;

    h_x2 = -1 ;
    h_y2 = -1 ;
    h_z2 = -1 ;

    D10(cerr<<"D10 reset highlighted cell\n")
}

bool check_position ( const tile * punt, const int &x, const int &y )
{
    D1(cerr<<"D1 check position\n")

    if ( ( punt->x1 ) >= x )
        return false ;
    if ( ( punt->y1 ) >= y )
        return false ;

    if ( ( punt->x2 ) <= x )
        return false ;
    if ( ( punt->y2 ) <= y )
        return false ;

    return true ;

    D10(cerr<<"D10 check position\n")
}
extern "C" gboolean handler_click_on_widget (GtkWidget *widget,
               GdkEventButton  *event,
               gpointer   user_data)
{
    D1(cerr<<"D1 handler click on widget\n")

    if ( play_ground != tiles )
        return TRUE ;
    const int _x = event->x ;
    const int _y = event->y ;

    if ( mode == h_c )
        if ( !remove_dummies() )
            return TRUE ;

    bool quit = false ;
    for ( int z = dim_Z-1 ; (z >= 0)&&(!quit) ; z-- )
        for ( int y = 0 ; (y < dim_Y)&&(!quit) ; y++ )
            for ( int x = 0 ; (x < dim_X)&&(!quit) ; x++ )
            {

                if ( ( !cube[x][y][z].empty )&&( !cube[x][y][z].lock ) )
                {
                    if ( check_position( &cube[x][y][z], _x, _y ) )
                    {
                        if ( !insert_half_pair( cube[x][y][z].num, x, y, z ) )
                            return TRUE ;
                        quit = true ;
                    }
                }
            }

    if ( !quit )
    {
        reset_highlighted_cell() ;
        reset_row() ;
        redraw_widget ( "playground" ) ;
    }

return TRUE ;

    D10(cerr<<"D10 handler click on widget\n")
}


extern "C" gboolean handler_delete_event ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)  
{
    D1(cerr<<"D1 handler delete event\n")

	if ( cube != NULL )
		delete_cube () ;
    if ( name != NULL )
        delete_tiles_names () ;
    if ( playing )
        end_game() ;

    gtk_main_quit() ;
    cout<<"programma terminato regolarmente\n" ;
    return TRUE ;

    D10(cerr<<"D10 handler delete event\n")
}

extern "C" gboolean handler_hide_window ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
    D1(cerr<<"D1 handler hide window\n")

    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;

    if ( playing )
        display_tiles() ;
    else
        display_empty() ;

    return TRUE ;

    D10(cerr<<"D10 handler hide window\n")
}

extern "C" gboolean handler_set_new_game ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
    D1(cerr<<"D1 handler set new game\n")

    end_game();

/*handler per rendere effettive le disposizioni per la nuova partita*/
    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_easy" ) ) )
        level = easy ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_medium" ) ) )
        level = medium ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_difficult" ) ) )
        level = difficult ;


    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_h-c" ) ) )
        mode = h_c ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_h-h" ) ) )
        mode = h_h ;


    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_airhead" ) ) )
    {
        ai = airhead ;
        lock_mix = false ;
    }
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_greedy" ) ) )
    {
        ai = greedy ;
        lock_mix = true ;
    }
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_thoughtful") ) )
    {
        ai = thoughtful ;
        lock_mix = true ;
        lock_undo = true ;
    }

    start_game () ;

/*alla fine nascondo la finestra*/
    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;

    display_tiles() ;

    return TRUE ;

    D10(cerr<<"D10 handler set new game\n")
}

void refresh_turn_label ( bool _switch)
{
    D2(cerr<<"D2 refresh turn label\n")

    if (playing)
    {
        char pl_1[MAXLUN] = "Player 1" ;
        char pl_2a[MAXLUN] = "Computer" ;
        char pl_2b[MAXLUN] = "Player 2" ;

        char * pl_2 = NULL ;
        if ( mode == h_c )
            pl_2 = pl_2a ;
        else if ( mode == h_h )
            pl_2 = pl_2b ;

        char markup_normal_string[MAXLUN] = "<span weight=\"normal\" size=\"medium\">" ;
        char markup_bold_string[MAXLUN] = "<span weight=\"bold\" size=\"larger\">" ;
        char end_markup[MAXLUN] = "</span>" ;


        if ( _switch == false )
        {
        /*player 1*/
        strcat( markup_bold_string, pl_1 ) ;
        strcat( markup_bold_string, end_markup ) ;
        gtk_label_set_markup( label_from_name("player1"), markup_bold_string ) ;

        strcat( markup_normal_string, pl_2 ) ;
        strcat( markup_normal_string, end_markup ) ;
        gtk_label_set_markup( label_from_name("player2"), markup_normal_string ) ;
        }
        else if ( _switch == true )
        {
        /*player 2*/
        strcat( markup_normal_string, pl_1 ) ;
        strcat( markup_normal_string, end_markup ) ;
        gtk_label_set_markup( label_from_name("player1"), markup_normal_string ) ;

        strcat( markup_bold_string, pl_2 ) ;
        strcat( markup_bold_string, end_markup ) ;
        gtk_label_set_markup( label_from_name("player2"), markup_bold_string ) ;
        }
    }
    else
    {
        gtk_label_set_text ( label_from_name("player1"), " " ) ;
        gtk_label_set_text ( label_from_name("player2"), " " ) ;
    }

    D9(cerr<<"D9 refresh turn label\n")
}

void refresh_scores_labels( const int &score1, const int &score2 )
{
    D2(cerr<<"D2 refresh scores label\n")

    char text_pl1[MAXLUN] ;
    char text_pl2[MAXLUN] ;

    char before[MAXLUN] = "Punteggio = " ;
    char after[MAXLUN] = " ." ;

    sprintf ( text_pl1, "%s%d%s", before, score1, after ) ;
    sprintf ( text_pl2, "%s%d%s", before, score2, after ) ;

    gtk_label_set_text ( label_from_name("score1"), text_pl1 ) ;
    gtk_label_set_text ( label_from_name("score2"), text_pl2 ) ;

    D9(cerr<<"D9 refresh scores label\n")
}

void refresh_down_label ( const int & couples )
{
    D2(cerr<<"D2 refresh down label\n")

    if ((playing)&&( play_ground == tiles ))
    {
        char before[MAXLUN] = "" ;
        char after[MAXLUN] = " removable couples left" ;

        if ( couples == 1 )
        {
            char b[MAXLUN] = "" ;
            char a[MAXLUN] = " removable couple left" ;
            int x, y ;
            for ( x = 0 ; b[x] != '\0' ; x++ )
                before[x]= b[x] ;
            before[x] = '\0';
            for ( y = 0 ; a[y] != '\0' ; y++ )
                after[y]= a[y] ;
            after[y] = '\0';
        }
        char sum[MAXLUN] ;

        sprintf ( sum, "%s %d %s", before, couples, after ) ;

        gtk_label_set_text ( label_from_name("label_down"), sum ) ;
    }
    else
    gtk_label_set_text ( label_from_name("label_down"), " " ) ;

    D9(cerr<<"D9 refresh down label\n")
}

GtkLabel * label_from_name ( const char * name )
{
    D2(cerr<<"D2 label from name\n")

    return GTK_LABEL(gtk_builder_get_object(builder, name ) ) ;

    D9(cerr<<"D9 label from name\n")
}

GtkWidget * widget_from_name ( const char * name )
{
    D2(cerr<<"D2 widget from name\n")

    return GTK_WIDGET(gtk_builder_get_object(builder, name ) ) ;

    D9(cerr<<"D9 widget from name\n")
}

GtkToggleButton * tb_from_name ( const char * name )
{
    D2(cerr<<"D2 togglebutton from name\n")

    return GTK_TOGGLE_BUTTON ( gtk_builder_get_object ( builder, name ) ) ;

    D9(cerr<<"D9 togglebutton from name\n")
}

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data )
{
    D1(cerr<<"D1 handler button pressed event\n")

    /*
     * widget       -> the object which received the signal
     */
    if      ( widget == (widget_from_name ( "new" ) ) )
    {
        display_empty() ;
        GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
        gtk_widget_show ( w_new_game ) ;
	}
    else if ( widget == (widget_from_name ( "mix" ) ) )
    {
        if ((playing)&&( play_ground == tiles )&&(!lock_mix))
        {
            if ( !mix_cube () )
                return TRUE ;
            else
            {
                check_cube() ;
                refresh_unlocked() ;
                if ( !sort_unlocked() )
                    return TRUE ;
                redraw_widget ( "playground" ) ;
                lock_undo = true ;
                lock_mix = true ;
            }
        }
        else if ( lock_mix )
        {
            gtk_label_set_text ( label_from_name("label_down"), "mix not allowed just after a mix and vs ai different from airhead" ) ;
        }
	}
    else if ( widget == (widget_from_name ( "undo" ) ) )
    {
        if ((playing)&&( play_ground == tiles )&&(!lock_undo))
        {
            undo_last_two_couples() ;
            clear_pair_removed () ;
            refresh_scores(_score1, _score2) ;
        }
        else if ( lock_undo )
        {
            gtk_label_set_text ( label_from_name("label_down"), "undo not allowed after mix and vs thoughtful ai" ) ;
        }
	}
    else if ( widget == (widget_from_name ( "tip" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
            tile * a = NULL ;
            tile * b = NULL ;
            bool do_not_use = false ;
            if ( airhead_extraction ( a, b, do_not_use ) )
            {
                find_coord ( a->num, h_x1, h_y1, h_z1 ) ;
                find_coord ( b->num, h_x2, h_y2, h_z2 ) ;
                redraw_widget ( "playground" ) ;
            }
        }
	}
    else if ( widget == (widget_from_name ( "rules" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
            display_rules();
        else if ((playing)&&( play_ground == rules ))
            display_tiles() ;

        if ( !playing )
            display_empty() ;
	}
    else if ( widget == (widget_from_name ( "load" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
        }
	}
    else if ( widget == (widget_from_name ( "save" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
        }
	}
    else if ( widget == (widget_from_name ( "exit" ) ) )
    {
	}

    return TRUE ;

    D10(cerr<<"D10 handler button pressed event\n")
}

void redraw_widget ( const char * name )
{
    D2(cerr<<"D2 redraw widget\n")

        GtkWidget * _widget = widget_from_name ( name ) ;

        int width   = 0 ;
        int height  = 0 ;
        width   = gtk_widget_get_allocated_width ( _widget ) ;
        height  = gtk_widget_get_allocated_height ( _widget ) ;

        gtk_widget_queue_draw_area( _widget, 0, 0, width, height ) ;

    D9(cerr<<"D9 redraw widget\n")
}


void draw_number_on_tile ( cairo_t * &cr_tile,
                           cairo_surface_t * &_obj,
                           const int &num )
{
    D2(cerr<<"D2 draw number on tile\n")

    bool point[10] ;
    #define P(a) point[(a)]=true ;
    /*
    * l'array point setta i punti da disegnare
    * _________
    * |0---1---2|
    * |----9----|
    * |3---4---5|
    * |---------|
    * |6---7---8|
    * ^^^^^^^^^
    */
    for ( int i=0 ; i<10 ; i++ )
        point[i]=false ;
    switch ( num )
    {
        case 1: P(4) ;
                break ;
        case 2: P(0) ; P(8) ;
                break ;
        case 3: P(0) ; P(4) ; P(8) ;
                break ;
        case 4: P(0) ; P(2) ; P(6) ; P(8) ;
                break ;
        case 5: P(0) ; P(2) ; P(4) ; P(6) ; P(8) ;
                break ;
        case 6: P(0) ; P(2) ; P(3) ; P(5) ; P(6) ; P(8) ;
                break ;
        case 7: P(0) ; P(2) ; P(3) ; P(5) ; P(6) ; P(8) ; P(9) ;
                break ;
        case 8: P(0) ; P(1) ; P(2) ; P(3) ; P(5) ; P(6) ; P(7) ; P(8) ;
                break ;
        case 9: P(0) ; P(1) ; P(2) ; P(3) ; P(4) ; P(5) ; P(6) ; P(7) ; P(8) ;
                break ;
    } ;

 	if (point[0])
        cairo_set_source_surface ( cr_tile, _obj, 4, 4 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[1])
        cairo_set_source_surface ( cr_tile, _obj, 19, 4 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[2])
        cairo_set_source_surface ( cr_tile, _obj, 34, 4 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[3])
        cairo_set_source_surface ( cr_tile, _obj, 4, 19 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[4])
        cairo_set_source_surface ( cr_tile, _obj, 19, 19 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[5])
        cairo_set_source_surface ( cr_tile, _obj, 34, 19 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[6])
        cairo_set_source_surface ( cr_tile, _obj, 4, 34 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[7])
        cairo_set_source_surface ( cr_tile, _obj, 19, 34 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[8])
        cairo_set_source_surface ( cr_tile, _obj, 34, 34 ) ;
        cairo_paint ( cr_tile ) ;
 	if (point[9])
        cairo_set_source_surface ( cr_tile, _obj, 19, 11 ) ;
        cairo_paint ( cr_tile ) ;

    D9(cerr<<"D9 draw number on tile\n")
}

cairo_surface_t * number_on_tile (  cairo_surface_t * _obj,
                                    const int &number )
{
    D2(cerr<<"D2 number on tile\n")

    cairo_surface_t * _surf_50x50 = cairo_image_surface_create
                                    ( CAIRO_FORMAT_RGB24, 50, 50 ) ;

    cairo_t * cr_tile = cairo_create ( _surf_50x50 ) ;

    cairo_set_source_rgb( cr_tile, peach.r , peach.g , peach .b ) ;
    cairo_paint( cr_tile ) ;
 
    draw_number_on_tile( cr_tile, _obj, number ) ;

    cairo_destroy ( cr_tile ) ;

    return _surf_50x50 ;
    /*la surface la dealloco nella funzione chiamante*/

    D9(cerr<<"D9 number on tile\n")
}

int number_from_string ( const char * word )
{
    D2(cerr<<"D2 number from string\n")

    int i = 0 ;
    while ( word[i]!= '\0' ) 
        i++;

    const char output = word[i-1];

    return (output-'0') ;

    D9(cerr<<"D9 number from string\n")
}

void sub_paint_tile ( cairo_t * &context, cairo_surface_t * &surf1, cairo_surface_t * &surf2,
                      const int &num, const char * &image_name, const tile_type &tt )
{
    surf1 = cairo_image_surface_create_from_png ( image_name ) ;
    if ( tt == tt_number )
    {
        surf2 = number_on_tile ( surf1, number_from_string ( name[num].word) ) ;
        cairo_set_source_surface ( context, surf2, 11, 1 ) ;
    }
    else if ( tt == tt_image )
    {
        cairo_set_source_surface ( context, surf1, 10, 0 ) ;
    }
    else
    /*debug this case*/ ;

    cairo_paint( context ) ;
    cairo_surface_destroy ( surf1 ) ;
    surf1 = NULL ;
    cairo_surface_destroy ( surf2 ) ;
    surf2 = NULL ;
}

cairo_surface_t * paint_tile (  const int &num,
                                const int &x,
                                const int &y,
                                const int &z )
{
    D2(cerr<<"D2 paint tile\n")

    cairo_surface_t * surface ;
    cairo_t * context ;

    surface = cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, 62, 62 ) ;
    context = cairo_create ( surface ) ;

    cairo_surface_t * _surface_tile = cairo_image_surface_create_from_png ( I_TILE ) ;
    cairo_set_source_surface ( context, _surface_tile , 0, 0 ) ;

    cairo_surface_destroy ( _surface_tile ) ;

    cairo_paint ( context ) ;

    cairo_surface_t * temp = NULL ;
    cairo_surface_t * _surface_from_png = NULL ;
    if (num<=35)
    {
        sub_paint_tile( context, _surface_from_png, temp, num, I_CIRCLE, tt_number ) ;
    }
    else if (num<=71)
    {
        sub_paint_tile( context, _surface_from_png, temp, num, I_BAMBOO, tt_number ) ;
    }
    else if (num<=107)
 	{
        sub_paint_tile( context, _surface_from_png, temp, num, I_CROSS, tt_number ) ;
    }
    else if(num<=123)
    {
        const int wind = (num-108)%4 ; /*0==east 1==sud 2==west 3==north*/
        switch (wind)
        {
            case 0: sub_paint_tile( context, _surface_from_png, temp, num, I_EAST, tt_image ) ;
                    break ;
            case 1: sub_paint_tile( context, _surface_from_png, temp, num, I_SUD, tt_image ) ;
                    break ;
            case 2: sub_paint_tile( context, _surface_from_png, temp, num, I_WEST, tt_image ) ;
                    break ;
            case 3: sub_paint_tile( context, _surface_from_png, temp, num, I_NORTH, tt_image ) ;
                    break ;
        };
    }
    else if (num<=135)
    {
        const int dragon = (num-124)%3 /*0==red 1==green 2==white*/;
        switch (dragon)
        {
            case 0: sub_paint_tile( context, _surface_from_png, temp, num, I_RED_DRAGON, tt_image ) ;
                    break ;
            case 1: sub_paint_tile( context, _surface_from_png, temp, num, I_GREEN_DRAGON, tt_image ) ;
                    break ;
            case 2: sub_paint_tile( context, _surface_from_png, temp, num, I_WHITE_DRAGON, tt_image ) ;
                    break ;
        };
    }
    else if (num<=143)
    {
        switch (num)
        {
            case 136:/*spring*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_SPRING, tt_image ) ;
                     break ;
            case 137:/*summer*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_SUMMER, tt_image ) ;
                     break ;
            case 138:/*autumn*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_AUTUMN, tt_image ) ;
                     break ;
            case 139:/*winter*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_WINTER, tt_image ) ;
                     break ;
            case 140:/*plumb*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_PLUMB, tt_image ) ;
                     break ;
            case 141:/*orchid*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_ORCHID, tt_image ) ;
                     break ;
            case 142:/*chrysantemum*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_CHRYSANTEMUM, tt_image ) ;
                     break ;
            case 143:/*bamboo_forest*/
                     sub_paint_tile( context, _surface_from_png, temp, num, I_BAMBOO_FOREST, tt_image ) ;
                     break ;
        }
    } else if (( num == TILES )||( num == TILES+1 ))
    {
        sub_paint_tile( context, _surface_from_png, temp, num, I_DUMMY, tt_image ) ;
    }

    if ( temp != NULL )
        cairo_surface_destroy ( temp ) ;
    if ( _surface_from_png != NULL )
        cairo_surface_destroy ( _surface_from_png ) ;



    if ( ( ( h_x1 == x )&&( h_y1 == y )&&( h_z1 == z ) ) ||
         ( ( h_x2 == x )&&( h_y2 == y )&&( h_z2 == z ) )
       )
    {
        cairo_set_line_width ( context, 1.5 ) ;
        cairo_set_source_rgb ( context, ferrari.r, ferrari.g, ferrari.b ) ;
        cairo_rectangle ( context, 13, 3, 46, 46 ) ;
        cairo_stroke ( context ) ;
    }

    cairo_destroy ( context ) ;

    return ( surface ) ;
    /* la surface la dealloco nella funzione chiamante*/

    D9(cerr<<"D9 paint tile\n")
}

void calculate_coor_x_y ( const int &x,
                          const int &y,
                          const int &z,
                          int &_dx,
                          int &_dy )
{
    D2(cerr<<"D2 calculate coor x y\n")

    _dx = 24 ;
    _dy = 50 ;
    _dx += x*51 ;
    _dy += y*51 ;
    _dx += z*10 ;
    _dy -= z*10 ;

    D9(cerr<<"D9 calculate coor x y\n")
}

extern "C" gboolean draw_removed_tiles  ( GtkWidget * widget,
                                          cairo_t * cr,
                                          gpointer user_data )
{
    D2(cerr<<"D2 draw removed tiles\n")

    cairo_surface_t* tile = NULL ;

    /*nota: 112 e' la larghezza di due tessere vicine*/
    int border_x = (gtk_widget_get_allocated_width ( widget ) - 112 )/2 ;
    int border_y = 50 ;

    if ( ( last_removed_pl1_a >= 0 ) && ( last_removed_pl1_a < TILES ) &&
         ( last_removed_pl1_b >= 0 ) && ( last_removed_pl1_b < TILES )    )
    {
        // removed by player 1

        tile = paint_tile ( last_removed_pl1_b, -2, -2, -2 ) ;
        cairo_set_source_surface ( cr, tile, border_x+51, border_y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;

        tile = paint_tile ( last_removed_pl1_a, -2, -2, -2 ) ;
        cairo_set_source_surface ( cr, tile, border_x, border_y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;
    }
    if ( ( last_removed_pl2_a >= 0 ) && ( last_removed_pl2_a < TILES ) &&
         ( last_removed_pl2_b >= 0 ) && ( last_removed_pl2_b < TILES )    )
    {
        int y = ( gtk_widget_get_allocated_height ( widget ) / 2 ) ;
        // removed by ai or player 2
        tile = paint_tile ( last_removed_pl2_b, -2, -2, -2 ) ;
        cairo_set_source_surface ( cr, tile, border_x+51, y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;

        tile = paint_tile ( last_removed_pl2_a, -2, -2, -2 ) ;
        cairo_set_source_surface ( cr, tile, border_x, y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;
    }
    return TRUE ;

    D9(cerr<<"D9 draw removed tiles\n")
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data )
{
    D2(cerr<<"D2 draw play ground\n")

    cairo_set_source_rgb(cr, green_forest.r, green_forest.g, green_forest.b ) ;
    cairo_paint(cr) ;

    switch ( play_ground )
    {
        case empty:
                    cairo_set_source_rgb(cr, sand.r, sand.g, sand.b ) ;
                    cairo_paint(cr) ;
                    break ;
        case rules:
                    {
                        cairo_set_source_rgb(cr, light_blue.r, light_blue.g, light_blue.b ) ;
                        cairo_paint(cr) ;

                        cairo_surface_t * surface ;
	                    cairo_t * cont ;
	                    char line_1[]="Mahjong rules";
	                    char line_2[]="Prima riga";
	                    char line_3[]="Seconda riga";
	                    cairo_font_extents_t fe;
	                    cairo_text_extents_t te;

	                    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, gtk_widget_get_allocated_width ( widget ), gtk_widget_get_allocated_height ( widget ));
	                    cont = cairo_create (surface);

	                    cairo_set_font_size (cont, 28);
	                    cairo_select_font_face (cont, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
	                    cairo_font_extents (cont, &fe);
	                    cairo_text_extents (cont, line_1, &te);

	                    cairo_move_to (cont, 252, 50);
	                    cairo_set_source_rgb(cont, light_orange.r, light_orange.g, light_orange.b ) ;
	                    cairo_show_text (cont, line_1);

	                    cairo_set_font_size (cont, 18);
	                    cairo_move_to (cont, 60, 80);
	                    cairo_text_extents (cont, line_2, &te);
	                    cairo_show_text (cont, line_2);

	                    cairo_set_font_size (cont, 18);
	                    cairo_move_to (cont, 60, 110);
	                    cairo_text_extents (cont, line_3, &te);
	                    cairo_show_text (cont, line_3);

                        cairo_set_source_surface ( cr, surface, 0, 0 ) ;
                        cairo_paint(cr);

	                    cairo_destroy (cont);
	                    cairo_surface_destroy (surface);
                    }
                    break ;
        case tiles:
                    for (int z = 0 ; z < 4 ; z++ )
                        for ( int y = 0 ; y < 8 ; y++ )
                            for ( int x = 11 ; x >= 0 ; x-- )
                            {
                                if (cube[x][y][z].empty)
                                    continue ;

                                cairo_surface_t* tile=paint_tile(cube[x][y][z].num,
                                                                      x, y, z ) ;
                                int coor_x, coor_y ;

                                calculate_coor_x_y ( x, y, z, coor_x, coor_y ) ;

                                if ( cube[x][y][z].num < TILES )
                                    set_coor_tile ( &cube[x][y][z], coor_x, coor_y ) ;

                                cairo_set_source_surface(cr, tile, coor_x, coor_y);
                                cairo_paint (cr) ;

                                cairo_surface_destroy ( tile ) ;
                            }
                    break ;
        case end:
                    const int score_1 = _score1 ;
                    const int score_2 = _score2 ;
                    if (( score_1 > score_2 )&&( mode == h_c ))
                    {
                        /* Player 1 won against ai */
                        cairo_set_source_rgb(cr, gold.r, gold.g, gold.b ) ;
                        cairo_paint(cr) ;
/*scrivere in carmine*/
                    }
                    else if (( score_1 < score_2 )&&( mode == h_c ))
                    {
                        /* Player 1 lost against ai */
                        cairo_set_source_rgb(cr, coffe.r, coffe.g, coffe.b ) ;
                        cairo_paint(cr) ;
/*scrivere in white*/
                    }
                    else if (( score_1 == score_2 )&&( mode == h_c ))
                    {
                        /* Player 1 equalized ai */
                        cairo_set_source_rgb(cr, lapislazuli.r, lapislazuli.g, lapislazuli.b ) ;
                        cairo_paint(cr) ;
/*scrivere in gold*/
                    }
                    else if (( score_1 > score_2 )&&( mode == h_h ))
                    {
                        /* Player 1 won against Player 2 */
                        cairo_set_source_rgb(cr, ferrari.r, ferrari.g, ferrari.b ) ;
                        cairo_paint(cr) ;
/*scrivere in peach*/
                    }
                    else if (( score_1 < score_2 )&&( mode == h_h ))
                    {
                        /* Player 1 lost against Player 2 */
                        cairo_set_source_rgb(cr, carmine.r, carmine.g, carmine.b ) ;
                        cairo_paint(cr) ;
/*scrivere in bianco*/
                    }
                    else if (( score_1 == score_2 )&&( mode == h_h ))
                    {
                        /* Player 1 equalized Player 2 */
                        cairo_set_source_rgb(cr, blue_persia.r, blue_persia.g, blue_persia.b ) ;
                        cairo_paint(cr) ;
/*scrivere in yellow*/
                    }
/*disegnare winner loser*/
                    break ;
    }
    return TRUE ;

    D9(cerr<<"D9 draw play ground\n")
}

void set_coor_tile ( tile * punt, const int &x, const int &y )
{
    D2(cerr<<"D2 set coor tile\n")

    punt->x1 = x + 10 ;
    punt->y1 = y ;

    punt->x2 = x + 10 + 50 ;
    punt->y2 = y + 50 ;

    D9(cerr<<"D9 set coor tile\n")
}
