using namespace std ;

enum p_player { p_human1, p_human2, p_ai } ;

extern int _score1 ;
extern int _score2 ;

extern bool playing ;
extern bool lock_undo ;

bool start_game () ;

void undo_last_two_couples () ;

void refresh_scores( int &score1, int &score2) ;

bool check_couple ( ) ;

void clear_pair_removed () ;

void refresh_pair_removed( const p_player &temp, const int &a, const int &b ) ;

bool opponent_round () ;

void reset_row() ;

bool insert_half_pair ( const int &num, const int &x, const int &y, const int &z ) ;

void end_game () ;
