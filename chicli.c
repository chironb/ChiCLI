// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2021 by: Chiron Bramberger
//
// ********************************************************************************

/* Compiling and deleting object files command, because who needs a make file!

cl65 -g -Osr -t c64 --static-locals  chicli.c  string_processing.c alias.c hotkeys.c hardware.c  commands.c -o chicli.prg && rm *.o && ls -l *.prg

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
    // along with this program.  If not, see <https://www.gnu.org/licenses/>

// ********************************************************************************


// ********************************************************************************
// VERSION
// ********************************************************************************

#define VERSION "v1.R8"
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
#include <device.h>
#include <ctype.h>

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

unsigned char read_drive_number;
unsigned int  read_bytes_free;
unsigned char first_partition = 0;
unsigned char last_partition;
unsigned char sd2iec_offset = 0;

unsigned char files_per_screen;
unsigned char listing_string[10] = "";

unsigned char screensaver_show_time = TRUE;
unsigned char screensaver_show_date = TRUE;

unsigned char rtc_device = 0;
												   //  text, background, border
unsigned char color_profiles[9][3] = {{06,01,01},  //  1 - C PET Style
								      {07,02,04},  //  2 - C VIC-20 Style
								      {15,07,15},  //  3 - C 64 Style
								      {14,12,14},  //  4 - C 128 VIC-II Style
								      {04,01,01},  //  5 - C 128 VDC Style
								      {01,02,02},  //  6 - Black on White
								      {02,01,01},  //  7 - White on Black
								      {02,12,13},  //  8 - Greyscale
								      {02,15,07}}; //  9 - Default
// 0 == not scanned
// 1 == detected
// 2 == NOT detected --> and therefore do not attempt to read a status, which would lock up the Commodore 64.
unsigned char  drive_detected[8]      = { 0,0,0,0,0,0,0,0 };
unsigned char  drive_detected_type[8] = { 0,0,0,0,0,0,0,0 }; // 0 = no drive detected, all others related to a specific drive

unsigned char alias_shortcut_list[MAX_ALIASES][MAX_ALIAS_LENGTH] ; // --> lots of RAM! like 2.5K !!!
unsigned char alias_command_list[MAX_ALIASES][MAX_ALIAS_LENGTH] ;  // --> see chicli.h to change this
unsigned char hotkeys_list[MAX_HOTKEYS][MAX_HOTKEY_LENGTH];

unsigned char entered_keystrokes[MAX_ENTERED_KEYSTROKES] ;
unsigned char position_at_prompt_x = 0;
unsigned char position_at_prompt_y = 0;	
unsigned char extracted_program_name[MAX_LENGTH_COMMAND];
unsigned char get_key = '\0';

unsigned char keystroke;
unsigned char previous_entered_keystrokes[MAX_ENTERED_KEYSTROKES] ;
unsigned char prompt[3] = "> "; // Example:  D8:games>  or:  D8:>   or just: >
unsigned char starting_x = 0;
unsigned char starting_y = 0;
unsigned char position_in_string = 0;
unsigned char previous_x = 0;
unsigned char previous_y = 0;

signed int result     = 0; // This needs to be signed, because cbm_read and  cbm_write return -1 in case of an error;
signed int result2    = 0;
signed int read_bytes = 0;

unsigned char i;  // shared by all for loops
unsigned char ii;

// ARRAYS
unsigned char disk_sector_buffer[MAX_DISK_SECTOR_BUFFER] = " "; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...

unsigned char user_input[MAX_ENTERED_KEYSTROKES]                = ""  ;
unsigned char user_input_processed[MAX_ENTERED_KEYSTROKES]      = ""  ;
 
unsigned char     user_input_command_type                       = 'e' ; // e for empty, s for string, n for number
unsigned char     user_input_arg1_type                          = 'e' ;
unsigned char     user_input_arg2_type                          = 'e' ;
unsigned char     user_input_arg3_type                          = 'e' ;

unsigned char 	  drive_command_string[MAX_LENGTH_COMMAND]		= ""  ;
unsigned char 	  drive_command_string2[MAX_LENGTH_COMMAND]		= ""  ;

unsigned char 	  user_input_command_string[MAX_LENGTH_COMMAND] = ""  ;
unsigned char     user_input_arg1_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char     user_input_arg2_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char     user_input_arg3_string[MAX_LENGTH_ARGS]       = ""  ;

unsigned long int user_input_command_number                     = 0   ;

unsigned int      user_input_arg1_number                        = 0   ;
unsigned int      user_input_arg2_number                        = 0   ;
unsigned int      user_input_arg3_number                        = 0   ;

unsigned char number_of_user_inputs = 0;

unsigned char * token_pointer_command;
unsigned char * token_pointer_arg1;
unsigned char * token_pointer_arg2;
unsigned char * token_pointer_arg3;

unsigned char original_position_at_prompt_y = 0;

unsigned int  error_number       ;
unsigned char error_message[32]  ;
unsigned char error_message2[32] ; // TODO: Save RAM here?
unsigned int  error_track        ;
unsigned int  error_block        ;

unsigned char dev = 0;
unsigned char par = '0';
unsigned char target_par = '0';
unsigned char source_par = '0';
unsigned char part_command[3] ;

struct        cbm_dirent dir_ent;  
unsigned int  number_of_files = 0;
unsigned char displayed_count = 0;

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
unsigned char sid_detected     = 0;
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

unsigned long int filesize = 0; //    filesize = (dir_ent.size*256)/1024;
unsigned int  display_filesize = 0; // trying to save space by not passing printf a long int

unsigned int stopwatch_start_stamp;


// ********************************************************************************
// FUNCTIONS
// ********************************************************************************

// ********************************************************************************
// NEW DIRECT DISK ACCESS FUNCTIONS FOR THE 1541 DISK DRIVE
// ********************************************************************************

// BEFORE: -rw-rw-r-- 1 chiron chiron 48044 Dec 25 16:39 chicli.prg
// AFTER:  -rw-rw-r-- 1 chiron chiron 47368 Dec 25 16:52 chicli.prg
// SAVING: 676 bytes!
unsigned char convert_partition_for_drive() {

	// Example: device_command[1] = convert_partition_for_drive();
	// Assumes:
	// dev = is the current device as a number
	// par = is the current partition as a character

	if ( get_drive_type(dev) == DRIVE_UIEC ) {
		return( par+1 );
	} else if (get_drive_type(dev) == DRIVE_1581 ) {
		return(  '0'  );
	} else {
		return(  par  );
	};//end-f

};//end-func


void change_1581_root_par() {
	// par = 0;
	strcpy(drive_command_string, "/");
	cbm_open(15, dev, 15, drive_command_string);
	cbm_close(15);
	printf("Partition root set.\n");
};//end-func


void list_and_change_1581_par(unsigned char change_par) {
	// to change: change_par == TRUE
	// to list:   change_par == FALSE

	memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));
	result = cbm_opendir(1, dev, "$"); // need to deal with errors here
	i = 0; // use this to count partitions

	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

		result = cbm_readdir(1, &dir_ent);

		if (result == 2) {
			break;
		};//end-if

		if (dir_ent.type == 1){ // dir_ent.name dir_ent.type dir_ent.size
			i++;
			if (i+47 == par && change_par == TRUE) { // if we are actually changing the partition
				printf(" %c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
				strcpy(drive_command_string, "/0:");
				strcat(drive_command_string, dir_ent.name);
				cbm_open(15, dev, 15, drive_command_string);
				cbm_close(15);
				printf("Partition %c set.\n", par+17);
				cbm_closedir(1);
				break;
			} else if (change_par == FALSE) {
				printf(" %c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
			};//end-if
		};//end-if

	};//end-for

	cbm_closedir(1); // This should only execute if we never find a partition at all.

};//end-func


unsigned char disk_label[16];
unsigned char disk_id[2];

void write_disk_label() {

	// Instead of passing a var, we use a global var for this purpose,
	// because it's faster in C programming esepcially on the 6502 processor.

	result = get_drive_type(dev);

	switch (result) {
		case DRIVE_1541 :
		case DRIVE_2031 :
		case DRIVE_SD2  :
		case DRIVE_UIEC : // This is supported but only for mounted D64 disk images.
			// These drives are supported! Keep executing.
		break;

		default :
			printf("Er: drv!\n"); // It's not one of the above drive. Break out of this function.
			return;
		break;
	};//end-switch

	// This is the original initial command DOS string for drive 0.
	strcpy(drive_command_string, "i0");
	drive_command_string[1] = par; // Here, we update the drive number with the current partition which is the variable par.

	// This is the block write command, file#:15 channel#:2 drive#:0 track#:18 sector#:0
	strcpy(drive_command_string2, "u1:2,0,18,0"); // Also used for: "u2:2,0,18,0"
	drive_command_string2[5] = par; // Here, we update the drive number with the current partition which is the variable par.

	result = cbm_open(15, dev, 15, drive_command_string); // Open with the init command.
	result = cbm_open( 2, dev,  2, "#"); // Open command channel.

	result = cbm_write(15, drive_command_string2, sizeof("u1:2,0,18,0")); // // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u1:2,0,18,0" --> Send the block read command to load track and sector to RAM command.
	result = cbm_write(15, "b-p:2,144", sizeof("b-p:2,144")); // Send the buffer pointer command to move the pointer to the start of the disk label.
	strcpy(disk_sector_buffer,disk_label); // Create our new disk label string.
	read_bytes = cbm_write(2, disk_sector_buffer, 16); // Write our new disk label's 16 bytes to the commmand channel.
	drive_command_string2[1] = '2';

	result = cbm_write(15, drive_command_string2, sizeof("u2:2,0,18,0")); // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u2:2,0,18,0" --> Send the buffer pointer command to move the pointer to the start of the disk label.
	result = cbm_open( 15, dev, 15, drive_command_string); // Open with the init command.

	cbm_close(2); // Close the command channel.
	cbm_close(15); // Close the other channel???

	result = cbm_open(15, dev, 15, drive_command_string); // Open with the init command.
	cbm_close(15); // Close the channel

};//end-func


void write_disk_id() {

	// Instead of passing a var, we use a global var for this purpose,
	// because it's faster in C programming esepcially on the 6502 processor.

	result = get_drive_type(dev);

	switch (result) {
		case DRIVE_1541 :
		case DRIVE_2031 :
		case DRIVE_SD2  :
		case DRIVE_UIEC : // This is supported but only for mounted D64 disk images.
			// These drives are supported! Keep executing.
		break;

		default :
			printf("Er: drv!\n"); // It's not one of the above drive. Break out of this function.
			return;
		break;
	};//end-switch

	// This is the original initial command DOS string for drive 0.
	strcpy(drive_command_string, "i0");
	drive_command_string[1] = par; // Here, we update the drive number with the current partition which is the variable par.

	// This is the block write command, file#:15 channel#:2 drive#:0 track#:18 sector#:0
	strcpy(drive_command_string2, "u1:2,0,18,0"); // Also used for: "u2:2,0,18,0"
	drive_command_string2[5] = par; // Here, we update the drive number with the current partition which is the variable par.

	result = cbm_open(15, dev, 15, drive_command_string); // Open ???
	result = cbm_open( 2, dev, 2, "#"); // Open command channel.

	result = cbm_write(15, drive_command_string2, sizeof("u1:2,0,18,0")); // // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u1:2,0,18,0" --> Send the block read command to load track and sector to RAM command.
 	result = cbm_write(15, "b-p:2,162", sizeof("b-p:2,162")); // Send the buffer pointer command to move the pointer to the start of the disk label.
	strncpy(disk_sector_buffer,disk_id,2); // Create our new disk label string.

	read_bytes = cbm_write(2, disk_id, 2); // Write our new disk label's 16 bytes to the commmand channel.
	drive_command_string2[1] = '2';

	result = cbm_write(15, drive_command_string2, sizeof("u2:2,0,18,0")); // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u2:2,0,18,0" --> Send the buffer pointer command to move the pointer to the start of the disk label.

	result = cbm_open(15, dev, 15, drive_command_string); // Open ???
	read_bytes = cbm_read(2, disk_sector_buffer, sizeof(disk_sector_buffer)); // Read the 16 bytes that are the current disk label from the commmand channel.
	cbm_close(2); // Close the command channel.
	cbm_close(15); // Close the other channel???

	result = cbm_open(15, dev, 15, drive_command_string); // Open with the init command.
	cbm_close(15); // Close the channel

};//end-func


void BYTE_TO_BINARY(byte) {
    cputc('B');
    cputc('I');
    cputc('N');
    cputc(':');
    cputc((byte & 0x80 ? '1' : '0'));
    cputc((byte & 0x40 ? '1' : '0'));
    cputc((byte & 0x20 ? '1' : '0'));
    cputc((byte & 0x10 ? '1' : '0'));
    cputc((byte & 0x08 ? '1' : '0'));
    cputc((byte & 0x04 ? '1' : '0'));
    cputc((byte & 0x02 ? '1' : '0'));
    cputc((byte & 0x01 ? '1' : '0'));
};//end_macro


// ********************************************************************************
// GENERIC PROMPTS FUNCTIONS
// ********************************************************************************

unsigned char they_are_sure(void) {

	unsigned char are_they_sure_keystroke;

	printf("Are you sure?(Y/N) ");

	while(1){

		wait_for_keypress(); 	// wait for a keypress, after flushing the buffer 
		are_they_sure_keystroke = cgetc();

		switch(are_they_sure_keystroke) {
		    case 'Y' :
		    case 'y' :
				printf("Ok.\n");
				return(1);
			break;

		    // There was this weird bug where I would hit Y but it would act like I didn't. 
		    // I can't re-create it reliably, so instead it's going to look for Y or N explictly.
		    case 'N' :
		    case 'n' :
				printf("Aborting.\n");
				// printf("are_they_sure_keystroke #:%u C:'%c'\n",are_they_sure_keystroke,are_they_sure_keystroke); // Debugging this weird error I can't always re-create.
				return(0);
			break;
		};//end_switch

	};//end-while

};//end-func

void hit_any_key() {

	printf("* Hit key *");

	wait_for_keypress(); 	 // wait for a keypress, after flushing the buffer 
	cgetc(); 				 // grab key but dion't keep it throw it away because we don't give any fucks!

	gotox(SCREEN_LEFT_EDGE); // blank the line and return to the left edge to continue printing 
	printf("                             ");
	gotox(SCREEN_LEFT_EDGE);

};//end-if

unsigned char convert_char(unsigned char char_to_convert) {

	// TODO: 1.R7b: Rewrite this with switch statements
	// TODO: THIS FUNCTION IS GARBAGE! Figure out WTF you really want it to do and REWRITE IT!!!
	if ((		char_to_convert >= 0x20 && \
	     		char_to_convert <= 0x7f)   ||   // if it *IS* a good displaying character
	     		char_to_convert >= 0xA0    ){

		// So basically do nothing???
		if (char_to_convert >= 65 && char_to_convert <= 90){   // if lower alphabet
			//char_to_convert = char_to_convert + 32 ;                    // then shift up 
		} else if (char_to_convert >= 97 && char_to_convert <= 122) { // else if upper alphabet
			//char_to_convert = char_to_convert - 32 ;                    // then shift down 
		};//end-if

	} else {

		char_to_convert = '.' ;                                     // output this instead 

	};//end-if

	return char_to_convert ;

};//end func


// TODO: 1.R7b: This should be in string_processing.c
char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    };//end-while
    return str;
};//end-func

void display_logo(unsigned char x, unsigned char y) {

	unsigned char current_x = 0;
	unsigned char current_y = 0;
	unsigned char current_color = 0;

	// Newest 64 Rainbow Logo    
	// 0123456789X12
	// "     #| #|#  "; 0
	// "    #| #|#|  "; 1
	// "   #| #| |#  "; 2
	// "  #| #|  ##  "; 3
	// " #| #|##|#|#|"; 4
	// "#| #|##|#|#| "; 5
	// "|#    |# ##  "; 6
	// "#|    #| #|  "; 7
	// "|#    |# |#  "; 8
	// "#|    #| ##  "; 9
	// " ##  #|  #|  "; 10
	// "  #|#|   |   "; 11

	unsigned char binary_logo[] = {
		/*"     ## "  "###  "*/
		 0b00000110, 0b11100000,
		/*"    ## #"  "###  "*/
		 0b00001101, 0b11100000,
		/*"   ## ##"  " ##  "*/
		 0b00011011, 0b01100000,
		/*"  ## ## "  " ##  "*/
		 0b00110110, 0b01100000,
		/*" ## ####"  "#####"*/
		 0b01101111, 0b11111000,
		/*"## #####"  "#### "*/
		 0b11011111, 0b11110000,
		/*"##    ##"  " ##  "*/
		 0b11000011, 0b01100000,
		/*"##    ##"  " ##  "*/
		 0b11000011, 0b01100000,
		/*"##    ##"  " ##  "*/
		 0b11000011, 0b01100000,
		/*"##    ##"  " ##  "*/
		 0b11000011, 0b01100000,
		/*" ##  ## "  " ##  "*/
		 0b01100110, 0b01100000,
		/*"  ####  "  " #   "*/
		 0b00111100, 0b01000000
	};//end-array

	#define CHAR_SQUARE 232

	unsigned char color_fade[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

	for ( i = 0 ; i <= 23 ; i++) { // first to last position in the binary logo bitmap array

		textcolor( color_fade[current_y] );

		for ( ii = 8 ; ii > 0 ; ii--) {
			if (get_bit(binary_logo[i],ii-1) ) {
				revers(TRUE);
				gotoxy    ( x+current_x , y+current_y );
				cputc( CHAR_SQUARE );
			} else {
				revers(FALSE);
				cputc( ' ' );
			};//end-if
			current_x++;
		};//end-for

		if (get_bit(i,0)) {
			current_y++;
			current_x = 0;
		};//end-if

	};//end-for

	revers(FALSE);
	textcolor( current_textcolor );

};//end func

void sys_info(unsigned char is_drive_detection_disabled) {	

    unsigned char how_many_fit_vice_pattern = 0;
    unsigned char how_many_fit_thec64mini_pattern = 0;
    unsigned char how_many_nibbles_fit_emulator_pattern = 0;

	starting_x = wherex();
	starting_y = wherey();

	ntscpal_detected = detect_ntsc_pal() ; // printf("N:%i ",ntscpal_detected);
		sid_detected = detect_sid()      ; // printf("S:%i ",sid_detected);
	 kernal_detected = detect_kernal()   ; // printf("K:%i ",kernal_detected);
	  model_detected = detect_model()    ; // printf("M:%i ",model_detected);
	    cpu_detected = detect_cpu()      ; // printf("C:%i \n",cpu_detected);

	display_logo(starting_x,starting_y+1);

	gotoxy(starting_x+14,starting_y+2); cputs("Model");
	gotoxy(starting_x+14,starting_y+3); cputs("Type");
	gotoxy(starting_x+14,starting_y+4); cputs("OS");
	gotoxy(starting_x+14,starting_y+5); cputs("CPU");
	gotoxy(starting_x+14,starting_y+6); cputs("GPU");
	gotoxy(starting_x+14,starting_y+7); cputs("Sound");

	#define SYS_INFO_RESULTS_OFFSET 20

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+2);

    // Simple default emulation detection
    // Based on the emulator VICE
	// Unused (16 bytes) at the end of screen memory: $07F8-$07FF 2040-2047
	// VICE default for this area of mem is to initialize it:
	// 00 00 FF FF FF FF 00 00
	// 00 00 FF FF FF FF 00 00

    for(i = 0; i <= 7 ; i++){
        if ( (PEEK(56296L+i) & 0x0F) == 0 ) how_many_nibbles_fit_emulator_pattern++;
        if (  PEEK(2032+i) == 0xFF)         how_many_fit_thec64mini_pattern++; // This range tends to be not all FF on a single real C64 I have, whereas the range.
    };//end_for

    if (PEEK(2024) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2025) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2026) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2027) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2028) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2029) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2030) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2031) == 0x00) how_many_fit_vice_pattern++;

	if ( how_many_fit_vice_pattern == 8 && how_many_nibbles_fit_emulator_pattern == 8) { // so if 1 out of 7 values is either 0x00 or 0xFF 
	    cputs("VICE Emu C");
	} else if ( how_many_fit_thec64mini_pattern == 8 && how_many_nibbles_fit_emulator_pattern == 8) { // so if 1 out of 7 values is either 0x00 or 0xFF 
	    cputs("TheC64   C");
	} else {
	    cputs("Commodore "); 
	};//end_switch

	switch(model_detected) {
		case 0 : cputs("128D");       break;
		case 1 : cputs("128");        break;	
		case 2 : cputs("128DCR");     break;
		case 3 : cputs("64 (Early)"); break; // TEXT TOO LONG! "64 (Early)" Fall through to next
		case 4 : cputs("64");         break;
		case 5 : cputs("64C");        break;
		case 6 : cputs("64");         break;
		case 7 : cputs("64C");        break;
		case 8 : cputs("SX-64");      break;
		case 9 : cputs("4064");       break;	// } else if (kernal_detected == 6)                                               { return(9); // Model: Educator 64
	};//end switch 

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+3);
	switch(ntscpal_detected) {
		case 0 : cputs("NTSC"); break;
		case 1 : cputs("PAL");  break;
	};//end switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+4);
	switch(kernal_detected) {
		case  1 : cputs("R1 901227-01"); break;
		case  2 : cputs("R2 901227-02"); break;
		case  3 : cputs("R3 901227-03"); break;
		case  4 : cputs("R3 251104-04"); break;
		case  5 : cputs("R3 251104-01"); break;
		case  6 : cputs("R3 901246-01"); break; // Fair to say this is most similar to a stock R3 901227-03 kernal. From here: "Note that some patches of 901227-03 are included." http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/revisions.txt
		case 10 : cputs("R3 JiffyDOS "); break;
		case 11 : cputs("SX-64 JiffyDOS"); break;
		default : cputs("?"); //end default
		// default : printf("?: %i",kernal_detected); break;
	};//end switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+5);
	cputs("MOS ");

	switch(cpu_detected) {
		case 9  : cputs("8502"); break;
		case 10 : cputs("8500"); break;
		case 11 : cputs("6510"); break;
		default : cputs("?");    //end-default
	};//end-switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+6);
	cputs("VIC-II ");

    switch(cpu_detected) {
        case 9 :
            switch(ntscpal_detected) {
                case 0 : cputs("8564");      break; // VIC-II in the NTSC C128
                case 1 : cputs("8566/8569"); break; // VIC-II in the PAL C128
            };//end_switch
        break;

        default :
            switch(sid_detected) {
                case 1 : 
                    switch(ntscpal_detected) {
                        case 0 : cputs("6567");      break; // VIC-II in the NTSC C64C
                        case 1 : cputs("6569/6572"); break; // VIC-II in the PAL C64C
                    };//end_switch
                break;

                case 2 : 
                    switch(ntscpal_detected) {
                        case 0 : cputs("8562"); break; // VIC-II in the NTSC C64C
                        case 1 : cputs("8565"); break; // VIC-II in the PAL C64C
                    };//end_switch
                break;

            };//end_switch
        break;

    };//end_switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+7);
	cputs("SID ");
	switch(sid_detected) {
		case    1 : cputs("6581");   break;
		case    2 : cputs("8580");   break;
		default   : /* do nothing */ break; //end default 
	};//end switch

	if ( is_drive_detection_disabled == TRUE ) {  // THIS IS CRASHES SOMETIMES AND WHEN I USE THE BUSCARD 		

		gotoxy(starting_x+14,starting_y+8); // if drive 8, dusoplay it 
		printf("Detect disabled.");

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

	gotoy(wherey()+1);

};//end func 


// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************

void find_rtc_device(void) {

	// TODO: This assume the SD2IEC has at the very least the software clock. Doesn't deal with SD2IEC devices with nothing enabled at all.

	rtc_device = 0;

	for (i = 8 ; i <= 15 ; i++) {
		if ( get_drive_type(i) == DRIVE_UIEC ) {
			rtc_device = i;
		};//end-if
	};//end-for

};//end-func

void display_date_nice() {

	find_rtc_device();
	if ( rtc_device == 0 ) {
		// printf("Error, no RTC found!\n");
		return;
	};//end-if

	result = cbm_open(15, rtc_device, 15, "t-ra");
	do {
		read_bytes = cbm_read(15, disk_sector_buffer, sizeof(disk_sector_buffer));
	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop
	cbm_close (15);

	// Convert the day-of-the-week to upper case.
	disk_sector_buffer[0] = toupper(disk_sector_buffer[0]);

	// Display the date sub-string
	printf("%.14s", disk_sector_buffer);

};//end func 

void display_time_nice() {

	find_rtc_device();
	if ( rtc_device == 0 ) {
		// printf("Error, no RTC found!\n");
		return;
	};//end-if

	result = cbm_open(15, rtc_device, 15, "t-ra");
	do {
		read_bytes = cbm_read(15, disk_sector_buffer, sizeof(disk_sector_buffer));
	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop
	cbm_close (15);

	// Convert the time am or pm to upper case.
	disk_sector_buffer[23] = toupper(disk_sector_buffer[23]);
	disk_sector_buffer[24] = toupper(disk_sector_buffer[24]);

	// Display the time sub-string
	printf("%.11s", disk_sector_buffer+14);

};//end func 

// Variable to get input.
unsigned char user_weekday_input[3];
unsigned char user_ampm_input[2];
unsigned char user_date_input[2];

void date_helper() {
	scanf("%s", &user_date_input);
	strcat(disk_sector_buffer,user_date_input);
};//end-func

void set_date() {

	find_rtc_device();
	if ( rtc_device == 0 ) {
		printf("Error, no RTC found!\n");
		return;
	};//end-if

	// Varible reused to build string for setting the date.
	strcpy(disk_sector_buffer,"t-wa");

	printf("Weekday XXXX:");
	scanf("%s", &user_weekday_input);
	user_weekday_input[0] = tolower(user_weekday_input[0]);
	user_weekday_input[1] = tolower(user_weekday_input[1]);
	user_weekday_input[2] = tolower(user_weekday_input[2]);
	strcat(disk_sector_buffer,user_weekday_input);
	strcat(disk_sector_buffer,". ");

	printf("Mon. XX:");
	date_helper();
	strcat(disk_sector_buffer,"/");

	printf("Day. XX:");
	date_helper();
	strcat(disk_sector_buffer,"/");

	printf("Year XX:");
	date_helper();
	strcat(disk_sector_buffer," ");

	printf("12 Hour:");
	date_helper();
	strcat(disk_sector_buffer,":");

	printf("Mins XX:");
	date_helper();
	strcat(disk_sector_buffer,":");

	printf("Secs XX:");
	date_helper();
	strcat(disk_sector_buffer," ");

	printf("AM/PM XX:");
	scanf("%s", &user_ampm_input);
	user_ampm_input[0] = tolower(user_ampm_input[0]);
	user_ampm_input[1] = tolower(user_ampm_input[1]);
	strcat(disk_sector_buffer,user_ampm_input);

	result = cbm_open(15, rtc_device, 15, disk_sector_buffer); // Example date and time string: "t-wasat. 11/13/21 10:57:00 pm"
	cbm_close (15);

};//end func 


// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************

void screensaver(void) {

	unsigned char current_x = 0;
	unsigned char current_y = 0;

	unsigned int rand_x = 0;
	unsigned int rand_y = 0;

	_randomize();

	clrscr();

	while(1){

		rand_x = rand();
		rand_y = rand();

		// TODO: This could be faster and smaller code.
		// The logo and always hang around the lower right more than anywhere else.
		// WTF??? I set the point to 20000 so it tends to move up and left and try to blaance it out a bit. 
		// Maybe it's just my machine. Need to test more.
		// This shit jumps around and it's fucking weird. Need to figure this out.
		// But it doesn't crash or hit the edges badly so fuck it?
		if ( rand_x > 20000L ) { // RAND_MAX 32767
			current_x++;
		} else {
			current_x--;
		};//end if

		if (current_x > 27) {
			current_x = 27;
		};//end if

		if ( rand_y > 20000L ) {
			current_y++;
		} else {
			current_y--;
		};//end if

		if (current_y > 11) {
			current_y = 11;
		};//end if

		clrscr();

		display_logo(current_x,current_y); // x cannot be greater than rand, y cannot be greater than 15

		find_rtc_device();

		if ( rtc_device != 0 ) {
			if (screensaver_show_date == TRUE && screensaver_show_time == TRUE) {
				gotoxy(current_x, current_y+12);
				display_date_nice();
				gotoxy(current_x+1, current_y+13);
				display_time_nice();
			} else if (screensaver_show_date == TRUE) {
				gotoxy(current_x, current_y+12);
				display_date_nice();
			} else if (screensaver_show_time == TRUE) {
				gotoxy(current_x+1, current_y+12);
				display_time_nice();
			};//end-if
		};//end-if

		wait(32766);

		if (kbhit() != 0) {
			cgetc();
			clrscr();
			break;
		};//end if 

	};//end-while

	textcolor  ( current_textcolor   );
	bgcolor    ( current_bgcolor     );
	bordercolor( current_bordercolor );

};//end-func


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

	// TODO: This is for helping to try and fix this bug: 
	// debug-args: When using debug-args, or looking at argv[0] directly, the argv program name loads wrong text, usually something from a printf statement. It’s as if the storing of text for the printf statements are overwriting the part of memory where the file name is stored. 
	// Here are some links to help:
	// https://github.com/cc65/cc65/blob/master/libsrc/c64/mainargs.s
	// https://github.com/cc65/cc65/blob/master/asminc/c64.inc

	reboot_register.pc = 0xFCE2; // this is the value that get sys'ed to reboot

	set_alias( "help" , "type chicli-readme" ); // like 20 bytes per call

	// ********************************************************************************
	// SET DEFAULT HOTKEYS
	// ********************************************************************************

	set_hotkey(1,"dir"); // like 10 bytes per call
	// set_hotkey(2,"");
	set_hotkey(3,"cls");
	// set_hotkey(4,"");
	set_hotkey(5,"stopwatch");
	// set_hotkey(6,"");
	set_hotkey(7,"ss");
	// set_hotkey(8,"");


	// ********************************************************************************
	// PROGRAM START
	// ********************************************************************************

	// ********************************************************************************
	// PROCESS ARGUMENTS
	// ********************************************************************************

    // Used for debugging
	if ( argc > 1  &&  matching("-ddd",argv[1]) ) {
		have_device_numbers_changed = TRUE; // this is loaded like this: RUN:REM -ddd // RUN:REM ARG1 " ARG2 IS QUOTED" ARG3 "" ARG5
	};//end if 

	// Start the prompt input stuff 
	starting_x = wherex();
	starting_y = wherey();

	// Display the logo, hardware info, and play the chirp 

	if ( argc > 1  &&  matching("-st",argv[1]) ){
		// read colors for text, background, and border, and set the vars so things don't get screwy 
		// poke 53280,0 (for the border) // poke 53281,0 (for the background) // POKE 646,X (text)
		// void set_colors(unsigned char text, unsigned char background, unsigned char border) {
		// set_colors( PEEK(????) , PEEK(0xD021) , PEEK(0xD020) );
		// Color RAM:
		// $D800-$DBE7 	55296-56295 		1/2 kB (1000 nibbles) of color memory.
		// $DBE8-$DBFF 	56296-56319 		Unused 

		current_textcolor   = read_nibble_low(0xD800);// PEEK(0xD800) & 0x0F;    
	    current_bgcolor     = read_nibble_low(0xD021);// PEEK(0xD021) & 0x0F ; //n eed to read teh low nibble, jsut doing a -240 this isn't right, because it depnds on the high nibble always being 1111 xxxx
	    current_bordercolor = read_nibble_low(0xD020);// PEEK(0xD020) & 0x0F ;

		textcolor(   current_textcolor   );
		bgcolor(     current_bgcolor     );
		bordercolor( current_bordercolor );

	} else {
		// New White on Black /* White on L.Blue on Blue */
		set_profile_colors(7);
		clrscr();
		display_title_text();
		sys_info(have_device_numbers_changed);	 // this is loaded like this: RUN:REM -ddd // RUN:REM ARG1 " ARG2 IS QUOTED" ARG3 "" ARG5	
		pet_chirp();
		puts("\n\nReady!");

	};//end if 

	//set current directory to whatevr it is.
	dev = getcurrentdevice();


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

		user_input_command_number = 0 ;
		user_input_arg1_number    = 0 ;
		user_input_arg2_number    = 0 ;
		user_input_arg3_number    = 0 ;

		user_input_command_type   = 'e' ;
		user_input_arg1_type      = 'e' ;
		user_input_arg2_type      = 'e' ;
		user_input_arg3_type      = 'e' ;

		number_of_user_inputs     = 0   ;

		// PRINT PROMPT
		gotox(SCREEN_LEFT_EDGE);
		cputs(prompt);
		cputc(CURSOR_CHARACTER);
		gotox(wherex()-1);
		position_in_string = 0;
		position_at_prompt_x = wherex();
		position_at_prompt_y = wherey();

		// Below happens once we've processed a command, but it should probably be done at the beginning.
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
				memset(entered_keystrokes,0,sizeof(entered_keystrokes));
				printf(" \n"); // this erases the cursor 
				break; // break out of this loop to beginning processing and executing command


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
			// F1-F8
			// ********************************************************************************
			} else if (keystroke >= 133 && keystroke <= 140) {	// F1 133 

                unsigned char hotkey_offset;
                hotkey_offset = 0;

                switch (keystroke) {
                    case 133 : hotkey_offset = 1; break; // F1 133 
                    case 137 : hotkey_offset = 2; break; // F2 137
                    case 134 : hotkey_offset = 3; break; // F3 134
                    case 138 : hotkey_offset = 4; break; // F4 138
                    case 135 : hotkey_offset = 5; break; // F5 135
                    case 139 : hotkey_offset = 6; break; // F6 139
                    case 136 : hotkey_offset = 7; break; // F7 136
                    case 140 : hotkey_offset = 8; break; // F8 140
                };//end_switch

				memset(entered_keystrokes,0,sizeof(entered_keystrokes));
				printf(" "); // this erases the cursor 

                load_hotkey_command(hotkey_offset);
			    break;


			// ********************************************************************************
			// TAB COMPLETION LEFT ARROW
			// // ********************************************************************************
			} else if (keystroke == 6 || keystroke == 31) { // Using CTRL+<- control + left arrow. Added 31 which is CTRL+7 because in VICE on a modern PC, CTRL+` is near the top left of the keyboard, and emits the same code as CTRL+7. This is for easy testing and use under VICE, so you don't have to go poking around for whatever <- Left Arrow is on your keyboard. On mine it's SHIFT+HYPHEN.

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

					if (entered_keystrokes[i] == ' ' || entered_keystrokes[i] == '/') {
						break;
					} else {
						tab_length++;
					};//end if 

				};//end for 

				tab_complete_position = entered_keystrokes_length-tab_length;
				tab_complete_pointer = tab_complete_pointer + tab_complete_position ; // update pointer to point at the char in entered_keystrokes

				// TODO: Add partition variable par to this!
				strcpy(listing_string,"$"); /* The default is $ to load the current directory.*/ 

				// if (get_drive_type(dev) == DRIVE_UIEC) {
				// 	string_add_character(listing_string,par+1); /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
				// } else if (get_drive_type(dev) == DRIVE_1581) {
				// 	string_add_character(listing_string,'0');
				// } else {
				// 	string_add_character(listing_string,par); /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
				// };/*end for*/

				// Example: device_command[1] = convert_partition_for_drive();
				string_add_character(listing_string,convert_partition_for_drive());

			 	result = cbm_opendir(1, dev, listing_string);

		        result = cbm_readdir(1, &dir_ent); // I thinjk this will avoid it using the disk or folder name as if it were a file name and errorously auto-completing using that string.

		        for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

			        result = cbm_readdir(1, &dir_ent);

					if (strncmp(tab_complete_pointer,dir_ent.name,tab_length) == 0) { 				// 	use strncmp to compare the first X chars in the current file to the extracted text
						strncpy(new_entered_keystrokes,entered_keystrokes,tab_complete_position); 	// copy beginning of entered keystrokes
						strcat(new_entered_keystrokes,dir_ent.name); 								// aoppend the found filename to teh end
						strcpy(entered_keystrokes,new_entered_keystrokes); 							//update teh entered_keystrokes with teh tab completed version
						break; 																		// break out ot eh loop to avoid loading teh resat of the files for no reason 
                    };//end_if 

			    };//end for

		        cbm_closedir(1);

				cursor_home();
				cursor_end(); //erase the current entered text on teh screen, update teh screen with teh newly tab completed version, update teh screen cursor which can all be done with cursor_end()


			// ********************************************************************************
			// LEFT ARROW
			// ********************************************************************************
			// } else if (keystroke == 6) { // CTRL + <- = 6 // we need this becuase left arrow is tab complete, but a filename might have an arrow in it, so CTRL plus LEFT ARROW will now emit an arrow character to compensate
			// 	keystroke = 95; //replcace CTRL LEFT ARROW with LEFT ARROW
			// 	cursor_add_character();

			} else if (keystroke == 95) { // left arrow is now jsut plain left arrow
				keystroke = 95;
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


			// ********************************************************************************
			// END PROCESS KEYSTROKE
			// ********************************************************************************
			};//end-if


		// ********************************************************************************
		// END KEYBOARD INPUT LOOP
		// ********************************************************************************
		};//end-while


        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
		// EXECUTE COMAMNDS
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************
        // ********************************************************************************


		// ********************************************************************************
		// PROCESS COMMAND
		// ********************************************************************************
			do_alias(entered_keystrokes); // look for teh alias string as the whole block of entered text
		    process_command();
			do_alias(user_input_command_string); // look again for an alias in the extracted command from teh entere text 


		// ********************************************************************************
		// CLEAR COMMAND
		// ********************************************************************************
		if ( matching("clear",user_input_command_string) || matching("cls",user_input_command_string) ) {

            clrscr();


		// ********************************************************************************
		// ECHO COMMAND
		// ********************************************************************************
		} else if ( matching("echo",user_input_command_string) ) {

			switch (number_of_user_inputs) {
			 	case 2 :
	 	 			printf("%s\n",user_input_arg1_string);
			    break;
			};//end switch	


		// ********************************************************************************
		// ALIAS COMMAND
		// ********************************************************************************
		} else if ( matching("alias",user_input_command_string) ) {

			result = 1; // Set this so that only if it's full does it trigger the full message.

			if (number_of_user_inputs == 1) { // there should be a var number_of_arguments as well 
				display_alias_all();
			} else if ( number_of_user_inputs==2 && (user_input_arg1_string[0]=='-'&&user_input_arg1_string[1]=='c') ){
				if (they_are_sure() == TRUE) {
					clear_alias_all();
				};//end if 
			} else if ( number_of_user_inputs==3 && (user_input_arg2_string[0]=='-'&&user_input_arg2_string[1]=='c') ){
				clear_alias(user_input_arg1_string);
			} else if (number_of_user_inputs == 3) {
				result = set_alias(user_input_arg1_string,user_input_arg2_string);
			} else {
				printf("Er arg\n");
			};//end-if

			if ( result == 0 ) { // set_alias returns 0 if there are no slots left
				 printf("Alias full!\n");
			};//end_if

			result = 0; // reset it when we are done. 


		// ********************************************************************************
		// UNALIAS COMMAND
		// ********************************************************************************
		} else if ( matching("unalias",user_input_command_string) ) {

			switch (number_of_user_inputs) {

				case 2 :
	 				clear_alias(user_input_arg1_string);
			    break;

			    default : 
			    	printf("Er arg\n");
			    //end default

			};//end-switch


		// ********************************************************************************
		// HOTKEY COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='h' && user_input_command_string[1]=='o' && user_input_command_string[2]=='t' && user_input_command_string[3]=='k') ) {

			if ( number_of_user_inputs==1) { // there should be a var number_of_arguments as well
				display_hotkeys();
			} else if ( number_of_user_inputs==2 && (user_input_arg1_string[0]=='-'&&user_input_arg1_string[1]=='c') ){
				if (they_are_sure()==TRUE) {
					clear_all_hotkeys();
				};//end_if
			} else if ( number_of_user_inputs==3 && (user_input_arg2_string[0]=='-'&&user_input_arg2_string[1]=='c') ){
				clear_hotkey(user_input_arg1_number);
			} else if (number_of_user_inputs==3) {
				set_hotkey(user_input_arg1_number,user_input_arg2_string);
			} else {
				printf("Er.\n");
			};//end-if


		// ********************************************************************************
		// ABOUT COMMAND / HELP COMMAND / VERSION COMMAND
		// ********************************************************************************
		} else if ( matching("about",user_input_command_string) || \
		            ((user_input_command_string[0]=='v' && user_input_command_string[1]=='e' && user_input_command_string[2]=='r')) ) {

			// display_title_text(); // title // This cost 19 bytes!
			display_description_text(); // description
			printf("GitHub.com/ChironB/ChiCLI\n\nLicence: GNU GPLv3\nVer:     %s\nDate:    %s\nTime:    %s\n", VERSION, __DATE__, __TIME__); // version


		// ********************************************************************************
		// KEYCODES COMMAND
		// ********************************************************************************
		} else if ( matching("keycodes",user_input_command_string) ) {

			printf("* Hit it RS/CTRL-C quits *\n");

			do {
				do {
					//nothing
				} while ( kbhit() == 0);//end do 

				get_key = cgetc();

				printf("HEX:%x DEC:%i CHR:%c\n", get_key, get_key, get_key); // TODO: Make this not display control chars and have all the columns aligned in teh output. Thanks!

			// RUN/STOP or CTRL-C
			}while(get_key != 3);//end-do


		// ********************************************************************************
		// ./ COMMAND
		// ********************************************************************************
		} else if ( user_input_command_string[0] == '.' && user_input_command_string[1] == '/' ) {

			// TODO: I think I can pass the entire path as part of the first string I pass to exec() so that it works in different folders!
			// TODO: rewrite this to inlude all args with quotes on the command line
			strcpy(extracted_program_name,user_input_command_string+2); // THIS GIVES ME EVERYTHING EXCEPT THE FIRST TWO CHARACTERS 
			exec(extracted_program_name, user_input_arg1_string);
		    return EXIT_SUCCESS;


		// ********************************************************************************
		// RUN COMMAND
		// ********************************************************************************
		} else if ( matching("run",user_input_command_string) ) {

			// TODO: rewrite this to inlude all args with quotes on the command line 	
		    // TODO: LOADING FROM ANOTHER DRIVE ISSUE
		    // Detect if the startup drive is different from the drive we are trying to load from 
		    // and then either use the exec() from teh startup drive, or, 
		    // use teh dracopy trick to laod from the other drive
		    // then email the mailing list ot find out how to change teh drive that exec() loads from 

            // The keyboard buffer starts from 631 decimal and ends to 640 decimal. 
            // Up to 10 keypresses can be stored on it. We can poke ASCII codes on these locations at will.
			POKE(0xD018, 21); // UPPER CASE/PETSCII MODE
		    clrscr();// clear screen
		    cputs("new");// print new
		    gotoxy(0,3);// move down 3 lines 
		    cputs("load\"");// print load"0:---name_of_program---",9

			// Place the partition number in the run command on the screen.
			if (get_drive_type(dev) == DRIVE_UIEC) {
			   cputc(par+1);
			} else {
				cputc(par);
			};//end-if
			drive_command_string[2] = '\0';
		    cputc(':');

		    cputs(user_input_arg1_string);//
		    cputs("\",");
		    printf("%i,1", dev);
		    gotoxy(0,8);// move down 5 lines
		    if ( matching("-t",user_input_arg2_string) ) { // TMP Turbo Macro Pro shortcut
		    	cputs("sys8*4096");// print run:rem
		    } else {
		    	cputs("run");// print run:rem
		    	if ( number_of_user_inputs != 2 ) {
		    		cputs(":rem ");// print run:rem
		    		cputs(user_input_arg2_string);// print ---arguments---
		    	};//end-if
		    };//end-if 
		    gotoxy(0,10);// move down 2 lines 
  			POKE(0x0277, 19); // HOME - Push the following keystrokes into the keyboard buffer: HOME RETURN RETURN RETURN 
		    POKE(0x0278, 13); // RETURN
		    POKE(0x0279, 13); // RETURN
		    POKE(0x027A, 13); // RETURN
		    POKE(0x00C6,  4); // Push number of characters in the keyboard buffer
		    return EXIT_SUCCESS;


		// ********************************************************************************
		// EXIT COMMAND
		// ********************************************************************************
		} else if ( matching("exit",user_input_command_string)   ||
					// matching("quit",user_input_command_string)   ||
					matching("endcli",user_input_command_string) ){

			if (they_are_sure() == TRUE) {
		    	POKE(0x0277, 78); // n
		    	POKE(0x0278, 69); // e
		    	POKE(0x0279, 87); // w
		    	POKE(0x027A, 13); // return	    	
		    	// POKE(0x027A, 147); // CLR/HOME to clear screen	    	

		    	POKE(0x00C6,  4); // Number of characters in the keyboard buffer
                // POKE(0x00C6,  5); // Number of characters in the keyboard buffer

		    	POKE(0xD018, 21); // UPPER CASE/PETSCII MODE

	    		//_sys((&)65126UL); // maybe this isn't even nesscary? 
		    	exit(EXIT_SUCCESS);  // whcih allows us to call exit properly, or we could say "press return to restart" and have hidden black on black text with SYS 64738 waiting for them 
			};//end if


		// ********************************************************************************
		// RESTART COMMAND
		// ********************************************************************************
		} else if ( matching("restart",user_input_command_string) ) {

		    if (they_are_sure() == TRUE) {
		        // printf("Restarting...\n");
		        exec(PROGRAM_NAME, "-r"); // This reloads whatever the program name is 
		        return EXIT_SUCCESS;      // I don't even know if we ever get here.
		    };//end_if


		// ********************************************************************************
		// REBOOT COMMAND
		// ********************************************************************************
		} else if ( matching("reboot",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {
	    		//printf("\n\nRebooting...\n\n");
	    		reboot_register.pc = 0xFCE2;
	    		_sys(&reboot_register);
			};//end if


		// ********************************************************************************
		// DOS COMMAND
		// ********************************************************************************
		} else if ( user_input_command_string[0] == 'd' && user_input_command_string[1] == 'o' && user_input_command_string[2] == 's' ) {

			// TODO: In this future, this should work like this:
			// dos cp1  -->  Executes DOS command on the current device using 1, dev, 15
			// dos 7 7 15 "do something" --> Executes DOS command as if it were done in Basic like this: OPEN 7,7,15 : PRINT#7,"DO SOMETHING" : CLOSE 7
			// So either we have 1 argument or 4 arguments.
			// TODO: Need to handle par stuff here! But do I? Not sure.

			if (they_are_sure() == TRUE) {
				result = cbm_open(1, dev, 15, user_input_arg1_string);
				printf( "cbm_open: %i \n", result );
				cbm_close(1);
			};//end if


		// ********************************************************************************
		// DISKCOPY COMMAND
		// ********************************************************************************
		} else if ( matching("diskcopy",user_input_command_string) ) {

			// TODO: In this future, this should work like this:
			// dos cp1  -->  Executes DOS command on the current device using 1, dev, 15
			// dos 7 7 15 "do something" --> Executes DOS command as if it were done in Basic like this: OPEN 7,7,15 : PRINT#7,"DO SOMETHING" : CLOSE 7
			// So either we have 1 argument or 4 arguments.

			strcpy (drive_command_string,"d1=0");

			printf("Put the source disk in drive a (0),\nand the target disk in drive b (1).\n");

			if (they_are_sure() == TRUE) {
				printf("DOS Command: %s... ", drive_command_string);
				result = cbm_open(1, dev, 15, drive_command_string);
				cbm_close(1);
				printf("Done!\n");
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
		    	POKE(0x027D, 32); //
		    	POKE(0x027E,145); // UP
		    	POKE(0x00C6, 10); // Number of characters in the keyboard buffer

		    	exit(EXIT_SUCCESS);  // whcih allows us to call exit properly, or we could say "press return to restart" and have hidden black on black text with SYS 64738 waiting for them 

			};//end if


		// ********************************************************************************
		// DRIVEDETECT COMMAND
		// ********************************************************************************
		} else if ( matching("drivedetect",user_input_command_string) ) {

             detect_drive(dev, TRUE); // disabled ecverything below, so you just go to the drive you want and runt eh command. for example: d8: and enter then type drive-detect


		// ********************************************************************************
		// STATUS COMMAND
		// ********************************************************************************
		} else if ( matching("status",user_input_command_string) ) {

			convert_device_string(user_input_arg1_string[2], user_input_arg1_number) ;

			switch (number_of_user_inputs) {
				case 1 : result = get_status(dev, TRUE); break;
				case 2 : result = get_status(user_input_arg1_number, TRUE); break;
			    default : printf("Er arg\n"); break;//end default
			};//end switch	


		// ********************************************************************************
		// INITIALIZE COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='i' && user_input_command_string[1]=='n' && user_input_command_string[2]=='i' && user_input_command_string[3]=='t') ) { // } else if ( matching("initialize",user_input_command_string) ) {

			result = get_status(dev, TRUE);

			if (result != 255) {
				printf("Initializing... ");

		    drive_command_string[0] = 'i';
			if (get_drive_type(dev) == DRIVE_UIEC) {
			    drive_command_string[1] = par+1;
			} else {
				drive_command_string[1] = par;
			};//end-if
			drive_command_string[2] = '\0';

				result = cbm_open(1, dev, 15, drive_command_string);
				cbm_close(1);

				if (result == 0) {
					printf("Done.\n");
				} else {
					printf("Er: %i\n", result);
				};//end-if
			};//end-if


		// ********************************************************************************
		// VALIDATE COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='v' && user_input_command_string[1]=='a' && user_input_command_string[2]=='l') ) {

		    if (they_are_sure() == TRUE) {
				result = get_status(dev, TRUE);

				drive_command_string[0] = 'v';
				if (get_drive_type(dev) == DRIVE_UIEC) {
				    drive_command_string[1] = par+1;
				} else {
					drive_command_string[1] = par;
				};//end-if
				drive_command_string[2] = '\0';

				if (result != 255) {

					printf("Validating... ");

					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);

					if (result == 0) {
						printf("Done.\n");
					} else {
						printf("Er: %i\n", result);
					};//end-if
				};//endif
		    } else {
		        printf("Cancelled.\n");
		    };//end_if


		// ********************************************************************************
		// SD2IEC COMMAND
		// ********************************************************************************
		} else if ( matching("sd2iec",user_input_command_string) ) {

			switch (number_of_user_inputs) {

				case 2 :

					result = get_status(dev, TRUE);

					if         ( matching("-hide-ext", user_input_arg1_string) ) {

						if (result != 255) {
							printf("Hiding exts... ");

							result = cbm_open(1, dev, 15, "xe+");
							cbm_close(1);

							if (result == 0) {
								printf("Done.\n");
							} else {
								printf("Er: %i\n", result);
							};//end-if
						};//end-if

					} else 	if ( matching("-show-ext", user_input_arg1_string) ) {

						if (result != 255) {
							printf("Showing exts... ");

							result = cbm_open(1, dev, 15, "xe-");
							cbm_close(1);

							if (result == 0) {
								printf("Done.\n");
							} else {
								printf("Er: %i\n", result); 
							};//end if 
						};//end if 


					} else 	if ( matching("-save",     user_input_arg1_string) ) {

						//if (result != 255) {
							printf("Saving SD2IEC config... ");

							result = cbm_open(1, dev, 15, "xw");
							cbm_close(1);

							if (result == 0) {
								printf("Done.\n");
							} else {
								printf("Er: %i\n", result);
							};//end if 
						//};//end if 

					} else {

						printf("Er arg\n");

					};//end-if
				break;

			    default :
			    	printf("Er arg\n");
			    break;//end-default

			};//end-switch


		// ********************************************************************************
		// CD COMMAND
		// ********************************************************************************
		} else if (user_input_command_string[0]=='c' && user_input_command_string[1]=='d') {

			strcpy (drive_command_string,"cd");

			switch (number_of_user_inputs) {

				case 1 :
					if ( user_input_command_string[2] == '.' && user_input_command_string[3] == '.' ) { 
						strcat (drive_command_string,command_cdback);
					} else if ( user_input_command_string[2] == '/' ) {
						strcat (drive_command_string,"//");
					};//end_if
				break;//end-case

				case 2 :
					if ( matching("..",user_input_arg1_string) ) {
						strcat (drive_command_string,command_cdback);

					} else 	if ( matching("/",user_input_arg1_string) ) {
						strcat (drive_command_string,"//");

					} else if ( user_input_arg1_string[0]=='d' && user_input_arg1_string[3]==':' ) {

						convert_device_string(user_input_arg1_string[2], user_input_arg1_number);
						if (user_input_arg1_number == 255) { // The above macro "function" updates user_input_arg1_number with 255 if user_input_arg1_string[2] is fucked.
							printf("Error: Invalid device.\n"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_CD;
						};//end-if
						change_drive(user_input_arg1_number);

						goto END_CD;

					} else if (user_input_arg1_string[0] == 'd' && user_input_arg1_string[4] == ':') { // copy * d08b:

						convert_device_string(user_input_arg1_string[2], user_input_arg1_number);
						if (user_input_arg1_number == 255) { // The above macro "function" updates user_input_arg1_number with 255 if user_input_arg1_string[2] is fucked.
							printf("Error: Invalid device.\n"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_CD;
						};//end-if
						change_drive(user_input_arg1_number);

						convert_partition_string(user_input_arg1_string[3], par);
						if (par == 255) { // The above macro "function" updates par with 255 if user_input_arg1_string[3] is fucked.
							printf("Error: Invalid partition.\n"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_CD;
						};//end-if

						if (get_drive_type(dev) == DRIVE_UIEC) {
							// Add the current partition, or drive, for MSD SD-2 and 4040 support.
							part_command[0] = 'c';
							part_command[1] = 'p';
							part_command[2] = par+1;
							part_command[3] = '\0';

							cbm_open(1, dev, 15, part_command);
							cbm_close(1);

							printf("Partition %c set.\n", par+17);

						} else if (get_drive_type(dev) == DRIVE_1581) {
							change_1581_root_par(); // We are doing this first, because the request is to change directly to the device's first, second, third, etc... partition.
							list_and_change_1581_par(TRUE);

						} else {
							printf("Drive %c set.\n", par+17);

						};//end-if

						goto END_CD;

					} else {
						strcat (drive_command_string,"/");
						strcat (drive_command_string,user_input_arg1_string);
						strcat (drive_command_string,"/");

					};//end-if
			    break;//end-case

			    default :
			    	printf("Er arg\n");
			    break;//end-default

			};//end switch	

			result = cbm_open(1, dev, 15, drive_command_string);
			cbm_close(1);

            END_CD : ; // This skips the above closing of the command channel, for cases where it isn't needed.


		// ********************************************************************************
		// MOUNT COMMAND
		// ********************************************************************************
		} else if ( matching("mount",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 2 :
					strcpy (drive_command_string,"cd:"); // This doesn't need to take into account the current partition because ONLY SD2IEC supports mounting disk images.
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Er arg\n");
			    //end default
			};//end switch


		// ********************************************************************************
		// UNMOUNT COMMAND
		// ********************************************************************************
		} else if ( matching("unmount",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 1 :
					strcpy (drive_command_string,"cd"); // This doesn't need to take into account the current partition because ONLY SD2IEC supports mounting disk images.
					strcat (drive_command_string,command_cdback);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;	

			    default : 
			    	printf("Er arg\n");
			    //end default
			};//end switch


		// ********************************************************************************
		// LP COMMAND OR LD COMMAND OR LIST PARTITIONS COMMAND OR LIST DRIVES COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='l' && user_input_command_string[1]=='d') || \
					(user_input_command_string[0]=='l' && user_input_command_string[1]=='p') ){

			// unsigned char read_drive_number;
			// unsigned int  read_bytes_free;
			// unsigned char first_partition = 0;
			// unsigned char last_partition;
			// unsigned char sd2iec_offset = 0;

			if (get_drive_type(dev) == DRIVE_UIEC) {

				memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));
				result = cbm_open(8, dev, CBM_READ, "$=p");
				read_bytes = cbm_read(8, disk_sector_buffer, sizeof(disk_sector_buffer));
				cbm_close (8);

				first_partition = 1;
				last_partition  = disk_sector_buffer[4];
				sd2iec_offset = 1;

			} else if (get_drive_type(dev) == DRIVE_SD2 || get_drive_type(dev) == DRIVE_4040) {
				last_partition = 1;
			} else {
				last_partition = 0;
			};//end-if

			printf("Device:"); 
			detect_drive(dev, TRUE); // TODO: Maybe move? This is WHY the 1581 resets to the root partition.

			if (get_drive_type(dev) == DRIVE_1581) {
				// void list_and_change_1581_par(unsigned char change_par) {
				list_and_change_1581_par(FALSE);



				// memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));
		  //   	result = cbm_opendir(1, dev, "$"); // need to deal with errors here
				// read_drive_number = 0; // use this to count partitions

		  //   	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

				// 	result = cbm_readdir(1, &dir_ent);

				// 	if (result == 2) {
				// 		break;
				// 	};//end-if

				// 	if (dir_ent.type == 1){ // dir_ent.name dir_ent.type dir_ent.size
				// 		read_drive_number++;
				// 		printf(" %c:%16.16s Size:%u BL\n", read_drive_number+48+16, dir_ent.name, dir_ent.size);
				// 	};//end-if

				// };//end-for

				// cbm_closedir(1);

			} else {

				for ( i = first_partition ; i <= last_partition ; i++ ) {

					strcpy(drive_command_string,"$0:");
					drive_command_string[1]=i+48;

					memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));
					result = cbm_open(8, dev, CBM_READ, drive_command_string);
					read_bytes = cbm_read(8, disk_sector_buffer, sizeof(disk_sector_buffer));
					cbm_close (8);

					read_drive_number = disk_sector_buffer[4];
					read_bytes_free = disk_sector_buffer[34];
					read_bytes_free = read_bytes_free + (disk_sector_buffer[35]*256);

					printf(" %c:%16.16s %2.2s %2.2s Free:%u BL\n", read_drive_number+48+17-sd2iec_offset, disk_sector_buffer+8, disk_sector_buffer+26, disk_sector_buffer+29, read_bytes_free);

				};//end-for

			};//end-if


		// ********************************************************************************
		// CHANGE PARTITION COMMAND
		// ********************************************************************************
		} else if ( /* matching("partition",user_input_command_string) || */ \
					matching("part",user_input_command_string)      || \
					matching("prt",user_input_command_string)       || \
					matching("drive",user_input_command_string)     ){

			convert_partition_string(user_input_arg1_string[0], par);

			if (get_drive_type(dev) == DRIVE_UIEC) {
				part_command[0] = 'c';
		        part_command[1] = 'p';
		        part_command[2] = par+1;
		        part_command[3] = '\0';

		        cbm_open(1, dev, 15, part_command);
				cbm_close(1);

				printf("Partition %c set.\n", par+17);

			} else if (get_drive_type(dev) == DRIVE_1581 && matching("root",user_input_arg1_string)) {
				par = 0;
				change_1581_root_par();

			} else if (get_drive_type(dev) == DRIVE_1581) {
				list_and_change_1581_par(TRUE);

				// memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));
		  //   	result = cbm_opendir(1, dev, "$"); // need to deal with errors here
				// i = 0; // use this to count partitions

		  //   	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

				// 	result = cbm_readdir(1, &dir_ent);

				// 	if (result == 2) {
				// 		break;
				// 	};//end-if

				// 	if (dir_ent.type == 1){ // dir_ent.name dir_ent.type dir_ent.size
				// 		i++;
				// 		// printf("i:%u,p:%u\n",i,par);
				// 		// printf("?%u>%c|%u:%16.16s Size:%u BL\n", par, i+48+16, i+48+16, dir_ent.name, dir_ent.size);
				// 		if (i+47 == par) {
				// 			printf(" %c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
				// 			// printf("?%c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
				// 			// printf("dir_ent.name: '%s'\n",dir_ent.name);
				// 			strcpy(drive_command_string, "/0:");
				// 			strcat(drive_command_string, dir_ent.name);
				// 			// printf("dir_ent.name: '%s'\n",dir_ent.name);
				// 			// printf("drive_command_string: '%s'\n",drive_command_string);
				// 			cbm_open(15, dev, 15, drive_command_string);
				// 			cbm_close(15);
				// 			printf("Partition %c set.\n", par+17);
				// 			// Partition areas which meet the qualifications of being a subdirectory can then be selected by the following command.
				// 			// PRINT#file#,"/0:partition name"
				// 			cbm_closedir(1);
				// 			break;
				// 		};//end-if
				// 	};//end-if

				// };//end-for

				// //printf("Partition not found!\n");
				// cbm_closedir(1); // This should only execute if we never find a partition at all.

			} else {
				printf("Drive %c set.\n", par+17);

			};//end-if


		// ********************************************************************************
		// PWD COMMAND - PRINT DEVICE/PARTITION
		// ******************************************************************************** 		
		} else if (user_input_command_string[0]=='p' && user_input_command_string[1]=='w' && user_input_command_string[2] == 'd') { // --> THIS SAVES 8 BYTES!!! --> } else if ( matching("cd",user_input_command_string) ) { 

			printf("Device:%i ", dev);
			if (get_drive_type(dev) == DRIVE_UIEC) {
				printf("Partition:%c\n", par+17);
			} else {
				printf("Drive:%c\n", par+17);
			};//end-if


		// ********************************************************************************
		// CHANGE DRIVE COMMAND
		// ********************************************************************************
        } else if ( user_input_command_string[0]=='d' && user_input_command_string[3]==':' ) {

			convert_device_string(user_input_command_string[2], user_input_arg1_number);
			change_drive(user_input_arg1_number);


		// ********************************************************************************
		// D08: COMMAND / D08A: COMMAND / CHANGE DRIVE AND PARTITION COMMAND
		// ********************************************************************************
        } else if ( user_input_command_string[0]=='d' && user_input_command_string[4]==':' ) {

			convert_device_string(user_input_command_string[2], user_input_arg1_number);
			change_drive(user_input_arg1_number);

			convert_partition_string(user_input_command_string[3], par);

			if (get_drive_type(dev) == DRIVE_UIEC) {
				part_command[0] = 'c';
		        part_command[1] = 'p';
		        part_command[2] = par+1;
		        part_command[3] = '\0';
		        cbm_open(1, dev, 15, part_command);
				cbm_close(1);
				printf("Partition %c set.\n", par+17);

			} else if (get_drive_type(dev) == DRIVE_1581) {
				change_1581_root_par(); // We are doing this first, because the request is to change directly to the device's first, second, third, etc... partition.
				list_and_change_1581_par(TRUE);

			} else {
				printf("Drive %c set.\n", par+17);

			};//end-if


		// ********************************************************************************
		// MAKE-DIR COMMAND
		// ********************************************************************************
		} else if ( matching("mkdir",user_input_command_string)    ||
				  ( user_input_command_string[0]=='m' && user_input_command_string[1]=='d') ) {

			switch (number_of_user_inputs) {
				case 2 :
					strcpy (drive_command_string,"md:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;

			    default :
			    	printf("Er arg\n");
			    //end-default
			};//end-switch


		// ********************************************************************************
		// REMOVE-DIR COMMAND
		// ********************************************************************************
		} else if ( matching("rmdir",user_input_command_string)      ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='d') ) {

			switch (number_of_user_inputs) {
				case 2 :
					strcpy (drive_command_string,"rd:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;

			    default :
			    	printf("Er arg\n");
			    //end default
			};//end switch


		// ********************************************************************************
		// DELETE COMMAND
		// ********************************************************************************
		} else if (	/* matching("delete",user_input_command_string) || */
		            (user_input_command_string[0]=='d' && user_input_command_string[1]=='e' && user_input_command_string[2]=='l') ||
					(user_input_command_string[0]=='d' && user_input_command_string[1]=='l') ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='m') ){

			switch (number_of_user_inputs) {
				case 2 : 	
					if ( matching("*",user_input_arg1_string) ) { 			// copy * d08:     

						dir_file_total = dir_file_count(); // get the total number of files to copy and store in dir_file_total 
						printf("Files to del: %i\n", dir_file_total);

						if (they_are_sure() == TRUE) {

							for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

								if (loop_k == 0) { //first entry is the disk anme 
									//do nothing
								} else { 
									dir_goto_file_index(loop_k);
									detected_filetype = dir_ent.type;
									strcpy (user_input_arg1_string, dir_ent.name);   

									strcpy (drive_command_string,"s0:");

									// if (get_drive_type(dev) == DRIVE_UIEC ) {
									// 	drive_command_string[1] = par+1;
									// } else if (get_drive_type(dev) == DRIVE_1581 ) {
									// 	drive_command_string[1] = '0';
									// } else {
									// 	drive_command_string[1] = par;
									// };//end-f

									// Example: device_command[1] = convert_partition_for_drive();
									drive_command_string[1] = convert_partition_for_drive();

									strcat (drive_command_string,user_input_arg1_string);

									printf("-> %s ",user_input_arg1_string);   
									result = cbm_open(1, dev, 15, drive_command_string);
									cbm_close(1);
									printf("[Deleted]\n");

                    				get_key = 0;
									if (kbhit() != 0) { /* If key has been pressed */
										get_key = cgetc();
									};/*end_if*/

									if (get_key == 3) { /* RUN/STOP or CTRL-C */
										printf("Deleting aborted!\n");
										get_key = 0;
										break;
									};/*end_if*/

								};//end if

							};//end for

						};//end if

					} else {

						strcpy (drive_command_string,"s0:");
						// if (get_drive_type(dev) == DRIVE_UIEC ) {
						// 	drive_command_string[1] = par+1;
						// } else if (get_drive_type(dev) == DRIVE_1581 ) {
						// 	drive_command_string[1] = '0';
						// } else {
						// 	drive_command_string[1] = par;
						// };//end-f

						// Example: device_command[1] = convert_partition_for_drive();
						drive_command_string[1] = convert_partition_for_drive();

						strcat (drive_command_string,user_input_arg1_string);

						result = cbm_open(1, dev, 15, drive_command_string);

						cbm_close(1);

					};//end-if
				break;

				default :
					printf("Er arg\n"); // printf("Er arg:%i\n", number_of_user_inputs);
				//end-default

			};//end-switch


		// ********************************************************************************
		// FORMAT COMMAND
		// ********************************************************************************
		} else if ( matching("format",user_input_command_string) ) {

			drive_command_string[0] = 'n';
			if (get_drive_type(dev) == DRIVE_UIEC) {
			    drive_command_string[1] = par+1;
			} else if (get_drive_type(dev) == DRIVE_UIEC) {
	    		drive_command_string[1] = '0';
			} else {
				drive_command_string[1] = par;
			};//end-if
			drive_command_string[2] = ':';
			drive_command_string[3] = '\0';

            if (user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='q') {

            	switch (number_of_user_inputs) {
				    case 3 :
                        strcat (drive_command_string,user_input_arg2_string);
			        break;

                    case 2 :
                        strcat (drive_command_string,"untitled");
                    break;

			        default :
			            goto FORMAT_ERROR;
			        break;
			    };//end switch

                printf("Quick ");

            } else {

			    switch (number_of_user_inputs) {
				    case 3 :
                        strcat (drive_command_string,user_input_arg1_string);
                        strcat (drive_command_string,",");
                        strcat (drive_command_string,user_input_arg2_string);
			        break;

                    case 1 :
                        strcat (drive_command_string,"untitled"); // do this, but then fall thruogh to the next one regardless
                        strcat (drive_command_string,",00");
			        break;

                    case 2 :
                        strcat (drive_command_string,user_input_arg1_string);
                        strcat (drive_command_string,",00");
                    break;

			        default :
			            goto FORMAT_ERROR;
			        break;
			    };//end-switch

            };//end_if

		    printf("Format: %s\n", drive_command_string);

		    if (they_are_sure() == TRUE) {
		        printf("Formatting... ");
			    result = cbm_open(1, dev, 15, drive_command_string);
			    cbm_close(1);
			    printf("Done.\n");
			    goto FORMAT_END;
		    } else {
		        goto FORMAT_END;
		    };//end_if

            FORMAT_ERROR : printf("Er.\n");
            FORMAT_END   : ; // do nothing


		// ********************************************************************************
		// RENAME COMMAND
		// ********************************************************************************
		} else if ( /* matching("rename",user_input_command_string) || */
		            (user_input_command_string[0]=='r' && user_input_command_string[1]=='e' && user_input_command_string[2]=='n') ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='n') ){

			strcpy (drive_command_string,"r");

			// if (get_drive_type(dev) == DRIVE_UIEC) { 			  // Detected SD2IEC
			// 	string_add_character(drive_command_string,par+1); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// } else if (get_drive_type(dev) == DRIVE_1581) {
			// 	string_add_character(drive_command_string,'0');
			// } else {											  // Any other device
			// 	string_add_character(drive_command_string,par);   // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// };//end-if

			// Example: device_command[1] = convert_partition_for_drive();
			string_add_character(drive_command_string,convert_partition_for_drive());

			strcat (drive_command_string,":");
			strcat (drive_command_string,user_input_arg2_string);
			strcat (drive_command_string,"=");

			// if (get_drive_type(dev) == DRIVE_UIEC) { 			  // Detected SD2IEC
			// 	string_add_character(drive_command_string,par+1); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// } else if (get_drive_type(dev) == DRIVE_1581) {
			// 	string_add_character(drive_command_string,'0');
			// } else {											  // Any other device
			// 	string_add_character(drive_command_string,par);   // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// };//end-if

			// Example: device_command[1] = convert_partition_for_drive();
			string_add_character(drive_command_string,convert_partition_for_drive());

			strcat (drive_command_string,":");

			strcat (drive_command_string,user_input_arg1_string);

			switch (number_of_user_inputs) {

				case 3 :
					result = cbm_open(1, dev, 15, drive_command_string);
					cbm_close(1);
			    break;

			    default :
			    	printf("Er arg\n");
			    //end default

			};//end switch


		// ********************************************************************************
		// DRIVESET COMMAND
		// ********************************************************************************
		} else if ( matching("driveset",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 2 :
					if (get_drive_type(dev) == DRIVE_UIEC) {
						uiec_set(dev, user_input_arg1_string);
					} else if (get_drive_type(dev) == DRIVE_1541 || get_drive_type(dev) == DRIVE_SD2 ) {
						c1541_set(dev, user_input_arg1_string); // They both write to the same memory area to set the device number in software
					} else {
						printf("Er: drv!\n");
					};//end-if
			    break;	

			    default : 
			    	printf("Er arg\n");
			    //end-default
			};//end-switch


		// ********************************************************************************
		// PEEK COMMAND
		// ********************************************************************************
		} else if ( matching("peek",user_input_command_string) ) {	

			peeked = PEEK(user_input_arg1_number);
            printf("Mem:%u is ", user_input_arg1_number);
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);


		// ********************************************************************************
		// PEEKING COMMAND
		// ********************************************************************************
		} else if ( matching("peeking",user_input_command_string) ) {	

            do {
                peeked = PEEK(user_input_arg1_number); // try DC01 or 56321 for keyboard, or D419 54297 and D41A 54298 for pots, or D012 53266 a Raster counter, which is constantly changing.
                printf("Mem:%u is ", user_input_arg1_number);
                BYTE_TO_BINARY(peeked);
                printf(" #:%u\n", peeked);
            } while ( !kbhit() );//end do 


		// ********************************************************************************
		// PEEKBIT COMMAND
		// ********************************************************************************
		} else if ( matching("peekbit",user_input_command_string) ) {

			peeked = PEEK(user_input_arg1_number);
	    	printf("Mem:%u Bit%u is:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);


		// ********************************************************************************
		// POKE COMMAND
		// ********************************************************************************
		} else if ( matching("poke",user_input_command_string) ) {	 

			peeked = PEEK(user_input_arg1_number);

            printf("Mem:%u was ", user_input_arg1_number);
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);

			POKE(user_input_arg1_number, user_input_arg2_number);

			peeked = PEEK(user_input_arg1_number);

            printf("Mem:%u now ", user_input_arg1_number);
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);


		// ********************************************************************************
		// POKEBIT COMMAND
		// ********************************************************************************
		} else if ( matching("pokebit",user_input_command_string) ) {

			peeked = PEEK(user_input_arg1_number);

	    	printf("Mem:%u Bit%u was:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);

            switch (user_input_arg3_number) {
                case 0 : POKE(user_input_arg1_number, clear_bit(peeked,user_input_arg2_number)); break;
                case 1 : POKE(user_input_arg1_number, set_bit(peeked,user_input_arg2_number));   break;
            };//end_switch

	    	printf("Mem:%u Bit%u now:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
            BYTE_TO_BINARY(peeked);
            printf(" #:%u\n", peeked);


		// ********************************************************************************
		// HEX COMMAND
		// ********************************************************************************
		} else if ( matching("hex2dec",user_input_command_string) ) {	

			sscanf(user_input_arg1_string, "%x", &peek_address);
	    	printf("DEC:%02u \n", peek_address );


		// ********************************************************************************
		// DEC COMMAND
		// ********************************************************************************
		} else if ( matching("dec2hex",user_input_command_string) ) {	

			sscanf(user_input_arg1_string, "%u", &peek_address);
	    	printf("HEX:%04X\n", peek_address );


		// ********************************************************************************
		// DEC COMMAND
		// ********************************************************************************
		} else if ( matching("dec2bin",user_input_command_string) ) {

			sscanf(user_input_arg1_string, "%u", &peek_address);
            BYTE_TO_BINARY(peek_address);
            printf("\n");


		// ********************************************************************************
		// = COMMAND / MATHS COMMAND 0--> This takes up 472 bytes!!!
		// ********************************************************************************
		} else if ( user_input_command_string[0] == '=' ) {

			long int answer, first_number, last_number;
			sscanf(user_input_arg1_string, "%li", &first_number);
			sscanf(user_input_arg3_string, "%li", &last_number);
			switch (user_input_arg2_string[0]) {
				case '+' : answer = first_number + last_number; break;
				case '-' : answer = first_number - last_number; break;
				case '*' : answer = first_number * last_number; break;
				case '/' : answer = first_number / last_number; break;
			  default  : puts("?"); break;
			};//end-switch
			printf("  = %li\n", answer);


		// ********************************************************************************
		// SYS COMMAND
		// ********************************************************************************
		} else if ( matching("sys",user_input_command_string) ) {

			sscanf(user_input_arg1_string, "%d", &reboot_register.pc);

			if (they_are_sure() == TRUE) {

				_sys(&reboot_register);

			};//end if


		// ********************************************************************************
		// VIEWMEM COMMAND
		// ********************************************************************************
		} else if ( matching("viewmem",user_input_command_string) ||
		            (user_input_command_string[0]=='v' && user_input_command_string[1]=='m') ){

            unsigned int mem_counter;
            mem_counter = 0;

			switch (number_of_user_inputs) {

			    case 2 :
			        start_address = atoi(user_input_arg1_string);
			        end_address = 0xFFFF; // use top of memory if nothing is given. This should fall through to the next case.
			    break;

				case 3 :
					// Used the ROM shortcut
					if ( matching("rom",user_input_arg1_string) ) {
						if        ( matching("kernal"    ,user_input_arg2_string) ) { // $E000-$FFFF, 57344-65535 KERNAL ROM
							start_address = 57344L; // Kernal ROM Starting address
		                	end_address   = 65535L; // Kernal ROM Ending address
		                } else {
		                	printf("Er arg!");
		                	goto END_VIEW_MEM;
		                };//end-if
					} else {
						// Manually entered addressess
		                start_address = atoi(user_input_arg1_string);
		                end_address   = atoi(user_input_arg2_string);
		                if (start_address > end_address) {
		                    start_address = end_address;
		                    end_address   = atoi(user_input_arg1_string);
		                };//end_if
		            };//end_if
			    break;

			    default : 
			        start_address = 0x00; // use top of memory if nothing is given. This should fall through to the next case.
			        end_address = 0xFFFF; // use top of memory if nothing is given. This should fall through to the next case.
			    break;

			};//end switch

	        hex_display_position_in_file = start_address ;
	        hex_display_position_x       = 0 ;
	        hex_display_position_y       = 0 ;

	        clrscr();

            for ( mem_counter = start_address ; mem_counter <= end_address ; mem_counter++) {

		        peeked = PEEK(mem_counter);

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

		        if (hex_display_position_y == 23) {
			        hex_display_position_x = 0;
			        hex_display_position_y = 0;
			        gotox(0);
			        gotoy(24);
			        printf("More:Any Back:UP Quit:RS");
			        do {
				        //nothing
			        } while (kbhit() == 0);//end do
			        get_key = cgetc();
			        if (get_key == 3) { // RUN/STOP or CTRL-C
				        printf("\n");
				        read_bytes = 0;
				        break;
				    } else if (get_key == 145) { // cursor up SHIFT+CRSR-UP/DOWN to rewind a screens worth of view 
				        hex_display_position_in_file = hex_display_position_in_file - 368;
				        mem_counter = mem_counter - 368; // TODO: When you CRSR-UP to go back, if it's part where you started, it crashes. The fix, is that it should work differently. It should have a start and end address that control everything, and are simplay set when the thing is called.
				        peeked = PEEK(mem_counter); // I think we need to do this again when rewinding.
			        };//end-if
			        clrscr();
		        };//end-if

		        // DISPLAY ADDRESS
		        if (hex_display_position_x == 0){
			        gotox(0);
			        gotoy(hex_display_position_y);
			        printf("%04X", hex_display_position_in_file);
		        };//end-if

		        // DISPLAY HEX AND CHAR
		        gotox((hex_display_position_x * 3) + 6);
		        printf("%02X", peeked);
		        gotox(hex_display_position_x + 31) ;
		        cputc( convert_char(peeked) );

		        // UPDATE COUNTERS
		        if (hex_display_position_x == 7 && hex_display_position_y != 23) {
			        hex_display_position_x = 0;
			        hex_display_position_y++;
		        } else {
			        hex_display_position_x++;
		        };//end-if

		        hex_display_position_in_file++;

	        };//end-for

			END_VIEW_MEM :
			printf("\n");


		// ********************************************************************************
		// DUMPMEM COMMAND
		// ********************************************************************************
		} else if ( (matching("dumpmem",user_input_command_string)) ){ // (user_input_command_string[0]=='d' && user_input_command_string[1]=='m') ){

			// Simple usage:   dump-mem 0 100 kernal.bin
			// Shortcut usage: dump-mem rom kernal kernal.bin

            unsigned int  mem_counter;
            unsigned char peeked_byte; // lame
            unsigned char peeked_string[10]; // super lame
            mem_counter = 0;
            peeked_byte = 0;
            strcpy(peeked_string,"");

			switch (number_of_user_inputs) {

				case 4 :

					// Used the ROM shortcut
					if ( matching("rom",user_input_arg1_string) ) {
						if        ( matching("kernal"    ,user_input_arg2_string) ) { // $E000-$FFFF, 57344-65535 KERNAL ROM
							start_address = 57344L; // Kernal ROM Starting address
		                	end_address   = 65535L; // Kernal ROM Ending address
		                } else {
		                	printf("Er arg!");
		                	goto END_DUMP_MEM;
		                };//end-if
					} else {
						// Manually entered addressess
		                start_address = atoi(user_input_arg1_string);
		                end_address   = atoi(user_input_arg2_string);
		                if (start_address > end_address) {
		                    start_address = end_address;
		                    end_address   = atoi(user_input_arg1_string);
		                };//end_if
		            };//end_if

					// Setup TARGET FILE for WRITING 
					strcpy (drive_command_string2, "0:"  );
					strcat (drive_command_string2, user_input_arg3_string); // filename
					strcat (drive_command_string2, ",w," );
					strcat (drive_command_string2, "p"   ); // needs to be PRG type becuase SEQ type adds the floowing text to the beginning of the file: C64File *FILENAME*

					// if (get_drive_type(dev) == DRIVE_UIEC ) { // This was like 30 bytes over
					// 	drive_command_string2[0] = par+1;
					// } else if (get_drive_type(dev) == DRIVE_1581 ) {
					// 	drive_command_string2[0] = '0';
					// } else {
					// 	drive_command_string2[0] = par;
					// };//end-f

					// Example: device_command[1] = convert_partition_for_drive();
					drive_command_string2[0] = convert_partition_for_drive();

				    printf("Saving %u - %u to\nfile:%s on device:%i\n",start_address,end_address,user_input_arg3_string,dev);

					if (they_are_sure() == FALSE) {
						break;
					};//end-if

					result2 = cbm_open(7, dev, CBM_WRITE, drive_command_string2);

					// Backup the processor port and then restore it when reading the character ROM???
					// I/O Area (memory mapped chip registers), Character ROM or RAM area (4096 bytes); depends on the value of bits #0-#2 of the processor port at memory address $0001:
					// read port to var
					// write new value to port
					// read byte in char rom
					// restore old value to port
					// write out to disk

					for ( mem_counter = start_address ; mem_counter <= end_address ; mem_counter++) {
						peeked_byte = PEEK(mem_counter);
						strcpy(peeked_string,""); // lame!!!
						string_add_character(peeked_string,peeked_byte); //super lame!!!
						result = cbm_write(7, peeked_string, 1);
						printf(".");
						if (end_address == 65535L && mem_counter == 65535L) {
							break;
						};//end-if
					};//end-for

					cbm_close (7);
					printf("\nDone.\n");
					printf("Saved memory %u - %u to\nfile:%s on device:%i\n",start_address,end_address,user_input_arg3_string,dev);

			    break;

			    default :
					printf("Error!\n");
			    break;

			};//end switch

			END_DUMP_MEM :
			printf("\n");


		// ********************************************************************************
		// COPY COMMAND
		// ********************************************************************************
		} else if ( matching("copy",user_input_command_string)                                 ||
				  ( (user_input_command_string[0]=='c')&&(user_input_command_string[1]=='p') ) ){

            use_dcopy = TRUE;

			source_par = par; // source_par = par; // the source partition should be whever we are
            target_par = par; // target_par = '0';

			get_key = 0; // This is the solution for that weird bug! 
			// Basically, a left-over character might be in this variable.
			// Now, when the first directory entry is returned, it's the disk name, so we don't do anything.
			// Turns out, this means that when the next bit of code runs to see if we've hit CTRL-C,
			// it thinks we've scanned the keyboard at least once, ebcasue we've run at least one call
			// of a copy function. But on the first run, which is the disk name, we haven't!
			// So we set this to nothing, so that it can't accidentally think we have pressed CTRL-C.
			// So this bug doesn't run all the time, only sometimes, because sometimes there's an 
			// old jkeybaord value in there, and only siometimes is that value an actual CTRL-C.
			// So this is the fix for a stupid bug I can't seem to re-create consistently and
			// has been chapping my ass forever!
			// This fix adds only 5 bytes to the overall compiled code.

            if (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') { // copy * d08:

		        convert_device_string(user_input_arg2_string[2], user_input_arg2_number);

            } else if (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') { // copy * d08b:

		        convert_device_string(user_input_arg2_string[2], user_input_arg2_number);
		        convert_partition_string(user_input_arg2_string[3], target_par); // do it deucimo...

            } else {

                user_input_arg2_number = dev;
                use_dcopy = FALSE;

            };//end_if

            // user_input_arg2_number is now the target device number, so...
            if (dev == user_input_arg2_number) { // if the source device (dev) and target device (user_input_arg2_number) are the same...
                use_dcopy = FALSE;               // ...don't use the dcopy function (it won't work)
            };//end_if

			// The source device is dev
			// The target device is user_input_arg2_number
			if (get_drive_type(dev) == DRIVE_UIEC) { // TODO: DOES THIS NEED 1581 SUPPORT?????
				// printf("Source is SD2IEC!\n");
				source_par = source_par+1;
			};//end-if

			// We can't use something like this: device_command[1] = convert_partition_for_drive(); because this is custom logic for this edge case.
			if (get_drive_type(user_input_arg2_number) == DRIVE_UIEC) {
				// printf("Target is NOT SD2IEC!\n");
				target_par = target_par+1;
			} else if ( get_drive_type(user_input_arg2_number)==DRIVE_1581 && user_input_arg2_string[4] == ':') {
				// printf("Partition direct copying on 1581\nis not supported.\n");
				printf("Not supported by the 1581.\n");
				goto end_copying;
			} else if ( get_drive_type(user_input_arg2_number)==DRIVE_1581) {
				target_par = '0';
			};//end-if

		    switch (number_of_user_inputs) {
			    case 3 :
				    if ( matching("*",user_input_arg1_string) ) { 			// copy * d08:     

					    dir_file_total = dir_file_count(); // get the total number of files to copy and store in dir_file_total 
					    printf("Files to copy: %i\n", dir_file_total);

					    if (they_are_sure() == TRUE) {

					        for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

						        if (loop_k == 0) { // first entry is the disk name.
						        	//do nothing
						        } else { 
						        	dir_goto_file_index(loop_k);
								    detected_filetype = dir_ent.type; //use this to detect if it's a directory or not

								    strcpy (user_input_arg1_string, dir_ent.name); // NOTE!!! If I switch this to arg2 isntead, teh partion copy works, but drive to drive doesn't work
                                    // strcpy (user_input_arg2_string, dir_ent.name);

								    if 	(dir_ent.type ==  2) {
				        				printf("Skipping: %s\n", dir_ent.name);

				        			} else {
							        	if (use_dcopy == TRUE) {   
										    dcopy(); // use dcopy for another drive and acopy for bulk moving to a folder on the current drive
									    } else {
										    acopy(); // use acopy to copy a bunch of files to another folder on the same drive 
									    };//end_if

								    };//end_if

							    };//end if 

								if (get_key == 3) { /* RUN/STOP or CTRL-C */
									printf("Copying aborted!\n");
									get_key = 0;
									goto end_copying;
								};/*end_if*/

						    };//end for 

					    };//end if 

				    } else if ( use_dcopy == TRUE ) {
					    printf("Detected file: %s ", user_input_arg1_string );
					    detected_filetype = detect_filetype(user_input_arg1_string, TRUE);
					    dcopy();

				    } else if ( use_dcopy == FALSE ) {
					    // copy();
					    acopy(); // advanced copy // TODO: IF I COMMENT THIS OUT, I SAVE 788 bytes! I think it's time to re-write this as a function and not a macro!

				    } else { // error 
					    printf("Syntax er.\n");

				    };//end if 
	        	break;	

	        	default : 
	        		printf("Er arg\n"); // printf("Er arg:%i\n", number_of_user_inputs);
	        	//end default

		    };//end switch

            goto end_copying;

            end_copying : ;
            // ********** END OF COPY ********** //


		// ********************************************************************************
		// TYPE COMMAND
		// ********************************************************************************
		} else if ( matching("type",user_input_command_string) ||
		            (user_input_command_string[0]=='c' && user_input_command_string[1]=='a' && user_input_command_string[2]=='t') ){ //matching("cat",user_input_command_string) ) 

			switch (number_of_user_inputs) {

				case 2 :
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // detect filetype
					switch(detected_filetype){
						case 2 : // case  2 : printf("DIR"); break;	// DIR
							printf("Err: Dir.\n");
						break;

						case 16 : // case 16 : printf("SEQ"); break; // SEQ
							type_text(user_input_arg1_string); // if SEQ use type
						break;

						case 17 : // case 17 : printf("PRG"); break; // PRG
							type_prg(user_input_arg1_string); // if PRG use type-prg
						break;

						case 18 : 
						case 19 : // case 19 : printf("REL"); break;	// REL
							type_hex(user_input_arg1_string); // if USR use type-hex
						break;

						default : // case  2 : printf("DIR"); break;	// DIR
							printf("Er: Type?\n");
						//end default
					};//end switch
				break;

				case 3 :
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // detect filetype
					if (matching("-hex",user_input_arg2_string)) {
						type_hex(user_input_arg1_string); 
					} else if (matching("-text",user_input_arg2_string)) {
						type_text(user_input_arg1_string); // if SEQ use type 
					} else {
						printf("Er arg3:%s\n", user_input_arg2_string);
					};//end if 
				break;

			    default :
			    	printf("Er arg\n");
			    //end default

			};//end switch


		// ********************************************************************************
		// LIST COMMAND
		// ********************************************************************************
		} else if ( matching("list",user_input_command_string) ||
		            (user_input_command_string[0]=='l' && user_input_command_string[1]=='s') ||
					(user_input_command_string[0]=='d' && user_input_command_string[1]=='i' && user_input_command_string[2]=='r') ){

			if        (user_input_command_string[0]=='d') {
				files_per_screen = 48; // 2 column display mode
			} else if (user_input_command_string[0]=='l') {
				files_per_screen = 24; // 1 column display mode
			};//end-if

			strcpy(listing_string,"$"); // The default is $ to load the current directory.

			// if (get_drive_type(dev) == DRIVE_UIEC) {
			// 	// printf("Detected SD2IEC!\n");
			// 	string_add_character(listing_string,par+1); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// } else if (get_drive_type(dev) == DRIVE_1581) {
			// 	string_add_character(listing_string,'0');
			// } else {
			// 	string_add_character(listing_string,par); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
			// };//end-if

			// Example: device_command[1] = convert_partition_for_drive();
			string_add_character(listing_string,convert_partition_for_drive());

			// Supported usage examples:
			// ls -s
			// ls chi*
			// ls -s chi*
			// ls chi* -ls

			switch (number_of_user_inputs) {
				case 2 :
					if        ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode
					} else if ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode
					} else {
						string_add_character(listing_string,':');
						strcat(listing_string, user_input_arg1_string);
					};//end_if
				break;

				case 3 :
					if        ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode
					    string_add_character(listing_string,':');
					    strcat(listing_string, user_input_arg2_string);
					} else if ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode
					    string_add_character(listing_string,':');
					    strcat(listing_string, user_input_arg2_string);
					} else if ( user_input_arg2_string[0]=='-' && user_input_arg2_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode{
					    string_add_character(listing_string,':');
					    strcat(listing_string, user_input_arg1_string);
					} else if ( user_input_arg2_string[0]=='-' && user_input_arg2_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode{
					    string_add_character(listing_string,':');
					    strcat(listing_string, user_input_arg1_string);
					};//end_if
				break;
			};//end-switch

		    printf("Device:%i ", dev,par+16);

		    result = cbm_opendir(1, dev, listing_string); // need to deal with errors here

		    displayed_count = 2; // this is two becuase we alrieady displaeyd two lines of text 

		    for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

				if (displayed_count == files_per_screen) {

					//printf(" * Hit Any Key * ");
					printf("* CONTINUE *");

					do {
						// nothing
					} while (kbhit() == 0);//end do 

					get_key = cgetc();
					gotox(0);
					printf("                                       ");
					gotox(0);

					if (get_key == 3) { // RUN/STOP or CTRL-C
						//printf("\n");
					    gotoy(wherey()); // This makes sure we don't scroll the screen adn miss the top line which is displaying a file name that woudl otherwise be lost. 
						break;
					};//end_if 

					displayed_count = 0;

				};//end if

		    	if (result != 0) {
		    		printf("Er: cbm_dir %i\n", result);
		    		break;
		    	};//end_if 

			    result = cbm_readdir(1, &dir_ent);

			    if (number_of_files == 0) { // the size is actually drive 0 or drive 1 from PET dual drives
					printf("Name:%s\n", dir_ent.name);

			    } else if (result == 2) {

                    if (wherex() != 0) {
                        printf("\n");
                    };//end_if

				    if (dir_ent.size == 0) {        printf("   0 B");
				    } else if (dir_ent.size == 1) { printf(" 256 B");
				    } else if (dir_ent.size == 2) { printf(" 512 B");
				    } else if (dir_ent.size == 3) { printf(" 768 B");
				    } else {
				    	filesize = dir_ent.size;
				    	filesize = filesize*256;
				    	filesize = filesize/1000;
				    	printf("%lu KB", filesize );
				    };//end if

			    	printf(" free space.\n");

			    	break;

			    } else { // print filenames 

			    	if (files_per_screen == 48) { // short 2-column form 

				        printf(" %16s ", dir_ent.name );

				        switch (dir_ent.type) {
				             case  2 : cputs("D"); break;
				             case 16 : cputs("S"); break;
				             case 17 : cputs("P"); break;
				             case 18 : cputs("U"); break;
				             case 19 : cputs("R"); break;
				             case  1 : cputs("C"); break; // For 1581 partitions.
				             default : cputs("?"); break;
				        };//end if 

					    if ((number_of_files % 2) == 0) {
					        printf("\n");
					    } else {
					        // Do nothing.
					    };//end_if

					    ++displayed_count;

			        } else { // long form  1-column mode files_per_screen == 24

				        gotox(3);
				        printf("%s", dir_ent.name );
		       		    gotox(20);

				        switch (dir_ent.type) {
				             case  2 : cputs("            (Dir)"); break;
				             case 16 : cputs("SEQ"); break;
				             case 17 : cputs("PRG"); break;
				             case 18 : cputs("USR"); break;
				             case 19 : cputs("REL"); break;
				             case  1 : cputs("CBM"); break; // For 1581 partitions.
				             default : printf("%i", dir_ent.type );  break;
				        };//end_swtich

				        if (dir_ent.type != 2) {

					        gotox(24);

                            switch(dir_ent.size) {
                                case 0     : printf("     0 B"); break;
                                case 1     : printf("   256 B"); break;
                                case 2     : printf("   512 B"); break;
                                case 3     : printf("   768 B"); break;
                                case 63999L: printf(">16384 KB"); break;
                                default    :
                                    filesize = 0;
                                    filesize = dir_ent.size; // You can't do this: filesize = (dir_ent.size*256); It gets fucked up. You need to do it in steps so the conversion between int and long int works. The guy that talks about writing for cc65 talks about this.
                                    filesize = (filesize*256);
                                    filesize = (filesize/1000);
                                    display_filesize = filesize;
					        	    printf(" %5u KB", display_filesize);
					        	break;
					        };//end_switch

                            gotox(34);

					        switch (dir_ent.access) {
					            case CBM_A_RO : cputs("  R"); break;
					            case CBM_A_WO : cputs("  W"); break;
					            case CBM_A_RW : cputs("R/W"); break;
					        };//end-switch

					    };//end-if

					    printf("\n");
					    ++displayed_count;

				    };//end-if

				};//end-if

			};//end-for

		    cbm_closedir(1);


		// ********************************************************************************
		// FILEDATE COMMAND
		// ********************************************************************************
		} else if ( (matching("filedate",user_input_command_string)) ) {

			// TODO: I want to re-create this as part of a fileinfo command, that spits out a whole list of info about a file.

			unsigned char one_char_string[2] = " ";
			unsigned char date_index;
			unsigned char little_disk_sector_buffer[16];
			unsigned char file_date_time[32];

			if (number_of_user_inputs != 2) {
				printf("Err: No file given.");
				goto END_FILEDATE;
			} else if ( get_drive_type(dev) != DRIVE_UIEC) {
				printf("Err: Device not SD2IEC.");
				goto END_FILEDATE;
			};//end-if

			strcpy(drive_command_string,"$=t0:"); // NOT WORTH CHANGING --> This only works because partition zero (0) is whatever the currently set partition is. I should probably make this take the specifically set partition instead.

			strcat(drive_command_string,user_input_arg1_string); // This lists the partitions.
			strcat(drive_command_string,"=l"); // This lists the partitions.

			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));

			result = cbm_open(8, dev, CBM_READ, drive_command_string);

			do {

				read_bytes = cbm_read(8, little_disk_sector_buffer, sizeof(little_disk_sector_buffer));

				for (i = 0 ; i < read_bytes ; i++) {

						if ( (little_disk_sector_buffer[i] >= 48 && little_disk_sector_buffer[i] <= 57) ||
						      isalpha(little_disk_sector_buffer[i])==TRUE ||
							 		 little_disk_sector_buffer[i]  == '/' ||
							         little_disk_sector_buffer[i]  == '.' ){
							one_char_string[0] = little_disk_sector_buffer[i];
							one_char_string[1] = '\0';
							strcat(disk_sector_buffer, one_char_string);
						} else {
							one_char_string[0] = ' ';
							one_char_string[1] = '\0';
							strcat(disk_sector_buffer, one_char_string);
						};//end-if

				};//end for

			} while( read_bytes == sizeof(little_disk_sector_buffer) ); //end loop

			cbm_close (8);

			date_index = strlen(disk_sector_buffer) - 66; // When tghe length is 121 then the starsting psotion is 59 // 121-59 = 62

			// Start at our date_index, and if we find a blank space, update the date_index
			// until we finally hit a printable character.
			while (disk_sector_buffer[date_index] == ' ') {
				date_index++;
			};//end-while

			strncpy(file_date_time, disk_sector_buffer+date_index, 20); // Create a date string

			remove_extra_spaces(file_date_time); // Remove any extra spaces in the middle

			file_date_time[11] = ':';

			printf("Timestamp: %s", file_date_time); // print the time stamp

			END_FILEDATE :
			printf("\n");


		// ********************************************************************************
		// CHIRP COMMAND
		// ********************************************************************************
		} else if ( matching("chirp",user_input_command_string) ) {

			pet_chirp(); // this whole section takes like 30 bytes


		// ********************************************************************************
		// SYSINFO COMMAND
		// ********************************************************************************
		} else if ( matching("sysinfo",user_input_command_string) ) {

			display_title_screen(have_device_numbers_changed); // Display the logo, hardware info, and play the chirp


		// ********************************************************************************
		// COLOR-SET COMMAND
		// ********************************************************************************
		} else if ( matching("colorset",user_input_command_string) ) {

			switch (number_of_user_inputs) {
				case 4 :
					current_textcolor = --user_input_arg1_number;
					textcolor(current_textcolor);
					current_bgcolor = --user_input_arg2_number;
					bgcolor(current_bgcolor);
					current_bordercolor = --user_input_arg3_number;
					bordercolor(current_bordercolor);
			    break;

			    default :
					printf("Er.\n");
			    break;//end default
			};//end switch


		// ********************************************************************************
		// PROFILE COMMAND
		// ********************************************************************************
		} else if ( matching("profile",user_input_command_string) ) {

			switch (number_of_user_inputs) {

				case 2 :
					if (	user_input_arg1_number == 0) {
						break;
					} else {
						set_profile_colors(user_input_arg1_number);
					};//end-if
			    break;

			    default : 
					printf("Er.\n");
			    break;//end-default

			};//end-switch


		// ********************************************************************************
		// SCREENSAVER COMMAND
		// ********************************************************************************
		} else if ( matching("screensaver",user_input_command_string) ||
		            (user_input_command_string[0]=='s' && user_input_command_string[1]=='s') ){

			switch (number_of_user_inputs) {

			    case 1 :
					screensaver();
				break;

				case 2 :
					if (        matching("-enable-time",user_input_arg1_string) ) {
						screensaver_show_time = TRUE;

					} else if ( matching("-enable-date",user_input_arg1_string) ) {
						screensaver_show_date = TRUE;

					} else if ( matching("-disable-time",user_input_arg1_string) ) {
						screensaver_show_time = FALSE;

					} else if ( matching("-disable-date",user_input_arg1_string) ) {
						screensaver_show_date = FALSE;

					};//end-if
					printf("Done!\n");
			    break;

			    default:
					printf("Er. args!\n");
				break;
			};//end_switch


		// ********************************************************************************
		// DATETIME COMMAND
		// ********************************************************************************
		} else if ( matching("datetime",user_input_command_string) ){

			switch (number_of_user_inputs) {
				case 2 :
					find_rtc_device();
					if ( rtc_device != 0 ) {
						printf("Current date & time:\n");
						display_date_nice();
						display_time_nice();
						printf("\n");
						set_date();
						printf("New date & time set to:\n");
						display_date_nice();
						display_time_nice();
						printf("\n");
					} else {
        				printf("Err: No RTC found!\n");
					};//end-if
			    break;

			    default:
			    	find_rtc_device();
					if ( rtc_device != 0 ) {
						display_date_nice();
						printf("\n");
						display_time_nice();
						printf("\n");
					} else {
						printf("Err: No RTC found!\n");
					};//end-if
				break;
			};//end_switch


		// ********************************************************************************
		// TIME COMMAND
		// ********************************************************************************
		} else if ( matching("time",user_input_command_string) ) {

			find_rtc_device();
			switch (rtc_device) {
				case 0  : printf("Err: No RTC found!\n");   break;
				default : display_time_nice();printf("\n"); break;
			};//end-switch


		// ********************************************************************************
		// DATE COMMAND
		// ********************************************************************************
		} else if ( matching("date",user_input_command_string) ) {

			find_rtc_device();
			switch (rtc_device) {
				case 0  : printf("Err: No RTC found!\n");   break;
				default : display_date_nice();printf("\n"); break;
			};//end-switch


		// ********************************************************************************
		// STOPWATCH COMMAND
		// ********************************************************************************
		} else if (  matching("stopwatch",user_input_command_string)                          ||
		             (user_input_command_string[0]=='s' && user_input_command_string[1]=='w') ){ // matching("stopwatch",user_input_command_string) ||

            // Jiffy Version - Commodore 64 Specific - Commodore PET has this at a different address
            if (stopwatch_start_stamp == 0) {

                stopwatch_start_stamp = (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162)); //PRINT (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162))

                printf("Stopwatch on.\n");

            } else {

                unsigned int stopwatch_difference;
                unsigned int stopwatch_seconds;
                unsigned int stopwatch_jiffies;

                stopwatch_difference = ( PEEK(161)*256 + PEEK(162) ) - stopwatch_start_stamp; // 18 mins max roughly 

                stopwatch_seconds = stopwatch_difference/60;
                stopwatch_jiffies = stopwatch_difference-(stopwatch_seconds*60);

                printf("Stopwatch: %u.%u secs.\n", stopwatch_seconds, ((stopwatch_jiffies*166)/100) );

                stopwatch_start_stamp = 0;

            };//end_if


		// ********************************************************************************
		// VARS COMMAND
		// ********************************************************************************
		} else if ( matching("vars",user_input_command_string) ) {

			printf("Maximum Bytes:\nCommand Line:%u\nArgs:%u\nDisk Buffer:%u\nAliases:%u\nAlias:%u\nHotkeys:%u\nHotkey:%u\n",MAX_LENGTH_COMMAND,MAX_LENGTH_ARGS,MAX_DISK_SECTOR_BUFFER,MAX_ALIASES,MAX_ALIAS_LENGTH,MAX_HOTKEYS,MAX_HOTKEY_LENGTH);


		// ********************************************************************************
		// CHANGELABEL COMMAND
		// ********************************************************************************
		} else if ( matching("changelabel",user_input_command_string) ) {

			// TODO: Needs checking for 1541 / 4040 / SD-2 and length of entered string is not greater than 16 chars

			switch (number_of_user_inputs) {
				case 2 :
					if (they_are_sure() == TRUE) {
						strcpy(disk_label, user_input_arg1_string);

						while(strlen(disk_label) < 16) {  // This added 18 bytes!
							string_add_character(disk_label, 0xA0);
						};//end-while

						write_disk_label();
						printf("Done!\n");
					};//end if
				break;

				default:
					printf("Er. args!\n");
				break;
			};//end_switch


		// ********************************************************************************
		// CHANGEID COMMAND
		// ********************************************************************************
		} else if ( matching("changeid",user_input_command_string) ) {

			// TODO: Needs checking for 1541 / 4040 / SD-2 and length of entered string is not greater than 16 chars

			switch (number_of_user_inputs) {
				case 2 :
					if (they_are_sure() == TRUE) {
						strncpy(disk_id, user_input_arg1_string, 2);
						write_disk_id();
						printf("Done!\n");
					};//end if
			    break;

			    default:
					printf("Er. args!\n");
				break;
			};//end_switch


		// // ********************************************************************************
		// // EASTEREGG COMMAND - This takes up 71 bytes!
		// // ********************************************************************************
		// } else if ( matching("easteregg",user_input_command_string) ) {

		// 	printf("IT IS YOUR EASTER EGG.\n");


		// ********************************************************************************
		// DEBUG-ARGS COMMAND
		// ********************************************************************************
		// } else if ( matching("debug-args",user_input_command_string) ) {

		// 	printf             ("# of args:%i\n", argc-1);
		//  	//printf             ("PRG Name:%s\n"      , argv[0]);
		//  	if (argc > 1)printf("Arg1:%s\n"         , argv[ 1]);
		//  	if (argc > 2)printf("Arg2:%s\n"         , argv[ 2]);
		//  	if (argc > 3)printf("Arg3:%s\n"         , argv[ 3]);
		//  	if (argc > 4)printf("Arg4:%s\n"         , argv[ 4]);
		//  	if (argc > 5)printf("Arg5:%s\n"         , argv[ 5]);
		// 	// if (argc > 6)printf("Arg  6:%s\n"         , argv[ 6]);
		// 	// if (argc > 7)printf("Arg  7:%s\n"         , argv[ 7]);
		// 	// if (argc > 8)printf("Arg  8:%s\n"         , argv[ 8]);
		// 	// if (argc > 9)printf("Arg  9:%s\n"         , argv[ 9]);
		// 	// if (argc >10)printf("Arg 10:%s\n"         , argv[10]);


		// ********************************************************************************
		// ERROR EMPTY COMMAND
		// ********************************************************************************
		} else if ( strlen(entered_keystrokes) == 0 ) {

		    // Do nothing.


		// ********************************************************************************
		// ERROR NO COMMAND FOUND
		// ********************************************************************************
		} else {

			// TODO: FIX THIS! 
			// HOLY FUCK this is a hack. I don't know why, but in
			// File: commands.h
			// Line: 17
			// #define process_command()
			// If a command is entered with nothing but spaces, it gets screwed up.
			// Specically, the string contains the first 4 chars in memory starting 
			// at memory lcoation 0x0000.
			// My guess is that when process_command() does it's things, a string
			// is de-referenced or a null string is copied... I dunno some bullshit.
			// Point is: when a string contains /6.0
			// (but dot is actually the petscii of CMD+C which is the upper left square)
			// it means it's null or fucked 
			// or poiting to where it shjouldn't which is a cc65 thing.
			// This was also affecting RUN when you had no argument,
			// it contains the same string /6.0
			// but after teh REM part, which means the same bug or whatever is
			// screwing up the argument processing as well.
			if ( strlen(entered_keystrokes) != strlen(user_input_command_string) ){
				printf("???\n", user_input_command_string);
			} else {
				printf("%s ?\n", user_input_command_string);
			};//end-if


		// ********************************************************************************
		// FINISHED COMMAND PROCESSING AND EXECUTING
		// ********************************************************************************
		};//end if


	};//end while loop
	// ********************************************************************************
	// END COMMAND PROCESSING LOOP
	// ********************************************************************************


	return(0);


};//end main
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// END MAIN
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
