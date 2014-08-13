#include <cstdlib>
#include <ctime>
const int dim_X = 12 ;
const int dim_Y = 8 ;
const int dim_Z = 4 ;


/*
 * Funzione che crea uno spazio tridimensionale (x, y, z) di tessere.
 */
void create_cube ( tile *** cube )
{
    cube = new tile ** [dim_X] ;

    for ( int x = 0 ; x < dim_X ; x++ )
    {
        cube[x] = new tile * [dim_Y] ;

        for ( int y = 0 ; y < dim_Y ; y++ )
            cube[x][y] = new tile [dim_Z] ;
    }
}

void initialize_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_X ; x++)
        for ( int y = 0 ; y < dim_Y ; y++)
            for ( int z = 0 ; z < dim_Z ; z++)
            {
                cube[x][y][z].empty = true ;
                cube[x][y][z].lock  = true ;
                cube[x][y][z].num   = -1 ;
                cube[x][y][z].value = 0 ;
            }
    unlocked[0] = NULL ;
}

void fill_cube ( tile *** cube )
{
    /*debug controllo array nomi tessere non vuoto*/

    int last = 0 ;

    switch ( level )
    {
        case easy:
                        fill_easy_layout() ;                        
                        break ;
        case medium:
                        fill_medium_layout() ;
                        break ;
        case difficult:
                        fill_difficult_layout() ;
                        break ;
    /*debug se last != 144*/
    }

    mix_cube () ;

}

/*attenziona che ricontrolla il cubo solo se il primo elemento e' a NULL*/
void refresh_unlocked ()
{
    if ( unlocked[0] == NULL )
    {
        int u = 0 ;
        for ( int x = 0 ; x < dim_X ; x++)
            for ( int y = 0 ; y < dim_Y ; y++)
                for ( int z = 0 ; z < dim_Z ; z++)
                    if ( ! cube[x][y][z].empty )
                    {
                        unlocked[u] = &(cube[x][y][z]) ;
                        u++ ;
                    }
/*debug se u > 144*/
    }
    sort_unlocked() ;
}

void sort_unlocked()
{
/*debug se array vuoto*/

    /* per il counting-sort mi servono i num
     * massimo e minimo presenti nelle tessere
     * puntate dagli elementi dell'array
     */
    int max = -1 ;
    int min = TILES + 1 ;
    for ( int i = 0 ; i < TILES ; i++ )
    {
        if ( ( unlocked[i] != NULL ) && ( (*unlocked[i]).num > max ) )
            max = (*unlocked[i]).num ;
        if ( ( unlocked[i] != NULL ) && ( (*unlocked[i]).num < min ) )
            min = (*unlocked[i]).num ;
    }

    /* mi creo un array ausiliario dinamico
     * e ne inizializzo le celle a zero
     */
    int temp = max - min + 1 ;
    int * aus = new int[temp] ;

    for ( int i = 0 ; i < temp ; i++ )
        aus[i] = 0 ;

    /* per ogni puntatore dell'array di partenza
     * conto in quello ausiliario quante occorrenze
     * ci sono tra tutti i num
     */
    for ( int j = 0 ; j < TILES ; j++ )
    {
        if ( unlocked[j] == NULL )
            continue ;

        aus[ (*unlocked[j]).num - min ] += 1 ;
    }

    /* modifico l'array ausiliario affinche'
     * - sia noto il numero di puntatori a tessere rimuovibili
     *          (ultima cella dell'array ausiliario)
     * - ogni cella contenga il numero di tessere puntate con
     *          campo num <= min+indice_cella
     */
    for ( int t = 1 ; t < temp ; t++ )
        aus[t] += aus[t-1] ;

    /* creo un array di output dinamico
     * con tante celle quanti sono i 
     * puntatori a tessere rimuovibili
     */
    tile * output = new tile[ aus[temp-1] ] ;

    /* inizializzo ogni cella dell'array di output scorrendo l'array
     * di partenza e trovando tramite l'array ausiliario la posizione
     * finale di ogni puntatore in ordine crescente di tessera_puntata.num
     */
    for ( int c = 0 ; c < TILES ; c++ )
    {
/*debug se scrive in una cella esterna all'array di output*/
        if ( unlocked[c] != NULL )
            output[ aus[( (*unlocked[c]).num - min )] - 1] = unlocked[c] ;
    }
    /* a questo punto aggiorno l'array di puntatori inserendo
     * i puntatori a tessere rimuovibili all'inizio in ordine decrescente
     * e a seguire tutti gli altri, posti a NULL
     */
    for ( int k = aus[temp-1] ; k >= 0  ; k-- )
        unlocked[k] = output[k] ;
    for ( int k = aus[temp-1] ; k < TILES ; k++ )
        unlocked[k] = NULL ;

    /* infine dealloco gli array dinamici usati
     */
    delete [] output ;
    delete [] aus ;
}

void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i )
{
    tile &rif_a = cube[x1][y1][z1] ;
    tile &rif_b = cube[x2][y2][z2] ;
    if ( (cube[x1][y1][z1].empty)||(cube[x2][y2][z2].empty) )
        i-- ;
    else
    {
        /*swap empty informations*/
        rif_a.empty = rif_a.empty ^ rif_b.empty ;
        rif_b.empty = rif_a.empty ^ rif_b.empty ;
        rif_a.empty = rif_a.empty ^ rif_b.empty ;
        /*swap lock informations*/
        rif_a.lock = rif_a.lock ^ rif_b.lock ;
        rif_b.lock = rif_a.lock ^ rif_b.lock ;
        rif_a.lock = rif_a.lock ^ rif_b.lock ;
        /*swap num informations*/
        rif_a.num = rif_a.num ^ rif_b.num ;
        rif_b.num = rif_a.num ^ rif_b.num ;
        rif_a.num = rif_a.num ^ rif_b.num ;
        /*swap value informations*/
        rif_a.value = rif_a.value ^ rif_b.value ;
        rif_b.value = rif_a.value ^ rif_b.value ;
        rif_a.value = rif_a.value ^ rif_b.value ;

    }
}

void mix_cube () ;
{
    srand ( time(0) ) ;

    /* genero un numero massimo di swap tra le tessere
     * compreso tra 1024 e 2048
     */
    int min = 1024 ;
    int max = min + ( rand() % 1025 ) ;

    for ( int i = 0 ; i < max ; i++ )
        swap_tiles( rand()%dim_X, rand()%dim_Y, rand()%dim_Z,
                    rand()%dim_X, rand()%dim_Y, rand()%dim_Z, i ) ;

}


void fill_easy_layout ()
{
    /*primo strato*/
	fill_floor ( 2, 9, 1, 6, 0, last ) ;
	fill_floor ( 0,11, 0, 0, 0, last ) ;
	fill_floor ( 0,11, 7, 7, 0, last ) ;
	fill_floor ( 0, 1, 3, 4, 0, last ) ;
	fill_floor (10,11, 3, 4, 0, last ) ;
	fill_cell  ( 1, 2, 0, last ) ;
	fill_cell  ( 1, 5, 0, last ) ;
	fill_cell  (10, 2, 0, last ) ;
	fill_cell  (10, 5, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 3, 8, 1, 6, 1, last ) ;
	fill_cell  ( 0, 0, 1, last ) ;
	fill_cell  ( 0, 7, 1, last ) ;
	fill_cell  (11, 0, 1, last ) ;
	fill_cell  (11, 7, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 4, 7, 2, 5, 2, last ) ;
    /*quarto strato*/
	fill_floor ( 5, 6, 3, 4, 3, last ) ;
}
void fill_medium_layout ()
{
    /*primo strato*/
	fill_floor ( 0,11, 2, 7, 0, last ) ;
	fill_floor ( 0,10, 0, 0, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 0, 0, 3, 7, 1, last ) ;
	fill_floor ( 2, 2, 3, 7, 1, last ) ;
	fill_floor ( 4, 4, 3, 7, 1, last ) ;
	fill_floor ( 6, 6, 3, 7, 1, last ) ;
	fill_floor ( 8, 8, 3, 7, 1, last ) ;
	fill_floor (10,10, 3, 7, 1, last ) ;
	fill_floor ( 2, 9, 0, 0, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 0, 0, 4, 7, 2, last ) ;
	fill_floor ( 2, 2, 4, 7, 2, last ) ;
	fill_floor ( 4, 4, 4, 7, 2, last ) ;
	fill_floor ( 6, 6, 4, 7, 2, last ) ;
	fill_floor ( 8, 8, 4, 7, 2, last ) ;
	fill_floor (10,10, 4, 7, 2, last ) ;
    /*quarto strato VUOTO volutamente*/
}
void fill_difficult_layout ()
{
    /*primo strato*/
	fill_floor ( 0, 4, 0, 3, 0, last ) ;
	fill_floor ( 7,11, 0, 0, 0, last ) ;
	fill_floor ( 0, 2, 5, 7, 0, last ) ;
	fill_floor ( 9,11, 5, 7, 0, last ) ;
	fill_floor ( 4, 7, 4, 7, 0, last ) ;
    /*secondo strato*/
	fill_floor ( 1, 3, 1, 2, 1, last ) ;
	fill_floor ( 8,10, 1, 2, 1, last ) ;
	fill_floor ( 0, 1, 6, 7, 1, last ) ;
	fill_cell  ( 0, 5, 1, last ) ;
	fill_cell  ( 2, 7, 1, last ) ;
	fill_floor (10,11, 6, 7, 1, last ) ;
	fill_cell  ( 9, 7, 1, last ) ;
	fill_cell  (11, 5, 1, last ) ;
	fill_floor ( 4, 7, 4, 7, 1, last ) ;
	fill_cell  ( 4, 4, 1, last ) ;
	fill_cell  ( 7, 4, 1, last ) ;
    /*terzo strato*/
	fill_floor ( 1, 2, 1, 2, 2, last ) ;
	fill_floor ( 9,10, 1, 2, 2, last ) ;
	fill_floor ( 0, 1, 7, 7, 2, last ) ;
	fill_cell  ( 0, 6, 2, last ) ;
	fill_floor (10,11, 7, 7, 2, last ) ;
	fill_cell  (11, 6, 2, last ) ;
	fill_floor ( 4, 7, 6, 7, 2, last ) ;
	fill_cell  ( 4, 5, 2, last ) ;
	fill_cell  ( 7, 5, 2, last ) ;
    /*quarto strato*/
	fill_cell  ( 0, 7, 3, last ) ;
	fill_cell  (11, 7, 3, last ) ;
	fill_cell  ( 4, 6, 3, last ) ;
	fill_cell  ( 7, 6, 3, last ) ;
	fill_floor ( 4, 7, 7, 7, 3, last ) ;
}
int tile_value (const int &name_position)
{
/*debug se name_position e' una posizione dell'array*/

    /* si tratta delle prime 108 tessere:
     * circle, bamboo, cross, numerati da 1 a 9
     */
    if      ( name_position <= 107 )
        return SEEDVALUE ;
    else
    /* si tratta delle 16 tessere dei venti:
     * east, sud, west, north
     */
    if ( name_position <= 123 )
        return WINDVALUE ;
    else
    /* si tratta delle 12 tessere dei draghi:
     * red, green, white dragon
     */
    if ( name_position <= 135 )
        return DRAGONVALUE ;
    else
    /* si tratta delle 4 tessere delle stagioni:
     * spring, summer, autumn, winter
     */
    if ( name_position <= 139 )
        return SEASONVALUE ;
    else
    /* si tratta delle 4 tessere dei fiori:
     * plumb, orchid, chrysanthemum, bamboo
     */
    if ( name_position <= 143 )
        return FLOWERVALUE ;
    else
    /* attenzione errore di gestione della memoria
     */
/*debug se name_position eccede l'array*/
        return -1 ;
        
}

void fill_cell ( const int &x, const int &y, const int &z, int &last)
{
/*debug se valori x, y, z consoni*/
    cube[x][y][z].empty = false ;
    cube[x][y][z].num = last ;
    cube[x][y][z].value = tile_value (last) ;
    last++ ;

}

void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last )
{
/*debug se valori x, y, z consoni*/
    for ( int x = x1 ; x <= x2 ; x++ )
        for ( int y = y1 ; y <=y2 ; y++ )
            fill_cell ( x, y, z, last ) ;
}

void delete_cube ( tile *** cube )
{
    for ( int x = 0 ; x < dim_X ; x++ )
    {
        for ( int y = 0 ; y < dim_Y ; y++ )
        {
            for ( int z = 0 ; z < dim_Z ; z++ )
            {
                if ( cube[x][y][z].name != NULL )
                    delete [] cube[x][y][z].name ;

                delete [] cube[x][y] ;
            }
        }

        delete [] cube[x] ;
    }

    delete [] cube ;
}


