// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// hotkeys.h
// ********************************************************************************





// ********************************************************************************
// HOTKEYS MACRO FUNCTIONS 
// ********************************************************************************

// #define set_hotkey(N,S) (strcpy(hotkeys_list[N-1],S))

#define set_hotkey(N,S) ( strncpy(hotkeys_list[N-1],S,(MAX_HOTKEY_LENGTH-1)) )

#define clear_hotkey(N) ( memset(hotkeys_list[N-1],'\0',sizeof(hotkeys_list[N-1])) )


#define clear_all_hotkeys()							       \
	(memset(hotkeys_list[0], 0, sizeof(hotkeys_list[0]))); \
	(memset(hotkeys_list[1], 0, sizeof(hotkeys_list[1]))); \
	(memset(hotkeys_list[2], 0, sizeof(hotkeys_list[2]))); \
	(memset(hotkeys_list[3], 0, sizeof(hotkeys_list[3]))); \
	(memset(hotkeys_list[4], 0, sizeof(hotkeys_list[4]))); \
	(memset(hotkeys_list[5], 0, sizeof(hotkeys_list[5]))); \
	(memset(hotkeys_list[6], 0, sizeof(hotkeys_list[6]))); \
	(memset(hotkeys_list[7], 0, sizeof(hotkeys_list[7]))); \
	(printf("All hotkeys cleared.\n"));					   \
//end macro func 


#define display_hotkeys() 										\
	(printf("F1 = %s\n", hotkeys_list[0]));						\
	(printf("F2 = %s\n", hotkeys_list[1]));						\
	(printf("F3 = %s\n", hotkeys_list[2]));						\
	(printf("F4 = %s\n", hotkeys_list[3]));						\
	(printf("F5 = %s\n", hotkeys_list[4]));						\
	(printf("F6 = %s\n", hotkeys_list[5]));						\
	(printf("F7 = %s\n", hotkeys_list[6]));						\
	(printf("F8 = %s\n", hotkeys_list[7]));						\
	printf("Maximum hotkey length:%u\nTotal hotkey slots:%u\n",	\
	        MAX_HOTKEY_LENGTH,MAX_HOTKEYS);						\
//end-macro-func


#define load_hotkey_command(N) 													\
	if (strlen(hotkeys_list[N-1]) > 0) {                                   		\
		strncpy(entered_keystrokes,hotkeys_list[N-1],MAX_HOTKEY_LENGTH-1); 	    \
	   	hotkeys_list[N-1][MAX_HOTKEY_LENGTH-1] = '\0'; /* WTF does this do? */  \
		replace_characters(entered_keystrokes, '\"', ' ');                		\
		replace_characters(entered_keystrokes, ';', '\"');                		\
		gotox(0);                                                               \
		while( wherex() != SCREEN_RIGHT_EDGE-1 ) cputc(' '); \
		gotox(0); \
		printf("> %s\n",entered_keystrokes);                                    \
	} else { 										                      		\
		gotox(0);                                    	    \
		gotoy(position_at_prompt_y);                                    	    \
		while( wherex() != SCREEN_RIGHT_EDGE-1 ) cputc(' '); \
		gotox(0);                                    	    \
		printf("> \nF%i is unset.\nUse 'hotkey' to set it.\n",N);              \
	}/*end if*/																	\
//end macro func

