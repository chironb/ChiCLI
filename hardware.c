// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2021 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// hardware.c 
// ********************************************************************************

#include <peekpoke.h>
#include <string.h>
#include <6502.h>
#include <cbm.h>
#include <stdlib.h>
#include <stdio.h>
#include <device.h>
#include <unistd.h>
#include <conio.h>

#ifndef CHICLI_H
#define CHICLI_H
#include "chicli.h"
#endif 

#ifndef HARDWARE_H
#define HARDWARE_H
#include "hardware.h"
#endif 

#ifndef STRING_PROCESSING_H
#define STRING_PROCESSING_H
#include "string_processing.h"
#endif 


extern unsigned int  error_number      ;
extern unsigned char error_message[32] ;
extern unsigned char error_message2[32];
extern unsigned int  error_track       ;
extern unsigned int  error_block       ;

extern unsigned char result ;
extern unsigned char dev ;
extern unsigned char read_bytes ;
extern unsigned char disk_sector_buffer[255] ; // need to compile with "static lcoals" or you get the error "Error: Too many local variables" but apparently this doesn't work if you have any recursive code --> cl65 -g -Oi -t c64 --static-locals  chicli-v02.c
extern struct cbm_dirent dir_ent; 
extern unsigned int  number_of_files ;
extern unsigned char color_profiles[14][3];

extern unsigned char current_textcolor;  
extern unsigned char current_bgcolor;
extern unsigned char current_bordercolor;
extern unsigned char have_device_numbers_changed ; // False - the druive nubmers  haven't been change in software, True - they have, so don't drive detect
extern unsigned char i;

extern unsigned char  drive_detected[8];
extern unsigned char  drive_detected_type[8];

extern unsigned char files_per_screen;
extern unsigned char listing_string[10];
extern unsigned char par;
            
// ********************************************************************************
// PRIVATE VARIABLES
// ********************************************************************************

// unsigned char  cbm_error0_text[] = "Ok"; // "Ok";
// unsigned char  cbm_error1_text[] = "Too many files"; // "Too many files";
// unsigned char  cbm_error2_text[] = "File open"; // "File open";
// unsigned char  cbm_error3_text[] = "File not open"; // "File not open";
// unsigned char  cbm_error4_text[] = "File not found"; // "File?";
// unsigned char  cbm_error5_text[] = "Dev?";
// unsigned char  cbm_error6_text[] = "Not input-file"; // "!InFile";
// unsigned char  cbm_error7_text[] = "Not output-file"; // "!OutFile";
// unsigned char  cbm_error8_text[] = "File-name?";
// unsigned char  cbm_error9_text[] = "Bad dev#"; // "Bad dev#";
// unsigned char cbm_error10_text[] = "R/S hit"; // "R/S press";
// unsigned char cbm_error11_text[] = "ErI/O";


unsigned char  cbm_error0_text[] = "Ok";
unsigned char  cbm_error1_text[] = "Files++!";
unsigned char  cbm_error2_text[] = "Open?";
unsigned char  cbm_error3_text[] = "!Open?";
unsigned char  cbm_error4_text[] = "File?";
unsigned char  cbm_error5_text[] = "Dev?";
unsigned char  cbm_error6_text[] = "!InFile";
unsigned char  cbm_error7_text[] = "!OutFile";
unsigned char  cbm_error8_text[] = "Fname?";
unsigned char  cbm_error9_text[] = "Bad Dev#";
unsigned char cbm_error10_text[] = "R/S!";
unsigned char cbm_error11_text[] = "ErI/O!";

// NEW 	
// "Ok";		
// "Files++";	
// "Open?";		
// "!Open?";	
// "File?";		
// "Dev?";		
// "In-file";	
// "Out-file";	
// "Fname";		
// "Dev#";		
// "R/S";		
// "Er:I/O";	

// OLD // 
// "Success."; // 
// "Too many files."; // 
// "File open."; // 
// "File not open."; // 
// "File not found."; // 
// "Device not present."; // 
// "Not input-file."; // 
// "Not output-file."; // 
// "Missing file-name."; // 
// "Illegal device-number."; // 
// "RUN/STOP key pushed."; // 
// "General I/O-error."; // 

//TODO: I think this needs to be applied to the drives 12 through 15, and also, should be done in a way better way

// FORMAT FOR CHANGING DEVICE NUMBER:
// PRINT#file#, "M-W:" CHR$(119) CHR$(0) CHR$(2) CHR$(address + 32) CHR$(address+64)
// EXAMPLE OF CHANGING DEVICE NUMBER (FROM 8 TO 9):
// 10 OPEN 15, 8, 15
// 20 PRINT# 15, "M-W" CHR$(119) CHR$(0) CHR$(2) CHR$(9 + 32) CHR$(9 + 64)

char command_set[]         = { 'u' , '0' , '>' , 0x08 };

// const char cdrive_command_set[] = { 'm' , '-' , 'w' , ':', 119 , 0 , 2 };
// char cdrive_command_set_final[] = { 'x' , 'x' , 'x' , 'x' , 'x' , 'x' , 'x', 'x' };

// const char command_set8[]         = { 'u' , '0' , '>' , 0x08 };
// const char command_set9[]         = { 'u' , '0' , '>' , 0x09 };
// const char command_set10[]        = { 'u' , '0' , '>' , 0x0A };
// const char command_set11[]        = { 'u' , '0' , '>' , 0x0B };

// const char command_set12[]        = { 'u' , '0' , '>' , 0x0C };
// const char command_set13[]        = { 'u' , '0' , '>' , 0x0D };
// const char command_set14[]        = { 'u' , '0' , '>' , 0x0E };
// const char command_set15[]        = { 'u' , '0' , '>' , 0x0F };

// const char cdrive_command_set8[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  8+32 ,  8+64 }; // we need to this bypass cc65 remapping ascii to petscii
// const char cdrive_command_set9[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  9+32 ,  9+64 };
// const char cdrive_command_set10[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 10+32 , 10+64 };
// const char cdrive_command_set11[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 11+32 , 11+64 };
// const char cdrive_command_set12[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 12+32 , 12+64 };
// const char cdrive_command_set13[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 13+32 , 13+64 };
// const char cdrive_command_set14[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 14+32 , 14+64 };
// const char cdrive_command_set15[] = { 'm' , '-' , 'w' , 119 , 0 , 2 , 15+32 , 15+64 };

char cdrive_command_set[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  8+32 ,  8+64 }; // we need to this bypass cc65 remapping ascii to petscii


// ********************************************************************************
// HARDWARE FUNCTIONS 
// ********************************************************************************


void wait(unsigned int delay) {
	int w;
	for (w = 0 ; w <= delay ; ++w) {
		//do nothing 
    };//end for 
};//end function 


void wait_one_second(void) {
	// int w;
	// for (w = 0 ; w <= 60 ; ++w) {
 //    	waitvsync();
 //    };//end for 

	sleep(1);

};//end function 


void pet_chirp(void){

	unsigned int duration = 450;
	unsigned char waveform = 0x21; // 0x11=trianlge 0x21=sawtooth 0x41=pulse 0x81=noise
	unsigned char i = 0;

    POKE(0xd418,15); //filter and volume 
    POKE(0xd405,9); //attack duration 	decay duration voice 1 
    //POKE(0xd402,0xFF); //pulse wave duty cycle voice 1 low byte 



	// $d407 (54279) 	frequency voice 2 low byte
	// $d408 (54280) 	frequency voice 2 high byte 

    for (i = 1 ; i <= 4 ; ++i) {

		// B-6	1975.5	$82B9
		POKE(0xd401,0x82); // high           
		POKE(0xd400,0xB9); // low           
		POKE(0xd404,waveform);   // start note         
		wait(duration);    // duration  
		POKE(0xd404,16);   // end note   

		// B-5	987.8	$415C
		POKE(0xd401,0x41); // high   
		POKE(0xd400,0x5C); // low   
		POKE(0xd404,waveform);   // start note 
		wait(duration);    // duration 
		POKE(0xd404,16);   // end note 

		// B-4	493.9	$20AE
		POKE(0xd401,0x20); // high
		POKE(0xd400,0xAE); // low
		POKE(0xd404,waveform);   // start note 
		wait(duration);    // duration 
		POKE(0xd404,16);   // end note 

	    // B-3	246.9	$1057
		POKE(0xd401,0x10); // high
		POKE(0xd400,0x57); // low
		POKE(0xd404,waveform);   // start note 
		wait(duration);    // duration 
		POKE(0xd404,16);   // end note       

		// B-4	493.9	$20AE
		POKE(0xd401,0x20); // high
		POKE(0xd400,0xAE); // low
		POKE(0xd404,waveform);   // start note 
		wait(duration);    // duration 
		POKE(0xd404,16);   // end note 

		// B-5	987.8	$415C
		POKE(0xd401,0x41); // high   
		POKE(0xd400,0x5C); // low   
		POKE(0xd404,waveform);   // start note 
		wait(duration);    // duration 
		POKE(0xd404,16);   // end note 

		// B-6	1975.5	$82B9
		POKE(0xd401,0x82); // high           
		POKE(0xd400,0xB9); // low           
		POKE(0xd404,waveform);   // start note         
		wait(duration);    // duration  
		//POKE(0xd404,16);   // end note  // comment this out to leave sustain 

	};//end for 

	// // B-6	1975.5	$82B9
	// POKE(0xd401,0x82); // high           
	// POKE(0xd400,0xB9); // low           
	// POKE(0xd404,17);   // start note         
	wait(30000);    // duration  
	POKE(0xd404,16);   // end note 

	POKE(0xd418,0xFF); //filter and volume 
	POKE(0xd405,0xFF); //attack duration 	decay duration voice 1 
	POKE(0xd401,0xFF); // high   
	POKE(0xd400,0xFF); // low   
	POKE(0xd404,0xFF);   // start note 
	POKE(0xd404,0xFF);   // end note 

	POKE(0xd418,0x00); //filter and volume 
	POKE(0xd405,0x00); //attack duration 	decay duration voice 1 
	POKE(0xd401,0x00); // high   
	POKE(0xd400,0x00); // low   
	POKE(0xd404,0x00);   // start note 
	POKE(0xd404,0x00);   // end note 

};//end func 


// ********************************************************************************
// DETECTION FUNCTIONS
// ********************************************************************************

unsigned char detect_sid(void) {

	// **********
	// DETECT SID 
	// **********

	// Taken from the work published here:

	// https://codebase64.org/doku.php?id=base:detecting_sid_type_-_safe_method

	// ;SID DETECTION ROUTINE

	// 	;By SounDemon - Based on a tip from Dag Lem.
	// 	;Put together by FTC after SounDemons instructions
	// 	;...and tested by Rambones and Jeff.

	// 	; - Don't run this routine on a badline
	// 	; - Won't work in VICE (always detects 6581) unless resid-fp emulation is enabled

	// 	sei		;No disturbing interrupts
	// 	lda #$ff
	// 	cmp $d012	;Don't run it on a badline.
	// 	bne *-3

	// 	;Detection itself starts here	
	// 	lda #$ff	;Set frequency in voice 3 to $ffff 
	// 	sta $d412	;...and set testbit (other bits doesn't matter) in $d012 to disable oscillator
	// 	sta $d40e
	// 	sta $d40f
	// 	lda #$20	;Sawtooth wave and gatebit OFF to start oscillator again.
	// 	sta $d412
	// 	lda $d41b	;Accu now has different value depending on sid model (6581=3/8580=2)
	// 	lsr		;...that is: Carry flag is set for 6581, and clear for 8580.
	// 	bcc model_8580
	// model_6581:
	// 	[...]

	// model_8580:
	// 	[...]

	unsigned char i 						 = 0; // For looping

	unsigned char D41B_before  				 = 0;
	unsigned char D41B_after   				 = 0;

	unsigned char one_D41B_after_value       = 0;
	unsigned char one_D41B_after_frequency   = 0;
	unsigned char two_D41B_after_value       = 0;
	unsigned char two_D41B_after_frequency   = 0;
	unsigned char three_D41B_after_value     = 0;
	unsigned char three_D41B_after_frequency = 0;

	unsigned char likely_8580 				 = 0;
	unsigned char likely_6581 				 = 0;

    // if ( (PEEK(0xD400) != 0x00) && (PEEK(0xD401) != 0x00) ) return(0); // These locations will float if no SID is installed, and should be 0 if there is a SID.

	D41B_before = PEEK(0xD41B); // Record iniital value 

	for (i = 0 ; i < 255 ; ++i) {

		POKE(0xD011, 0x0B); 		// Load 0x0B into 0xD011(53265) - Disable VIC-II
		POKE(0xD412, 0xFF); 		// Load 0xFF into 0xD412   		- Set frequency in voice 3 to $ffff
		POKE(0xD40E, 0xFF); 		// Load 0xFF into 0xD40E   		- ...and set testbit (other bits doesn't matter) in $d012 to disable oscillator
		POKE(0xD40F, 0xFF); 		// Load 0xFF into 0xD40F
		POKE(0xD412, 0x20); 		// Load 0x20 into 0xD412        - Sawtooth wave and gatebit OFF to start oscillator again.
		POKE(0xD011, 0x1B); 		// Load 0x1B into 0xD011(53265) - Enable VIC-II
		D41B_after = PEEK(0xD41B);	// Record current value

		// check curent value to see if it's sotred
		if 			( D41B_after == one_D41B_after_value ) { 	// does teh current value equal slot one's value 
			++one_D41B_after_frequency;							// if so, update the count for this slot

		} else if 	( D41B_after == two_D41B_after_value ) { 	// does teh current value equal slot two's value 
			++two_D41B_after_frequency;							// if so, update the count for this slot

		} else if 	( D41B_after == three_D41B_after_value ) {  // does teh current value equal slot three's value 
			++three_D41B_after_frequency;						// if so, update the count for this slot

		// if it doesn't match any slots, try to allocate one
		} else if 	( one_D41B_after_frequency == 0 ) {			// if this slot one empty?
			one_D41B_after_value = D41B_after;					// if so, update the value for this slot and...
			++one_D41B_after_frequency;							// ...update the count for this slot 

		} else if 	( two_D41B_after_frequency == 0 ) {			// if this slot one empty?
			two_D41B_after_value = D41B_after;					// if so, update the value for this slot and...
			++two_D41B_after_frequency;							// ...update the count for this slot 

		} else if 	( three_D41B_after_frequency == 0 ) {		// if this slot one empty?
			three_D41B_after_value = D41B_after;				// if so, update the value for this slot and...
			++three_D41B_after_frequency;						// ...update the count for this slot 		

		} else {
			// DEBUGGING
			// printf("Something weird happened...\n");
			// printf("D41B_after:               H:0x%02X D:%u\n", D41B_after, D41B_after);
			// printf("Restarting...\n\n");

			//exit(EXIT_SUCCESS);
			D41B_after   			   = 0;	
			one_D41B_after_value       = 0;	
			one_D41B_after_frequency   = 0;	
			two_D41B_after_value       = 0;	
			two_D41B_after_frequency   = 0;
			three_D41B_after_value     = 0;	
			three_D41B_after_frequency = 0;
			i = 0;

		};//end if 

	};//end for 

	// DEBUGGING
	// printf ("D41B_before:               H:0x%02X D:%u\n",	D41B_before,				D41B_before					);
	// printf ("one_D41B_after_value:      H:0x%02X D:%u\n", 	one_D41B_after_value,     	one_D41B_after_value		);
	// printf ("one_D41B_after_frequency:  #:%u\n", 			one_D41B_after_frequency, 	one_D41B_after_frequency	);
	// printf ("two_D41B_after_value:      H:0x%02X D:%u\n", 	two_D41B_after_value,		two_D41B_after_value		);
	// printf ("two_D41B_after_frequency:  #:%u\n", 			two_D41B_after_frequency,	two_D41B_after_frequency	);
	// printf ("three_D41B_after_value:    H:0x%02X D:%u\n", 	three_D41B_after_value,		three_D41B_after_value		);
	// printf ("three_D41B_after_frequency:#:%u\n", 			three_D41B_after_frequency,	three_D41B_after_frequency	);	

	// Scan for 8580 --> 0x08 & any of 0x33 0x32
	if (one_D41B_after_value == 0x08 || two_D41B_after_value == 0x08 || three_D41B_after_value == 0x08) {
		++likely_8580;
	};//end if 

	if (one_D41B_after_value == 0x32 || two_D41B_after_value == 0x32 || three_D41B_after_value == 0x32) {
		++likely_8580;
	};//end if 

	// Scan for 6581 --> 0x09 & any of 0x34 0x33
	if (one_D41B_after_value == 0x09 || two_D41B_after_value == 0x09 || three_D41B_after_value == 0x09) {
		++likely_6581;
	};//end if

	if (one_D41B_after_value == 0x34 || two_D41B_after_value == 0x34 || three_D41B_after_value == 0x34) {
		++likely_6581;
	};//end if

	// Codes:
	// 1 - 6581
	// 2 - 8580
	// 0 - Undetermined
	if (likely_6581 > likely_8580) {
		return(1);
	} else if (likely_8580 > likely_6581) {
		return(2);
	} else {
		return(0);
	};// end if

};//end func


unsigned char detect_cpu(void) {

	unsigned char gotten_cpu = 0; 
	unsigned char sid_detected = 0;

	gotten_cpu = getcpu(); 
	sid_detected = detect_sid();

	// cc65 has a getcpu() function that only tells you if it's a 6502 or others, but not the Commodore specific ones 
	// Detect 6510/8500/8502 --> Address 0x00 should be 47, whereas regular 6502's seem to have 76 there
	// Detect SID to guess that it's a Commodore 64C or not 
	// Detect a Commodore 128 --> Address 0xD030 (53296) is always 255 on a non-Commodore 128 6502 CPU 

	// TODO: This can be faster and shorter.
	if (getcpu() == 0 ) { 
		if        (gotten_cpu == 0 && PEEK(0) == 47 && PEEK(0xD030)  != 255) { return(9 ); // 9   MOS 8502 (Commodore 128)
		} else if (gotten_cpu == 0 && PEEK(0) == 47 && sid_detected == 2  )  { return(10); // 10  MOS 8500 (Commodore 64C)
		} else if (gotten_cpu == 0 && PEEK(0) == 47 && sid_detected != 2  )  { return(11); // 11  MOS 6510 (Commodore 64)
		} else {                  return(0);						  				       // 0   MOS 6502 
		};//end if 
	} else if (gotten_cpu == 1 ) {return(1);  // 1   MOS 65C02    
	} else if (gotten_cpu == 2 ) {return(2);  // 2   MOS 65816    
	} else if (gotten_cpu == 3 ) {return(3);  // 3   MOS 4510     
	} else if (gotten_cpu == 4 ) {return(4);  // 4   MOS 65SC02   
	} else if (gotten_cpu == 5 ) {return(5);  // 5   MOS 65CE02   
	} else if (gotten_cpu == 6 ) {return(6);  // 6   MOS HUC6280  
	} else if (gotten_cpu == 7 ) {return(7);  // 7   MOS 2A0x     
	} else if (gotten_cpu == 8 ) {return(8);  // 8   MOS 45GS02  
	} else    				     {return(255);// 255 Unknown 
	};//end if 

};//end func


unsigned char detect_kernal(void) {

	// Taken from here: 
	// http://commodore64.se/wiki/index.php/Commodore_64_KERNAL_ROM_versions

	//  Format for list:

	//  Address: 901227-01 (Commodore 64 KERNAL R1,  $FF80 content $AA)
	//           901227-02 (Commodore 64 KERNAL R2,  $FF80 content $00)
	//           901227-03 (Commodore 64 KERNAL R3,  $FF80 content $03)
	//           ??????-?? (SX-64 or DX-64 KERNAL,   $FF80 content $43)
	//           ??????-?? (4064 PET 64 Educator 64, $FF80 content $64)

	// The KERNAL ROM R1 was obviously used only in early NTSC systems. It lacks the PAL/NTSC detection, 
	// and always uses white color while clearing the screen. 
	// The PAL/NTSC flag ($2A6) 

	// NTSC 0 PAL 1

    // kernal.sx.251104-04.bin 2018-03-07 8192
    //     This 8-kilobyte ROM is the Commodore SX-64 KERNAL, which is based on
    //     901227-03.

	unsigned char kernal_value = 0;
	kernal_value = PEEK(0xFF80); // 65408

			switch (kernal_value) {

				case 0xAA : return(1); break;		// 901227-01 (C64   KERNAL R1, $FF80=$AA 65408=170)

				case 0x00 : return(2); break;		// 901227-02 (C64   KERNAL R2, $FF80=$00 65408=  0)

				case 0x03 :							// 901227-03 (C64   KERNAL R3, $FF80=$03 65408=  3)
					// If KERNAL R3 and KERNAL start-up screen text is "MO" then it's a regular C64
					if (        PEEK(58497L)==77 && PEEK(58498L)==79 ) {
						return(3);
					} else if ( PEEK(58497L)==68 && PEEK(58498L)==79 && PEEK(58677L)==6 ) { // $E535: (default cursor colour) 901227-03: $0E (light blue) 251104-04: $06 (dark blue)
						return(11);
					// Else if JIFFYDOS!
					} else if ( PEEK(58497L)==68 && PEEK(58498L)==79 ) {
						return(10);
					// Else if JiffyDOS for SX-64!
					} else if ( PEEK(58497L)==83 && PEEK(58498L)==88 ) { // 251104-01 !!! Early SX-64 Kernal ROM!
						return(5);
					// Something weird, we shouldn't ever get this output.
					} else {
						return(255); 
					};//end-if
				break;

				case 0x43 : return(4); break;		// 251104-04 (SX-64 KERNAL R3, $FF80=$43 65408= 67)

				case 0x64 : return(6); break;		// 251104-04 (4064  KERNAL ??, $FF80=$64 65408=100)

			    default   : return(0); break;  	    // Error, don't know why we are getting this output 

			};//end switch 

}; // end func


unsigned char detect_ntsc_pal(void) {

	// Taken from here: 
	// http://commodore64.se/wiki/index.php/Commodore_64_KERNAL_ROM_versions

	//  Format for list:

	//  Address: 901227-01 (Commodore 64 KERNAL R1, $FF80 content $AA)
	//           901227-02 (Commodore 64 KERNAL R2, $FF80 content $00)
	//           901227-03 (Commodore 64 KERNAL R3, $FF80 content $03)
	//           ??????-?? (SX-64 or DX-64 KERNAL, $FF80 content $43)
	//           ??????-?? (4064 aka PET 64 aka Educator 64, $FF80 content $64)

	// The KERNAL ROM R1 was obviously used only in early NTSC systems. It lacks the PAL/NTSC detection, 
	// and always uses white color while clearing the screen. 
	// The PAL/NTSC flag ($2A6) 

	// NTSC 0 PAL 1

	unsigned char kernal_value = 0;
	unsigned char ntscpal_kernal_value = 0;
	kernal_value = PEEK(0xFF80);
	ntscpal_kernal_value = PEEK(0x02A6);

	switch (kernal_value) {

		case 0xAA : 		// 901227-01 (Commodore 64 KERNAL R1, $FF80 content $AA)	
			return(0);		// Return 0 for NTSC
		default :
			return(ntscpal_kernal_value); // Returns 0 for NTSC or 1 for PAL 
	};//end switch 


}; // end func


unsigned char detect_model(void) {

	unsigned char ntscpal_detected = 0;
	unsigned char sid_detected     = 0;
	unsigned char kernal_detected  = 0;
	unsigned char detected_cpu     = 0;
	unsigned char detected_1571    = 0;

	unsigned char result = 0;
	unsigned char disk_sector_buffer[40] = " "; //TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...
	unsigned char read_bytes = 0;


	// First, why am I doing this twice? This whole function is a waste and needs to be tighten up a lot 
	ntscpal_detected = detect_ntsc_pal() ;	
		sid_detected = detect_sid()      ;
	 kernal_detected = detect_kernal()   ;	
	    detected_cpu = detect_cpu()      ;


	

	// I think this below is freezing up the computer 
	// Drive 8
	// result = cbm_open(1, 8, 15, "uj");
	// if (result == 0) {
	// 	wait_one_second();
	// 	result = cbm_open(1, 8, 15, "\0");		                                                              
	// 	do {
	// 		read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));
	// 	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop
	// 	process_status(disk_sector_buffer); 
	// 	if (strcmp(error_message, "cbm dos v3.0 1571") == 0) {  // shoudl be "cbm dos v3.0 1571" but let's use "cbm dos v2.6 1541" for testing for now 
	// 		detected_1571 = 1;
	// 	};//end if 
	// 	cbm_close (1);
	// } else if (result == 5) {		
	// 	cbm_close (1);
	// } else {
	// 	cbm_close (1);
	// };//end if

//				case 0x64 : return(6); break;		// 251104-04 (4064 PET 64 Educator 64 KERNAL ??, $FF80 content $64)


	if        (kernal_detected == 4 || kernal_detected == 5 || kernal_detected == 11 ) { return(8); // Model: Commodore SX-64 251104-01 or 251104-04 Kernal ROM
	} else if (kernal_detected == 6)                                                   { return(9); // Model: Educator 64
	} else if (detected_cpu    == 9  && sid_detected == 1 && detected_1571   == 1)     { return(0); // Model: Commodore 128D       CPU: 8502 GPU: 8564 NTSC or 8566/69 PAL SID: 6581 Drive: Anything other than 1571 
	} else if (detected_cpu    == 9  && sid_detected == 1)                             { return(1); // Model: Commodore 128        CPU: 8502 GPU: 8564 NTSC or 8566/69 PAL SID: 6581 Drive: Must have 1571 
    } else if (detected_cpu    == 9  && sid_detected == 1)                             { return(2); // Model: Commodore 128DCR     CPU: 8502 GPU: 8564 NTSC or 8566/69 PAL SID: 8580 Drive: Must have 1571 
	} else if (ntscpal_detected == 0 && sid_detected == 1 && kernal_detected == 1)     { return(3); // Model: Commodore 64 (Early) \n"); if NTSC + 6581 + KERNAL R1 901227-01 --> C64 (Early) --> VIC-II 6567	
	} else if (ntscpal_detected == 0 && sid_detected == 1 )                            { return(4); // Model: Commodore 64         \n"         if NTSC + 6581 + KERNAL R2 901227-02 --> C64 		   --> VIC-II 6567
	} else if (ntscpal_detected == 0 && sid_detected == 2 )                            { return(5); // Model: Commodore 64C        \n"        if NTSC + 8580 + Any		 	       --> C64C 	   --> VIC-II 8562
	} else if (ntscpal_detected == 1 && sid_detected == 1 )                            { return(6); // Model: Commodore 64         \n"         if PAL  + 6581 + Any 			       --> C64 		   --> VIC-II 6569/6572/6573
	} else if (ntscpal_detected == 1 && sid_detected == 2 )                            { return(7); // Model: Commodore 64C        \n"        if PAL  + 8580 + Any		  		   --> C64C 	   --> VIC-II 8565
	};//end if 

};//end func


// ********************************************************************************
// DISK FUNCTIONS
// ********************************************************************************

void process_status(char * input_string) {   
	error_number  = atoi(strtok(input_string, ",")); 	// Extract the first token   
	strcpy(error_message,strtok(NULL, ",")        );	// Extract the second token
	error_track   = atoi(strtok(NULL, ",")        );    // Extract the third token
	error_block   = atoi(strtok(NULL, ",")        );	// Extract the fourth token
};//end func 


void display_cbm_error( unsigned char cbm_error_code ) {
	printf("CBM #%i: ", cbm_error_code);
	switch (cbm_error_code) {
		case  0 : cputs( cbm_error0_text); break;
		case  1 : cputs( cbm_error1_text); break;
		case  2 : cputs( cbm_error2_text); break;
		case  3 : cputs( cbm_error3_text); break;
		case  4 : cputs( cbm_error4_text); break;
		case  5 : cputs( cbm_error5_text); break;
		case  6 : cputs( cbm_error6_text); break;
		case  7 : cputs( cbm_error7_text); break;
		case  8 : cputs( cbm_error8_text); break;
		case  9 : cputs( cbm_error9_text); break;
		case 10 : cputs(cbm_error10_text); break;
		case 11 : cputs(cbm_error11_text); break;
		default : cputs("Err?");       //end default
	};//end switch	
	cputc('\n');
};//end func 









// #define set_drive_type(drive,type) drive_detected_type[drive-8] = type
// #define get_drive_type(drive) drive_detected_type[drive-8]
unsigned char detect_drive(unsigned char device_number, unsigned char display_status) {

	// if any drives have changed, set a dirty bit, and don't run any of the code below

	unsigned char cbm_result = 255; // we need to do this 

		// // reset all the drives
		// for (i == 8 ; i <= 15 ; i++) {
		// 	get_drive_type(i) = 0;
		// };//end-for

	set_drive_detection(device_number,0);
	set_drive_type(device_number,0);

	if (have_device_numbers_changed == TRUE) { 
		printf("Detect disabled.\n");
		return(254); //this means the detection never ran, because drives have changed
	};//end if 

	cbm_result = cbm_open(1, device_number, 15, "uj"); // reset the drive

	switch (cbm_result) { 
		case 0 : 
			set_drive_detection(device_number,1); // successfully detected
	    break;

		case 5 :
			set_drive_detection(device_number,2); // NOT detected // 0 == not scanned , 1 == detected , 2 == scanned but NOT detected
			//display_cbm_error(cbm_result);  
			//printf("Drive D%i is not detected.\n", device_number); // everything is okay, and we go the drive identity 
			cbm_close(1);
			return(255); // returns a code that means we didn't see teh drive 
	    break;

		default :
			set_drive_detection(device_number,0); // unknown failure; 0 == not successfully scanned either way 
	    //end default
	};//end switch


	#define DRIVE_RESET_WAIT_TIME 23000

	wait(DRIVE_RESET_WAIT_TIME);
	//wait_one_second(); // we need to wait a second for teh drive to reset itself. If we don't do this, the computer freezes up when we send the drive a command too soon.





	result = cbm_open(1, device_number, 15, "");
	// printf("");
	// printf("%i",result);
	
	
	// Why do we wait here? 
	// wait(DRIVE_RESET_WAIT_TIME);
	//wait_one_second();
	
	
	
	
	// do {

		// read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));
		// printf("after read_bytes:%i\n", read_bytes);
	// } while( read_bytes == sizeof(disk_sector_buffer) ); //end loop
	
	// for ( i = 0 ; i < 255 ; i++ ) {
		// printf("before read_bytes:%i\n", read_bytes);
		read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));
		// printf("B");
		// printf("after read_bytes:%i\n", read_bytes);
	// 	if ( read_bytes != sizeof(disk_sector_buffer) ) {
	// 		break;
	// 	};//end if 

	// };//end for 

	cbm_close(1);

	// printf("C");
	// gotox(wherex()-3);

	process_status(disk_sector_buffer); 	

	// printf("error_message:%s\n",error_message);
	// 
	// printf("error_message[ 0]:%i\n",error_message[ 0]);
	// printf("error_message[ 1]:%i\n",error_message[ 1]);
	// printf("error_message[13]:%i\n",error_message[13]);
	// printf("error_message[14]:%i\n",error_message[14]);
	// printf("error_message[20]:%i\n",error_message[20]);
	// printf("error_message[21]:%i\n",error_message[21]);

	// ********************
	// COMMODORE 1541
	// ********************
	if        ( matching( "cbm dos v2.6 1541", error_message) ) { 
		set_drive_type(device_number, DRIVE_1541);
		// strcpy(error_message2,"CBM DOS v2.6 1541");
		// strcpy(error_message,error_message2);
		strupper(error_message); // make the string upper case
		error_message[8] = 'v';


	// ********************
	// SD2IEC OLD FIRMWARE
	// ********************
	// We want to detec the beginning "uiec" so we know it's an SD2IEC, and maybe display "SD2IEC v0.11.3"
	// } else if ( matching( "uiec v0.11.3"      , error_message) ) {
	// Looks like "uiec v0.10.3" -->	v0.10.3 	sd2iec release version 0.10.3 	was the firmware that came with my SD2IEC.
	// Start displaying at position 6
	} else if ( (error_message[0]=='u' && \
 				 error_message[1]=='i' && \
 				 error_message[2]=='e' && \
 				 error_message[3]=='c'    ) ){
		set_drive_type(device_number, DRIVE_UIEC );
		strcpy(error_message2,"SD2IEC v");
		//output from position 6 until the end.
		string_add_character(error_message2,error_message[ 6]);
		string_add_character(error_message2,error_message[ 7]);
		string_add_character(error_message2,error_message[ 8]);
		string_add_character(error_message2,error_message[ 9]);
		string_add_character(error_message2,error_message[10]);
		string_add_character(error_message2,error_message[11]);
		// Final string:
		strcpy(error_message,error_message2);


	// ********************
	// SD2IEC NEW FIRMWARE
	// ********************
	// We want to detec the middle "ATENTDEAD" and only display something like: "SD2IEC v1.0.0 24"
	// Use position 8 to 12 (inclusive)
	// Use position 24, 25
	// Test 0,1 (SD) & 13,14 (AT) & 20,21 (AD)
	} else if ( (error_message[0] == 83  && \
 				 error_message[1] == 68  && \
 				 error_message[13]== 97  && \
 				 error_message[14]== 116 && \
 				 error_message[20]== 97  && \
 				 error_message[21]== 100    ) ){

		// } else if ( matching( "SD2IEC V1.0.0ATENTDEAD0-24", error_message) ) { 
		set_drive_type(device_number, DRIVE_UIEC);
		strcpy(error_message2,"SD2IEC v");
		string_add_character(error_message2,error_message[ 8]); // TODO: Dude... like... strncpy error_message+8
		string_add_character(error_message2,error_message[ 9]);
		string_add_character(error_message2,error_message[10]);
		string_add_character(error_message2,error_message[11]);
		string_add_character(error_message2,error_message[12]);
		// string_add_character(error_message2,' ');
		// string_add_character(error_message2,error_message[24]);
		// string_add_character(error_message2,error_message[25]);
		// Final string:
		strcpy(error_message,error_message2);


	// ********************
	// COMMODORE 1581
	// ********************
	} else if ( matching( "copyright cbm dos v10 1581", error_message) ) {
		set_drive_type(device_number, DRIVE_1581);
	    // strcpy(error_message,"CBM DOS v10 1581");
		// strcpy(error_message,error_message2);
		strncpy(error_message,error_message+10,17);
		strupper(error_message); // make the string upper case
		error_message[8] = 'v';


	// ********************
	// MSD SD-2
	// ********************
	} else if ( matching( "m.s.d. dos v2.3", error_message) ) {
		set_drive_type(device_number, DRIVE_SD2);
		// strcpy(error_message2,"M.S.D. DOS v2.3");
		// strcpy(error_message,error_message2);
		strupper(error_message); // make the string upper case
		error_message[11] = 'v';


	// ********************
	// COMMODORE 4040
	// ********************
	} else if ( matching( "cbm dos v2", error_message) ) {
		set_drive_type(device_number, DRIVE_4040);
		// strcpy(error_message2,"CBM DOS v2");
		// strcpy(error_message,error_message2);
		strupper(error_message); // make the string upper case
		error_message[8] = 'v';


	// ********************
	// COMMODORE 2031
	// ********************
	} else if ( matching( "cbm dos v2.6 2031", error_message) ) {
		set_drive_type(device_number, DRIVE_2031);
		// strcpy(error_message2,"CBM DOS v2.6 2031");
		// strcpy(error_message,error_message2);
		strupper(error_message); // make the string upper case
		error_message[8] = 'v';


	// ********************
	// COMMODORE SFD-1001
	// ********************
	} else if ( matching( "cbm dos v2.7", error_message) ) {
		set_drive_type(device_number, DRIVE_SFD1001);
		// strcpy(error_message2,"CBM DOS v2.7");
		// strcpy(error_message,error_message2);
		strupper(error_message); // make the string upper case
		error_message[8] = 'v';


	// ********************
	// UNKNOWN DRIVE
	// ********************
	} else {
		strupper(error_message); // make the string upper case


	};//end if 	



    // TODO: This should go into a section below, so we only cut it off at length 22 when it's being displayed as part of the sysinfo or title screen.
    error_message[22]='\0'; // GitHub Issue#???: Text too makes it look bad: SD2IEC V1.0.0ATENTDEAD0-24 - Testing: // strcpy(error_message,"SD2IEC V1.0.0ATENTDEAD0-24");

	if ( display_status == TRUE ) { // This is what runs when I call this from the command line.
		if ( result == 2 && error_number == 73 ) {
			printf("%i %s\n", device_number, error_message ); //  everything is okay, and we go the drive identity 
		} else {
			display_cbm_error(cbm_result); // yes, so do it 
			printf("Drv D%i Er:\n", device_number);
			printf("E#:%i TR:%i BL:%i\n", error_number, error_track , error_block );
			printf("ED:%s\n", error_message );
		};//end if 
	} else if ( display_status == 3 ) { // This is what runs when it gets called from within sysinfo. Special case. So like TRUE display it, FALSE, don't display it, 3 just for sysinfo
		// NEW! Mon Dec 13 505PM
		// This is to try and address the weird display of null strings with that stupid /6_O string.
		// TODO: This is a kludgy fix. The /6_O string.it whatever I see when there's some weird unuised or unformatted or wahtever string fuckl I hate this. It matches some text starting rigth at 0x0000 so WTF??? it's like a null pointer or somethign but it's not. Fuck fuck fuck.
		if ( result == 2 && error_number == 73 && strlen(error_message) != 4 ) {
			printf("%02i %s", device_number, error_message ); //  everything is okay, and we go the drive identity 
		};//end if 
	};//end if 

	return(cbm_result);

};//end func


unsigned char get_status(unsigned char device_number, unsigned char display_status) {

	if (have_device_numbers_changed == TRUE) { 
		printf("Detect disabled.\n");
		if (they_are_sure() == FALSE) {
			return(254);//
		};//end if 
	};//end if 

	if (get_drive_detection(device_number) == 0) { // 0 == not scanned , 1 == detected , 2 == scanned but NOT detected (and therefore do not attempt to read a status, which would lock up the Commodore 64)
		printf("Detecting...\n");
		detect_drive(device_number,display_status);
	};//endif 

	if (get_drive_detection(device_number) == 2) {
		printf("Err!\n");// Error not detected
		return(255);
	};//endif 

	result = cbm_open(1, device_number, 15, "");

	// printf("device_number:%u\n",device_number);
	// printf("result:%u\n",result);

	do {
		// printf("1? ");
		read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));
		// printf("2? ");
	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop

	process_status(disk_sector_buffer); 

	cbm_close(1);

	if ( display_status == TRUE ) {
	    printf("Device:%i ", device_number);
		if ( result == 0 && error_number == 0 ) {
			printf("Status ok.\n");
		} else {
			//printf("Device:\n", device_number);
			printf("\nE#:%i TR:%i BL:%i\nED:%s\n", error_number, error_track , error_block, error_message);
			//printf("ED:%s\n", error_message );
		};//end if 
	};//end if 

	return(error_number); // either there's no drive, and we return 255, or we just send what the drive sends us in error_number

};//end func


void change_drive(unsigned char device_number) {

	unsigned char device_result = 0;

	if (have_device_numbers_changed == FALSE) {

		device_result = get_status(device_number, TRUE);

		if (device_result != 255) {
			dev = getfirstdevice();
			while (dev != device_number) {			
				dev = getnextdevice(dev);			
			};//end while 
		};//end if 
		chdir(&dev);
	} else {
		printf("Drive detect disabled.\n"); // Detect disabled.
		dev = device_number; // TODO: there has to be a better, safer way.
	};//end if 

};//end func 



unsigned char detect_filetype(unsigned char * filename, unsigned char print_typefile) {

	// result = cbm_opendir(1, dev); // need to deal with errors here

	strcpy(listing_string,"$"); // Load directory "$", load drive 0 dir "$0", drive 1 "$1".

	if (get_drive_type(dev) == DRIVE_UIEC) {
		// printf("Detected SD2IEC!\n");
		string_add_character(listing_string,par+1); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
	} else {
		string_add_character(listing_string,par); // Add the current partition, or drive, for MSD SD-2 and 4040 support.
	};//end-if

	result = cbm_opendir(1, dev, listing_string); // need to deal with errors here

	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

		if (result != 0) {
			printf("Er cbm_o:%i\n", result);
			break;
		};//end if 

	    result = cbm_readdir(1, &dir_ent);

		if (number_of_files != 0 || number_of_files != 2 ) { //  0 dir_ent.name ==  DISK NAME  //  2 dir_ent.size == FREE DISK SPACE 

	    	if ( matching(dir_ent.name,filename) ) { // current file == filename we want

	    		if (print_typefile == TRUE) {

					switch (dir_ent.type) {
						case  2 : printf("DIR"); break;	// DIR
						case 16 : printf("SEQ"); break; // SEQ
						case 17 : printf("PRG"); break; // PRG
						case 18 : printf("USR"); break; // USR
						case 19 : printf("REL"); break;	// REL
						default : printf("???"); //end default
					};//end switch

					printf(" ");

					switch (dir_ent.access) {
						case CBM_A_RO : printf("R");   break; // R
						case CBM_A_WO : printf("W");   break; // W
						case CBM_A_RW : printf("R/W"); break; // R/W
						default       : printf("???"); //end default
					};//end switch

					printf("\n");

				};//end if 

				cbm_closedir(1);

				return(dir_ent.type); // break out of loop and exit function 

			};//end if 

		};//end if 

	};//end for 

	cbm_closedir(1); // if we made it this far, that means we never found the file.

	printf("Er: no file.\n");//file nto found  

	return(255); 

};//end func 





void c1541_set(unsigned char old_drive_number, unsigned char * new_drive_number) {

	// if this is running, set a global dirty byte to skip any drive detection at any point in the future
    
    // printf("This is test");//25 bytes over
    // printf("123456789012345678901234567890");//25 bytes over
	
	unsigned char new_drive_number_as_int;
	
	have_device_numbers_changed = TRUE; // ocne this is set, drive detect is disabled until the program ends 

	result = cbm_open(1, old_drive_number, 15, "");
	// 
	// printf("before: cdrive_command_set_final: %s size:%u \n",cdrive_command_set_final,sizeof(cdrive_command_set_final));
	// strcpy(cdrive_command_set_final, cdrive_command_set);
	// 

 //    switch( atoi(new_drive_number) ) { //{ 'm' , '-' , 'w' ,':' , 119 , 0 , 2 , 8+32 , 8+64 };
 //        case  8 : cdrive_command_set_final[6] = ( 8+32) ; cdrive_command_set_final[7] = ( 8+64); break;
 //        case  9 : cdrive_command_set_final[6] = ( 9+64) ; cdrive_command_set_final[7] = ( 9+64); break;
 //        case 10 : cdrive_command_set_final[6] = (10+64) ; cdrive_command_set_final[7] = (10+64); break;
 //        case 11 : cdrive_command_set_final[6] = (11+64) ; cdrive_command_set_final[7] = (11+64); break;
 //        case 12 : cdrive_command_set_final[6] = (12+64) ; cdrive_command_set_final[7] = (12+64); break;
 //        case 13 : cdrive_command_set_final[6] = (13+64) ; cdrive_command_set_final[7] = (13+64); break;
 //        case 14 : cdrive_command_set_final[6] = (14+64) ; cdrive_command_set_final[7] = (14+64); break;
 //        case 15 : cdrive_command_set_final[6] = (15+64) ; cdrive_command_set_final[7] = (15+64); break;
 //    };//end_switch
 //    printf("after: cdrive_command_set_final: %s size:%u \n",cdrive_command_set_final,sizeof(cdrive_command_set_final));
    // cbm_write(1, cdrive_command_set_final, sizeof(cdrive_command_set_final) );

    // switch( atoi(new_drive_number) ) {
    //     case 8  : cbm_write (1, cdrive_command_set8,  sizeof(cdrive_command_set8)  ); break;
    //     case 9  : cbm_write (1, cdrive_command_set9,  sizeof(cdrive_command_set9)  ); break;
    //     case 10 : cbm_write (1, cdrive_command_set10, sizeof(cdrive_command_set10) ); break;
    //     case 11 : cbm_write (1, cdrive_command_set11, sizeof(cdrive_command_set11) ); break;
    //     
    //     case 12 : cbm_write (1, cdrive_command_set12, sizeof(cdrive_command_set11) ); break;
    //     case 13 : cbm_write (1, cdrive_command_set13, sizeof(cdrive_command_set12) ); break;
    //     case 14 : cbm_write (1, cdrive_command_set14, sizeof(cdrive_command_set13) ); break;
    //     case 15 : cbm_write (1, cdrive_command_set15, sizeof(cdrive_command_set14) ); break;
    // };//end_switch


	// const char cdrive_command_set[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  8+32 ,  8+64 }; // we need to this bypass cc65 remapping ascii to petscii
	new_drive_number_as_int = atoi(new_drive_number);
	cdrive_command_set[6] = new_drive_number_as_int+32;
	cdrive_command_set[7] = new_drive_number_as_int+64;
	cbm_write (1, cdrive_command_set,  sizeof(cdrive_command_set)  );


	// if (strcmp(new_drive_number, "8") == 0 ) {
	// 	cbm_write (1, cdrive_command_set8, sizeof (cdrive_command_set8));
	// 	// new_drive_number = 8;
	// 	//strcat(drive_command_string, "`"); //this gets mapped 08 <-> 14
	// 	// printf("wtf9\n");
	// } else if (strcmp(new_drive_number, "9") == 0 ) {  
	// 	cbm_write (1, cdrive_command_set9, sizeof (cdrive_command_set9));
	// 	// new_drive_number = 9;
	// 	//strcat(drive_command_string, "\x09"); //no mapping 09 <-> 09
	// 	// printf("wtf9\n");
	// } else if (strcmp(new_drive_number, "10") == 0 ) { 
	// 	cbm_write (1, cdrive_command_set10, sizeof (cdrive_command_set10));
	// 	// new_drive_number = 10;
	// 	//strcat(drive_command_string, "\x0D");  //no mapping 0A <-> 0D
	// 	// printf("wtf10\n");
	// } else if (strcmp(new_drive_number, "11") == 0 ) {
	// 	cbm_write (1, cdrive_command_set11, sizeof (cdrive_command_set11));
	// 	// new_drive_number = 11;
	// 	//strcat(drive_command_string, "\x11");  //no mapping 0B <-> 11
	// 	// printf("wtf11\n");
	// };//end if 

	// printf("drive_command_string: %s\n", drive_command_string);

	// printf("Setting drive number... ");

	//result = cbm_open(1, drive_number, 15, drive_command_string);
	//result = cbm_open(1, drive_number, 15, user_input_arg2-48);
	cbm_close(1);

	if (result == 0) {
		//printf("cbm_open result: %i\n", result);
		printf("Drive set.\n");
	} else {
		printf("Er.\n");
	};//end if 
};//end func 





// TODO: Maybe this should all be a part of the drive-set section, instead of costly functions.
void uiec_set(unsigned char old_drive_number, unsigned char * new_drive_number) {


	have_device_numbers_changed = TRUE; // ocne this is set, drive detect is disabled until the program ends. 	// if this is running, set a global dirty byte to skip any drive detection at any point in the future


    switch( atoi(new_drive_number) ) {
        case  8 : command_set[3] = (0x08) ; break; // Are you fucking kidding me? I wrote this? WTF? I must have been too tired to think.
        case  9 : command_set[3] = (0x09) ; break;
        case 10 : command_set[3] = (0x0A) ; break;
        case 11 : command_set[3] = (0x0B) ; break;
        case 12 : command_set[3] = (0x0C) ; break;
        case 13 : command_set[3] = (0x0D) ; break;
        case 14 : command_set[3] = (0x0E) ; break;
        case 15 : command_set[3] = (0x0F) ; break;
    };//end_switch

	result = cbm_open(15, old_drive_number, 15, "");
    cbm_write(15, command_set, sizeof(command_set) );
	cbm_close(15);



 //    switch( atoi(new_drive_number) ) {
 //        case 8  : cbm_write (1, command_set8,  sizeof (command_set8)  ); break;
 //        case 9  : cbm_write (1, command_set9,  sizeof (command_set9)  ); break;
 //        case 10 : cbm_write (1, command_set10, sizeof (command_set10) ); break;
 //        case 11 : cbm_write (1, command_set11, sizeof (command_set11) ); break;
 //        
 //        case 12 : cbm_write (1, command_set12, sizeof (command_set12) ); break;
 //        case 13 : cbm_write (1, command_set13, sizeof (command_set13) ); break;
 //        case 14 : cbm_write (1, command_set14, sizeof (command_set14) ); break;
 //        case 15 : cbm_write (1, command_set15, sizeof (command_set15) ); break;
 //    };//end_switch

	// if (strcmp(new_drive_number, "8") == 0 ) {
	// 	cbm_write (1, command_set8, sizeof (command_set8));
	// 	// new_drive_number = 8;
	// 	//strcat(drive_command_string, "`"); //this gets mapped 08 <-> 14
	// 	// printf("wtf9\n");
	// } else if (strcmp(new_drive_number, "9") == 0 ) {  
	// 	cbm_write (1, command_set9, sizeof (command_set9));
	// 	// new_drive_number = 9;
	// 	//strcat(drive_command_string, "\x09"); //no mapping 09 <-> 09
	// 	// printf("wtf9\n");
	// } else if (strcmp(new_drive_number, "10") == 0 ) { 
	// 	cbm_write (1, command_set10, sizeof (command_set10));
	// 	// new_drive_number = 10;
	// 	//strcat(drive_command_string, "\x0D");  //no mapping 0A <-> 0D
	// 	// printf("wtf10\n");
	// } else if (strcmp(new_drive_number, "11") == 0 ) {
	// 	cbm_write (1, command_set11, sizeof (command_set11));
	// 	// new_drive_number = 11;
	// 	//strcat(drive_command_string, "\x11");  //no mapping 0B <-> 11
	// 	// printf("wtf11\n");
	// };//end if 

	// printf("drive_command_string: %s\n", drive_command_string);

	// printf("Setting drive number... ");

	//result = cbm_open(1, drive_number, 15, drive_command_string);
	//result = cbm_open(1, drive_number, 15, user_input_arg2-48);
	// cbm_close(1);

	if (result == 0) {
		//printf("cbm_open result: %i\n", result);
		printf("Drive set.\n");
	} else {
		printf("Unknown error.\n");
	};//end if 
};//end func 




// MSD SD-2
/************************************************************

From the MSD SD-2 Manual:

CHANGING THE DEVICE NUMBER BY SOFTWARE
The device number is changed by performing a MEMORY-WRITE to
locations $0077 and $0078. The MEMORY-WRITE command for changing
the device number is performed after the error channel has been
opened and has the following format:

PRINT#<file#>"M-W"CHR$(119)CHR$(0)CHR$(2)CHR$(dv+32)CHR$(dv+64)

The dv represents the device number that is desired. An example of
a simple routine for changing the device number to device number 9
is given below:

10 OPEN 15,8,15
20 PRINT#15,"M-W"CHR$(119)CHR$(0)CHR$(2)CHR$(9+32)CHR$(9+64)
30 END

It is usually desirable to change the device number in hardware
unless a temporary change is all that is desired. In order to use
the software method, only one drive can be powered on, its device
number changed, then another drive powered on and its device
number changed until all drives are on. If this procedure is not
followed, the disk drives will conflict with each other.

************************************************************/















void set_colors(unsigned char text, unsigned char background, unsigned char border) {
	textcolor(     text-1 );
	bgcolor( background-1 );
	bordercolor( border-1 );

    current_textcolor   = text-1 ;
    current_bgcolor     = background-1 ;
    current_bordercolor = border-1 ;

};//end func 

void set_profile_colors(unsigned char profile_to_set) {
	
	//unsigned char color_profiles[][] = {{14,01,01}, //  1 - C PET Style

	// need to offset because teh array starts at 0 but we ask teh user to start picking profiles taht start at 1

	set_colors( color_profiles[profile_to_set-1][0] , color_profiles[profile_to_set-1][1] , color_profiles[profile_to_set-1][2] );

};//end func 








