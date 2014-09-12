using namespace std ;

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

void import_tiles_names () ;

void delete_tiles_names () ;

bool save_game_on_file ( const couple ** mov, char * filename,
                         const int &row, const int &col ) ;

bool load_game_from_file ( couple** &mov, const char * filename, int &row, int &col ) ;
