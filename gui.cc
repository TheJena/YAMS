#include <iostream>
#include <gtk/gtk.h>
#include <cstdlib>
#include <ctime>
#include "data_structure.h"
#include "gui.h"
#include "initializations_operations.h"

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

const char* W_newgame           = "window_new_game" ;

GtkBuilder * builder ;

colour green_forest = { 34.0/255, 139.0/255, 34.0/255 } ;
colour peach = { 255.0/255, 229.0/255, 180.0/255 } ;


extern "C" gboolean handler_delete_event ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)  
{
	if ( cube != NULL )
		delete_cube () ;

/*deallocare correttamente a chiusura applicazione
  array 2d con nomi tessere*/
    gtk_main_quit() ;
    cout<<"programma terminato regolarmente\n" ;
    return TRUE ;
}

extern "C" gboolean handler_hide_window ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;
    return TRUE ;
}

extern "C" gboolean handler_set_new_game ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
/*handler per rendere effettive le disposizioni per la nuova partita*/
    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_easy" ) ) )
        level = easy ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_medium" ) ) )
        level = medium ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_difficult" ) ) )
        level = difficult ;

    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_h-c" ) ) )
        /*modalita' umano vs computer*/ ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_h-h" ) ) )
        /*modalita' umano vs umano*/ ;

    if      ( gtk_toggle_button_get_active ( tb_from_name ( "rb_airhead" ) ) )
        ai = airhead ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_greedy" ) ) )
        ai = greedy ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_thoughtful") ) )
        ai = thoughtful ;

    initialize_cube() ;
    fill_cube() ;
    check_cube() ;
    unlocked[0] = NULL ;
    refresh_unlocked() ;

    redraw_widget ( "playground" ) ;

/*alla fine nascondo la finestra*/
    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;
    return TRUE ;
}

/*
 * Funzione che ritorna un puntatore a widget
 * dato in input il nome del widget
 */
GtkWidget * widget_from_name ( const char * name )
{
    return GTK_WIDGET(gtk_builder_get_object(builder, name ) ) ;
}

/*
 * Funzione che ritorna un puntatore a toggle button
 * dato in input il nome del togglebutton
 */
GtkToggleButton * tb_from_name ( const char * name )
{
    return GTK_TOGGLE_BUTTON ( gtk_builder_get_object ( builder, name ) ) ;
}

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data )
{
    /*
     * widget       -> the object which received the signal
     */
    if      ( widget == (widget_from_name ( "new" ) ) )
    {
        GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
        gtk_widget_show ( w_new_game ) ;
	}
    else if ( widget == (widget_from_name ( "mix" ) ) )
    {
        mix_cube () ;
        redraw_widget ( "playground" ) ;
	}
    else if ( widget == (widget_from_name ( "undo" ) ) )
    {
		cerr<<"undo pressed"<<endl ;
	}
    else if ( widget == (widget_from_name ( "redo" ) ) )
    {
		cerr<<"redo pressed"<<endl ;
	}
    else if ( widget == (widget_from_name ( "load" ) ) )
    {
		cerr<<"load pressed"<<endl ;
	}
    else if ( widget == (widget_from_name ( "save" ) ) )
    {
		cerr<<"save pressed"<<endl ;
	}
    else if ( widget == (widget_from_name ( "exit" ) ) )
    {
		cerr<<"exit pressed"<<endl ;
	}

    return TRUE ;
}

/*
 * Funzione che ridisegna il widget passato per nome
 */
void redraw_widget ( const char * name )
{
        GtkWidget * _widget = widget_from_name ( name ) ;

        int width   = 0 ;
        int height  = 0 ;
        width   = gtk_widget_get_allocated_width ( _widget ) ;
        height  = gtk_widget_get_allocated_height ( _widget ) ;

        gtk_widget_queue_draw_area( _widget, 0, 0, width, height ) ;
}


void draw_number_on_tile ( cairo_t * &cr_tile,
                           cairo_surface_t * &_obj,
                           const int &num )
{

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
}

cairo_surface_t * number_on_tile ( cairo_surface_t * _obj,
                                   const int &number )
{
    cairo_surface_t * _surf_50x50 = cairo_image_surface_create
                                    ( CAIRO_FORMAT_RGB24, 50, 50 ) ;

    cairo_t * cr_tile = cairo_create ( _surf_50x50 ) ;

    cairo_set_source_rgb( cr_tile, peach.r , peach.g , peach .b ) ;
    cairo_paint( cr_tile ) ;
 
    draw_number_on_tile( cr_tile, _obj, number ) ;

    return _surf_50x50 ;
}

int number_from_string ( const char * word )
{
    int i = 0 ;
    while ( word[i]!= '\0' ) 
        i++;

    const char output = word[i-1];

    return (output-'0') ;
}

cairo_surface_t * paint_tile ( const int &num,
                               const int &x,
                               const int &y,
                               const int &z )
{
    cairo_surface_t * surface ;
    cairo_t * context ;

    surface = cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, 62, 62 ) ;
    context = cairo_create ( surface ) ;

    cairo_set_source_surface ( context,
                               cairo_image_surface_create_from_png ( I_TILE ),
                               0,
                               0
                             ) ;
    cairo_paint ( context ) ;

    cairo_surface_t * temp ;
    if (num<=35)
    {
        temp = number_on_tile ( cairo_image_surface_create_from_png ( I_CIRCLE ),
                                number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if (num<=71)
    {
        temp = number_on_tile ( cairo_image_surface_create_from_png ( I_BAMBOO ),
                                number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if (num<=107)
 	{
        temp = number_on_tile ( cairo_image_surface_create_from_png ( I_CROSS ),
                                number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if(num<=123)
    {
        const int wind = (num-108)%4 ; /*0==east 1==sud 2==west 3==north*/
        switch (wind)
        {
            case 0: cairo_set_source_surface
                    ( context, cairo_image_surface_create_from_png ( I_EAST ),
                      10, 0
                    ) ;
                    break ;
            case 1: cairo_set_source_surface
                    ( context, cairo_image_surface_create_from_png ( I_SUD ),
                      10, 0 
                    ) ;
                    break ;
            case 2: cairo_set_source_surface
                    ( context, cairo_image_surface_create_from_png ( I_WEST ),
                      10, 0
                    ) ;
                    break ;
            case 3: cairo_set_source_surface
                    ( context, cairo_image_surface_create_from_png ( I_NORTH ),
                      10, 0
                    ) ;
                    break ;
        };
    }
    else if (num<=135)
    {
        const int dragon = (num-124)%3 /*0==red 1==green 2==white*/;
        switch (dragon)
        {
            case 0: cairo_set_source_surface
                    ( context,
                      cairo_image_surface_create_from_png ( I_RED_DRAGON ),
                      10,
                      0
                    ) ;
                    break ;
            case 1: cairo_set_source_surface
                    ( context, 
                      cairo_image_surface_create_from_png ( I_GREEN_DRAGON ),
                      10,
                      0
                    ) ;
                    break ;
            case 2: cairo_set_source_surface
                    ( context,
                      cairo_image_surface_create_from_png ( I_WHITE_DRAGON ),
                      10,
                      0
                    ) ;
                    break ;
        };
    }
    else if (num<=143)
    {
        switch (num)
        {
            case 136:/*spring*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_SPRING ),
                       10,
                       0
                     ) ;
                     break ;
            case 137:/*summer*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_SUMMER ),
                       10,
                       0
                     ) ;
                     break ;
            case 138:/*autumn*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_AUTUMN ),
                       10,
                       0
                     ) ;
                     break ;
            case 139:/*winter*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_WINTER ),
                       10,
                       0
                     ) ;
                     break ;
            case 140:/*plumb*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_PLUMB ),
                       10,
                       0
                     ) ;
                     break ;
            case 141:/*orchid*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_ORCHID ),
                       10,
                       0
                     ) ;
                     break ;
            case 142:/*chrysantemum*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_CHRYSANTEMUM ),
                       10,
                       0
                     ) ;
                     break ;
            case 143:/*bamboo_forest*/
                     cairo_set_source_surface
                     ( context,
                       cairo_image_surface_create_from_png ( I_BAMBOO_FOREST ),
                       10,
                       0
                     ) ;
                     break ;
        }
    }

    cairo_paint ( context ) ;

    return ( surface ) ;
}

void calculate_coor_x_y ( const int &x,
                          const int &y,
                          const int &z,
                          int &_dx,
                          int &_dy )
{
    _dx = 50 ;
    _dy = 50 ;
    _dx += x*50 ;
    _dy += y*50 ;
    _dx += z*10 ;
    _dy -= z*10 ;
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data )
{
    cairo_set_source_rgb(cr, green_forest.r, green_forest.g, green_forest.b ) ;
    cairo_paint(cr) ;

    for (int z = 0 ; z < 4 ; z++ )
        for ( int y = 0 ; y < 8 ; y++ )
            for ( int x = 11 ; x >= 0 ; x-- )
            {
                if (cube[x][y][z].empty)
                    continue ;

                cairo_surface_t * tile = paint_tile ( cube[x][y][z].num,
                                                      x, y, z ) ;
                int coor_x, coor_y ;
                calculate_coor_x_y ( x, y, z, coor_x, coor_y ) ;
                cairo_set_source_surface ( cr, tile, coor_x, coor_y ) ;
                cairo_paint (cr) ;
            }
    return TRUE ;
}
