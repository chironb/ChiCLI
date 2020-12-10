// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// chicli.h
// ********************************************************************************

// ********************************************************************************
// MACRO VARS
// ********************************************************************************

#define TRUE 1
#define FALSE 0

#define MAX_ENTERED_KEYSTROKES 80  // Commodore 40 column machines let you input a max line length of 80 characters
#define MAX_LENGTH_COMMAND 40 // Commodore DOS maximum length of filenames is 16 characters, but we also want quotes around the string, so we add two characters for that // fuck it make it 40 
#define MAX_LENGTH_ARGS 40 // Commodore DOS maximum length of command string is 40 characters

#define MAX_ALIASES 16 // unsigned char alias_shortcut_list[MAX_ALIASES][MAX_LENGTH_COMMAND] ; // --> lots of RAM! like 2.5K !!!
#define MAX_HOTKEYS 8  

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 25

#define SCREEN_RIGHT_EDGE SCREEN_WIDTH-1
#define SCREEN_LEFT_EDGE 0
#define SCREEN_BOTTOM_EDGE SCREEN_HEIGHT-1
#define SCREEN_TOP_EDGE 0
#define CURSOR_CHARACTER 230 // This is the checkerboard character 

// the following if for type-prg functionality 
#define first_header_byte  				0 
#define second_header_byte 				1
#define first_basic_line_address_byte   2
#define second_basic_line_address_byte  3
#define first_basic_line_number_byte    4
#define second_basic_line_number_byte   5
#define body_bytes         				6
#define end_of_line_byte   				7
#define end_of_prg_byte    				8


unsigned char they_are_sure(void);
unsigned char convert_char(unsigned char char_to_convert) ;


// ********************************************************************************
// MACRO FUNCTIONS
// ********************************************************************************

#define matching(testing_string,original_string) (strcmp(original_string,testing_string)==0)

#define display_title_text() printf("ChiCLI %s (c) 2020 Chiron Bramberger\n", VERSION)

#define display_description_text() printf("A CLI for Commodore Computers!\n")

#define wait_for_keypress() 			\
										\
	while(kbhit()) { /* flush buffer */	\
		cgetc();						\
	};/*end while*/						\
										\
	while(!kbhit()) {					\
		/*do nothing*/					\
	};/*end while*/ 					\
										\
//end macro func 


#define display_title_screen(has_ddd)  \
	clrscr();					\
	display_title_text();		\
	sys_info(has_ddd);					\
	pet_chirp();				\
//end macro func 

// ********************************************************************************
// GENERAL FUNCTIONS
// ********************************************************************************

// void chi_gotox( signed int x_to_goto );  // these are debugging
// void chi_gotoy( signed int y_to_goto ); // these are debugging


// ********************************************************************************
// GENERIC PROMPTS FUNCTIONS 
// ********************************************************************************

// unsigned char they_are_sure(void);
// void hit_any_key(void); // this should be a MACRO function 







