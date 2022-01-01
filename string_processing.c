// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// string_processing.c 
// ********************************************************************************

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif 

#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H
#include "string_processing.h"
#endif 



// ********************************************************************************
// STRING PROCESSING FUNCTIONS
// ********************************************************************************

unsigned char 	string_add_character ( unsigned char * input_string , unsigned char character_to_add ) {

	unsigned char string_length;
	string_length = strlen(input_string);

	switch (string_length) { 								// check if the string is full 

		case MAX_ENTERED_KEYSTROKES-1 :
			return(0); 										// return 0 - string full error code
		break;

		default :
			input_string[string_length] = character_to_add; // put new character at one past(strlen) the end of the string(strlen-1)
			input_string[string_length+1] = 0; 				// this shouldn't ever overflow beause this where we are checking it!
			return(1); 										// return 1 - we successfuly added a character to teh string 
		//end default

	};//end switch

};//end func 


unsigned char string_insert_character ( unsigned char * input_string , unsigned char character_to_insert , unsigned char position_to_insert ) {

	unsigned char output_string[MAX_ENTERED_KEYSTROKES] = "";
	unsigned char i;
	unsigned char string_length;

	string_length = strlen(input_string);

	switch (string_length) { 			// check if the string is full 

		case MAX_ENTERED_KEYSTROKES-1 :
			return(0); 					// return 0 - string full error code
		break;

		default :
			for (i = 0; i < MAX_ENTERED_KEYSTROKES ; i++) {
				if (i == position_to_insert ) { //?
					string_add_character(output_string, character_to_insert);
					string_add_character(output_string, input_string[i]);
				} else {
					string_add_character(output_string, input_string[i]);
				};//end if
			};//end for
		//end default

	};//end switch

	strcpy(input_string, output_string); // overwrite input string with the newly inserted string

	return(1);

};//end func


unsigned char string_remove_character ( unsigned char * input_string ) {

	unsigned char string_length;
	string_length = strlen(input_string);

	switch (string_length) { 					// check if the string is empty 

		case 0 :
			return(0); 							// return 0 - string empty error code
		break;

		default :
			input_string[string_length-1] = 0; 	// subtract 1 from the length to get the last position char, and then we want to remove it, so write \0 to it 
			return(1); 							// return 1 - we successfuly removed a character to teh string 
		//end default

	};//end switch

};//end func 


unsigned char string_deinsert_character ( unsigned char * input_string , unsigned char position_to_remove ) {

	unsigned char output_string[MAX_ENTERED_KEYSTROKES] = "";
	unsigned char i;
	unsigned char string_length;

	string_length = strlen(input_string);

	switch (string_length) { 			// check if the string is empty 

		case 0 :
			return(0); 					// return 0 - string full error code
		break;

		default :
			for (i = 0; i < MAX_ENTERED_KEYSTROKES ; i++) {
				if (i == position_to_remove) {
					//do nothing 
				} else {
					string_add_character(output_string, input_string[i]);
				};//end if 
			};//end for 
		//end default

	};//end switch 

	strcpy(input_string, output_string); // overwrite input string with the newly removed character string 

	return(1);

};//end func


unsigned char display_substring( unsigned char * string_to_display , unsigned char positon_to_start ) {

	unsigned char i;
	unsigned char string_length;
	unsigned char screen_scrolled = 0; 	// 0 - not scrolled, 1 - has scrolled once, 2 - has scrolled twice, etc...

	string_length = strlen(string_to_display); 

	for (i = positon_to_start; i < string_length ; i++) {

		if ( wherex() == SCREEN_RIGHT_EDGE && wherey() == SCREEN_BOTTOM_EDGE) {
			printf("\n"); 				//scroll the display once 
			gotoxy(SCREEN_RIGHT_EDGE,wherey()-1); 		// sometimes we move 2 lines, sometimes 1 line, move back to where we were, offset by the scroll
			cputc(string_to_display[i]);
			screen_scrolled++;
		} else {
			cputc(string_to_display[i]);
		};//end if

	};//end for 

	return(screen_scrolled);

};//end func 


unsigned char display_blank_substring( unsigned char * string_to_display , unsigned char positon_to_start ) {

	unsigned char i;
	unsigned char string_length;
	unsigned char screen_scrolled = 0; 	// 0 - not scrolled, 1 - has scrolled once, 2 - has scrolled twice, etc...

	string_length = strlen(string_to_display); 

	for (i = positon_to_start; i <= string_length ; i++) { // note: the <= isn't a mistake. We want to clear the string AND the cursor character! This does that.

		if ( wherex() == SCREEN_RIGHT_EDGE && wherey() == SCREEN_BOTTOM_EDGE) {
			printf("\n"); 				//scroll the display once 
			gotoxy(SCREEN_RIGHT_EDGE,wherey()-1); 		// sometimes we move 2 lines, sometimes 1 line, move back to where we were, offset by the scroll
			cputc(' ');
			screen_scrolled++;
		} else {
			cputc(' ');
		};//end if

	};//end for 

	return(screen_scrolled);

};//end func 


void strip_newlines(unsigned char * input_string) {
	if ( input_string[strlen(input_string)-1] == '\n' ) {
		input_string[strlen(input_string)-1]='\0';
	} else {
		// do nothing
	};//end if 
};//end func 


void replace_characters(unsigned char * input_string , unsigned char character_to_find , unsigned char replace_with_character ) {

	unsigned char i;

	for (i = 0 ; i < strlen(input_string) ; i++) {
		if ( input_string[i] == character_to_find ) {
			input_string[i] = replace_with_character;
		};//end if 
	};//end for 

};//end func


void process_quotes(unsigned char * input_string , unsigned char * output_string) {

	unsigned char i;
	unsigned char j = 0;
	unsigned char quote_status = 0; 									// 1 quote open, 0 quote closed 

	for (i = 0 ; i < strlen(input_string) ; i++) {

		if ( input_string[i] == '\"' && quote_status == 0) { 			// start of quoted area
			quote_status = 1;
			output_string[j] = ' ';
			j++;

		} else if ( input_string[i] == '\"' && quote_status == 1) { 	// end of quoted area
			quote_status = 0;
			output_string[j] = ' ';
			j++;

		} else if ( input_string[i] == ' ' && quote_status == 1 ) {
			output_string[j] = '\"'; 									//when saving a file on the C64, you can't save a char by typing CMD+RETURN, which is keycode = 141	
			j++;

		} else {
			output_string[j] = input_string[i]; 						// copy existing characters
			j++;

		};//end if 

	};//end for 

	output_string[j] = '\0';

};//end func


void remove_extra_spaces(unsigned char * input_string ) {

	unsigned char i  = 0;
	unsigned char ii = 0;
	unsigned char found_a_space = 0;
	unsigned char output_string[MAX_ENTERED_KEYSTROKES] = "";

	for (i = 0 ; i < strlen(input_string) ; i++) {

		if ( input_string[i] == ' ' &&  found_a_space == 0 ) {
			output_string[ii] = ' ';
			ii++;
			found_a_space = 1;

		} else if ( input_string[i] == ' ' &&  found_a_space == 1 ){
			found_a_space = 1;

		} else {
			output_string[ii] = input_string[i];
			ii++;
			found_a_space = 0;

		};//end if

	};//end for

	strcpy(input_string, output_string); // overwrite input string with the newly inserted string 

};//end func


unsigned char is_string_only_numbers(unsigned char * input_string ) {

	unsigned char i  = 0;

	for (i = 0 ; i < strlen(input_string) ; i++) {

		// TODO: Also... this is cheap and stupid. There's for sure a better way.
		switch(input_string[i]) {
			case '0' : ;
			case '1' : ;
			case '2' : ;
			case '3' : ;
			case '4' : ;
			case '5' : ;
			case '6' : ;
			case '7' : ;
			case '8' : ;
			case '9' : break;
			default  : return(0); break;
		};//end-switch

	};//end for 

	return(1); 								// is_string_only_zeros == TRUE (1) means the stirng is only zeros, otherwise FALSE (0) means there's more than zeros

};//end func
