// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
//
/* Compiling, deleting object files, and launching VICE in fast prg loading mode: 
cl65 -g -Osr -t c64 --static-locals  chicli.c  string_processing.c alias.c hardware.c  commands.c -o chicli-16.prg  &&  rm *.o  &&  x64sc -autostartprgmode 1 chicli-16.prg
*/ 
    // This program is free software: you can redistribute it and/or modify
    // it under the terms of the GNU General Public License as published by
    // the Free Software Foundation, either version 3 of the License, or
    // (at your option) any later version.

    // This program is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    // GNU General Public License for more details.

    // You should have received a copy of the GNU General Public License
    // along with this program.  If not, see <https://www.gnu.org/licenses/>.	

// ********************************************************************************

// ********************************************************************************
// VERSION
// ********************************************************************************

#define VERSION "v0.16"
#define PROGRAM_NAME "chicli"

// ********************************************************************************
// INCLUDES
// ********************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cbm.h>
#include <cc65.h>
#include <conio.h>
#include <6502.h>
#include <peekpoke.h>
#include <time.h> 
#include <device.h>


#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif 

#ifndef HOTKEYS_H
#define HOTKEYS_H
#include "hotkeys.h"
#endif 

#ifndef COMMANDS_H
#define COMMANDS_H
#include "commands.h" 
#endif 

#ifndef CURSOR_H
#define CURSOR_H
#include "cursor.h"
#endif 

#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H
#include "string_processing.h"
#endif 

#ifndef ALIAS_H
#define ALIAS_H
#include "alias.h"
#endif 

#ifndef HARDWARE_H
#define HARDWARE_H
#include "hardware.h"
#endif 


// ********************************************************************************
// GLOBAL VARS
// ********************************************************************************

unsigned char color_profiles[14][3] = {{06,01,01}, //  1 - C PET Style
								       {07,02,04}, //  2 - C VIC-20 Style
								       {15,07,15}, //  3 - C 64 Style
								       {07,02,04}, //  4 - C SX-64 Style
								       {14,12,14}, //  5 - C 128 VIC-II Style
								       {04,01,01}, //  6 - C 128 VDC Style
								       {02,07,07}, //  7 - AmigaDOS v1.3 Style
								       {02,07,15}, //  8 - White on Blue on L.Blue
								       {01,02,02}, //  9 - Black on White
								       {02,01,01}, // 10 - White on Black
								       {02,12,13}, // 11 - Greyscale One
								       {01,02,16}, // 12 - Greyscale Two
								       {02,15,07}};// 13 - Default

unsigned char alias_shortcut_list[MAX_ALIASES][MAX_LENGTH_COMMAND] ; // --> lots of RAM! like 2.5K !!!
unsigned char alias_command_list[MAX_ALIASES][MAX_LENGTH_COMMAND] ;  // --> see chicli.h to change this
unsigned char hotkeys_list[MAX_HOTKEYS][MAX_LENGTH_COMMAND] ;
unsigned char entered_keystrokes[MAX_ENTERED_KEYSTROKES] ;
unsigned char position_at_prompt_x = 0;
unsigned char position_at_prompt_y = 0;	
unsigned char extracted_program_name[MAX_LENGTH_COMMAND];
unsigned char get_key = '\0';

unsigned char keystroke;
unsigned char previous_entered_keystrokes[MAX_ENTERED_KEYSTROKES] ;
unsigned char prompt[16] = "> "; // Example:  D8:games>  or:  D8:>   or just: >
unsigned char starting_x = 0;
unsigned char starting_y = 0;
unsigned char position_in_string = 0;
unsigned char previous_x = 0;
unsigned char previous_y = 0;
unsigned char result = 0;
unsigned char result2 = 0;
unsigned char read_bytes = 0;

unsigned char i; // shared by all for loops

// ARRAYS
unsigned char disk_sector_buffer[255] = " "; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...

unsigned char user_input[MAX_ENTERED_KEYSTROKES]                = ""  ;
unsigned char user_input_processed[MAX_ENTERED_KEYSTROKES]      = ""  ;
 
unsigned char     user_input_command_type                       = 'e' ; // e for empty, s for string, n for number
unsigned char     user_input_arg1_type                          = 'e' ; 
unsigned char     user_input_arg2_type                          = 'e' ;  
unsigned char     user_input_arg3_type                          = 'e' ;  
unsigned char     user_input_arg4_type                          = 'e' ;  

unsigned char 	  drive_command_string[MAX_LENGTH_COMMAND]		= "drive_command_string not used yet"  ;
unsigned char 	  drive_command_string2[MAX_LENGTH_COMMAND]		= "drive_command_string2 not used yet"  ;

unsigned char 	  user_input_command_string[MAX_LENGTH_COMMAND] = ""  ;
unsigned char     user_input_arg1_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char     user_input_arg2_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char     user_input_arg3_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char     user_input_arg4_string[MAX_LENGTH_ARGS]       = ""  ;	

unsigned long int user_input_command_number                     = 0   ;
unsigned int user_input_arg1_number                        = 0   ;  
unsigned int user_input_arg2_number                        = 0   ;  
unsigned int user_input_arg3_number                        = 0   ;  
unsigned int user_input_arg4_number                        = 0   ;  

unsigned char number_of_user_inputs = 0;

unsigned char * token_pointer_command;
unsigned char * token_pointer_arg1;
unsigned char * token_pointer_arg2;
unsigned char * token_pointer_arg3;
unsigned char * token_pointer_arg4;

unsigned char original_position_at_prompt_y = 0;

unsigned int  error_number      ;
unsigned char error_message[32] ;
unsigned int  error_track       ;
unsigned int  error_block       ;

unsigned char dev = 0;

struct        cbm_dirent dir_ent;  
unsigned int  number_of_files = 0;
unsigned char displayed_count = 0;
unsigned int  file_size = 0;

const char command_cdback[] = { ':', 0x5F, '\0' };

unsigned char detected_filetype = 0;
unsigned char detected_filetype_char[] = "  ";

unsigned char spot_in_prg = 0;
unsigned char previous_byte = ' ';

unsigned char hex_display_position_x = 0; 
unsigned char hex_display_position_y = 0;
unsigned int  hex_display_position_in_file = 0; // check how logn long int is 

unsigned char new_drive_number = 0;

unsigned int  peek_address  = 1000;
unsigned int  start_address = 1000;
unsigned int  end_address   = 1000;
unsigned int  poke_address  = 1000;
unsigned char peeked = 0;
unsigned char poked = 0;

unsigned char user_input_number1 = 0;

	unsigned char ntscpal_detected = 0;
	unsigned char 	sid_detected   = 0;
	unsigned char kernal_detected  = 0;
	unsigned char model_detected   = 0;
	unsigned char cpu_detected     = 0;

struct regs reboot_register; // this is needed for doing a sys() command, like rebooting

unsigned char use_dcopy = 0;

unsigned char current_textcolor;
unsigned char current_bgcolor;
unsigned char current_bordercolor;

unsigned char have_device_numbers_changed = FALSE; // False - the druive nubmers  haven't been change in software, True - they have, so don't drive detect

unsigned char dir_file_total = 0;
unsigned char current_dir_file_index = 0;

unsigned char iii = 0;
unsigned char jjj = 0;
unsigned char loop_k = 0;

// ********************************************************************************
// FUNCTIONS
// ********************************************************************************

// void gotox( signed int x_to_goto ) { // these are debugging

// 	if (x_to_goto < 0) {
// 		// bordercolor(  7-1 ); // blue
// 	}else if (x_to_goto > SCREEN_RIGHT_EDGE) {
// 		// bordercolor(  3-1 ); // red
// 	} else {
// 		// bordercolor(  15-1 ); // light blue 
// 	};//end if  

// 	gotox(x_to_goto);

// };//end func


// void gotoy( signed int y_to_goto ) {// these are debugging

// 	if (y_to_goto < 0) {
// 		//bordercolor(  8-1 ); // yellow
// 	}else if (y_to_goto > SCREEN_BOTTOM_EDGE) {
// 		//bordercolor(  9-1 ); // orange
// 	} else {
// 		//bordercolor(  15-1 ); // light blue 
// 	};//end if  

// 	gotoy(y_to_goto);

// };//end func


// ********************************************************************************
// GENERIC PROMPTS FUNCTIONS 
// ********************************************************************************

unsigned char they_are_sure(void) {

	unsigned char are_they_sure_keystroke;

	printf("Are you sure (Y/N)?\n");

	wait_for_keypress(); 	// wait for a keypress, after flushing the buffer 
	are_they_sure_keystroke = cgetc();

	if (are_they_sure_keystroke == 'Y' || are_they_sure_keystroke == 'y') {
		printf("Proceeding.\n");
		return(1);
	} else {		
		printf("Aborting.\n");
		return(0);
	};//end if 

};//end if 


// this should be a MACRO function 
void hit_any_key() {

	printf("* Hit key *");

	wait_for_keypress(); 	// wait for a keypress, after flushing the buffer 
	cgetc(); 				// grab key but dion't keep it throw it away because we don't give any fucks!

	gotox(SCREEN_LEFT_EDGE);				// blank the line and return to the left edge to continue printing 
	printf("                             ");
	gotox(SCREEN_LEFT_EDGE);	

};//end if 

unsigned char convert_char(unsigned char char_to_convert) {
  
  if ((char_to_convert >= 0x20 && char_to_convert <= 0x7f) ||   // if it *IS* a good displaying character
       char_to_convert >= 0xA0 ){    
    
	  if (char_to_convert >= 65 && char_to_convert <= 90){   // if lower alphabet
	    
	     //char_to_convert = char_to_convert + 32 ;                    // then shift up 
	  
	  } else if (char_to_convert >= 97 && char_to_convert <= 122) { // else if upper alphabet
	    
	     //char_to_convert = char_to_convert - 32 ;                    // then shift down 

	  };//end if 

  } else {

  	char_to_convert = '.' ;                                     // output this instead 

  };//end if   

  return char_to_convert ;

};//end func 






char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}



void display_logo(unsigned char x, unsigned char y) {

	//TODO: prevent the lgoo from trypign to drawe itself outisde teh screen 

	// unsigned char line1[] = "B   XXXX     ";
	// unsigned char line2[] = "B  XXXXX     ";
	// unsigned char line3[] = "B XX|   XXXXX";
	// unsigned char line4[] = "BXX|    XXXX ";
	// unsigned char line5[] = "BXX          ";
	// unsigned char line6[] = "BXX|    PXXXX ";
	// unsigned char line7[] = "B XX|   PXXXXX";
	// unsigned char line8[] = "B  XXXXX     ";
	// unsigned char line9[] = "B   XXXX     ";


	unsigned char line1[] = "   XXXX     ";
	unsigned char line2[] = "  XXXXX     ";
	unsigned char line3[] = " XX|   XXXXX";
	unsigned char line4[] = "XX|    XXXX ";
	unsigned char line5[] = "XX          ";
	unsigned char line6[] = "XX|    XXXX ";
	unsigned char line7[] = " XX|   XXXXX";
	unsigned char line8[] = "  XXXXX     ";
	unsigned char line9[] = "   XXXX     ";

	unsigned char line6r[] = "XXXX";
	unsigned char line7r[] = "XXXXX";

	// full checkerboard = 230    bottom checkerboard = 232      side checkerboard = 220 
	// ctrl+7 = 31  blue        ctrl+3 = 28 red        ctrl+3 = 5 whtie    ctrl+4 = cyan     ctrl+5 = 156 purple 

	// TODO: this takes up like 256 or so bytes
	replace_char(line1, 'X', 230);
	replace_char(line2, 'X', 230);
	replace_char(line3, 'X', 230);
	replace_char(line4, 'X', 230);
	replace_char(line5, 'X', 230);
	replace_char(line6, 'X', 230);
	replace_char(line7, 'X', 230);
	replace_char(line8, 'X', 230);
	replace_char(line9, 'X', 230);

	replace_char(line1, '|', 220);
	replace_char(line2, '|', 220);
	replace_char(line3, '|', 220);
	replace_char(line4, '|', 220);
	replace_char(line5, '|', 220);
	replace_char(line6, '|', 220);
	replace_char(line7, '|', 220);
	replace_char(line8, '|', 220);
	replace_char(line9, '|', 220);

	replace_char(line6r, 'X', 230);
	replace_char(line7r, 'X', 230);

	// strcpy(line1, replace_char(line1, 'X', 230)); strcpy(line1, replace_char(line1, '|', 220)); strcpy(line1, replace_char(line1, '_', 232)); strcpy(line1, replace_char(line1, 'B', 31)); strcpy(line1, replace_char(line1, 'R', 28));  strcpy(line1, replace_char(line1, 'W', 5)); strcpy(line1, replace_char(line1, 'P', 156)); 
	// strcpy(line2, replace_char(line2, 'X', 230)); strcpy(line2, replace_char(line2, '|', 220)); strcpy(line2, replace_char(line2, '_', 232)); strcpy(line2, replace_char(line2, 'B', 31)); strcpy(line2, replace_char(line2, 'R', 28));  strcpy(line2, replace_char(line2, 'W', 5)); strcpy(line2, replace_char(line2, 'P', 156)); 
	// strcpy(line3, replace_char(line3, 'X', 230)); strcpy(line3, replace_char(line3, '|', 220)); strcpy(line3, replace_char(line3, '_', 232)); strcpy(line3, replace_char(line3, 'B', 31)); strcpy(line3, replace_char(line3, 'R', 28));  strcpy(line3, replace_char(line3, 'W', 5)); strcpy(line3, replace_char(line3, 'P', 156)); 
	// strcpy(line4, replace_char(line4, 'X', 230)); strcpy(line4, replace_char(line4, '|', 220)); strcpy(line4, replace_char(line4, '_', 232)); strcpy(line4, replace_char(line4, 'B', 31)); strcpy(line4, replace_char(line4, 'R', 28));  strcpy(line4, replace_char(line4, 'W', 5)); strcpy(line4, replace_char(line4, 'P', 156)); 
	// strcpy(line5, replace_char(line5, 'X', 230)); strcpy(line5, replace_char(line5, '|', 220)); strcpy(line5, replace_char(line5, '_', 232)); strcpy(line5, replace_char(line5, 'B', 31)); strcpy(line5, replace_char(line5, 'R', 28));  strcpy(line5, replace_char(line5, 'W', 5)); strcpy(line5, replace_char(line5, 'P', 156)); 
	// strcpy(line6, replace_char(line6, 'X', 230)); strcpy(line6, replace_char(line6, '|', 220)); strcpy(line6, replace_char(line6, '_', 232)); strcpy(line6, replace_char(line6, 'B', 31)); strcpy(line6, replace_char(line6, 'R', 28));  strcpy(line6, replace_char(line6, 'W', 5)); strcpy(line6, replace_char(line6, 'P', 156)); 
	// strcpy(line7, replace_char(line7, 'X', 230)); strcpy(line7, replace_char(line7, '|', 220)); strcpy(line7, replace_char(line7, '_', 232)); strcpy(line7, replace_char(line7, 'B', 31)); strcpy(line7, replace_char(line7, 'R', 28));  strcpy(line7, replace_char(line7, 'W', 5)); strcpy(line7, replace_char(line7, 'P', 156)); 
	// strcpy(line8, replace_char(line8, 'X', 230)); strcpy(line8, replace_char(line8, '|', 220)); strcpy(line8, replace_char(line8, '_', 232)); strcpy(line8, replace_char(line8, 'B', 31)); strcpy(line8, replace_char(line8, 'R', 28));  strcpy(line8, replace_char(line8, 'W', 5)); strcpy(line8, replace_char(line8, 'P', 156)); 
	// strcpy(line9, replace_char(line9, 'X', 230)); strcpy(line9, replace_char(line9, '|', 220)); strcpy(line9, replace_char(line9, '_', 232)); strcpy(line9, replace_char(line9, 'B', 31)); strcpy(line9, replace_char(line9, 'R', 28));  strcpy(line9, replace_char(line9, 'W', 5)); strcpy(line9, replace_char(line9, 'P', 156)); 

	gotoxy(x,y+0); 

	if (current_bgcolor == 6) { // blue but like it's offset... 
		textcolor( 1-1 ); // if background is blue, use black 
	} else {
		textcolor( 7-1 ); // if background isn't blue, use blue 
	};//switch

	gotoxy(x,y+0); printf("%s",line1);
	gotoxy(x,y+1); printf("%s",line2);
	gotoxy(x,y+2); printf("%s",line3);
	gotoxy(x,y+3); printf("%s",line4);
	gotoxy(x,y+4); printf("%s",line5);
	gotoxy(x,y+5); printf("%s",line6);
	gotoxy(x,y+6); printf("%s",line7);
	gotoxy(x,y+7); printf("%s",line8);
	gotoxy(x,y+8); printf("%s",line9);	

	gotoxy(x,y+0); printf("%c",28); // 28 red

	gotoxy(x+7,y+5); printf("%s",line6r);
	gotoxy(x+7,y+6); printf("%s",line7r);

	textcolor( current_textcolor );

	// printf(" current%ibgcolor:\n", current_bgcolor);

};//end func



void sys_info(unsigned char is_drive_detection_disabled) {	

	// put logo 1 char in 
	// put text at positoin 14 on screeen 
	// put dtected text at positon 25 on screen 

	starting_x = wherex();
	starting_y = wherey();

	ntscpal_detected = detect_ntsc_pal() ;	
	// printf("N:%i ",ntscpal_detected);

		sid_detected = detect_sid()      ;
	// printf("S:%i ",sid_detected);

	 kernal_detected = detect_kernal()   ;	
	// printf("K:%i ",kernal_detected);

	  model_detected = detect_model()    ;
	// printf("M:%i ",model_detected);

	    cpu_detected = detect_cpu()      ;
	// printf("C:%i \n",cpu_detected);

	display_logo(starting_x+1,starting_y+1);

	// textcolor(    2-1 );

	gotoxy(starting_x+14,starting_y+2); cputs("Model:");
	gotoxy(starting_x+14,starting_y+3); cputs("Region:");
	gotoxy(starting_x+14,starting_y+4); cputs("Kernal:");
	gotoxy(starting_x+14,starting_y+5); cputs("Processor:");
	gotoxy(starting_x+14,starting_y+6); cputs("Graphics:");
	gotoxy(starting_x+14,starting_y+7); cputs("Sound:");

	gotoxy(starting_x+25,starting_y+2);

	cputs("Commodore ");
	switch(model_detected) {
		case 0 : cputs("128D"); break;
		case 1 : cputs("128"); break;	
		case 2 : cputs("128DCR"); break;
		case 3 : 
		case 4 : cputs("64"); break;
		case 5 : cputs("64C"); break;
		case 6 : cputs("64"); break;
		case 7 : cputs("64C"); break;
	};//end switch 

	gotoxy(starting_x+25,starting_y+3);
	switch(ntscpal_detected) {
		case 0 : cputs("NTSC"); break;
		case 1 : cputs("PAL"); break;	
	};//end switch

	gotoxy(starting_x+25,starting_y+4);
	switch(kernal_detected) {
		case 1 : cputs("R1 901227-01"); break;
		case 2 : cputs("R2 901227-02"); break;
		case 3 : cputs("R3 901227-03"); break;	
	};//end switch

	gotoxy(starting_x+25,starting_y+5);
	cputs("MOS ");

	switch(cpu_detected) {
		case 0  : cputs("6502"); break;
		case 1  : cputs("65C02"); break;	
		case 2  : cputs("65816"); break;
		case 3  : cputs("4510"); break;
		case 4  : cputs("65SC02"); break;
		case 5  : cputs("65CE02"); break;
		case 6  : cputs("HUC6280"); break;
		case 7  : cputs("2A0x"); break;
		case 8  : cputs("45GS02"); break;
		case 9  : cputs("8502"); break;
		case 10 : cputs("8500"); break;
		case 11 : cputs("6510"); break;
		default : cputs("Unknown"); //end default
	};//end switch 

	gotoxy(starting_x+25,starting_y+6);
	cputs("VIC-II ");
	if        (ntscpal_detected == 0 && cpu_detected == 9 ) { cputs("8564"); 	// if it's a C128 CPU then we know what teh vic is based on region 
	} else if (ntscpal_detected == 1 && cpu_detected == 9 ) { cputs("8566/69"); 	// if it's a C128 CPU then we know what teh vic is based on region 
	} else if (ntscpal_detected == 0 && sid_detected == 1 ) { cputs("6567"); 	// if NTSC + 6581 + KERNAL R1 901227-01 --> C64 (Early) --> VIC-II 6567		 			// if NTSC + 6581 + KERNAL R2 901227-02 --> C64 		--> VIC-II 6567
	} else if (ntscpal_detected == 0 && sid_detected == 2 ) { cputs("8562"); 	// if NTSC + 8580 + Any					--> C64C 		--> VIC-II 8562
	} else if (ntscpal_detected == 1 && sid_detected == 1 ) { cputs("6569/7x"); 	// if PAL + 6581 + Any 				    --> C64 		--> VIC-II 6569/6572/6573
	} else if (ntscpal_detected == 1 && sid_detected == 2 ) { cputs("8565"); 	// if PAL + 8580 + Any					--> C64C 		--> VIC-II 8565
	};//end if 

	gotoxy(starting_x+25,starting_y+7);
	cputs("SID ");
	switch(sid_detected) {		
		case    1 : cputs("6581"); break;
		case    2 : cputs("8580"); break;	
		default   : cputs("Unknown"); //end default 
	};//end switch



	// printf("|%i|", is_drive_detection_disabled);



	if ( is_drive_detection_disabled == TRUE ) {  // THIS IS CRASHES SOMETIMES AND WHEN I USE THE BUSCARD 		
		
		gotoxy(starting_x+14,starting_y+8); // if drive 8, dusoplay it 
		printf("Dectect disabled.");

	} else {

		gotoxy(starting_x+14,starting_y+8); // if drive 8, dusoplay it 
		detect_drive(8, 3); 				// move back to x+25 and wherey()+1

		gotoxy(starting_x+14,wherey()+1);	// if drive 9, dusoplay it 
		detect_drive(9, 3);  			// move back to x+25 and wherey()+1

		gotoxy(starting_x+14,wherey()+1); // if drive 8, dusoplay it 
		detect_drive(10, 3); 			// move back to x+25 and wherey()+1

		gotoxy(starting_x+14,wherey()+1);	// if drive 9, dusoplay it 
		detect_drive(11, 3);  			// move back to x+25 and wherey()+1	

	};//endif 

	// positon befiore returnging to console 
	// textcolor(    2-1 );
	gotoy(wherey()+1);

};//end func 















// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************


struct tm set_time;
time_t set_time_offset;
time_t curtime;
struct tm *loctime;
unsigned int user_date_input;



void display_date_nice() {

	char buffer[40];

	/* Get the current time. */
	curtime = time (NULL)+set_time_offset;

	/* Convert it to local time representation. */
	loctime = localtime (&curtime);

	/* Print out the date and time in the standard format. */
	//fputs (asctime (loctime), stdout);

	/* Print it out in a nice format. */
	strftime (buffer, 40, "%A, %B %d, %Y.\n", loctime);
	fputs (buffer, stdout);
	strftime (buffer, 40, "%I:%M:%S %p.\n", loctime);
	fputs (buffer, stdout);

};//end func 

void display_time_nice() {

	char buffer[40];

	/* Get the current time. */
	curtime = time (NULL)+set_time_offset;

	/* Convert it to local time representation. */
	loctime = localtime (&curtime);

	/* Print out the date and time in the standard format. */
	//fputs (asctime (loctime), stdout);

	/* Print it out in a nice format. */
	// strftime (buffer, 40, "%A, %B %d, %Y.\n", loctime);
	// fputs (buffer, stdout);
	strftime (buffer, 40, "%I:%M:%S %p", loctime);
	fputs (buffer, stdout);

};//end func 


void set_date() { 

	unsigned int user_date_input;

	/* Get the current time. */
	curtime = time (NULL)+set_time_offset;

	printf("Year: ");
	scanf("%i", &user_date_input);
    set_time.tm_year = user_date_input-1900;  // Year - 1900 - Unix Epoch

	printf("Mon: ");
	scanf("%i", &user_date_input);
    set_time.tm_mon = user_date_input-1;        // Month, where 0 = jan

	printf("Day: ");
	scanf("%i", &user_date_input);
    set_time.tm_mday = user_date_input;         // Day of the month
    
	printf("Hour: ");
	scanf("%i", &user_date_input);
    set_time.tm_hour = user_date_input;   // current hour assuming 24-hour am pm clock 

	printf("Min: ");					
	scanf("%i", &user_date_input);
    set_time.tm_min = user_date_input;  

	printf("Sec: ");					
	scanf("%i", &user_date_input);
    set_time.tm_sec = user_date_input;  

    //printf("Set date and time.\n");

	//printf("Enter Seconds #: ");					
	//scanf("%i", &user_date_input);
    set_time.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown

    set_time_offset = mktime(&set_time);

    set_time_offset = set_time_offset; // - (60*60); // Why is this needed? WTF??? 

    set_time_offset = set_time_offset - curtime;

};//end func 


// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************












void screensaver(void) {

	unsigned char current_x = 15;
	unsigned char current_y = 10;

	_randomize();

	clrscr();

	// bgcolor(     1-1 );
	// bordercolor( 1-1 ); 
	// textcolor(   2-1 );

	while(1){

		if ( (rand()&1) == 1) { //RAND_MAX 32767
			current_x++;
		} else {
			current_x--;
		};//end if 

		if (current_x > 27) {
			current_x = 27;
		};//end if 

		if ( (rand()&1) == 1) {
			current_y++;
		} else {
			current_y--;
		};//end if 

		if (current_y > 14) {
			current_y = 14;
		};//end if 

		clrscr(); 

		if (current_bgcolor == 2-1) { // blue but like it's offset... 
			textcolor( 1-1 ); // if background is blue, use black 
		} else {
			textcolor( 2-1 ); // if background isn't blue, use blue 
		};//switch		gotoxy(current_x+1, current_y+10);

		gotoxy(current_x+1, current_y+10);

		display_time_nice();

		display_logo(current_x,current_y); //x cannot be greater than rand, y cannot be greater than 15
		wait(32766);
		wait(32766);

		if (kbhit() != 0) {
			cgetc();
			clrscr();
			/* White on L.Blue on Blue */
			// textcolor(    2-1 );
			// bgcolor(     15-1 );
			// bordercolor(  7-1 ); 
			break;
		};//end if 

	};//end while loop

	textcolor  ( current_textcolor   );
	bgcolor    ( current_bgcolor     );
	bordercolor( current_bordercolor );

};//end func






// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// MAIN 
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************


int main( int argc, char* argv[] ) {


	reboot_register.pc = 0xFCE2; // this is the value that get sys'ed to reboot 


	// ********************************************************************************
	// SET DEFAULT ALIASES 
	// ********************************************************************************
	
	set_alias( "ls"       , "list"        );   
	set_alias( "dir"      , "list"        );   
	set_alias( "directory", "list"        );	
	set_alias( "del"      , "delete"      );     
	set_alias( "rm"       , "delete"      );     
	set_alias( "ren"      , "rename"      );  
	set_alias( "md"       , "make-dir"    );       
	set_alias( "mkdir"    , "make-dir"    );       
	set_alias( "rd"       , "remove-dir"  );       
	set_alias( "rmdir"    , "remove-dir"  ); 	
	set_alias( "cls"      , "clear"       );    
	set_alias( "cp"       , "copy"        );   
	set_alias( "cat"      , "type"        );           	
	set_alias( "cd.."     , "cd .."       ); 
	set_alias( "cd/"      , "cd /"        ); 
	set_alias( "ss"       , "screensaver" ); 
	set_alias( "quit"     , "exit"        ); 
	set_alias( "endcli"   , "exit"        ); 	
	set_alias( "ver"      , "version"     ); 
	set_alias( "lic"      , "licence"     ); 
        

	// ********************************************************************************
	// SET DEFAULT HOTKEYS 
	// ********************************************************************************
	
	set_hotkey(1,"cls");
	set_hotkey(2,"");
	set_hotkey(3,"ls");		
	set_hotkey(4,"");
	set_hotkey(5,"screensaver");
	set_hotkey(6,"");
	set_hotkey(7,"help");
	set_hotkey(8,"");

	// ********************************************************************************
	// PROCESS ARGUMENTS
	// ********************************************************************************
	

	if ( argc > 1  &&  matching("-ddd",argv[1]) ) {
		have_device_numbers_changed = TRUE; // this is loaded like this: RUN:REM -ddd // RUN:REM ARG1 " ARG2 IS QUOTED" ARG3 "" ARG5		
	};//end if 


	// ********************************************************************************
	// PROGRAM START 
	// ********************************************************************************



	/* White on L.Blue on Blue */
	set_profile_colors(13);

	// Start the prompt input stuff 
	starting_x = wherex();
	starting_y = wherey();

	// Display the logo, hardware info, and play the chirp 
	display_title_screen(have_device_numbers_changed); // this disabled the drive detection rountine 

	//set current directory to whatevr it is 
	dev = getcurrentdevice();
	//printf("Startup Drive D%i\n", dev);

	// ********************************************************************************
	// COMMAND PROCESSING LOOP
	// ********************************************************************************
	
	while(1) {	

		// ERASE EVERYTHING!	
		memset( entered_keystrokes,        0, sizeof(entered_keystrokes)    );

		memset( user_input_processed,      0, sizeof(user_input_processed)      );	
		memset( user_input_command_string, 0, sizeof(user_input_command_string) );
		memset( user_input_arg1_string,    0, sizeof(user_input_arg1_string)    );
		memset( user_input_arg2_string,    0, sizeof(user_input_arg2_string)    );
		memset( user_input_arg3_string,    0, sizeof(user_input_arg3_string)    );
		memset( user_input_arg4_string,    0, sizeof(user_input_arg4_string)    );		

		user_input_command_number = 0 ;
		user_input_arg1_number    = 0 ;
		user_input_arg2_number    = 0 ;
		user_input_arg3_number    = 0 ;
		user_input_arg4_number    = 0 ;

		user_input_command_type   = 'e' ;
		user_input_arg1_type      = 'e' ;
		user_input_arg2_type      = 'e' ;
		user_input_arg3_type      = 'e' ;
		user_input_arg4_type      = 'e' ;

		number_of_user_inputs     = 0   ;

		// PRINT PROMPT
		gotox(SCREEN_LEFT_EDGE);
		cputs(prompt);
		cputc(CURSOR_CHARACTER);
		gotox(wherex()-1);
		position_in_string = 0;
		position_at_prompt_x = wherex();
		position_at_prompt_y = wherey();

		// Below happens once we've processed a command, 
		// but it should probably be done at the beginning 
		memset(entered_keystrokes,0,sizeof(entered_keystrokes)); // this won't be needed once I'm done testing

		// ********************************************************************************
		// KEYBOARD INPUT LOOP
		// ********************************************************************************
		
		while(1) {		

			wait_for_keypress(); // wait for a keypress, after flushing the buffer 
			keystroke = cgetc();


			// ********************************************************************************
			// PROCESS KEYSTROKE
			// ********************************************************************************


			// ********************************************************************************
			// ENTER
			// ********************************************************************************
			if (keystroke == 13 || keystroke == 141) { 	 // return or shift/cmd return 					

			    if ( position_in_string == strlen(entered_keystrokes) ) {
			    	printf(" \n"); // this erases the cursor 
			    } else {
			    	printf("%c\n",entered_keystrokes[position_in_string]); //this erases teh cyursor and restores the character underneth it 
			    };//
		
				//TODO: This needs to be different. A scan to test if all the chars are spaces, which is tested before before copying the current to teh previosu 
				remove_extra_spaces(entered_keystrokes); 

				if (strlen(entered_keystrokes) != 0) {
					strcpy(previous_entered_keystrokes, entered_keystrokes); //backup command so pressing up cursor will recall it 
				};//end if 
				break; // break out of this loop to beginning processing and executing command


			// ********************************************************************************
			// BACKSPACE
			// ********************************************************************************
			} else if ( keystroke == 20 || keystroke == 19 || keystroke == 8 ) { // BACKSPACE = 20  or CLR HOME or CTRL+H
		    
				cursor_backspace();


			// ********************************************************************************
			// DELETE
			// ********************************************************************************
			} else if (keystroke == 148  || keystroke == 147 || keystroke == 27) { // CMD+DEL or SHIFT+DEL or CTRL+: as FORWARD DELETE

				cursor_delete();


			// ********************************************************************************
			// UP CURSOR
			// ********************************************************************************
			} else if (keystroke == 145 || keystroke == 9) {	// UP    = 145 or CTRL+I = 9
				
				cursor_up();


			// ********************************************************************************
			// DOWN CURSOR
			// ********************************************************************************
			} else if (keystroke == 17 || keystroke == 11) {	// DOWN  = 17  or CTRL+K = 11

				cursor_down();


			// ********************************************************************************
			// LEFT CURSOR
			// ********************************************************************************
			} else if (keystroke == 157 || keystroke == 10) {	// LEFT  = 157 or CTRL+J = 10

				cursor_left();
			 

			// ********************************************************************************
			// RIGHT CURSOR
			// ********************************************************************************
			} else if (keystroke == 29 || keystroke == 12) {	// RIGHT = 29  or CTRL+L = 12
				
				cursor_right();


			// ********************************************************************************
			// CTRL+C / RUN STOP
			// ********************************************************************************
			} else if (keystroke == 3) {	// CTRL+C / RUN/STOP = 3

			    cputc(' ');
				gotoy(wherey()+1);
				gotox(SCREEN_LEFT_EDGE);
				cputs(prompt);
				cputc(CURSOR_CHARACTER);
				gotox(wherex()-1);			
				memset(entered_keystrokes,0,sizeof(entered_keystrokes));


			// ********************************************************************************
			// HOME (CTRL+U)
			// ********************************************************************************
			} else if (keystroke == 21) { //|| keystroke == 95) {	// CTRL+H (TEMP british pound key)

				cursor_home();		


			// ********************************************************************************
			// END (CTRL+O)
			// ********************************************************************************
			} else if (keystroke == 15) { // || keystroke == 92) {	// CTRL+: (TEMP british pound key)

				cursor_end();


			// ********************************************************************************
			// F1
			// ********************************************************************************
			} else if (keystroke == 133) {	// F1

				load_hotkey_command(1);
				break;


			// ********************************************************************************
			// F2 (SHIFT+F1)
			// ********************************************************************************
			} else if (keystroke == 137) {	// F2 (SHIFT+F1)

				load_hotkey_command(2);
				break;


			// ********************************************************************************
			// F3
			// ********************************************************************************
			} else if (keystroke == 134) {	// F3

				load_hotkey_command(3);
				break;


			// ********************************************************************************
			// F4 (SHIFT+F3)
			// ********************************************************************************
			} else if (keystroke == 138) {	// F4 (SHIFT+F3)

				load_hotkey_command(4);
				break;


			// ********************************************************************************
			// F5
			// ********************************************************************************
			} else if (keystroke == 135) {	// F5

				load_hotkey_command(5);
				break;


			// ********************************************************************************
			// F6 (SHIFT+F5)
			// ********************************************************************************
			} else if (keystroke == 139) {	// F6 (SHIFT+F5)

				load_hotkey_command(6);
				break;


			// ********************************************************************************
			// F7
			// ********************************************************************************
			} else if (keystroke == 136) {	// F7

				load_hotkey_command(7);
				break;


			// ********************************************************************************
			// F8 (SHIFT+F7)
			// ********************************************************************************
			} else if (keystroke == 140) {	// F8 (SHIFT+F7)

				load_hotkey_command(8);
				break;

			// ********************************************************************************
			// TAB COMPLETION LEFT ARROW 
			// ********************************************************************************
			} else if (keystroke == 95) { // We are using LEFT ARROW for tab complete, because teh COmodoore 64 doesn't have a TAB key. HOwever, now we can't enter an arrow character, so it's compendated below 

				// do stuff


				// Go over the input string backwards and extract the last characters 

				//entered_keystrokes[position_in_string]
				//MAX_LENGTH_COMMAND

				unsigned char tab_length = 0;
				unsigned char entered_keystrokes_length = 0;
				unsigned char tab_complete_position = 0;
				unsigned char *tab_complete_pointer;
				unsigned char new_entered_keystrokes[MAX_LENGTH_COMMAND];

				entered_keystrokes_length = strlen(entered_keystrokes);
				tab_complete_pointer = entered_keystrokes; // update pointer so that it points to teh beginning character of the entered_keystrokes string
				tab_length = 0;
				memset( new_entered_keystrokes, 0, sizeof(new_entered_keystrokes) );				

				// remove cursor, we will put it back when we redraw the line with a cursor_end();
				cputc(' ');
				

				for (i = (entered_keystrokes_length-1) ; i > 0 ; i--) {

					if (entered_keystrokes[i] == ' ') {
						break;
					} else {
						tab_length++;
					};//end if 

				};//end for 

				tab_complete_position = entered_keystrokes_length-tab_length;

				tab_complete_pointer = tab_complete_pointer + tab_complete_position ; // update pointer to point at the char in entered_keystrokes
				
				//loop						
				dir_file_count(dir_file_total); // get the total number of files to copy and store in dir_file_total 

				//printf("\ndir_file_total:%i\n", dir_file_total);

				for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

					if (loop_k == 0) { //first entry is the disk anme 
						//do nothing
					} else { 
						dir_goto_file_index(loop_k);
						//printf("dir_ent.name:%s\n",dir_ent.name);//debug
						if (strncmp(tab_complete_pointer,dir_ent.name,tab_length) == 0) { 				// 	use strncmp to compare the first X chars in the current file to the extracted text
							strncpy(new_entered_keystrokes,entered_keystrokes,tab_complete_position); 	// copy beginning of entered keystrokes
							strcat(new_entered_keystrokes,dir_ent.name); 								// aoppend the found filename to teh end
							strcpy(entered_keystrokes,new_entered_keystrokes); 							//update teh entered_keystrokes with teh tab completed version					
							// cursor_end(); // put this at the end  																
							break; 																		// break out ot eh loop to avoid loading teh resat of the files for no reason 
						} else {
							// do nothing 																// maybe play a bell/beep of some kind
						};//end_if 
					};//end if 

				};//end for 

				cursor_home();
				cursor_end(); //erase the current entered text on teh screen, update teh screen with teh newly tab completed version, update teh screen cursor which can all be done with cursor_end()

			// ********************************************************************************
			// LEFT ARROW 
			// ********************************************************************************
			} else if (keystroke == 6) { // CTRL + <- = 6 // we need this becuase left arrow is tab complete, but a filename might have an arrow in it, so CTRL plus LEFT ARROW will now emit an arrow character to compensate
				keystroke = 95; //replcace CTRL LEFT ARROW with LEFT ARROW
				cursor_add_character();


			// ********************************************************************************
			// TYPEABLE KEYS : 
			// ********************************************************************************
			//  Spacebar = 32 Shift+Spacebar = 160
			//  ! = 33. ) = 40
			//  0 = 48, 9 = 57, (Numbers 48-57)
			//  a = 65, z = 90, A = 193,Z = 218 (Letters 65-90 193-218)
			//  ! = 33, ) = 41, 
			// <- = 95, + = 43, - = 45,B# = 92
			//  @ = 64, * = 42,/\ = 94			
			//  : = 58, ; = 59, = =	61, [ = 91, ] = 93, 	    
			//  , = 44, . = 46, / = 47, < = 60. > = 62, ? = 63
			//
			// TYPEABLE KEYS TOTAL RANGES:
			// 33 41-47 58-64 91-95 
			// 
			// GRAPHIC TYPABLE KEYS:	
			// 161-169 
			// 166 220 168 
			// 171 179 177 178 163 183 184 162 185 175 164 223 222
			// 176 174 172 187 165 180 181 161 182
			// 173 189 188 190 191 170 167 
			//
			// GRAPHIC TYPABLE KEYS RANGES:
			// 161-168 170-185 187-223
			// ********************************************************************************			
			} else if ( keystroke ==  32 					  || 
					 // keystroke ==  160					  || // this character screws things up 
					   (keystroke >=  33 && keystroke <=  40) || 
					   (keystroke >=  41 && keystroke <=  47) || 
	 
					   (keystroke >=  48 && keystroke <=  57) || 
					   (keystroke >=  65 && keystroke <=  90) || 
					   (keystroke >= 193 && keystroke <= 218) || 

					   (keystroke >= 161 && keystroke <= 168) || 
					   (keystroke >= 170 && keystroke <= 185) || 
					   (keystroke >= 187 && keystroke <= 223) || 

					   (keystroke >=  58 && keystroke <=  64) || 
					   (keystroke >=  91 && keystroke <=  95)    ){	// TODO: Need to check if we are inserting characters into a full string, in which case do nothing 

				
				cursor_add_character();

			};//end if - END PROCESS KEYSTROKE

		};//end while loop - END KEYBOARD INPUT LOOP


		// ********************************************************************************
		// PROCESS COMMAND
		// ********************************************************************************
			// printf("user_input_command_string:%s\n", user_input_command_string);

			// printf("user_input_arg1_string:%s\n", user_input_arg1_string);
			// printf("user_input_arg2_string:%s\n", user_input_arg2_string);
			// printf("user_input_arg3_string:%s\n", user_input_arg3_string);
			// printf("user_input_arg4_string:%s\n", user_input_arg4_string);

			// printf("user_input_arg1_number:%i\n", user_input_arg1_number);
			// printf("user_input_arg2_number:%i\n", user_input_arg2_number);
			// printf("user_input_arg3_number:%i\n", user_input_arg3_number);
			// printf("user_input_arg4_number:%i\n", user_input_arg4_number);

			do_alias(entered_keystrokes); // look for teh alias string as the whole block of entered text
		    process_command();
			do_alias(user_input_command_string); // look again for an alias in the extracted command from teh entere text 

																	
																								
	

			// if (is_string_only_numbers(user_input_arg1_string) == 1) {
			// 	user_input_arg1_number = atoi(user_input_arg1_string);
			// };//end if 

			//unsigned long int user_input_arg1_number                        = 0   ;  


			// user_input_arg1_number = 11111; 

			// printf("user_input_command_string:%s\n", user_input_command_string);

			// printf("user_input_arg1_string:%s\n", user_input_arg1_string);
			// printf("user_input_arg2_string:%s\n", user_input_arg2_string);
			// printf("user_input_arg3_string:%s\n", user_input_arg3_string);
			// printf("user_input_arg4_string:%s\n", user_input_arg4_string);

			// printf("user_input_arg1_number:%i\n", user_input_arg1_number);
			// printf("user_input_arg2_number:%i\n", user_input_arg2_number);
			// printf("user_input_arg3_number:%i\n", user_input_arg3_number);
			// printf("user_input_arg4_number:%i\n", user_input_arg4_number);

			// printf ("is_string_only_numbers(user_input_arg1_string):%i\n", is_string_only_numbers(user_input_arg1_string) );
			// printf ("atoi(user_input_arg1_string):%i\n", atoi(user_input_arg1_string) );


		// ********************************************************************************
		// EXECUTE COMAMNDS
		// ********************************************************************************


		// ********************************************************************************
		// CLEAR COMMAND 
		// ********************************************************************************
		if        ( matching("clear",user_input_command_string) ) {

			if (number_of_user_inputs == 1) {
				clrscr();
			} else {
				printf("Err args: %s\n", user_input_command_string);
			};//end if 


		// ********************************************************************************
		// ALIAS COMMAND 
		// ********************************************************************************
		} else if ( matching("alias",user_input_command_string) ) {

			if (number_of_user_inputs == 1) { // there should be a var number_of_arguments as well 
				display_alias_all();
			} else if (number_of_user_inputs == 2 && matching("-clear",user_input_arg1_string)  ) {
				if (they_are_sure() == TRUE) {
					clear_alias_all();
				};//end if 
			} else if (number_of_user_inputs == 2) {
				clear_alias(user_input_arg1_string);
			} else if (number_of_user_inputs == 3) {
				set_alias(user_input_arg1_string,user_input_arg2_string);
			} else if (number_of_user_inputs == 4 && matching("=",user_input_arg2_string)) {
				set_alias(user_input_arg1_string,user_input_arg3_string);
			} else {
				printf("Err args:%i\n", number_of_user_inputs);
			};//end if 		


		// ********************************************************************************
		// UNALIAS COMMAND 
		// ********************************************************************************
		} else if ( matching("unalias",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				
				case 1 : 				
	 				clear_alias(user_input_arg1_string);
			    break;				
	 				
			    default : 
			    	printf("Err args.\n");
			    //end default

			};//end switch	


		// ********************************************************************************
		// HOTKEY COMMAND 
		// ********************************************************************************
		} else if ( matching("hotkey",user_input_command_string) ) {

			if (number_of_user_inputs == 1) { // there should be a var number_of_arguments as well 
				display_hotkeys();
			} else if (number_of_user_inputs == 2 && matching("-clear",user_input_arg1_string)  ) {
				if (they_are_sure() == TRUE) {
					clear_all_hotkeys();
				};//end if 				
			} else if (number_of_user_inputs == 3) {
				set_hotkey(user_input_arg1_number,user_input_arg2_string);
			} else if (number_of_user_inputs == 4 && matching("=",user_input_arg2_string)) {
				set_hotkey(user_input_arg1_number,user_input_arg3_string);			
			} else {
				printf("Err args:%i\n", number_of_user_inputs);
			};//end if 	


		// ********************************************************************************
		// ECHO COMMAND 
		// ********************************************************************************
		} else if ( matching("echo",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				
				case 2 : 				
	 				printf("%s\n",user_input_arg1_string);
			    break;				
	 				
			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default

			};//end switch	


		// ********************************************************************************
		// HELP COMMAND 
		// ********************************************************************************
		} else if ( matching("help",user_input_command_string) ) {
			
			//     "                                       "	
			printf("ChiCLI Commands:\n");
			printf("help   about    version     sys-info\n"); 
			printf("alias  hotkey   profile-set color-set\n"); 
			printf("clear  list     d#:   cd    run   ./\n"); 
			printf("type   copy     rename      delete\n"); 
			printf("format status   dos-command drive-set\n"); 
			printf("peek   sys      validate    view-mem\n"); 
			printf("poke   keycodes screensaver initialize\n"); 
			printf("exit   reboot   restart     shutdown\n"); 
			printf("echo   time     datetime    licence\n"); 
			printf("chirp  make-dir remove-dir  debug-args\n");
			printf("uiec-hide-ext   uiec-show-ext\n");
			printf("Please see chicli-readme for details.\n");


		// ********************************************************************************
		// VERSION COMMAND
		// ********************************************************************************
		} else if ( matching("version",user_input_command_string) ) {

			display_title_text();
			//printf("Licenced under terms of the GNU GPL v3\n");
			printf("Ver:  %s\n", VERSION);
			printf("Date: %s\n",__DATE__);
			printf("Time: %s\n",__TIME__);


		// ********************************************************************************
		// ABOUT COMMAND
		// ********************************************************************************
		} else if ( matching("about",user_input_command_string) ) {
			
			display_title_text();
			display_description_text();
			printf("github.com/chironb/ChiCLI\n");
			//printf("Licenced under terms of the GNU GPL v3\n");
			//printf("www.gnu.org/licenses/\n");



		// ********************************************************************************
		// LICENCE COMMAND
		// ********************************************************************************
		} else if ( matching("licence",user_input_command_string) ) {
			
			display_title_text();
			printf("Licenced under terms of the GNU GPL v3.\nwww.gnu.org/licenses/\nTo read, enter: type chicli-licence\n");

			// printf("This program is free software: you can\nredistribute it and/or modify it under\nthe terms of the GNU General Public\nLicense as published by the Free\nSoftware Foundation.\nThis program is distributed in the\nhope that it will be useful, but\nWITHOUT ANY WARRANTY.\n");
			// printf("Licence download: www.gnu.org/licenses/\n");


		    //printf("This program is free software: you can\nredistribute it and/or modify it under\nthe terms of the GNU General Public\nLicense as published by the Free\nSoftware Foundation, either version 3\nof the License, or (at your option) any later version.\n\n");
		    //printf("This program is distributed in the hope\nthat it will be useful, but WITHOUT ANY\nWARRANTY; without even the implied\nwarranty of MERCHANTABILITY or FITNESS\nFOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more\ndetails.\n\n");
		    //printf("You should have received a copy of the\nGNU General Public License along with\nthis program.  If not, see\n<https://www.gnu.org/licenses/>.\n");


		// ********************************************************************************
		// KEYCODES COMMAND
		// ********************************************************************************
		} else if ( matching("keycodes",user_input_command_string) ) {
                  //                                        "
			printf("  * Hit any key RUNSTOP/CTRL-C quits *\n"); 			

			do {

				do {
					//nothing
				} while ( kbhit() == 0);//end do 
				
				get_key = cgetc();

				printf("Keycode: DEC:%i \n", get_key);

			// RUN/STOP or CTRL-C
			} while( get_key != 3);//end do 


		// ********************************************************************************
		// ./ COMMAND
		// ********************************************************************************
		} else if ( user_input_command_string[0] == '.' && user_input_command_string[1] == '/' ) {		

			// switch (number_of_user_inputs) {
				
			// 	case 2 : 				
					if (they_are_sure() == TRUE) {			
						strcpy(extracted_program_name,user_input_command_string+2); // THIS GIVES ME EVERYTHING EXCEPT THE FIRST TWO CHARACTERS 
						printf("Running: %s\n", extracted_program_name);				
			    		exec(extracted_program_name, user_input_arg1_string); 
		    			return EXIT_SUCCESS;
		    		};//end if 
			    // break;				
	 			
	 			// // TODO: rewrite this to inlude all args with quotes on the command line 	
			  //   default : 
			  //   	printf("Echo args error. # of args:%i\n", number_of_user_inputs);
			  //   //end default


		// ********************************************************************************
		// RUN COMMAND
		// ********************************************************************************
		} else if ( matching("run",user_input_command_string) ) {

			// switch (number_of_user_inputs) {
				
			// 	case 2 : 				
					if (they_are_sure() == TRUE) {			
						//strcpy(extracted_program_name,user_input_command_string+2); // THIS GIVES ME EVERYTHING EXCEPT THE FIRST TWO CHARACTERS 
						printf("Running: %s \"%s\"\n", user_input_arg1_string, user_input_arg2_string);				
			    		exec(user_input_arg1_string, user_input_arg2_string); 
		    			return EXIT_SUCCESS;
		    		};//end if 
			    // break;				
	 			
			// TODO: rewrite this to inlude all args with quotes on the command line 	

		    // TODO: LOADING FROM ANOTHER DRIVE ISSUE
		    // Detect if the startup drive is different from the drive we are trying to load from 
		    // and then either use the exec() from teh startup drive, or, 
		    // use teh dracopy trick to laod from the other drive
		    // then email the mailing list ot find out how to change teh drive that exec() loads from 

		// ********************************************************************************
		// EXIT COMMAND 
		// ********************************************************************************
		} else if ( matching("exit",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {
				exit(0);
			};//end if
				
	 				
		// ********************************************************************************
		// RESTART COMMAND 
		// ********************************************************************************
		} else if ( matching("restart",user_input_command_string) ) {




			switch (number_of_user_inputs) {
				// don't ask if there are any parameters, like -y for autoamticaly saying yes 
				case 2 : 				
						printf("Running %s\nPlease wait...\n", PROGRAM_NAME);
						exec(PROGRAM_NAME, "-r"); // This reloads whatever the program name is 
						return EXIT_SUCCESS;      // I don't even know if we ever get here.
			    break;				
	 				
			    default : 
			    	if (they_are_sure() == TRUE) {
						printf("Running %s\nPlease wait...\n", PROGRAM_NAME);
						exec(PROGRAM_NAME, "-r"); // This reloads whatever the program name is 
						return EXIT_SUCCESS;      // I don't even know if we ever get here.
					};//end if
			    //end default

			};//end switch	


			


		// ********************************************************************************
		// REBOOT COMMAND 
		// ********************************************************************************
		} else if ( matching("reboot",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {
	    		printf("\n\nRebooting...\n\n");
	    		reboot_register.pc = 0xFCE2;
	    		_sys(&reboot_register);
			};//end if


		// ********************************************************************************
		// DOS-COMMAND COMMAND 
		// ********************************************************************************
		} else if ( matching("dos-command",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {
				result = cbm_open(1, dev, 15, user_input_arg1_string);
				printf( "cbm_open result: %i \n", result );
				cbm_close(1);
			};//end if


		// ********************************************************************************
		// SHUTDOWN COMMAND 
		// ********************************************************************************
		} else if ( matching("shutdown",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {

			    clrscr();

			    /* Blue on Black */
				textcolor(    7-1 );
				bgcolor(      1-1 );
				bordercolor(  1-1 ); 

		    	//printf("\n\n\n\n\n\n\n\n\n\n\n");
		    	gotoy(11);
		    	printf("           You may now safely\n");
		    	printf("      ");
		    	cputc(230);
		    	printf(" shut off your Commodore. ");
		    	cputc(230); // we put this here because it makes a pattern when we push keystroke onto the keyboard buffer below
		
		    	//Addresses 631-640 (Hex: $0277-$0280, name KEYD) is the keyboard buffer used by Commodore basic. 
		    	// UP 6-SPACES HOME | 145 32 32 32 32 32 32 19

		    	POKE(0x0277,145); // UP
		    	POKE(0x0278, 32); // r
		    	POKE(0x0279, 32); // e
		    	POKE(0x027A, 32); // a
		    	POKE(0x027B, 32); // d
		    	POKE(0x027C, 32); // y
		    	POKE(0x027D, 32); // .		    	
		    	POKE(0x027E,145); // UP
		    	// POKE(0x027F,230); // CHECKERBOARD
		    	// POKE(0x0280,157); // LEFT
		    	POKE(0x00C6, 10); // Number of characters in the keyboard buffer

		    	exit(EXIT_SUCCESS);  // whcih allows us to call exit properly, or we could say "press return to restart" and have hidden black on black text with SYS 64738 waiting for them 
				
			};//end if


		// ********************************************************************************
		// DETECT_DRIVE COMMAND 
		// ********************************************************************************
		} else if ( matching("detect-drive",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 1 : 				
	 					result = detect_drive(dev, TRUE);
	 					// printf("result:%i\n", result);
						// printf("E#:%i TR:%i BL:%i\n", error_number, error_track , error_block );
						// printf("ED:%s\n", error_message );
			    break;	

				case 2 : 				
	 					result = detect_drive(user_input_arg1_number, TRUE);
	 					// printf("result:%i\n", result);
						// printf("E#:%i TR:%i BL:%i\n", error_number, error_track , error_block );
						// printf("ED:%s\n", error_message );
			    break;				
	 				
			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default

			};//end switch	


		// ********************************************************************************
		// STATUS COMMAND 
		// ********************************************************************************
		} else if ( matching("status",user_input_command_string) ) {


			if        ( matching( "d8:",user_input_arg1_string) ) { 
				user_input_arg1_number = 8;
				use_dcopy = TRUE;
			} else if ( matching( "d9:",user_input_arg1_string) ) { 
				user_input_arg1_number = 9;  
				use_dcopy = TRUE;
			} else if ( matching("d10:",user_input_arg1_string) ) { 
				user_input_arg1_number = 10;
				use_dcopy = TRUE;
			} else if ( matching("d11:",user_input_arg1_string) ) { 
				user_input_arg1_number = 11;
				use_dcopy = TRUE;
			} else if ( matching("d12:",user_input_arg1_string) ) { 
				user_input_arg1_number = 12;
				use_dcopy = TRUE;	
			} else if ( matching("d13:",user_input_arg1_string) ) { 
				user_input_arg1_number = 13;
				use_dcopy = TRUE;
			} else if ( matching("d14:",user_input_arg1_string) ) { 
				user_input_arg1_number = 14;
				use_dcopy = TRUE;
			} else if ( matching("d15:",user_input_arg1_string) ) { 
				user_input_arg1_number = 15;
				use_dcopy = TRUE;															
			} else {
				use_dcopy = FALSE;
			};//end if

			switch (number_of_user_inputs) {
				
				case 1 : 				
	 					result = get_status(dev, TRUE);
			    break;		

				case 2 : 				
	 					result = get_status(user_input_arg1_number, TRUE);
			    break;			
	 				
			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default

			};//end switch	


		// ********************************************************************************
		// INITIALIZE COMMAND 
		// ********************************************************************************
		} else if ( matching("initialize",user_input_command_string) ) {

			// Initialize drive (INITIALIZE) e.g. to detect a disk change
			// OPEN 1,8,15,"I0":CLOSE 1

			result = get_status(dev, TRUE);

			if (result != 255) {
				printf("Initializing...\n");

				result = cbm_open(1, dev, 15, "i0");
				cbm_close(1);

				if (result == 0) {
					printf("Done.\n");
				} else {
					printf("Err: %i\n", result);
				};//end if 
			};//end if 


		// ********************************************************************************
		// VALIDATE COMMAND 
		// ********************************************************************************
		} else if ( matching("validate",user_input_command_string) ) {

			// Initialize drive (INITIALIZE) e.g. to detect a disk change
			// OPEN 1,8,15,"I0":CLOSE 1

			result = get_status(dev, TRUE);

			if (result != 255) {
				printf("Validating...\n");

				result = cbm_open(1, dev, 15, "v0");
				cbm_close(1);

				if (result == 0) {
					printf("Done.\n");
				} else {
					printf("Err: %i\n", result);
				};//end if 
			};//end if 


		// ********************************************************************************
		// UIEC-HIDE-EXT COMMAND 
		// ********************************************************************************
		} else if ( matching("uiec-hide-ext",user_input_command_string) ) {

			// OPEN lf,dv,15:PRINT#lf,"XE{+|-}":CLOSElf
			// + is used to enable hiding.
			// - is used to disable hiding.

			result = get_status(dev, TRUE);

			if (result != 255) {
				printf("Setting UIEC to hide extentions...\n");

				result = cbm_open(1, dev, 15, "xe+");
				cbm_close(1);

				if (result == 0) {
					printf("Done.\n");
				} else {
					printf("Err: %i\n", result);
				};//end if 
			};//end if 

					
		// ********************************************************************************
		// UIEC-SHOW-EXT COMMAND 
		// ********************************************************************************
		} else if ( matching("uiec-show-ext",user_input_command_string) ) {

			// OPEN lf,dv,15:PRINT#lf,"XE{+|-}":CLOSElf
			// + is used to enable hiding.
			// - is used to disable hiding.

			result = get_status(dev, TRUE);

			if (result != 255) {
				printf("Setting UIEC to show extentions...\n");

				result = cbm_open(1, dev, 15, "xe-");
				cbm_close(1);

				if (result == 0) {
					printf("Done.\n");
				} else {
					printf("Err: %i\n", result);
				};//end if 
			};//end if 


		// ********************************************************************************
		// CD COMMAND 
		// ********************************************************************************
		} else if ( matching("cd",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				
				// case 1 : 				
	 		// 		printf("TODO: Print current directory.\n");
			 //    break;		

				case 2 : 				
					strcpy (drive_command_string,"cd");

					if ( matching("..",user_input_arg1_string) ) {
						strcat (drive_command_string,command_cdback);

					} else 	if ( matching("/",user_input_arg1_string) ) {
						strcat (drive_command_string,"//");

					} else {
						strcat (drive_command_string,"/");
						strcat (drive_command_string,user_input_arg1_string);
						strcat (drive_command_string,"/");

					};//end if 					

					result = cbm_open(1, dev, 15, drive_command_string);		
					cbm_close(1);
			    break;			
	 				
			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default

			};//end switch	


		// ********************************************************************************
		// MOUNT COMMAND 
		// ********************************************************************************
		} else if ( matching("mount",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 2 : 				
					strcpy (drive_command_string,"cd:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch


		// ********************************************************************************
		// UNMOUNT COMMAND 
		// ********************************************************************************
		} else if ( matching("unmount",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 1 : 				
					strcpy (drive_command_string,"cd");
					strcat (drive_command_string,command_cdback);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch


		// ********************************************************************************
		// D8: COMMAND 
		// ********************************************************************************
		} else if ( matching("d8:",user_input_command_string) ) {

			change_drive(8);


		// ********************************************************************************
		// D9: COMMAND 
		// ********************************************************************************
		} else if ( matching("d9:",user_input_command_string) ) {

			change_drive(9);


		// ********************************************************************************
		// D10: COMMAND 
		// ********************************************************************************
		} else if ( matching("d10:",user_input_command_string) ) {

			change_drive(10);


		// ********************************************************************************
		// D11: COMMAND 
		// ********************************************************************************
		} else if ( matching("d11:",user_input_command_string) ) {

			change_drive(11);


		// ********************************************************************************
		// D12: COMMAND 
		// ********************************************************************************
		} else if ( matching("d12:",user_input_command_string) ) {

			change_drive(12);


		// ********************************************************************************
		// D13: COMMAND 
		// ********************************************************************************
		} else if ( matching("d13:",user_input_command_string) ) {

			change_drive(13);


		// ********************************************************************************
		// D14: COMMAND 
		// ********************************************************************************
		} else if ( matching("d14:",user_input_command_string) ) {

			change_drive(14);


		// ********************************************************************************
		// D15: COMMAND 
		// ********************************************************************************
		} else if ( matching("d15:",user_input_command_string) ) {

			change_drive(15);									


		// ********************************************************************************
		// MAKE-DIR COMMAND 
		// ********************************************************************************
		} else if ( matching("make-dir",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 2 : 				
					strcpy (drive_command_string,"md:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch


		// ********************************************************************************
		// REMOVE-DIR COMMAND 
		// ********************************************************************************
		} else if ( matching("remove-dir",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 2 : 	
					strcpy (drive_command_string,"rd:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch


		// ********************************************************************************
		// DELETE COMMAND 
		// ********************************************************************************
		} else if ( matching("delete",user_input_command_string) ) {

			// delete test --> deletes test in the current dir
			// delete *    --> deletes all teh files iun teh current dir 

			switch (number_of_user_inputs) {
				case 2 : 	
					if ( matching("*",user_input_arg1_string) ) { 			// copy * d8:     

						dir_file_count(dir_file_total); // get the total number of files to copy and store in dir_file_total 
						printf("Files to delete: %i\n", dir_file_total);

						if (they_are_sure() == TRUE) {

							for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

								if (loop_k == 0) { //first entry is the disk anme 
									//do nothing
								} else { 
									dir_goto_file_index(loop_k);
									detected_filetype = dir_ent.type;
									strcpy (user_input_arg1_string, dir_ent.name);   

									strcpy (drive_command_string,"s:");
									strcat (drive_command_string,user_input_arg1_string);

									printf("-> %s ",user_input_arg1_string);   
									result = cbm_open(1, dev, 15, drive_command_string);
									cbm_close(1);
									printf("[Deleted]\n");
								};//end if 

							};//end for 

						};//end if 

					} else {  

						strcpy (drive_command_string,"s:");
						strcat (drive_command_string,user_input_arg1_string);
						result = cbm_open(1, dev, 15, drive_command_string);
						cbm_close(1);

					};//end if 
				break;	

				default : 
					printf("Err args:%i\n", number_of_user_inputs);
				//end default

			};//end switch










		// ********************************************************************************
		// FORMAT COMMAND 
		// ********************************************************************************
		} else if ( matching("format",user_input_command_string) ) {

			// format test 1 

			switch (number_of_user_inputs) {
				case 3 : 				
					strcpy (drive_command_string,"n0:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,",");
					strcat (drive_command_string,user_input_arg1_string);

					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch







		// ********************************************************************************
		// RENAME COMMAND 
		// ********************************************************************************
		} else if ( matching("rename",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 3 : 				
					strcpy (drive_command_string,"r:");
					strcat (drive_command_string,user_input_arg2_string);
					strcat (drive_command_string,"=");
					strcat (drive_command_string,user_input_arg1_string);

					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch



		// ********************************************************************************
		// DETECT-FILETYPE COMMAND 
		// ********************************************************************************
		} else if ( matching("detect-filetype",user_input_command_string) ) {	 // Detect File Type: PRG SEQ REL USR DIR 		

			detect_filetype(user_input_arg1_string, TRUE);

			// case  2 : printf("DIR"); break;	// DIR
			// case 16 : printf("SEQ"); break; // SEQ
			// case 17 : printf("PRG"); break; // PRG
			// case 18 : printf("USR"); break; // USR
			// case 19 : printf("REL"); break;	// REL
			// default : printf("???"); //end default






		// ********************************************************************************
		// DRIVE-SET COMMAND 
		// ********************************************************************************
		} else if ( matching("drive-set",user_input_command_string) ) {	 

			// drive-set -uiec 10 
			// drive-set -1541 11 
			
			switch (number_of_user_inputs) {
				case 3 : 	
					if ( matching("-1541",user_input_arg1_string) ) {

						c1541_set(dev, user_input_arg2_string);

					} else if ( matching("-uiec",user_input_arg1_string) ) {

						uiec_set(dev, user_input_arg2_string);

					};//end if
			    break;	

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default
			};//end switch






// 		} else if ( matching("1541-set",user_input_command_string) ) {	 

// 				// drive-set 11    (uses current active drive you're in) 

// 				c1541_set(dev, user_input_arg1_string);




// 		// ********************************************************************************
// 		// UIEC-SET COMMAND 
// 		// ********************************************************************************
// 		} else if ( matching("uiec-set",user_input_command_string) ) {	 

// 				// drive-set 11    (uses current active drive you're in) 

// 				uiec_set(dev, user_input_arg1_string);




		// ********************************************************************************
		// POKE COMMAND 
		// ********************************************************************************
		} else if ( matching("poke",user_input_command_string) ) {	 

			peeked = PEEK(user_input_arg1_number);

	    	printf("Memory at:%u was DEC:%i CHR:%c \n", user_input_arg1_number, peeked, peeked );

			POKE(user_input_arg1_number, user_input_arg2_number);

			peeked = PEEK(user_input_arg1_number);

	    	printf("Memory at:%u now DEC:%i CHR:%c \n", user_input_arg1_number, peeked, peeked );


		// ********************************************************************************
		// PEEK COMMAND 
		// ********************************************************************************
		} else if ( matching("peek",user_input_command_string) ) {	

			peeked = PEEK(user_input_arg1_number);

	    	printf("Memory at:%u is DEC:%i CHR:%c \n", user_input_arg1_number, peeked, peeked );



		// ********************************************************************************
		// PEEK-HEX COMMAND 
		// ********************************************************************************
		} else if ( matching("peek-hex",user_input_command_string) ) {	

			//int sscanf(const char *str, const char *format, ...)

	    	// printf("Enter address to peek in HEX: ");
			sscanf(user_input_arg1_string, "%x", &peek_address); //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX

			peeked = PEEK(peek_address);

	    	printf("Memory at:%04X is HEX:%02X CHR:%c \n", peek_address, peeked, peeked );



		// ********************************************************************************
		// POKE-HEX COMMAND 
		// ********************************************************************************
		} else if ( matching("poke-hex",user_input_command_string) ) {	 

			sscanf(user_input_arg1_string, "%x", &poke_address); //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX
			sscanf(user_input_arg2_string, "%x", &poked);		 //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX

			peeked = PEEK(poke_address);

	    	printf("Memory at:%04X was HEX:%02X CHR:%c \n", poke_address, peeked, peeked );

			POKE(poke_address, poked);

			peeked = PEEK(poke_address);

	    	printf("Memory at:%04X now HEX:%02X CHR:%c \n", poke_address, peeked, peeked );



		// ********************************************************************************
		// SYS COMMAND 
		// ********************************************************************************
		} else if ( matching("sys",user_input_command_string) ) {	 

	// struct regs reboot_register;
	// 	    	printf("\n\nRestarting...\n\n");
	    	//reboot_register.pc = 0xFCE2;
	//     	_sys(&reboot_register);

	  //   	printf("Enter address to SYS: ");
			// scanf("%d", &poke_address);	

			// printf("Are you sure you want to execute this?\n");			
			// printf("(Yes/No): ");
			// scanf("%s", &user_input_arg2);

			// 0xFCE2 - 64738 // "%x" for hex and "%d" for double int
			// top of memory 0xFFFF 65535
			// a GOOD TEST: sys 4000 - this scres up the program, but it stilll kinda works, so you can then test taht REBOOT wroks, for eaxcmpel  
			sscanf(user_input_arg1_string, "%d", &reboot_register.pc); //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX

			//printf("Address to sys to: %d\n", reboot_register.pc);

			if (they_are_sure() == TRUE) {

				_sys(&reboot_register);

			};//end if


			//sscanf(user_input_arg2_string, "%x", &poked);		 //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX


			// if ( strcmp(user_input_arg2, "Yes") == 0 || strcmp(user_input_arg2, "yes") == 0 || strcmp(user_input_arg2, "y") == 0 || strcmp(user_input_arg2, "Y") == 0 ) {

			// 	printf("Executing...\n");

				// reboot_register.pc = poke_address;
				// _sys(&reboot_register);

			// };//end if 




		// ********************************************************************************
		// VIEW-MEM COMMAND 
		// ********************************************************************************
		} else if ( matching("view-mem",user_input_command_string) ) {	 

	  //   	printf("Enter low address in HEX: ");
			// scanf("%x", &start_address);	

	  //   	printf("Enter high address in HEX: ");
			// scanf("%x", &end_address);

			sscanf(user_input_arg1_string, "%x", &start_address); //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX
			sscanf(user_input_arg2_string, "%x", &end_address);		 //TODO: THIS FUNCTION ALONE COSTS FUCKING 2K!!!!! find a better way to convert HEX




			hex_display_position_in_file = start_address ;
			hex_display_position_x       = 0 ;
			hex_display_position_y       = 0 ;

			
			clrscr();



				for ( i = 0 ; i <= (end_address - start_address) ; i++) {
					
					peeked = PEEK(start_address+i);

					// Example:
					//      
					//      0  1  2  3  4  5  6  7
					//      |  |  |  |  |  |  |  |
					// 0000 01 08 0F 08 C6 07 9E 32 ....F..2
					// 0008 30 36 35 20 5B 33 5D 00 065 [3].
					// 0010 00 00 A9 00 8D 20 D0 8D ..?.. P.
					// |    |  |  |  |  |  |  |  |  |      |  |
					// 0    5  8  11 14 17 20 23 26 29     36 39                 

					// PAUSE AT NEAR BOTTOM OF SCREEN 
					// if hex_display_position_x == 7 && hex_display_position_y == 24
					// hex_display_position_x = 0
					// increment hex_display_position_y = 0
					if (hex_display_position_y == 23) {
						hex_display_position_x = 0;
						hex_display_position_y = 0;
						gotox(0);
						gotoy(24);
						printf("  Any key for more or RUN/STOP to quit "); 			
						do {
							//nothing
						} while (kbhit() == 0);//end do 
						get_key = cgetc();
						if (get_key == 3) { // RUN/STOP or CTRL-C
							printf("\n");
							read_bytes = 0;
							break;
						};//end if 
						clrscr();
					};//end if 

					// if hex_display_position_x == 0  
					// move to x=0
					// DISPLAY ADDRESS
					if (hex_display_position_x == 0){
						gotox(0);
						gotoy(hex_display_position_y);
						printf("%04X", hex_display_position_in_file);
					};//end if 

					// DISPLAY HEX AND CHAR 
					// move to ((hex_display_position_x * 3)+5)
					// display hex value 
					// move to (hex_display_position_x+29) 
					// display character if valid to display 
					gotox((hex_display_position_x * 3) + 6);
					printf("%02X", peeked);
					gotox(hex_display_position_x + 31) ;
					cputc( convert_char(peeked) );				

					// UPDATE COUNTERS
					// if hex_display_position_x == 7 && hex_display_position_y != 24
					// hex_display_position_x = 0
					// increment hex_display_position_y
					// increment hex_display_position_in_file by 8
					if (hex_display_position_x == 7 && hex_display_position_y != 23) {						
						hex_display_position_x = 0;
						hex_display_position_y++;						

					// if hex_display_position_x == 0 to 6
					// incremenmt hex_display_position_x 
					} else {
						hex_display_position_x++;							

					};//end if 

					hex_display_position_in_file++;

				};//end for 


			printf("\n");










		// ********************************************************************************
		// COPY COMMAND 
		// ********************************************************************************
		} else if ( matching("copy",user_input_command_string) ) {

			// copy test test2 --> creates a new file called test2 that contains the contents of test using copy()
			// copy test d8:   --> copies test to drive 8 using dcopy()
			// copy * d8:      --> copies all the files in the current folder to d8:
			// copy * stuff    --> ERROR for now, but in teh future, this should copy into another folder on the SD card when it's UIEC

			if        ( matching( "d8:",user_input_arg2_string) ) { 
				user_input_arg2_number = 8;
				use_dcopy = TRUE;
			} else if ( matching( "d9:",user_input_arg2_string) ) { 
				user_input_arg2_number = 9;  
				use_dcopy = TRUE;
			} else if ( matching("d10:",user_input_arg2_string) ) { 
				user_input_arg2_number = 10;
				use_dcopy = TRUE;
			} else if ( matching("d11:",user_input_arg2_string) ) { 
				user_input_arg2_number = 11;
				use_dcopy = TRUE;
			} else if ( matching("d12:",user_input_arg2_string) ) { 
				user_input_arg2_number = 12;
				use_dcopy = TRUE;	
			} else if ( matching("d13:",user_input_arg2_string) ) { 
				user_input_arg2_number = 13;
				use_dcopy = TRUE;
			} else if ( matching("d14:",user_input_arg2_string) ) { 
				user_input_arg2_number = 14;
				use_dcopy = TRUE;
			} else if ( matching("d15:",user_input_arg2_string) ) { 
				user_input_arg2_number = 15;
				use_dcopy = TRUE;															
			} else {
				use_dcopy = FALSE;
			};//end if

		switch (number_of_user_inputs) {
			case 3 : 	
				if ( matching("*",user_input_arg1_string) && use_dcopy == TRUE) { 			// copy * d8:     

					dir_file_count(dir_file_total); // get the total number of files to copy and store in dir_file_total 
					printf("Files to copy: %i\n", dir_file_total);

					if (they_are_sure() == TRUE) {

					    for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

						    if (loop_k == 0) { //first entry is the disk anme 
						    	//do nothing
						    } else { 
						    	dir_goto_file_index(loop_k);
								detected_filetype = dir_ent.type;
								strcpy (user_input_arg1_string, dir_ent.name);    
								dcopy();
							};//end if 

						};//end for 

					};//end if 

				} else if ( use_dcopy == TRUE ) {
					printf("Detected file: %s ", user_input_arg1_string );                                  
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE);
					dcopy();

				} else if ( use_dcopy == FALSE ) {
					copy();

				} else { // error 
					printf("Copy syntax error.\n");

				};//end if 
	    	break;	

	    	default : 
	    		printf("Err args:%i\n", number_of_user_inputs);
	    	//end default

		};//end switch








		// ********************************************************************************
		// TYPE COMMAND 
		// ********************************************************************************
		} else if ( matching("type",user_input_command_string) ) {

			switch (number_of_user_inputs) {

				case 2 : 				
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // detect filetype
					switch(detected_filetype){
						case 2 : // case  2 : printf("DIR"); break;	// DIR
							printf("Error: this is a directory.\n");
						break;

						case 16 : // case 16 : printf("SEQ"); break; // SEQ
							type_text(user_input_arg1_string); // if SEQ use type
						break;

						case 17 : // case 17 : printf("PRG"); break; // PRG
							type_prg(user_input_arg1_string); // if PRG use type-prg
						break;

						case 18 : // case 18 : printf("USR"); break; // USR
							type_hex(user_input_arg1_string); // if USR use type-hex
						break;

						case 19 : // case 19 : printf("REL"); break;	// REL
							type_hex(user_input_arg1_string); // if USR use type-hex
						break;

						default : // case  2 : printf("DIR"); break;	// DIR
							printf("Error: unknown type.\n");
						//end default 
					};//end switch
				break;

				case 3 :
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // detect filetype
					if (detected_filetype == 255) {
						printf("Error: unknown type.\n");
						break;
					};//end if 
					if (matching("-hex",user_input_arg2_string)) {
						type_hex(user_input_arg1_string); 
					} else {
						printf("Unknown option: %s\n", user_input_arg2_string);
					};//end if 
				break;

			    default : 
			    	printf("Err args:%i\n", number_of_user_inputs);
			    //end default

			};//end switch


		// // ********************************************************************************
		// // TYPE-PRG COMMAND 
		// // ********************************************************************************
		// } else if ( matching("type-prg",user_input_command_string) ) {

		// 	type_prg(user_input_arg1_string);


		// // ********************************************************************************
		// // TYPE-HEX COMMAND 
		// // ********************************************************************************
		// } else if ( matching("type-hex",user_input_command_string) ) {

		// 	type_hex(user_input_arg1_string);






		// ********************************************************************************
		// LIST COMMAND 
		// ********************************************************************************
		} else if ( matching("list",user_input_command_string) ) {

		    printf("Directory of D%i:\n", dev);

		    result = cbm_opendir(1, dev); // need to deal with errors here

		    //printf("cbm_opendir result: %i\n", result);

		    displayed_count = 2;//this is two becuase we alrieady displaeyd two lines of text 

		    for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

				if (displayed_count == 24) {

					printf("     * Press any key to continue *"); 			
					do {
						//nothing
					} while (kbhit() == 0);//end do 
					get_key = cgetc();
					gotox(0);
					printf("                                       ");
					gotox(0);
					if (get_key == 3) { // RUN/STOP or CTRL-C
						printf("\n");
						break;
					};//end if 

					// gotox(0);
					// printf("                                       ");
					// gotox(0);
					//clrscr();
					displayed_count = 0;

				};//end if

		    	if (result != 0) {
		    		printf("Err: cbm_opendir %i\n", result);
		    		break;
		    	};//end if 

			    result = cbm_readdir(1, &dir_ent);

			    if (number_of_files == 0) {
			    	//printf("\ncbm_readdir  result: %i\n", result);
			    	printf("Disk Name: %s\n", dir_ent.name);
			    	// the size is actually drive 0 or drive 1 from PET dual drives
			    } else if (result == 2) {

				    if (dir_ent.size == 0) {
				    	printf("   0 B");
				    } else if (dir_ent.size == 1) {
				    	printf(" 256 B");
				    } else if (dir_ent.size == 2) {
				    	printf(" 512 B");
				    } else if (dir_ent.size == 3) {
				    	printf(" 768 B");	
				    } else if (dir_ent.size == 4) {
				    	printf("1024 B");	 
				    } else {
				    	printf("%i KB", (dir_ent.size*256)/1024 );
				    };//end if   

			    	printf(" free disk space.\n", (dir_ent.size*256)/1024);
			    	break;

			    } else { 
			    	//printf("\ncbm_readdir  result: %i\n", result);
			    	// printf("File Name: %s\n", dir_ent.name);
				    // printf("File Block Size: %i\n", dir_ent.size);
				    // printf("File Type: %i\n", dir_ent.type);
				    // printf("File Access: %i\n ", dir_ent.access);    	
				    gotox(3);
				    //printf("\"%s\"", dir_ent.name );
				    printf("%s", dir_ent.name );

		   		    gotox(20);
				    if        (dir_ent.type ==  2) {
				    	printf("          (Dir)");
				    } else if (dir_ent.type == 16) {
				    	printf("SEQ");		    	
				    } else if (dir_ent.type == 17) {
				    	printf("PRG");
				    } else if (dir_ent.type == 18) {
				    	printf("USR");				        	
				    } else if (dir_ent.type == 19) {
				    	printf("REL");		    	
				    } else {
				    	printf("%i", dir_ent.type );
				    };//end if 
		   		    //printf("%i", dir_ent.type );
				    

				    if (dir_ent.type != 2) {

					    gotox(24);
					    if (dir_ent.size == 0) {
					    	printf("   0 B");
					    } else if (dir_ent.size == 1) {
					    	printf(" 256 B");
					    } else if (dir_ent.size == 2) {
					    	printf(" 512 B");
					    } else if (dir_ent.size == 3) {
					    	printf(" 768 B");	
					    } else if (dir_ent.size == 4) {
					    	printf("1024 B");	 
					    } else {
					    	file_size = (dir_ent.size*256)/1024;
					    	if (file_size <= 9) {
					    		printf("   %i KB", file_size);
					    	} else if (file_size <= 99) {
					    		printf("  %i KB", file_size);
					    	} else if (file_size <= 999) {
					    		printf(" %i KB", file_size);
					    	} else if (file_size <= 9999) {
					    		printf("%i KB", file_size);	
					    	} else {
					    		printf("%iKB", file_size);		
					    	};//end if     		
					    };//end if    			    	

					    gotox(32);
					    if (dir_ent.access == CBM_A_RO) {
					    	printf("R");
					    } else if (dir_ent.access == CBM_A_WO) {
					    	printf("W");
					    } else if (dir_ent.access == CBM_A_RW) {
					    	printf("R/W");
					    };//end if 

					};//end if 

					printf("\n");
					++displayed_count;


				};//end if 

			};//end for 

		    cbm_closedir(1);	







		// ********************************************************************************
		// CHIRP COMMAND 
		// ********************************************************************************
		} else if ( matching("chirp",user_input_command_string) ) {

			pet_chirp();


		// ********************************************************************************
		// SYS-INFO COMMAND 
		// ********************************************************************************
		} else if ( matching("sys-info",user_input_command_string) ) {

			// Display the logo, hardware info, and play the chirp 
			display_title_screen(have_device_numbers_changed);


		// ********************************************************************************
		// COLOR-SET COMMAND 
		// ********************************************************************************
		} else if ( matching("color-set",user_input_command_string) ) {
	
			printf("1-Blk 2-Wht 3-Red 4-Cyn 5-Pur 6-Grn\n");
			printf("7-Blu 8-Yel 9-Org 10-Br 11-Pk 12-DGr\n");
			printf("13-Gr 14-LG 15-LB 16-LG\n");
			printf("0-Skip\n");

			printf("Text: ");
			scanf("%i", &user_input_number1);
			if (user_input_number1 != 0) {
				--user_input_number1;
				textcolor(user_input_number1); 
			};//end if
			
			printf("Background: ");
			scanf("%i", &user_input_number1);
			if (user_input_number1 != 0) {
				--user_input_number1;
				bgcolor(user_input_number1); 
			};//end if

			printf("Border: ");
			scanf("%i", &user_input_number1);
			if (user_input_number1 != 0) {
				--user_input_number1;
				bordercolor(user_input_number1); 
			};//end if

			
		// ********************************************************************************
		// PROFILE-SET COMMAND 
		// ********************************************************************************
		} else if ( matching("profile-set",user_input_command_string) ) {


			switch (number_of_user_inputs) {
				case 2 : 
					if (	user_input_arg1_number == 0) {
						printf("Invalid profile.\n");	
					} else {	
						set_profile_colors(user_input_arg1_number);
						clrscr();
					};//end if 
			    break;	

			    default : 
					printf(" 1-CPET 2-CVIC-20 3-C64 4-CSX-64\n");
					printf(" 5-C128/VIC 6-C128/VDC 7-AmigaDOS \n");
					printf(" 8-White/Blue/L.Blue 9-B/W 10-W/B\n");
					printf("11-Grey1 12-Grey2 13 Default 0-Skip\n");
					printf("Select Color Profile:");
					scanf("%i", &user_input_number1);					

					set_profile_colors(user_input_number1);

					user_input_number1 = 0;

					clrscr();
			    //end default
			};//end switch








		// ********************************************************************************
		// DISPLAY-LOGO COMMAND 
		// ********************************************************************************
		} else if ( matching("display-logo",user_input_command_string) ) {

			display_logo(wherex()+1,wherey()+1);
			printf("\n");
			textcolor(    2-1 );


		// ********************************************************************************
		// SCREENSAVER COMMAND 
		// ********************************************************************************
		} else if ( matching("screensaver",user_input_command_string) ) {

			screensaver();


		// // ********************************************************************************
		// // DATETIME-SET COMMAND 
		// // ********************************************************************************
		// } else if ( matching("datetime-set",user_input_command_string) ) {

	 // 		set_date();
		// 	display_date_nice();


		// ********************************************************************************
		// DATETIME COMMAND 
		// ********************************************************************************
		} else if ( matching("datetime",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 1 : 
					display_date_nice();
			    break;	

				case 2 : 
					set_date();
					display_date_nice();
			    break;	

			    default : 
			    	printf("Invalid args.\n");
			    //end default
			};//end switch 
	   		

		// ********************************************************************************
		// TIME COMMAND 
		// ********************************************************************************
		} else if ( matching("time",user_input_command_string) ) {

	   		display_time_nice();
	   		printf("\n");


		// ********************************************************************************
		// DEBUG-ARGS COMMAND 
		// ********************************************************************************
		} else if ( matching("debug-args",user_input_command_string) ) {

			printf("Number of args:%i\n", argc);
		 	printf("Program Name:%s\n", argv[0]);	
		 	if (argc > 1)printf("Arg 1:%s\n", argv[1]);
		 	if (argc > 2)printf("Arg 2:%s\n", argv[2]);
		 	if (argc > 3)printf("Arg 3:%s\n", argv[3]);
		 	if (argc > 4)printf("Arg 4:%s\n", argv[4]);


		// ********************************************************************************
		// ERROR EMPTY COMMAND
		// ********************************************************************************
		} else if ( strlen(entered_keystrokes) == 0 ) {

			// printf("Error: nothing entered.\n");


		// ********************************************************************************
		// ERROR NO COMMAND FOUND
		// ********************************************************************************
		} else {

			printf("Unknown command: %s\n", user_input_command_string);


		};//end if 
		// ********************************************************************************
		// FINISHED COMMAND PROCESSING AND EXECUTING
		// ********************************************************************************


	};//end while loop
	// ********************************************************************************
	// COMMAND PROCESSING LOOP
	// ********************************************************************************


	return(0);


};//end main 















