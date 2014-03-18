#include"TSP/TSP.hpp"
auto main ( ) -> int
{
	TSP_PRINTF ( "%c\n" , 'a' ) ;
	TSP_PRINTF ( "%s\n" , "hoge" ) ;
	TSP_PRINTF ( "%d\n" , 1 ) ;
	TSP_PRINTF ( "%f\n" , 0.1 ) ;
	TSP_PRINTF ( "%%\n%c\n%s\n%d\n%f\nHello! TSP!\n" , 'e' , "piyo" , 1 , 0.25 ) ;
//	TSP_PRINTF ( "%f\n" , "hoge" ) ; // <- Compile Error
}