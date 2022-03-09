// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************

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

#define VERSION "v1.06"
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




// extern char     c64_ram;        // This is needed for the C64 RAM Driver!

// extern multi_purpose_buffer multi_buffer;              // The multi_purpose_buffer is a union so we can swap in and out different kinds of variables into an extended page of RAM.

// extern unsigned char first_xor_length_hash( const unsigned char *key );
// extern unsigned char pearson_hash( const unsigned char *key );

// ********************************************************************************
// GLOBAL VARS
// ********************************************************************************

// ********************************************************************************

unsigned char temp_deciseconds;
unsigned char temp_seconds;
unsigned char temp_minutes;

unsigned int  temp_cycles_counter;
unsigned char temp_estimate;
unsigned char temp_time_combined;

// ********************************************************************************

unsigned char supercpu_turbo_status = TRUE;

unsigned char cdarrow[] = { 'c' , 'd' , 0x5f , 0x00 };

unsigned char read_drive_number;
unsigned int  read_bytes_free;
unsigned char first_partition = 0;
unsigned char last_partition;
unsigned char sd2iec_offset = 0;

unsigned char files_per_screen;
unsigned char listing_string[MAX_LENGTH_ARGS] = ""; // TODO: OPTIMIZE OUT???

unsigned char screensaver_show_time = TRUE;
unsigned char screensaver_show_date = TRUE;
unsigned char screensaver_show_temp = FALSE;

unsigned char rtc_device = 0;
												   //  text, background, border
unsigned char color_profiles[8][3] = {{06,01,01},  //  1 - C PET Style
								      {07,02,04},  //  2 - C VIC-20 Style
								      {15,07,15},  //  3 - C 64 Style
								      {14,12,14},  //  4 - C 128 VIC-II Style
								      {04,01,01},  //  5 - C 128 VDC Style
								      {01,02,02},  //  6 - Black on White
								      {02,01,01},  //  7 - White on Black
								      {02,12,13}  //  8 - Greyscale
								      //{02,15,07},  //  9 - Default // TODO: Update this in the manuals.
								      };
// 0 == not scanned
// 1 == detected
// 2 == NOT detected --> and therefore do not attempt to read a status, which would lock up the Commodore 64.
unsigned char drive_detected[8]      = { 0,0,0,0,0,0,0,0 };
unsigned char drive_detected_type[8] = { 0,0,0,0,0,0,0,0 }; // 0 = no drive detected, all others related to a specific drive

unsigned char alias_shortcut_list[MAX_ALIASES][MAX_ALIAS_LENGTH];
unsigned char alias_command_list[MAX_ALIASES][MAX_ALIAS_LENGTH];
unsigned char hotkeys_list[MAX_HOTKEYS][MAX_HOTKEY_LENGTH];

unsigned char position_at_prompt_x = 0;
unsigned char position_at_prompt_y = 0;	
unsigned char get_key = '\0';

// ********************************************************************************

unsigned char entered_keystrokes[MAX_ENTERED_KEYSTROKES];
unsigned char command_history[MAX_COMMAND_HISTORY][MAX_ENTERED_KEYSTROKES];
unsigned char position_in_history = 255; // Default is 255 whcih means our position in history is 0, but not stashed.
unsigned char command_history_counter = 0;

// ********************************************************************************

unsigned char keystroke;
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

// unsigned char previous_entered_keystrokes[MAX_ENTERED_KEYSTROKES];
// unsigned char extracted_program_name[MAX_COMMODORE_DOS_FILENAME];

// ARRAYS
// unsigned char disk_ sector_buffer[MAX_DISK_ SECTOR_BUFFER]     = " "; // TODO: OPTIMIZE WITH PAGING
unsigned char disk_buffer[MAX_DISK_BUFFER]     = " "; // TODO: OPTIMIZE WITH PAGING
unsigned char little_disk_buffer[MAX_LENGTH_ARGS] = " "; // TODO: CREATE LITTLE 32 BUFFER HERE???

// unsigned char user_input[MAX_ENTERED_KEYSTROKES]                = ""  ; // This was straight up never used.
unsigned char user_input_processed[MAX_ENTERED_KEYSTROKES]      = ""  ;

// unsigned char user_input_command_type                       = 'e' ; // e for empty, s for string, n for number
// unsigned char user_input_arg1_type                          = 'e' ;
// unsigned char user_input_arg2_type                          = 'e' ;
// unsigned char user_input_arg3_type                          = 'e' ;

unsigned char drive_command_string[MAX_LENGTH_ARGS]		= ""  ; // Updated to MAX_LENGTH_ARGS which is 32
// unsigned char drive_command_string2[MAX_LENGTH_COMMAND]		= ""  ;

unsigned char user_input_command_string[MAX_LENGTH_COMMAND] = ""  ;
unsigned char user_input_arg1_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char user_input_arg2_string[MAX_LENGTH_ARGS]       = ""  ;
unsigned char user_input_arg3_string[MAX_LENGTH_ARGS]       = ""  ;

// unsigned long int user_input_command_number                     = 0   ; // <-- MAJOR CHANGE - Seems like this variable is never used.

unsigned int  user_input_arg1_number                        = 0   ;
unsigned int  user_input_arg2_number                        = 0   ;
unsigned int  user_input_arg3_number                        = 0   ;

unsigned char number_of_user_inputs = 0;

unsigned char * token_pointer_command;
unsigned char * token_pointer_arg1;
unsigned char * token_pointer_arg2;
unsigned char * token_pointer_arg3;

unsigned char original_position_at_prompt_y = 0;

unsigned int  error_number       ;
unsigned char error_message[32]  ;
// unsigned char error_message2[32] ;
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

const char    command_cdback[] = { 0x5F, '\0' };

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



#define temp_conversion_length 37

void print_temp(){

			// I've used a lookup table because I think
			// it's both faster and takes less space.
			// This is because the alternative is modelling this
			// as an exponential function.
			// This isn't ideal, because cc65 doesn't have
			// floating point math. But even if it did,
			// doing floating point on the 6502 is
			// very slow and takes up a lot of program space
			// and RAM space as well.
			// This table takes up about 36 x 2 = 72 bytes.
			// The average 6502 instruction takes 4 bytes.
			// Therefore, the only way to improve on this,
			// Would be to create a rountine that takes up
			// less than 18 instructions in 6502 machine code.
			// TODO: If someone wants to re-write this
			// TODO: figuring out the exponential function,
			// TODO: and thus replace the lookup table and code,
			// TODO: with better code that compliles or assembles
			// TODO: into less than 18 instructions,
			// TODO: or 72 bytes, be my guest!
			// TODO: Otherwise, this is good enough for me!

			unsigned int temp_conversion[] = 	{ // Position 0 == 30'C
													64000L,	// 31
													57135L,	// 32
													50717L,	// 33
													44044L,	// 34
													39221L,	// 35
													34473L,	// 36
													30580L,	// 37
													28000L,	// 38
													25298L,	// 39
													21837L,	// 40
													19577L,	// 41
													16826L,	// 42
													14888L,	// 43
													12943L,	// 44
													11213L,	// 45
													 9721L, // 46
													 8910L, // 47
													 8300L, // 48
													 7600L, // 49
													 6945L, // 50
													 6400L, // 51
													 5896L, // 52
													 5283L, // 53
													 4770L, // 54
													 4335L, // 55
													 3857L, // 56
													 3400L, // 57
													 3047L, // 58
													 2655L, // 59
													 2404L, // 60
													 2299L, // 61
													 2099L, // 62
													 1800L, // 63
													 1653L, // 64
													 1427L, // 65
													 1203L,  // 66
													 1146L  // 67
												};//end-array


			temp_cycles_counter = 0;
			temp_estimate 		= 0;
			temp_time_combined	= 0;

			// To measure the temperature, we see how many cycles it takes 
			// for the CPU's unused pin to return to low from high.
			// Since this varies with temperature, we can correlate the time in cycles
			// with the measured temperature, and use the above array
			// as a lookup table to quickly convert
			// from elapsed cycles to degree Celius.

			// TODO: We want to know if it's too cold and display < 30'C.
			// TODO: But we need to avoid a 2-byte unsigned int rolling over and giving us shit numbers.
			// TODO: So we should record some timer number here, and compare when the loop is finished.
			// TODO: Or byte the bullet and use rasterlines or something else.

			switch(model_detected) {
				case 3 :
				case 4 :
				case 5 :
				case 6 :
				case 7 : /* Model supported, do nothing. */ break;
				default: goto TEMP_CPU_NOT_SUPPORTED;       break;
			};//end-switch

			switch(cpu_detected) {
				case 10 :
				case 11 : /* CPU supported, do nothing. */ break;
				default: goto TEMP_CPU_NOT_SUPPORTED;      break;
			};//end-switch

			POKE(0xDC08,0); /* Start TOD Clock. Deciseconds */
			POKE(0xDC09,0); /* Start TOD Clock. Seconds     */

			POKE(0,175); 								// Set high bit direction to output.
			POKE(1,182); 								// Set high bit to high.
			POKE(0,47);  								// Set high bit direction to input.

			while(PEEK(1)==182){
				++temp_cycles_counter;
			};//end-while

			temp_deciseconds = PEEK(0xDC08);
			temp_seconds     = PEEK(0xDC09);

			// TODO: Create a varible number that's the temperature.
			// TODO: All these test just update that number.
			// TODO: For the 3.2 seconds, just add ten.
			// TODO: So 3.2 turns into 32, etc...
			// TODO: Then do the routine on that.
			// TODO: However...
			// TODO: I should be able to y=mx+b that shit into a formula.
			// TODO: Even though it's not really linear,
			// TODO: For this area on the exponential curve,
			// TODO: it's almost linear.
			// TODO: Therefore this would be an acceptable approximation.
			// TODO: Then do as few printf statements as possible.
			// TODO: Should only need 3 final printf statements!
			// TODO: The <10, the actual, and the >65.
			// Saved 203 bytes doing this without all the printf statements!

			temp_time_combined = (temp_seconds*10) + temp_deciseconds;

			if (temp_seconds >= 4) {
				printf("!! CPU < 14'C");
				goto END_TEMP;

			} else if ( (temp_seconds >= 1) && (temp_seconds <= 3) ){
		       switch (temp_time_combined) {
		   			// case 49 : /* 10'C */		/* RAN OUT OF MEMORY FOR THIS - NOT WORTH IT - HARDLY EVER GONNA BE THIS COLD!!! */
					// case 48 : /* 10'C */
					// case 47 : /* 10'C */ temp_estimate = 10; break;
					// case 46 : /* 11'C */
					// case 45 : /* 11'C */ temp_estimate = 11; break;
					// case 44 : /* 12'C */
					// case 43 : /* 12'C */ temp_estimate = 12; break;
					// case 42 : /* 13'C */
					// case 41 : /* 13'C */ temp_estimate = 13; break;
					// case 40 : /* 14'C */ temp_estimate = 14; break;

		            case 39 : /* 14'C */ temp_estimate = 14; break;
					case 38 : /* 15'C */
					case 37 : /* 15'C */ temp_estimate = 15; break;
					case 36 : /* 16'C */
					case 35 : /* 16'C */ temp_estimate = 16; break;
					case 34 : /* 17'C */
					case 33 : /* 17'C */ temp_estimate = 17; break;
					case 32 : /* 18'C */
					case 31 : /* 18'C */ temp_estimate = 18; break;
					case 30 : /* 19'C */ temp_estimate = 19; break;

		            case 29 : /* 20'C */ temp_estimate = 20; break;
					case 28 : /* 21'C */
					case 27 : /* 21'C */ temp_estimate = 21; break;
					case 26 : /* 22'C */
					case 25 : /* 22'C */ temp_estimate = 22; break;
					case 24 : /* 23'C */
					case 23 : /* 23'C */ temp_estimate = 23; break;
					case 22 : /* 24'C */
					case 21 : /* 24'C */ temp_estimate = 24; break; 
					case 20 : /* 25'C */ temp_estimate = 25; break;

		            case 19 : /* 25'C */ temp_estimate = 25; break;
					case 18 : /* 26'C */
					case 17 : /* 26'C */ temp_estimate = 26; break; 
					case 16 : /* 27'C */
					case 15 : /* 27'C */ temp_estimate = 27; break;
					case 14 : /* 28'C */
					case 13 : /* 28'C */ temp_estimate = 28; break;
					case 12 : /* 29'C */
					case 11 : /* 29'C */ temp_estimate = 29; break;
					case 10 : /* 30'C */ temp_estimate = 30; break;
				};//end-switch

				goto PRINT_TEMP;

			} else {
				for (i = 0 ; i <= temp_conversion_length-1 ; ++i) {
					if (temp_cycles_counter > temp_conversion[i]) break;
				};//end-for

				temp_estimate = i-1+31;

				if (temp_cycles_counter < 1146L) {			// This means it's less than 3712L, and therefore warmer than 50 'C.
					printf("!! CPU > 67'C"); // - Max is 70'C!"); // TODO: This needs to be different and be the same number of characters as the normal output.
					goto END_TEMP;
				} else {									// This means that we exited somewhere in between and can use the lookup table to show the temp.
					goto PRINT_TEMP;
				};//end-if
			};//end-if


			PRINT_TEMP:;
			printf("CPU Temp %u'C", temp_estimate);
			// if above 60? print warning! - Max is 70'C!");

			TEMP_CPU_NOT_SUPPORTED:;
			// printf("Not supported.");
			END_TEMP:;

};//end-func







// SAVED 47 bytes!
void wait_for_keypress() {
	while(kbhit()) { /* flush buffer */
		cgetc();
	};/*end while*/

	while(!kbhit()) {
		/*do nothing*/
	};/*end while*/
};//end-func

// ********************************************************************************
// NEW DIRECT DISK ACCESS FUNCTIONS FOR THE 1541 DISK DRIVE
// ********************************************************************************

void change_partition_command() {
	// This is only supported by the SD2IEC and CMD HD.
	part_command[0] = 'c';
	part_command[1] = 'p';
	part_command[2] = par+1;
	part_command[3] = '\0';

	// cbm_open(1, dev, 15, part_command);
	// cbm_close(1);
	execute_dos_command(part_command);

	printf("Partition %c set.\n", par+17);
};//end-func

unsigned char convert_partition_for_drive() {

	// Example: device_command[1] = convert_partition_for_drive();
	// Assumes:
	// dev = is the current device as a number
	// par = is the current partition as a character

	if ( get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD ) {
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
	// cbm_open(15, dev, 15, drive_command_string);
	// cbm_close(15);
	execute_dos_command(drive_command_string);
	printf("Root set.\n");
};//end-func


void list_and_change_1581_par(unsigned char change_par) {
	// to change: change_par == TRUE
	// to list:   change_par == FALSE

	// result = cbm_opendir(1, dev, "$"); // need to deal with errors here
	open_dir_safely(1, dev, "$"); // need to deal with errors here

	i = 0; // use this to count partitions

	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

		// result = cbm_readdir(1, &dir_ent);
		result = read_dir_safely(1, &dir_ent);

		if (result == 2) {
			break;
		};//end-if

		if (dir_ent.type == 1){ // dir_ent.name dir_ent.type dir_ent.size
			i++;
			if (i+47 == par && change_par == TRUE) { // if we are actually changing the partition
				printf(" %c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
				strcpy(drive_command_string, "/0:");
				strcat(drive_command_string, dir_ent.name);

				// cbm_open(15, dev, 15, drive_command_string);
				// cbm_close(15);
				execute_dos_command(drive_command_string);

				printf("Partition %c set.\n", par+17);
				// cbm_closedir(1);
				close_dir_safely(1);
				break;
			} else if (change_par == FALSE) {
				printf(" %c:%16.16s Size:%u BL\n", i+48+16, dir_ent.name, dir_ent.size);
			};//end-if
		};//end-if

	};//end-for

	// cbm_closedir(1); // This should only execute if we never find a partition at all.
	close_dir_safely(1); // This should only execute if we never find a partition at all.

};//end-func


unsigned char disk_label[16];
unsigned char disk_id[2];

void write_disk_header(unsigned char write_label) {

	// Instead of passing a var, we use a global var for this purpose,
	// because it's faster in C programming esepcially on the 6502 processor.

	// Replace drive_command_string with a simple 2 or 3 character variable
	// Then use drive_command_string to replace drive_command_string2

	unsigned char small_command_string[4]="i0";

	result = get_drive_type(dev);

	switch (result) {
		case DRIVE_1541 :
		case DRIVE_2031 :
		case DRIVE_SD2  :
		case DRIVE_UIEC : // This is supported but only for mounted D64 disk images.
			// These drives are supported! Keep executing.
		break;

		default :
			display_error_drive(); // It's not one of the above drive. Break out of this function.
			return;
		break;
	};//end-switch

	// This is the original initial command DOS string for drive 0.
	small_command_string[1] = par; // Here, we update the drive number with the current partition which is the variable par.

	// This is the block write command, file#:15 channel#:2 drive#:0 track#:18 sector#:0
	strcpy(drive_command_string, "u1:2,0,18,0"); // Also used for: "u2:2,0,18,0"
	drive_command_string[5] = par; // Here, we update the drive number with the current partition which is the variable par.

	// result = cbm_open(15, dev, 15, small_command_string ); // Open ???
	// result = cbm_open( 2, dev,  2, "#"); // Open command channel.
	// result = cbm_write(15, drive_command_string, sizeof("u1:2,0,18,0")); // // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u1:2,0,18,0" --> Send the block read command to load track and sector to RAM command.

	open_file_safely(15, dev, 15, small_command_string ); // Open ???
	open_file_safely( 2, dev,  2, "#"); // Open command channel.
	result = write_file_safely(15, drive_command_string, sizeof("u1:2,0,18,0")); // // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u1:2,0,18,0" --> Send the block read command to load track and sector to RAM command.

	if (write_label == TRUE) {
		// result = cbm_write(15, "b-p:2,144", sizeof("b-p:2,144")); // Send the buffer pointer command to move the pointer to the start of the disk label.
		result = write_file_safely(15, "b-p:2,144", sizeof("b-p:2,144")); // Send the buffer pointer command to move the pointer to the start of the disk label.
		strcpy(little_disk_buffer,disk_label); // Create our new disk label string.
		// read_bytes = cbm_write(2, little_disk_buffer, 16); // Write our new disk label's 16 bytes to the commmand channel.
		read_bytes = write_file_safely(2, little_disk_buffer, 16); // Write our new disk label's 16 bytes to the commmand channel.

 	} else if (write_label == FALSE) {
 		// result = cbm_write(15, "b-p:2,162", sizeof("b-p:2,162")); // Send the buffer pointer command to move the pointer to the start of the disk label.
 		result = write_file_safely(15, "b-p:2,162", sizeof("b-p:2,162")); // Send the buffer pointer command to move the pointer to the start of the disk label.
		strncpy(little_disk_buffer,disk_id,2); // Create our new disk label string.
		// read_bytes = cbm_write(2, disk_id, 2); // Write our new disk label's 16 bytes to the commmand channel.
		read_bytes = write_file_safely(2, disk_id, 2); // Write our new disk label's 16 bytes to the commmand channel.
	};//end-if

	drive_command_string[1] = '2';

	// result = cbm_write(15, drive_command_string, sizeof("u2:2,0,18,0")); // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u2:2,0,18,0" --> Send the buffer pointer command to move the pointer to the start of the disk label.
	// result = cbm_open( 15, dev, 15, small_command_string ); // Open ???
	// read_bytes = cbm_read(2, little_disk_buffer, sizeof(little_disk_buffer)); // Read the 16 bytes that are the current disk label from the commmand channel.

	write_file_safely(15, drive_command_string, sizeof("u2:2,0,18,0")); // TODO: <-- WARNING! THIS IS A *WACKJOB* THING TO DO BUT WAS THE ONLY WAY IT WOULD WORK! "u2:2,0,18,0" --> Send the buffer pointer command to move the pointer to the start of the disk label.
	open_file_safely( 15, dev, 15, small_command_string ); // Open ???
	read_bytes = read_file_safely(2, little_disk_buffer, sizeof(little_disk_buffer)); // Read the 16 bytes that are the current disk label from the commmand channel.


	close_file_safely(2); // Close the command channel.
	close_file_safely(15); // Close the other channel???

	open_file_safely(15, dev, 15, small_command_string ); // Open with the init command.
	close_file_safely(15); // Close the channel

};//end-func


// void BYTE_TO_BINARY(byte) {
//     cputc('B');
//     cputc('I');
//     cputc('N');
//     cputc(':');
//     cputc((byte & 0x80 ? '1' : '0'));
//     cputc((byte & 0x40 ? '1' : '0'));
//     cputc((byte & 0x20 ? '1' : '0'));
//     cputc((byte & 0x10 ? '1' : '0'));
//     cputc((byte & 0x08 ? '1' : '0'));
//     cputc((byte & 0x04 ? '1' : '0'));
//     cputc((byte & 0x02 ? '1' : '0'));
//     cputc((byte & 0x01 ? '1' : '0'));
// };//end_macro

// BEFORE: 47774
// AFTER:  47669
// SAVED:    105
//     * OR*
// BEFORE:   168
// AFTER:     83
// SAVED:     85
// void byte_to_binary(byte) {

// 	unsigned int counter;

// 	cputs("BIN:");

// 	counter=1;while(1){
// 	    cputc((byte & counter ? '1' : '0'));
// 		if(counter>=256)break;counter=counter*2;
// 	};//end-while

// };//end_macro

void byte_to_binary(byte) {

	unsigned int counter;

	cputs("BIN:");

	counter=128;while(1){
	    cputc((byte & counter ? '1' : '0'));
		if(counter==1)break;counter=counter/2;
	};//end-while

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

	} else {

		char_to_convert = '.' ;                                     // output this instead 

	};//end-if

	return char_to_convert ;

};//end func


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
				revers( TRUE );
				gotoxy( x+current_x , y+current_y );
				cputc( CHAR_SQUARE );
			} else {
				revers( FALSE );
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


unsigned char ntsc_status = TRUE; // NTSC is the TRUE C64!!!


void fifteen_nops(void) {
    /* 10 */	__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");
    /* 10 */	__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");
    /* 10 */	__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");
    /* 10 */	__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");
    /*  5 */	__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");__asm__ ("NOP");
};//end-func


unsigned char 	  raster_scan_line;
unsigned long int estimated_original_hz;
unsigned char     estimated_mhz_only;
unsigned long int estimated_remaining_hz;

// Using this for the fastest loops below for CPU MHz detection.
// https://cc65.github.io/doc/cc65.html#register-vars
#pragma register-vars(on)

void print_speed(unsigned char display_mhz) {

 	register unsigned char cycles_to_burn; // This is a zero page variable so it's super fast. https://cc65.github.io/doc/cc65.html#register-vars
	unsigned long int scaling_factor;
	unsigned char detected_cpu;
	unsigned char mhz_string[6];

	// 1.02 MHz NTSC / 0.985 MHz PAL
	// The clock speed doubles when 80-column display modes are in use (2.02 MHz for NTSC, 1.97 MHz for PAL)
	// C64  CLOCK=1022730 for NTSC systems and CLOCK= 985250 for PAL systems.
	// C128 CLOCK=2045460 for NTSC systems and CLOCK=1970500 for PAL systems.

	// TODO: FUCKING PAL! I FUCKING HATE PAL!!!
	// TODO: I need to make a function full of 10 NOP's
	// TODO: And then call that to save some space.
	// TODO: And then I need to make PAL versions
	// TODO: For the C64/C128 and the SuperCPU.
	// TODO: FUCK YOU PAL!

       estimated_mhz_only = 0;
   estimated_remaining_hz = 0;
	estimated_original_hz = 0;
	     raster_scan_line = 0;
						i = 0;
		   scaling_factor = 0;
		   cycles_to_burn = 0;

	SEI(); 				// Disable interupts.

	detected_cpu = detect_cpu();

	if ( detected_cpu == CPU_65816 ) {

		// Display mode $D011 Bit #7: Read: Current raster line (bit #8). C64 generates 262 (0-255 0) scanlines total on NTSC, 312 for Pal
		// TODO: Do teh proper bit shift thing here!
		while(1){
			if ( PEEK(0xD012) == 0 && PEEK(0xD011) == 27 ) break;
		};//end-while

		for (i = 0; i < 7; ++i) ;		// *THIS IS HERE INTENTIONALLY!* This is because this slows down the SuperCPU and somehow this makes the timing math work out perfectly!

		for (i = 1; i <= 60; ++i) {
		    fifteen_nops();
		};//end-for

		raster_scan_line = PEEK(0xD012); 												// Record the current raster scan line.

		if (ntsc_status == TRUE ) {
			scaling_factor = 122727600 ; // This is the NTSC scaling factor for 6502 based systems like the C64, C128 and Ultimate 64.
		} else {
			scaling_factor = 120200500 ; // This is the PAL >:-| scaling factor for 6502 based systems like the C64, C128 and Ultimate 64.
		};//end-if

	} else { // 6510 CPU

		POKE(0xD030,253);	// Put C128 into fast mode. This comes *BEFORE* we start the timing so it doesn't affect the timing. However, the math works out better above with the SuperCPU.

		// Display mode $D011 Bit #7: Read: Current raster line (bit #8). C64 generates 262 (0-255 0) scanlines total on NTSC, 312 for Pal
		// TODO: Do teh proper bit shift thing here!

		while(1){
			if ( PEEK(0xD012) == 0 && PEEK(0xD011) == 27 ) break;
		};//end-while

		while(cycles_to_burn != 50) { // 50 worked but doesn't scale up preciscly.
            fifteen_nops();
			++cycles_to_burn;															// Do literally nothing... one more time. Makes the math easier.
		};//end-while

		raster_scan_line = PEEK(0xD012); 												// Record the current raster scan line.

		if (ntsc_status == TRUE ) {
			scaling_factor = 94091160 ; // This is the NTSC scaling factor for 6502 based systems like the C64, C128 and Ultimate 64.
		} else {
			scaling_factor = 93598750 ; // This is the PAL >:-| scaling factor for 6502 based systems like the C64, C128 and Ultimate 64.
		};//end-if

	};//end-if

	POKE(0xD030,252);																// Put C128 into C64 *normal* mode.
	CLI(); 																			// Enable interupts.

	// if (raster_scan_line == 0) return;												// If for some reason this completes in less than a raster line of time, and to avoid dividing by zero, exti without printing anything.

	// custom shit goes here
	// else do the math below
	// This is prolly chearting prolly...
	if ( detected_cpu == CPU_65816 ) {

		switch(raster_scan_line) {
			case   5 :
			case   6 :
			case   7 : strcpy(mhz_string,"20.45"); break; // TODO: This doesn't really work until I do a better job of determining NTSC/PAL by counting rasterlines. // if (ntsc_status==TRUE) strcpy(mhz_string,"20.45"); else strcpy(mhz_string,"20.03"); break; // added 27 bytes

			case 118 :
			case 119 :
			case 120 :
			case 121 :
			case 122 : strcpy(mhz_string,"1.02"); break;

			default  : goto NON_STANDARD_RESULT;

		};//end-switch

	} else {

		switch(raster_scan_line) {
			case  0 :
			case  1 : strcpy(mhz_string,"> 41");  break;
			case  2 : strcpy(mhz_string,"40.91"); break;
			case  3 : strcpy(mhz_string,"30.68"); break;
			case  4 : strcpy(mhz_string,"20.45"); break;
			case  5 : strcpy(mhz_string,"17.90"); break;
			case  6 : strcpy(mhz_string,"15.34"); break;
			case  7 : strcpy(mhz_string,"12.78"); break;
			case  8 : strcpy(mhz_string,"10.23"); break;

			case 44 :
			case 45 :
			case 46 :
			case 47 :
			case 48 : if (ntsc_status==TRUE) strcpy(mhz_string,"2.04"); else strcpy(mhz_string,"1.97"); break;

			case 91 :
			case 92 :
			case 93 : strcpy(mhz_string,"1.02");  break;

			default  : goto NON_STANDARD_RESULT;

		};//end-switch

	};//end-if

	goto STANDARD_RESULT;

	NON_STANDARD_RESULT:;
	estimated_original_hz  = scaling_factor / raster_scan_line; 					// This is the scaling factor. C64/C128/Ultimate=171818640 and SuperCPU=225000600.
	estimated_mhz_only     = estimated_original_hz / 1000000; 						// Get only the MHz.
	estimated_remaining_hz = estimated_original_hz - (estimated_mhz_only*1000000);	// Get the rest of the hz
	estimated_remaining_hz = estimated_remaining_hz / 10000; 						// We only want the lazt 2 decimals.

	if (display_mhz == TRUE) printf("%u.%02lu MHz", estimated_mhz_only, estimated_remaining_hz );			// Print the result if it's been told to.

	goto END_SPEED;

	STANDARD_RESULT:;
	if (display_mhz == TRUE) printf("%s MHz", mhz_string);

	END_SPEED:;

};//end-func



void sysinfo(unsigned char is_drive_detection_disabled) {

    unsigned char how_many_fit_vice_pattern = 0;
    unsigned char how_many_fit_thec64mini_pattern = 0;
    unsigned char how_many_nibbles_fit_emulator_pattern = 0;
    unsigned char previous_drive_y = 0;

	starting_x = wherex();
	starting_y = wherey();

	supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

	ntscpal_detected = detect_ntsc_pal();
		sid_detected = detect_sid();
	 kernal_detected = detect_kernal();
	  model_detected = detect_model();
	    cpu_detected = detect_cpu();

	supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

	switch(ntscpal_detected) {
		case 0 : ntsc_status = TRUE;  break; // NTSC
		case 1 : ntsc_status = FALSE; break; // PAL
	};//end switch

	display_logo(starting_x,starting_y+1);

	gotoxy(starting_x+14,starting_y+2); cputs("MODEL");
	gotoxy(starting_x+14,starting_y+3); cputs("TYPE");
	gotoxy(starting_x+14,starting_y+4); cputs("BIOS");
	gotoxy(starting_x+14,starting_y+5); cputs("CPU");
	gotoxy(starting_x+14,starting_y+6); cputs("GPU");
	gotoxy(starting_x+14,starting_y+7); cputs("SOUND");

	#define SYS_INFO_RESULTS_OFFSET 20

	// For debugging. This let's me see if things fail specifically.
	#define showquestion() /* cputs("?") */

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+2);

    // Simple default emulation detection
    // Based on the emulator VICE
	// Unused (16 bytes) at the end of screen memory: $07F8-$07FF 2040-2047
	// VICE default for this area of mem is to initialize it:
	// 00 00 FF FF FF FF 00 00
	// 00 00 FF FF FF FF 00 00

    for(i = 0; i <= 7 ; i++){
        if ( (PEEK(56296L+i) & 0x0F) == 0 ) how_many_nibbles_fit_emulator_pattern++;
        if (  PEEK(2032+i) == 0xFF)         how_many_fit_thec64mini_pattern++; 			// This range tends to be not all FF on a single real C64 I have, whereas the range.
    };//end_for

    if (PEEK(2024) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2025) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2026) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2027) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2028) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2029) == 0xFF) how_many_fit_vice_pattern++;
    if (PEEK(2030) == 0x00) how_many_fit_vice_pattern++;
    if (PEEK(2031) == 0x00) how_many_fit_vice_pattern++;

	// Most of the time, the first check is good enough to detect VICE without accidentally thinking a real C64 is in fact emulated.
	// However, the SuperCPU mirrors RAM and therefore the how_many_nibbles_fit_emulator_pattern check doesn't work.
	// Thusly, we test for the presence of the SuperCPU's WDC 65816 chip, and the regular how_many_fit_vice_pattern test.
	if ( (how_many_fit_vice_pattern == 8) && (how_many_nibbles_fit_emulator_pattern == 8 || cpu_detected == 2) ) { // so if 1 out of 7 values is either 0x00 or 0xFF 
	    cputs("VICE Emu C");
	} else if ( how_many_fit_thec64mini_pattern == 8 && how_many_nibbles_fit_emulator_pattern == 8) { // so if 1 out of 7 values is either 0x00 or 0xFF 
	    cputs("TheC64   C");
	} else {
	    cputs("Commodore ");
	};//end_switch

	// TODO: For the weird problem, maybe I need to move this code to run and display itself *BEFORE* the emulator detection!
	switch(model_detected) {
		case 0 : cputs("128D");       break;
		case 1 : cputs("128");        break;
		case 2 : cputs("128DCR");     break;
		case 3 : cputs("64 (Early)"); break;
		case 5 :
		case 7 : cputs("64C");        break;
		case 8 : cputs("SX-64");      break;
		case 9 : cputs("4064");       break;
		case 4 :
		case 6 :
		default: cputs("64");         break; // Catch-all when we have trouble detecting the exact model.
	};//end switch 

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+3);
	switch(ntscpal_detected) {
		case 0 : cputs("NTSC"); break;
		case 1 : cputs("PAL");  break;
		default: showquestion();break; // Should never see this!
	};//end switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+4);
	switch(kernal_detected) {
		case  1 : cputs("R1 901227-01");   break; // R1 C64 First Kernal
		case  2 : cputs("R2 901227-02");   break; // R2 C64 Early Kernal
		case  3 : cputs("R3 901227-03");   break; // R3 C64 Kernal
		case  4 : cputs("R3 251104-04");   break; // R4 SX-64 Common Latest Kernal
		case  5 : cputs("R3 251104-01");   break; // R1 SX-64 Rare First Kernal
		case  6 : cputs("R3 901246-01");   break; // 4064 Educator 64 --> ? Fair to say this is most similar to a stock R3 901227-03 kernal. From here: "Note that some patches of 901227-03 are included." http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/revisions.txt
		case 10 : cputs("R3 JiffyDOS ");   break; // R3 C64 JiffyDOS
		case 11 : cputs("SX-64 JiffyDOS"); break; // R3 SX-64 JiffyDOS
		default : showquestion();          break; // Default Unknown Kernal
	};//end switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+5);
	//cputs("MOS ");

	// /* Possible returns of getcpu() */
	// #define CPU_6502        0
	// #define CPU_65C02       1
	// #define CPU_65816       2
	// #define CPU_4510        3
	// #define CPU_65SC02      4
	// #define CPU_65CE02      5
	// #define CPU_HUC6280     6
	// #define CPU_2A0x        7
	// #define CPU_45GS02      8

	switch(cpu_detected) {
		case 11 : cputs("MOS 6510 ");  break;
		case 10 : cputs("MOS 8500 ");  break;
		case 9  : cputs("MOS 8502 ");  break;
		case 2  : cputs("WDC 65816 "); break;
		case 0  : cputs("MOS 6502 "); break;
		default : cputs("MOS ");       break; // printf("R:%u",cpu_detected); break;
	};//end-switch

	print_speed(TRUE);

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+6);
	cputs("VIC-II ");

	// switch(model_detected) {
	// 	case 0 : cputs("128D");       break;
	// 	case 1 : cputs("128");        break;
	// 	case 2 : cputs("128DCR");     break;
	// 	case 3 : cputs("64 (Early)"); break;
	// 	case 5 :
	// 	case 7 : cputs("64C");        break;
	// 	case 8 : cputs("SX-64");      break;
	// 	case 9 : cputs("4064");       break;
	// 	case 4 :
	// 	case 6 :
	// 	default: cputs("64");         break; // Catch-all when we have trouble detecting the exact model.
	// };//end switch 

    switch(model_detected) {
        case 0 : case 1 : case 2 : // C128 C128D C128DCR
            switch(ntscpal_detected) {
                case 0 : cputs("8564");      break; // VIC-II in the NTSC C128
                case 1 : cputs("8566/8569"); break; // VIC-II in the PAL C128
            };//end_switch
        break;

        case 5 : case 7 :  // C64C
            switch(ntscpal_detected) {
                case 0 : cputs("8562"); break; // VIC-II in the NTSC C64C
                case 1 : cputs("8565"); break; // VIC-II in the PAL C64C
            };//end_switch
        break;

        case 4 : case 6 : case 8 : case 9 :  // C64 SX-64 4064
            switch(ntscpal_detected) {
		        case 0 : cputs("6567");      break; // VIC-II in the NTSC C64
		        case 1 : cputs("6569/6572"); break; // VIC-II in the PAL C64
            };//end_switch
        break;

		default :
			showquestion();
		break;

    };//end_switch

	gotoxy(starting_x+SYS_INFO_RESULTS_OFFSET,starting_y+7);
	cputs("SID ");
	switch(sid_detected) {
		case    1 : cputs("6581");  break;
		case    2 : cputs("8580");  break;
		default   : showquestion(); break;
	};//end switch

	switch(is_drive_detection_disabled) {

		case TRUE : // THIS IS CRASHES SOMETIMES AND WHEN I USE THE BUSCARD.
			gotoxy(starting_x+14,starting_y+8); // if drive 8, display it 
			printf("Detect disabled.");
		break;

		default :

			gotoy(starting_y+8); // Starting position

			for (i = 0 ; i <= 7 ; i++) {

				gotox(starting_x+14); 				// Go the starting location for y.
				previous_drive_y = 	wherey();		// Remember the line we are on.
				result = detect_drive(8+i, 3);  	// Detect the drive, and print the device string, if any.
				if (result==255) {					// If a drive is NOT detected, or the BusCardII drive NOT detected...
					gotoy(previous_drive_y);		// ...move the cursor back to where it was so we don't print blank lines.
				} else {							// If a drive IS detected then...
					gotoy(previous_drive_y+1);		// ...move the cursor down one.
				};//end-if

			};//end-for

		break;

	};//end-switch

	// OLD gotoy(wherey()+1);

	if (wherey() < 15) {
		gotoy(15);
	};//end-if

	gotox(0);

};//end func 




// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************
// ********************************************************************************

void find_rtc_device(void) {

	// TODO: This assume the SD2IEC has at the very least the software clock. Doesn't deal with SD2IEC devices with nothing enabled at all.

	unsigned char drive_to_test;

	rtc_device = 0;

	for (i = 8 ; i <= 15 ; i++) {

		// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		drive_to_test = get_drive_type(i);

		// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

		if ( drive_to_test==DRIVE_UIEC || drive_to_test==DRIVE_CMDHD || drive_to_test==DRIVE_IDE64) { // This is the list of supported RTC devices.

			rtc_device = i;

		};//end-if
	};//end-for

};//end-func

// This saved: 72 bytes!!!!
void datetime_helper() {

	find_rtc_device();

	// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

	if ( rtc_device == 0 ) {
		// printf("Error, no RTC found!\n");
		return;
	};//end-if

	// result = cbm_open(15, rtc_device, 15, "t-ra");
	result = open_file_safely(15, rtc_device, 15, "t-ra");
	do {
		// read_bytes = cbm_read(15, little_disk_buffer, sizeof(little_disk_buffer));
		read_bytes = read_file_safely(15, little_disk_buffer, sizeof(little_disk_buffer));
	} while( read_bytes == sizeof(little_disk_buffer) ); //end loop
	// cbm_close (15);
	close_file_safely(15);

	// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

};//end-func


void display_date_nice() {

	datetime_helper();

	// Convert the day-of-the-week to upper case.
	little_disk_buffer[0] = toupper(little_disk_buffer[0]);

	// Display the date sub-string
	printf("%.14s", little_disk_buffer);

};//end func 

void display_time_nice() {

	datetime_helper();

	// Convert the time am or pm to upper case.
	little_disk_buffer[23] = toupper(little_disk_buffer[23]);
	little_disk_buffer[24] = toupper(little_disk_buffer[24]);

	// Display the time sub-string
	printf("%.11s", little_disk_buffer+14);

};//end func 

// This saved FUCKING 545!!!! This is because this was the last call of scanf!!! So it doesn't include the whole thing in the binary. SWEET!!!
// // Variable to get input.
// unsigned char user_weekday_input[3];
// unsigned char user_ampm_input[2];
// unsigned char user_date_input[2];

// void date_helper() {
// 	scanf("%s", &user_date_input);
// 	strcat(little_disk_buffer,user_date_input);
// };//end-func


// ********************************************************************************

void display_title_screen(unsigned char has_ddd) {
	clrscr();
	display_title_text();
	sysinfo(has_ddd);
	pet_chirp();

	// Might do this later.
	// find_rtc_device();
	// switch (rtc_device) {
	// 	case 0  : break;
	// 	default : display_date_nice(); display_time_nice(); printf("\n"); break;
	// };//end-switch

	printf("Ready!\n");
};//end-func


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

		if (screensaver_show_temp == TRUE ) {
			gotoxy(current_x, current_y+13);
			print_temp();
		} else {
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
		};//end-if

		//wait(32766);
		delay_one_second();
		delay_one_second();

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
// ********************************************************************************
// ********************************************************************************
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

	// ********************************************************************************
	// DEFAULT ALIASES
	// ********************************************************************************
	// sizing( "1234567890123456" , "1234567890123456" )  // Adds approx. 25 bytes per call.
	set_alias( "cd.."             , "cd .."            ); // 1
	set_alias( cdarrow            , "cd .."            ); // 2
	set_alias( "cd/"              , "cd /"             ); // 3
	set_alias( "mount"            , "cd"               ); // 4
	set_alias( "unmount"          , "cd .."            ); // 5
	set_alias( "date"             , "datetime -d"      ); // 6
	set_alias( "time"             , "datetime -t"      ); // 7
//	set_alias( "screensaver"      , "ss"               ); // 8
	// sizing( "1234567890123456" , "1234567890123456" )

	// ********************************************************************************
	// SET DEFAULT HOTKEYS
	// ********************************************************************************
	//  sizing(X,"1234567890123456" )  // Adds approx. 10 bytes per call.
	set_hotkey(1,"ls"               ); // 1
//	set_hotkey(2,""                 ); // 2
	set_hotkey(3,"cls"              ); // 3
//	set_hotkey(4,""                 ); // 4
	set_hotkey(5,"stopwatch"        ); // 5
 	set_hotkey(6,"sysinfo"          ); // 6
	set_hotkey(7,"ss"               ); // 7
	set_hotkey(8,"help /"           ); // 8
    //  sizing(X,"1234567890123456" )

	// ********************************************************************************
	// PROGRAM START
	// ********************************************************************************

	// ********************************************************************************
	// PROCESS ARGUMENTS
	// ********************************************************************************

    // ***** USED FOR DEBUGGING IN THE FUTURE *****
	// if ( argc > 1  &&  matching("-ddd",argv[1]) ) {
	// 		have_device_numbers_changed = TRUE; // this is loaded like this: RUN:REM -ddd // RUN:REM ARG1 " ARG2 IS QUOTED" ARG3 "" ARG5
	// };//end-if


	// ********************************************************************************
	// PUT THE CURSOR AT THE TOP OF THE SCREEN
	// ********************************************************************************

	starting_x = wherex();
	starting_y = wherey();




	// Display the logo, hardware info, and play the chirp 

	if ( argc > 1  &&  matching("-st",argv[1]) ){
		// Read colors for text, background, and border, and set the vars so things don't get screwy 
		// Color RAM:
		// $D800-$DBE7 	55296-56295 		1/2 kB (1000 nibbles) of color memory.
		// $DBE8-$DBFF 	56296-56319 		Unused 

		current_textcolor   = read_nibble_low(0xD800);// PEEK(0xD800) & 0x0F ;
	    current_bgcolor     = read_nibble_low(0xD021);// PEEK(0xD021) & 0x0F ; // need to read the low nibble, just doing a -240 this isn't right, because it depends on the high nibble always being 1111 xxxx
	    current_bordercolor = read_nibble_low(0xD020);// PEEK(0xD020) & 0x0F ;

		textcolor(   current_textcolor   );
		bgcolor(     current_bgcolor     );
		bordercolor( current_bordercolor );

	} else {
		// New White on Black /* White on L.Blue on Blue */
		set_profile_colors(7);

		// NEW
		display_title_screen(have_device_numbers_changed);

	};//end if 

	//set current directory to whatevr it is.
	dev = getcurrentdevice();

	// ********************************************************************************
	// CHECK FOR RTC AND CORRECTLY SET
	// ********************************************************************************

	screensaver_show_time = FALSE; // The default is to not show the date and time in the screensaver,
	screensaver_show_date = FALSE; // unless we have found an RTC and it is set correctly.

	find_rtc_device(); // Attempt to find a Real-Time Clock RTC device. This has SuperCPU disabling and enabling built-in.

	if (rtc_device != 0) { // We found a RTC device, this is the number of the first device found.

		// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		// result = cbm_open(15, rtc_device, 15, "t-ra"); // Send date and time command.
		result = open_file_safely(15, rtc_device, 15, "t-ra"); // Send date and time command.
		do { 											// Read the time and date.
			// read_bytes = cbm_read(15, little_disk_buffer, sizeof(little_disk_buffer));
			read_bytes = read_file_safely(15, little_disk_buffer, sizeof(little_disk_buffer));
		} while( read_bytes == sizeof(little_disk_buffer) );
		// cbm_close (15);									// Close the command channel.
		close_file_safely(15);									// Close the command channel.

		// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

		process_status(little_disk_buffer); // Process this as if it was a status.

		// SD2IEC RTC Status Message Error Numbers:
		// 0  - There is a RTC and it is set properly.
		// 31 - There is a RTC *but* it is not set.
		// 30 - There is *no* RTC.

		if ( error_number == 0 ) {
			screensaver_show_time = TRUE;
			screensaver_show_date = TRUE;
		};//end-if

	};//end-if


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

		// user_input_command_number = 0 ; // <-- MAJOR CHANGE - Seems like this variable is never used.
		user_input_arg1_number    = 0 ;
		user_input_arg2_number    = 0 ;
		user_input_arg3_number    = 0 ;

		// user_input_command_type   = 'e' ;
		// user_input_arg1_type      = 'e' ;
		// user_input_arg2_type      = 'e' ;
		// user_input_arg3_type      = 'e' ;

		number_of_user_inputs     = 0   ;

		// PRINT PROMPT
		textcolor  ( current_textcolor   ); // These added 10 bytes!
		bgcolor    ( current_bgcolor     );
		bordercolor( current_bordercolor );

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
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// ********************************************************************************
			// PROCESS KEYSTROKE
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
			// ENTER
			// ********************************************************************************
			if (keystroke == 13 || keystroke == 141) { 	 // return or shift/cmd return 

			    if ( position_in_string == strlen(entered_keystrokes) ) {
			    	printf(" \n"); // this erases the cursor 
			    } else {
			    	printf("%c\n",entered_keystrokes[position_in_string]); //this erases teh cyursor and restores the character underneth it 
			    };//end-if

				//TODO: This needs to be different. A scan to test if all the chars are spaces, which is tested before before copying the current to teh previosu 
				remove_extra_spaces(entered_keystrokes); 

				if (strlen(entered_keystrokes) != 0) {

					// Shift Update
					// strcpy(command_history[4],command_history[3]); // Copy 3 to 4 --> i == 3
					// strcpy(command_history[3],command_history[2]); // Copy 2 to 3 --> i == 2
					// strcpy(command_history[2],command_history[1]); // Copy 1 to 2 --> i == 1
					// strcpy(command_history[1],entered_keystrokes); // Copy E to 1 --> i == 0

					// Shift Update
					command_history_counter = MAX_COMMAND_HISTORY-2; // Not starting that zero, because that's for the currently stashed entered characters.
					while(1){

						// printf("command_history_counter:%u\n",command_history_counter);

						if (command_history_counter==0) {
							strcpy(command_history[1],entered_keystrokes); // Copy E to 1.
							// printf("ch[%u],ek[]\n",1);
						} else {
							strcpy(command_history[command_history_counter+1],command_history[command_history_counter]); // Copy 1 to 2
							// printf("ch[%u],ch[%u]\n",command_history_counter+1,command_history_counter);
						};//end-if

						if(command_history_counter==0)break;--command_history_counter; // We break out when it's three, because four gets lost.
					};//end-while

				};//end-if

				command_history[0][0]='\0'; // Clear stashed command.
				position_in_history = 255; // Reset for next run.

				break; // break out of this loop to beginning processing and executing command


			// ********************************************************************************
			// BACKSPACE
			// ********************************************************************************
			} else if ( keystroke == 20 || keystroke == 19 || keystroke == 8 ) { // BACKSPACE = 20  or CLR HOME or CTRL+H

				cursor_backspace();


			// ********************************************************************************
			// DELETE
			// ********************************************************************************
			} else if (keystroke == 148  || keystroke == 147 || keystroke == 27) { // CMD+DEL or SHIFT+DEL or CTRL+: as FORWARD DELETE This is supposed to work on a real C64 as well as VICE from a positional sperspective and in teh deafult mode of VICE (ie -= you dont' ahve  to the chagne teh keybaord settings)

				cursor_delete();


			// ********************************************************************************
			// UP CURSOR
			// ********************************************************************************
			} else if (keystroke == 145 || keystroke == 9) {	// UP    = 145 or CTRL+I = 9

				previous_x = position_at_prompt_x;
				previous_y = position_at_prompt_y;

				gotox(previous_x);
				gotoy(previous_y);

				original_position_at_prompt_y = previous_y;

				result = display_blank_substring(entered_keystrokes, 0);
				if (result > 0) { /* has scrolled */
					previous_y = previous_y-result;
				};/*end if*/

				gotox(previous_x);
				gotoy(previous_y);

				/* press up */
				if (position_in_history == 255 || position_in_history == 0) {
					strcpy(command_history[0],entered_keystrokes); // stash here
					position_in_history = 1;
				} else if (position_in_history >= MAX_COMMAND_HISTORY-1) {
					position_in_history = MAX_COMMAND_HISTORY-1;
				} else {
					++position_in_history;
				};/*end-if*/

				memset( entered_keystrokes,        0, sizeof(entered_keystrokes)    );
				strcpy(entered_keystrokes,command_history[position_in_history]);

				result = display_substring(entered_keystrokes, 0);
				if (result > 0) { /* has scrolled */
					previous_y = previous_y-result;
					position_at_prompt_y = position_at_prompt_y-result;
				};/*end-if*/

				previous_x = wherex();
				previous_y = wherey();

				cputc(CURSOR_CHARACTER);
				gotox(previous_x);
				gotoy(previous_y);

				position_in_string = strlen(entered_keystrokes);

				if ( original_position_at_prompt_y == SCREEN_BOTTOM_EDGE  &&
					wherey() == SCREEN_BOTTOM_EDGE-1 ) {
					position_at_prompt_y--;
				};/*end if*/


			// ********************************************************************************
			// DOWN CURSOR
			// ********************************************************************************
			} else if (keystroke == 17 || keystroke == 11) {	// DOWN  = 17  or CTRL+K = 11

				previous_x = position_at_prompt_x;
				previous_y = position_at_prompt_y;

				gotox(previous_x);
				gotoy(previous_y);

				original_position_at_prompt_y = previous_y;

				result = display_blank_substring(entered_keystrokes, 0);
				if (result > 0) { /* has scrolled */
					previous_y = previous_y-result;
				};/*end if*/

				gotox(previous_x);
				gotoy(previous_y);

				/* press down */
				if (position_in_history == 255 || position_in_history == 0 ) {
					/* Do nothing. */
				} else {
					--position_in_history;
				};/*end-if*/

				memset( entered_keystrokes,        0, sizeof(entered_keystrokes)    );
				strcpy(entered_keystrokes,command_history[position_in_history]);

				result = display_substring(entered_keystrokes, 0);
				if (result > 0) { /* has scrolled */
					previous_y = previous_y-result;
					position_at_prompt_y = position_at_prompt_y-result;
				};/*end-if*/

				previous_x = wherex();
				previous_y = wherey();

				cputc(CURSOR_CHARACTER);
				gotox(previous_x);
				gotoy(previous_y);

				position_in_string = strlen(entered_keystrokes);

				if ( original_position_at_prompt_y == SCREEN_BOTTOM_EDGE  &&
					wherey() == SCREEN_BOTTOM_EDGE-1 ) {
					position_at_prompt_y--;
				};/*end if*/


			// ********************************************************************************
			// LEFT CURSOR
			// ********************************************************************************
			} else if (keystroke == 157 || keystroke == 10) {	// LEFT  = 157 or CTRL+J = 10

				cursor_left();


			// ********************************************************************************
			// RIGHT CURSOR
			// ********************************************************************************
			} else if (keystroke == 12 || keystroke == 29) {	// RIGHT = 29  or CTRL+L = 12

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
			// CTRL+D - Switch to Next Device Number - For Robin
			// ********************************************************************************
			} else if (keystroke == 4) { // || keystroke == 92) {	// CTRL+: (TEMP british pound key)

				// Drive cycling shortcut using CTRL+D? So create a keycode entry,
				// then look at the current dev number, then cycle until max device number,
				// going back to 8. Make it work like a hotkey but it's hard wired.

				unsigned char dev_to_test = dev;

				memset(entered_keystrokes,0,sizeof(entered_keystrokes));
				printf(" \n");																	// This erases the cursor.

				switch(have_device_numbers_changed) {
					case TRUE :
						printf("Drive detect disabled.\n");
						goto NO_DEVICE_TO_SWITCH_TO;
					break;
				};//end-switch

				for ( i = 1 ; i <= 8 ; ++i ) {

					if (dev_to_test+i > 15) { 													// If past max device, loop around to the beginning.
						dev_to_test = 8;
						i = 0;
					};//end-if

					if (get_drive_detection(dev_to_test+i) == 1) break;

					if (dev_to_test+i == dev) goto NO_DEVICE_TO_SWITCH_TO;

				};//end-for

				strcpy(entered_keystrokes,"cd d10:"); 											// Setup the string, so we only need to update the first digit in two cases.

				switch (dev_to_test+i) { 														// Convert the second digit.
					case  8 : entered_keystrokes[4] = '0'; entered_keystrokes[5] = '8'; break;
					case  9 : entered_keystrokes[4] = '0'; entered_keystrokes[5] = '9'; break;
					case 10 : 							   entered_keystrokes[5] = '0'; break;
					case 11 : 							   entered_keystrokes[5] = '1'; break;
					case 12 : 							   entered_keystrokes[5] = '2'; break;
					case 13 : 							   entered_keystrokes[5] = '3'; break;
					case 14 : 							   entered_keystrokes[5] = '4'; break;
					case 15 : 							   entered_keystrokes[5] = '5'; break;
				};//end_switch

				NO_DEVICE_TO_SWITCH_TO:;

			    break;


			// // ********************************************************************************
			// // CTRL+E - Switch to Previous Device Number
			// // ********************************************************************************
			// } else if (keystroke == 5) { // || keystroke == 92) {	// CTRL+: (TEMP british pound key)

			// 	// So without doing some way more compliated tracking of valid devices,
			// 	// this was the easiest way to get most of what he was talking about.
			// 	// YOu press CTRL+D to cycle forward one device number,
			// 	// and CTRL+E to cycle backward one device number.
			// 	// I think it's useful enough as a shortcut for now.

			// 	// This cost about 109 bytes!

			// 	memset(entered_keystrokes,0,sizeof(entered_keystrokes));
			// 	printf(" \n"); // this erases the cursor 

			// 	strcpy(entered_keystrokes,"cd d10:"); // Setup the string, so we only need to update the first digit in two cases.

			// switch (dev) { // Convert the second digit.
			// 	case  8 : 							   entered_keystrokes[5] = '5'; break; // If  8 goto 15
			// 	case  9 : entered_keystrokes[4] = '0'; entered_keystrokes[5] = '8'; break; // If  9 goto  8
			// 	case 10 : entered_keystrokes[4] = '0'; entered_keystrokes[5] = '9'; break; // If 10 goto  9
			// 	case 11 : 							   entered_keystrokes[5] = '0'; break; // If 11 goto 10
			// 	case 12 : 							   entered_keystrokes[5] = '1'; break; // If 12 goto 11
			// 	case 13 : 							   entered_keystrokes[5] = '2'; break; // If 13 goto 12
			// 	case 14 : 							   entered_keystrokes[5] = '3'; break; // If 14 goto 13
			// 	case 15 :							   entered_keystrokes[5] = '4'; break; // If 15 goto 14
			// };//end_switch

			//     break;


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
			// TAB COMPLETION LEFT ARROW / AUTO COMPLETE
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

				// Example: device_command[1] = convert_partition_for_drive();
				string_add_character(listing_string,convert_partition_for_drive());

				// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			 	// result = cbm_opendir(1, dev, listing _string);
		   		// result = cbm_readdir(1, &dir_ent); // I thinjk this will avoid it using the disk or folder name as if it were a file name and errorously auto-completing using that string.
			 	open_dir_safely(1, dev, listing_string);
		        read_dir_safely(1, &dir_ent); // I thinjk this will avoid it using the disk or folder name as if it were a file name and errorously auto-completing using that string.


		        for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

			        // result = cbm_readdir(1, &dir_ent);
			        read_dir_safely(1, &dir_ent); 

					if (strncmp(tab_complete_pointer,dir_ent.name,tab_length) == 0) { 				// 	use strncmp to compare the first X chars in the current file to the extracted text
						strncpy(new_entered_keystrokes,entered_keystrokes,tab_complete_position); 	// copy beginning of entered keystrokes
						strcat(new_entered_keystrokes,dir_ent.name); 								// aoppend the found filename to teh end
						strcpy(entered_keystrokes,new_entered_keystrokes); 							//update teh entered_keystrokes with teh tab completed version
						break; 																		// break out ot eh loop to avoid loading teh resat of the files for no reason 
                    };//end_if 

			    };//end for

		        // cbm_closedir(1);
		        close_dir_safely(1);

				// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

				cursor_home();
				cursor_end(); //erase the current entered text on teh screen, update teh screen with teh newly tab completed version, update teh screen cursor which can all be done with cursor_end()


			// ********************************************************************************
			// LEFT ARROW
			// ********************************************************************************
			// } else if (keystroke == 6) { // CTRL + <- = 6 // we need this becuase left arrow is tab complete, but a filename might have an arrow in it, so CTRL plus LEFT ARROW will now emit an arrow character to compensate
			// 	keystroke = 95; //replcace CTRL LEFT ARROW with LEFT ARROW
			// 	cursor_ add_character();
			// } else if (keystroke == 95) { // left arrow is now jsut plain left arrow
			// 	// keystroke = 95;
			// 	cursor_ add_character();

			// ********************************************************************************
			// SPECIAL SPACEBAR - This costs about 18 bytes
			// ********************************************************************************
			} else if (keystroke == 14) { // CTRL+N for SPACEBAR --> I did this for running in WARP mode under VICE, so the key repeat doesn't make it impossible.
			 	keystroke = 32;
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

					   (keystroke >= 160 && keystroke <= 168) ||
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
		// PROCESS COMMAND
		// ********************************************************************************
			// TODO: TRIM OUT BEGINNING AND ENDING WHITESPACES
			trim_leading_space(entered_keystrokes);
			trim_trailing_space(entered_keystrokes);

			// Fix maths or = command if they forget to add spaces
			if ( entered_keystrokes[0]=='=' ){

				ii = 0;

				for (i = 0; i < MAX_ENTERED_KEYSTROKES ; ++i) {

					if (entered_keystrokes[i] == '=' ||
						entered_keystrokes[i] == '+' ||
						entered_keystrokes[i] == '-' ||
						entered_keystrokes[i] == '*' ||
						entered_keystrokes[i] == '/' ){

							user_input_processed[ii]=' '; // Using this because at this point this string isn't being used.
							++ii;
							user_input_processed[ii]=entered_keystrokes[i];
							++ii;
							user_input_processed[ii]=' ';
							++ii;

					} else {

							user_input_processed[ii]=entered_keystrokes[i];
							++ii;

					};//end-if

				};//end for

				strcpy(entered_keystrokes,user_input_processed);
				user_input_processed[0]='\0'; // Clear this because it's about to be used.

			};//end-if

			do_alias(entered_keystrokes); // look for the alias string as the whole block of entered text
		    process_command();
			do_alias(user_input_command_string); // look again for an alias in the extracted command from teh entere text 

			// printf("user_input_command_string:%s\n",user_input_command_string);
			//
			// printf("user_input_arg1_string:%s\n",user_input_arg1_string);
			// printf("user_input_arg2_string:%s\n",user_input_arg2_string);
			// printf("user_input_arg3_string:%s\n",user_input_arg3_string);
			//
			// printf("user_input_arg1_number:%u\n",user_input_arg1_number);
			// printf("user_input_arg2_number:%u\n",user_input_arg2_number);
			// printf("user_input_arg3_number:%u\n",user_input_arg3_number);


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



		/**********************************************************************************/
		// LIST COMMAND
		// ********************************************************************************
		if ( matching("list",user_input_command_string) 								 							      ||
		    (user_input_command_string[0]=='l' && user_input_command_string[1]=='s') 									  ||
		    (user_input_command_string[0]=='d' && user_input_command_string[1]=='i' && user_input_command_string[2]=='r') ||
		    (user_input_command_string[0]=='l' && user_input_command_string[1]== 0 )									  ){

			// Supported usage examples:
			// ls -s
			// ls chi*
			// ls -s chi*
			// ls chi* -ls

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			files_per_screen = 24; // Default - May change in the future so this is a little redundant.

			if        (user_input_command_string[0]=='d') {
				files_per_screen = 48; // 2 column display mode
			} else if (user_input_command_string[0]=='l') {
				files_per_screen = 24; // 1 column display mode
			};//end-if

			START_LIST_COMMAND:;

			strcpy(listing_string,"$"); // The default is $ to load the current directory.

			string_add_character(listing_string,convert_partition_for_drive());

			switch (number_of_user_inputs) {
				case 2 :
					if        ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode
					} else if ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode
					} else {
						string_add_character(listing_string,':');
						strncat(listing_string, user_input_arg1_string, MAX_LENGTH_ARGS-4);
					};//end_if
				break;

				case 3 :
					if        ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode
					    string_add_character(listing_string,':');
					    strncat(listing_string, user_input_arg2_string, MAX_LENGTH_ARGS-4);
					} else if ( user_input_arg1_string[0]=='-' && user_input_arg1_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode
					    string_add_character(listing_string,':');
					    strncat(listing_string, user_input_arg2_string, MAX_LENGTH_ARGS-4);
					} else if ( user_input_arg2_string[0]=='-' && user_input_arg2_string[1]=='s' ) {
					    files_per_screen = 48; // 2 column display mode{
					    string_add_character(listing_string,':');
					    strncat(listing_string, user_input_arg1_string, MAX_LENGTH_ARGS-4);
					} else if ( user_input_arg2_string[0]=='-' && user_input_arg2_string[1]=='l' ) {
					    files_per_screen = 24; // 1 column display mode{
					    string_add_character(listing_string,':');
					    strncat(listing_string, user_input_arg1_string, MAX_LENGTH_ARGS-4);
					};//end_if
				break;
			};//end-switch



			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
		 //    result = cbm_opendir(1, dev, listing _string); // need to deal with errors here
			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			// TODO: WHY DID I HAVE TO MAKE THIS 2 TO GET IT TO WORK???
			// TODO: The error was 2 file open so maybe a previous function leaves shit open or something.
			result = open_dir_safely(2, dev, listing_string);

		    printf("Device:%i ", dev,par+16);

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

				// TODO: I'm not sure why, but *I THINK???* the SuperCPU
				// TODO: and the cc65 code for cbm_readdir (see below)
				// TODO: is reading from the device way to fast
				// TODO: and because of that, it gets garbage sometimes.
				// TODO: I think this because the two errors I get are:
				// TODO: 1) Instead of getting the last line I get error #3.
				// TODO: 2) Sometimes I get number way too big for the file size.
				// TODO: These would be at the beginning of reading a line.
				// TODO: So that's why I think it would be crapping out,
				// TODO: because sending the request way way too soon!
				// TODO: I think the fix is to either write my own
				// TODO: directory reading routines, or,
				// TODO: simply putting the SuperCPU into 1 MHz mode
				// TODO: when using the cc65 cbm_dir.c commands.

				// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
				// result = cbm_readdir(1, &dir_ent);
				// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.
				result = read_dir_safely(2, &dir_ent);

				// Moving this here, fixes a bug I discovered when trying to figure out the SuperCPU.
				// https://github.com/cc65/cc65/blob/master/libsrc/cbm/cbm_dir.c
				// The Meaning Return Codes for cbm_readdir:
				// Success - 0  - Reading directory-line was successful.
				// Success - 2  - Last line.  Then, l_dirent->size = the number of "blocks free."
				// Failure - !0 - Non-zero if reading directory failed, or no more file-names to read.
				// Failure - 3  - Call to cbm_k_readst()==TRUE before getting to the *FIRST* quote that denotes the *BEGINNING* of the file name.
				// Failure - 4  - Call to cbm_k_readst()==TRUE before getting to the *LAST* quote that denotes the *ENDING* of the file name.
				// Failure - 5  - Call to cbm_k_readst()==TRUE before getting to the file-type column.
				// Failure - 6  - Call to cbm_k_readst()==TRUE before we could read to the 0 character at the end of the line.
		    	if (result != 0 && result != 2) {
		    		printf("Er: cbm_readdir %i\n", result);
		    		break;
		    	};//end_if

			    if (number_of_files == 0) { // the size is actually drive 0 or drive 1 from PET dual drives
					printf("Name:%s\n", dir_ent.name);

			    } else if (result == 2) {

                    if (wherex() != 0) {
                        printf("\n");
                    };//end_if

				 // if (dir_ent.size == 0) {        printf("   0 B");
				 // } else if (dir_ent.size == 1) { printf(" 256 B");
				 // } else if (dir_ent.size == 2) { printf(" 512 B");
				 // } else if (dir_ent.size == 3) { printf(" 768 B");
				    if (dir_ent.size == 0) {        printf("  0.00 KB");
				    } else if (dir_ent.size == 1) { printf("  0.25 KB");
				    } else if (dir_ent.size == 2) { printf("  0.50 KB");
				    } else if (dir_ent.size == 3) { printf("  0.75 KB");
				    } else {
				    	filesize = dir_ent.size;
				    	filesize = filesize*256;
				    	filesize = filesize/1000;
				    	printf("%lu KB", filesize );
				    };//end if

					if (get_drive_type(dev) == DRIVE_IDE64) {
						cputs(" used");
					} else {
			    		cputs(" free");
					};//end if
					printf(" space.\n");

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
                             // case 0     : printf("     0 B"); break;
                             // case 1     : printf("   256 B"); break;
                             // case 2     : printf("   512 B"); break;
                             // case 3     : printf("   768 B"); break;
				    			case 0     : printf("  0.00 KB"); break;
				    			case 1     : printf("  0.25 KB"); break;
				    			case 2     : printf("  0.50 KB"); break;
				    			case 3     : printf("  0.75 KB"); break;
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

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
		 //    cbm_closedir(1);
			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			close_dir_safely(2);

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// CLEAR COMMAND
		// ********************************************************************************
		// if ( matching("clear",user_input_command_string) || matching("cls",user_input_command_string) ) {
		} else if ( matching("clear",user_input_command_string) || matching("cls",user_input_command_string) ) {

            clrscr();


		/**********************************************************************************/
		//  D##: COMMAND / CHANGE DEVICE
		// ********************************************************************************
        } else if ( user_input_command_string[0]=='d' && (user_input_command_string[3]==':'||user_input_command_string[3]=='['||user_input_command_string[3]==';') ) {

			// supercpu_disable();											// Turn off SuperCPU accelleration for timing sensitive disk routines.
			strcpy (drive_command_string,"cd");							// The "cd" command that we are (holy shit) jumping into is expecting this to be set.
			strcpy (user_input_arg1_string, user_input_command_string);	// It's also expecting this, so how much are we actually saving?
			goto START_CHANGE_DEVICE ; 									// THIS IS INSANE! This jumps into the part of the "cd" command which does this. FOR FUCK'S SAKE MAKE THIS A FUNCTION! This saves space and program time.
																		// After jumping into these routines, the "cd" command will turn on SuperCPU accelleration back on when it's finished.


		/**********************************************************************************/
		// D##?: COMMAND / CHANGE DEVICE
		// ********************************************************************************
        } else if (user_input_command_string[0] == 'd' && (user_input_command_string[4]==':'||user_input_command_string[4]=='['||user_input_command_string[4]==';')) {

			// supercpu_disable();											// Turn off SuperCPU accelleration for timing sensitive disk routines.
			strcpy (drive_command_string,"cd");							// The "cd" command that we are (holy shit) jumping into is expecting this to be set.
			strcpy (user_input_arg1_string, user_input_command_string);	// It's also expecting this, so how much are we actually saving?
			goto START_CHANGE_DEVICE_AND_PARTITION ; 					// THIS IS INSANE! This jumps into the part of the "cd" command which does this. FOR FUCK'S SAKE MAKE THIS A FUNCTION! This saves space and program time.
																		// After jumping into these routines, the "cd" command will turn on SuperCPU accelleration back on when it's finished.


		/**********************************************************************************/
		// PART COMMAND
		// ********************************************************************************
		} else if ( /* matching("partition",user_input_command_string) || */ \
					matching("part",user_input_command_string)      || \
					matching("prt",user_input_command_string)       || \
					matching("drive",user_input_command_string)     ){

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			// if ( matching("root",user_input_arg1_string) ) {
			if ( user_input_arg1_string[0]=='r' ){
				strcpy(user_input_arg1_string,"/");
			};//end-if

			par = convert_partition_string(user_input_arg1_string[0]); // convert_partition_string(user_input_arg1_string[0], par);

			// ROOT
			if (par == '/') {
				if ( get_drive_type(dev)==DRIVE_1581 ) {
					par = 0;
					change_1581_root_par();
				} else {
					// Do the cd root thing for everything else.
					strcpy (drive_command_string,"cd//");
					// result = cbm_open(1, dev, 15, drive_command_string); 
					// cbm_close(1);
				};//end-if

			} else if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) {
				change_partition_command();

			} else if (get_drive_type(dev)==DRIVE_1581) {
				change_1581_root_par();
				list_and_change_1581_par(TRUE);

			} else {
				printf("Drive %c set.\n", par+17);

			};//end-if

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// CD COMMAND
		// ********************************************************************************
		} else if ( matching("c",user_input_command_string)  ||
					matching("cd",user_input_command_string) ){

			strcpy (drive_command_string,"cd");

			switch (number_of_user_inputs) {

				case 2 :
					// cd ..
					if ( user_input_arg1_string[0]=='.' &&  get_drive_type(dev) == DRIVE_1581) {
						goto END_1581_UNSUPPORTED;

					} else if ( matching("..",user_input_arg1_string) ) {
						strcat (drive_command_string,command_cdback);

					// cd / for 1581
					} else if ( matching("/",user_input_arg1_string) && get_drive_type(dev) == DRIVE_1581) {
						change_1581_root_par();
						goto END_CD;

					// cd /
					} else 	if ( matching("/",user_input_arg1_string) ) {
						strcat (drive_command_string,"//");

					// cd d##:
					} else if ( user_input_arg1_string[0]=='d' && (user_input_arg1_string[3]==':'||user_input_arg1_string[3]=='['||user_input_arg1_string[3]==';') ) {

						START_CHANGE_DEVICE : ;

						user_input_arg1_number = convert_device_string(user_input_arg1_string[2]);
						if (user_input_arg1_number == 255) { // The above macro "function" updates user_input_arg1_number with 255 if user_input_arg1_string[2] is fucked.
							printf("Device"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_INVALID_ERROR;
						};//end-if
						change_drive(user_input_arg1_number);
						goto END_CD;

					// cd d##?:
					} else if (user_input_arg1_string[0] == 'd' && (user_input_arg1_string[4]==':'||user_input_arg1_string[4]=='['||user_input_arg1_string[4]==';')) {

						START_CHANGE_DEVICE_AND_PARTITION : ;

						user_input_arg1_number = convert_device_string(user_input_arg1_string[2]); // convert_device_string(user_input_arg1_string[2], user_input_arg1_number);	// Convert number to valid device character

						if (user_input_arg1_number == 255) { 										// If it's wrong, do this. Also ??? --> The above macro "function" updates user_input_arg1_number with 255 if user_input_arg1_string[2] is fucked.
							printf("Device"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_INVALID_ERROR;															// Jump directly out of here to the end because we are done.
						};//end-if

						change_drive(user_input_arg1_number); 										// Here we are changing the drive or partition, using the nubmer we converted above.

						par = convert_partition_string(user_input_arg1_string[3]);					// Pull the drive or partition letter out of the user input and convert it to a valid character to use in changin the partition.
						if (par == 255) { 															// If it's wrong, do this. Also ??? --> The above macro "function" updates par with 255 if user_input_arg1_string[3] is fucked.
							printf("Partition"); // printf("Er arg:%i\n", number_of_user_inputs);
							goto END_INVALID_ERROR;															// Jump directly out of here to the end because we are done.
						};//end-if																	// All that's left to do is to change the partition or drive.

						if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) {	// If it's an SD2IEC or a CMD HD
							change_partition_command();												// Change the partition using the custom function.

						} else if (get_drive_type(dev) == DRIVE_1581) {								// If it's an 1581... with it's fucking rediculus partitions functionality...
							change_1581_root_par(); 												// We are doing this first, because the request is to change directly to the device's first, second, third, etc... partition.
							list_and_change_1581_par(TRUE);											// Change the partition using the custom function.

						} else {																	// For everything else, which is single or dual drives
							printf("Drive %c set.\n", par+17);										// We jsut remember what the currently sert partition is, so it gets passed to whatever dos commands need it set.

						};//end-if

						goto END_CD;

					// cd [PARTITION] 1581
					} else if (get_drive_type(dev) == DRIVE_1581) {
						strcpy (drive_command_string,"/0:");
						strcat (drive_command_string,user_input_arg1_string);

					// cd [DIRECTORY] CMD HD
					} else if (get_drive_type(dev) == DRIVE_CMDHD) {
					// If we are on a CMD HD device, and trying to "cd" into a
					// non-directory or non-1581-partition ( 1=="CBM" 2=="DIR" )
					// then it'll let us do that, and then get screwed up.
					// So we need to prevent that. The SD2IEC handles this
					// gracefully, however, we shouldn't try it anyway.
					// i = detect_filetype(user_input_arg1_string,FALSE);
					//	if (i==1 || i==2){
						if (detect_filetype(user_input_arg1_string,FALSE)==2) {
							strcpy (drive_command_string,"cd0:");
							drive_command_string[2] = convert_partition_for_drive();
							strcat(drive_command_string,user_input_arg1_string);
						} else {
							goto END_ERROR;
						};//end-if

					// cd [DIRECTORY] VICE FS DRIVER
					} else if (get_drive_type(dev)==DRIVE_VICEFS) {
					// The VICE FS driver *DOES NOT* support drive names, even 0!!!
					// This cost me an extra 31 bytes! Every God damn thing all the God damn time...
						strcpy(drive_command_string,"cd:");
						strcat(drive_command_string,user_input_arg1_string);

					// cd [DIRECTORY] SD2IEC AND ANYTHING ELSE
					} else { // if (get_drive_type(dev) == DRIVE_UIEC || get_drive_type(dev) == DRIVE_IDE64) {
						strcpy (drive_command_string,"cd0:");
						drive_command_string[2] = convert_partition_for_drive();
						strcat(drive_command_string,user_input_arg1_string);

					// // cd [WHATEVER] on drives that don't support directories
					// } else {
					// 	goto END_ERROR;

					};//end-if

			    break;//end-case

			    default :
						goto END_ERROR;
			    break;//end-default

			};//end switch

			// result = cbm_open(1, dev, 15, drive_command_string); 
			// cbm_close(1);
			execute_dos_command(drive_command_string);

			goto END_CD;

			// Doing all this saved about 50 bytes!
			END_ERROR : ;
			display_error();
			goto END_CD;

			END_1581_UNSUPPORTED : ;
			printf("1541 can't do this.\n");
			goto END_CD;

			END_INVALID_ERROR : ;
			printf(" invalid error.\n");
			goto END_CD;

            END_CD : ; // This skips the above closing of the command channel, for cases where it isn't needed.

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// ECHO COMMAND
		// ********************************************************************************
		} else if ( matching("echo",user_input_command_string) ) {

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			printf("%s\n",user_input_arg1_string);

			// switch (number_of_user_inputs) {
			// 		case 2 :
	 		//  		printf("%s\n",user_input_arg1_string);
			//		break;
			// };//end switch


		/**********************************************************************************/
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
				display_error_arguments();
			};//end-if

			if ( result == 0 ) { // set_alias returns 0 if there are no slots left
				 printf("Er: full!\n");
			};//end_if

			result = 0; // reset it when we are done. 


		/**********************************************************************************/
		// UNALIAS COMMAND
		// ********************************************************************************
		} else if ( matching("unalias",user_input_command_string) ) {

			switch (number_of_user_inputs) {

				case 2 :
	 				clear_alias(user_input_arg1_string);
			    break;

			    default : 
			    	display_error_arguments();
			    //end default

			};//end-switch


		/**********************************************************************************/
		// HOTKEY COMMAND / HOTKEYS COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='h' && user_input_command_string[1]=='o' && user_input_command_string[2]=='t') ) {

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
				display_error();
			};//end-if


		/**********************************************************************************/
		// ABOUT COMMAND / VERSION COMMAND
		// ********************************************************************************
		} else if ( matching("about",user_input_command_string) || \
		            ((user_input_command_string[0]=='v' && user_input_command_string[1]=='e' && user_input_command_string[2]=='r')) ) {

			// display_title_text(); // title // This cost 19 bytes!
			display_description_text(); // description - This costs about 36 bytes!
			printf("GitHub.com/ChironB/ChiCLI\n\nLic  GNU GPLv3\nVer  %s\nDate %s\nTime %s\n", VERSION, __DATE__, __TIME__); // version


		/**********************************************************************************/
		// KEYCODES COMMAND
		// ********************************************************************************
		} else if ( matching("keycodes",user_input_command_string) ) {

			printf("* Hit it RS/CTRL-C quits *\n");
			do {
				do {
					//nothing
				} while ( kbhit() == 0);//end do 

				get_key = cgetc();

				printf("HEX:%02x DEC:%03u CHR:", get_key, get_key); // TODO: Make this not display control chars and have all the columns aligned in teh output. Thanks!
				cputc( convert_char(get_key) );
				printf("\n");

			// RUN/STOP or CTRL-C
			}while(get_key != 3);//end-do


		/**********************************************************************************/
		// ./ COMMAND
		// ********************************************************************************
		} else if ( user_input_command_string[0]=='.' && user_input_command_string[1]=='/' ){

			if (strlen(user_input_command_string) <= 2) goto END_ERROR_DOTSLASH;

			supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			// TODO: I think I can pass the entire path as part of the first string I pass to exec() so that it works in different folders!
			// TODO: rewrite this to inlude all args with quotes on the command line
			// strcpy(extracted_program_name,user_input_command_string+2); // THIS GIVES ME EVERYTHING EXCEPT THE FIRST TWO CHARACTERS
			// exec(extracted_program_name, user_input_arg1_string);

			// TODO: Maybe this should only let you try and run PRG files???
			if (detect_filetype(user_input_command_string+2,TRUE)==17) {
				exec(user_input_command_string+2, user_input_arg1_string);
		    	return EXIT_SUCCESS;
			} else {
				goto END_ERROR_DOTSLASH;
			};//end-if

			END_ERROR_DOTSLASH:;
			display_error();


		/**********************************************************************************/
		// RUN COMMAND
		// ********************************************************************************
		} else if ( matching("run",user_input_command_string) ) {

			// TODO: rewrite this to inlude all args with quotes on the command line 	
		    // TODO: LOADING FROM ANOTHER DRIVE ISSUE
		    // Detect if the startup drive is different from the drive we are trying to load from 
		    // and then either use the exec() from teh startup drive, or, 
		    // use teh dracopy trick to laod from the other drive
		    // then email the mailing list ot find out how to change teh drive that exec() loads from 

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			if (detect_filetype(user_input_arg1_string,TRUE)!=17) {
				goto ERROR_ARGS;
			};//end-if

            // The keyboard buffer starts from 631 decimal and ends to 640 decimal. 
            // Up to 10 keypresses can be stored on it. We can poke ASCII codes on these locations at will.
			POKE(0xD018, 21); // UPPER CASE/PETSCII MODE
		    clrscr();// clear screen
		    cputs("new");// print new
		    gotoxy(0,3);// move down 3 lines 
		    cputs("load\"");// print load"0:---name_of_program---",9

			cputc(convert_partition_for_drive());

			drive_command_string[2] = '\0';
		    cputc(':');
		    cputs(user_input_arg1_string);
		    cputs("\",");
			printf("%i,", dev); // 8,

		    if ( matching("-t",user_input_arg2_string) ) { // TMP Turbo Macro Pro shortcut
		    	cputc('1'); // LOAD "RUNME" ,8,1
		    	gotoxy(0,8);// move down 5 lines
		    	cputs("sys8*4096");// print run:rem
		    } else if ( matching("-0",user_input_arg2_string) ) {
		    	cputc('0'); // LOAD "RUNME" ,8,0
		    	gotoxy(0,8);// move down 5 lines
		    	cputs("run");// print run:rem
		    	switch (number_of_user_inputs) {
					case 4 :
						cputs(":rem ");// print run:rem
						cputs(user_input_arg3_string);// print ---argument 1---
					break;
		    	};//end-switch
		    } else if ( matching("-1",user_input_arg2_string) ) {
		    	cputc('1'); // LOAD "RUNME" ,8,1
		    	gotoxy(0,8);// move down 5 lines
		    	cputs("run");// print run:rem
		    	switch (number_of_user_inputs) {
					case 4 :
						cputs(":rem ");// print run:rem
						cputs(user_input_arg3_string);// print ---argument 1---
					break;
		    	};//end-switch
		    } else {
		    	cputc('1'); // LOAD "RUNME" ,8,1
		    	gotoxy(0,8);// move down 5 lines
		    	cputs("run");// print run:rem
				switch (number_of_user_inputs) {
					case 3 :
						cputs(":rem ");// print run:rem
						cputs(user_input_arg2_string);// print ---argument 1---
					break;
					case 4 :
						cputs(":rem ");// print run:rem
						cputs(user_input_arg2_string);// print ---argument 1---
						cputc(' ');
						cputs(user_input_arg3_string);// print ---argument 2---
					break;
		    	};//end-switch
		    };//end-if

		    gotoxy(0,10);// move down 2 lines 
  			POKE(0x0277, 19); // HOME - Push the following keystrokes into the keyboard buffer: HOME RETURN RETURN RETURN 
		    POKE(0x0278, 13); // RETURN
		    POKE(0x0279, 13); // RETURN
		    POKE(0x027A, 13); // RETURN
		    POKE(0x00C6,  4); // Push number of characters in the keyboard buffer

			supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			// em_uninstall();                             // Remove the driver on exit. em_uninstall: Uninstall an already loaded extended memory driver. --> https://cc65.github.io/doc/funcref.html#ss3.111

		    return EXIT_SUCCESS;


		/**********************************************************************************/
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

		    	exit(EXIT_SUCCESS);  // whcih allows us to call exit properly, or we could say "press return to restart" and have hidden black on black text with SYS 64738 waiting for them 
			};//end if


		// /**********************************************************************************/
		// // RESTART COMMAND
		// // ********************************************************************************
		// } else if ( matching("restart",user_input_command_string) ) {

		//     if (they_are_sure() == TRUE) {
		//     	supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
		//         exec(PROGRAM_NAME, "-r"); // This is supposed to reload whatever the program name is.
		//         return EXIT_SUCCESS; // Yes, this is in fact needed.
		//     };//end_if


		/**********************************************************************************/
		// REBOOT COMMAND
		// ********************************************************************************
		} else if ( matching("reboot",user_input_command_string) ) {

			if (they_are_sure() == TRUE) {
	    		reboot_register.pc = 0xFCE2;
	    		_sys(&reboot_register);
			};//end if


		/**********************************************************************************/
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

				// em_uninstall();                             // Remove the driver on exit. em_uninstall: Uninstall an already loaded extended memory driver. --> https://cc65.github.io/doc/funcref.html#ss3.111

		    	exit(EXIT_SUCCESS);  // whcih allows us to call exit properly, or we could say "press return to restart" and have hidden black on black text with SYS 64738 waiting for them 

			};//end if


		/**********************************************************************************/
		// @ COMMAND / @ SHORTCUT / JiffyDOS Like Shortcut - For Robin
		// ********************************************************************************
		} else if ( entered_keystrokes[0]=='@' ){

			if (entered_keystrokes[1]=='\0') {
				get_status(dev, TRUE);
			} else {
				execute_dos_command(entered_keystrokes+1);
			};//end-if


		/**********************************************************************************/
		// $ COMMAND / $ SHORTCUT / JiffyDOS Like Shortcut - For Robin
		// ********************************************************************************
		} else if ( user_input_command_string[0]=='$' ){

			if (user_input_command_string[1]=='\0') {
				goto START_LIST_COMMAND;
			} else {
				//user_input_command_string[0]=' ';
				// strcpy(user_input_arg1_string,user_input_command_string+1);
				strncpy(user_input_arg1_string,user_input_command_string+1,MAX_LENGTH_ARGS-1);
				number_of_user_inputs=2;
				// printf("user_input_arg1_string:%s\n",user_input_arg1_string);
				goto START_LIST_COMMAND;
			};//end-if


		/**********************************************************************************/
		// DOS COMMAND
		// ********************************************************************************
		} else if ( user_input_command_string[0] == 'd' && user_input_command_string[1] == 'o' && user_input_command_string[2] == 's' ) {

			// TODO: In this future, this should work like this:
			// dos cp1  -->  Executes DOS command on the current device using 1, dev, 15
			// dos 7 7 15 "do something" --> Executes DOS command as if it were done in Basic like this: OPEN 7,7,15 : PRINT#7,"DO SOMETHING" : CLOSE 7
			// So either we have 1 argument or 4 arguments.
			// TODO: Need to handle par stuff here! But do I? Not sure.

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			if (they_are_sure() == TRUE) {
				// result = cbm_open(1, dev, 15, user_input_arg1_string);
				// cbm_close(1);
				execute_dos_command(user_input_arg1_string); // This saved 27 bytes!
			};//end if


		/**********************************************************************************/
		// DISKCOPY COMMAND
		// ********************************************************************************
		} else if ( matching("diskcopy",user_input_command_string) ) {

			// TODO: In this future, this should work like this:
			// dos cp1  -->  Executes DOS command on the current device using 1, dev, 15
			// dos 7 7 15 "do something" --> Executes DOS command as if it were done in Basic like this: OPEN 7,7,15 : PRINT#7,"DO SOMETHING" : CLOSE 7
			// So either we have 1 argument or 4 arguments.

			strcpy (drive_command_string,"d1=0");

			printf("Copy drive a to b.\n");

			if (they_are_sure() == TRUE) {
				printf("DOS: %s... ", drive_command_string);

				// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

				// result = cbm_open(1, dev, 15, drive_command_string);
				// cbm_close(1);
				execute_dos_command(drive_command_string);

				// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

				printf("Done!\n");
			};//end if


		/**********************************************************************************/
		// DRIVEDETECT COMMAND / DRVDETECT COMMAND
		// ********************************************************************************
		} else if ( matching("drvdetect",user_input_command_string) ) {

			// supercpu_disable(); 		// Turn off SuperCPU accelleration for timing sensitive disk routines.

			detect_drive(dev, TRUE); 	// disabled ecverything below, so you just go to the drive you want and runt eh command. for example: d8: and enter then type drive-detect

			// supercpu_enable(); 			// Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// STATUS COMMAND
		// ********************************************************************************
		} else if ( matching("status",user_input_command_string) ) {

			// supercpu_disable(); 		// Turn off SuperCPU accelleration for timing sensitive disk routines.

			user_input_arg1_number = convert_device_string(user_input_arg1_string[2]); // convert_device_string(user_input_arg1_string[2], user_input_arg1_number) ;

			switch (number_of_user_inputs) {
				case 1  : result = get_status(dev, TRUE); 					 break;
				case 2  : result = get_status(user_input_arg1_number, TRUE); break;
			    default : display_error_arguments(); 						 break;
			};//end switch	

			// supercpu_enable(); 			// Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// INITIALIZE COMMAND / INIT COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='i' && user_input_command_string[1]=='n' && user_input_command_string[2]=='i' && user_input_command_string[3]=='t') ) { // } else if ( matching("initialize",user_input_command_string) ) {

			single_char_dos_command('i');


		/**********************************************************************************/
		// VALIDATE COMMAND / VAL COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='v' && user_input_command_string[1]=='a' && user_input_command_string[2]=='l') ) {

			single_char_dos_command('v');


		/**********************************************************************************/
		// IDE64 COMMAND - For Leif
		// ********************************************************************************
		} else if ( matching("ide64",user_input_command_string) ) {

			// ide64 -wp --> Write protects the drive.
			// ide64 -we --> Write enables the drive.

			if ( 		 matching("-wp", user_input_arg1_string) ) {
				strcpy(drive_command_string,"w-1");
			} else 	if ( matching("-we", user_input_arg1_string) ) {
				strcpy(drive_command_string,"w-0");
			} else {
				display_error();
				goto END_IDE64;
			};//end if

			execute_dos_command(drive_command_string);

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			// result = cbm_open(1, dev, 15, drive_command_string);
			// cbm_close(1);

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			END_IDE64 : ;


		/**********************************************************************************/
		// SD2IEC COMMAND
		// ********************************************************************************
		} else if ( matching("sd2iec",user_input_command_string) ) {

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			switch (number_of_user_inputs) {

				case 2 :

					result = get_status(dev, TRUE);

					if         ( matching("-hide-ext", user_input_arg1_string) ) {

						if (result != 255) {
							printf("Hiding exts... ");

							execute_dos_command("xe+");
							goto SD2IEC_FINISHED;

							// result = cbm_open(1, dev, 15, "xe+");
							// cbm_close(1);

							// if (result == 0) {
							// 	// printf("Done.\n");
							// 	goto SD2IEC_END;
							// } else {
							// 	// printf("Er: %i\n", result);
							// 	goto SD2IEC_ERROR;
							// };//end-if
						};//end-if

					} else 	if ( matching("-show-ext", user_input_arg1_string) ) {

						if (result != 255) {
							printf("Showing exts... ");

							execute_dos_command("xe-");
							goto SD2IEC_FINISHED;

							// result = cbm_open(1, dev, 15, "xe-");
							// cbm_close(1);

							// if (result == 0) {
							// 	// printf("Done.\n");
							// 	goto SD2IEC_END;
							// } else {
							// 	// printf("Er: %i\n", result);
							// 	goto SD2IEC_ERROR;
							// };//end if
						};//end if


					} else 	if ( matching("-save",     user_input_arg1_string) ) {
							printf("Saving SD2IEC config... ");

							execute_dos_command("xw");
							goto SD2IEC_FINISHED;

							// result = cbm_open(1, dev, 15, "xw");
							// cbm_close(1);

							// if (result == 0) {
							// 	// printf("Done.\n");
							// 	goto SD2IEC_END;
							// } else {
							// 	// printf("Er: %i\n", result);
							// 	goto SD2IEC_ERROR;
							// };//end if
					} else {
						// printf("Er arg\n");
						goto SD2IEC_ERROR;
					};//end-if
				break;

			    default :
			    	// printf("Er arg\n");
			    	goto SD2IEC_ERROR;
			    break;

			};//end-switch

			SD2IEC_ERROR : ;
			display_error();
			// goto SD2IEC_FINISHED;
			// SD2IEC_END : ;
			// printf("Done.\n");
			SD2IEC_FINISHED : ;
			// printf("Done.\n");

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// 1571 COMMAND
		// ********************************************************************************
		} else if ( matching("1571",user_input_command_string) ) {

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			// Single Sided Example: 1571 -s
			// Double Sided Example: 1571 -d

			strcpy(drive_command_string,"u0>m1");

			if (number_of_user_inputs!=2)       goto END_1571_ERROR;
			if (user_input_arg1_string[0]!='-') goto END_1571_ERROR;

			switch (user_input_arg1_string[1]) {
				case 's' : // Single Sided Example: 1571 -s "u0>m0"
					drive_command_string[4]='0';
				break;

				case 'd' : // Double Sided Example: 1571 -d "u0>m1"
					// Already set; Do nothing.
				break;

			    default :
			    	goto END_1571_ERROR;
			    break;//end-default
			};//end-switch

			// cbm_open(1, dev, 15, drive_command_string); // "U0>M1" = 1571 Mode
			// cbm_close(1);

			execute_dos_command(drive_command_string);

			goto END_1571;
			END_1571_ERROR : ;
			display_error();
			// goto END_1571;
			END_1571 : ;

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		// /**********************************************************************************/
		// // MOUNT COMMAND
		// // ********************************************************************************
		// } else if ( matching("mount",user_input_command_string) ) { // About 302 bytes for both

		// 	supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		// 	// If we are on a CMD HD device, and trying to "mount" anything,
		// 	// that's not supported. So prevent that from happeneing.
		// 	if (get_drive_type(dev)==DRIVE_CMDHD) goto END_MOUNT;

		// 	switch (number_of_user_inputs) {
		// 		case 2 :
		// 			strcpy (drive_command_string,"cd:"); // This doesn't need to take into account the current partition because ONLY SD2IEC supports mounting disk images.
		// 			strcat (drive_command_string,user_input_arg1_string);
		// 			strcat (drive_command_string,"");
		// 			result = cbm_open(1, dev, 15, drive_command_string);
		// 			cbm_close(1);
		// 	    break;

		// 	    default :
		// 	    	END_MOUNT :
		// 	    	printf("Er!\n");
		// 	    break;
		// 	};//end-switch

		// 	supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// // UNMOUNT COMMAND
		// // ********************************************************************************
		// } else if ( matching("unmount",user_input_command_string) ) {

		// 	supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		// 	switch (number_of_user_inputs) {
		// 		case 1 :
		// 			strcpy (drive_command_string,"cd"); // This doesn't need to take into account the current partition because ONLY SD2IEC supports mounting disk images.
		// 			strcat (drive_command_string,command_cdback);
		// 			strcat (drive_command_string,"");
		// 			result = cbm_open(1, dev, 15, drive_command_string);
		// 			cbm_close(1);
		// 	    break;	

		// 	    default : 
		// 	    	printf("Er arg\n");
		// 	    //end default
		// 	};//end switch

		// 	supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// LP COMMAND OR LD COMMAND OR LIST PARTITIONS COMMAND OR LIST DRIVES COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='l' && user_input_command_string[1]=='d') || \
					(user_input_command_string[0]=='l' && user_input_command_string[1]=='p') ){

      		// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			first_partition = 0;
			last_partition  = 0;
			sd2iec_offset   = 0;

			if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) { // || get_drive_type(dev)==DRIVE_IDE64

				memset(disk_buffer,0,sizeof(disk_buffer));

				// result = cbm_open(8, dev, CBM_READ, "$=p");
				open_file_safely(8, dev, CBM_READ, "$=p");

				// read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
				read_bytes = read_file_safely(8, disk_buffer, sizeof(disk_buffer));

				close_file_safely(8);
				// cbm_close (8);

				first_partition = 1;
				last_partition  = 0;
				// last_partition  = disk_buffer[4];
				sd2iec_offset = 1;

				for (i = 0; i <= read_bytes ; i++) {
					if (disk_buffer[i] == 0x22) { // Let's count the quote characters.
						last_partition++;
					};//end-if
				};//end-for

				last_partition = (last_partition / 2) - 2; // There's only 2 quote per entry, so divide by 2, and then we subtract 1 because we don't want the first entry because it's the header.

			} else if (get_drive_type(dev) == DRIVE_SD2 || get_drive_type(dev) == DRIVE_4040) {
				last_partition = 1;
			} else {
				last_partition = 0;
			};//end-if

			printf("Device:"); 
			detect_drive(dev, TRUE); // TODO: Maybe move? This is WHY the 1581 resets to the root partition.

			if (get_drive_type(dev) == DRIVE_1581) {

				list_and_change_1581_par(FALSE);

			} else if (get_drive_type(dev) == DRIVE_VICEFS) {

				// Do nothing.

			} else {

				for ( i = first_partition ; i <= last_partition ; i++ ) {

					strcpy(drive_command_string,"$0:");
					drive_command_string[1]=i+48;

					memset(disk_buffer,0,sizeof(disk_buffer));

					// result = cbm_open(8, dev, CBM_READ, drive_command_string);
					open_file_safely(8, dev, CBM_READ, drive_command_string);

					// read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
					read_file_safely(8, disk_buffer, sizeof(disk_buffer));

					close_file_safely(8);
					// cbm_close (8);

					read_drive_number = disk_buffer[4];
					read_bytes_free = disk_buffer[34];
					read_bytes_free = read_bytes_free + (disk_buffer[35]*256);

					printf(" %c:%16.16s %2.2s %2.2s Free:%u BL\n", read_drive_number+48+17-sd2iec_offset, disk_buffer+8, disk_buffer+26, disk_buffer+29, read_bytes_free);

				};//end-for

			};//end-if

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			// clear_out(multi_buffer);


		/**********************************************************************************/
		// PWD COMMAND - PRINT DEVICE/PARTITION
		// ******************************************************************************** 		
		} else if (user_input_command_string[0]=='p' && user_input_command_string[1]=='w' && user_input_command_string[2] == 'd') { // --> THIS SAVES 8 BYTES!!! --> } else if ( matching("cd",user_input_command_string) ) { 

			// strcpy(drive_command_string,"$0:");
			// drive_command_string[1]=i+48;

			// memset(disk_buffer,0,sizeof(disk_buffer));

			// open_file_safely(8, dev, CBM_READ, drive_command_string);
			// read_file_safely(8, disk_buffer, sizeof(disk_buffer));
			// close_file_safely(8);

			open_dir_safely(2, dev, "$");
			read_dir_safely(2, &dir_ent);
			close_dir_safely(2);

			printf("Dev:%i ", dev);
			if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) {
				printf("Part:%c", par+17);
			} else {
				printf("Drv:%c", par+17);
			};//end-if

			// printf(" Name:%16.16s \n", disk_buffer+8);
			printf(" Name:%16.16s \n", dir_ent.name);


		/**********************************************************************************/
		// MAKE-DIR COMMAND / MKDIR COMMAND
		// ********************************************************************************
		} else if ( matching("mkdir",user_input_command_string)    ||
				  ( user_input_command_string[0]=='m' && user_input_command_string[1]=='d') ) {

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			switch (number_of_user_inputs) {
				case 2 :
					strcpy (drive_command_string,"md:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					// result = cbm_open(1, dev, 15, drive_command_string);
					// cbm_close(1);
					execute_dos_command(drive_command_string);
			    break;

			    default :
			    	display_error_arguments();
			    //end-default
			};//end-switch

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// REMOVE-DIR COMMAND
		// ********************************************************************************
		} else if ( matching("rmdir",user_input_command_string)      ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='d') ) {

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			switch (number_of_user_inputs) {
				case 2 :
					strcpy (drive_command_string,"rd:");
					strcat (drive_command_string,user_input_arg1_string);
					strcat (drive_command_string,"");
					// result = cbm_open(1, dev, 15, drive_command_string);
					// cbm_close(1);
					execute_dos_command(drive_command_string);
			    break;

			    default :
			    	display_error_arguments();
			    //end default
			};//end switch

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// DELETE COMMAND
		// ********************************************************************************
		} else if (	/* matching("delete",user_input_command_string) || */
		            (user_input_command_string[0]=='d' && user_input_command_string[1]=='e' && user_input_command_string[2]=='l') ||
					(user_input_command_string[0]=='d' && user_input_command_string[1]=='l') ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='m') ){

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			switch (number_of_user_inputs) {
				case 2 :
					if ( matching("*",user_input_arg1_string) ) { 			// copy * d08:

						strcpy(listing_string,"$"); // The default is $ to load the current directory.
						string_add_character(listing_string,convert_partition_for_drive());
						dir_file_total = dir_file_count(listing_string); // get the total number of files to copy and store in dir_file_total 
						printf("Delete %i files?\n", dir_file_total);

						if (they_are_sure() == TRUE) {

							fresh_start : ; // This is a jump point. We have to start, and re-start anew, every time. Basically, when we delete a file, the list of files changes, and as such, we need to start all over again looking for the first valid candidate to delete.
											// This kind of programming may offend some but I don't care! Yeah, I broke out of a for loop just to run it all over again, and I don't even care! Fucks given == NULL.

							strcpy(listing_string,"$"); // The default is $ to load the current directory.
							string_add_character(listing_string,convert_partition_for_drive());
							dir_file_total = dir_file_count(listing_string); // get the total number of files to copy and store in dir_file_total 

							for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

                    				get_key = 0;
									if (kbhit() != 0) { /* If key has been pressed */
										get_key = cgetc();
									};/*end_if*/

									if (get_key == 3) { /* RUN/STOP or CTRL-C */
										// printf("Aborted!\n");
										get_key = 0;
										goto END_DELETE;
									};/*end_if*/

								if (loop_k == 0) { //first entry is the disk anme 
									//do nothing

								} else {
									dir_goto_file_index(loop_k);
									detected_filetype = dir_ent.type;

									strcpy (user_input_arg1_string, dir_ent.name);

									if 	(dir_ent.type == 2 || dir_ent.type == 1 ) { // 1 == "CBM" for 1581 partitions, and 2 == "DIR" for SD2IEC directories.
										// We don't print anything because if we did, it would output the same directories as skipped multiple times, because we iterate over the remaining list of file over and over again.

									} else {
										strcpy (drive_command_string,"s0:");
										drive_command_string[1] = convert_partition_for_drive();
										strcat (drive_command_string,user_input_arg1_string);

										printf("-> %s ",user_input_arg1_string);

										// result = cbm_open(1, dev, 15, drive_command_string);
										// cbm_close(1);
										execute_dos_command(drive_command_string);

										printf("Deleted\n");
										goto fresh_start; // Since we've deleted a file, the file list and length have changed, so we jump back to the fresh_start. 
														  // This is such a hacky non-modern way of doing things, however, it's also the most efficent, in terms of code size. It's what you would do in aseembly or even BASIC. This is *why* C Programming is so cool - You can do things like this! Also, *FUCK* object oriented programming!
									};//end-if

								};//end if

							};//end for

						};//end if

					} else {

						strcpy (drive_command_string,"s0:");
						drive_command_string[1] = convert_partition_for_drive();
						strcat (drive_command_string,user_input_arg1_string);

						// result = cbm_open(1, dev, 15, drive_command_string);
						// cbm_close(1);
						execute_dos_command(drive_command_string);

					};//end-if
				break;

				default :
					display_error_arguments(); // printf("Er arg:%i\n", number_of_user_inputs);
				//end-default

			};//end-switch

			END_DELETE : ;

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// FORMAT COMMAND
		// ********************************************************************************
		} else if ( matching("format",user_input_command_string) ) {

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			drive_command_string[0] = 'n';

			drive_command_string[1] = convert_partition_for_drive();

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

			    // result = cbm_open(1, dev, 15, drive_command_string);
			    // cbm_close(1);
				execute_dos_command(drive_command_string);

			    printf("Done.\n");
			    goto FORMAT_END;
		    } else {
		        goto FORMAT_END;
		    };//end_if

            FORMAT_ERROR : display_error();
            FORMAT_END   : ; // do nothing

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// RENAME COMMAND
		// ********************************************************************************
		} else if ( /* matching("rename",user_input_command_string) || */
		            (user_input_command_string[0]=='r' && user_input_command_string[1]=='e' && user_input_command_string[2]=='n') ||
					(user_input_command_string[0]=='r' && user_input_command_string[1]=='n') ){

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			strcpy (drive_command_string,"r");

			// Example: device_command[1] = convert_partition_for_drive();
			if (get_drive_type(dev) != DRIVE_VICEFS) { 			  // If it's *NOT* the VICE FS DRIVER
				string_add_character(drive_command_string,convert_partition_for_drive());
			};//end-if

			strcat (drive_command_string,":");
			strcat (drive_command_string,user_input_arg2_string);
			strcat (drive_command_string,"=");

			// Example: device_command[1] = convert_partition_for_drive();
			if (get_drive_type(dev) != DRIVE_VICEFS) { 			  // If it's *NOT* the VICE FS DRIVER
				string_add_character(drive_command_string,convert_partition_for_drive());
				strcat (drive_command_string,":");
			};//end-if

			strcat (drive_command_string,user_input_arg1_string);

			switch (number_of_user_inputs) {

				case 3 :
					// result = cbm_open(1, dev, 15, drive_command_string);
					// cbm_close(1);
					execute_dos_command(drive_command_string);
			    break;

			    default :
			    	display_error_arguments();
			    //end default

			};//end switch

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// DRIVESET COMMAND / DRVSET COMMAND
		// ********************************************************************************
		} else if ( matching("drvset",user_input_command_string) ) {

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			result = get_drive_type(dev);

			switch (number_of_user_inputs) {
				case 2 :
					switch (result) { 	   // Result of drive scan.
						case DRIVE_1541  : // They both write to the same memory area to set the device number in software
						case DRIVE_SD2   :
							// c1541_set(dev, user_input_arg1_string); // We are NOW assuming the current dev is the thing.
							c1541_set(dev, user_input_arg1_string);
						break;

						case DRIVE_UIEC  : // The SD2IEC, CMD HD, IDE64, 1570, 1571, and 1581 all use this method.
						case DRIVE_CMDHD :
						case DRIVE_IDE64 :
						case DRIVE_1570  :
						case DRIVE_1571  :
						case DRIVE_1581  :
							// uiec_set(dev, user_input_arg1_string); // We are NOW assuming the current dev is the thing.
							uiec_set(user_input_arg1_string);
						break;

						default :
							goto DRIVESET_ERROR;
							// printf("Er.\n");
						break;
					};//end-switch
				break;

			    default :
			    	goto DRIVESET_ERROR;
			    break;
			};//end-switch


			// TODO: THIS WORKS! We are going to move into the newly assigned drive by hacking our way into the "cd" command, just like my mount hack! Automatically change to the new device number. People assume this should happen when it doesn't. I don't like it... but it's better for usability.
			strcpy (drive_command_string,"cd");							// The "cd" command that we are (holy shit) jumping into is expecting this to be set.

			// Let's make a "d08:" style string.
			strcpy(user_input_arg3_string,"d"); 					// We recycle user_input_arg3_string because this command doesn't use this string.
			if ( user_input_arg1_string[0]=='8' || user_input_arg1_string[0]=='9' ) {
				strcat(user_input_arg3_string,"0");
			};//end-if
			strcat(user_input_arg3_string,user_input_arg1_string);	// We copy user_input_arg1_string because it's a string of the target drive number.
			strcat(user_input_arg3_string,":");						// The last part we need for this string.

			strcpy(user_input_arg1_string,user_input_arg3_string);  // The "cd" command expects the device in this format and in this argument string.

			printf("Drive reassigned, changing to %s now.\n",user_input_arg1_string);

			goto START_CHANGE_DEVICE ; 									// THIS IS INSANE! This jumps into the part of the "cd" COMMAND which does this. FOR FUCK'S SAKE MAKE THIS A FUNCTION! This saves space and program time.
																		// After jumping into these routines, the "cd" COMMAND will turn on SuperCPU accelleration back on when it's finished.
			DRIVESET_ERROR:;
			display_error();


		/**********************************************************************************/
		// PEEK COMMAND
		// ********************************************************************************
		} else if ( matching("peek",user_input_command_string) ) {	

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			peeked = PEEK(user_input_arg1_number);
            printf("Mem:%u is ", user_input_arg1_number);
            byte_to_binary(peeked);
            printf(" #:%u\n", peeked);


		/**********************************************************************************/
		// PEEKING COMMAND
		// ********************************************************************************
		} else if ( matching("peeking",user_input_command_string) ) {	

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

            do {
                peeked = PEEK(user_input_arg1_number); // try DC01 or 56321 for keyboard, or D419 54297 and D41A 54298 for pots, or D012 53266 a Raster counter, which is constantly changing.
                printf("Mem:%u is ", user_input_arg1_number);
                byte_to_binary(peeked);
                printf(" #:%u\n", peeked);
            } while ( !kbhit() );//end do 


		/**********************************************************************************/
		// POKE COMMAND
		// ********************************************************************************
		} else if ( matching("poke",user_input_command_string) ) {

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			if (does_string_contain_character(user_input_arg1_string,',')==TRUE) {
				// printf("Er arg: Use spaces between args.\n");
				display_error_arguments();
				goto POKE_END;
			};//end-if

			peeked = PEEK(user_input_arg1_number);

            printf("Mem:%u was ", user_input_arg1_number);
            byte_to_binary(peeked);
            printf(" #:%u\n", peeked);

			POKE(user_input_arg1_number, user_input_arg2_number);

			peeked = PEEK(user_input_arg1_number);

            printf("Mem:%u now ", user_input_arg1_number);
            byte_to_binary(peeked);
            printf(" #:%u\n", peeked);

            POKE_END:;


		/**********************************************************************************/
		// Dropping these commands saves 530 bytes! They are fairly low in their utility and not the best implementation.
		// ********************************************************************************

		// /**********************************************************************************/
		// // PEEKBIT COMMAND
		// // ********************************************************************************
		// } else if ( matching("peekbit",user_input_command_string) ) {

		// 	peeked = PEEK(user_input_arg1_number);
		// 	printf("Mem:%u Bit%u is:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
		// 	byte_to_binary(peeked);
		// 	printf(" #:%u\n", peeked);


		// /**********************************************************************************/
		// // POKEBIT COMMAND
		// // ********************************************************************************
		// } else if ( matching("pokebit",user_input_command_string) ) {

		// 	peeked = PEEK(user_input_arg1_number);

		// 	printf("Mem:%u Bit%u was:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
		// 	byte_to_binary(peeked);
		// 	printf(" #:%u\n", peeked);

		// 	switch (user_input_arg3_number) {
		// 	case 0 : POKE(user_input_arg1_number, clear_bit(peeked,user_input_arg2_number)); break;
		// 	case 1 : POKE(user_input_arg1_number, set_bit(peeked,user_input_arg2_number));   break;
		// 	};//end_switch

		// 	printf("Mem:%u Bit%u now:%u ", user_input_arg1_number, user_input_arg2_number, get_bit(peeked,user_input_arg2_number) );
		// 	byte_to_binary(peeked);
		// 	printf(" #:%u\n", peeked);


		// /**********************************************************************************/
		// // HEX2DEC COMMAND
		// // ********************************************************************************
		// } else if ( matching("hex2dec",user_input_command_string) ) {	

		// 	sscanf(user_input_arg1_string, "%x", &peek_address);
	 //    	printf("DEC:%02u \n", peek_address );


		// /**********************************************************************************/
		// // DEC2HEX COMMAND
		// // ********************************************************************************
		// } else if ( matching("dec2hex",user_input_command_string) ) {	

		// 	sscanf(user_input_arg1_string, "%u", &peek_address);
	 //    	printf("HEX:%04X\n", peek_address );


		// /**********************************************************************************/
		// // DEC2BIN COMMAND
		// // ********************************************************************************
		// } else if ( matching("dec2bin",user_input_command_string) ) {

		// 	sscanf(user_input_arg1_string, "%u", &peek_address);
  //           byte_to_binary(peek_address);
  //           printf("\n");

		// /**********************************************************************************/
		// CONVERT COMMAND
		// ********************************************************************************
		} else if ( matching("convert",user_input_command_string) ) {

			// convert -d2h 255

			switch (number_of_user_inputs) {
				case 3 :

					if ( matching("-h2d", user_input_arg1_string) ){
						sscanf(user_input_arg2_string, "%x", &peek_address);
			 			printf("DEC:%02u \n", peek_address );
			 			goto END_CONVERT;
			 		};//end-if

					sscanf(user_input_arg2_string, "%u", &peek_address);

					if ( matching("-d2h", user_input_arg1_string) ){
						printf("HEX:%04X\n", peek_address );

					} else 	if ( matching("-d2b", user_input_arg1_string) ){
						byte_to_binary(peek_address);
						printf("\n");

					} else {
						display_error_arguments();

					};//end if

				break;

			};//end-switch

			END_CONVERT:;


		// /**********************************************************************************/
		// = COMMAND / MATHS COMMAND 0--> This takes up 472 bytes!!!
		// ********************************************************************************
		} else if ( user_input_command_string[0] == '=' ) {

			long int answer, first_number, last_number;

			switch (number_of_user_inputs) {
				case 4 :
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
				break;
				default:
					display_error_arguments();
				break;
			};//end-switch


		// /**********************************************************************************/
		// SYS COMMAND - This costs 75 bytes
		// ********************************************************************************
		} else if ( matching("sys",user_input_command_string) ) {

			if (number_of_user_inputs == 1) goto ERROR_ARGS;

			sscanf(user_input_arg1_string, "%d", &reboot_register.pc);

			if (they_are_sure() == TRUE) {

				_sys(&reboot_register);

			};//end if


		// /**********************************************************************************/
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
		                	display_error_arguments();
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


		// /**********************************************************************************/
		// DUMPMEM COMMAND
		// ********************************************************************************
		} else if ( (matching("dumpmem",user_input_command_string)) ){ // (user_input_command_string[0]=='d' && user_input_command_string[1]=='m') ){

			// Simple usage:   dump-mem 0 100 kernal.bin
			// Shortcut usage: dump-mem rom kernal kernal.bin

            unsigned int  mem_counter;
            unsigned char peeked_byte; // lame
            unsigned char peeked_string[10]; // super lame

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

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
		                	display_error_arguments();
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
					strcpy (drive_command_string, "0:"  );
					strcat (drive_command_string, user_input_arg3_string); // filename
					strcat (drive_command_string, ",w,p" ); // needs to be PRG type becuase SEQ type adds the floowing text to the beginning of the file: C64File *FILENAME*

					drive_command_string[0] = convert_partition_for_drive();

				    printf("Saving %u - %u to\nfile:%s on Drv:%i\n",start_address,end_address,user_input_arg3_string,dev);

					if (they_are_sure() == FALSE) {
						break;
					};//end-if

					// result2 = cbm_open(7, dev, CBM_WRITE, drive_command_string);
					result2 = open_file_safely(7, dev, CBM_WRITE, drive_command_string);

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

						// result = cbm_write(7, peeked_string, 1);
						result = write_byte_to_file_safely(7, peeked_string);

						//printf(".");// This saved 12 bytes!
						if (end_address == 65535L && mem_counter == 65535L) {
							break;
						};//end-if
					};//end-for

					// cbm_close (7);
					close_file_safely(7);

					printf("Saved %u - %u to\nfile:%s on Drv:%i\n",start_address,end_address,user_input_arg3_string,dev);

			    break;

			    default :
					display_error_arguments();
			    break;

			};//end switch

			END_DUMP_MEM :
			printf("\n");

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		// /**********************************************************************************/
		// COPY COMMAND
		// ********************************************************************************
		} else if ( matching("copy",user_input_command_string)                                 ||
				  ( (user_input_command_string[0]=='c')&&(user_input_command_string[1]=='p') ) ){

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

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

        	if ( user_input_arg2_string[0]=='d' && (user_input_arg2_string[3]==':'||user_input_arg2_string[3]=='['||user_input_arg2_string[3]==';') ) {
            // if (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') { // copy * d08:

		    	user_input_arg2_number = convert_device_string(user_input_arg2_string[2]); // convert_device_string(user_input_arg2_string[2], user_input_arg2_number);

			} else if ( user_input_arg2_string[0]=='d' && (user_input_arg2_string[4]==':'||user_input_arg2_string[4]=='['||user_input_arg2_string[4]==';') ) {
            // } else if (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') { // copy * d08b:

		        user_input_arg2_number = convert_device_string(user_input_arg2_string[2]); // convert_device_string(user_input_arg2_string[2], user_input_arg2_number);
		        // convert_partition_string(user_input_arg2_string[3], target_par); // do it deucimo...
		        target_par = convert_partition_string(user_input_arg2_string[3]);

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
			if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) { // TODO: DOES THIS NEED 1581 SUPPORT?????
				source_par = source_par+1;
			};//end-if

			// We can't use something like this: device_command[1] = convert_partition_for_drive(); because this is custom logic for this edge case.
			if (get_drive_type(user_input_arg2_number)==DRIVE_UIEC || get_drive_type(user_input_arg2_number)==DRIVE_CMDHD) {
				// printf("Target is NOT SD2IEC!\n");
				target_par = target_par+1;
			} else if ( get_drive_type(user_input_arg2_number)==DRIVE_1581 && user_input_arg2_string[4] == ':') {
				// printf("Partition direct copying on 1581\nis not supported.\n");
				goto COPY_UNSUPPORTED;
			} else if ( get_drive_type(user_input_arg2_number)==DRIVE_1581) {
				target_par = '0';
			};//end-if

			if (get_drive_type(dev)==DRIVE_VICEFS && get_drive_type(user_input_arg2_number)==DRIVE_VICEFS) {
				goto COPY_UNSUPPORTED;
			} else if (get_drive_type(dev)==DRIVE_IDE64 && get_drive_type(user_input_arg2_number)==DRIVE_IDE64) {
				goto COPY_UNSUPPORTED;
			};//end-if

		    switch (number_of_user_inputs) {
			    case 3 :
				    if ( matching("*",user_input_arg1_string) ) { 			// copy * d08:     

						strcpy(listing_string,"$"); // TODO: If I change things here, I can make DOS wildcard copying work... I think...
						string_add_character(listing_string,convert_partition_for_drive());
					    dir_file_total = dir_file_count(listing_string); // get the total number of files to copy and store in dir_file_total 
					    printf("Files to copy: %i\n", dir_file_total);

					    if (they_are_sure() == TRUE) {

					        for (loop_k = 0; loop_k <= dir_file_total ; loop_k++) {

						        if (loop_k == 0) { // first entry is the disk name.
						        	//do nothing
						        } else { 
						        	dir_goto_file_index(loop_k);
								    detected_filetype = dir_ent.type; //use this to detect if it's a directory or not

								    strcpy (user_input_arg1_string, dir_ent.name);

								    if 	(dir_ent.type==1 || dir_ent.type==2) { // 1 is CBM, 2 is DIR
				        				// printf("Skipping: %s\n", dir_ent.name); // Let's not do this and make it match the del command. This saves 34 bytes.

				        			} else {
							        	if (use_dcopy == TRUE) {
										    dcopy(); // DCOPY COMMAND - Between Devices Copying - ChiCLI manually shovels bytes around.
									    } else {
										    acopy(); // ACOPY COMMAND - Drive Within Drive Copying - Drive and DOS handles the copying.
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
	        		display_error_arguments(); // printf("Er arg:%i\n", number_of_user_inputs);
	        	//end default

		    };//end switch

            goto end_copying;

			COPY_UNSUPPORTED : ;
			printf("Drive can't copy.\n");

            end_copying : ;

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

            // ********** END OF COPY ********** //


		// /**********************************************************************************/
		// TYPE COMMAND
		// ********************************************************************************
		} else if ( matching("type",user_input_command_string) ){ // (user_input_command_string[0]=='c' && user_input_command_string[1]=='a' && user_input_command_string[2]=='t') ){ //matching("cat",user_input_command_string) ) 

			switch (number_of_user_inputs) {

				case 2 :
					detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // detect filetype
					switch(detected_filetype){
						case 1 : // case  2 : printf("DIR"); break;	// CBM
						case 2 : // case  2 : printf("DIR"); break;	// DIR
							printf("Not file.\n");
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
							goto TYPE_ERROR;
						break;
					};//end switch
				break;

				case 3 :
					// detected_filetype = detect_filetype(user_input_arg1_string, TRUE); // Detect filetype
					// SAVED 15 BYTES!!!

					// if (matching("-hex",user_input_arg2_string)) {
					// 	type_hex(user_input_arg1_string); 
					// } else if (matching("-text",user_input_arg2_string)) {
					// 	type_text(user_input_arg1_string);
					// } else if (matching("-prg",user_input_arg2_string)) {
					// 	type_prg(user_input_arg1_string);
					// } else {
					// 	printf("Er arg3:%s\n", user_input_arg2_string);
					// };//end if
					// SAVED 81 BYTES!!!

					if (user_input_arg2_string[0]!='-') goto TYPE_ERROR;

					if        (user_input_arg2_string[1]=='h') { type_hex(  user_input_arg1_string );
					} else if (user_input_arg2_string[1]=='t') { type_text( user_input_arg1_string );
					} else if (user_input_arg2_string[1]=='p') { type_prg(  user_input_arg1_string );
					} else {
						goto TYPE_ERROR;
					};//end if 
				break;

			    default :
			    	goto TYPE_ERROR;
			    break;

			};//end switch

			goto TYPE_END;
			TYPE_ERROR : ;
			display_error();
			TYPE_END : ;

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		// /**********************************************************************************/
		// FILEDATE COMMAND
		// ********************************************************************************
		} else if ( (matching("filedate",user_input_command_string)) ) {

			// TODO: I want to re-create this as part of a fileinfo command, that spits out a whole list of info about a file.

			unsigned char one_char_string[2] = " ";
			unsigned char date_index;
			unsigned char little_disk_buffer[16];
			unsigned char file_date_time[32];

			// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

			if (number_of_user_inputs != 2) {
				printf("File?");
				goto END_FILEDATE;
			} else if ( get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD ) {
				// Do nothing, this is fine.
			} else {
				display_error_rtc(); // printf("No RTC.");
				goto END_FILEDATE;
			};//end-if

			strcpy(drive_command_string,"$=t0:"); // NOT WORTH CHANGING --> This only works because partition zero (0) is whatever the currently set partition is. I should probably make this take the specifically set partition instead.

			strcat(drive_command_string,user_input_arg1_string); // This lists the partitions.
			strcat(drive_command_string,"=l"); // This lists the partitions.

			memset(disk_buffer ,0,sizeof(disk_buffer ));

			// result = cbm_open(8, dev, CBM_READ, drive_command_string);
			result = open_file_safely(8, dev, CBM_READ, drive_command_string);

			while(1) {

				// read_bytes = cbm_read(8, little_disk_buffer, sizeof(little_disk_buffer));
				read_bytes = read_file_safely(8, little_disk_buffer, sizeof(little_disk_buffer));

				if ( read_bytes == 0 ) break;

				i=0;
				while(1){
				// for (i = 0 ; i < read_bytes ; i++) {

						if ( (little_disk_buffer[i] >= 48 && little_disk_buffer[i] <= 57) ||
						      isalpha(little_disk_buffer[i])==TRUE 						  ||
							 		 little_disk_buffer[i]  == '/' 						  ||
							         little_disk_buffer[i]  == '.' 						  ){
							one_char_string[0] = little_disk_buffer[i];
							one_char_string[1] = '\0';
							strcat(disk_buffer , one_char_string);
						} else {
							one_char_string[0] = ' ';
							one_char_string[1] = '\0';
							strcat(disk_buffer , one_char_string);
						};//end-if

					if(i==read_bytes-1)break;++i;
				};//end-while
				// };//end for

			};//end-while
			// } while( read_bytes == sizeof(little_disk_buffer) ); //end loop

			// cbm_close (8);

			close_file_safely(8);

			date_index = strlen(disk_buffer) - 66; // When the length is 121 then the starsting psotion is 59 // 121-59 = 62

			// Start at our date_index, and if we find a blank space, update the date_index
			// until we finally hit a printable character.
			while (disk_buffer[date_index] == ' ') {

				date_index++;
			};//end-while

			strncpy(file_date_time, disk_buffer +date_index, 20); // Create a date string

			remove_extra_spaces(file_date_time); // Remove any extra spaces in the middle

			file_date_time[11] = ':';

			printf("-> %s", file_date_time); // print the time stamp

			END_FILEDATE :
			printf("\n");

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			// clear_out(multi_buffer);


		// /**********************************************************************************/
		// CHIRP COMMAND
		// ********************************************************************************
		} else if ( matching("chirp",user_input_command_string) ) {

			pet_chirp(); // this whole section takes like 30 bytes


		// /**********************************************************************************/
		// SYSINFO COMMAND
		// ********************************************************************************
		} else if ( matching("sysinfo",user_input_command_string) ) {

			display_title_screen(have_device_numbers_changed); // Show logo, hardware, & play chirp. SuperCPU dealt with inside the sysinfo() function.


		// /**********************************************************************************/
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
					display_error();
			    break;//end default
			};//end switch


		// /**********************************************************************************/
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
					display_error();
			    break;//end-default

			};//end-switch


		// /**********************************************************************************/
		// SCREENSAVER COMMAND
		// ********************************************************************************
		} else if ( (user_input_command_string[0]=='s' && user_input_command_string[1]=='s') ){ // matching("screensaver",user_input_command_string) ||

			// TODO: Update the HELP files to reflect that enable and disable is now just letters.

			switch (number_of_user_inputs) {

			    case 1 :
					screensaver();
				break;

				case 2 :
					// if (        matching("-et",user_input_arg1_string) ) {
					// 	screensaver_show_time = TRUE;

					// } else if ( matching("-ed",user_input_arg1_string) ) {
					// 	screensaver_show_date = TRUE;

					// } else if ( matching("-dt",user_input_arg1_string) ) {
					// 	screensaver_show_time = FALSE;

					// } else if ( matching("-dd",user_input_arg1_string) ) {
					// 	screensaver_show_date = FALSE;

					// } else if ( matching("-temp",user_input_arg1_string) ) {
					// 	screensaver_show_date = FALSE;

					// };//end-if
					if (        user_input_arg1_string[1]=='e' && user_input_arg1_string[2]=='t' ) { // et - enable time
						screensaver_show_time = TRUE;

					} else if ( user_input_arg1_string[1]=='e' && user_input_arg1_string[2]=='d' ) { // ed - enable date
						screensaver_show_date = TRUE;

					} else if ( user_input_arg1_string[1]=='d' && user_input_arg1_string[2]=='t' ) { // dt - disable time
						screensaver_show_time = FALSE;

					} else if ( user_input_arg1_string[1]=='d' && user_input_arg1_string[2]=='d' ) { // dd - disable date
						screensaver_show_date = FALSE;

					} else if ( user_input_arg1_string[1]=='t' && user_input_arg1_string[2]=='e' ) { // te - temp enable
						screensaver_show_temp = TRUE;

					} else if ( user_input_arg1_string[1]=='t' && user_input_arg1_string[2]=='d' ) { // td - temp disable
						screensaver_show_temp = FALSE;

					};//end-if
					printf("Done!\n");
			    break;

			    default:
					display_error_arguments();
				break;
			};//end_switch


		// /**********************************************************************************/
		// DATETIME COMMAND
		// ********************************************************************************
		} else if ( matching("datetime",user_input_command_string) ){

			// SuperCPU dealt with inside the find_rtc_device() & datetime_helper() functions.

			find_rtc_device();
			switch (rtc_device) {
				case 0 : goto DT_NO_RTC_FOUND; break;
			};//end-switch

			switch (number_of_user_inputs) { // added 63 bytes! but saved 103!
				case 2 :
					if (user_input_arg1_string[1]=='d') {
						display_date_nice();
					} else if (user_input_arg1_string[1]=='t') {
						display_time_nice();
					};//end-if
					goto END_DATETIME;
				break;

				case 3 :
						// Varible reused to build string for setting the date.
						strcpy(drive_command_string,"t-wa"); 										// Start of date time set DOS command.

						//if ( matching(user_input_arg1_string,"-set") ) { 						// If arg 1 is "-set".
						if (user_input_arg1_string[1]=='s'){									// If arg 1 is "-set".
							strcat(drive_command_string,user_input_arg2_string); 					// Then arg 2 much be the date adn time to set this thing to.
						} else { 																// Otherwise they fucked up.
							goto DT_NO_RTC_FOUND;
						};//end-if

						// printf("Format: \"sat. 11/13/21 10:57:00 pm\"\n");
						if (they_are_sure() == FALSE) goto END_DATETIME;

						// result = cbm_open(15, rtc_device, 15, drive_command_string); 				// Example date and time string: "t-wasat. 11/13/21 10:57:00 pm"
						// cbm_close (15);
						execute_dos_command(drive_command_string);

						goto END_DATETIME;
			    break;

			    default:
						display_date_nice();
						display_time_nice();
						goto END_DATETIME;
				break;
			};//end_switch

			DT_NO_RTC_FOUND :
			display_error_rtc();
			END_DATETIME : ;
			printf("\n");


		// /**********************************************************************************/
		// // TIME COMMAND
		// // ********************************************************************************
		// } else if ( matching("time",user_input_command_string) ) {

		// 	// SuperCPU dealt with inside the find_rtc_device() & datetime_helper() functions.

		// 	find_rtc_device();
		// 	switch (rtc_device) {
		// 		case 0  : printf("No RTC.\n");   		break;
		// 		default : display_time_nice();printf("\n"); break;
		// 	};//end-switch


		// /**********************************************************************************/
		// // DATE COMMAND
		// // ********************************************************************************
		// } else if ( matching("date",user_input_command_string) ) {

		// 	// SuperCPU dealt with inside the find_rtc_device() & datetime_helper() functions.

		// 	find_rtc_device();
		// 	switch (rtc_device) {
		// 		case 0  : printf("No RTC.\n");			break;
		// 		default : display_date_nice();printf("\n"); break;
		// 	};//end-switch


		// /**********************************************************************************/
		// STOPWATCH COMMAND - 254 bytes cost
		// ********************************************************************************
		} else if (  matching("stopwatch",user_input_command_string)                          ||
		             (user_input_command_string[0]=='s' && user_input_command_string[1]=='w') ){ // matching("stopwatch",user_input_command_string) ||

			// BEFORE: 47654
			// AFTER:  47607
			// SAVED:     47

			// // Jiffy Version - Commodore 64 Specific - Commodore PET has this at a different address
			// unsigned int stopwatch_difference;
			// unsigned int stopwatch_seconds;
			// unsigned int stopwatch_jiffies;

			union {
				unsigned char bcd_seconds;
				struct {
					unsigned int first:  4;
					unsigned int second: 3;
					unsigned int :	   0+8;
				} digit ;//end-struct
			} packed_time_seconds ;//end-union

			union {
				unsigned char bcd_minutes;
				struct {
					unsigned int first:  4;
					unsigned int second: 3;
					unsigned int :     0+8;
				} digit ;//end-struct
			} packed_time_minutes ;//end-union

            if (stopwatch_start_stamp == 0) {

                // stopwatch_start_stamp = (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162)); //PRINT (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162))
                // printf("Stopwatch on.\n");

				POKE(0xDC08,0); /* Start TOD Clock. Deciseconds */
				POKE(0xDC09,0); /* Start TOD Clock. Seconds     */
				POKE(0xDC0A,0); /* Start TOD Clock. Mins        */
				printf("Stopwatch on.\n");
				stopwatch_start_stamp = 1;

            } else {

                // stopwatch_difference = ( PEEK(161)*256 + PEEK(162) ) - stopwatch_start_stamp; // 18 mins max roughly 
                // stopwatch_seconds = stopwatch_difference/60;
                // stopwatch_jiffies = stopwatch_difference-(stopwatch_seconds*60);
                // printf("Time: %u.%u secs.\n", stopwatch_seconds, ((stopwatch_jiffies*166)/100) );

				temp_deciseconds 		= PEEK(0xDC08);
				packed_time_seconds.bcd_seconds = PEEK(0xDC09);
				packed_time_minutes.bcd_minutes = PEEK(0xDC0A);

				printf("Time: %u%u:%u%u.%u \n",	packed_time_minutes.digit.second	,
												packed_time_minutes.digit.first		,
												packed_time_seconds.digit.second	,
												packed_time_seconds.digit.first		,
												temp_deciseconds								);
				stopwatch_start_stamp = 0;

            };//end_if


		// /**********************************************************************************/
		// // VARS COMMAND - Costs 146 bytes!
		// // ********************************************************************************
		// } else if ( matching("vars",user_input_command_string) ) {

		// 	printf("Input:%u\nArgs:%ux3\nHistory:%u\nBuffer:%u\nAlias:%ux%u\nHotkey:%ux%u\n",MAX_LENGTH_COMMAND,MAX_LENGTH_ARGS,MAX_COMMAND_HISTORY-1,MAX_DISK_BUFFER,MAX_ALIAS_LENGTH,MAX_ALIASES,MAX_HOTKEY_LENGTH,MAX_HOTKEYS);


		// /**********************************************************************************/
		// // RELABEL COMMAND
		// // ********************************************************************************
		} else if ( matching("relabel",user_input_command_string) ) {

			// label chicli-disk 15

			switch (number_of_user_inputs) {
				case 3 :
					if (they_are_sure() == TRUE) {
						strcpy(disk_label, user_input_arg1_string);

						while(strlen(disk_label) < 16) {  // This added 18 bytes!
							string_add_character(disk_label, 0xA0);
						};//end-while
						write_disk_header(TRUE); // TRUE means we write the label

						strncpy(disk_id, user_input_arg2_string, 2);
						write_disk_header(FALSE); // FALSE means we write the id

						// printf("Done!\n");
					};//end if
				break;

				default:
					display_error_arguments();
				break;
			};//end_switch

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		// /**********************************************************************************/
		// // CHANGELABEL COMMAND
		// // ********************************************************************************
		// } else if ( matching("changelabel",user_input_command_string) ) {

		// 	// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		// 	// TODO: Needs checking for 1541 / 4040 / SD-2 and length of entered string is not greater than 16 chars

		// 	switch (number_of_user_inputs) {
		// 		case 2 :
		// 			if (they_are_sure() == TRUE) {
		// 				strcpy(disk_label, user_input_arg1_string);

		// 				while(strlen(disk_label) < 16) {  // This added 18 bytes!
		// 					string_add_character(disk_label, 0xA0);
		// 				};//end-while

		// 				write_disk_header(TRUE); // TRUE means we write the label
		// 				// printf("Done!\n");
		// 			};//end if
		// 		break;

		// 		default:
		// 			printf("Er. args!\n");
		// 		break;
		// 	};//end_switch

		// 	// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		// /**********************************************************************************/
		// // CHANGEID COMMAND
		// // ********************************************************************************
		// } else if ( matching("changeid",user_input_command_string) ) {

		// 	// TODO: Needs checking for 1541 / 4040 / SD-2 and length of entered string is not greater than 16 chars

		// 	// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.

		// 	switch (number_of_user_inputs) {
		// 		case 2 :
		// 			if (they_are_sure() == TRUE) {
		// 				strncpy(disk_id, user_input_arg1_string, 2);
		// 				write_disk_header(FALSE); // FALSE means we write the id
		// 			};//end if
		// 	    break;

		// 	    default:
		// 			printf("Er. args!\n");
		// 		break;
		// 	};//end_switch

		// 	// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.


		/**********************************************************************************/
		// SPEED COMMAND - This command costs 431 bytes! Ouch!
		// ********************************************************************************
		} else if ( matching("speed",user_input_command_string) ) {

            printf("CPU Speed: ");                              					// -POSSIBLE RETURNS OF GETCPU()-
            print_speed(TRUE);                                  					// CPU_6502    0 <-- Ultimate 64
            switch (number_of_user_inputs) {                    					// CPU_65C02   1
                case 1 :                                        					// CPU_65816   2 <-- SuperCPU
                    printf("\n");                               					// CPU_4510    3
                break;                                          					// CPU_65SC02  4
                                                                					// CPU_65CE02  5
                default:                                        					// CPU_HUC6280 6
                    result = detect_cpu();                      					// CPU_2A0x    7
                    printf("\nCPU #: %u\nRasters: %u\n", result,raster_scan_line);	// CPU_45GS02  8
                break;                                             					// MOS 8502    9 <-- Commodore 128
            };//end-switch															// MOS 8500   10 <-- Commodore C64
            				                                      					// MOS 6510   11 <-- Commodore 64


		/**********************************************************************************/
		// TEMP COMMAND - 686 bytes for this!
		// ********************************************************************************
		} else if( matching("temp",user_input_command_string) || matching("tmp",user_input_command_string) ){

			print_temp();

			// END_TEMP: ;
			switch (number_of_user_inputs) {
				case 1 :
					printf("\n");
				break;

				default:
					printf(" Cycles:%u Secs:%u.%u \n", temp_cycles_counter, temp_seconds, temp_deciseconds);
				break; // FOR DEBUGGING
			};//end-switch

			// goto END_TEMP_CPU_NOT_SUPPORTED;

			// TEMP_CPU_NOT_SUPPORTED:;
			// printf("Model not supported.\n");
			// END_TEMP_CPU_NOT_SUPPORTED:;


		/**********************************************************************************/
		// HELP COMMAND
		// ********************************************************************************
          } else if ( (user_input_command_string[0]=='?') ||
                      (user_input_command_string[0]=='h' && user_input_command_string[1]=='e' && user_input_command_string[2]=='l') ||
                      (user_input_command_string[0]=='m' && user_input_command_string[1]=='a' && user_input_command_string[2]=='n') ){

			unsigned char help_entry_detected;
			unsigned char help_entry_found;
			unsigned char current_command_position; // The current char we are checking
			unsigned char converted_character; // We need to convert thigns betwen ascii and petscii

			#define marker_character 95                // Compensated for PETSCII bullshit cc65 automatic fuck FML.

			// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

			help_entry_detected = 0;
			help_entry_found = 0;
			current_command_position = 0;

			if ( user_input_arg1_string[0]=='?' ) { // ? --> help
			     strcpy(user_input_arg1_string,"help");
			} else if ( user_input_arg1_string[0]=='/' ) { // / --> commands
			     strcpy(user_input_arg1_string,"commands");
			};//end-if

			// chicli-help-h.seq - Hardware Help.
			// chicli-help-1.seq - Command Help for a through d.
			// chicli-help-2.seq - Command Help for e through p.
			// chicli-help-3.seq - Command Help for r through z.
			// chicli-help-0.seq - Command Help for all other commands.

			strcpy(drive_command_string,"  chicli-help--"); // Template

			// Adding this cost 67 bytes!
			if ( matching("-h",user_input_arg2_string) ) {
				drive_command_string[14]='h';
			} else if ( user_input_arg1_string[0] >= 'a' && user_input_arg1_string[0] <= 'd' ) {
				drive_command_string[14]='1';
			} else if ( user_input_arg1_string[0] >= 'e' && user_input_arg1_string[0] <= 'p' ) {
				drive_command_string[14]='2';
			} else if ( user_input_arg1_string[0] >= 'r' && user_input_arg1_string[0] <= 'z' ) {
				drive_command_string[14]='3';
			} else {
				drive_command_string[14]='0';
			};//end-if

			switch (number_of_user_inputs) {
				case 1 :
					strcpy(user_input_arg1_string,"help"); // If they entered nothing, load help for help.
					drive_command_string[14]='2'; // Help for help needs this file.
					// Then fall through to the next case...

				case 2 : // This should be case 2 when we are actually using good shit below.
				case 3 : // This is because the code for the hardware lookup is exactly the same.

					printf("Finding...\n\n");

					// user_input_arg1_string = toupper(user_input_arg1_string); //  disk_buffer[0] = toupper(disk_buffer[0]);

					// - Have a single file called chicli-help.seq OR chicli-hardware.seq
					memset(disk_buffer,0,sizeof(disk_buffer));
					// strcpy (drive_command_string,"0:chicli-help,r,s");

					// printf("drive_command_string+2:%s\n",drive_command_string+2);

					// Check if the required help file exists.
					if (detect_filetype(drive_command_string+2,FALSE)==16) {
						// It's good, do nothing and keep going.
					} else {
						goto END_HELP; // If it doesn't exist, exit now.
					};//end-if

					drive_command_string[0] = convert_partition_for_drive();
					drive_command_string[1] = ':';
					strcat(drive_command_string,",r,s"); // It must be a SEQ file. If it is, this adds the right stuff to the command string.

					// printf("drive_command_string:%s\n",drive_command_string);

					// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
					// result = cbm_open(8, dev, CBM_READ, drive_command_string);
					// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.
					result = open_file_safely(8, dev, CBM_READ, drive_command_string);

					// - Each entry starts with _commandname
                    do {

                        // supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
                        // read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
                        // supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.
						read_bytes = read_file_safely(8, disk_buffer, sizeof(disk_buffer));

                        // loop over buffer and barf out bytes to the screen 

						// for (i = 0 ; i <= read_bytes-1 ; i++) { // <-- THIS FOR DOESN'T WORK! It rolls over before it can hit the exit condition.
                        i=0;
                        while(1){

                            // Convert for matching because ascii to petscci bullshit...
                            if ( isalpha(user_input_arg1_string[current_command_position]) ) {
                                converted_character = toupper(user_input_arg1_string[current_command_position])-96;
                            } else {
                                converted_character = user_input_arg1_string[current_command_position];
                            };//end-if

                            // - Searching for marker and we are displaying because we matched an entry
                            if ( (disk_buffer[i] != marker_character) && (help_entry_found==TRUE) ) {
                                printf("%c", disk_buffer[i]); // Output the character because we've matched the command.

                            // - Quiting because we've found and matched an entry and we found a marker
                            } else if ( (disk_buffer[i] == marker_character) && (help_entry_found==TRUE) ) {
                                goto END_HELP;

                            // - Searching for marker and haven't found an entry
                            } else if ( (disk_buffer[i] != marker_character) && (help_entry_detected==FALSE) ) {
                                // Do nothing and keep searching.

                            // - We found an entry
                            } else if ( (disk_buffer[i] == marker_character) && (help_entry_detected==FALSE) ) {
                                help_entry_detected = TRUE; // We found an entry marker. Don't display anything yet.

                            // - Searching to match because we found an entry
                            } else if ( (disk_buffer[i] != marker_character) && (help_entry_detected==TRUE) ) {

                                // If we've matched all the characters, the index will have been incremented past the last position, and therefore we know we have found the string.
                                if ( current_command_position == strlen(user_input_arg1_string) ) {
                                     help_entry_found         = TRUE;   // We've found it, so make help_entry_found = TRUE.
                                     help_entry_detected      = FALSE;  // Also make help_entry_detected = FALSE, because we don't want to keeping trying to compare the current char to the commnad we're trying to find.
                                     current_command_position = 0;      // Reset this for the next time we search.

                                // If the current buffer character matches the index in the command we are trying to find...
                                // } else if ( disk_buffer[i] == converted_character + 32 ) {
                                } else if ( disk_buffer[i] == converted_character ) {
                                     current_command_position++;        // We matched a character, so increment and keep searching.

                                // If it doesn't match, stop trying to match...
                                // } else if ( disk_buffer[i] != converted_character + 32 ) {
                                } else if ( disk_buffer[i] != converted_character ) {
                                     help_entry_detected = FALSE;  // The characters don't match, so stop searching, and go back to looking for the next marker.
                                     current_command_position = 0;      // Reset this for the next time we search.

                                };//end-if

                            // - If we found another entry marker before we've matched the command.
                            } else if ( (disk_buffer[i] == marker_character) && (help_entry_detected==TRUE) ) {
                                help_entry_detected = FALSE; // We found an entry marker but didn't match the command. Go back to searching for another marker.

                            };//end-if

							if(i==read_bytes-1)break;++i;
                        };//end-while

                      get_key = 0;
                      if (kbhit() != 0) { /* If key has been pressed */
                           get_key = cgetc();
                      };/*end_if*/

                      if (get_key == 3) { /* RUN/STOP or CTRL-C */
                           goto END_HELP;
                      };/*end_if*/

                    } while( read_bytes == sizeof(disk_buffer) ); //end loop

				break;

				default:
					display_error_arguments();
				break;

			};//end_switch

			END_HELP :

			// supercpu_disable(); // Turn off SuperCPU accelleration for timing sensitive disk routines.
			// cbm_close(8);
			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.
			close_file_safely(8);

			if (help_entry_found==FALSE) printf("Unfound.\n");

			// supercpu_enable(); // Turn SuperCPU accelleration back on since we are done timing sensitive disk routines.

			// clear_out(multi_buffer);


		/**********************************************************************************/
		// SUPERCPU COMMAND
		// ********************************************************************************
		} else if ( matching("supercpu",user_input_command_string) ) {

			if ( 		 user_input_arg1_string[1]=='t' ){ // matching("-t", user_input_arg1_string
				supercpu_turbo_status = TRUE;
				supercpu_enable();
				printf("Turbo on.\n");
			} else 	if ( user_input_arg1_string[1]=='n' ){ // matching("-n", user_input_arg1_string) ) {
				supercpu_turbo_status = FALSE;
				supercpu_disable();
				printf("Normal on.\n");
			} else {
				display_error_arguments();
				goto END_SUPERCPU;
			};//end if

			END_SUPERCPU: ;


		// /**********************************************************************************/
		// // EGG COMMAND - This takes up 64 bytes!
		// // ********************************************************************************
		// } else if ( matching("egg",user_input_command_string) ) {

		// 	printf("IT IS YOUR EASTER EGG.\n");


		// /**********************************************************************************/
		// ERROR EMPTY COMMAND
		// ********************************************************************************
		} else if ( strlen(entered_keystrokes) == 0 ) {

		    // Do nothing.


		// /**********************************************************************************/
		// ERROR NO COMMAND FOUND
		// ********************************************************************************
		} else {

			// TODO: FIX THIS! 
			// HOLY FUCK this is a hack. I don't know why, but in
			// File: commands.h
			// Line: 17
			// #define process _command()
			// If a command is entered with nothing but spaces, it gets screwed up.
			// Specically, the string contains the first 4 chars in memory starting 
			// at memory lcoation 0x0000.
			// My guess is that when process _command() does it's things, a string
			// is de-referenced or a null string is copied... I dunno some bullshit.
			// Point is: when a string contains /6.0
			// (but dot is actually the petscii of CMD+C which is the upper left square)
			// it means it's null or fucked 
			// or poiting to where it shjouldn't which is a cc65 thing.
			// This was also affecting RUN when you had no argument,
			// it contains the same string /6.0
			// but after teh REM part, which means the same bug or whatever is
			// screwing up the argument processing as well.

			// if ( strlen(entered_keystrokes) != strlen(user_input_command_string) ){
			// 	printf("???\n", user_input_command_string);
			// } else {
			// 	printf("%s ?\n", user_input_command_string);
			// };//end-if

			printf(" ?\n");

			// printf("Unknown command.\n");

		// ********************************************************************************
		// FINISHED COMMAND PROCESSING AND EXECUTING
		// ********************************************************************************
		};//end if

		goto END_OF_MAIN_LOOP;

		ERROR_ARGS:;
		display_error_arguments();

		END_OF_MAIN_LOOP:;

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
