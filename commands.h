// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
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
	token_pointer_arg4    = strtok(NULL                , " ");									\
																								\
	if (token_pointer_command != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg1    != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg2    != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg3    != NULL) number_of_user_inputs++ ;								\
	if (token_pointer_arg4    != NULL) number_of_user_inputs++ ;								\
																								\
	strncpy(user_input_command_string,       token_pointer_command, MAX_LENGTH_COMMAND - 1 ); 	\
	strncpy(user_input_arg1_string,   token_pointer_arg1   , MAX_LENGTH_ARGS    - 1 ); 			\
	strncpy(user_input_arg2_string,   token_pointer_arg2   , MAX_LENGTH_ARGS    - 1 ); 			\
	strncpy(user_input_arg3_string,   token_pointer_arg3   , MAX_LENGTH_ARGS    - 1 ); 			\
	strncpy(user_input_arg4_string,   token_pointer_arg4   , MAX_LENGTH_ARGS    - 1 ); 			\
																								\
	//do_alias(user_input_command_string);														\
																								\
	replace_characters(user_input_command_string, '\"', ' ');									\
	replace_characters(user_input_arg1_string,    '\"', ' ');									\
	replace_characters(user_input_arg2_string,    '\"', ' ');									\
	replace_characters(user_input_arg3_string,    '\"', ' ');									\
	replace_characters(user_input_arg4_string,    '\"', ' ');									\
																								\
	if (strlen(user_input_command_string) > 0) { user_input_command_type = 's'; };/*end if*/	\
	if (strlen(user_input_arg1_string)    > 0) { user_input_arg1_type    = 's'; };/*end if*/	\
	if (strlen(user_input_arg2_string)    > 0) { user_input_arg2_type    = 's'; };/*end if*/	\
	if (strlen(user_input_arg3_string)    > 0) { user_input_arg3_type    = 's'; };/*end if*/	\
	if (strlen(user_input_arg4_string)    > 0) { user_input_arg4_type    = 's'; };/*end if*/	\
																								\
	if (is_string_only_numbers(user_input_command_string) == 1) { user_input_command_type = 'n'; user_input_command_number = atoi(user_input_command_string); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg1_string)    == 1) { user_input_arg1_type    = 'n'; user_input_arg1_number    = atoi(user_input_arg1_string   ); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg2_string)    == 1) { user_input_arg2_type    = 'n'; user_input_arg2_number    = atoi(user_input_arg2_string   ); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg3_string)    == 1) { user_input_arg3_type    = 'n'; user_input_arg3_number    = atoi(user_input_arg3_string   ); };/*end if*/ \
	if (is_string_only_numbers(user_input_arg4_string)    == 1) { user_input_arg4_type    = 'n'; user_input_arg4_number    = atoi(user_input_arg4_string   ); };/*end if*/ \
//end macro func 


// unsigned char dir_file_total = 0;
// unsigned char current_dir_file_index = 0;

#define dir_file_count(total_files)											\
 																			\
	total_files = 0; 														\
 																			\
	result = cbm_opendir(1, dev);  	     									\
 																			\
	for (iii = 0; iii <= 254 ; iii++) { 									\
 		// printf("iii:%i\n", iii);											\
		if (result != 0) { 													\
			// printf("dir_file_count result != 0 --> %i\n",result); 			\
			break; 															\
		};/*end if*/  														\
 																			\
	    result = cbm_readdir(1, &dir_ent); 									\
 																			\
	    if (iii == 0) {  													\
			// printf("Start.\n"); 											\
 																			\
	    } else if (result == 2) {   										\
			// printf("End.\n"); 												\
 																			\
	    } else {  															\
			total_files++; 													\
 																			\
		};/*end if*/  														\
 																			\
	};/*end for*/  															\
 																			\
	cbm_closedir(1);	 													\
//end macro func 

#define dir_goto_file_index(file_index) 											\
 																					\
 																					\
	result = cbm_opendir(1, dev);  	     											\
 																					\
	for (jjj = 0; jjj <= file_index ; jjj++) {										\
 																					\
 																					\
	    result = cbm_readdir(1, &dir_ent); 											\
	    /*printf("%s\n", dir_ent.name); */											\
 																					\
	};/*end for*/  																	\
 																					\
	cbm_closedir(1);	 															\
//end macro func 



// ********************************************************************************
// COPY COMMAND 
// ********************************************************************************

#define copy()											  \
	strcpy (drive_command_string,"c0:"); 				  \
	strcat (drive_command_string,user_input_arg2_string); \
	strcat (drive_command_string,"=");					  \
	strcat (drive_command_string,user_input_arg1_string); \
														  \
	result = cbm_open(1, dev, 15, drive_command_string);  \
	cbm_close(1); 									      \
//end macro func 


// ********************************************************************************
// ACOPY COMMAND - Advanced Copy - Will Replace Copy 
// ********************************************************************************
#define acopy() \
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
	source_filename_pointer = &user_input_arg1_string[0]; /* update pointer --> user_input_arg1_string_length string */ \
	target_filename_pointer = &user_input_arg2_string[0]; /* update pointer --> user_input_arg1_string_length string */ \
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
	right_slash_position = (user_input_arg1_string_length - 1) - right_slash_index;			\
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
	\
	/* -------------------- BUILD STRING -------------------- */ \
	\
	strcpy (drive_command_string,"c0"); /* start copy command, not gonna deal with partitions for now */ \
	\
	if (target_path[0] == '/') { \
		strcat (drive_command_string,"/"); /* if it start with slash / then it's refering to the root folder and it needs two slashes // instead */ \
		strcat (drive_command_string,target_path); /* add the target path */ \
	                                                                               \
	} else if (target_path_length != 0) {  \
		strcat (drive_command_string,"/");    \
		strcat (drive_command_string,target_path); /* add the target path */ \
	                                                                               \
	};/*end_if*/ \
	                                                                               \
	strcat (drive_command_string,":"); /* finish the target path  */ \
	\
	if (target_filename_length == 0 ) { \
		strcat (drive_command_string,source_filename_pointer); /* add the SOURCE filename  becasuse it's inferred */ \
	} else {                                                                \
		strcat (drive_command_string,target_filename_pointer); /* add the target filename  */ \
	};/*end_if*/ \
	                                                                               \
	strcat (drive_command_string,"=");					   \
	                                                                               \
	if (source_path[0] == '/') { \
		strcat (drive_command_string,"/"); /* if it start with slash / then it's refering to the root folder and it needs two slashes // instead */ \
		strcat (drive_command_string,source_path); /* add the source path */ \
	                                                                               \
	} else if (source_path_length != 0) {  \
		strcat (drive_command_string,"/");   \
		strcat (drive_command_string,source_path); /* add the source path */ \
	                                                                               \
	};/*end_if*/ \
	                                                                               \
	strcat (drive_command_string,":"); /* finish the source path  */ \
	strcat (drive_command_string,source_filename_pointer); /* add the source filename  */ \
	printf("-->%s\n", drive_command_string); \
	                                                                               \
	if (copy_error_status == TRUE) {  \
		printf("ERR: File not found!\n");  \
	} else { \
		result = cbm_open(1, dev, 15, drive_command_string);   \
		cbm_close(1);  \
	};/*end_if*/ \
	\
	/* printf("2 tp:%s tf:%s\nsp:%s sf:%s\n", target_path, target_filename_pointer, source_path, source_filename_pointer); */ \
//end macro_func

// ********************************************************************************
// DCOPY COMMAND 
// ********************************************************************************

#define dcopy()											  												\
	                                                                                                    \
	switch (detected_filetype) {                                                                        \
		case  2 : /* bad */      break;	/* DIR */                                                       \
		case 16 : strcpy(detected_filetype_char,"s"); break; /* SEQ */                                  \
		case 17 : strcpy(detected_filetype_char,"p"); break; /* PRG */                                  \
		case 18 : strcpy(detected_filetype_char,"u"); break; /* USR */                                  \
		case 19 : strcpy(detected_filetype_char,"l"); break; /* REL */                                  \
		default : printf("???"); /* end default */                                                      \
	};/* end switch */                                                                                  \
                                                                                                        \
	switch (number_of_user_inputs) {                                                                    \
		case 3 : 				                                                                        \
			/* Setup SOURCE FILE for READING */                                                         \
			strcpy (drive_command_string, "");                                                          \
			strcat (drive_command_string, user_input_arg1_string); /*filename*/                         \
			strcat (drive_command_string, ",r,");                                                       \
			strcat (drive_command_string, detected_filetype_char); 				                        \
                                                                                                        \
			// printf("OPEN 6,%i,%i,%s\n", dev, CBM_READ, drive_command_string);		                \
                                                                                                        \
			result = cbm_open(6, dev, CBM_READ, drive_command_string);                                  \
                                                                                                        \
			/* Setup TARGET FILE for WRITING */                                                         \
			strcpy (drive_command_string2, "");                                                         \
			strcat (drive_command_string2, user_input_arg1_string); /*filename*/                        \
			strcat (drive_command_string2, ",w,");                                                      \
			strcat (drive_command_string2, detected_filetype_char); 			                        \
                                                                                                        \
			// printf("OPEN 7,%i,%i,%s\n", user_input_arg2_number, CBM_WRITE, drive_command_string2);	\
                                                                                                        \
			result2 = cbm_open(7, user_input_arg2_number, CBM_WRITE, drive_command_string2);            \
                                                                                                        \
			printf("-> %s [",user_input_arg1_string);                                                   \
                                                                                                        \
			memset(disk_sector_buffer,0,sizeof(disk_sector_buffer));                                    \
                                                                                                        \
			if (result == 0 && result2 == 0) {                                                          \
				do {                                                                                    \
					read_bytes = cbm_read(6, disk_sector_buffer, sizeof(disk_sector_buffer));           \
					result = cbm_write(7, disk_sector_buffer, read_bytes); 								\
					if ( (read_bytes == -1) || (result == -1) ) {										\
						printf("Copy error!");															\
						break;																			\
					};/*end_if*/																		\
					printf(".");																		\
					memset(disk_sector_buffer,0,sizeof(disk_sector_buffer)); 							\
				} while( read_bytes == sizeof(disk_sector_buffer) ); 									\
			};/* end if */ 																				\
																										\
			cbm_close (6);																				\
			cbm_close (7);																				\
																										\
			printf("]\n");																		        \
	    break;																							\
	    																								\
	    default : 																						\
	    	printf("Err args:%i\n", number_of_user_inputs);						                        \
	    /* end default */																				\
	};/* end switch */																					\
//end macro func 





// ********************************************************************************
// COMMANDS FUNCTIONS 
// ********************************************************************************

void type_text( unsigned char * file_to_type ) ;
void type_prg( unsigned char * file_to_type ) ;
void type_hex( unsigned char * file_to_type ) ;


