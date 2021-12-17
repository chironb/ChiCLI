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


extern unsigned char disk_sector_buffer    [MAX_DISK_SECTOR_BUFFER] ; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...
extern unsigned char drive_command_string  [MAX_LENGTH_COMMAND]	    ;
extern unsigned char drive_command_string2 [MAX_LENGTH_COMMAND]	    ;
extern unsigned char get_key ;
extern unsigned char result ;
extern unsigned char dev ;
extern unsigned char read_bytes ;
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

			strcpy (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,");
			strcat (drive_command_string, detected_filetype_char);

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

			strcpy (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,p");

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

			strcpy (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,");
			strcat (drive_command_string, detected_filetype_char);

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





