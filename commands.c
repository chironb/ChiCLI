// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
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
unsigned char basic_tokens[76][7] = {
	"end"    ,
	"for"    ,
	"next"   ,
	"data"   ,
	"input#" , /* input# bug here? */
	"input"  , /* input bug here? */
	"dim"    ,
	"read"   ,
	"let"    ,
	"goto"   ,
	"run"    ,
	"if"     ,
	""       , /* RESTORE is handled manually to save the length of this array */
	"gosub"  ,
	"return" , /* bug here? */
	"rem"    , /* bug here? */
	"stop"   ,
	"on"     ,
	"wait"   ,
	"load"   ,
	"save"   ,
	"verify" , /* bug here? */
	"def"    , /* bug here? */
	"poke"   ,
	"print#" , /* bug here? */
	"print"  , /* bug here? */
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
	"right$" , /* bug here? */
	"mid$"   , /* bug here? */
	"go"
};//end-array


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

extern unsigned char disk_sector_buffer    [MAX_DISK_SECTOR_BUFFER] ; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...
extern unsigned char drive_command_string  [MAX_LENGTH_COMMAND]	    ;
extern unsigned char drive_command_string2 [MAX_LENGTH_COMMAND]	    ;
extern unsigned char get_key ;
extern unsigned char dev ;
extern signed int result    ; // This needs to be signed, because cbm_read and  cbm_write return -1 in case of an error;
extern signed int result2   ; // This needs to be signed, because cbm_read and  cbm_write return -1 in case of an error;
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



// unsigned char dir_file_total = 0;
// unsigned char current_dir_file_index = 0;

// #define set_drive_detection(drive,detected) drive_detected[drive-8] = detected
// #define get_drive_detection(drive) drive_detected[drive-8]
// #define set_drive_type(drive,type) drive_detected_type[drive-8] = type
// #define get_drive_type(drive) drive_detected_type[drive-8]

// unsigned char  drive_detected[8]      = { 0,0,0,0,0,0,0,0 };
// unsigned char  drive_detected_type[8] = { 0,0,0,0,0,0,0,0 }; // 0 = no drive detected, all others related to a specific drive


// BEFORE: 47895
// AFTER:  47721
// SAVED:    174

unsigned char dir_file_count(){

	// unsigned char dir_file_total = 0;

	unsigned char total_files = 0;

	strcpy(listing_string,"$0"); /* The default is $ to load the current directory.*/

	if (get_drive_type(dev) == DRIVE_UIEC) {
		listing_string[1] = par+1; /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
	} else {
		listing_string[1] = par; /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
	};/*end for*/

 	result = cbm_opendir(1, dev, listing_string);

	for (iii = 0; iii <= 254 ; iii++) {
 		/* printf("iii:%i\n", iii); */
		if (result != 0) {
			/* printf("dir_file_count result != 0 --> %i\n",result); */
			break;
		};/*end if*/

	    result = cbm_readdir(1, &dir_ent);

	    if (iii == 0) {
			/* printf("Start.\n"); */

	    } else if (result == 2) {
			/* printf("End.\n"); */

	    } else {
			total_files++;

		};/*end if*/

	};/*end for*/

	cbm_closedir(1);

	return(total_files);

};//end-func


void dir_goto_file_index(unsigned char file_index) 	{

	// printf("file_index:%u\n",file_index);

	strcpy(listing_string,"$0"); /* The default is $ to load the current directory.*/

	if (get_drive_type(dev) == DRIVE_UIEC) {
		listing_string[1] = par+1; /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
	} else {
		listing_string[1] = par; /* Add the current partition, or drive, for MSD SD-2 and 4040 support.*/
	};/*end for*/

 	result = cbm_opendir(1, dev, listing_string);

	for (jjj = 0; jjj <= file_index ; jjj++) {

	    result = cbm_readdir(1, &dir_ent);
	    // printf("%s\n", dir_ent.name);

	};/*end for*/

	cbm_closedir(1);

};//end macro func 




// Doing this saved: 48005 - 47530 = 475 bytes!
void dcopy() {
	                                                                                                    
	switch (detected_filetype) {                                                                        
		case  2 : /* bad */      break;	/* DIR */                                                       
		case 16 : detected_filetype_char[0]='s';detected_filetype_char[1]='\0'; break; /* SEQ */       
		case 17 : detected_filetype_char[0]='p';detected_filetype_char[1]='\0'; break; /* PRG */                                  
		case 18 : detected_filetype_char[0]='u';detected_filetype_char[1]='\0'; break; /* USR */                                  
		case 19 : detected_filetype_char[0]='l';detected_filetype_char[1]='\0'; break; /* REL */                                  
		default : printf("?"); /* end default */                                                        
	};/* end switch */                                                                                  
                                                                                                        
	switch (number_of_user_inputs) {                                                                    
		case 3 : 				                                                                       
			/* Setup SOURCE FILE for READING */                                                         
			strcpy (drive_command_string, "");                                                          
			strncat(drive_command_string,&source_par,1);              /* this is the source partition */              
			strcat (drive_command_string, ":");                                                       
			strcat (drive_command_string, user_input_arg1_string); /*filename*/                         
			strcat (drive_command_string, ",r,");                                                       
			strcat (drive_command_string, detected_filetype_char); 				                        
                                                                                                        
			/* printf("OPEN 6,%i,%i,%s\n", dev, CBM_READ, drive_command_string); */		                
                                                                                                        
			result = cbm_open(6, dev, CBM_READ, drive_command_string);                                  
                                                                                                        
			/* Setup TARGET FILE for WRITING */                                                         
			strcpy (drive_command_string2, "");                                                         
			strncat(drive_command_string2,&target_par,1);              /* this is the target partition */              
			strcat (drive_command_string2, ":");                                                       
			strcat (drive_command_string2, user_input_arg1_string); /*filename*/                        
			strcat (drive_command_string2, ",w,");                                                      
			strcat (drive_command_string2, detected_filetype_char); 			                        
                                                                                                        
			/* printf("OPEN 7,%i,%i,%s\n", user_input_arg2_number, CBM_WRITE, drive_command_string2); */
                                                                                                        
			result2 = cbm_open(7, user_input_arg2_number, CBM_WRITE, drive_command_string2);            
                                                                                                        
			printf("-> %s [",user_input_arg1_string);                                                   
                                                                                                        
			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));                                    
                                                                                                        
			if (result == 0 && result2 == 0) {                                                          
				do {                                                                                    
					read_bytes = cbm_read(6, disk_sector_buffer, sizeof(disk_sector_buffer));           
					result = cbm_write(7, disk_sector_buffer, read_bytes); 								
					if ( (read_bytes == -1) || (result == -1) ) {										
						printf("dcopy() er!");															
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
                                                                                                        
					memset(disk_sector_buffer,0,sizeof(disk_sector_buffer)); 							
				} while( read_bytes == sizeof(disk_sector_buffer) );/* end do */						
			};/* end if */ 																				
																										
			cbm_close (6);																				
			cbm_close (7);																				
																										
			printf("]\n");																		        
	    break;																							
	    																								
	    default : 																						
	    	printf("Er args:%i\n", number_of_user_inputs);						                        
	    /* end default */																				
	};/* end switch */																					
};//end-func




// Doing this saved: 47570 - 46784 = 786 bytes!

// ********************************************************************************
// ACOPY COMMAND - Drive within Drive Copying - Drive and DOS handles the copying.
// ********************************************************************************
void acopy() {
	                                                                               \
	unsigned char user_input_arg1_string_length; \
	unsigned char user_input_arg2_string_length; \
	unsigned char right_slash_index; \
	unsigned char right_slash_position; \
	                                                                               \
	unsigned char  source_path[MAX_COMMODORE_DOS_FILENAME]; \
	unsigned char  source_path_length; \
	unsigned char *source_filename_pointer; \
	unsigned char  source_filename_length; \
	                                                                               \
	unsigned char  target_path[MAX_COMMODORE_DOS_FILENAME]; \
	unsigned char  target_path_length;			 \
	unsigned char *target_filename_pointer; \
	unsigned char  target_filename_length;	 \
	                                                                               \
	unsigned char  copy_error_status; \
	                                                                               \
	memset( source_path, 0, sizeof(source_path) ); \
	memset( target_path, 0, sizeof(target_path) ); \
	                                                                               \
	user_input_arg1_string_length = 0; \
	user_input_arg2_string_length = 0; \
	                                                                               \
	right_slash_index = 0; \
	                                                                               \
	user_input_arg1_string_length = strlen(user_input_arg1_string); \
	user_input_arg2_string_length = strlen(user_input_arg2_string); \
	                                                                               \
	source_filename_pointer = user_input_arg1_string; /* update pointer --> user_input_arg1_string_length string */ \
	/* NEW!!! */\
    /* if ( matching("d9b:",user_input_arg2_string) ) { */ \
    if ( (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') || (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') ) { \
	    target_filename_pointer = user_input_arg1_string; /* update pointer --> user_input_arg1_string_length string */ \
	    /* puts("???"); */ \
	} else { \
	    target_filename_pointer = user_input_arg2_string; \
	};/*end_if*/\
	                                                                               \
	source_filename_length = 0; \
	target_filename_length = 0; \
	                                                                               \
	copy_error_status = FALSE; \
	                                                                               \
	/* -------------------- SOURCE -------------------- */ \
	                                                                               \
	right_slash_index = 0; \
	                                                                               \
	for (i = (user_input_arg1_string_length) ; i > 0 ; i--) { \
		if (user_input_arg1_string[i-1] == '/') { \
			break; \
		} else { \
			right_slash_index++; /* how many positons left does the rightmost slash / occur */ \
		};/*end_if*/ \
	};/*end_if*/  \
	                                                                               \
	right_slash_position = (user_input_arg1_string_length - 1) - right_slash_index;	/* last positoin minus 3 of chars from end where we found the right-most slash */		\
	\
	if (right_slash_index == user_input_arg1_string_length) {/* if we searched the whole string and found no / slashes */ \
		/* leave source filename_pointer as-is, leave source path as-is */ \
	\
	} else if (right_slash_position == 0) { /* if right_slash_position == 0 then there's only a slash at the very beginning */ \
		source_filename_pointer = source_filename_pointer + 1; \
	\
	} else if (right_slash_index == 0) { /* if there's a slash at the very end */ \
		copy_error_status = TRUE; /* ERROR! in this case, there can't be only a path as the source, cause that's not a file */ \
	\
	} else { \
		source_filename_pointer = source_filename_pointer + (right_slash_position+1); \
	\
	};/*end_if*/ \
	\
	source_filename_length = strlen(source_filename_pointer); \
	\
	source_path_length = user_input_arg1_string_length - source_filename_length; \
	source_filename_length = strlen(source_filename_pointer); \
	strncpy(source_path , user_input_arg1_string , source_path_length ); /* copy the arg2 into the path leaving out the last filename characters */ \
	\
	/* -------------------- TARGET -------------------- */ \
	\
	right_slash_index = 0; \
	\
	for (i = (user_input_arg2_string_length) ; i > 0 ; i--) { \
		if (user_input_arg2_string[i-1] == '/') { \
			break; \
		} else { \
			right_slash_index++; /* how many positons left does the rightmost slash / occur */ \
		};/*end_if*/ \
	};/*end_for*/ \
	\
	right_slash_position = (user_input_arg2_string_length - 1) - right_slash_index; \
	\
	if (right_slash_index == user_input_arg2_string_length) { /* if we searched the whole string and found no slashes then... */ \
		/* use this string as the filename , the path is left empty */ \
	\
	} else if (right_slash_position == 0) { /* if there's only a slash at the very beginning */ \
		target_filename_pointer = target_filename_pointer + 1; \
	\
	} else if (right_slash_index == 0) { /* if there's a slash at the very end */ \
		target_filename_pointer = source_filename_pointer; /* then they haven't given us a filename, and the source filename is implied */ \
		strcpy(target_path,user_input_arg2_string); \
	                                                                               \
	} else { \
		target_filename_pointer = target_filename_pointer + (right_slash_position+1); \
	                                                                               \
	};/*end_if*/ \
	                                                                               \
	\
	target_filename_length = strlen(target_filename_pointer); \
	                                                                               \
	target_path_length = user_input_arg2_string_length - target_filename_length; \
	target_filename_length = strlen(target_filename_pointer); \
	strncpy(target_path , user_input_arg2_string , target_path_length ); /* copy the arg2 into the path leaving out the last filename characters */ \
	/* NEW!!! */ \
    /* if ( matching("d9b:",user_input_arg2_string) ) { */ \
    if ( (user_input_arg2_string[0] == 'd' && user_input_arg2_string[3] == ':') || (user_input_arg2_string[0] == 'd' && user_input_arg2_string[4] == ':') ) { \
	    strcpy(target_path , "" ); /* clear it */ \
	    /* puts("???"); */ \
	};/*end_if*/\
	\
	\
	/* -------------------- BUILD STRING -------------------- */                                               \
    /* start copy command, not gonna deal with partitions for now */                                           \
	/* strcpy (drive_command_string,"c0"); */                                                                  \
	/* --> Copy FILEORIGINAL from Partition 1 to Partition 2 Called FILECOPY */                                \
	                                                                                                           \
                                               /*      C2/:FILECOPY=1/:FILEORIGINAL */                         \
	/*printf("source_par:%c target_par:%c\n", source_par, target_par);    */                                   \
    strcpy (drive_command_string,"c");         /* this is the copy command */                                  \
    strncat (drive_command_string,&target_par,1);  /* this is the target partition */                          \

	if (get_drive_type(dev) == DRIVE_UIEC) {
	    strcat (drive_command_string,"/");         /* !!!!! this is the beginning of the target path */        \
	} else {
		// do nothing
	};//end-if

    strcat (drive_command_string,target_path); /* this is the target path */                                   \
    strcat (drive_command_string,":");         /* this finishes the target path */                             \
	if (target_filename_length == 0 ) {        /* add the SOURCE filename because it's inferred */             \
		strcat (drive_command_string,source_filename_pointer);                                                 \
	} else {                                   /* otherwise, add the target filename */                        \
		strcat (drive_command_string,target_filename_pointer);                                                 \
	};/*end_if*/                                                                                               \
	                                                                                                           \
	strcat (drive_command_string,"=");                     /* this finished the target path */                 \
	                                                                                                           \
	strncat (drive_command_string,&source_par,1);              /* this is the source partition */              \

	if (get_drive_type(dev) == DRIVE_UIEC) {
	    strcat (drive_command_string,"/");         /* !!!!! this is the beginning of the target path */        \
	} else {
		// do nothing
	};//end-if

	strcat (drive_command_string,source_path);             /* this is the source path */                       \
	strcat (drive_command_string,":");                     /* this finishes the source path */                 \
	strcat (drive_command_string,source_filename_pointer); /* add the source filename  */                      \
	                                                                                                           \
	printf("-->%s\n", drive_command_string);                                                                   \
	                                                                                                           \
	if (copy_error_status == TRUE) {                                                                           \
		printf("ERR: File not found!\n");                                                                      \
	} else {                                                                                                   \
		result = cbm_open(1, dev, 15, drive_command_string);                                                   \
		cbm_close(1);                                                                                          \
	};/*end_if*/                                                                                               \
	                                                                                                           \
};//end-func








void type_text( unsigned char * file_to_type ) {

	// I think I need to blank out the disk_sector_buffer right here.			
	memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));

	// if (detected_filetype == 

	printf("Detected file: %s ", file_to_type );
	detected_filetype = detect_filetype(file_to_type, TRUE);

	switch (detected_filetype) {
		case  2 : /* bad */      break;	// DIR
		case 16 : strcpy(detected_filetype_char,"s"); break; // SEQ
		case 17 : strcpy(detected_filetype_char,"p"); break; // PRG
		case 18 : strcpy(detected_filetype_char,"u"); break; // USR
		case 19 : strcpy(detected_filetype_char,"l"); break; // REL // seems liek you need to pass L instaed of R 
		default : printf("???"); //end default
	};//end switch

	// strcpy (drive_command_string, file_to_type);
	// strcat (drive_command_string, ",r,");
	// strcat (drive_command_string, detected_filetype_char);

	/* Setup SOURCE FILE for READING */                                                         
	strcpy (drive_command_string, "");                                                          
	if (get_drive_type(dev) == DRIVE_UIEC) {
	    drive_command_string[0] = par+1;
	} else {
		drive_command_string[0] = par;
	};//end-if
	drive_command_string[1] = ':';
	drive_command_string[2] = '\0';
	strcat (drive_command_string, file_to_type); /*filename*/                         
	strcat (drive_command_string, ",r,");                                                       
	strcat (drive_command_string, detected_filetype_char); 				                        

	// printf("drive_command_string: %s\n", drive_command_string);

	result = cbm_open(8, dev, CBM_READ, drive_command_string);

    //displayed_count = 0;//this is two becuase we alrieady displaeyd two lines of text 


	do {
		read_bytes = cbm_read(8, disk_sector_buffer, sizeof(disk_sector_buffer));

		// loop over buffer and barf out bytes to the screen 

		for (i = 0 ; i < read_bytes ; i++) {

			//printf("%i", wherey());

			if (wherey() == 24) {

				printf("     * Press any key to continue *"); 			
				do {
					//nothing
				} while (kbhit() == 0);//end do 
				get_key = cgetc();
				if (get_key == 3) { // RUN/STOP or CTRL-C
					printf("\n");
					read_bytes = 0;
					break;
				};//end if 

				gotox(0);
				printf("                                       ");
				gotox(0);
				clrscr();
				//displayed_count = 0;

			};//end if

			printf( "%c", disk_sector_buffer[i] ); // this needs to be way better obviously...

			// if ( wherex() == 0 ) {
			// 	++displayed_count;
			// }//end if

		};//end for 

	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop


	cbm_close (8);

	printf("\n");

};//end func 


void type_prg( unsigned char * file_to_type ) {

	// I was helped in this section by studying this code:
	// https://github.com/doj/dracopy/blob/master/src/cat.c
	// As well as this website:
	// https://wpguru.co.uk/2014/06/reading-and-writing-sequential-data-on-the-commodore-1541/
	// for Commodore 64 Basic 2.0 info, I used this:
	// https://www.c64-wiki.com/wiki/BASIC_token

			/* Setup SOURCE FILE for READING */                                                         
			strcpy (drive_command_string, "");                                                          
			if (get_drive_type(dev) == DRIVE_UIEC) {
			    drive_command_string[0] = par+1;
			} else {
				drive_command_string[0] = par;
			};//end-if
			drive_command_string[1] = ':';
			drive_command_string[2] = '\0';
			strcat (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,p");

			// printf("drive_command_string: %s\n", drive_command_string);

			// Blank out the disk_sector_buffer, so there isn't any garbage characters in it.
			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));

			result = cbm_open(8, dev, CBM_READ, drive_command_string);

			spot_in_prg = first_header_byte;

			do {
				read_bytes = cbm_read(8, disk_sector_buffer, sizeof(disk_sector_buffer));

				for (i = 0 ; i < read_bytes ; i++) { // loop over buffer and barf out bytes to the screen

					if (wherey() == 24) {

						printf("     * Press any key to continue *");
						do {
							//nothing
						} while (kbhit() == 0);//end do 
						get_key = cgetc();
						if (get_key == 3) { // RUN/STOP or CTRL-C
							printf("\n");
							read_bytes = 0;
							break;
						};//end if 

						gotox(0);
						printf("                                       "); // TODO: There's gotta be a better way of doing this!
						gotox(0);
						clrscr();

					};//end if

					if (disk_sector_buffer[i] == 0 && previous_byte == 0) { // We are at the end of the program 
						printf("\nFooter:0x%04X\n", (disk_sector_buffer[i]*256)+previous_byte); // This prints out the actual footer. But it should always be the same.
						goto type_prg_exit_point; // goto jump here beacuse it's the end of the basic stub (even though there's more file)
					};//end if

					switch ( spot_in_prg ) {

						case first_header_byte :
							//printf("H:%i",disk_sector_buffer[i]);// output nothing					   // this is statring address low byte, so we can't output anything until we get teh next byte which is the high byte 
							previous_byte = disk_sector_buffer[i]; // store in previous character
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case second_header_byte :
							//printf("%i\n",disk_sector_buffer[i]); // use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							// don't need to store anything in previous byte
							printf("Header:0x%04X\n\n", (disk_sector_buffer[i]*256)+previous_byte);
							// printf("0x%04X\n\n", (disk_sector_buffer[i]*256)+previous_byte);
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case first_basic_line_address_byte :
							//printf("A:%i",disk_sector_buffer[i]); // use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							previous_byte = disk_sector_buffer[i]; // store in previous character
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case second_basic_line_address_byte :
							//printf("%i",disk_sector_buffer[i]); // use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							// don't need to store anything in previous byte

							//THIS ENABLES SHOWING TEH ADDRESS of each basic line 
							//printf("A:%04X|", (disk_sector_buffer[i]*256)+previous_byte);
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case first_basic_line_number_byte :
							//printf("L:%i",disk_sector_buffer[i]); // use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							previous_byte = disk_sector_buffer[i]; // store in previous character
							spot_in_prg++; 						   // increment spot_in_prg
						break;

						case second_basic_line_number_byte :
							//printf("%i ",disk_sector_buffer[i]); // use previous to output "Pointer to beginning of "next" BASIC line, in low-byte/high-byte order"
							// don't need to store anything in previous byte
							printf("%u ", (disk_sector_buffer[i]*256)+previous_byte);
							spot_in_prg++; 						   // increment spot_in_prg
						break;	

						// Fuck it. Time to make this an array!
						// The initial lookup code saved 775 bytes,
						// and the "restore" exception case
						// saved another 54 bytes!
						// This 775 + 54 saved a total of 829 bytes!!!

						case body_bytes :
							if ( disk_sector_buffer[i] == 0 ) {
								printf("\n"); 
								previous_byte = disk_sector_buffer[i]; 
								spot_in_prg = first_basic_line_address_byte;
							} else if ( disk_sector_buffer[i] == 140 ) { 	 // Handled manually, to save the overall size of the array.
								printf("restore");							 // This string is 7 chars, which is the only one. So we make the max length of each string 6 chars instead of 7.
							} else if ( disk_sector_buffer[i] >= 128 && disk_sector_buffer[i] <= 203 ) {
								//printf("i%u:");
								printf("%s",basic_tokens[ disk_sector_buffer[i]-128 ]);
							} else {
							    printf( "%c", disk_sector_buffer[i] ); // this needs to be way better obviously...
							};//end-if

							// switch ( disk_sector_buffer[i] ) {
							// 	case   0 : printf("\n"     ); previous_byte = disk_sector_buffer[i]; spot_in_prg = first_basic_line_address_byte; break;
							// 	case 128 : printf("end"    ); break;
							// 	case 129 : printf("for"    ); break;
							// 	case 130 : printf("next"   ); break;
							// 	case 131 : printf("data"   ); break;
							// 	case 132 : printf("input#" ); break; /* bug here? */
							// 	case 133 : printf("input"  ); break; /* bug here? */
							// 	case 134 : printf("dim"    ); break;
							// 	case 135 : printf("read"   ); break;
							// 	case 136 : printf("let"    ); break;
							// 	case 137 : printf("goto"   ); break;
							// 	case 138 : printf("run"    ); break;
							// 	case 139 : printf("if"     ); break;
							// 	case 140 : printf("restore"); break;
							// 	case 141 : printf("gosub"  ); break;
							// 	case 142 : printf("return" ); break;
							// 	case 143 : printf("rem"    ); break;
							// 	case 144 : printf("stop"   ); break;
							// 	case 145 : printf("on"     ); break;
							// 	case 146 : printf("wait"   ); break;
							// 	case 147 : printf("load"   ); break;
							// 	case 148 : printf("save"   ); break;
							// 	case 149 : printf("verify" ); break; /* bug here? */
							// 	case 150 : printf("def"    ); break; /* bug here? */
							// 	case 151 : printf("poke"   ); break;
							// 	case 152 : printf("print#" ); break; /* bug here? */
							// 	case 153 : printf("print"  ); break; /* bug here? */
							// 	case 154 : printf("cont"   ); break;
							// 	case 155 : printf("list"   ); break;
							// 	case 156 : printf("clr"    ); break;
							// 	case 157 : printf("cmd"    ); break;
							// 	case 158 : printf("sys"    ); break;
							// 	case 159 : printf("open"   ); break;
							// 	case 160 : printf("close"  ); break;
							// 	case 161 : printf("get"    ); break;
							// 	case 162 : printf("new"    ); break;
							// 	case 163 : printf("tab("   ); break;
							// 	case 164 : printf("to"     ); break;
							// 	case 165 : printf("fn"     ); break;
							// 	case 166 : printf("spc("   ); break;
							// 	case 167 : printf("then"   ); break;
							// 	case 168 : printf("not"    ); break;
							// 	case 169 : printf("step"   ); break;
							// 	case 170 : printf("+"      ); break;
							// 	case 171 : printf("-"      ); break; // TODO: Check if something is wrong with this??? THIS: − isn't a hyphen but a unicode thingy 
							// 	case 172 : printf("*"      ); break;
							// 	case 173 : printf("/"      ); break;
							// 	case 174 : printf("%c", 94 ); break; // ↑ - up arrow --> http://sta.c64.org/cbm64pet.html
							// 	case 175 : printf("and"    ); break;
							// 	case 176 : printf("or"     ); break;
							// 	case 177 : printf(">"      ); break;
							// 	case 178 : printf("="      ); break;
							// 	case 179 : printf("<"      ); break;
							// 	case 180 : printf("sgn"    ); break;
							// 	case 181 : printf("int"    ); break;
							// 	case 182 : printf("abs"    ); break;
							// 	case 183 : printf("usr"    ); break;
							// 	case 184 : printf("fre"    ); break;
							// 	case 185 : printf("pos"    ); break;
							// 	case 186 : printf("sqr"    ); break;
							// 	case 187 : printf("rnd"    ); break;
							// 	case 188 : printf("log"    ); break;
							// 	case 189 : printf("exp"    ); break;
							// 	case 190 : printf("cos"    ); break;
							// 	case 191 : printf("sin"    ); break;
							// 	case 192 : printf("tan"    ); break;
							// 	case 193 : printf("atn"    ); break;
							// 	case 194 : printf("peek"   ); break;
							// 	case 195 : printf("len"    ); break;
							// 	case 196 : printf("str$"   ); break;
							// 	case 197 : printf("val"    ); break;
							// 	case 198 : printf("asc"    ); break;
							// 	case 199 : printf("chr$"   ); break;
							// 	case 200 : printf("left$"  ); break;
							// 	case 201 : printf("right$" ); break; /* bug here? */
							// 	case 202 : printf("mid$"   ); break; /* bug here? */
							// 	case 203 : printf("go"     ); break;
							//     default  : printf( "%c", disk_sector_buffer[i] ); // this needs to be way better obviously...
							// };// end switch

							// DO NOT increment spot_in_prg becuase we don't know if we are finished yet
						break;

						default :
							printf("?");
						break;

					};//end switch

				};//end for

			} while( read_bytes == sizeof(disk_sector_buffer) ); //end do loop

			//jump to here beacuse it's the end of the basic stub (even though there's more file)
			type_prg_exit_point: 

			cbm_close (8); // printf("Display complete.\n");

};//end func 


void type_hex( unsigned char * file_to_type ) {


			printf("Detected file: %s ", file_to_type );
			detected_filetype = detect_filetype(file_to_type, TRUE);
		
			switch (detected_filetype) {
				case  2 : /* bad */      break;	// DIR
				case 16 : strcpy(detected_filetype_char,"s"); break; // SEQ
				case 17 : strcpy(detected_filetype_char,"p"); break; // PRG
				case 18 : strcpy(detected_filetype_char,"u"); break; // USR
				case 19 : strcpy(detected_filetype_char,"l"); break; // REL // seems liek you need to pass L instaed of R 
				default : printf("???"); //end default
			};//end switch

			// switch (detected_filetype) { // this only saves 4 bytes
			// 	case  2 : /* bad */      break;	// DIR
			// 	case 16 : detected_filetype_char[0]='s';detected_filetype_char[1]='\0'; break; // SEQ
			// 	case 17 : detected_filetype_char[0]='p';detected_filetype_char[1]='\0'; break; // PRG
			// 	case 18 : detected_filetype_char[0]='u';detected_filetype_char[1]='\0'; break; // USR
			// 	case 19 : detected_filetype_char[0]='l';detected_filetype_char[1]='\0'; break; // REL // seems liek you need to pass L instaed of R 
			// 	default : printf("???"); //end default
			// };//end switch

			/* Setup SOURCE FILE for READING */                                                         
			strcpy (drive_command_string, "");                                                          
			if (get_drive_type(dev) == DRIVE_UIEC) {
			    drive_command_string[0] = par+1;
			} else {
				drive_command_string[0] = par;
			};//end-if
			drive_command_string[1] = ':';
			drive_command_string[2] = '\0';
			strcat (drive_command_string, file_to_type); /*filename*/                         
			strcat (drive_command_string, ",r,");                                                       
			strcat (drive_command_string, detected_filetype_char); 				                        

			// strcpy (drive_command_string, file_to_type);
			// strcat (drive_command_string, ",r,");
			// strcat (drive_command_string, detected_filetype_char);

			// printf("drive_command_string: %s\n", drive_command_string);

			sleep(1);

			printf("\n");
			
			// I think I need to blank out the disk_sector_buffer right here.			
			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));

			hex_display_position_in_file = 0 ;
			hex_display_position_x       = 0 ;
			hex_display_position_y       = 0 ;

			result = cbm_open(8, dev, CBM_READ, drive_command_string);

		    //displayed_count = 0;//this is two becuase we alrieady displaeyd two lines of text 
			
			clrscr();

			do {
				read_bytes = cbm_read(8, disk_sector_buffer, sizeof(disk_sector_buffer));

				// loop over buffer and barf out bytes to the screen 

				for (i = 0 ; i < read_bytes ; i++) {
					
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
					printf("%02X", disk_sector_buffer[i]);
					gotox(hex_display_position_x + 31) ;
					cputc(convert_char(disk_sector_buffer[i]));				

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

			} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop


			cbm_close (8);

			printf("\n");


};//end func 





