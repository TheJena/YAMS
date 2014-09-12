#include "data_structure.h"
#include "io_file.h"

extern bool lock_mix ;

void create_cube () ;

void delete_cube ( ) ;

void initialize_cube () ;

void fill_cube ( ) ;

bool mix_cube () ;

void check_cube () ;

void refresh_unlocked () ;

bool check_pair ( const tile * const a, const tile * const b,
                  tile * &first, tile * &second ) ;

void initialize_neighbor ( tile * &left_a, tile * &right_a, tile * &under_a,
                           tile * &left_b, tile * &right_b, tile * &under_b,
                           const int &xa, const int &ya, const int &za,
                           const int &xb, const int &yb, const int &zb,
                           tile * &a, tile * &b ) ;

void check_convenience ( tile * a, tile * b, bool &exit ) ;

bool airhead_extraction ( tile * &first, tile * &second, bool &exit ) ;

bool extract_pair ( couple *  pair ) ;

void find_coord ( const int &num, int &_x, int &_y, int &_z ) ;

bool between ( const int &min, const int &middle, const int &max ) ;

void check_cell ( const int &x, const int &y, const int &z ) ;

bool left_cell ( const int &x, const int &y, const int &z ) ;

bool right_cell ( const int &x, const int &y, const int &z ) ;

bool sort_unlocked() ;

int count_pairs_removable ( const int &count) ;

void sort_sub_array ( tile ** out, int start, int end ) ;

void swap_tiles ( const int &x1, const int &y1, const int &z1,
                  const int &x2, const int &y2, const int &z2, int &i ) ;

void fill_easy_layout ( int &last ) ;

void fill_medium_layout ( int &last ) ;

void fill_difficult_layout ( int &last ) ;

int tile_value (const int &name_position) ;

void fill_cell ( const int &x, const int &y, const int &z, int &last) ;

void fill_floor( const int &x1, const int &x2, const int &y1, const int &y2,
                 const int &z, int &last ) ;

void reset_cell ( const int &x, const int &y, const int &z ) ;

bool remove_dummies() ;
