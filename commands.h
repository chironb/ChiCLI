// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************


// ********************************************************************************
// commands.h
// ********************************************************************************


// ********************************************************************************
// COMMANDS MACRO FUNCTIONS 
// ********************************************************************************

#define process_command() 																		\
																								\
	strip_newlines(entered_keystrokes);															\
	process_quotes(entered_keystrokes,user_input_processed); 									\
																								\
	/* THIS is to reverse some cc65 being clever ASCII to PETSCII bullshit fuck */				\
	replace_characters(user_input_processed, 160, ' '); 										\
																								\
	remove_extra_spaces(user_input_processed);													\
																								\
	token_pointer_command = strtok(user_input_processed, " ");									\
	token_pointer_arg1    = strtok(NULL                , " ");									\
	token_pointer_arg2    = strtok(NULL                , " ");									\
	token_pointer_arg3    = strtok(NULL                , " ");									\
																								\
	if (token_pointer_command != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg1    != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg2    != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg3    != NULL) number_of_user_inputs++ ;								\
																								\
	strncpy(user_input_command_string,       token_pointer_command, MAX_LENGTH_COMMAND - 1 ); 	\
	strncpy(user_input_arg1_string,   token_pointer_arg1   , MAX_LENGTH_ARGS    - 1 ); 			\
	strncpy(user_input_arg2_string,   token_pointer_arg2   , MAX_LENGTH_ARGS    - 1 ); 			\
	strncpy(user_input_arg3_string,   token_pointer_arg3   , MAX_LENGTH_ARGS    - 1 ); 			\
																								\
	replace_characters(user_input_command_string, '\"', ' ');									\
	replace_characters(user_input_arg1_string,    '\"', ' ');									\
	replace_characters(user_input_arg2_string,    '\"', ' ');									\
	replace_characters(user_input_arg3_string,    '\"', ' ');									\
																								\
	if (strlen(user_input_command_string) > 0) { user_input_command_type = 's'; };/*end if*/	\
	if (strlen(user_input_arg1_string)    > 0) { user_input_arg1_type    = 's'; };/*end if*/	\
	if (strlen(user_input_arg2_string)    > 0) { user_input_arg2_type    = 's'; };/*end if*/	\
	if (strlen(user_input_arg3_string)    > 0) { user_input_arg3_type    = 's'; };/*end if*/	\
																								\
	/* if (is_string_only_numbers(user_input_command_string) == 1) { user_input_command_type = 'n'; user_input_command_number = atoi(user_input_command_string); }; // <-- MAJOR CHANGE - Seems like this variable is never used. */ \
	if (is_string_only_numbers(user_input_arg1_string)    == 1) { user_input_arg1_type    = 'n'; user_input_arg1_number    = atoi(user_input_arg1_string   ); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg2_string)    == 1) { user_input_arg2_type    = 'n'; user_input_arg2_number    = atoi(user_input_arg2_string   ); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg3_string)    == 1) { user_input_arg3_type    = 'n'; user_input_arg3_number    = atoi(user_input_arg3_string   ); };/*end if*/ \
//end macro func 


// ********************************************************************************
// COMMANDS FUNCTIONS 
// ********************************************************************************

unsigned char dir_file_count(unsigned char * listing_string) ;
void dir_goto_file_index(unsigned char file_index) ;
void dcopy() ;
void acopy() ;

void type_text( unsigned char * file_to_type ) ;
void type_prg( unsigned char * file_to_type ) ;
void type_hex( unsigned char * file_to_type ) ;
