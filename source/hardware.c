// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 - 2022 by: Chiron Bramberger
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
//extern unsigned char disk_sector_buffer[255] ; // need to compile with "static lcoals" or you get the error "Error: Too many local variables" but apparently this doesn't work if you have any recursive code --> cl65 -g -Oi -t c64 --static-locals  chicli-v02.c
extern unsigned char disk_sector_buffer[MAX_DISK_SECTOR_BUFFER];
extern struct cbm_dirent dir_ent; 
extern unsigned int  number_of_files ;
extern unsigned char color_profiles[14][3];

extern unsigned char current_textcolor;  
extern unsigned char current_bgcolor;
extern unsigned char current_bordercolor;
extern unsigned char have_device_numbers_changed ; // False - the druive nubmers  haven't been change in software, True - they have, so don't drive detect
extern unsigned char i;

extern unsigned char drive_detected[8];
extern unsigned char drive_detected_type[8];

extern unsigned char files_per_screen;
extern unsigned char listing_string[10];
extern unsigned char par;

extern unsigned char ntsc_status;

// ********************************************************************************
// PRIVATE VARIABLES
// ********************************************************************************



//TODO: I think this needs to be applied to the drives 12 through 15, and also, should be done in a way better way

// FORMAT FOR CHANGING DEVICE NUMBER:
// PRINT#file#, "M-W:" CHR$(119) CHR$(0) CHR$(2) CHR$(address + 32) CHR$(address+64)
// EXAMPLE OF CHANGING DEVICE NUMBER (FROM 8 TO 9):
// 10 OPEN 15, 8, 15
// 20 PRINT# 15, "M-W" CHR$(119) CHR$(0) CHR$(2) CHR$(9 + 32) CHR$(9 + 64)

// For the SD2IEC, CMD HD, 1570 and 1571
char command_set[]         = { 'u' , '0' , '>' , 0x08 };

// For the 1541 and MSD SD2
char cdrive_command_set[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  8+32 ,  8+64 }; // we need to this bypass cc65 remapping ascii to petscii


// ********************************************************************************
// HARDWARE FUNCTIONS
// ********************************************************************************

// Convert from partition or drive string to DOS ready string
// Valid partitions: a,b,c,d,e,f,g,h,i
// We "return" 255 if the input was out of range or invalid.
// 
// old --> convert_partition_string(user_input_arg1_string[0], par);
// new --> par = convert_partition_string(user_input_arg1_string[0]);
// 47843 - 47717 = 126 bytes!!!
unsigned char convert_partition_string(unsigned char user_input_partition) {
    switch(user_input_partition) {
    	case '/' : return('/');
        case 'a' : ;
        case 'b' : ;
        case 'c' : ;
        case 'd' : ;
        case 'e' : ;
        case 'f' : ;
        case 'g' : ;
        case 'h' : ;
        case 'i' : return (user_input_partition - 17); break;
        default  : return (255); break; /*INVALID PARTITION*/
    };/*end-switch*/
};//end-func

void wait(unsigned int delay) {
	int w;
	for (w = 0 ; w <= delay ; ++w) {
		//do nothing
	};//end for

	// TODO: Change this in the future
    // stopwatch_start_stamp = (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162)); //PRINT (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162))
    // stopwatch_difference  = ( PEEK(161)*256 + PEEK(162) ) - stopwatch_start_stamp; // 18 mins max roughly 

	// unsigned int wait_start;
	// unsigned int wait_difference = 0;

	// printf("WTF???\n");

	// wait_start = (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162)); //PRINT (PEEK(160)*65536)+(PEEK(161)*256+PEEK(162))
	// wait_difference = ( PEEK(161)*256 + PEEK(162) ) - wait_start;

	// while(wait_difference <= delay) {
	// 	wait_difference = ( PEEK(161)*256 + PEEK(162) ) - wait_start;
	// 	printf("wd:%u",wait_difference);
	// };//end for

};//end function


// Saved 7 bytes!
// void wait_one_second(void) {
// 	sleep(1);
// };//end function


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

	// 	sei		    ;No disturbing interrupts
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
		if 			( D41B_after == one_D41B_after_value ) { 	// does the current value equal slot one's value 
			++one_D41B_after_frequency;							// if so, update the count for this slot

		} else if 	( D41B_after == two_D41B_after_value ) { 	// does the current value equal slot two's value 
			++two_D41B_after_frequency;							// if so, update the count for this slot

		} else if 	( D41B_after == three_D41B_after_value ) {  // does the current value equal slot three's value 
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
			D41B_after   			   = 0;
			one_D41B_after_value       = 0;
			one_D41B_after_frequency   = 0;
			two_D41B_after_value       = 0;
			two_D41B_after_frequency   = 0;
			three_D41B_after_value     = 0;
			three_D41B_after_frequency = 0;

			// THIS IS WHAT HANGS THINGS!!! WHY DID I DO THIS???
			// i = 0;

		};//end if

	};//end for

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
	if 		  (likely_6581 > likely_8580) {
		return(1);
	} else if (likely_8580 > likely_6581) {
		return(2);
	} else {
		return(0);
	};// end if

};//end func


void peep( unsigned char high, unsigned char low ) {

	// Voice 1
	POKE(0xd401,high); // high
	POKE(0xd400,low);  // low
	POKE(0xd404,0x21); // start sawtooth note
	wait(450);         // duration
	POKE(0xd404,16);   // end note

};//end-func

void pet_chirp(void){

	unsigned char i = 0;

	// This is probably just for debugging. But if non-original Commodore SID chips cause problems, I might just have to do this until I can properly detect them.
	// if ( detect_sid()==0 ) return; // Don't try to play anything we can't detect a supported SID.

    POKE(0xd418,15); // filter and volume
    POKE(0xd405,9);  // attack duration / decay duration voice 1

	// NTSC frequency table
	// https://codebase64.org/doku.php?id=base:ntsc_frequency_table
	//
	// PAL frequency table
	// https://codebase64.org/doku.php?id=base:pal_frequency_table

    for (i = 1 ; i <= 4 ; ++i) {

		if (ntsc_status==TRUE) { // It's NTSC!!!!
			peep(0xFD, 0x40); 	 // B-6 $FD40
			peep(0x7E, 0xA0);    // B-5 $7EA0
			peep(0x3F, 0x50);    // B-4 $3F50
			peep(0x1F, 0xA8);    // B-3 $1FA8
			peep(0x3F, 0x50);    // B-4 $3F50
			peep(0x7E, 0xA0);    // B-5 $7EA0
			peep(0xFD, 0x40);    // B-6 $FD40
		} else { 			     // If *not* NTSC... so it's PAL!
			peep(0xFF, 0xFF);    // B-6 $FFFF
			peep(0x83, 0x71);    // B-5 $8371
			peep(0x41, 0xB8);    // B-4 $41B8
			peep(0x20, 0xDC);    // B-3 $20DC
			peep(0x41, 0xB8);    // B-4 $41B8
			peep(0x83, 0x71);    // B-5 $8371
			peep(0xFF, 0xFF);    // B-6 $FFFF // This is flat! God damn PAL!!! I can't make it higher... it's at the highest pitch for PAL! Every God damn thing all the God damn time!
		};//end-if

	};//end-for

	if (ntsc_status==TRUE) { // It's NTSC!!!!
		POKE(0xd405,8);  	 // attack duration / decay duration voice 1
		POKE(0xd401,0xFD); 	 // high // This is higher because you perceive notes as being flat the higher they go.
		POKE(0xd400,0xFF); 	 // low // This is called stretch tuning. It's a thing. My piano has this.
		POKE(0xd404,0x21); 	 // start note
	} else { 				 // If *not* NTSC... so it's PAL!
		POKE(0xd405,8);  	 // attack duration / decay duration voice 1
		POKE(0xd401,0xFF); 	 // high // Can't go higher! Stupid PAL!
		POKE(0xd400,0xFF); 	 // low
		POKE(0xd404,0x21); 	 // start note
	};//end-if

	wait(30000);       // duration
	POKE(0xd404,16);   // end note

	POKE(0xd418,0xFF); // filter and volume
	POKE(0xd405,0xFF); // attack duration 	decay duration voice 1
	POKE(0xd401,0xFF); // high
	POKE(0xd400,0xFF); // low
	POKE(0xd404,0xFF); // start note
	POKE(0xd404,0xFF); // end note

};//end func


unsigned char detect_cpu(void) {

	unsigned char gotten_cpu = 0; 
	unsigned char sid_detected = 0;

	gotten_cpu = getcpu();
	sid_detected = detect_sid();

	// cc65 has a getcpu() function that only tells you if it's a 6502 or others, but not the Commodore specific ones.
	// Detect 6510/8500/8502 --> Address 0x00 should be 47, whereas regular 6502's seem to have 76 there.
	// Detect SID to guess that it's a Commodore 64C or not.
	// Detect a Commodore 128 --> Address 0xD030 (53296) is always 255 on a non-Commodore 128 6502 CPU.

	// TODO: This can be faster and shorter.
	if (getcpu() == 0 ) {
		if        (gotten_cpu == 0 && PEEK(0) == 47 && PEEK(0xD030) != 255)  { return(9 ); // 9   MOS 8502 (Commodore 128)
		} else if (gotten_cpu == 0 && PEEK(0) == 47 && sid_detected == 2  )  { return(10); // 10  MOS 8500 (Commodore 64C)
		} else if (gotten_cpu == 0 && PEEK(0) == 47 && sid_detected == 1  )  { return(11); // 11  MOS 6510 (Commodore 64)
		} else {                  											   return(0);  // 0   MOS ?
		};//end if
	// Before: 47829
	// After:  47765
	// Saved:     64!
	// } else if (gotten_cpu == 1 ) {return(1);   // 1   MOS 65C02 // THIS WOULD NEVER BE IN A Commodore 64!!!
	// } else if (gotten_cpu == 2 ) {return(2);   // 2   MOS 65816
	// } else if (gotten_cpu == 3 ) {return(3);   // 3   MOS 4510
	// } else if (gotten_cpu == 4 ) {return(4);   // 4   MOS 65SC02
	// } else if (gotten_cpu == 5 ) {return(5);   // 5   MOS 65CE02
	// } else if (gotten_cpu == 6 ) {return(6);   // 6   MOS HUC6280
	// } else if (gotten_cpu == 7 ) {return(7);   // 7   MOS 2A0x
	// } else if (gotten_cpu == 8 ) {return(8);   // 8   MOS 45GS02
	} else    				     {return(255); // 255 Unknown
	};//end if

};//end func


unsigned char detect_kernal(void) {
	// I have discovered a *NEW* byte to look at in any kernal and figure out what it is. I need to re-write this to use this, becuase it'll save a bunch of code!

	// 	REVIEW - E4AC (58540) - Each Kernal:
	// - 901227-01 --> 0x2B
	// - 901227-02 --> 0x5C
	// - 901227-03 --> 0x81
	// - 251104-01 --> 0x00
	// - 251104-04 --> 0xB3
	// - 901246-01 --> 0x63

	// C64 R3 58497 _77_ && 58498 79 && 58677 14
	// SX  R3 58497 _83_ && 58498 88 && 58677 6
	// J64 R3 58497 _68_ && 58498 79 && 58677 14
	// JSX R3 58497 _68_ && 58498 79 && 58677 6

	unsigned char kernal_value = 0;
	kernal_value = PEEK(0xE4AC);

	switch (kernal_value) {
		case 0x2B : return(1); break;					// 901227-01 - R1 C64 First Kernal
		case 0x5C : return(2); break;					// 901227-02 - R2 C64 Early Kernal
		case 0x81 :
			switch ( PEEK(58497L) ) {
				case 68 :
					switch (PEEK(58677L)) {
						case 14 : return(10); break;	// R3 C64 JiffyDOS
						case  6 : return(11); break;	// R3 SX-64 JiffyDOS
					};//end-switch
				break;
			};//end-switch
			return(3); 									// 901227-03 - R3 C64 Kernal
		break;
		case 0xB3 : return(4); break;					// 251104-04 - R4 SX-64 Common Latest Kernal
		case 0x00 :	return(5); break;					// 251104-01 - R1 SX-64 Rare First Kernal
		case 0x63 : return(6); break;					// 901246-01 - 4064 Educator 64 --> ? Fair to say this is most similar to a stock R3 901227-03 kernal. From here: "Note that some patches of 901227-03 are included." http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/revisions.txt
	    default   : return(0); break;  					// Default Unknown Kernal
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
// TODO: !!!!!!!!!!!!!!! DELETE THE LINE BELOW! IT'S NOT USED AND IS STUPID!!!
// unsigned char disk_sector_buffer[40] = " "; // TODO: Look up how big this string can actually get in Commodore DOS / 1541 stuff...
	unsigned char read_bytes = 0;

	// First, why am I doing this twice? This whole function is a waste and needs to be tighten up a lot 
	 ntscpal_detected = detect_ntsc_pal() ;
	 	 sid_detected = detect_sid()      ;
	  kernal_detected = detect_kernal()   ;
 	     detected_cpu = detect_cpu()      ;

	if (detected_cpu == 9) {
		detect_drive(8, FALSE);
		if (get_drive_type(8)==DRIVE_1571) detected_1571=1; // If we've got a 1571 on device 8 then maybe we've got a C128D
	};//end-if

	// SID Return Values:		// CPU Return Values:
	// 1 - 6581 				// 9  - MOS 8502 Commodore 128
	// 2 - 8580 				// 10 - MOS 8500 Commodore 64C
	// 0 - Undetermined 		// 11 - MOS 6510 Commodore 64

	if        (kernal_detected  == 4 || kernal_detected == 5 || kernal_detected == 11) { return(8); // SX-64 251104-01 or 251104-04 Kernal ROM
	} else if (kernal_detected  == 6 )                                                 { return(9); // Educator 64
// TODO: THIS CANNOT BE 0 SINCE THERE"S NO ELS CONDITION!!!!! Need to rework! it's a SID problem!
	} else if (detected_cpu     == 9 &&    sid_detected == 1 &&   detected_1571 == 1)  { return(0); // 128D       CPU:8502 GPU:8564 NTSC or 8566/69 PAL SID:6581 Drive: Must have 1571
	} else if (detected_cpu     == 9 &&    sid_detected == 1 )                         { return(1); // 128        CPU:8502 GPU:8564 NTSC or 8566/69 PAL SID:6581 Drive: Anything other than 1571
    } else if (detected_cpu     == 9 &&    sid_detected == 2 )     					   { return(2); // 128DCR     CPU:8502 GPU:8564 NTSC or 8566/69 PAL SID:8580 Don't need to worry about the drive, because the DCR is the only machine with the 8502 CPU and 8580 SID.
	} else if (ntscpal_detected == 0 &&    sid_detected == 1 && kernal_detected == 1)  { return(3); // 64 (Early) NTSC + 6581 + KERNAL R1 901227-01 --> C64 (Early) --> VIC-II 6567	
	} else if (ntscpal_detected == 0 &&    sid_detected == 1 )                         { return(4); // 64         NTSC + 6581 + KERNAL R2 901227-02 --> C64 	    --> VIC-II 6567
	} else if (ntscpal_detected == 0 &&    sid_detected == 2 )                         { return(5); // 64C        NTSC + 8580 + Any		 	        --> C64C 	    --> VIC-II 8562
	} else if (ntscpal_detected == 1 &&    sid_detected == 1 )                         { return(6); // 64         PAL  + 6581 + Any 			    --> C64 		--> VIC-II 6569/6572/6573
	} else if (ntscpal_detected == 1 &&    sid_detected == 2 )                         { return(7); // 64C        PAL  + 8580 + Any		  		    --> C64C 	    --> VIC-II 8565
	} else 																			   { return(255); // Couldn't detect a specific model!
	};//end if

};//end func


// ********************************************************************************
// DISK FUNCTIONS
// ********************************************************************************

void process_status(char * input_string) {
	error_number  = atoi(strtok(input_string, ",")); 	// Extract the first token.
	strcpy(error_message,strtok(NULL, ",")        );	// Extract the second token.
	error_track   = atoi(strtok(NULL, ",")        );    // Extract the third token.
	error_block   = atoi(strtok(NULL, ",")        );	// Extract the fourth token.
};//end func


unsigned char detect_drive(unsigned char device_number, unsigned char display_status) {

	// if any drives have changed, set a dirty bit, and don't run any of the code below

	unsigned char cbm_result = 255; // we need to do this 

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

		case 5 : // NOT detected // 0 == not scanned , 1 == detected , 2 == scanned but NOT detected
			set_drive_detection(device_number,2);
			cbm_close(1);
			return(255); // returns a code that means we didn't see teh drive 
	    break;

		default :
			set_drive_detection(device_number,0); // unknown failure; 0 == not successfully scanned either way 
	    break;
	};//end switch


	// #define DRIVE_RESET_WAIT_TIME 23000
	#define DRIVE_RESET_WAIT_TIME 25000L // TODO: If this doesn't solve Raj's Pi1541 problem, remember to SWITCH IT BACK FOR SPEED!

	// TODO: I think this is where Leif was having a problem with his Ultimate whatever freezeing when running in 48MHz mode or whatever baloney it was doing.
	wait(DRIVE_RESET_WAIT_TIME); // we need to wait a second for teh drive to reset itself. If we don't do this, the computer freezes up when we send the drive a command too soon.
	// TODO: I also think this is where Raj was having a problem. I think the Pi1541 wasn't getting enough time to do a reset.
	// It sucks, but maybe I need to wait a full second? That's probably too much, but for now let's see if it solves the problem!

	// sleep(1); // This should wait 1 second. I hope cc65 is using Jiffies or something.
	// Balls. This just counts cycles dammit.
	// https://github.com/cc65/cc65/blob/master/libsrc/common/sleep.c

	result = cbm_open(1, device_number, 15, "");

	read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));

	cbm_close(1);

	process_status(disk_sector_buffer);

	// printf("em:%s\n",error_message);

	// ****************************************
	// COMMODORE 1541 - IEC
	// ****************************************
	if        ( matching( "cbm dos v2.6 1541", error_message) ) {
		set_drive_type(device_number, DRIVE_1541);

	// ****************************************
	// COMMODORE 1541 - JiffyDOS - IEC
	// ****************************************
	} else if ( matching( "jiffydos 5.0 1541", error_message) ) {
	 	set_drive_type(device_number, DRIVE_1541);

	// ****************************************
	// SD2IEC OLD FIRMWARE - IEC
	// ****************************************
	// We want to detec the beginning "uiec" so we know it's an SD2IEC, and maybe display "SD2IEC v0.11.3"
	// } else if ( matching( "uiec v0.11.3"      , error_message) ) {
	// Looks like "uiec v0.10.3" -->	v0.10.3 	sd2iec release version 0.10.3 	was the firmware that came with my SD2IEC.
	// Start displaying at position 6
	} else if ( (error_message[0]=='u' && \
 				 error_message[1]=='i' && \
 				 error_message[2]=='e' && \
 				 error_message[3]=='c' ) ){
		set_drive_type(device_number, DRIVE_UIEC );
		strcpy(error_message2,"SD2IEC v");
		string_add_character(error_message2,error_message[ 6]); //output from position 6 until the end.
		string_add_character(error_message2,error_message[ 7]);
		string_add_character(error_message2,error_message[ 8]);
		string_add_character(error_message2,error_message[ 9]);
		string_add_character(error_message2,error_message[10]);
		string_add_character(error_message2,error_message[11]);
		strcpy(error_message,error_message2); // Final string

	// ****************************************
	// SD2IEC NEW FIRMWARE - IEC
	// ****************************************
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

		set_drive_type(device_number, DRIVE_UIEC);
		strcpy(error_message2,"SD2IEC v");
		string_add_character(error_message2,error_message[ 8]); // TODO: Dude... like... strncpy error_message+8
		string_add_character(error_message2,error_message[ 9]);
		string_add_character(error_message2,error_message[10]);
		string_add_character(error_message2,error_message[11]);
		string_add_character(error_message2,error_message[12]);

		strcpy(error_message,error_message2); // Final string

	// ****************************************
	// VICE FS DRIVER 2.0 - IEC
	// ****************************************
	} else if ( matching( "vice fs driver v2.0", error_message) ) {
		set_drive_type(device_number, DRIVE_VICEFS);

	// ****************************************
	// COMMODORE 1570 - IEC
	// ****************************************
	} else if ( matching( "cbm dos v3.0 1570", error_message) ) {
		set_drive_type(device_number, DRIVE_1570);

	// ****************************************
	// COMMODORE 1570 - JiffyDOS - IEC
	// ****************************************
	// } else if ( matching( "cbm dos v3.0 1570", error_message) ) { // I can't find this. I don't think one was made??? Maybe it uses the 1541?
	// 	set_drive_type(device_number, DRIVE_1570);

	// ****************************************
	// COMMODORE 1571 - IEC
	// ****************************************
	} else if ( matching( "cbm dos v3.0 1571", error_message) ) {
		set_drive_type(device_number, DRIVE_1571);

	// ****************************************
	// COMMODORE 1571 - JiffyDOS - IEC
	// ****************************************
	} else if ( matching( "jiffydos 6.0 1571", error_message) ) {
		set_drive_type(device_number, DRIVE_1571);
		// strncpy(error_message,error_message+10,16);
		// error_message[16] = '\0';

	// ****************************************
	// COMMODORE 1581 - IEC
	// ****************************************
	} else if ( matching( "copyright cbm dos v10 1581", error_message) ) {
		set_drive_type(device_number, DRIVE_1581);
		strncpy(error_message,error_message+10,16);
		error_message[16] = '\0';

	// ****************************************
	// COMMODORE 1581 - JIFFYDOS - IEC
	// ****************************************
	} else if ( matching( "(c) 1989 jiffydos 6.0 1581", error_message) ) {
		set_drive_type(device_number, DRIVE_1581);
		strncpy(error_message,error_message+9,17);
		error_message[17] = '\0';  // "JiffyDOS 6.0 1581"

	// ****************************************
	// CMD HD - IEC
	// ****************************************
	} else if ( matching( "cmd hd dos v1.92", error_message) ) {
		set_drive_type(device_number, DRIVE_CMDHD);

	// ****************************************
	// COMMODORE CMD FD-2000 / FD-4000 - IEC
	// ****************************************
	} else if ( matching( "cmd fd dos v1.40", error_message) ) {
		set_drive_type(device_number, DRIVE_FD2000); 				// Note: This is currently just using DRIVE_CMDHD because all these drives should work in exactly the same way.

	// ****************************************
	// MSD SD-2 - IEC *OR* IEEE-488
	// ****************************************
	} else if ( matching( "m.s.d. dos v2.3", error_message) ) {
		set_drive_type(device_number, DRIVE_SD2);

	// ****************************************
	// MSD SD-2 - JiffyDOS - IEC *OR* IEEE-488
	// ****************************************
	// TODO: Maybe I should order a ROM from Jim for this drive.

	// ****************************************
	// COMMODORE 2031 - IEEE-488
	// ****************************************
	} else if ( matching( "cbm dos v2.6 2031", error_message) ) {
		set_drive_type(device_number, DRIVE_2031);

	// ****************************************
	// COMMODORE SFD-1001 - IEEE-488
	// ****************************************
	} else if ( matching( "cbm dos v2.7", error_message) ) {
		set_drive_type(device_number, DRIVE_SFD1001);

	// ****************************************
	// COMMODORE 4040 - IEEE-488
	// ****************************************
	} else if ( matching( "cbm dos v2", error_message) ) {
		set_drive_type(device_number, DRIVE_4040);

	// ****************************************
	// UNKNOWN DRIVE
	// ****************************************
	} else {
		// Do nothing.

	};//end if

	strupper(error_message);

    // TODO: This should go into a section below, so we only cut it off at length 22 when it's being displayed as part of the sysinfo or title screen.
    error_message[22]='\0'; // GitHub Issue#???: Text too makes it look bad: SD2IEC V1.0.0ATENTDEAD0-24 - Testing: // strcpy(error_message,"SD2IEC V1.0.0ATENTDEAD0-24");

	if ( display_status == TRUE ) { // This is what runs when I call this from the command line.
		if ( result == 2 && error_number == 73 ) {
			printf("%i %s\n", device_number, error_message ); //  everything is okay, and we go the drive identity 
		} else {
			// display_cbm_error(cbm_result); // yes, so do it
			// Removing the function altogetehr: 
			// BEFORE: 47955 bytes
			// AFTER:  47788 bytes
			// SAVED:    177 bytes!
			printf("CBM #%i: \n", cbm_result);
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

	do {
		read_bytes = cbm_read(1, disk_sector_buffer, sizeof(disk_sector_buffer));
	} while( read_bytes == sizeof(disk_sector_buffer) ); //end loop

	process_status(disk_sector_buffer); 

	cbm_close(1);

	if ( display_status == TRUE ) {
	    printf("Device:%i ", device_number);
		if ( result == 0 && error_number == 0 ) {
			printf("Status ok.\n");
		} else {
			printf("\nE#:%i TR:%i BL:%i\nED:%s\n", error_number, error_track , error_block, error_message);
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
		chdir(&dev); // This actually changes to the device.
	} else {
		printf("Drive detect disabled.\n"); // Detect disabled.
		dev = device_number; // TODO: there has to be a better, safer way.
	};//end if 

};//end func 


unsigned char detect_filetype(unsigned char * filename, unsigned char print_typefile) {

	listing_string[0] = '$'; // This is the short version of the DOS command

	if (get_drive_type(dev)==DRIVE_UIEC || get_drive_type(dev)==DRIVE_CMDHD) {
	    listing_string[1] = par+1;
	} else if (get_drive_type(dev)==DRIVE_1581 || get_drive_type(dev)==DRIVE_VICEFS) {
		listing_string[1] = '0';
	} else {
		listing_string[1] = par;
	};//end-if

	// listing_string[2] = convert_partition_for_drive(); // need to export this function or move it to this hardware.c module.
	listing_string[3] = '\0';

	result = cbm_opendir(1, dev, listing_string); // need to deal with errors here

	for (number_of_files = 0; number_of_files <= 255 ; number_of_files++) {

		if (result != 0 && print_typefile==TRUE) {
			printf("Er cbm_o:%i\n", result);
			break;
		};//end if

	    result = cbm_readdir(1, &dir_ent);

		if (number_of_files != 0 || number_of_files != 2 ) { //  0 dir_ent.name ==  DISK NAME  //  2 dir_ent.size == FREE DISK SPACE 

	    	if ( matching(dir_ent.name,filename) ) { // current file == filename we want

	    		if (print_typefile == TRUE) {

					switch (dir_ent.type) {
						case  1 : printf("CBM"); break;	// DIR
						case  2 : printf("DIR"); break;	// DIR
						case 16 : printf("SEQ"); break; // SEQ
						case 17 : printf("PRG"); break; // PRG
						case 18 : printf("USR"); break; // USR
						case 19 : printf("REL"); break;	// REL
						// TODO: Detect "CBM" type here for 1581 support!!!
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

	if (print_typefile==TRUE) {
		printf("Er no file\n");//file nto found
	};//end if

	return(255);

};//end func


void c1541_set(unsigned char old_drive_number, unsigned char * new_drive_number) {

	// if this is running, set a global dirty byte to skip any drive detection at any point in the future

	unsigned char new_drive_number_as_int;

	have_device_numbers_changed = TRUE; // ocne this is set, drive detect is disabled until the program ends 

	result = cbm_open(1, old_drive_number, 15, "");

	// const char cdrive_command_set[]  = { 'm' , '-' , 'w' , 119 , 0 , 2 ,  8+32 ,  8+64 }; // we need to this bypass cc65 remapping ascii to petscii
	new_drive_number_as_int = atoi(new_drive_number);
	cdrive_command_set[6] = new_drive_number_as_int+32;
	cdrive_command_set[7] = new_drive_number_as_int+64;
	cbm_write (1, cdrive_command_set,  sizeof(cdrive_command_set)  );

	cbm_close(1);

	if (result == 0) {
		printf("Drive set.\n");
	} else {
		printf("Er.\n");
	};//end if
};//end func


// MSD SD-2 --> SAME AS 1541!
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


// TODO: Maybe this should all be a part of the drive-set section, instead of costly functions.
void uiec_set(unsigned char old_drive_number, unsigned char * new_drive_number) {

	have_device_numbers_changed = TRUE; // ocne this is set, drive detect is disabled until the program ends. 	// if this is running, set a global dirty byte to skip any drive detection at any point in the future

	command_set[3] = atoi(new_drive_number); // Please tell me there isn't some stupid reason this *DOESN'T* work!

	result = cbm_open(15, old_drive_number, 15, "");
    cbm_write(15, command_set, sizeof(command_set) );
	cbm_close(15);

	if (result == 0) {
		printf("Drive set.\n");
	} else {
		printf("?\n");
	};//end if

};//end func


void set_colors(unsigned char text, unsigned char background, unsigned char border) {
	textcolor(     text-1 );
	bgcolor( background-1 );
	bordercolor( border-1 );
    current_textcolor   = text-1 ;
    current_bgcolor     = background-1 ;
    current_bordercolor = border-1 ;
};//end func


void set_profile_colors(unsigned char profile_to_set) {

	set_colors( color_profiles[profile_to_set-1][0] , color_profiles[profile_to_set-1][1] , color_profiles[profile_to_set-1][2] ); 	// need to offset because teh array starts at 0 but we ask teh user to start picking profiles taht start at 1

};//end func
