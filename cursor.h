// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// cursor.h
// ********************************************************************************

// ********************************************************************************
// CURSOR MACRO FUNCTIONS 
// ********************************************************************************

#define cursor_home() 																		\
	if ( position_in_string == strlen(entered_keystrokes) ) {								\
		cputc(' ');																			\
	} else {																				\
		cputc(entered_keystrokes[position_in_string]); /* restore char where cursor was */	\
	};/*end if*/																			\
																							\
	gotox(position_at_prompt_x);														\
	gotoy(position_at_prompt_y);														\
																							\
	position_in_string = 0;																	\
																							\
	previous_x = wherex();	/* remember where we are...    */								\
	previous_y = wherey(); 	/* ...so we can come back here */								\
	cputc(CURSOR_CHARACTER);																\
	gotoy(previous_y);  /* go back to...  */											\
	gotox(previous_x);  /* ...where we were, but move forward 1	*/						\
//end macro func


#define cursor_end() 																		\
	if ( position_in_string != strlen(entered_keystrokes) ) {								\
																							\
		gotox(position_at_prompt_x);													\
		gotoy(position_at_prompt_y);													\
																							\
		result = display_substring(entered_keystrokes, 0);	/* moving forward so +1 */		\
		if (result == 1) { 									/* has scrolled */				\
			previous_y = previous_y-result;													\
		};/*end if*/ 			    														\
																							\
		position_in_string = strlen(entered_keystrokes);									\
																							\
		cputc(CURSOR_CHARACTER);															\
		gotox(wherex()-1);																\
																							\
	};/*end if*/																			\
//end macro func


// we need to create a "stashed" commmand string, so when they press down it gets them back to whatever they werwe typign 
#define cursor_up() 													\
	previous_x = position_at_prompt_x;									\
	previous_y = position_at_prompt_y;									\
																		\
	gotox(previous_x);												\
	gotoy(previous_y);												\
																		\
	original_position_at_prompt_y = previous_y;							\
																		\
	result = display_blank_substring(entered_keystrokes, 0);			\
	if (result > 0) { /* has scrolled */								\
		previous_y = previous_y-result;									\
	};/*end if*/ 														\
																		\
	gotox(previous_x);												\
	gotoy(previous_y);												\
																		\
	strcpy(entered_keystrokes, previous_entered_keystrokes);			\
																		\
	result = display_substring(entered_keystrokes, 0);					\
	if (result > 0) { /* has scrolled */								\
		previous_y = previous_y-result;									\
		position_at_prompt_y = position_at_prompt_y-result;				\
	};/*end if*/ 														\
																		\
	previous_x = wherex();			    								\
	previous_y = wherey();												\
																		\
	cputc(CURSOR_CHARACTER);											\
	gotox(previous_x);												\
	gotoy(previous_y);												\
																		\
	position_in_string = strlen(entered_keystrokes); 					\
																		\
	if ( original_position_at_prompt_y == SCREEN_BOTTOM_EDGE  &&  wherey() == SCREEN_BOTTOM_EDGE-1 ) {	\
		position_at_prompt_y--;											\
	};/*end if*/ 														\
//end macro func


// we need to create a "stashed" commmand string, so when they press down it gets them back to whatever they werwe typign 
#define cursor_down() 																	\
	previous_x = position_at_prompt_x;													\
	previous_y = position_at_prompt_y;													\
																						\
	gotox(previous_x);																\
	gotoy(previous_y);																\
																						\
	result = display_blank_substring(entered_keystrokes, 0);							\
	cputc(' ');																			\
																						\
	if (result > 0) { /* has scrolled */ 												\
		previous_y = previous_y-result;													\
	};/*end if*/																		\
																						\
	gotox(previous_x);																\
	gotoy(previous_y);																\
																						\
	cputc(CURSOR_CHARACTER);															\
	gotox(previous_x);																\
	gotoy(previous_y);																\
																						\
	/* this is right - cursor should be at NEXT char after the end of the string */		\
	position_in_string = 0;  															\
																						\
	memset(entered_keystrokes,0,sizeof(entered_keystrokes));							\
																						\
	position_at_prompt_x = wherex();													\
	position_at_prompt_y = wherey();													\
//end macro func


#define cursor_backspace() 																		\
	if (position_in_string==(strlen(entered_keystrokes))&&strlen(entered_keystrokes)!= 0){ 		\
		string_remove_character(entered_keystrokes); /* are we at the end? */					\
	} else if (position_in_string>0&&strlen(entered_keystrokes)!=0) { /* or middle? */			\
		string_deinsert_character(entered_keystrokes, position_in_string-1);					\
	};/*end if*/																				\
																								\
	if (position_in_string == 0) {																\
																								\
		/* do nothing */ 																		\
																								\
	} else if (wherex() == 0) {		/*since there's a prompt, it must me on a second line*/ 	\
																								\
		gotoy(wherey()-1); 		/*go up one row*/ 											\
		previous_y = wherey();	/* .remember where we are... so we can come back here	*/  	\
		gotox(SCREEN_RIGHT_EDGE);         		/*go to the end of the line */				\
		cputc(CURSOR_CHARACTER);	       			/*draw teh cursor*/							\
		result = result = display_substring(entered_keystrokes, position_in_string);			\
																								\
		if (result == 1) { 			/* has scrolled*/											\
			previous_y = previous_y-1;															\
		};/*end if*/																			\
																								\
		cputc(' ');	       			/*erase old character*/										\
		gotoy(previous_y);   	/*go back to the line we were on*/							\
		gotox(SCREEN_RIGHT_EDGE); 				/*go back to where we were, minus one*/		\
		position_in_string--;  																	\
																								\
	} else if (wherex() == SCREEN_RIGHT_EDGE) {													\
																								\
		previous_y = wherey();		/* .remember where we are... so we can come back here*/		\
		gotox(SCREEN_RIGHT_EDGE-1);         		/*go to the end of the line */			\
		cputc(CURSOR_CHARACTER);	       			/*draw teh cursor*/							\
		result = display_substring(entered_keystrokes, position_in_string); 					\
		if (result == 1) { /* has scrolled*/													\
			previous_y = previous_y-1;															\
		};/*end if*/																			\
		cputc(' ');	       			/*erase old character*/										\
		gotoy(previous_y);   	/*go back to the line we were on*/							\
		gotox(SCREEN_RIGHT_EDGE-1);	/*go back to where we were, minus one*/					\
		position_in_string--;  																	\
																								\
	} else if (position_in_string != 0) {														\
																								\
		gotox(wherex()-1); 		/*backup */													\
		cputc(CURSOR_CHARACTER);	       			/*draw cursor*/								\
		cputc(' ');	       			/*erase old cursor*/										\
		gotox(wherex()-1); 		/*go back again	    */										\
		previous_x = wherex();		/* remember where we are...*/								\
		previous_y = wherey();		/* ...so we can come back here	    	*/					\
		result = display_substring(entered_keystrokes, position_in_string);/*display rest*/		\
		if (result == 1) { 			/* has scrolled*/											\
			previous_y = previous_y-1;															\
		};/*end if*/																			\
		cputc(' ');	       			/*erase old character*/										\
		gotox(previous_x-1); 	/*go back to where we were, minus one*/						\
		gotoy(previous_y);   	/*go back to the line we were on  */						\
		position_in_string--;																	\
																								\
	};/*end if*/																				\
//end macro func


#define cursor_delete() 																				\
	if ( position_in_string != strlen(entered_keystrokes) && strlen(entered_keystrokes) != 0 ){  		\
		string_deinsert_character(entered_keystrokes, position_in_string); /* at end & not empty */		\
		previous_x = wherex();			    															\
		previous_y = wherey();																			\
    	result = display_substring(entered_keystrokes, position_in_string); /* moving forward so +1 */ 	\
    	if (result == 1) { /* has scrolled */															\
			previous_y = previous_y-1;																	\
		};/*end if*/			 																		\
		cputc(' ');																						\
    	gotox(previous_x);																			\
    	gotoy(previous_y);																			\
    	cputc(CURSOR_CHARACTER);																		\
		gotox(previous_x);																			\
		gotoy(previous_y);																			\
	};/*end if*/																						\
//end macro func


#define cursor_left() 																				\
	if (position_in_string == 0) {																	\
																									\
		/* do nothing */																			\
																									\
	} else {																						\
																									\
		if (wherex() == 0) {																		\
			previous_x = wherex();		/* remember where we are...*/								\
	     	previous_y = wherey();		/* ...so we can come back here*/							\
																									\
	     	gotox(SCREEN_LEFT_EDGE);																\
	     	cputc(' ');																				\
																									\
			gotoy(previous_y);		/* go back to... 	*/										\
			gotox(previous_x);		/* ...where we were	*/										\
																									\
			/* write out the rest of the string after (+1) where we are (where the cursor is) */	\
			/* if the output has scrolled, then our old position has moved up...			  */	\
			/* ...so we need to update where we thought we were, 							  */	\
			/* so we can return to the right looking place on the newly scrolled screen       */	\
																									\
			result = display_substring(entered_keystrokes, position_in_string); 					\
			if (result == 1) { 																		\
				previous_y = previous_y-result; 													\
			};/*end if*/																			\
																									\
			position_in_string--;		/* update where we are in the string*/						\
																									\
			gotoy(previous_y-1);	/* go back to...  */										\
			gotox(SCREEN_RIGHT_EDGE);  			/* ...where we were, but move forward 1*/		\
			cputc(CURSOR_CHARACTER); 		  		/* draw the cursor	*/							\
			gotoy(previous_y-1);    /* go back to...  */										\
			gotox(SCREEN_RIGHT_EDGE);  			/* ...where we were*/							\
																									\
		} else if (wherex() == SCREEN_RIGHT_EDGE) {													\
																									\
			previous_x = wherex(); 		/* remember where we are...							 */		\
	     	previous_y = wherey(); 		/* ...so we can come back here */							\
			gotox(wherex()-1);																	\
			cputc(' '); 				/* this is for when we are at the end of the string  */		\
			cputc(' '); 				/* this is for when we are at the end of the string */		\
			gotoy(previous_y);    	/* go back to...   */										\
			gotox(previous_x-1);  	/* ...where we were */										\
																									\
			position_in_string--; 		/* update where we are in the string */						\
																									\
			previous_x = wherex(); 		/* remember where we are... */								\
	     	previous_y = wherey(); 		/* ...so we can come back here */							\
																									\
			result = display_substring(entered_keystrokes, position_in_string); 					\
			if (result == 1) { 																		\
				previous_y = previous_y-result; 													\
			};/*end if*/																			\
																									\
			gotoy(previous_y);    	/* go back to...   */										\
			gotox(previous_x);  	/* ...where we were, but move forward 1 */					\
			cputc(CURSOR_CHARACTER); 	/* draw the cursor	*/										\
			gotoy(previous_y);    	/* go back to...   */										\
			gotox(previous_x);  	/* ...where we were */										\
																									\
		} else {																					\
																									\
			previous_x = wherex(); 		/* remember where we are... */								\
	     	previous_y = wherey(); 		/* ...so we can come back here */							\
			gotox(wherex()-1);																	\
			cputc(' '); 				/* this is for when we are at the end of the string  */		\
			cputc(' '); 				/* this is for when we are at the end of the string */		\
			gotox(wherex()-2);																	\
																									\
			position_in_string--; 		/* update where we are in the string */						\
																									\
			previous_x = wherex(); 		/* remember where we are... */								\
	     	previous_y = wherey(); 		/* ...so we can come back here */							\
																									\
			result = display_substring(entered_keystrokes, position_in_string); 					\
			if (result == 1) { 																		\
				previous_y = previous_y-result; 													\
			};/*end if*/																			\
																									\
			gotoy(previous_y);    	/* go back to...   */										\
			gotox(previous_x);  	/* ...where we were, but move forward 1 */					\
			cputc(CURSOR_CHARACTER); 	/*  		// draw the cursor					 */			\
			gotoy(previous_y);    	/* go back to...   */										\
			gotox(previous_x);  	/* ...where we were */										\
																									\
		};/*end if*/																				\
																									\
	};/*end if*/																					\
//end macro func


#define cursor_right() 																\
	if (position_in_string == strlen(entered_keystrokes)) {							\
																					\
		/* do nothing */															\
																					\
	} else {																		\
																					\
		if (wherex() == SCREEN_RIGHT_EDGE) {										\
																					\
			previous_x = wherex(); 		/* remember where we are... */				\
	     	previous_y = wherey(); 		/* ...so we can come back here */			\
																					\
			result = display_substring(entered_keystrokes, position_in_string);		\
			if (result == 1) { 														\
				previous_y = previous_y-result; 									\
			};/*end if*/ 															\
																					\
			gotoy(previous_y+1);	/* go back to...  					    */  \
			gotox(SCREEN_LEFT_EDGE);/* ...where we were, but move forward 1 */  \
			cputc(CURSOR_CHARACTER); 	/* draw the cursor					    */  \
			gotoy(previous_y+1);    /* go back to...        				*/  \
			gotox(SCREEN_LEFT_EDGE);/* ...where we were   					*/  \
			position_in_string++;		/* update where we are in the string    */  \
																					\
		} else  {																	\
																					\
			previous_x = wherex(); 		/* remember where we are... */				\
	     	previous_y = wherey(); 		/* ...so we can come back here */			\
																					\
			result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { 														\
				previous_y = previous_y-result; 									\
			};/*end if*/ 															\
																					\
			gotoy(previous_y);    	/* go back to...   						*/	\
			gotox(previous_x+1);  	/* ...where we were, but move forward 1 */	\
			cputc(CURSOR_CHARACTER); 	/* draw the cursor					 	*/	\
			gotoy(previous_y);    	/* go back to...   						*/	\
			gotox(previous_x+1);  	/* ...where we were 					*/	\
			position_in_string++; 		/* update where we are in the string 	*/	\
		};/*end if*/ 																\
																					\
	};/*end if*/ 																	\
//end macro func




#define cursor_add_character() 														\
	/* ADD CHARACTER TO END */														\
	if ( position_in_string == strlen(entered_keystrokes) ) { /* if at end */		\
	/* is teh command string  not full? if it is, do nothing */			 			\
		if ( string_add_character(entered_keystrokes, keystroke) != 0 ) { 			\
			if (wherex() == SCREEN_RIGHT_EDGE-1) {									\
				cputc(keystroke);													\
				previous_x = SCREEN_RIGHT_EDGE;										\
				previous_y = wherey();												\
				cputc(CURSOR_CHARACTER);											\
			  	gotox(previous_x);												\
			  	gotoy(previous_y);												\
			} else if (wherex() == SCREEN_RIGHT_EDGE) {								\
				if ( wherex() == SCREEN_RIGHT_EDGE && wherey() == SCREEN_BOTTOM_EDGE) {				\
					printf("\n");													\
					gotoy(SCREEN_BOTTOM_EDGE-1);													\
					gotox(SCREEN_RIGHT_EDGE);									\
					position_at_prompt_y--;	/* since we scrolled */					\
				};/*end if*/ 														\
				cputc(keystroke);													\
				cputc(CURSOR_CHARACTER);											\
				gotox(SCREEN_LEFT_EDGE);			    						\
			} else {																\
				cputc(keystroke);													\
				cputc(CURSOR_CHARACTER);											\
				gotox(wherex()-1);												\
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
			gotox(previous_x);													\
			gotoy(previous_y);													\
			cputc(CURSOR_CHARACTER);												\
			gotox(previous_x);													\
			gotoy(previous_y);													\
		} else if (wherex() == SCREEN_RIGHT_EDGE) {									\
			if ( wherex() == SCREEN_RIGHT_EDGE && wherey() == SCREEN_BOTTOM_EDGE) {					\
				printf("\n");														\
				gotoy(SCREEN_BOTTOM_EDGE-1);														\
				gotox(SCREEN_RIGHT_EDGE);										\
				position_at_prompt_y--;	/* since we scrolled */						\
			};/*end if*/															\
			cputc(keystroke);														\
			gotox(SCREEN_LEFT_EDGE);											\
	    	previous_y = wherey();													\
			result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { /* since we scrolled */								\
				previous_y = previous_y-result;										\
			};/*end if*/ 															\
			gotox(SCREEN_LEFT_EDGE);											\
			gotoy(previous_y);													\
			cputc(CURSOR_CHARACTER); 												\
			gotox(SCREEN_LEFT_EDGE);											\
			gotoy(previous_y);													\
		} else {																	\
			cputc(keystroke);		    											\
	    	previous_x = wherex();													\
	    	previous_y = wherey();		    										\
	    	result = display_substring(entered_keystrokes, position_in_string); 	\
			if (result == 1) { /* since we scrolled */								\
				previous_y = previous_y-result;										\
			};/*end if*/			    											\
	    	gotox(previous_x);													\
	    	gotoy(previous_y);													\
	    	cputc(CURSOR_CHARACTER);												\
			gotox(previous_x);													\
			gotoy(previous_y);													\
		};/*end if*/																\
																					\
		string_insert_character(entered_keystrokes, keystroke, position_in_string);	\
		position_in_string++;														\
	};/*end if*/																	\
//end macro func

