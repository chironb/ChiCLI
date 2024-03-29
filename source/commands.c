// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// commands.c
// ********************************************************************************

#include <string.h>
#include <cbm.h>
#include <conio.h>
#include <stdio.h>
#include <unistd.h>

#ifndef CHICLI_H
	#define CHICLI_H
	#include "chicli.h"
#endif

#ifndef COMMANDS_H
	#define COMMANDS_H
	#include "hardware.h"
#endif

// I did this!
// This could be optimized so that there's a smaller than 7 char string array per entry,
// and then a small list of exceptions, so that the overall array can take up less memory.
// For example: only "restore" is length 7.
// 76 * 7 = 532
// 76 * 6 = 456 (Saves 76 bytes, but adds one if else statement and printf statement, total actual savings: 54 bytes!)
// Trying to save more would create more code because we'd have to deal with multiple cases.
// Since only one string is 7 chars long, this works out nicely!
// 0 = 128 to 75 = 203
// TODO: There must be a way to do this where each string has a variable length.
// Probably pointers to strings. Might not save that much?
// 76 * 2 = 152 for the pointers + like half the text maybe so 266 = 418? Not that much better then what I've done I think.
// But maybe I should test it anyway!
// I made a mistake. This should have been 1 character more than the longest string, because you need
// the string terminator \0 or you get that weird double printing bug.


// Converting to an array of pointers to string!
// BEFORE: 47886
// AFTER:  47792
// SAVED:  94! + 17 for RESTORE which was handled separately. TOTAL: 111!
unsigned char *basic_tokens[76] = {
	"end"    ,
	"for"    ,
	"next"   ,
	"data"   ,
	"input#" ,
	"input"  ,
	"dim"    ,
	"read"   ,
	"let"    ,
	"goto"   ,
	"run"    ,
	"if"     ,
	"restore",
	"gosub"  ,
	"return" ,
	"rem"    ,
	"stop"   ,
	"on"     ,
	"wait"   ,
	"load"   ,
	"save"   ,
	"verify" ,
	"def"    ,
	"poke"   ,
	"print#" ,
	"print"  ,
	"cont"   ,
	"list"   ,
	"clr"    ,
	"cmd"    ,
	"sys"    ,
	"open"   ,
	"close"  ,
	"get"    ,
	"new"    ,
	"tab("   ,
	"to"     ,
	"fn"     ,
	"spc("   ,
	"then"   ,
	"not"    ,
	"step"   ,
	"+"      ,
	"-"      ,
	"*"      ,
	"/"      ,
	"^"      ,
	"and"    ,
	"or"     ,
	">"      ,
	"="      ,
	"<"      ,
	"sgn"    ,
	"int"    ,
	"abs"    ,
	"usr"    ,
	"fre"    ,
	"pos"    ,
	"sqr"    ,
	"rnd"    ,
	"log"    ,
	"exp"    ,
	"cos"    ,
	"sin"    ,
	"tan"    ,
	"atn"    ,
	"peek"   ,
	"len"    ,
	"str$"   ,
	"val"    ,
	"asc"    ,
	"chr$"   ,
	"left$"  ,
	"right$" ,
	"mid$"   ,
	"go"
};//end-array


unsigned char convert_partition_for_drive();

extern unsigned char listing_string[10];

extern unsigned char drive_detected[8];
extern unsigned char drive_detected_type[8]; // 0 = no drive detected, all others related to a specific drive

extern void string_add_character();
extern unsigned char par;
extern struct cbm_dirent dir_ent;

extern unsigned char number_of_user_inputs;
extern unsigned char source_par;
extern unsigned char user_input_arg1_string[MAX_LENGTH_ARGS];
extern unsigned char user_input_arg2_string[MAX_LENGTH_ARGS];
extern unsigned int user_input_arg2_number;
extern unsigned char target_par;

extern unsigned char disk_buffer[MAX_DISK_BUFFER] ; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...

extern unsigned char drive_command_string  [MAX_LENGTH_COMMAND]	    ;
// extern unsigned char drive_command_string2 [MAX_LENGTH_COMMAND]	    ;

extern unsigned char get_key ;
extern unsigned char dev ;
extern signed int result    ; // This needs to be signed, because cbm_read and  cbm_write return -1 in case of an error;
extern signed int result2   ;
extern signed int read_bytes;
extern unsigned char i; // shared by all for loops
extern unsigned char spot_in_prg ;
extern unsigned char previous_byte ;
extern unsigned char detected_filetype ;
extern unsigned char detected_filetype_char[2];

extern unsigned char hex_display_position_x ; 
extern unsigned char hex_display_position_y ;
extern unsigned int  hex_display_position_in_file ; // check how logn long int is 

extern unsigned char dir_file_total;
extern unsigned char current_dir_file_index;
extern unsigned char iii;
extern unsigned char jjj;


// ********************************************************************************
// COMMANDS FUNCTIONS
// ********************************************************************************

unsigned char dir_file_count(unsigned char * listing_string){

	unsigned char total_files = 0;

 	// result = cbm_opendir(1, dev, listing_string);
 	result = open_dir_safely(1, dev, listing_string);

	for (iii = 0; iii <= 254 ; iii++) {

	    // result = cbm_readdir(1, &dir_ent);
	    result = read_dir_safely(1, &dir_ent);

		if (result != 0) {
			break;
		};/*end if*/

	    if (iii == 0) {
			/* Start */
	    } else if (result == 2) {
			/* End */
	    } else {
			total_files++;
		};/*end if*/

	};/*end for*/

	// cbm_closedir(1);
	close_dir_safely(1);

	return(total_files);

};//end-func


void dir_goto_file_index(unsigned char file_index) 	{

	strcpy(listing_string,"$"); // The default is $ to load the current directory.

	string_add_character(listing_string,convert_partition_for_drive());

 	// result = cbm_opendir(1, dev, listing_string);
 	result = open_dir_safely(1, dev, listing_string);

	for (jjj = 0; jjj <= file_index ; jjj++) {

	    // result = cbm_readdir(1, &dir_ent);
	    result = read_dir_safely(1, &dir_ent);

	};//end-for

	// cbm_closedir(1);
	close_dir_safely(1);

};//end macro func 


// TODO: We should keep a byte variable that is set to 0 at the start of copying,
// TODO: and if there's an error, we set the byte to 1,
// TODO: and then we can display and copying error message at the end of copying.


// ********************************************************************************
// DCOPY COMMAND - Between Devices Copying - ChiCLI manually shovels bytes around.
// ********************************************************************************
void dcopy() {

	// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

	switch (detected_filetype) {
		case  1 : /* bad */      break;	// CBM
		case  2 : /* bad */      break;	// DIR
		case 16 : detected_filetype_char[0]='s';detected_filetype_char[1]='\0'; break; /* SEQ */
		case 17 : detected_filetype_char[0]='p';detected_filetype_char[1]='\0'; break; /* PRG */
		case 18 : detected_filetype_char[0]='u';detected_filetype_char[1]='\0'; break; /* USR */
		case 19 : detected_filetype_char[0]='l';detected_filetype_char[1]='\0'; break; /* REL */
		default : printf("?"); /* end default */
	};/* end switch */

	switch (number_of_user_inputs) {
		case 3 :
			/* Setup SOURCE FILE for READING */
			strcpy (drive_command_string , "");
			strncat(drive_command_string ,&source_par,1);              /* this is the source partition */
			strcat (drive_command_string , ":");
			strcat (drive_command_string , user_input_arg1_string); /*filename*/
			strcat (drive_command_string , ",r,");
			strcat (drive_command_string , detected_filetype_char);

			// result = cbm_open(6, dev, CBM_READ, drive_command_string );
			result = open_file_safely(6, dev, CBM_READ, drive_command_string );

			/* Setup TARGET FILE for WRITING */
			strcpy (drive_command_string, "");
			strncat(drive_command_string,&target_par,1);              /* this is the target partition */
			strcat (drive_command_string, ":");
			strcat (drive_command_string, user_input_arg1_string); /*filename*/
			strcat (drive_command_string, ",w,");
			strcat (drive_command_string, detected_filetype_char);

			// result2 = cbm_open(7, user_input_arg2_number, CBM_WRITE, drive_command_string);
			result2 = open_file_safely(7, user_input_arg2_number, CBM_WRITE, drive_command_string);

			printf("-> %s [",user_input_arg1_string);

			memset(disk_buffer,0,sizeof(disk_buffer));

			if (result == 0 && result2 == 0) {
				// do {
				while(1){

					// read_bytes = cbm_read(6, disk_buffer, sizeof(disk_buffer));
					read_bytes = read_file_safely(6, disk_buffer, sizeof(disk_buffer));

					if(read_bytes==0)break;

					// result = cbm_write(7, disk_buffer, read_bytes);
					result = write_file_safely(7, disk_buffer, read_bytes);

					if ( (read_bytes == -1) || (result == -1) ) {
						printf("Device write error!");
						break;
					};/*end_if*/

					printf(".");

                    get_key = 0;
					if (kbhit() != 0) { /* If key has been pressed */
					    get_key = cgetc();
				    };/*end_if*/

					if (get_key == 3) { /* RUN/STOP or CTRL-C */
					    break;
				    };/*end_if*/

					memset(disk_buffer,0,sizeof(disk_buffer));

				};//end-while
				// } while( read_bytes == sizeof(disk_buffer) ); // TODO: WHY THE FUCK DOES THIS WORK HERE, BUT DIDN'T WORK FOR THE TYPE FUNCTIONS???
			};/* end if */

			// cbm_close (6);
			// cbm_close (7);
			close_file_safely(6);
			close_file_safely(7);

			printf("]\n");
	    break;

	    default :
	    	printf("Er args:%i\n", number_of_user_inputs);
	    break;/* end default */
	};/* end switch */

	// clear_out(multi_buffer);

};//end-func


// ********************************************************************************
// ACOPY COMMAND - Drive Within Drive Copying - Drive and DOS handles the copying.
// ********************************************************************************
void acopy() {

	unsigned char user_input_arg1_string_length;
	unsigned char user_input_arg2_string_length;
	unsigned char right_slash_index;
	unsigned char right_slash_position;

	unsigned char  source_path[MAX_COMMODORE_DOS_FILENAME];
	unsigned char  source_path_length;
	unsigned char *source_filename_pointer;
	unsigned char  source_filename_length;

	unsigned char  target_path[MAX_COMMODORE_DOS_FILENAME];
	unsigned char  target_path_length;
	unsigned char *target_filename_pointer;
	unsigned char  target_filename_length;

	unsigned char  copy_error_status;

	memset( source_path, 0, sizeof(source_path) );
	memset( target_path, 0, sizeof(target_path) );

	user_input_arg1_string_length = 0;
	user_input_arg2_string_length = 0;

	right_slash_index = 0;

	user_input_arg1_string_length = strlen(user_input_arg1_string);
	user_input_arg2_string_length = strlen(user_input_arg2_string);

	source_filename_pointer = user_input_arg1_string; /* update pointer --> user_input_arg1_string_length string */

    if ( (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') || (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') ) {
	    target_filename_pointer = user_input_arg1_string; /* update pointer --> user_input_arg1_string_length string */
	} else {
	    target_filename_pointer = user_input_arg2_string;
	};/*end_if*/

	source_filename_length = 0;
	target_filename_length = 0;

	copy_error_status = FALSE;

	/* -------------------- SOURCE -------------------- */ 

	right_slash_index = 0; 

	for (i = (user_input_arg1_string_length) ; i > 0 ; i--) {
		if (user_input_arg1_string[i-1] == '/') {
			break; 
		} else { 
			right_slash_index++; /* how many positons left does the rightmost slash / occur */
		};/*end_if*/
	};/*end_if*/

	right_slash_position = (user_input_arg1_string_length - 1) - right_slash_index;	/* last positoin minus 3 of chars from end where we found the right-most slash */

	if (right_slash_index == user_input_arg1_string_length) {/* if we searched the whole string and found no / slashes */
		/* leave source filename_pointer as-is, leave source path as-is */

	} else if (right_slash_position == 0) { /* if right_slash_position == 0 then there's only a slash at the very beginning */
		source_filename_pointer = source_filename_pointer + 1;

	} else if (right_slash_index == 0) { /* if there's a slash at the very end */
		copy_error_status = TRUE; /* ERROR! in this case, there can't be only a path as the source, cause that's not a file */

	} else {
		source_filename_pointer = source_filename_pointer + (right_slash_position+1);

	};/*end_if*/

	source_filename_length = strlen(source_filename_pointer);

	source_path_length = user_input_arg1_string_length - source_filename_length;
	source_filename_length = strlen(source_filename_pointer);
	strncpy(source_path , user_input_arg1_string , source_path_length ); /* copy the arg2 into the path leaving out the last filename characters */

	/* -------------------- TARGET -------------------- */

	right_slash_index = 0;

	for (i = (user_input_arg2_string_length) ; i > 0 ; i--) {
		if (user_input_arg2_string[i-1] == '/') {
			break;
		} else {
			right_slash_index++; /* how many positons left does the rightmost slash / occur */
		};/*end_if*/
	};/*end_for*/

	right_slash_position = (user_input_arg2_string_length - 1) - right_slash_index;

	if (right_slash_index == user_input_arg2_string_length) { /* if we searched the whole string and found no slashes then... */
		/* use this string as the filename , the path is left empty */

	} else if (right_slash_position == 0) { /* if there's only a slash at the very beginning */
		target_filename_pointer = target_filename_pointer + 1;

	} else if (right_slash_index == 0) { /* if there's a slash at the very end */
		target_filename_pointer = source_filename_pointer; /* then they haven't given us a filename, and the source filename is implied */
		strcpy(target_path,user_input_arg2_string);

	} else {
		target_filename_pointer = target_filename_pointer + (right_slash_position+1);

	};/*end_if*/

	target_filename_length = strlen(target_filename_pointer);

	target_path_length = user_input_arg2_string_length - target_filename_length;
	target_filename_length = strlen(target_filename_pointer);
	strncpy(target_path , user_input_arg2_string , target_path_length ); /* copy the arg2 into the path leaving out the last filename characters */

    if ( (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') || (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') ) {
	    strcpy(target_path , "" ); /* clear it */
	};/*end_if*/

	/* -------------------- BUILD STRING -------------------- */
	/* --> Copy FILEORIGINAL from Partition 1 to Partition 2 Called FILECOPY */

                                               			// Example: C2/:FILECOPY=1/:FILEORIGINAL
    strcpy (drive_command_string,"c");         			// This is the copy command
    // if (get_drive_type(dev)!=DRIVE_VICEFS) {   			// The VICE FS DRIVER doesn't fucking support *ANY* within device copying! Every God damn thing all the God damn time...
    strncat (drive_command_string,&target_par,1);	// this is the target partition
    // };//end-if

	if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) {
	    strcat (drive_command_string,"/");         		// !!!!! this is the beginning of the target path
	} else {
		// do nothing
	};//end-if

    strcat (drive_command_string,target_path); 			/* this is the target path */
    strcat (drive_command_string,":");         			/* this finishes the target path */
	if (target_filename_length == 0 ) {        			/* add the SOURCE filename because it's inferred */
		strcat (drive_command_string,source_filename_pointer);
	} else {                                   			/* otherwise, add the target filename */
		strcat (drive_command_string,target_filename_pointer);
	};/*end_if*/

	strcat (drive_command_string,"=");                  /* this finished the target path */

	strncat (drive_command_string,&source_par,1);       /* this is the source partition */

	if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD || get_drive_type(dev)==DRIVE_VICEFS) {
	    strcat (drive_command_string,"/");         /* !!!!! this is the beginning of the target path */
	} else {
		// do nothing
	};//end-if

	strcat (drive_command_string,source_path);             /* this is the source path */
	strcat (drive_command_string,":");                     /* this finishes the source path */
	strcat (drive_command_string,source_filename_pointer); /* add the source filename  */

	printf("-->%s\n", drive_command_string);

	if (copy_error_status == TRUE) {
		printf("Error: File not found!\n");
	} else {
		// result = cbm_open(1, dev, 15, drive_command_string);
		// cbm_close(1);
		execute_dos_command(drive_command_string);
	};/*end_if*/

};//end-func


// ********************************************************************************
// TYPE COMMAND - TEXT - Displays a file as if it were a text file.
// ********************************************************************************
void type_text( unsigned char * file_to_type ) {

	// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

	memset(disk_buffer,0,sizeof(disk_buffer)); // I think I need to blank out the disk_buffer right here.

	printf("Detected file: %s ", file_to_type );
	detected_filetype = detect_filetype(file_to_type, TRUE);

	switch (detected_filetype) {
		case  1 : /* bad */      break;	// CBM
		case  2 : /* bad */      break;	// DIR
		case 16 : strcpy(detected_filetype_char,"s"); break; // SEQ
		case 17 : strcpy(detected_filetype_char,"p"); break; // PRG
		case 18 : strcpy(detected_filetype_char,"u"); break; // USR
		case 19 : strcpy(detected_filetype_char,"l"); break; // REL // seems like you need to pass L instaed of R 
		default : printf("???"); //end default
	};//end switch

	/* Setup SOURCE FILE for READING */
	strcpy (drive_command_string, "");

	// Example: device_command[1] = convert_partition_for_drive();
	drive_command_string[0] = convert_partition_for_drive();

	drive_command_string[1] = ':';
	drive_command_string[2] = '\0';
	strcat (drive_command_string, file_to_type); /*filename*/
	strcat (drive_command_string, ",r,");
	strcat (drive_command_string, detected_filetype_char);

	// result = cbm_open(8, dev, CBM_READ, drive_command_string);
	result = open_file_safely(8, dev, CBM_READ, drive_command_string);

	while(1){
	// do {
		// read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
		read_bytes = read_file_safely(8, disk_buffer, sizeof(disk_buffer));

		if(read_bytes==0)break; // If we are finished reading the file, then we need to exit right now. If this isn't here, then the loop below loops forever.

		// loop over buffer and barf out bytes to the screen 
		// for (i = 0 ; i < read_bytes ; i++) {
		i=0;
		while(1){

			if (wherey() == 24) {

				printf("     * Press any key to continue *");
				do {
					//nothing
				} while (kbhit() == 0);//end do 
				get_key = cgetc();
				if (get_key == 3) { // RUN/STOP or CTRL-C
					printf("\n");
					read_bytes = 0;
					goto END_TYPE_TEXT;
				};//end if 

				gotox(0);
				printf("                                       ");
				gotox(0);
				clrscr();

			};//end if

			printf( "%c", disk_buffer[i] ); // this needs to be way better obviously...

			if(i==read_bytes-1)break;++i;
		};//end-while
		// };//end for 

		// printf( "\n2nd read_bytes:%i\n", read_bytes ); // 0 ~ 254 = 255  **0 ~ 255 = 256**  0 ~ 256 = 257 // disk_buffer size = 256 (0-255)

	};//end-while
	// } while( read_bytes != 0 ); //end loop

	// cbm_close (8);
	END_TYPE_TEXT: ;
	close_file_safely(8);

	printf("\n");

	// clear_out(multi_buffer);

};//end func


// ********************************************************************************
// TYPE COMMAND - PROGRAM - Displays a file as if it were a BASIC program file.
// ********************************************************************************
void type_prg( unsigned char * file_to_type ) {

			// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

			// I was helped in this section by studying this code:
			// https://github.com/doj/dracopy/blob/master/src/cat.c
			// As well as this website:
			// https://wpguru.co.uk/2014/06/reading-and-writing-sequential-data-on-the-commodore-1541/
			// for Commodore 64 Basic 2.0 info, I used this:
			// https://www.c64-wiki.com/wiki/BASIC_token

			/* Setup SOURCE FILE for READING */
			strcpy (drive_command_string, "");

			drive_command_string[0] = convert_partition_for_drive();

			drive_command_string[1] = ':';
			drive_command_string[2] = '\0';
			strcat (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,p");

			// Blank out the disk_buffer, so there isn't any garbage characters in it.
			memset(disk_buffer,0,sizeof(disk_buffer));

			// result = cbm_open(8, dev, CBM_READ, drive_command_string);
			result = open_file_safely(8, dev, CBM_READ, drive_command_string);

			spot_in_prg = first_header_byte;

			// do {
			while(1){

				// read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
				read_bytes = read_file_safely(8, disk_buffer, sizeof(disk_buffer));

				if(read_bytes==0)break;

				i=0;
				while(1){
				// for (i = 0 ; i < read_bytes ; i++) { // loop over buffer and barf out bytes to the screen

					if (wherey() == 24) {

						printf("     * Press any key to continue *");
						do {
							//nothing
						} while (kbhit() == 0);//end do 
						get_key = cgetc();
						if (get_key == 3) { // RUN/STOP or CTRL-C
							printf("\n");
							read_bytes = 0;
							goto END_TYPE_PRG;
						};//end if 

						gotox(0);
						printf("                                       "); // TODO: There's gotta be a better way of doing this!
						gotox(0);
						clrscr();

					};//end if

					if (disk_buffer[i] == 0 && previous_byte == 0) {                     // We are at the end of the program 
						printf("\nFooter:0x%04X\n", (disk_buffer[i]*256)+previous_byte); // This prints out the actual footer. But it should always be the same.
						goto type_prg_exit_point;                                               // goto jump here beacuse it's the end of the basic stub (even though there's more file)
					};//end if

					switch ( spot_in_prg ) {

						case first_header_byte :                   	// this is statring address low byte, so we can't output anything until we get teh next byte which is the high byte
							previous_byte = disk_buffer[i]; 	// store in previous character
							spot_in_prg++; 						   	// increment spot_in_prg
						break;

						case second_header_byte : 					// use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							printf("Header:0x%04X\n\n", (disk_buffer[i]*256)+previous_byte); // don't need to store anything in previous byte
							spot_in_prg++; 							// increment spot_in_prg
						break;

						case first_basic_line_address_byte : 		// use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							previous_byte = disk_buffer[i]; 	// store in previous character
							spot_in_prg++; 						   	// increment spot_in_prg
						break;

						case second_basic_line_address_byte :
							//THIS ENABLES SHOWING TEH ADDRESS of each basic line 
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case first_basic_line_number_byte :
							previous_byte = disk_buffer[i]; // store in previous character
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case second_basic_line_number_byte :
							printf("%u ", (disk_buffer[i]*256)+previous_byte); // don't need to store anything in previous byte
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case body_bytes :

							if ( disk_buffer[i] == 0 ) {
								printf("\n"); 
								previous_byte = disk_buffer[i]; 
								spot_in_prg = first_basic_line_address_byte;
							// } else if ( disk_buffer[i] == 140 ) { 	// SAVED:17 bytes here!  // Handled manually, to save the overall size of the array.
							// 	printf("restore");							 // This string is 7 chars, which is the only one. So we make the max length of each string 6 chars instead of 7.
							} else if ( disk_buffer[i] >= 128 && disk_buffer[i] <= 203 ) {
								printf("%s",basic_tokens[ disk_buffer[i]-128 ]);
							} else {
							    printf( "%c", disk_buffer[i] ); // TODO: this needs to be way better obviously...
							};//end-if

							// DO NOT increment spot_in_prg becuase we don't know if we are finished yet

						break;

						default :
							printf("?");
						break;

					};//end switch

					if(i==read_bytes-1)break;++i;
				};//end-while
				// };//end for

			};//end-while
			// } while( read_bytes == sizeof(disk_buffer) ); //end do loop

			//jump to here beacuse it's the end of the basic stub (even though there's more file)
			type_prg_exit_point: ;

			// cbm_close(8);
			END_TYPE_PRG: ;
			close_file_safely(8);

			// clear_out(multi_buffer);

};//end-func


// ********************************************************************************
// TYPE COMMAND - HEXIDECIMAL - Displays a file as series of characters and in hex.
// ********************************************************************************
void type_hex( unsigned char * file_to_type ) {

			// swap_in(DISK_BUFFER, multi_buffer, DISK_BUFFER);

			printf("Detected file: %s ", file_to_type );
			detected_filetype = detect_filetype(file_to_type, TRUE);

			switch (detected_filetype) {
				case  1 : /* bad */      break;	// CBM
				case  2 : /* bad */      break;	// DIR
				case 16 : strcpy(detected_filetype_char,"s"); break; // SEQ
				case 17 : strcpy(detected_filetype_char,"p"); break; // PRG
				case 18 : strcpy(detected_filetype_char,"u"); break; // USR
				case 19 : strcpy(detected_filetype_char,"l"); break; // REL // seems liek you need to pass L instaed of R 
				default : printf("???"); //end default
			};//end switch

			/* Setup SOURCE FILE for READING */
			strcpy (drive_command_string, "");

			drive_command_string[0] = convert_partition_for_drive();

			drive_command_string[1] = ':';
			drive_command_string[2] = '\0';
			strcat (drive_command_string, file_to_type); /*filename*/
			strcat (drive_command_string, ",r,");
			strcat (drive_command_string, detected_filetype_char);

			// sleep(1); // TODO: WHY IS THIS HERE???

			printf("\n");

			// I think I need to blank out the disk_buffer right here.
			memset(disk_buffer,0,sizeof(disk_buffer));

			hex_display_position_in_file = 0 ;
			hex_display_position_x       = 0 ;
			hex_display_position_y       = 0 ;

			// result = cbm_open(8, dev, CBM_READ, drive_command_string);
			result = open_file_safely(8, dev, CBM_READ, drive_command_string);

			clrscr();

			// do {
			while(1){

				// read_bytes = cbm_read(8, disk_buffer, sizeof(disk_buffer));
				read_bytes = read_file_safely(8, disk_buffer, sizeof(disk_buffer));

				if(read_bytes==0)break;

				// for (i = 0 ; i < read_bytes ; i++) { // loop over buffer and barf out bytes to the screen 
				i=0;
				while(1){

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
						printf("  Any key for more or RUN/STOP to quit ");
						do {
							// Do nothing.
						} while (kbhit() == 0);//end do
						get_key = cgetc();
						if (get_key == 3) { // RUN/STOP or CTRL-C
							printf("\n");
							read_bytes = 0;
							goto END_TYPE_HEX;
						};//end if 
						clrscr();
					};//end if 

					// DISPLAY ADDRESS
					if (hex_display_position_x == 0){
						gotox(0);
						gotoy(hex_display_position_y);
						printf("%04X", hex_display_position_in_file);
					};//end if 

					// DISPLAY HEX AND CHAR 
					gotox((hex_display_position_x * 3) + 6);
					printf("%02X", disk_buffer[i]);
					gotox(hex_display_position_x + 31) ;
					cputc(convert_char(disk_buffer[i]));

					// UPDATE COUNTERS
					if (hex_display_position_x == 7 && hex_display_position_y != 23) {
						hex_display_position_x = 0;
						hex_display_position_y++;

					} else {
						hex_display_position_x++;

					};//end if 

					hex_display_position_in_file++;

					if(i==read_bytes-1)break;++i;
				};//end-while
				// };//end for

			};//end-while
			// } while( read_bytes == sizeof(disk_buffer) ); //end do loop

			// cbm_close (8);
			END_TYPE_HEX: ;
			close_file_safely(8);

			printf("\n");

			// clear_out(multi_buffer);

};//end func


void single_char_dos_command(unsigned char single_char) {

	if (they_are_sure() == TRUE) {
		result = get_status(dev, TRUE);

		if (result != 255) {

			printf("Processing... ");

			drive_command_string[0] = single_char;
			drive_command_string[1] = convert_partition_for_drive();
			drive_command_string[2] = '\0';

			execute_dos_command(drive_command_string);

			printf("Done.\n");

		};//endif
	} else {
		printf("Cancelled.\n");
	};//end_if

};//end-func
