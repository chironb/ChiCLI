// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// alias.h 
// ********************************************************************************

// ********************************************************************************
// ALIAS MACRO FUNCTIONS 
// ********************************************************************************

	// we need to convert some of these I think...


// ********************************************************************************
// ALIAS FUNCTIONS 
// ********************************************************************************

unsigned char do_alias( unsigned char *input_string ) ;
unsigned char clear_alias( unsigned char *input_string ) ;
void clear_alias_all( void ) ;
unsigned char set_alias( unsigned char *input_string , unsigned char *actual_command) ;
void display_alias_all( void ) ;