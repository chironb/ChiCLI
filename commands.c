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


extern unsigned char disk_sector_buffer[255]; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...
extern unsigned char 	  drive_command_string[MAX_LENGTH_COMMAND]	;
extern unsigned char 	  drive_command_string2[MAX_LENGTH_COMMAND]	;
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

			// this section was helped by me studying this code: 
			// https://github.com/doj/dracopy/blob/master/src/cat.c
			// as well as this website
			// https://wpguru.co.uk/2014/06/reading-and-writing-sequential-data-on-the-commodore-1541/

			// for Commodore 64 Basic 2.0 info, I used this: 
			// https://www.c64-wiki.com/wiki/BASIC_token

			//printf("%i", wherey());

			strcpy (drive_command_string, file_to_type);
			strcat (drive_command_string, ",r,p");

			//printf("\n");
			
			// I think I need to blank out the disk_sector_buffer right here.			
			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));

			result = cbm_open(8, dev, CBM_READ, drive_command_string);

		    //displayed_count = 0;//this is two becuase we alrieady displaeyd two lines of text 



			spot_in_prg = first_header_byte;

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


					// // the following if for type-prg functionality 
					// #define first_header_byte  				0 
					// #define second_header_byte 				1
					// #define first_basic_line_address_byte   2
					// #define second_basic_line_address_byte  3
					// #define first_basic_line_number_byte    4
					// #define second_basic_line_number_byte   5
					// #define body_bytes         				6
					// #define end_of_line_byte   				7
					// #define end_of_prg_byte    				8
					// unsigned char spot_in_prg = 0;
					// unsigned char previous_byte = ' ';

					// do things here


					if (disk_sector_buffer[i] == 0 && previous_byte == 0) { // we are at the end of the program 
						//printf("\nEnd of program.\n");						
						printf("\nFooter:0x%04X\n", (disk_sector_buffer[i]*256)+previous_byte);
						//goto jump here beacuse it's the end of the basic stub (even though there's more file) 	
						goto type_prg_exit_point;	
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





						case body_bytes :
							switch ( disk_sector_buffer[i] ) {
								case   0 : printf("\n"     ); previous_byte = disk_sector_buffer[i]; spot_in_prg = first_basic_line_address_byte; break;
								case 128 : printf("end"    ); break;
								case 129 : printf("for"    ); break;
								case 130 : printf("next"   ); break;
								case 131 : printf("data"   ); break;
								case 132 : printf("input#" ); break;
								case 133 : printf("input"  ); break;
								case 134 : printf("dim"    ); break;
								case 135 : printf("read"   ); break;
								case 136 : printf("let"    ); break;
								case 137 : printf("goto"   ); break;
								case 138 : printf("run"    ); break;
								case 139 : printf("if"     ); break;
								case 140 : printf("restore"); break;
								case 141 : printf("gosub"  ); break;
								case 142 : printf("return" ); break;
								case 143 : printf("rem"    ); break;
								case 144 : printf("stop"   ); break;
								case 145 : printf("on"     ); break;
								case 146 : printf("wait"   ); break;
								case 147 : printf("load"   ); break;
								case 148 : printf("save"   ); break;
								case 149 : printf("verify" ); break;
								case 150 : printf("def"    ); break;
								case 151 : printf("poke"   ); break;
								case 152 : printf("print#" ); break;
								case 153 : printf("print"  ); break;
								case 154 : printf("cont"   ); break;
								case 155 : printf("list"   ); break;
								case 156 : printf("clr"    ); break;
								case 157 : printf("cmd"    ); break;
								case 158 : printf("sys"    ); break;
								case 159 : printf("open"   ); break;
								case 160 : printf("close"  ); break;
								case 161 : printf("get"    ); break;
								case 162 : printf("new"    ); break;
								case 163 : printf("tab("   ); break;
								case 164 : printf("to"     ); break;
								case 165 : printf("fn"     ); break;
								case 166 : printf("spc("   ); break;
								case 167 : printf("then"   ); break;
								case 168 : printf("not"    ); break;
								case 169 : printf("step"   ); break;
								case 170 : printf("+"      ); break;
								case 171 : printf("-"      ); break; // TODO: Check if something is wrong with this??? THIS: − isn't a hyphen but a unicode thingy 
								case 172 : printf("*"      ); break;
								case 173 : printf("/"      ); break;
								case 174 : printf("%c", 94 ); break; // ↑ - up arrow --> http://sta.c64.org/cbm64pet.html
								case 175 : printf("and"    ); break;
								case 176 : printf("or"     ); break;
								case 177 : printf(">"      ); break;
								case 178 : printf("="      ); break;
								case 179 : printf("<"      ); break;
								case 180 : printf("sgn"    ); break;
								case 181 : printf("int"    ); break;
								case 182 : printf("abs"    ); break;
								case 183 : printf("usr"    ); break;
								case 184 : printf("fre"    ); break;																	
								case 185 : printf("pos"    ); break;
								case 186 : printf("sqr"    ); break;
								case 187 : printf("rnd"    ); break;
								case 188 : printf("log"    ); break;
								case 189 : printf("exp"    ); break;
								case 190 : printf("cos"    ); break;
								case 191 : printf("sin"    ); break;
								case 192 : printf("tan"    ); break;
								case 193 : printf("atn"    ); break;
								case 194 : printf("peek"   ); break;
								case 195 : printf("len"    ); break;
								case 196 : printf("str$"   ); break;
								case 197 : printf("val"    ); break;
								case 198 : printf("asc"    ); break;
								case 199 : printf("chr$"   ); break;
								case 200 : printf("left$"  ); break;
								case 201 : printf("right$" ); break;
								case 202 : printf("mid$"   ); break;
								case 203 : printf("go"     ); break;
							    default  : printf( "%c", disk_sector_buffer[i] ); // this needs to be way better obviously...
							};// end switch
							// DO NOT increment spot_in_prg becuase we don't know if we are finished yet
						break;

						// case end_of_line_byte :
						// 	if (previous_byte == 0) { // we are at the end of the program 
						// 		printf("\n End of program.\n\n");// output a newline and maybe an end of program messager
						// 		// might need to break out of a loop here, in case there is other stuff at the end of the basic program 
						// 	} else {
						// 		// this is the end of a basic line, so output a newline to teh screen 
						// 		printf("\n");//
						// 		previous_byte = disk_sector_buffer[i]; // store in previous character, because we will need it if we reach the end of the program 
						// 	};//end if
						// 	spot_in_prg = first_basic_line_address_byte; 						   // iwe have finished a line, so reset back to the begging of a line status, not beginning of file status 
						// break;

						default : 
							printf("?");
							//spot_in_prg++; 						   // increment spot_in_prg

					};//end switch


					


				};//end for 

			} while( read_bytes == sizeof(disk_sector_buffer) ); //end do loop


			//jump to here beacuse it's the end of the basic stub (even though there's more file) 
			type_prg_exit_point: 

			cbm_close (8);

			//printf("Display complete.\n");

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





