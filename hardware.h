// ********************************************************************************
//
// ChiCLI - Chiron's CLI for 8-Bit Commodore Computers
// (c) 2020 by: Chiron Bramberger
//
// ********************************************************************************

// ********************************************************************************
// hardware.h 
// ********************************************************************************

// ********************************************************************************
// COMMODORE 64 COLORS
// ********************************************************************************

#define BLACK        1-1
#define WHITE        2-1
#define RED          3-1
#define CYAN         4-1
#define PURPLE       5-1
#define GREEN        6-1
#define BLUE         7-1
#define YELLOW       8-1
#define ORANGE       9-1
#define DARK_ORANGE 10-1
#define BROWN       10-1
#define LIGHT_RED   11-1
#define PINK        11-1
#define DARK_GREY   12-1
#define DARK_GRAY   12-1
#define GREY        13-1
#define GRAY        13-1
#define LIGHT_GREEN 14-1
#define LIGHT_BLUE  15-1
#define LIGHT_GREY  16-1
#define LIGHT_GRAY  16-1

// ********************************************************************************
// HARDWARE MACRO FUNCTIONS 
// ********************************************************************************

#define set_drive_detection(drive,detected) drive_detected[drive-8] = detected
#define get_drive_detection(drive) drive_detected[drive-8]
#define set_drive_type(drive,type) drive_detected_type[drive-8] = type
#define get_drive_type(drive) drive_detected_type[drive-8]

// DRIVE TYPE MACROS
// Commodore IEEE Floppy
#define    DRIVE_2031 0x31 // *****
#define    DRIVE_2040 0x20 
#define    DRIVE_2041 0x21	 
#define    DRIVE_3040 0x30 
#define    DRIVE_4031 0x31 
#define    DRIVE_4040 0x44 // ?????
#define DRIVE_SFD1001 0x01 // *****
#define    DRIVE_8050 0x50	 
#define    DRIVE_8060 0x60 
#define    DRIVE_8061 0x61	 
#define    DRIVE_8250 0x50 
#define    DRIVE_8280 0x80 

// Commodore Serial Floppy 
#define    DRIVE_1540 0x40 
#define    DRIVE_1541 0x41 // *****
#define    DRIVE_1542 0x42 
#define    DRIVE_1551 0x51 // (SFS481)
#define    DRIVE_1563 0x63 
#define    DRIVE_1570 0x70 
#define    DRIVE_1571 0x71 
#define    DRIVE_1572 0x72  
#define    DRIVE_1581 0x81 
   
// Commodore Hard Drive 
#define    drive_9060 0x96 
#define    drive_9090 0x99 
 
// CMD Hard Drives 
#define DRIVE_HD_40   0xC4 
#define DRIVE_HD_170  0xC7 
#define DRIVE_HD_340  0xC3 
#define DRIVE_HD_500  0xC5 
#define DRIVE_HD_1000 0xC1 
#define DRIVE_HD_2000 0xC2 

// Modern SD Card Drives
#define  DRIVE_UIEC   0xEC // *****
#define  DRIVE_PI1541 0x1E // !!!!!

// DRIVE TYPE STRINGS
#define DRIVE_2031_NAME    "C 2031" // removed Commodore from these
#define DRIVE_SFD1001_NAME "C SFD 1001"
#define DRIVE_1541_NAME    "C 1541"
#define DRIVE_UIEC_NAME    "uIEC"
#define DRIVE_PI1541_NAME  "Pi1541"


// Things shoudl work liek this: 
// when you run detect-drive, it udpates teh system to knwo what drive is in what drvice # slot
// when you run change-drive or whatrebtr, it udpates teh system to know what deivce # is what
// when command run,. they check this string, to know what commands are supported and what drives are active
// this keep sthings fast
// the expectation is that you're not gonna change the physcial hardware and then not run drive-detect
// also we can check if drive-detect has ever been run and if not run it, whcih would update things
// the idea of protecting the situation wher you change the hardware then runa  command without detect-drive is too much
// running drive-detect before every command would be too slow (we need 1 sec after the drive reboots itself) 


// ********************************************************************************
// HARDWARE FUNCTIONS 
// ********************************************************************************

void wait(unsigned int delay);
void wait_one_second(void);
void process_status(char * input_string);
void pet_chirp(void);

unsigned char detect_sid(void) ;
unsigned char detect_cpu(void) ;
unsigned char detect_kernal(void) ;
unsigned char detect_ntsc_pal(void) ; 
unsigned char detect_model(void) ; 

void process_status(char * input_string);
unsigned char get_status(unsigned char device_number, unsigned char display_status);
unsigned char detect_drive(unsigned char device_number, unsigned char display_status);
void change_drive(unsigned char device_number);
void display_cbm_error( unsigned char cbm_error_code );
unsigned char detect_filetype(unsigned char * filename, unsigned char print_typefile);
void c1541_set(unsigned char old_drive_number, unsigned char * new_drive_number);
void uiec_set(unsigned char old_drive_number, unsigned char * new_drive_number);
void set_colors(unsigned char text, unsigned char background, unsigned char border);
void set_profile_colors(unsigned char profile_to_set);





