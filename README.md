# ChiCLI

A CLI for Commodore Computers!

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_R2_screenshot.png?raw=true)

ChiCLI v1.R2 (c) 2021 Chiron Bramberger

A Commodore 64 command line interface terminal, similar to and inspired by: AmigaDOS, Commodore DOS, MS-DOS, and Linux.

ChiCLI - Features List:

- Engage your Commodore 64 using a terminal or command line interface familiar to modern computers.
- Get a system informational overview of your Commodore 64 at startup.
- View and manage files and floppies on any drive: type, run, rename, copy, delete, format, validate, and initialize.
- Copy all files within a folder or disk to another device using the * wildcard. Use this to extract an entire disk image to a folder or floppy disk on supported device.
- Change drive device number of 1541 drives and SD2IEC devices.
- Advanced file management on supported devices: make directory, remove directory, rename directories, mount disk images, extract files from disk images and copy them to a folder on the same device, view and access partitions, and detect device type.
- View date and time on SD2IEC, view file date and time stamps, set the time and date on RTC supported devices. Show the time and if wanted date on a built-in screen saver.
- Change and save settings on an SD2IEC, such as device number and whether to hide or show file extensions.
- View memory, peek and poke memory, repeatedly peek at memory, and save memory ranges as a file. Can be used to dump your Commodore 64 kernal ROM to a file.
- Execute "sys" or other dos commands easily.
- The Turbo Macro Pro shortcut and be used with the built-in "run" command and it's "-t" option to load and execute Turbo Macro Pro quickly and easily. For example just type: "run tmp -t".
- Show key codes for keys and combinations of key presses on the keyboard.
- Do basic maths and convert numbers to other format like decimal, hex and binary numbers.
- Use a simple stopwatch using the command line or hot key F5.
- Create custom shortcuts using aliases and hotkeys.
- Change color profiles or set your own custom color scheme.
- Use the built-in alias "help" to view the included manual which contains an overview of all the commands and features.

Made in Canada. :-)

You can download a disk image with this program and it's supporting Commodore readable text files here: 
https://github.com/chironb/ChiCLI/raw/main/chicli-disk.d64

This is release-candidate quality software. Regardless, it comes with no warranty or guarantees of any kind.

Testing successfully with the following hardware:
- Commodore 64 based on NTSC mainboards: 326298, 250407, 250425, 250466, 250469, Kernal's 2 & 3
- The Commodore SX-64
- TheC64 Mini hardware, which is based on Linux and VICE
- VICE: Emulated Commodore machines: 64, 64C, SX-64, 4064, NTSC, PAL
- SD2IEC by Jim Brain --> Get One Here --> http://store.go4retro.com/uiec-sd/
- IDE64 tested by Leif Bloomquist --> Leif's GitHub --> https://github.com/LeifBloomquist
- Commodore 1541 Disk Drive with stock ROM
- Commodore 2031 via a Batteries Included IEEE-488 BusCardII
- Commodore SFD 1001 via a Batteries Included IEEE-488 BusCardII
- VICE: Emulated Commodore drives: 1541, 1541-II, 1570, 1571
- Commodore 64 and Commodore 1541 with JiffyDOS installed in both
- Commodore 64 with Epyx Fast Load and Super Snapshot 4
- Commodore Flyer Internet Modem --> http://www.retroswitch.com/products/flyer/

Latest News:
v1.R2 - Release Candidate 2 - Dec 1 2021
- Optimized: *PACKS -EVEN MORE- IN A SINGLE APPLICATION!* Even more optimizations!
- Added:     Support for reading RTC from an RTC upgraded SD2IEC!!!
- Added:     Support for setting RTC.
- Added:     The filedate command, which allows you to view the time stamp of a given file name.
- Updated:   The datetime command uses the RTC from the SD2IEC exculsively, even if it doesn't have an RTC, as the SD2IEC uses a software clock instead.
- Updated:   The datetime command is seprate from the separate date and time commands.
- Updated:   Screensaver to use RTC time. Wasn't needed - Worked automatically!
- Updated:   Make logo show nothing instead of time if there’s no SD2IEC.
- Updated:   Support for checking if RTC exists. If it doesn't in any way, everything is handled graciously.
- Added:     A list-partitions command, which shows the names adn letters of available partitions on the device.
- Updated:   You can use the cd command to change drives and partitons!
- Updated:   Abilty to abort a multi-file copy by pressing CTRL-C or RUNSTOP.
- Updated:   Abilty to abort a multi-file delete by pressing CTRL-C or RUNSTOP.
- Added:     Created dump-mem command which let's you save a file of memory.
- Updated:   The dump-mem command has shortcuts so it's easy to dump your BASIC or kernal ROMs.
- Updated:   Change all uiec references to SD2IEC.
- Fixed:     Surpressed crap characters from showing when using run without arguments.
- Fixed:     Change sys-info to sysinfo to fit my Amiga muscle memory.
- Fixed:     Don't do general emulation detection, just VICE and TheC64/Mini. This fixes a false positive emulation detection on real hardware.
- Fixed:     Properly detect the SX-64 using either of it's two stock kernals, or the JiffyDOS kernal.
- Fixed:     When loading JiffyDOS, it shows as a R3 kernal, but then, there was nothing showing for OS. Now it shows JiffyDOS text for OS.
- Fixed:     Detects when JiffyDOS is installed for both regular and SX Commodore 64's!
- Optimized: Saved 53 bytes! --> Remove Header text from BASIC display and all of footer to save RAM.
- Optimized: Saved 5,358 bytes!!! --> Removed current cc65 date and time code and time includes. Huge savings!!!

v1.RC - Release Candidate 1 - Oct 25 2021
- Added:     A brand new stylized rainbow 64 logo!
- Updated:   The startup and "sysinfo" command screen now has more room for detected hardware.
- Updated:   The hardware detection now identifies the earliest C64's.
- Updated:   Made the drive detection faster, so starting up the program is faster.
- Added:     Option to the "run" command "-t" which automatically loads and runs Turbo Macro Pro.
- Updated:   The command "run" automatically loads program into memory using ",1".
- Updated:   Added the list of colors and profiles to the help file instead of built into the program. This saves memory.
- Updated:   The "profile-set" and "color-set" so they take their options on the command line instead of asking questions. This saves memory. 

v0.23 - Aug 28 2021
- Optimized: *PACKS MORE IN A SINGLE APPLICATION!* The previous version was already completely filling up memory. Every new feature required saving as many bytes as possible. As a result, much of the program is optimized for space and often faster as well!
- Added:     *DETECTS EMULATORS!* The "sysinfo" can now detect if the machine is running on actual Commodore hardware, or under emulation! It is listed like this: "Model: TheC64   C64" or "Model: VICE Emu C64" or "Model: VICE Emu CSX-64" or "Model: Emulated C64"
- Updated:   *QUICK FORMAT* The "format" command now supports the Commodore DOS ability to do a quick format, instead of a full format. Use format -q diskname to quick format a disk calling it diskname.
- Updated:   *AUTO-COMPLETE IS WAY FASTER!* I tested it in a long directory, and it went from 70 seconds, to about 3.5 - around 1750% faster!
- Updated:   *TWO-COLUMN LIST!* The "list" command now has a shortened two column mode. This fits more files on the screen at the same time!
- Added:     *EXTRACT D64 IMAGES!* Extract files on a single SD2IEC device and single SD card! Major new functionality to the "copy" command to support copying files from one partition to another within a given device. This lets you mount a D64 disk image on one partition, and extract the files to a folder on another partition. Example: "copy * d09b:" copies all the files in the current partition to the second partition.
- Added:     A new command called "pwd" to show the current device number and partition letter.
- Added:     A command called "partition" to quickly switch the current partition on a device. Example: "partition a" or "part b" or part 0" to switch to a default partition when drives don't support partitions.
- Added:     Commands and support for partitions. There are 9 partitions supported, as letters: "a" through "i". The command "d09a:" changes to the first partition on device 9, "d09b:" to the second. Partition 0 is a special setting that's needed for drives that don't support partitions like the 1541. 
- Added:     New "stopwatch" command that starts when you run it, and when run again, stops and outputs the elapsed seconds since it was last called. It resets on the second call as well. The maximum time is about 18 mins.
- Added:     New maths command called "=" which lets you do very simple 2-term integer calculations. Examples: "= 2 + 2" "= 2 - 2" "= 4 * 4" "= 8 / 2"
- Added:     Command "dec2bin" to convert from decimal to binary. Use "dec2bin 124" to get BIN:01111100
- Added:     Commands "hex2dec" and "dec2hex" to convert numbers between decimal and hexidecial. Use it like this: hex2dec 8000 and get this: HEX:8000 DEC:32768. Use dec2hex 32768 to get DEC:32768 HEX:8000.
- Added:     New "poke-bit" command to make it easier to change just a single bit in memory. Example: poke-bit 1024 1 0 would set the second least significant bit of memory location 1024 to 0.
- Added:     New "peek-bit" command to make it easier to get the value of a single bit in memory. Example: peek-bit 1024 1 outputs the value of the second least significant bit at decimal memory location 1024. 
- Added:     New "peeking" command for constantly reading memory and displaying the output. This is good for watching a constantly changing memory location, like the userport or paddle input.
- Updated:   The "peek" and "poke" commands now also display the value in binary.
- Updated:   The "view-mem" command now does ranges, or defaults to moving forward indefinitly. It also lets you press CURSOR-UP to go back a page.
- Updated:   The "format" command so that just typing "format" alone will use the file name "untitled" by default. Also, "format diskname" without an ID, will default to "0".
- Updated:   The "hotkey" command when removing a defined alias is done like this: "hotkey do-it -clear"
- Updated:   The "alias" command when removing a defined alias is done like this: "alias do-it -clear" 
- Updated:   The command "uiec-save" from "uiec-save-cfg" removing redundant text.
- Updated:   The command hotkeys how shows the PETSCII code in addition to the decimal and hexidecimal. This might change or break the layout temporarily if the PETSCII is for changing colours or cursor position.
- Updated:   The command to run Commodore DOS commands is now just "dos".
- Updated:   Combined version, about, and licence into one command showing all that information at once.
- Updated:   Updated "datetime -set" so that it reminds you to put set the hour in 24-hour format. For example 1:00 PM is 13.
- Updated:   Added "Ready!" to the opening prompt.
- Updated:   Restored a hotkey for listing directories using F3.
- Updated:   When you type help, it automatically tries to type chicli-readme instead of telling you to do that.
- Updated:   Have added additonal testing of other drives using the VICE emulator to my testing routine. And by routine I mean stuff I remeber to try and see if it looks borked.
- Updated:   The status text for the Commdoore 1581 drive returns: "COPYRIGHT CBM DOS V10 1581" which doesn't match the 1541, 1570, or 1571 drive text. I'm finding and replacing this text with "CBM DOS V10 1581". Anyone who wants to see the original text can use 'dos-command' to manually spit it out. 
- Updated:   The start-up screen uses leading zeros for D08: and D09: to align the text better. The program was also updated to support using d8: or d08: and d9: or d09: where that applies.
- Updated:   Auto-complete uses CTRL+LEFT-ARROW and LEFT-ARROW emits the left-arrow character. This is more inituitive and user friendly.
- Restored:  Undocumented commands time, drive-detect, detect-filetype and display_logo, chirp, etc...
- Fixed:     The new fast auto-complete feature had a bug where it would return the disk or folder label depending on the situation.
- Fixed:     A bug in the command "list" where if you used RUN/STOP or CTRL+C to stop a listing, the top-most line displaying a file name would be scrolled up off the screen. So you might miss it. Instead, it now doesn't do that, and the prmopt replaces the text saying to hit any key to continue.
- Fixed:     A bug in the command "alias" where, for example, if you made an alias like this: " alias $ "list -s" ", and then made another like this: " alias $ list ", then there would be two alias created with the same name. Now it updates the existing alias with the new replacement text as one would expect it to.
- Fixed:     A bug in the command "view-mem" where things got wonky after the first screen. Also a bunch of other fixes.
- Fixed:     So many little fixes I can't remember them all! The alias and hotkey functionality is much improved and more consistent.
- Fixed:     A bug in displaying larger files using the "list" command. It now displays the file sizes correctly in kilobytes. The maximum filesize reported is about 16000 KB but on the SD2IEC the files are still the correct size as stored on the SD card.
- Fixed:     GitHub Issue #1: https://github.com/chironb/ChiCLI/issues/1 - Long strings sent from the drive would break the layout of the opening screen. This is better now! It cuts off anything that's too long. 

v0.18 - Dec 15 2020 17:33:55
- Updated:   Command type has the added option -text to force it display the file as if it were a text file. Now type can auto-detect the filetype and display it accordingly, or it can be put into -hex mode or -text mode. 
- Added:     A new shortcut alias rn for rename, and a new shortcut cler for clear. This is because if you type fast on a Commodore keyboard, certain key combinations tend to get lost, because the keyboard doesn't have any anti-ghosting diodes. For example pressing and holding R and then E blocks E for registering. Pressing and holding E then A blocks A. 
- Updated:   Made a little more room in RAM for type update, by re-writing the CBM error codes text to be shorter but mostly as informative. 
- Fixed:     Run command now supported running on any drives. NOTE: The command ./ does not, but that's because it's intended for a different purpose, basically letting you run commands that are small and external as if they were built into the system. BUG: It can only load files from the drive it was loaded from. Running programs from a different drive issue. Also, Exomizer version works inconsistently with loading and running files from within ChiCLI. Not sure why, probably needs some tweaking in the way exomizer is configured in terms of memory layout. I think the fix for one is the fix for both, which is to use the dracopy method for loading external programs. 
- Fixed:     If you set the date and time more than once, it gets screwy. Fix: needed to reset to zero the time offset variable in set_date();
- Fixed:     Displays both Aliases empty and aliases full. Process: start ChiCLI, set datetime, run screensaver, then alias (with none set). Fix was that result, a global var, wasn't being cleared before being used in the alias code. So it was assumed, erroniously, that it would be 0 unless changed by the testing for the alias list being full. This is the problem with global variables, obviously, but in such a limited system, it's faster and makes more sense to use them. 
- Fixed:     Doing the command 'type basic-program' would print out minus signs weird. Turns out, I entered another unicode character instead of a normal keyboard hyphen - character. I made a basic program to barf out every single command so I can check that I'm translating the tokens properly.
- Added:     The alias ss = screensaver to built-in aliases. 
- Fixed:     Command screensaver now respects the currently set text color as well. 
- Fixed:     Command sysinfo wasn't using the current text color. Bug in color-set was fixed, where the global car tracking the current text color is now updated properly. 
- Fixed:     When displaying a Basic file using type, it makes the Basic commands in uppercase, but the screen is in lower case mode, so it should be in lowercase. 
- Fixed:     When displaying a Basic file using type, case 174 : printf("↑") wouldn't work, because I'm using a unicode character in the cc65 source code. Fixed with printf("%c", 94 ) instead, which sends the correct PETSCII character. 
- Fixed:     Remaining free disk space is often reported wrong and too small. It needed a long int.
- Fixed:     File sizes reported are sometimes wonky. Was using 1024 instead of 1000 to divide into kilobytes. 
- Fixed:     When exiting to dos, you need to enter NEW before loading anything 
- Fixed:     Need to inform the user when they can't set another alias because all the slots are full
- Fixed:     Setting a background color to red screws up the red part of displaying the Commodore logo on sysinfo.
- Fixed:     Figured out how to make colors and color profiles consistent and retained after calling an external program and returning back into ChiCLI using the -sf option. 
- Optimized: Converted last aliases to built-in, saving migs and megs of RAMs, and freeing up slots for users. There are 8 free now. 
- Added:     Updated readme's with a list of the built-in common aliases.
- Fixed:     Copy path bugs. Now you can do this: --> copy sauce tartar  --> Copies "sauce" in the current folder to another file in the current folder called "tartar" --> copy sauce /tartar --> Copies "sauce" in the current folder to another file in the root folder called "tartar" --> copy sourcedir/sauce targetdir/tartar --> Copies "sauce" in the current folder's folder called sourcedir to the current folder's folder called targetdir named "tartar"
- Updated:   Copy now works with paths. You can now do things like copy * //allfiles/ or copy //somefile //somedir/newfilename! You must end folder with a slash / as it doesn't yet automatically figure out if you mean copy the file to this filename, or copy this file to this folder name using the original implied filename. 
- Added:     The most common alias are built into the system, so they don't take up alias slots. However, there's only 8 now, at least until I squeeze out some more ram. 
- Updated:   The list command takes up one less screenline when displaying.
- Updated:   Tab completion now works when the previous separator is '/' and not just a ' ' space. This means you can type ./somefile and hit tab, and it will complete it something like ./somefilename, which is pretty cool! It also works like this: copy somefile /somed and it would complete it to copy /somedir allowing you to then finish typing copy /somedir/somefile which is also pretty cool!
- Added:     Ability to launch in -skiptitle or -st mode, which skips the hardware screen gets you to the prompt faster. This is also created for external programs that are launched from it and then re-load and re-run it, created a more seamless experience. There are two sample files to test this with: echoarg and lorem. Echoarg just prints whatever you arguments you send it. Lorem just prints out the lorum ipsum text. They both call ChiCLI -sf when they finished so it doesn't appear like you've left the system. 
- Added:     Ability to write current config to the EEPROM of the UIEC 
- Fixed:     When copying files, if there’s an error, it borks the whole system.
- Fixed:     Pressing RUN/STOP on the bottom line screws the cursor up.
- Fixed:     debug-args: 4 args is supported (should be 5).
- Added:     Now supports setting UIEC devices to hide or show extensions!
- Added:     Added make-dir and remove-dir commands.
- Added:     Also supports tab completion!
- Updated:   Tab completion how hides the cursor until it is finished searching.
- Updated:   Now supports bulk deleting and copying of files!
- Updated:   Added support for drive numbers 8 through 15.

Known Issues:
- Attempting to view the time stamp of files within a SD2IEC mounted disk image show only '0' characters, since .D64 disk images don't support time stamps, even though the SD2IEC does.
- Note: Commodore Flyer Internet Modem: This is a great product! The issues below stem, in part, from the fact that it is creating an Ethernet connection over the IEC serial protocol, and is blazing a new trail. Hopefully I'll be able to work some issues and hopefully document any firmware issues that may exist and pass them along to the developer, Retroswitch.
- Commodore Flyer Internet Modem: The ability to load and run directly from cloud storage, which is device 7, isn't supported yet within ChiCLI. You can, however, do everything normally from Basic first, then run ChiCLI without issues.
- Commodore Flyer Internet Modem: The ability to manage the Flyer settings configuration from within ChiCLI isn't supported yet.
- Commodore Flyer Internet Modem: The ability to mount different disk images from within ChiCLI isn't supported yet.
- Commodore Flyer Internet Modem: The Flyer doesn't seem to maintain the file extensions, such as PRG or SEQ, when files are copied from another device to the Flyer. Everything ends up as PRG. This is an issue with the Flyer firmware I think.
- Commodore Flyer Internet Modem: The Flyer has a configuration for JiffyDOS, but within ChiCLI when using JiffyDOS, the Flyer doesn't work properly. Have to investigate, but I think this is also a FLyer firmware issue.
- Commodore Flyer Internet Modem: The Flyer doesn't appear to support copying a file within a disk image. You copy from another device to it, but the DOS copy command itself doesn't appear to be supported.
- Removed: Tough one: debug-args: When using debug-args, or looking at argv[0] directly, the argv program name loads wrong text, usually something from a printf statement. It’s as if the storing of text for the printf statements are overwriting the part of memory where the file name is stored. 
Here are some links to help:
https://github.com/cc65/cc65/blob/master/libsrc/c64/mainargs.s
https://github.com/cc65/cc65/blob/master/asminc/c64.inc

Opportunities for Optimization of CPU and RAM Usage (This is old but should be re-visited):
- The list command has lots of room to remove code and printf statements.
- I need to make an entry in this list, every time I think of a place to optimize, so I don't forget and miss some low hanging fruit.
- I've got to go through the code and replace every integer variable with either a signed or unsigned char. This will save a *bunch* of RAM!
- If I re-write the alias code so that I can define the size of the string holding each alias. Right now, there's bug, and when I change the MACRO definition, it borks it up
- I can save a few vars if I clean up the color-set and profile-set code, so it reads and writes directly and only to memory, and looks it up directly from memory when needed. Now that I read it for start-up in -skiptitle mode, I can just tidy up and start using my memory helper macros.
- I should check the string helper actual functions, as I'm sure there are redundancies from merging code I developed separately and then integrated later. 
- I should create a few static strings that I re-use as much as possible, so I'm not wasting RAM with lots of string literals all over the joint. 
- I should figure out where I can ditch printf for conio functions, where possible. 
- Write something that automatically opens and prints a text file, so I don't have to ask the user to enter: "type chicli-readme" but instead it just gets called when they enter: "help". 
- Even more text file based help, and have it search for a certain tag in the text file. Have something like the user enter: "help alias" or "man alias" or "alias -h" or "alias --h" and it pulls up the section of the readme that has the relevant info. 

Commodore Systems - Available Memory to cc65
- https://www.cc65.org/doc/internal.txt

<pre>
Quick Summary of CC65 and RAM:
PET : 31K available for C programs.
C64 : 50K available for C programs.
128 : 41K available for C programs.
</pre>

Additional Resources:

CC65 Optimizations and Available Memory 

Optimization Links

Docs for cc65 
- https://cc65.github.io/doc/coding.html

Really Great Guide Readable on GitHub
- https://github.com/ilmenit/CC65-Advanced-Optimizations

Optimizing C array lookups for the 6502
- http://8bitworkshop.com/blog/compilers/2019/03/17/cc65-optimization.html

Amateur Optimization Hour, part 2
- https://coronax.wordpress.com/2018/09/13/amateur-optimization-hour-part-2/

nesdoug - NES Programming with cc65 - How cc65 works
- https://nesdoug.com/2018/09/05/how-cc65-works/

Programming NES games in C by Shiru
- https://shiru.untergrund.net/articles/programming_nes_games_in_c.htm

<pre>
---------------------------------------
cHIcli - qUICK cOMMAND lIST
---------------------------------------
./           EXIT       PWD
=            FILEDATE   REBOOT
ABOUT        FORMAT     RENAME
ALIAS        HELP       RESTART
CD           HEX2DEC    RMDIR
CHIRP        HOTKEY     RUN
CLEAR        INITIALIZE SCREENSAVER
COLOR-SET    KEYCODES   SHUTDOWN
COPY         LIST       STATUS
DATE         LP         STOPWATCH
DATETIME     MKDIR      SYS
DEC2BIN      MOUNT      SYSINFO
DEC2HEX      PARTITION  TIME
DELETE       PEEK       TYPE
DOS          PEEK-BIT   SD2IEC
DRIVE-DETECT PEEKING    UNALIAS
DRIVE-SET    POKE       UNMOUNT
DUMP-MEM     POKE-BIT   VALIDATE
ECHO         PROFILE    VIEW-MEM


---------------------------------------
cHIcli - bUILT-iN aLIAS lIST
---------------------------------------
CAT    --> TYPE
CLS    --> CLEAR
CP     --> COPY
DEL/DL --> DELETE
DIR    --> LIST
ENDCLI --> EXIT
HK     --> HOTKEY
INIT   --> INITIALIZE
LS     --> LIST
LP     --> LIST-PARTITIONS
MD     --> MAKE-DIR
PART   --> PARTITION
QUIT   --> EXIT
RD     --> REMOVE-DIR
REN    --> RENAME
RM     --> DELETE
RN     --> RENAME
SS     --> SCREENSAVER
SW     --> STOPWATCH
VAL    --> VALIDATE
VER    --> ABOUT

---------------------------------------
PROFILE [pROFILE nUMBER]
---------------------------------------
1 - pet
2 - vic-20
3 - c64
4 - c128
5 - c128 - 2
6 - bLACK ON wHITE (dEFAULT)
7 - wHITE ON bLACK
8 - gREY

---------------------------------------
COLOR-SET [tEXT] [bACKGROUND] [bORDER]
---------------------------------------
1 - bLACK        9 - oRANGE
2 - wHITE       10 - bROWN
3 - rED         11 - lT. rED
4 - cYAN        12 - dK. gREY
5 - pURPLE      13 - gREY
6 - gREEN       14 - lT. gREEN
7 - bLUE        15 - lT. bLUE
8 - yELLOW      16 - lT. gREY

---------------------------------------
cHIcli - qUICK cOMMAND eXAMPLES
---------------------------------------
> ./EXAMPLE
> = 2 + 2
> = 2 - 2
> = 4 * 4
> = 8 / 2
> ABOUT
> ALIAS
> ALIAS L "LS -S"
> ALIAS L -CLEAR
> CD MISC-FOLDER
> CD..
> CD/
> CHIRP
> CLEAR
> COLOR-SET 1 15 7
> COPY * D08:
> COPY * D08B:
> COPY SOMEFILE SOMEDIR/
> COPY SOMEFILE.TXT D08:
> COPY SOMEFILE.TXT D08b:
> COPY SOMEFILE.TXT SOMEFILECOPY.TXT
> D08:
> D09B:
> DATE
> DATETIME -SET
> DEC2BIN 55
> DEC2HEX 8000
> DEL *
> DEL SOME*
> DEL SOMEFILE.TXT
> DOS "VALIDATE0"
> DRIVE-DETECT
> DRIVE-SET -1541 10
> DRIVE-SET -SD2IEC 11
> DUMP-MEM 1000 2000 MEMORY.BIN
> DUMP-MEM ROM KERNAL KERNAL.ROM
> ECHO "cHIcli IS THE coolest!"
> EXIT
> FILEDATE CHICLI-README
> FORMAT MYDISK 0
> FORMAT MYDISK
> FORMAT
> FORMAT -Q MYDISK
> FORMAT -Q
> HELP
> HEX2DEC FFFF
> HOTKEY
> HOTKEY 1 "LS -S"
> HOTKEY 1 -CLEAR
> INITIALIZE
> KEYCODES
> LIST
> LIST -S
> MKDIR TEST-DIR
> MOUNT CHICLI-DISK.D64
> PARTITION B
> PEEK 1000
> PEEK-BIT 65535 0
> PEEKING 53266
> POKE 1000 33
> POKE-BIT 1000 3 1
> PROFILE 1
> PWD
> REBOOT
> RENAME OLDFILENAME NEWFILENAME
> RESTART
> RMDIR TEST-DIR
> RUN HELLO-WORLD
> SCREENSAVER
> SHUTDOWN
> STATUS
> STOPWATCH
> SYS 64738
> SYSINFO
> TIME
> TYPE CHICLI
> TYPE CHICLI -HEX
> TYPE CHICLI-README
> SD2IEC -HIDE-EXT
> SD2IEC -SHOW-EXT
> SD2IEC -SAVE
> UNMOUNT
> VALIDATE
> VIEW-MEM 65535
> VIEW-MEM 56296 56303

---------------------------------------
cHIcli - dETAILED hELP
---------------------------------------
user interface
---------------------------------------
tEXT eNTRY

cHIcli USES CUSTOM ROUTINES TO
MANAGE THE INPUT AND EDITING OF TEXT
ON THE COMMAND LINE. iT AUTOMATICALLY
INSERTS CHARACTERS, AND SUPPORTS 
BACKSPACE AND DELETE. iT ALSO SUPPORTS
CURSOR KEYS AND A HOME AND END KEYS.

tHE cOMMODORE CURSOR KEYS ARE SUPPORTED
AS WELL AS EXTRA KEYS TO MAKE IT EASIER
FOR PEOPLE WHO ARE COMING FROM THE
lINUX OR wINDOWS KEYBOARD LAYOUTS.

nOTE: 
del AND home ARE TREATED THE SAME.

cursor-left ctrl+j - cURSOR lEFT
cursor-left ctrl+l - cURSOR rIGHT
cursor-up   ctrl+i - lAST eNTERED tEXT
cursor-down ctrl+k - eRASE lINE

del ctrl+h         - bACKSPACE
shift+del ctrl+:   - fORWARD dELETE

ctrl+u             - sTART OF lINE
ctrl+o             - eND OF lINE

runstop ctrl+c     - cANCELS INPUT
---------------------------------------
tAB cOMPLETION

cOMMODORE 64: uSE ctrl+lEFT-aRROW

oN THE cOMMODORE 64 THERE IS NO tab 
KEY ON THE KEYBOARD. tHE CLOSEST IS THE
left arrow KEY ABOVE THE ctrl KEY AT 
THE FAR LEFT OF THE KEYBOARD. 

iN ORDER TO ENTER A LEFT ARROW 
CHARACTER, FOR EXAMPLE IF YOU NEED TO 
LOAD A FILE NAME THAT CONTAINS THE 
LEFT ARROW CHARACTER, YOU CAN PRESS:
left arrow 
TO ENTER THIS CHARACTER.

tHE TAB COMPLETION WILL START SEARCHING
THE CURRENT FOLDER FOR FILES WHOSE
BEGINNING MATCH THE LAST STRING OF 
TEXT ENTERED ON THE COMMAND LINE THAT'S
SEPARATED BY AN EMPTY SPACE. 

pLEASE NOTE: i'VE MADE THIS WAY FASTER
NOW. hOWEVER, IF YOU HAVE AN sd2iec 
WITH AN ENORMOUS NUMBER OF FILES, IT'S
GOING TO TAKE TIME TO LOAD EACH FILE 
NAME AND COMPARE IT TO THE TEXT YOU'RE
TRYING TO AUTO-COMPLETE.
---------------------------------------
hOTKEYS

hOTKEY MANAGES THE ABILITY TO ASSIGN
COMMAND STRINGS TO THE FUNCTION KEYS. 

eXAMPLES:
HOTKEY                  - lISTS HOTKEYS
HOTKEY 1 "ECHO hI!"     - aSSIGNS f1
HOTKEY 2 LS             - aSSIGNS f2
HOTKEY 1 -CLEAR         - cLEARS f1
HOTKEY -CLEAR           - cLEARS ALL

yOU CAN ALSO DO SOMETHING A LITTLE MORE
COMPLEX LIKE THIS: 

HOTKEY 1 = "ECHO ;cHIcli rULES!;"

tHE SEMI-COLON IS AUTOMATICALLY 
SUBSTITUTED FOR QUOTES WHEN THE SYSTEM
PROCESSES THE ECHO COMMAND. 
---------------------------------------
aLIASES

aLIAS MANAGED THE ALIASING SYSTEM. yOU
CAN SET YOUR OWN, CLEAR THEM, OR CLEAR
ALL OF THE ALIASES. 

tHERE ARE A NUMBER OF PRE-CONFIGURED 
ALIASES ALREADY SET, BASED ON COMMANDS
FROM cOMMODORE dos, aMIGAdos, ms-dos,
AND lINUX.

ALIAS                 - lISTS ALIASES
ALIAS HI "ECHO hI!"   - sETS AN ALIAS
ALIAS VM VIEW-MEM     - sETS AN ALIAS
ALIAS VM -CLEAR       - cLEARS AN ALIAS
ALIAS -CLEAR          - cLEARS ALL

ALIAS CR = "ECHO ;cHIcli rULES!;"

tHE SEMI-COLON IS AUTOMATICALLY 
SUBSTITUTED FOR QUOTES WHEN THE SYSTEM
PROCESSES THE ECHO COMMAND. 
---------------------------------------
aRGUMENTS

cHIcli SUPPORTS ARGUMENTS PASSED BY 
cOMMODORE bASIC. 

iT USES THE FOLLOWING FORMAT: 
run:rem arg1 "arg2 quoted" arg3 "" arg5

sUPPORTED OPTIONS:

dISABLE dRIVE dETECT: -DDD

tHIS DISABLED DRIVE DETECTION. tHIS IS
ALSO DISABLED ONCE YOU CHANGE DRIVE 
NUMBERS IN SOFTWARE. iN THE FUTURE, 
THIS WILL BE ADDRESSED DIFFERENTLY. 

eXAMPLE:
load"chicli",8
run:rem -ddd

sKIP tITLE: -SKIPTITLE OR -ST

tHIS SKIPS THE HARDWARE SCREEN GETS 
YOU TO THE PROMPT FASTER. tHIS IS ALSO 
CREATED FOR EXTERNAL PROGRAMS THAT ARE 
LAUNCHED FROM IT AND THEN RE-LOAD AND 
RE-RUN IT, CREATED A MORE SEAMLESS 
EXPERIENCE. 

eXAMPLE FROM bASIC:
load"chicli",8
run:rem -st

eXAMPLE FROM WITHIN cHIcli:
./CHICLI -ST
RUN CHICLI -ST
---------------------------------------
cOMPRESSED cHIcli FOR fASTER lOADING

cHIcli CAN BE COMPRESSED USING VARIOUS
CRUNCHING UTILITIES. 

eXOMIZER IS BEING TESTED WITH cHIcli,
AND YOU CAN DOWNLOAD THE COMPRESSED
VERSION CALLED CHICLI-EXO.PRG IN THE 
PROJECT'S FOLDER. 

tHE PROGRAM IS COMPRESSED BY ABOUT 50%
AND DECOMPRESSES IN ABOUT 5 SECONDS. 

tHE PASSING OF ARGUMENTS TO A 
COMPRESSED VERSION OF cHIcli HAS BEEN 
TESTED AND WORKS.
---------------------------------------
commands
---------------------------------------
./

tHIS LOADS AND RUNS A COMMAND AFTER 
EXITING THE cHIcli SYSTEM. tHIS DOESN'T
WORK AUTOCOMPLETE.

eXAMPLE:
./KONG-ARCADE
---------------------------------------
=

mATHS: tHE EQUALS COMMAND LETS YOU DO 
VERY SIMPLE CALCULATIONS. iT'S LIMITED 
TO INTEGERS AND ONLY TWO TERMS AND THE
BASIC OPERATIONS OF PLUS, MINUS, 
MULTIPLY, AND DIVIDE.

nOTE: yOU NEED TO HAVE SPACES BETWEEN
THE COMMAND, TERMS, AND OPERATION.
pLEASE SEE THE EXAMPLE BELOW.

eXAMPLE: = 2 + 2
eXAMPLE: = 2 - 2
eXAMPLE: = 4 * 4
eXAMPLE: = 8 / 2
---------------------------------------
ABOUT

tHIS DISPLAYS INFORMATION ABOUT 
THE SOFTWARE, AS WELL AS LINKS TO THE 
PROJECT ON THE WEB.

eXAMPLE: ABOUT
---------------------------------------
CD 

tHIS LET'S YOU CHANGE THE CURRENT 
WORKING FOLDER. iT ALSO LETS YOU MOVE
TO THE PREVIOUS OR ROOT FOLDERS

YOU CAN ALSO CHANGE TO DIFFERENT 
DEVICES AND PARTITIONS AS WELL.

eXAMPLE: MOVE INTO A FOLDER
CD SIDFILES

eXAMPLE: MOVE UP ONE FOLDER
CD .. OR CD..

eXAMPLE: MOVE TO THE ROOT FOLER
CD /

eXAMPLE: CHANGE TO DEVICE 9
CD D09:

eXAMPLE: CHANGE TO:
DEVICE 9 PARTITION B
CD D09B:
---------------------------------------
CHIRP

pLAYS THE cHIcli STARTUP SOUND!

eXAMPLE: CHIRP
---------------------------------------
CLEAR

tHIS CLEARS THE SCREEN OF TEXT.

eXAMPLE: CLEAR
---------------------------------------
COLOR-SET

cHANGES THE CURRENT TEXT, BACKGROUND 
AND BORDER COLORS, ALLOWING YOU TO SET
EACH INDIVIDUALLY.

COLOR-SET [tEXT] [bACKGROUND] [bORDER]

1 - bLACK        9 - oRANGE
2 - wHITE       10 - bROWN
3 - rED         11 - lT. rED
4 - cYAN        12 - dK. gREY
5 - pURPLE      13 - gREY
6 - gREEN       14 - lT. gREEN
7 - bLUE        15 - lT. bLUE
8 - yELLOW      16 - lT. gREY

eXAMPLE: COLOR-SET 1 15 7
---------------------------------------
COPY

tHIS LET'S YOU COPY FILES. yOU CAN 
ALSO USE * TO COPY ALL THE FILES 
WITHIN THE CURRENT DISK, IMAGE, OR 
DIRECTORY/FOLDER YOU ARE CURRENTLY IN, 
TO ANOTHER DRIVE NUMBER. 

fOLDERS MUST END WITH A SLASH / TO 
SHOW THEY ARE FOLDERS AND NOT A NEW
FILENAME FOR THE COPIED FILE.

yOU CAN ABORT MULTI-FILE COPYING
BY PRESSING ctrl-c OR run/stop
WHILE COPYING. nOTE: yOU MAY HAVE TO
PRESS ctrl-c REPEATLY UNTIL IT
STOPS.

eXAMPLES:

cOPY A SINGLE FILE WITHIN THE CURRENT
DIRECTORY (DUPLICATE):
COPY SOMEFILE.TXT SOMEFILE-COPY.TXT

cOPY A FILE INTO A FOLDER
COPY HELLOWORLD TEST-DIR/HELLOWORLD
COPY SOMEFILE /SOMEDIR/SOMEFILE

cOPY A FILE TO DRIVE 8:
COPY SOMEFILE.TXT D8:

cOPY ALL THE FILES IN THE CURRENT 
DIRECTORY TO ANOTHER DRIVE:
COPY * D08:

cOPY ALL THE FILES IN THE CURRENT
DIRECTORY TO ANOTHER PARTITION ON THE
SAME DEVICE:
COPY * D08B:
---------------------------------------
D##: 

cHANGES THE CURRENTLY ACTIVE DRIVE.
tHIS IS THE DRIVE YOU'RE "IN".

dRIVE NUMBERS 8 THROUGH 15 ARE 
SUPPORTED, DEPENDING ON YOUR 
HARDWARE.

eXAMPLE: D09:
---------------------------------------
D##?: 

cHANGES THE CURRENTLY ACTIVE DRIVE.
tHIS IS THE DRIVE YOU'RE "IN".

tHE CHARACTER AFTER THE 2 DIGITS IS 
THE PARTITION. uSE A FOR THE FIRST
PARTITION, AND B FOR THE SECOND,
ETC. tHE MAXIMUM PARTITIONS SUPPORTED
IS 9, USING LETTERS A THROUGH I.

dRIVE NUMBERS 8 THROUGH 15 ARE 
SUPPORTED, DEPENDING ON YOUR 
HARDWARE.

eXAMPLE: D09B:
---------------------------------------
DATETIME

dISPLAYS THE CURRENT DATE AND TIME.

aLL TIME AND DATE HANDLING BY cHIcli
IS DONE ON THE sd2iec DEVICE
EXCLUSIVELY.

sOME VERSIONS OF THE sd2iec, DEPENDING
ON HARDWARE OR FIRMWARE, HAVE A CLOCK
BUILT INTO THE DEVICE. oNE VERSION HAS
A SOFTWARE CLOCK THAT ONLY OPERATES
WHILE THE DEVICE IS POWERED. oNCE IT IS
POWERED OFF THE DATE AND TIME ARE LOST.

hOWEVER, THERE ARE VERSIONS OF THE
sd2iec THAT HAVE, OR CAN BE UPGRADED,
WITH A REAL-TIME CLOCK (rtc).

cHIcli SUPPORTS BOTH. yOU CAN VIEW
THE DATE AND TIME, AND EVEN SET THE
CLOCK, ALL WITHOUT LEAVING THE
ENVIRONMENT.

tHE VERSIONS THAT SUPPORT SOME FORM OF
A CLOCK, ALSO AUTOMATICALLY SAVE THE
TIME STAMP WHEN FILES ARE CREATED OR
MODIFIED ON THE sd CARD OF THE sd2iec.

aLSO SEE THE 'FILEDATE' COMMAND,
AS IT ALLOWS YOU TO LOOK-UP A GIVEN
FILE NAME AND GET IT'S TIME STAMP.

tO SET THE DATE AND TIME USE THE
PARAMETER "-SET".

eXAMPLE: DATETIME
eXAMPLE: DATETIME -SET
---------------------------------------
DEC2BIN

mATHS: cONVERTS FROM A DECIMAL NUMBER 
TO A BINARY NUMBER.

eXAMPLE: DEC2BIN 254
oUTPUTS: bin:11111110
---------------------------------------
DEC2HEX

mATHS: cONVERTS FROM A DECIMAL NUMBER 
TO A HEXIDECIMAL NUMBER.

eXAMPLE: DEC2HEX 234
oUTPUTS: hex:00EA
---------------------------------------
DELETE

tHIS LET'S YOU DELETE FILES. yOU CAN 
ALSO USE * TO DELETE ALL THE FILES 
WITHIN THE CURRENT DISK, IMAGE, OR 
FOLDER YOU ARE CURRENTLY IN.

yOU CAN ABORT MULTI-FILE DELETING
BY PRESSING ctrl-c OR run/stop
WHILE DELETING. nOTE: yOU MAY HAVE TO
PRESS ctrl-c REPEATLY UNTIL IT
STOPS.

eXAMPLES:
DELETE SOMEFILE.TXT - dELETES A FILE
DELETE *            - dELETES ALL FILES
                      IN CURRENT FOLDER
---------------------------------------
DOS

tHIS LETS YOU EXECUTE A CUSTOM COMMAND
INTENDED FOR A cOMMODORE dos DEVICE.
iT EXPECTS A SECOND ARGUMENT TO BE THE
COMMAND TO SEND TO THE CURRENTLY
SELECTED DRIVE. iT USES THE COMMAND
CHANNEL 1. iT PASSES THE GIVEN STRING
DIRECTLY TO THE cOMMODORE dos DEVICE.

nOTE: rEMEMBER THAT MOST EXAMPLES ARE
GIVEN IN UPPER CASE BECAUSE THAT IS THE
DEFAULT cOMMODORE bASIC STARTUP MODE.
cOMMODORE dos EXPECTS LOWER CASE, SO
WHEN USING THIS COMMAND IN cHIcli, YOU
NEED TO USE LOWER CASE, AS IN THE 
EXAMPLE GIVEN BELOW. 

eXAMPLE: 
DOS INITIALIZE0
---------------------------------------
DRIVE-DETECT

gETS THE DRIVE NAME STRING FROM THE
CURRENT DEVICE.

eXAMPLE: DRIVE-DETECT
---------------------------------------
DRIVE-SET

tHIS LETS YOU CHANGE THE DRIVE NUMBER 
OF THE CURRENT DRIVE.

fOR sd2iec DEVICES, YOU CAN USE THE
SD2IEC-SAVE COMMAND TO MAKE THIS 
CHANGE IN DEVICE NUMBER PERMANENT.
 
eXAMPLES: 
DRIVE-SET -1541 10
DRIVE-SET -SD2IEC 11 
---------------------------------------
DUMP-MEM

tHIS LET'S YOU DUMP AN AREA OF MEMORY
TO A FILE IN THE CURRENT DIRECTORY 
YOU'RE IN. 

tHE FIRST ARGUMENT IS THE STARTING
LOCATION IN MEMORY EXPRESSED AS A
DECIMAL NUMBER. 

tHE SECOND IS THE ENDING ADDRESS IN
MEMORY.

tHE FINAL ARGUMENT IS THE NAME OF THE
FILE THAT SHOULD BE USED TO SAVE THE
BYTES READ FROM MEMORY.

hERE'S AN EXAMPLE:
DUMP-MEM 1000 2000 MEMORY.BIN

hOWEVER, THERE IS ALSO A SHORTCUT,
FOR SITUATION WHEN YOU WANT TO
SAVE THE CONTENT OF YOUR KERNAL OR
basic romS. uSE "ROM" WITHOUT 
QUOTE FOR THE FIRST ARGUMENT, AND
EITHER "KERNAL" OR "BASIC" FOR 
THE SECOND ARGUMENT. tHE THIRD
ARGUMENT IS STILL THE FILE NAME.

hERE'S AN EXAMPLE:
DUMP-MEM ROM KERNAL KERNAL.ROM
---------------------------------------
ECHO

dISPLAYS THE TEXT AS THE FIRST
ARGUMENT. yOU CAN ALSO USE QUOTES
AROUND THE TEXT YOU WANT ECHOED.

eXAMPLE: ECHO "cHIcli IS THE coolest!"
---------------------------------------
EXIT

tHIS EXITS cHIcli.

eXAMPLE: EXIT
---------------------------------------
FILEDATE

iF YOU HAVE AN sd2iec, IT IS CAPABLE
OF DISPLAYING TIME STAMPS FOR A GIVEN
FILE. tHIS COMMAND SHOWS YOU A TIME
STAMP FOR THE FILE GIVEN, AS IT HAS
BEEN RECORDED ON THE fat32 sd CARD.
iF YOU DON'T HAVE A rtc INSTALLED ON
YOUR sd2iec, THEN ANY FILES SAVED
BY THE sd2iec ITSELF WHILE USING A
cOMMODORE 64 WILL NOT HAVE ACCURATE
TIME STAMPS. hOWEVER, ANY FILES SAVED
ON THE sd BY A DEVICE WITH A CLOCK
THAT'S SET CORRECTLY WILL HAVE THE
CORRECT TIME FOR WHEN THE FILE WAS
LAST MODIFIED.

aDDITIONALLY, IF YOUR sd2iec HAS THE
SOFTWARE CLOCK ENABLED, AND THE TIME
AND DATE HAVE BEEN SET SINCE IT WAS
POWERED ON, THEN IT MAY BE POSSIBLE
TO SAVE FILES WITH THE CORRECT TIME
STAMP INFORMATION AS WELL.

pLEASE NOTE, AS OF THE WRITING OF THIS
DOCUMENT, IT IS POSSIBLE YOUR MODERN
COMPUTER OR DEVICE WILL SAVE THE TIME
STAMP INFORMATION IN utc AND THEN
LOCALLY DISPLAY THE TIME STAMPS
CORRECTLY FOR YOUR TIME ZONE. tHE
sd2iec WILL DISPLAY WHATEVER TIME
STAMP IS SAVED ON THE sd CARD, SO
THERE MAY BE A DISCREPANCY BETWEEN
THE SAVED TIME AND WHAT YOU EXPECTED.

aTTEMPTING TO ACCOUNT AND ADJUST FOR
THIS IS BEYOND THE CURRENT SCOPE OF
THIS PROGRAM, SINCE cHIcli HAS MOVED
ALL TIME AND DATE HANDLING TO THE
sd2iec DEVICE EXCLUSIVELY.

eXAMPLE: FILEDATE CHICLI-README
---------------------------------------
FORMAT

fORMAT A DISK, BASED ON THE DEVICE 
YOUR CURRENTLY IN. 
fIRST ARG IS THE DISK NAME.
sECOND ARG IS THE DISK NUMBER

iF YOU OMIT THE NAME, FORMAT WILL 
DEFAULT TO CALLING THE DISK BY THE 
NAME "UNTITLED. iF YOU OMIT THE DISK
id, FORMAT WILL GIVE IT "0" BY 
DEFAULT.

tHE "FORMAT" COMMAND NOW SUPPORTS THE 
cOMMODORE dos ABILITY TO DO A QUICK 
FORMAT, INSTEAD OF A FULL FORMAT. uSE 
FORMAT -Q DISKNAME TO QUICK FORMAT 
A DISK CALLING IT DISKNAME.

eXAMPLES: 
FORMAT MYDISK 0
FORMAT MYDISK
FORMAT
FORMAT -Q MYDISK
FORMAT -Q
---------------------------------------
HELP 

dISPLAYS A QUICK LIST OF COMMANDS. 
NOTE: tHE FILE CHICLI-README.TXT 
CONTAINS DETAILED HELP.

eXAMPLE: HELP
---------------------------------------
HEX2DEC

mATHS: cONVERTS FROM A HEXIDECIMAL 
NUMBER TO A DECIMAL NUMBER.

eXAMPLE: HEX2DEC ffff
oUTPUTS: dec:65535
---------------------------------------
INITIALIZE

eXECUTES THE cOMMODORE dos DRIVE 
COMMAND FOR INITIALIZE, BASED ON THE 
CURRENT DRIVE.

eXAMPLE: INITIALIZE
---------------------------------------
KEYCODES

dISPLAYS THE KEY CODE OF A KEY THAT'S 
PRESSED IN DECIMAL VALUE. run/stop OR
ctrl+c QUITS THE COMMAND.

KEYCODES
---------------------------------------
LIST

tHIS DISPLAYS A LISTING OF THE FILES 
WITHIN THE DISK, IMAGE, OR FOLDER 
YOU ARE CURRENTLY IN.

oPTION: -S
tHIS LISTS THE CONTENTS IN A FORMAT
THAT'S TWICE AS WIDE, SHOWING 2 FILES
FOR EACH LINE OF THE SCREEN.

eXAMPLE: LIST
eXAMPLE: LIST -S
---------------------------------------
LIST-PARTITIONS / LP

THIS COMMAND LISTS THE PARTITIONS ON
THE CURRENT DEVICE.

EXAMPLE:
LP
---------------------------------------
MKDIR

mAKES A NEW, EMPTY DIRECTORY, USING 
THE PROVIDED NAME.

eXAMPLE: MKDIR GAME-IMAGES
---------------------------------------
MOUNT

tHIS MOUNTS A DISK IMAGE ON A sd2iec
DEVICE, LIKE THE sd2iec. yOU CAN THEN
COPY SOME OF ALL OF THE FILES TO A REAL
FLOPPY DISK. 

eXAMPLE: 
MOUNT CHICLI-DISK.D64
---------------------------------------
PARTITION

tHIS QUICKLY CHANGES THE CURRENT 
PARTITION BASED ON THE SUPPLIED LETTER.
tHE SHORTCUT "PART" MAKES USING THIS 
HANDY TO CHANGE PARTITIONS FAST. oNLY
WORKS ON DRIVES THAT SUPPORT PARTITIONS.

eXAMPLE: PARTITION B
---------------------------------------
PEEK      

rETURNS THE VALUE STORED IN MEMORY AT
THE GIVEN ADDRESS. wORKS JUST AS IN 
cOMMODORE bASIC.

tHIS TAKES THE ADDRESS AS A 
DECIMAL VALUE.

eXAMPLE: 
PEEK 1000
---------------------------------------
PEEK-BIT

rETURNS THE VALUE OF A GIVEN BIT 
STORED IN MEMORY AT THE GIVEN ADDRESS. 

eXAMPLE: 
PEEK 1024 2
tHIS GRABS THE THIRD LEAST SIGNIFICANT 
BIT FROM MEMORY ADDRESS 1024 IN 
DECIMAL FORMAT.
---------------------------------------
POKE

uPDATES THE VALUE STORED IN MEMORY AT
THE GIVEN ADDRESS. wORKS JUST AS IN 
cOMMODORE bASIC.

tHIS VERSION TAKES THE ADDRESS AND 
VALUE AS A DECIMAL NUMBER.

eXAMPLE: 
POKE 1000 33
---------------------------------------
POKE-BIT

wRITES THE VALUE OF A GIVEN BIT 
STORED IN MEMORY AT THE GIVEN ADDRESS. 

eXAMPLE: 
PEEK 1024 2 1
wRITES 1 TO THE THIRD LEAST SIGNIFICANT
BIT FROM MEMORY ADDRESS 1024 IN
DECIMAL FORMAT
---------------------------------------
PROFILE    

cHANGES THE CURRENT TEXT, BACKGROUND 
AND BORDER COLORS, TO ONE OF A SET OF
PRE-DETERMINED PROFILES.

PROFILE [pROFILE nUMBER]

1 - pet    5 - c128 - 2
2 - vic-20 6 - bLACK ON wHITE (dEFAULT)
3 - c64    7 - wHITE ON bLACK
4 - c128   8 - gREY

eXAMPLE: 
PROFILE 1
---------------------------------------
PWD

sHOWS YOU THE CURRENTLY SET DEVICE 
AND PARTITION.

eXAMPLE: PWD
---------------------------------------
REBOOT

tHIS RESTARTS THE cOMMODORE BY CALLING
THE SYSTEM RESET VECTOR.

nOTE: iF YOU'RE USING A fAST lOAD
CARTRIDGE, THIS WILL RESET WITHOUT
fAST lOAD. yOU'LL HAVE TO SYS TO A
SPECIAL VALUE TO RELOAD THE fAST lOAD
CARTIDGE.

nOTE: tHE ePYX fAST lOAD CARTRIDGE
WILL NOT BE LOADED IF YOU CALL THE
REBOOT COMMAND. tO RE-ENABLE IT,
YOU NEED TO RUN THIS COMMAND IN
cOMMOODRE bASIC: SYS 57194

eXAMPLE: REBOOT
---------------------------------------
RENAME 

rENAMES A FILE.

iF A FILE IS USING A WEIRD OR HARD TO 
ENTER CHARACTER, YOU CAN USE A WILDCARD
TO RENAME IT.

eXAMPLE: RENAME OLDFILENAME NEWFILENAME
eXAMPLE: RENAME WEIRDFILE* FRIENDLYFILE
---------------------------------------
RESTART

tHIS EXITS cHIcli AND AUTOMATICALLY
LOADS AND RUNS cHIcli FROM DISK AGAIN.

eXAMPLE: RESTART
---------------------------------------
REMOVE-DIR

rEMVOE A DIRECTORY. 

eXAMPLE: REMOVE-DIR TESTDIR
---------------------------------------
RUN 

tHIS LOADS AND RUNS A COMMAND AFTER 
EXITING THE cHIcli SYSTEM. yOU CAN USE
TAB COMPLETION WITH THIS COMMAND.

iF A FILE IS USING A WEIRD OR HARD TO
ENTER CHARACTER, YOU CAN USE A WILDCARD
TO RUN IT.

eXAMPLE: RUN KONG*
---------------------------------------
SCREENSAVER

dISPLAYS A SCREENSAVER THAT IS A MOVING
cOMMODORE INSPIRED LOGO.

tHE SCREENSAVER ALSO DISPLAYS THE DATE
AND TIME, IF USED WITH AN sd2iec.

yOU CAN ALSO ENABLE OR DISABLE WHETHER
THE DATE AND/OR TIME IS DISPLAYED.

bY DEFAULT ONLY THE TIME IS DISPLAYED,
AND ONLY IF A sd2iec SOFTWARE OR rtc
CLOCK IS DETECTED.

yOU CAN USE THE FOLLOWING AGRUMENTS
TO TOGGLE THE OPTIONS:

-ENABLE-TIME
-ENABLE-DATE
-DISABLE-TIME
-DISABLE-DATE

eXAMPLES:
SCREENSAVER
SCREENSAVER -DISABLE-TIME
SCREENSAVER -ENABLE-DATE
---------------------------------------
SHUTDOWN   

tHIS EXITS THE PROGRAM AND DISPLAYS
A MESSAGE LETTING YOU KNOW IT'S SAFE
TO TURN OFF YOUR cOMMODORE.  

eXAMPLE: SHUTDOWN
---------------------------------------
STATUS 

tHIS RETRIVES THE STATUS OF THE 
CURRENT DRIVE.

eXAMPLE: STATUS
---------------------------------------
STOPWATCH

tHE "STOPWATCH" COMMAND STARTS WHEN 
YOU RUN IT, AND WHEN RUN AGAIN, STOPS 
AND OUTPUTS THE ELAPSED SECONDS SINCE 
IT WAS LAST CALLED. iT RESETS ON THE 
SECOND CALL AS WELL. tHE MAXIMUM TIME 
IS ABOUT 18 MINS.

eXAMPLE: STOPWATCH
---------------------------------------
SYS

tHIS CAUSES EXECUTION TO JUMP TO THE 
MEMORY LOCATION GIVEN. iT EXPECTS A
NUMBER IN DECIMAL. tHIS WORKS JUST 
LIKE THE cOMMODORE bASIC COMMAND.

eXAMPLE: SYS 64738
---------------------------------------
SYSINFO

aTTEMPTS TO DETECT YOUR HARDWARE AND
DISPLAYS INFORMATION ABOUT YOUR SYSTEM.
iT ASSUMES THAT YOU'VE GOT A STANDARD
STOCK cOMMODORE. iT ALSO PLAYS A CHIME
BASED ON THE CHIME USED ON THE 
cOMMODORE pet AND cOMMODORE pc 
COMPUTERS.

iT WILL ALSO DETECT IF THE SYSTEM IS 
RUNNING UNDER EMULATION. aLTHOUGH IT'S
NOT INFALLIBLE, IT WILL RECOGNISE 
vice AS WELL AS tHEc64.

fEEL FREE TO DONATE ADDITIONAL HARDWARE
IF YOU'D LIKE MORE SPECIALIZED SYSTEMS
TO BE RECOGNISED.

eXAMPLE: SYSINFO
---------------------------------------
TIME

dISPLAYS THE CURRENT TIME.

tO SET THE DATE AND TIME USE THE
COMMAND DATETIME WITH THE
PARAMETER "-SET".

eXAMPLE: TIME
---------------------------------------
TYPE

tHIS IS A FILE VIEWER. iT CAN DISPLAY
FILES IN THE FORMATS seq, prg, OR ANY
OTHER TYPE OF FILE. 

iT ATTEMPTS TO AUTO-DETECT THE FILE 
TYPE AND SHOWS THE RELEVANT OUTPUT.

fOR seq FILES, IT DISPLAYS THEM AS IF
THEY WERE TEXT FILES. 

fOR prg FILES, IT DISPLAYS THEM AS 
cOMMODORE bASIC WOULD DISPLAY THEM.
iT SUBSTITUTES THE CODES FOR THEIR 
RELATED BASIC TOKENS.

iF YOU'D LIKE TO VIEW THE FILE'S RAW
HEXIDECIMAL VALUES, YOU CAN APPEND
-HEX AS THE LAST ARGUMENT. 

eXAMPLE: 
TYPE CHICLI
TYPE CHICLI-README
TYPE CHICLI -HEX
---------------------------------------
SD2IEC

tHIS COMMAND DOES sd2iec SPECIFIC 
FUNCTIONS. tHE SECOND ARGUMENT
DETERMINES WHICH FUNCTION YOU'RE
CALLING ON THE DEVICE.

-HIDE-EXT

tHIS IS A SD2IEC SPECIFIC SETTING THAT
CAUSES IT TO HIDE FILES ENDING IN
cOMMODORE dos EXTENSIONS, SUCH AS
.PRG OR .SEQ AND INSTEAD AUTOMATICALLY
REPORTS THEM TO THE SYSTEM AS IF THEY
WERE IN FACT THAT TYPE OF FILE.

pLEASE SEE YOUR SD2IEC DOCUMENTATION.

eXAMPLE:
SD2IEC -HIDE-EXT

-SHOW-EXT

tHIS IS A SD2IEC SPECIFIC SETTING THAT
CAUSES IT TO SHOW FILES ENDING IN 
cOMMODORE dos EXTENSIONS, SUCH AS
.PRG OR .SEQ AND REPORTS ALL FILES 
AS prg FILES UNLESS cOMMODORE dos
AS SET SPECIFICALLY.

pLEASE SEE YOUR SD2IEC DOCUMENTATION. 

eXAMPLE: 
SD2IEC -SHOW-EXT

-SAVE

sAVES THE CURRENT SETTINGS TO THE 
sd2iec. fOR EXAMPLE, IF YOU RE-ASSIGN
THE sd2iec DEVICE 10, THE NEXT TIME
THE DEVICE IS POWER-CYCLED, IT WILL
REVERT BACK TO IT'S PREVIOUS SETTING.
hOWEVER, USING THIS COMMAND, YOU CAN 
CAUSE IT TO SAVE THE CURRENT SETTINGS.
tHEREFORE, IN OUR EXAMPLE, WHEN THE 
DEVICE IS POWER-CYCLED, IT WILL START
UP SET TO DEVICE 10.

eXAMPLE: 
SD2IEC -SAVE
---------------------------------------
UNMOUNT

tHIS UNMOUNTS A PREVIOUSLY MOUNTED 
DISK IMAGE. iT AUTOMATICALLY POPS YOU 
BACK TO THE PREVIOUS FOLDER. 

eXAMPLE: UNMOUNT
---------------------------------------
VALIDATE

eXECUTES THE cOMMODORE dos DRIVE 
COMMAND FOR VALIDATE, BASED ON THE 
CURRENT DRIVE.

eXAMPLE: VALIDATE
---------------------------------------
VIEW-MEM   

dISPLAYS MEMORY IN HEXIDECIMAL AND IN 
PLAIN TEXT VALUE.

eXAMPLE: VIEW-MEM A000
---------------------------------------

i HAD FUN MAKING cHIcli AND i HOPE YOU
ENJOY USING IT!

- cHIRON bRAMBERGER

---------------------------------------
</pre>
