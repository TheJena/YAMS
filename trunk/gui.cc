#include <fstream>
#include <iostream>
#include <gtk/gtk.h>
#include <ctime>
#include <cstdlib>
#include "data_structure.h"
#include "gui.h"
#include "initializations_operations.h"
#include "io_file.h"


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

enum event_box { empty, rules, tiles, end } play_ground ;

GtkBuilder * builder ;

colour green_forest = { 34.0/255, 139.0/255, 34.0/255 } ;
colour peach = { 255.0/255, 229.0/255, 180.0/255 } ;
colour black = { 0.0/255, 0.0/255, 0.0/255 } ;
colour red = { 255.0/255, 0.0/255, 0.0/255 } ;

int h_x1 = -1 ;
int h_y1 = -1 ;
int h_z1 = -1 ;

int h_x2 = -1 ;
int h_y2 = -1 ;
int h_z2 = -1 ;

void display_end ()
{
    play_ground = end ;
    redraw_widget ("playground") ;
}

void display_empty ()
{
    play_ground = empty ;
    redraw_widget ("playground") ;
}

void display_tiles ()
{
    play_ground = tiles ;
    redraw_widget ("playground") ;
}

void display_rules ()
{
    play_ground = rules ;
    redraw_widget ("playground") ;
}

void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z )
{
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
}

void reset_highlighted_cell ()
{
    h_x1 = -1 ;
    h_y1 = -1 ;
    h_z1 = -1 ;

    h_x2 = -1 ;
    h_y2 = -1 ;
    h_z2 = -1 ;
}

bool check_position ( const tile * punt, const int &x, const int &y )
{
    if ( ( punt->x1 ) >= x )
        return false ;
    if ( ( punt->y1 ) >= y )
        return false ;

    if ( ( punt->x2 ) <= x )
        return false ;
    if ( ( punt->y2 ) <= y )
        return false ;

    return true ;

}
extern "C" gboolean handler_click_on_widget (GtkWidget *widget,
               GdkEventButton  *event,
               gpointer   user_data)
{
    const int _x = event->x ;
    const int _y = event->y ;

    bool quit = false ;
    for ( int z = dim_Z-1 ; (z >= 0)&&(!quit) ; z-- )
        for ( int y = 0 ; (y < dim_Y)&&(!quit) ; y++ )
            for ( int x = 0 ; (x < dim_X)&&(!quit) ; x++ )
            {

                if ( ( !cube[x][y][z].empty )&&( !cube[x][y][z].lock ) )
                {
                    if ( check_position( &cube[x][y][z], _x, _y ) )
                    {
                        insert_half_pair( cube[x][y][z].num, x, y, z ) ;


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
}


extern "C" gboolean handler_delete_event ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)  
{
	if ( cube != NULL )
		delete_cube () ;
    if ( name != NULL )
        delete_tiles_names () ;
    if ( playing )
        end_game() ;

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

    if ( playing )
        display_tiles() ;
    else
        display_empty() ;

    return TRUE ;
}

extern "C" gboolean handler_set_new_game ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
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
        ai = airhead ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_greedy" ) ) )
        ai = greedy ;
    else if ( gtk_toggle_button_get_active ( tb_from_name ( "rb_thoughtful") ) )
        ai = thoughtful ;

    start_game () ;

/*alla fine nascondo la finestra*/
    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;

    display_tiles() ;

    return TRUE ;
}

void refresh_down_label ( const int & couples )
{
    if ((playing)&&( play_ground == tiles ))
    {
        char before[] = "Sono rimaste " ;
        char after[] = " coppie rimuovibili" ;

        if ( couples == 1 )
        {
            char b[] = "E' rimasta " ;
            char a[] = " coppia disponibile" ;
            int x, y ;
            for ( x = 0 ; b[x] != '\0' ; x++ )
                before[x]= b[x] ;
            before[x] = '\0';
            for ( y = 0 ; a[y] != '\0' ; y++ )
                after[y]= a[y] ;
            after[y] = '\0';
        }
        char sum[100] ;

        sprintf ( sum, "%s %d %s", before, couples, after ) ;

        gtk_label_set_text ( label_from_name("label_down"), sum ) ;
    }
    else
    gtk_label_set_text ( label_from_name("label_down"), " " ) ;

}

/*
 * Funzione che ritorna un puntatore a label
 * dato in input il nome della label
 */
GtkLabel * label_from_name ( const char * name )
{
    return GTK_LABEL(gtk_builder_get_object(builder, name ) ) ;
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
        display_empty() ;
        GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
        gtk_widget_show ( w_new_game ) ;
	}
    else if ( widget == (widget_from_name ( "mix" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
            mix_cube () ;
            check_cube() ;
            refresh_unlocked() ;
            sort_unlocked() ;
            redraw_widget ( "playground" ) ;
        }
	}
    else if ( widget == (widget_from_name ( "undo" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
	    	cerr<<"undo pressed"<<endl ;
            undo_last_two_couples() ;
        }
	}
    else if ( widget == (widget_from_name ( "tip" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
cerr<<"tip pressed"<<endl ;
            tile * a = NULL ;
            tile * b = NULL ;
            bool do_not_use = false ;
            airhead_extraction ( a, b, do_not_use ) ;
            find_coord ( a->num, h_x1, h_y1, h_z1 ) ;
            find_coord ( b->num, h_x2, h_y2, h_z2 ) ;
            redraw_widget ( "playground" ) ;
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

		cerr<<"rules pressed"<<endl ;
	}
    else if ( widget == (widget_from_name ( "load" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
cerr<<"load pressed"<<endl ;

        }
	}
    else if ( widget == (widget_from_name ( "save" ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
cerr<<"save pressed"<<endl ;
        }
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

cairo_surface_t * number_on_tile (  cairo_surface_t * _obj,
                                    const int &number )
{
    cairo_surface_t * _surf_50x50 = cairo_image_surface_create
                                    ( CAIRO_FORMAT_RGB24, 50, 50 ) ;

    cairo_t * cr_tile = cairo_create ( _surf_50x50 ) ;

    cairo_set_source_rgb( cr_tile, peach.r , peach.g , peach .b ) ;
    cairo_paint( cr_tile ) ;
 
    draw_number_on_tile( cr_tile, _obj, number ) ;

    cairo_destroy ( cr_tile ) ;

    return _surf_50x50 ;
    /*la surface la dealloco nella funzione chiamante*/
}

int number_from_string ( const char * word )
{
    int i = 0 ;
    while ( word[i]!= '\0' ) 
        i++;

    const char output = word[i-1];

    return (output-'0') ;
}

cairo_surface_t * paint_tile (  const int &num,
                                const int &x,
                                const int &y,
                                const int &z )
{
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
        _surface_from_png = cairo_image_surface_create_from_png ( I_CIRCLE ) ;
        temp = number_on_tile ( _surface_from_png, number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if (num<=71)
    {
        _surface_from_png = cairo_image_surface_create_from_png ( I_BAMBOO ) ;
        temp = number_on_tile ( _surface_from_png, number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if (num<=107)
 	{
        _surface_from_png = cairo_image_surface_create_from_png ( I_CROSS ) ;
        temp = number_on_tile ( _surface_from_png, number_from_string ( name[num].word ) ) ;
        cairo_set_source_surface ( context, temp, 11, 1 ) ;
    }
    else if(num<=123)
    {
        const int wind = (num-108)%4 ; /*0==east 1==sud 2==west 3==north*/
        switch (wind)
        {
            case 0: _surface_from_png = cairo_image_surface_create_from_png ( I_EAST ) ;
                    cairo_set_source_surface
                    ( context, _surface_from_png ,
                      10, 0
                    ) ;
                    break ;
            case 1: _surface_from_png = cairo_image_surface_create_from_png ( I_SUD ) ;
                    cairo_set_source_surface
                    ( context, _surface_from_png,
                      10, 0 
                    ) ;
                    break ;
            case 2: _surface_from_png = cairo_image_surface_create_from_png ( I_WEST ) ;
                    cairo_set_source_surface
                    ( context, _surface_from_png,
                      10, 0
                    ) ;
                    break ;
            case 3: _surface_from_png = cairo_image_surface_create_from_png ( I_NORTH ) ;
                    cairo_set_source_surface
                    ( context, _surface_from_png,
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
            case 0: _surface_from_png = cairo_image_surface_create_from_png ( I_RED_DRAGON ) ;
                    cairo_set_source_surface
                    ( context,
                      _surface_from_png,
                      10,
                      0
                    ) ;
                    break ;
            case 1: _surface_from_png = cairo_image_surface_create_from_png ( I_GREEN_DRAGON ) ;
                    cairo_set_source_surface
                    ( context, 
                      _surface_from_png,
                      10,
                      0
                    ) ;
                    break ;
            case 2: _surface_from_png = cairo_image_surface_create_from_png ( I_WHITE_DRAGON ) ;
                    cairo_set_source_surface
                    ( context,
                      _surface_from_png,
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
                     _surface_from_png = cairo_image_surface_create_from_png ( I_SPRING ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 137:/*summer*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_SUMMER ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 138:/*autumn*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_AUTUMN ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 139:/*winter*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_WINTER ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 140:/*plumb*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_PLUMB ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 141:/*orchid*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_ORCHID ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 142:/*chrysantemum*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_CHRYSANTEMUM ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
            case 143:/*bamboo_forest*/
                     _surface_from_png = cairo_image_surface_create_from_png ( I_BAMBOO_FOREST ) ;
                     cairo_set_source_surface
                     ( context,
                       _surface_from_png,
                       10,
                       0
                     ) ;
                     break ;
        }
    }

    cairo_paint ( context ) ;

    if ( temp != NULL )
        cairo_surface_destroy ( temp ) ;
    if ( _surface_from_png != NULL )
        cairo_surface_destroy ( _surface_from_png ) ;



    if ( ( ( h_x1 == x )&&( h_y1 == y )&&( h_z1 == z ) ) ||
         ( ( h_x2 == x )&&( h_y2 == y )&&( h_z2 == z ) )
       )
    {
        cairo_set_line_width ( context, 1.5 ) ;
        cairo_set_source_rgb ( context, red.r, red.g, red.b ) ;
        cairo_rectangle ( context, 13, 3, 46, 46 ) ;
        cairo_stroke ( context ) ;
    }

    cairo_destroy ( context ) ;

    return ( surface ) ;
    /* la surface la dealloco nella funzione chiamante*/
}

void calculate_coor_x_y ( const int &x,
                          const int &y,
                          const int &z,
                          int &_dx,
                          int &_dy )
{
    _dx = 24 ;
    _dy = 50 ;
    _dx += x*51 ;
    _dy += y*51 ;
    _dx += z*10 ;
    _dy -= z*10 ;
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data )
{
    cairo_set_source_rgb(cr, green_forest.r, green_forest.g, green_forest.b ) ;
    cairo_paint(cr) ;

    switch ( play_ground )
    {
        case empty:
                    break ;
        case rules:
/*disegnare regole*/
cerr<<"WE\n";
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

                                set_coor_tile ( &cube[x][y][z], coor_x, coor_y ) ;

                                cairo_set_source_surface(cr, tile, coor_x, coor_y);
                                cairo_paint (cr) ;

                                cairo_surface_destroy ( tile ) ;
                            }
                    break ;
        case end:
/*disegnare winner loser*/
                    break ;
    }
    return TRUE ;
}

void set_coor_tile ( tile * punt, const int &x, const int &y )
{
    punt->x1 = x + 10 ;
    punt->y1 = y ;

    punt->x2 = x + 10 + 50 ;
    punt->y2 = y + 50 ;
}
