// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// hotkeys.c
// ********************************************************************************

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#ifndef HOTKEYS_H
#define HOTKEYS_H
#include "hotkeys.h"
#endif

#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif 

#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H
#include "string_processing.h"
#endif 

extern unsigned char hotkeys_list[MAX_HOTKEYS][MAX_HOTKEY_LENGTH];
extern unsigned char i; // shared by all for loops
extern unsigned char entered_keystrokes[MAX_ENTERED_KEYSTROKES] ;
extern unsigned char position_at_prompt_y;

// ********************************************************************************
// HOTKEYS MACRO FUNCTIONS
// ********************************************************************************

void set_hotkey( unsigned char hotkey_number , unsigned char *actual_command) {
	strncpy(hotkeys_list[hotkey_number-1],actual_command,(MAX_HOTKEY_LENGTH-1));
};//end-func


void clear_hotkey(unsigned char N) {
	memset(hotkeys_list[N-1],0,sizeof(hotkeys_list[N-1]));
};//end-func


void clear_all_hotkeys() {
	for (i = 0 ; i <= 7 ; i++) {
		memset(hotkeys_list[i], 0, sizeof(hotkeys_list[i]));
	};//end-for
	printf("All hotkeys cleared.\n");
};//end-func


void display_hotkeys() {
	for (i = 0 ; i <= 7 ; i++) {
		printf("F%u = %s\n", i+1, hotkeys_list[i]);
	};//end-for
	printf("Total hotkey slots:%u\n",MAX_HOTKEYS);
};//end-func


void load_hotkey_command(unsigned char N) {
	if (strlen(hotkeys_list[N-1]) > 0) {
		strncpy(entered_keystrokes,hotkeys_list[N-1],MAX_HOTKEY_LENGTH-1);
	   	hotkeys_list[N-1][MAX_HOTKEY_LENGTH-1] = '\0'; /* WTF does this do? */
		replace_characters(entered_keystrokes, '\"', ' ');
		replace_characters(entered_keystrokes, ';', '\"');
		gotox(0);
		while( wherex() != SCREEN_RIGHT_EDGE-1 ) cputc(' ');
		gotox(0);
		printf("> %s\n",entered_keystrokes);
	} else {
		gotox(0);
		gotoy(position_at_prompt_y);
		while( wherex() != SCREEN_RIGHT_EDGE-1 ) cputc(' ');
		gotox(0);
		printf("> \nF%i is unset.\nUse 'hotkey' to set it.\n",N);
	}/*end if*/
};//end-func
