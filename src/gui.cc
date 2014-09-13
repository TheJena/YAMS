/**
 * @file
 * File che contiene l'implementazione del modulo GUI. Qui vengono definite le
 * funzioni che operano sulla parte grafica dell' applicazione.
 */
/*inizio implementazione modulo_GUI*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "debug_macros.h"
#include "io_file.h"                // include gia' la definizione struct_couple
#include "gui.h"        //include gia le definizioni enum_layout e enum_p_player
#include "cube.h"                       //include gia la definizione struct_tile
#include "game.h"      // include gia' le definizioni struct__string e enum_game
#include "movements.h"                //include gia'le definizioni enum_computer

/**
 * Enumerato che identifica il tipo di tessera: numerica o dipinta.
 */
#define enum_tile_type
/**
 * Enumerato che identifica la schermata da stampare nell'area centrale della
 * finestra.
 */
#define enum_event_box
#include "data_structures.h"
#undef enum_tile_type
#undef enum_event_box

using namespace std ;

/*inizio implementazione struttura dati pubblica*/
layout level = easy ;
bool lock_undo = false ;
bool lock_mix = false ;
int last_removed_pl1_a = -1 ;     // removed by player 1
int last_removed_pl1_b = -1 ;     // removed by player 1
int last_removed_pl2_a = -1 ;     // removed by ai or player 2
int last_removed_pl2_b = -1 ;     // removed by ai or player 2
/*fine implementazione struttura dati pubblica*/

/*inizio struttura dati privata*/
/**
 * Struttura dati che memorizza un colore secondo le componenti Red Green Blue
 */
struct colour {
                double r ;
                double g ;
                double b ;
              } ;

static const char* I_AUTUMN            = "../img/autumn.png" ;
static const char* I_BAMBOO            = "../img/bamboo.png" ;
static const char* I_BAMBOO_FOREST     = "../img/bamboo_forest.png" ;
static const char* I_CHRYSANTEMUM      = "../img/chrysantemum.png" ;
static const char* I_CIRCLE            = "../img/circle.png" ;
static const char* I_CROSS             = "../img/cross.png" ;
static const char* I_EAST              = "../img/east.png" ;
static const char* I_GREEN_DRAGON      = "../img/green_dragon.png" ;
static const char* I_NORTH             = "../img/north.png" ;
static const char* I_ORCHID            = "../img/orchid.png" ;
static const char* I_PLUMB             = "../img/plumb.png" ;
static const char* I_RED_DRAGON        = "../img/red_dragon.png" ;
static const char* I_SPRING            = "../img/spring.png" ;
static const char* I_SUD               = "../img/sud.png" ;
static const char* I_SUMMER            = "../img/summer.png" ;
static const char* I_TILE              = "../img/tile_shape.png" ;
static const char* I_WEST              = "../img/west.png" ;
static const char* I_WHITE_DRAGON      = "../img/white_dragon.png" ;
static const char* I_WINTER            = "../img/winter.png" ;
static const char* I_DUMMY             = "../img/dummy.png" ;
static const char* F_GLADE             = "./gui.glade" ;
static const char* W_newgame           = "window_new_game" ;
static const char* W_savegame          = "window_save_game" ;
static const char* W_loadgame          = "window_load_game" ;

static const colour black =         { 0.0/255, 0.0/255, 0.0/255 } ;
static const colour blue_persia =   { 28.0/255, 57.0/255, 187.0/255 } ;
static const colour carmine =       { 150.0/255, 0.0/255, 24.0/255 } ;
static const colour coffe =         { 111.0/255, 78.0/255, 55.0/255 } ;
static const colour ferrari =       { 204.0/255, 0.0/255, 0.0/255 } ;
static const colour gold =          { 255.0/255, 215.0/255, 0.0/255 } ;
static const colour green_forest =  { 34.0/255, 139.0/255, 34.0/255 } ;
static const colour lapislazuli =   { 38.0/255, 97.0/255, 156.0/255 } ;
static const colour light_orange =  { 255.0/255, 153.0/255, 0.0/255 } ;
static const colour peach =         { 255.0/255, 229.0/255, 180.0/255 } ;
static const colour sand =          { 244.0/255, 164.0/255, 96.0/255 } ;
static const colour white =         { 255.0/255, 255.0/255, 255.0/255 } ;
static const colour yellow =        { 255.0/255, 255.0/255, 0.0/255 } ;

/**
 * Costante privata che identifica il numero massimo di caratteri di una
 * generica stringa.
 */
static const int MAXLUN = 100 ;

/**
 * Costante privata che identifica il numero massimo di caratteri di una
 * linea da stampare nell' area centrale dello schermo.
 */
static const int MAXLINE = 80 ;

/**
 * Variabile privata che indica il punteggio del primo giocatore.
 */
static int _score1 = 0 ;

/**
 * Variabile privata che indica il punteggio del secondo giocatore.
 */
static int _score2 = 0 ;

/**
 * Variabile privata che indica la coordinata x della prima cella da evidenziare.
 */
static int h_x1 = -1 ;

/**
 * Variabile privata che indica la coordinata y della prima cella da evidenziare.
 */
static int h_y1 = -1 ;

/**
 * Variabile privata che indica la coordinata z della prima cella da evidenziare.
 */
static int h_z1 = -1 ;

/**
 * Variabile privata che indica la coordinata x della seconda cella da evidenziare.
 */
static int h_x2 = -1 ;

/**
 * Variabile privata che indica la coordinata y della seconda cella da evidenziare.
 */
static int h_y2 = -1 ;

/**
 * Variabile privata che indica la coordinata z della seconda cella da evidenziare.
 */
static int h_z2 = -1 ;

static GtkBuilder * builder ;

/**
 * Variabile privata che indica cosa stampare al centro dello schermo.
 */
static event_box play_ground = rules ;
/*fine struttura dati privata*/

/*inizio prototipi funzioni handler*/
/**
 * Handler che disegna a lato le tessere rimosse
 */
extern "C" gboolean draw_removed_tiles  ( GtkWidget * widget,
                                          cairo_t * cr,
                                          gpointer user_data ) ;

/**
 * Handler che disegna il tavolo di gioco
 */
extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data ) ;

/**
 * Handler che nasconde la finestra genitore.
 */
extern "C" gboolean handler_hide_window ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data) ;

/**
 * Handler che chiama la funzione ::save_game.
 */
extern "C" gboolean handler_set_save_game ( GtkWidget * widget,
                                            GdkEvent * event,
                                            gpointer user_data) ;

/**
 * Handler che chiama la funzione ::load_game.
 */
extern "C" gboolean handler_set_load_game ( GtkWidget * widget,
                                            GdkEvent * event,
                                            gpointer user_data) ;

/**
 * Handler che riconosce quale widget lo ha chiamato ed agisce di conseguenza...
 * - avviando una nuova partita,
 * - mescolando le tessere,
 * - tornando indietro di una mossa,
 * - suggerendo una coppia removibile,
 * - mostrando le regole,
 * - caricando una partita da file,
 * - salvando una partita su file,
 * - aprendo il manuale / la documentazione,
 * - mostrando le generalita' del programma.
 */
extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data ) ;

/**
 * Handler che acquisisce le disposizioni per la nuova partita.
 */
extern "C" gboolean handler_set_new_game ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data) ;

/**
 * Handler che informa l' applicazione che un click e' stato rilasciato sulla
 * superficie di gioco.
 */
extern "C" gboolean handler_click_on_widget (GtkWidget *widget,
               GdkEventButton  *event,
               gpointer   user_data) ;

/**
 * Handler che predispone alla chiusura del programma.
 */
extern "C" gboolean handler_delete_event ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)  ;
/*fine prototipi funzioni handler*/

/*inizio prototipi funzioni private*/
/**
 * Funzione privata che disegna un testo sulla superficie di gioco.
 * @param[in] bg colore sfondo
 * @param[in] text_color colore testo
 * @param[out] widget widget chiamante e da ridisegnare
 * @param[out] cr contesto grafico di cairo
 * @param[in] title titolo del testo
 * @param[in] _line matrice contenente il testo
 * @param[in] h_title altezza titolo
 * @param[in] h_text altezza testo
 */
static void draw_text_on_play_ground ( const colour &bg, const colour &text_color,
                                GtkWidget * &widget, cairo_t * &cr,
                                char* title, char _line[][MAXLINE],
                                const int &h_title, const int &h_text      );

/**
 * Funzione privata che disegna su una tessera la disposizione del seme a
 * seconda del numero della tessera.
 * @param[out] cr contesto cairo
 * @param[in] _obj superficie da disegnare
 * @param[in] num numero in base al quale disporre le superfici affinche
 * disegnino il numero stesso. ( da 1 a 9 )
 */
static void draw_number_on_tile ( cairo_t * &cr_tile,
                           cairo_surface_t * &_obj,
                           const int &num ) ;

/**
 * Funzione privata intermedia per disegnare una tessera
 * @param[out] context contesto cairo
 * @param[out] surf1 superficie dove importare l'immagine della tessera vuota
 * @param[out] surf2 superficie dove importare la superficie ritornata dalla
 * funzione ::number_on_tile
 * @param[in] num numero identificativo della tessera
 * @param[in] image_name nome dell' immagine da porre sopra alla tessera vuota
 * @param[in] tt tipo di tessera: numerica o dipinta
 */
static void sub_paint_tile ( cairo_t * &context, cairo_surface_t * &surf1,
                      cairo_surface_t * &surf2, const int &num,
                      const char * &image_name, const tile_type &tt ) ;


/**
 * Funzione privata per disegnare una tessera
 * @param[in] num numero identificativo della tessera
 * @param[in] x, y, z coordinate della tessera
 * @return un puntatore a superficie
 */
static cairo_surface_t * paint_tile (  const int &num,
                                const int &x,
                                const int &y,
                                const int &z ) ;

/**
 * Funzione privata per disegnare un numero su una tessera
 * @param[out] _obj superficie su cui disegnare
 * @param[in] number numero identificativo della tessera
 * @return un puntatore a superficie
 */
static cairo_surface_t * number_on_tile (  cairo_surface_t * _obj,
                                    const int &number ) ;

static GtkEntry * entry_from_name ( const char * name ) ;

static GtkLabel * label_from_name ( const char * name ) ;

static GtkWidget * widget_from_name ( const char * name ) ;

static GtkToggleButton * tb_from_name ( const char * name ) ;

/**
 * Funzione privata che controlla se le coordinate passate in input sono interne
 * all' area della tessera passata per puntatore.
 * @param[in] punt puntatore alla tessera
 * @param[in] x, y coordinate da verificare
 * @return true se le coordinate sono interne alla faccia superiore della
 * tessera, false altrimenti.
 */
static bool check_position ( const tile * punt, const int &x, const int &y ) ;

/**
 * Funzione privata che estrae dal nome della tessera il numero di semi che
 * bisogna disegnarci sopra.
 * @param[in] word nome tessera
 * @return un numero da 1 a 9
 */
static int number_from_string ( const char * word ) ;

/**
 * Funzione privata per calcolare le coordinate del piano di gioco cui stampare
 * la tessera.
 * @param[in] _dx, dy distanze di offset dallo spigolo in alto a sinistra cui
 * disegnare la tessera
 * @param[in] x, y, z coordinate della tessera
 */
static void calculate_coor_x_y ( const int &x,
                          const int &y,
                          const int &z,
                          int &_dx,
                          int &_dy ) ;

/**
 * Funzione privata che assegna ad ogni tessera le coordinate dei vertici
 * opposti del quadrato che rappresenta la faccia superiore della tessera.
 * @param[in] punt puntatore alla tessera
 * @param[in] x, y coordinate vertici opposti faccia superiore tessera
 */
static void set_coor_tile ( tile * punt, const int &x, const int &y ) ;
/*fine prototipi funzioni private*/

/*inizio implementazione funzioni pubbliche*/
void call_gtk_main()
{
    D1(cerr<<"D1 call gtk main\n")

    gtk_main () ;

    D10(cerr<<"D10 call gtk main\n")
}

void call_gtk_main_quit()
{
    D1(cerr<<"D1 call gtk main quit\n")

    gtk_main_quit() ;

    D10(cerr<<"D10 call gtk main quit\n")
}

void initialize_gtk ( int argc, char * argv[] )
{
    D1(cerr<<"D1 initialize gtk\n")

    gtk_init ( &argc, &argv) ;
    builder = gtk_builder_new () ;
    gtk_builder_add_from_file ( builder, F_GLADE, NULL ) ;
    gtk_builder_connect_signals(builder, NULL);

    D10(cerr<<"D10 initialize gtk\n")
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

    _score1 = score1 ;
    _score2 = score2 ;

    D3(cerr<<"D3 refresh scores label"<<endl)
    D4(cerr<<"D4 _score1="<<_score1<<" "<<"_score2="<<_score2<<endl)

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

    D3(cerr<<"D3 refresh down label"<<endl)
    D4(cerr<<"D4 couples="<<couples<<endl)

    if ((playing)&&( play_ground == tiles ))
    {
        assert(playing==true);

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

void clear_pair_removed ()
{
    D2(cerr<<"D2 clear pair removed\n")

    refresh_pair_removed( p_human1, -1, -1 ) ;
    refresh_pair_removed( p_ai, -1, -1 ) ;

    D9(cerr<<"D9 clear pair removed\n")
}

void refresh_pair_removed( const p_player &temp, const int &a, const int &b )
{
    D2(cerr<<"D2 refresh pair removed\n")

    D3(cerr<<"D3 refresh pair removed"<<endl)
    D4(cerr<<"D4 temp="<<temp<<" a="<<a<<" b="<<b<<endl)

    if ( ( temp == p_ai ) || ( temp == p_human2 ) )
    {
        last_removed_pl2_a = a ;        // removed by ai or player 2
        last_removed_pl2_b = b ;        // removed by ai or player 2
    }
    else if ( temp == p_human1 )
    {
        last_removed_pl1_a = a ;        // removed by player 1
        last_removed_pl1_b = b ;        // removed by player 1
    }

    redraw_widget("drawingarea_right") ;

    D9(cerr<<"D9 refresh pair removed\n")
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

void display_end ()
{
    D1(cerr<<"D1 display end\n")

    refresh_scores() ;
    play_ground = end ;
    redraw_widget ("playground") ;

    D10(cerr<<"D10 display end\n")
}



void set_highlighted_cell ( const int &n, const int &x, const int &y, const int &z )
{
    D1(cerr<<"D1 set highlighted cell\n")

    D3(cerr<<"D3 highlighted cell"<<endl)
    D4(cerr<<"D4 n="<<n<<" x="<<x<<" y="<<y<<" z="<<z<<endl)

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
/*fine implementazione funzioni pubbliche*/

/*inizio implementazione funzioni handler*/
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
        assert(tile != NULL );
        cairo_set_source_surface ( cr, tile, border_x+51, border_y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;

        tile = paint_tile ( last_removed_pl1_a, -2, -2, -2 ) ;
        assert(tile != NULL );
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
        assert(tile != NULL );
        cairo_set_source_surface ( cr, tile, border_x+51, y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;

        tile = paint_tile ( last_removed_pl2_a, -2, -2, -2 ) ;
        assert(tile != NULL );
        cairo_set_source_surface ( cr, tile, border_x, y ) ;
        cairo_paint(cr) ;
        cairo_surface_destroy ( tile ) ;
    }

    D9(cerr<<"D9 draw removed tiles\n")

    return TRUE ;
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget,
                                       cairo_t * cr,
                                       gpointer user_data )
{
    D2(cerr<<"D2 draw play ground\n")

    D3(cerr<<"D3 draw play ground"<<endl)
    D4(cerr<<"D4 play_ground="<<play_ground<<endl)

    switch ( play_ground )
    {
        case empty:
                    cairo_set_source_rgb(cr, sand.r, sand.g, sand.b ) ;
                    cairo_paint(cr) ;
                    break ;
        case rules:
                    {
                    char title[] = "Mahjong rules";
                    char lines[13][MAXLINE] =
        	{ {"In this game you have to remove couples of tiles."},
        	{"You can play against three level of artificial intelligence, or "},
        	{"against a friend, anyway try to collect couples with values as "},
        	{"great as possible."},
        	{"( Flowers = 14, Seasons = 12, Dragons = 5, Winds = 3,"},
        	{"Bamboos / Circles / Crosses = 1 )"},
        	{"Against airhead AI, you can mix tiles, use tips and undo"},
        	{"movements, against greedy AI, you cannotmix tiles, and against"},
        	{"thoughtful AI you cannot undo movements and use tips too."},
        	{"Choose also your tiles layout between the three ones"},
        	{"available: easy~pyramid, medium~cloud, difficult~throneroom."},
        	{""},
        	{""}
        	} ;
            /*_______________max_lun_with_characters_______________*/
            /*                       max lun with spaces                        */
                    draw_text_on_play_ground ( white, light_orange, widget,
                                               cr, title, lines, 28, 18 ) ;
                    }
                    break ;
        case tiles:
                    {
                    cairo_set_source_rgb(cr, green_forest.r, green_forest.g, green_forest.b ) ;
                    cairo_paint(cr) ;

                    for (int z = 0 ; z < dim_Z ; z++ )
                        for ( int y = 0 ; y < dim_Y ; y++ )
                            for ( int x = dim_X-1 ; x >= 0 ; x-- )
                            {
                                if (cube[x][y][z].empty)
                                    continue ;

                                cairo_surface_t* tile=paint_tile(cube[x][y][z].num,
                                                                      x, y, z ) ;
                                assert(tile != NULL );
                                int coor_x, coor_y ;

                                calculate_coor_x_y ( x, y, z, coor_x, coor_y ) ;

                                if ( cube[x][y][z].num < TILES )
                                    set_coor_tile ( &cube[x][y][z], coor_x, coor_y ) ;

                                cairo_set_source_surface(cr, tile, coor_x, coor_y);
                                cairo_paint (cr) ;

                                cairo_surface_destroy ( tile ) ;
                            }
                    }
                    break ;
        case end:
                    {
                    refresh_scores() ;
                    const int score_1 = _score1 ;
                    const int score_2 = _score2 ;
                    if (( score_1 > score_2 )&&( mode == h_c ))
                    {
                        char title[] = "Player 1 won against AI" ;
                        draw_text_on_play_ground ( gold, carmine, widget, cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    else if (( score_1 < score_2 )&&( mode == h_c ))
                    {
                        char title[] = "Player 1 lost against AI" ;
                        draw_text_on_play_ground ( coffe, white, widget, cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    else if (( score_1 == score_2 )&&( mode == h_c ))
                    {
                        char title[] = "Player 1 equalized AI" ;
                        draw_text_on_play_ground ( lapislazuli, gold, widget,cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    else if (( score_1 > score_2 )&&( mode == h_h ))
                    {
                        char title[] = "Player 1 won against Player 2" ;
                        draw_text_on_play_ground ( ferrari, peach, widget, cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    else if (( score_1 < score_2 )&&( mode == h_h ))
                    {
                        char title[] = "Player 1 lost against Player 2" ;
                        draw_text_on_play_ground ( carmine, white, widget, cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    else if (( score_1 == score_2 )&&( mode == h_h ))
                    {
                        char title[] = "Player 1 equalized Player 2" ;
                        draw_text_on_play_ground(blue_persia, yellow, widget,cr,
                                                   title, NULL, 28, 0 ) ;
                    }
                    }
                    break ;
    }

    D9(cerr<<"D9 draw play ground\n")

    return TRUE ;
}

extern "C" gboolean handler_hide_window ( GtkWidget * widget,
                                           GdkEvent * event,
                                           gpointer user_data)
{
    D1(cerr<<"D1 handler hide window\n")

    gtk_widget_hide ( gtk_widget_get_toplevel(widget) ) ;

    if ( playing )
        display_tiles() ;
    else
        display_empty() ;

    D10(cerr<<"D10 handler hide window\n")

    return TRUE ;
}

extern "C" gboolean handler_set_save_game ( GtkWidget * widget,
                                            GdkEvent * event,
                                            gpointer user_data)
{
    D1(cerr<<"D1 handler set save game\n")

    GtkEntry* aus = entry_from_name( "text_save_on_file" ) ;
    char * filename = const_cast<char*>(gtk_entry_get_text( aus )) ;

    if ( save_game ( filename ) )
        gtk_label_set_text ( label_from_name("label_down"), "file saved successfully" ) ;
    else
        gtk_label_set_text ( label_from_name("label_down"), "an error occurred while saving" ) ;

    D3(cerr<<"D3 handler set save game"<<endl)
    D4(cerr<<"D4 filename="<<filename<<endl)

    gtk_widget_hide ( gtk_widget_get_toplevel(widget) ) ;

    D10(cerr<<"D10 handler set save game\n")

    return TRUE ;
}

extern "C" gboolean handler_set_load_game ( GtkWidget * widget,
                                            GdkEvent * event,
                                            gpointer user_data)
{
    D1(cerr<<"D1 handler set load game\n")

    GtkEntry* aus = entry_from_name( "text_load_from_file" ) ;
    char * filename = const_cast<char*>(gtk_entry_get_text( aus )) ;

    if ( load_game ( filename ) )
    {
        gtk_label_set_text ( label_from_name("label_down"), "file loaded successfully" ) ;
        D5(cerr<<"D5 caricamento da file eseguito correttamente"<<endl)
    }
    else
    {
        gtk_label_set_text ( label_from_name("label_down"), "an error occurred while loading" ) ;
        D5(cerr<<"D5 errore durante il caricamento da file"<<endl)
    }

    D3(cerr<<"D3 handler set load game"<<endl)
    D4(cerr<<"D4 filename="<<filename<<endl)

    gtk_widget_hide ( gtk_widget_get_toplevel(widget) ) ;

    D10(cerr<<"D10 handler set load game\n")

    return TRUE ;
}

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data )
{
    D1(cerr<<"D1 handler button pressed event\n")

    if    ( ( widget == (widget_from_name ( "new" ) ) ) ||
            ( widget == (widget_from_name ( "menuitem_new" ) ) ) )
    {
        display_empty() ;
        GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
        gtk_widget_show ( w_new_game ) ;
	}
    else if ( ( widget == (widget_from_name ( "mix" ) ) ) ||
              ( widget == (widget_from_name ( "menuitem_mix" ) ) ) )
    {
        if ((playing)&&( play_ground == tiles )&&(!lock_mix))
        {
            if ( !mix_cube () )
            {
                D3(cerr<<"D3 handler button pressed event"<<endl)
                D7(cerr<<"D7 mix cube returned false"<<endl)
                return TRUE ;
            }
            else
            {
                check_cube() ;
                refresh_unlocked() ;
                if ( !sort_unlocked() )
                {
                    D3(cerr<<"D3 handler button pressed event"<<endl)
                    D6(cerr<<"D6 sort unlocked returned false"<<endl)
                    return TRUE ;
                }
                redraw_widget ( "playground" ) ;
                lock_undo = true ;
                lock_mix = true ;
            }
        }
        else if ((playing)&&( play_ground == tiles )&&( lock_mix ))
        {
            gtk_label_set_text ( label_from_name("label_down"), "mix not allowed just after a mix and vs ai different from airhead" ) ;
        }
	}
    else if ( ( widget == (widget_from_name ( "undo" ) ) ) ||
              ( widget == (widget_from_name ( "menuitem_undo" ) ) ) )
    {
        if ((playing)&&( play_ground == tiles )&&(!lock_undo))
        {
            undo_last_two_couples() ;
            clear_pair_removed () ;
            refresh_scores() ;
        }
        else if ((playing)&&( play_ground == tiles )&&( lock_undo ))
        {
            gtk_label_set_text ( label_from_name("label_down"), "undo not allowed after mix and vs thoughtful ai" ) ;
        }
	}
    else if ( ( widget == (widget_from_name ( "tip" ) ) ) ||
              ( widget == (widget_from_name ( "menuitem_tip" ) ) ) )
    {
        if ((playing)&&( play_ground == tiles )&&( ai != thoughtful))
        {
            tile * a = NULL ;
            tile * b = NULL ;
            bool do_not_use = false ;
            if ( airhead_extraction ( a, b, do_not_use ) )
            {
                assert(a!=NULL) ;
                assert(b!=NULL) ;
                find_coord ( a->num, h_x1, h_y1, h_z1 ) ;
                find_coord ( b->num, h_x2, h_y2, h_z2 ) ;
                redraw_widget ( "playground" ) ;
            }
        }
        if ((playing)&&( play_ground == tiles )&&( ai == thoughtful ))
            gtk_label_set_text ( label_from_name("label_down"), "tips not allowed vs thoughtful ai" ) ;
	}
    else if ( ( widget == (widget_from_name ( "rules" ) ) ) ||
              ( widget == (widget_from_name ( "menuitem_rules" ) ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
            display_rules();
        else if ((playing)&&( play_ground == rules ))
            display_tiles() ;

        if ( !playing )
            display_empty() ;
	}
    else if ( ( widget == (widget_from_name ( "load" ) ) ) ||
              ( widget == (widget_from_name ( "menuitem_load" ) ) ) )
    {
        GtkWidget * w_load_game = widget_from_name ( W_loadgame ) ;
        gtk_widget_show ( w_load_game ) ;
	}
    else if ( ( widget == (widget_from_name ( "save" ) ) ) || 
              ( widget == (widget_from_name ( "menuitem_save" ) ) ) )
    {
        if ((playing)&&( play_ground == tiles ))
        {
            GtkWidget * w_save_game = widget_from_name ( W_savegame ) ;
            gtk_widget_show ( w_save_game ) ;
        }
	}
    else if ( widget == (widget_from_name ( "menuitem_man" ) ) )
    {
        system("iceweasel ../doc/html/index.html &") ;
	}
    else if ( widget == (widget_from_name ( "menuitem_about" ) ) )
    {
	}

    D10(cerr<<"D10 handler button pressed event\n")

    return TRUE ;
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

    if ( mode == h_h )
        ai = airhead ;

    D3(cerr<<"D3 handler set new game"<<endl)
    D4(cerr<<"D4 level="<<level<<" mode="<<mode<<" ai="<<ai<<" lock mix="<<
             lock_mix<<" lock undo="<<lock_undo<<endl)

    start_game () ;

/*alla fine nascondo la finestra*/
    GtkWidget * w_new_game = widget_from_name ( W_newgame ) ;
    gtk_widget_hide ( w_new_game ) ;

    display_tiles() ;

    D10(cerr<<"D10 handler set new game\n")

    return TRUE ;
}

extern "C" gboolean handler_click_on_widget (GtkWidget *widget,
               GdkEventButton  *event,
               gpointer   user_data)
{
    D1(cerr<<"D1 handler click on widget\n")

    if ( play_ground != tiles )
        return TRUE ;

    assert(event!=NULL) ;

    const int _x = event->x ;
    const int _y = event->y ;

    if ( mode == h_c )
        if ( !remove_dummies() )
        {
            D3(cerr<<"D3 handler click on widget"<<endl)
            D8(cerr<<"D8 remove dummies returned false"<<endl)
            return TRUE ;
        }

    bool quit = false ;
    for ( int z = dim_Z-1 ; (z >= 0)&&(!quit) ; z-- )
        for ( int y = 0 ; (y < dim_Y)&&(!quit) ; y++ )
            for ( int x = 0 ; (x < dim_X)&&(!quit) ; x++ )
            {
                if ( &cube[x][y][z] == NULL )
                {
                    delete_cube() ;
                    create_cube() ;
                    level = easy ;
                    mode = h_c ;
                    ai = airhead ;
                    lock_mix = false ;
                    lock_undo = false ;
                    end_game() ;
                    start_game () ;
                    display_tiles() ;
                }
                if ( ( !cube[x][y][z].empty )&&( !cube[x][y][z].lock ) )
                {
                    if ( check_position( &cube[x][y][z], _x, _y ) )
                    {
                        if ( !insert_half_pair( cube[x][y][z].num, x, y, z ) )
                        {
                            D3(cerr<<"D3 handler click on widget"<<endl)
                            D8(cerr<<"D8 insert half pair returned false"<<endl)
                            gtk_label_set_text ( label_from_name("label_down"), "-" ) ;
                            return TRUE ;
                        }
                        quit = true ;
                    }
                }
            }

    if ( !quit )
    {
        D3(cerr<<"D3 handler click on widget"<<endl)
        D8(cerr<<"D8 quit was false"<<endl)
        reset_highlighted_cell() ;
        reset_row() ;
        redraw_widget ( "playground" ) ;
    }

    D10(cerr<<"D10 handler click on widget\n")

    return TRUE ;
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

    call_gtk_main_quit() ;

    D10(cerr<<"D10 handler delete event\n")

    return TRUE ;
}
/*fine implementazione funzioni handler*/

/*inizio implementazione funzioni private*/
static void draw_text_on_play_ground ( const colour &bg, const colour &text_color,
                                GtkWidget * &widget, cairo_t * &cr,
                                char* title, char _line[][MAXLINE],
                                const int &h_title, const int &h_text      )

{
    D1(cerr<<"D1 draw text on play ground\n")

    static const int title_x = 252 ;
    static const int title_y = 50 ;
    static const int newline = 30 ;

    static const int line_x = 60 ;
    static const int line_y = title_y + newline ;

    cairo_set_source_rgb(cr, bg.r, bg.g, bg.b ) ;
	cairo_paint(cr) ;

	cairo_surface_t * surface ;
	cairo_t * cont ;

	cairo_font_extents_t fe;
	cairo_text_extents_t te;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                          gtk_widget_get_allocated_width
                                                                   ( widget ),
                                          gtk_widget_get_allocated_height
                                                                   ( widget )) ;
	cont = cairo_create (surface);

	cairo_set_source_rgb(cont, text_color.r, text_color.g, text_color.b ) ;
	cairo_select_font_face (cont, "sans-serif", CAIRO_FONT_SLANT_ITALIC,
                            CAIRO_FONT_WEIGHT_NORMAL);
	cairo_font_extents (cont, &fe);

    if ( title != NULL )
    {


    	cairo_set_font_size (cont, h_title);

        if ( _line == NULL )
        	cairo_move_to (cont, 185, title_y);
        else
        	cairo_move_to (cont, title_x, title_y);
	    cairo_text_extents (cont, title, &te);
    	cairo_show_text (cont, title);
    }
    if ( _line != NULL )
    {
	    cairo_select_font_face (cont, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_NORMAL);
	    cairo_set_font_size (cont, h_text);
	    for ( int i = 0 ; i < 13 ; i++ )
	    {
	        cairo_move_to (cont, line_x, line_y+newline*i);
        	cairo_text_extents (cont, _line[i], &te);
	        cairo_show_text (cont, _line[i]);
	    }
    }
	cairo_set_source_surface ( cr, surface, 0, 0 ) ;
	cairo_paint(cr);

	cairo_destroy (cont);
	cairo_surface_destroy (surface);

    D10(cerr<<"D10 draw text on play ground\n")
}

static void draw_number_on_tile ( cairo_t * &cr_tile,
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

static void sub_paint_tile ( cairo_t * &context, cairo_surface_t * &surf1,
                      cairo_surface_t * &surf2, const int &num,
                      const char * &image_name, const tile_type &tt )
{
    D2(cerr<<"D2 sub paint tile\n")

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

    D9(cerr<<"D9 sub paint tile\n")
}

static cairo_surface_t * paint_tile (  const int &num,
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

    D9(cerr<<"D9 paint tile\n")

    assert( surface != NULL ) ;

    return ( surface ) ;
    /* la surface la dealloco nella funzione chiamante*/
}

static cairo_surface_t * number_on_tile (  cairo_surface_t * _obj,
                                    const int &number )
{
    D2(cerr<<"D2 number on tile\n")

    cairo_surface_t * _surf_50x50 = cairo_image_surface_create
                                    ( CAIRO_FORMAT_RGB24, 50, 50 ) ;

    cairo_t * cr_tile = cairo_create ( _surf_50x50 ) ;

    cairo_set_source_rgb( cr_tile, peach.r , peach.g , peach.b ) ;
    cairo_paint( cr_tile ) ;
 
    draw_number_on_tile( cr_tile, _obj, number ) ;

    cairo_destroy ( cr_tile ) ;

    D9(cerr<<"D9 number on tile\n")

    assert( _surf_50x50 != NULL ) ;

    return _surf_50x50 ;
    /*la surface la dealloco nella funzione chiamante*/
}

static GtkEntry * entry_from_name ( const char * name )
{
    D2(cerr<<"D2 entry from name\n")

    D9(cerr<<"D9 entry from name\n")

    return GTK_ENTRY(gtk_builder_get_object(builder, name ) ) ;
}

static GtkLabel * label_from_name ( const char * name )
{
    D2(cerr<<"D2 label from name\n")

    D9(cerr<<"D9 label from name\n")

    return GTK_LABEL(gtk_builder_get_object(builder, name ) ) ;
}

static GtkWidget * widget_from_name ( const char * name )
{
    D2(cerr<<"D2 widget from name\n")

    D9(cerr<<"D9 widget from name\n")

    return GTK_WIDGET(gtk_builder_get_object(builder, name ) ) ;
}

static GtkToggleButton * tb_from_name ( const char * name )
{
    D2(cerr<<"D2 togglebutton from name\n")

    D9(cerr<<"D9 togglebutton from name\n")

    return GTK_TOGGLE_BUTTON ( gtk_builder_get_object ( builder, name ) ) ;
}

static bool check_position ( const tile * punt, const int &x, const int &y )
{
    D1(cerr<<"D1 check position\n")

    assert(punt!=NULL);

    if ( ( punt->x1 ) >= x )
        return false ;
    if ( ( punt->y1 ) >= y )
        return false ;

    if ( ( punt->x2 ) <= x )
        return false ;
    if ( ( punt->y2 ) <= y )
        return false ;

    D10(cerr<<"D10 check position\n")

    return true ;
}

static int number_from_string ( const char * word )
{
    D2(cerr<<"D2 number from string\n")

    int i = 0 ;
    while ( word[i]!= '\0' ) 
        i++;

    const char output = word[i-1];

    D3(cerr<<"D3 number from string"<<endl)
    D4(cerr<<word<<" returned "<<output-'0'<<endl)


    D9(cerr<<"D9 number from string\n")

    return (output-'0') ;
}

static void calculate_coor_x_y ( const int &x,
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

static void set_coor_tile ( tile * punt, const int &x, const int &y )
{
    D2(cerr<<"D2 set coor tile\n")

    assert( punt != NULL ) ;

    punt->x1 = x + 10 ;
    punt->y1 = y ;

    punt->x2 = x + 10 + 50 ;
    punt->y2 = y + 50 ;

    D9(cerr<<"D9 set coor tile\n")
}
/*fine implementazione funzioni private*/

/*fine implementazione modulo_GUI*/
