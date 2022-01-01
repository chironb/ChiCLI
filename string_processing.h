// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// string_processing.h
// ********************************************************************************

#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif

// ********************************************************************************
// STRING PROCESSING MACRO FUNCTIONS
// ********************************************************************************

#define matching(testing_string,original_string) (strcmp(original_string,testing_string)==0)


// ********************************************************************************
// STRING PROCESSING FUNCTIONS
// ********************************************************************************

unsigned char string_add_character ( unsigned char * input_string , unsigned char character_to_add );
unsigned char string_insert_character ( unsigned char * input_string , unsigned char character_to_insert , unsigned char position_to_insert );
unsigned char string_remove_character ( unsigned char * input_string );
unsigned char string_deinsert_character ( unsigned char * input_string , unsigned char position_to_remove );
unsigned char display_substring( unsigned char * string_to_display , unsigned char positon_to_start );
unsigned char display_blank_substring( unsigned char * string_to_display , unsigned char positon_to_start );
void          strip_newlines(unsigned char * input_string);
void          replace_characters(unsigned char * input_string , unsigned char character_to_find , unsigned char replace_with_character );
void          process_quotes(unsigned char * input_string , unsigned char * output_string);
void          remove_extra_spaces(unsigned char * input_string );
unsigned char is_string_only_numbers(unsigned char * input_string );
