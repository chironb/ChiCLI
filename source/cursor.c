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

#ifndef CURSOR_H
	#define CURSOR_H
	#include "cursor.h"
#endif

#ifndef STRING_PROCESSING_H
	#define STRING_PROCESSING_H
	#include "string_processing.h"
#endif


extern unsigned char entered_keystrokes[MAX_ENTERED_KEYSTROKES];
extern unsigned char position_in_string;
extern unsigned char previous_x;
extern unsigned char previous_y;
extern unsigned char position_at_prompt_x;
extern unsigned char position_at_prompt_y;
extern signed int    result;
extern unsigned char keystroke;

// Saved 195 bytes!
void cursor_home(void){																			\
	if ( position_in_string != 0 ) {															\
		if ( position_in_string == strlen(entered_keystrokes) ) { /* See Note above. */         \
			cputc(' ');																			\
		} else {																				\
			cputc(entered_keystrokes[position_in_string]); /* restore char where cursor was */	\
		};/*end-if*/																			\
																								\
		gotox(position_at_prompt_x);															\
		gotoy(position_at_prompt_y);															\
																								\
		position_in_string = 0;																	\
																								\
		previous_x = wherex();	 /* remember where we are...             */						\
		previous_y = wherey(); 	 /* ...so we can come back here          */						\
		cputc(CURSOR_CHARACTER); /* ...so we can come back here          */						\
		gotoy(previous_y);       /* go back to...                        */						\
		gotox(previous_x);       /* ...where we were, but move forward 1 */						\
	};/*end-if*/																				\
};//end-func

void cursor_end(void){																		\
	if ( position_in_string != strlen(entered_keystrokes) ) {								\
																							\
		gotox(position_at_prompt_x);														\
		gotoy(position_at_prompt_y);														\
																							\
		result = display_substring(entered_keystrokes, 0);	/* moving forward so +1 */		\
		if (result == 1) { 									/* has scrolled         */		\
			previous_y = previous_y-result;													\
		};/*end-if*/ 			    														\
																							\
		position_in_string = strlen(entered_keystrokes);									\
																							\
		if (wherex()==39) {																	\
			/* actual cursor has line wrapped therefore:        */ 							\
			/* move up one and then move to the last x position */ 							\
			/* otherwise just move back one                     */ 							\
			cputc(CURSOR_CHARACTER);														\
			gotoy(wherey()-1);																\
			gotox(39);																		\
		} else {																			\
			cputc(CURSOR_CHARACTER);														\
			gotox(wherex()-1);																\
		};/*end-if*/																		\
																							\
	};/*end-if*/																			\
};//end-func

void cursor_add_character(void){
	/* ADD CHARACTER TO END */														\
	if ( position_in_string == strlen(entered_keystrokes) ) { /* if at end */		\
	/* is teh command string  not full? if it is, do nothing */			 			\
		if ( string_add_character(entered_keystrokes, keystroke) != 0 ) { 			\
			if (wherex() == SCREEN_RIGHT_EDGE-1) {									\
				cputc(keystroke);													\
				previous_x = SCREEN_RIGHT_EDGE;										\
				previous_y = wherey();												\
				cputc(CURSOR_CHARACTER);											\
			  	gotox(previous_x);													\
			  	gotoy(previous_y);													\
			} else if (wherex() == SCREEN_RIGHT_EDGE) {								\
				if (wherex()==SCREEN_RIGHT_EDGE && wherey()==SCREEN_BOTTOM_EDGE){	\
					printf("\n");													\
					gotoy(SCREEN_BOTTOM_EDGE-1);									\
					gotox(SCREEN_RIGHT_EDGE);										\
					position_at_prompt_y--;	/* since we scrolled */					\
				};/*end if*/ 														\
				cputc(keystroke);													\
				cputc(CURSOR_CHARACTER);											\
				gotox(SCREEN_LEFT_EDGE);			    							\
			} else {																\
				cputc(keystroke);													\
				cputc(CURSOR_CHARACTER);											\
				gotox(wherex()-1);													\
			};/*end if*/															\
			position_in_string++;													\
		};/*end if*/																\
																					\
	/* INSERT CHARACTER IN THE MIDDLE 		*/										\
	/* if anywhere else but string not full */										\
	} else if (strlen(entered_keystrokes) != (MAX_ENTERED_KEYSTROKES-1) ) { 		\
		if (wherex() == SCREEN_RIGHT_EDGE-1) {										\
			cputc(keystroke);														\
			previous_x = SCREEN_RIGHT_EDGE;											\
			previous_y = wherey();													\
			result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { /* has scrolled */									\
				previous_y = previous_y-result;										\
			};/*end if*/															\
			gotox(previous_x);														\
			gotoy(previous_y);														\
			cputc(CURSOR_CHARACTER);												\
			gotox(previous_x);														\
			gotoy(previous_y);														\
		} else if (wherex() == SCREEN_RIGHT_EDGE) {									\
			if ( wherex() == SCREEN_RIGHT_EDGE && wherey() == SCREEN_BOTTOM_EDGE) {	\
				printf("\n");														\
				gotoy(SCREEN_BOTTOM_EDGE-1);										\
				gotox(SCREEN_RIGHT_EDGE);											\
				position_at_prompt_y--;	/* since we scrolled */						\
			};/*end if*/															\
			cputc(keystroke);														\
			gotox(SCREEN_LEFT_EDGE);												\
	    	previous_y = wherey();													\
			result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { /* since we scrolled */								\
				previous_y = previous_y-result;										\
			};/*end if*/ 															\
			gotox(SCREEN_LEFT_EDGE);												\
			gotoy(previous_y);														\
			cputc(CURSOR_CHARACTER); 												\
			gotox(SCREEN_LEFT_EDGE);												\
			gotoy(previous_y);														\
		} else {																	\
			cputc(keystroke);		    											\
	    	previous_x = wherex();													\
	    	previous_y = wherey();		    										\
	    	result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { /* since we scrolled */								\
				previous_y = previous_y-result;										\
			};/*end if*/			    											\
	    	gotox(previous_x);														\
	    	gotoy(previous_y);														\
	    	cputc(CURSOR_CHARACTER);												\
			gotox(previous_x);														\
			gotoy(previous_y);														\
		};/*end if*/																\
																					\
		string_insert_character(entered_keystrokes, keystroke, position_in_string);	\
		position_in_string++;														\
	};/*end if*/																	\
};//end-func
