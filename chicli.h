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

#define MAX_ENTERED_KEYSTROKES 60 // Commodore 40 column machines let you input a max line length of 80 characters
#define MAX_LENGTH_COMMAND 40 	  // Commodore DOS maximum length of filenames is 16 characters, but we also want quotes around the string, so we add two characters for that // fuck it make it 40 
#define MAX_LENGTH_ARGS 40 		  // Commodore DOS maximum length of command string is 40 characters

#define MAX_ALIASES 8    		  // 8 TODO:RAM: unsigned char alias_shortcut_list[MAX_ALIASES][MAX_LENGTH_COMMAND] ; // --> lots of RAM! like 2.5K !!!
#define MAX_ALIAS_LENGTH 40		  // TODO:RAM: this needs to match the MAX_LENGTH_COMMAND until I fix it 
#define MAX_HOTKEYS 8  			  // TODO:RAM: 
#define MAX_HOTKEY_LENGTH 20 	  // TODO:RAM: 

#define MAX_COMMODORE_DOS_FILENAME 16 

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
// MACRO HELPER FUNCTIONS
// ********************************************************************************

// LOGIC HELPERS FUNCTIONS ********************************************************

#define true 1
#define TRUE 1
#define True 1
#define false 0
#define FALSE 0
#define False 0

// LOOP HELPERS FUNCTIONS ********************************************************

#define LOOP while(TRUE)
#define loop while(TRUE)

// MEMORY HELPERS FUNCTIONS *******************************************************

#define read_nibble_low(address) PEEK(address) & 0x0F



		// write - nibble1 and nibble2 are what gets written
		// char byte;
		// byte = (byte & 0xF0) | (nibble1 & 0xF); // write low quartet
		// byte = (byte & 0x0F) | ((nibble2 & 0xF) << 4); // write high quartet

		// read - input_b() is what gets read, and then the nibble is written to X and Y and Z 
		// X = input_b() & 0x0F; / bits 0 to 3
		// Y = input_b() >> 4; // bits 4 to 7
		// Z = (input_b() & 0x3C) >> 2; // bits 2 to 5 

// #define write_memory(address,val) (*(unsigned char*) (address) = (val)) // write_memory(0xD800, 0x12); // stores 0x12 at address 0xd800, useful for hardware registers
// #define read_memory(address)      (*(unsigned char*) (address))         // B = read_memory(0xD800);    // read a value from address 0xd800, into B, which should be a char palettes
// PEEK(0xD800) & 0x0F;

// In the jump table, no expressions are supported. The array index used in the goto must be a simple variable or a constant.
// Binary literals, a C++14 feature and a GCC C extension, are accepted. They can be disabled with the --standard option.

//             unsigned char foo = 0b101; // sets it to 5
            
// #define get_bit(data,y)   ( (data>>y) & 1 ) // Return Data.Y value - // uint8_t bit_2 = get_bit(number,2); // bit_2 = 1
// #define set_bit(data,y)   data |= (1 << y)  // Set Data.Y   to 1   - // set_bit(number,1); // number =  0x07 => 0b00000111 // bits are right to left so the right most bit is bit 0 
// #define clear_bit(data,y) data &= ~(1 << y) // Clear Data.Y to 0   - // clear_bit(number,2); // number =0x03 => 0b0000011
// #define bit_0 0
// #define bit_1 1
// #define bit_2 2
// #define bit_3 3
// #define bit_4 4
// #define bit_5 5
// #define bit_6 6
// #define bit_7 7

// #define write_memory(address,val) (*(unsigned char*) (address) = (val)) // write_memory(0xD800, 0x12); // stores 0x12 at address 0xd800, useful for hardware registers
// #define read_memory(address)      (*(unsigned char*) (address))         // B = read_memory(0xD800);    // read a value from address 0xd800, into B, which should be a char palettes






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







