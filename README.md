# ChiCLI

A CLI for Commodore Computers!

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_R3_screenshot.png?raw=true)

ChiCLI v1.R5 (c) 2021 Chiron Bramberger

A Commodore 64 command line interface terminal, similar to and inspired by: AmigaDOS, Commodore DOS, MS-DOS, and Linux.

Pronunciation of ChiCLI: ky see-elle-eye

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
- SD2IEC by Jim Brain --> Get One Here --> http://store.go4retro.com/uiec-sd/ (Not Sponsored)
- IDE64 tested by Leif Bloomquist --> Leif's GitHub --> https://github.com/LeifBloomquist
- Commodore 1541 Disk Drive with stock ROM
- MSD SD-2 Disk Drive via Serial IEC
- MSD SD-2 Disk Drive via a Batteries Included IEEE-488 BusCardII
- Commodore 2031 via a Batteries Included IEEE-488 BusCardII
- Commodore 4040 via a Batteries Included IEEE-488 BusCardII
- Commodore SFD 1001 via a Batteries Included IEEE-488 BusCardII
- VICE: Emulated Commodore drives: 1541, 1541-II, 1570, 1571
- Commodore 64 and Commodore 1541 with JiffyDOS installed
- Commodore 64 with Epyx Fast Load and Super Snapshot 4
- Commodore Flyer Internet Modem --> http://www.retroswitch.com/products/flyer/ (Not Sponsored)

Latest News and Changelog:

v1.R5 - Release Candidate 5 - Dec ?? 2021
- Updated:   Converted both copy macros to proper functions and saved 1261 bytes total!!! This means I can enable more and larger aliases and hotkeys and still have some extra room for refinements before a final version 1.0 release!

v1.R4 - Release Candidate 4 - Dec 17 2021
- Issue:     I think my SD card was dying which sent me down a few rabbit holes... however, this lead to some fixes...
- Issue:     Needed to make multiple small updates to ensure copy and delete work properly between various target partitions and drives, within and across devices.

v1.R3 - Release Candidate 3 - Dec 16 2021
- Optimized: *NOW A SUPER-CHARGED JAM-PACKED MEGA-APPLICATION!* I'm always optimizing to squeeze in more commands!
- Added:     The changename command, which lets you change the name of a disk without formatting it. This only works on 1541, 2031, and SD-2 drives.
- Added:     The changeid command, which lets you change the ID of a disk without formatting it. This only works on 1541, 2031, and SD-2 drives.
- Added:     The diskcopy command, which executes a diskcopy from the source drive a (or drive 0) to the target drive b (drive 1), on dual drives that support it, such as the 4040 and the SD-2.
- Added:     The dumpmem command, which let's you dump a section of memory to disk.
- Added:     The vars command, which shows a list of the bytes that can be used for certain system functions, for example how many characters can be used in a hotkey or alias.
- Updated:   The Commodore 4040 is now tested on real hardware and supported.
- Updated:   The MSD SD-2 is now tested on real hardware and supported.
- Updated:   The list / dir command now works in long or short mode, and you can use wildcards to get matching listing. By default, dir uses two column mode, and list uses one column mode, like in the Amiga Shell.
- Updated:   The driveset command now detects what device you're on and automatically uses the right code for that device. Currently supports the SD2IEC, 1541 and SD-2 drives.
- Updated:   The changelabel, changeid, and driveset commands to only work on supported devices.
- Updated:   The command names so that no commands have hyphens in them. Just makes it hard to type when you're muscle memory is from modern keyboards, since the Commodore 64 has switched plus and minus key's positions.
- Updated:   The type command to take up less room when outputting BASIC listings. Also some a bug fix for this.
- Updated:   The logo function now displays the logo using way less code and memory. It's saved as a bitmap array and a line color list. It doesn't have diagonal color pattern anymore, which I preferred, but it's worth the savings!
- Fixed:     Bug in dumpmem where it always saves to partition / drive 0, which works fine on any SD2IEC partition, but doesn't work on SD-2 or 4040.
- Fixed:     Bug in the updated type command for BASIC.
- Fixed:     The copy command had a weird, hard to re-create bug, whereby you would press 'y' to continue, but it would abort instead. This is identified and fixed.
- Fixed:     The BusCard II no longer outputs junky characters for devices that aren't installed.
- Issue:     The Commodore 4040: Disks cannot be renamed or ID changed using the changename or changeid commands. The Commodore DOS on these early drives works differently and will need some effort to work around.
- Issue:     The hardware detection may hang on ridiculous systems for people who run on crazy shenanigans hardware! First noticed an issue on a 1541 Ultimate. Since I don't offically support non-standard Commodore 64 configurations, this is technically outside the scope of this application. Therefore, I'll try to address for the final 1.0 version, but no promises. I think it's hanging on the drive detection, because what I saw had the SID detection completed, but was otherwise hung.

v1.R2 - Release Candidate 2 - Dec 1 2021
- Optimized: *PACKS -EVEN MORE- IN A SINGLE APPLICATION!* Even more optimizations!
- Added:     Support for reading RTC from an RTC upgraded SD2IEC!!!
- Added:     Support for setting RTC.
- Added:     The filedate command, which allows you to view the time stamp of a given file name.
- Updated:   The datetime command uses the RTC from the SD2IEC exculsively, even if it doesn't have an RTC, as the SD2IEC uses a software clock instead.
- Updated:   The datetime command is seprate from the separate date and time commands.
- Updated:   Screensaver to use RTC time. Wasn't needed - Worked automatically!
- Updated:   Make logo show nothing instead of time if there’s no SD2IEC.
- Updated:   Support for checking if RTC exists before doing time and date things.
- Added:     A list-partitions command, which shows the names and letters of available partitions on the device.
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
----- ChiCLI - Quick Command List -----

   ./          driveset   profile
   =           dumpmem    pwd
   about       echo       reboot
   alias       exit       rename
   cd          filedate   restart
   changeid    format     rmdir
   changelabel help       run
   chirp       hex2dec    screensaver
   clear       hotkey     sd2iec
   colorset    initialize shutdown
   copy        keycodes   status
   d##:        list       stopwatch
   d##?:       ld         sys
   date        mkdir      sysinfo
   datetime    mount      time
   dec2bin     part       type
   dec2hex     peek       unalias
   delete      peekbit    unmount
   diskcopy    peeking    validate
   dos         poke       vars
   drivedetect pokebit    viewmem

---------------------------------------
ChiCLI - Built-In Alias List
---------------------------------------
cat    --> type
cls    --> clear
cp     --> copy
del/dl --> delete
dir    --> list
endcli --> exit
hk     --> hotkey
init   --> initialize
ls     --> list
ld     --> lp
md     --> make-dir
part   --> drive
quit   --> exit
rd     --> remove-dir
ren    --> rename
rm     --> delete
rn     --> rename
ss     --> screensaver
sw     --> stopwatch
val    --> validate
ver    --> about

---------------------------------------
profile [Profile Number]
---------------------------------------
1 - PET
2 - VIC-20
3 - C64
4 - C128
5 - C128 - 2
6 - Black on White (Default)
7 - White on Black
8 - Grey

---------------------------------------
color-set [Text] [Background] [Border]
---------------------------------------
1 - Black        9 - Orange
2 - White       10 - Brown
3 - Red         11 - Lt. Red
4 - Cyan        12 - Dk. Grey
5 - Purple      13 - Grey
6 - Green       14 - Lt. Green
7 - Blue        15 - Lt. Blue
8 - Yellow      16 - Lt. Grey

---------------------------------------
ChiCLI - Quick Command Examples
---------------------------------------
> ./example
> = 2 + 2
> = 2 - 2
> = 4 * 4
> = 8 / 2
> about
> alias
> alias l "ls -s"
> alias l -clear
> cd misc-folder
> cd..
> cd/
> changelabel new-disk-name
> changeid fx
> chirp
> clear
> colorset 1 15 7
> copy * d08:
> copy * d08b:
> copy somefile somedir/
> copy somefile.txt d08:
> copy somefile.txt d08B:
> copy somefile.txt somefilecopy.txt
> d08:
> d09b:
> date
> datetime -set
> dec2bin 55
> dec2hex 8000
> del *
> del some*
> del somefile.txt
> diskcopy
> dos "validate0"
> drive a
> drive b
> drivedetect
> driveset 10
> driveset 11
> dumpmem 1000 2000 memory.bin
> dumpmem rom kernal kernal.rom
> echo "ChiCLI is the COOLEST!"
> exit
> filedate chicli-readme
> format mydisk 0
> format mydisk
> format
> format -q mydisk
> format -q
> help
> hex2dec ffff
> hotkey
> hotkey 1 "ls -s"
> hotkey 1 -clear
> initialize
> keycodes
> list
> list -s
> list -l
> list *.d64
> list -s *.d64
> list *.d64 -l
> mkdir test-dir
> mount chicli-disk.d64
> part b
> peek 1000
> peekbit 65535 0
> peeking 53266
> poke 1000 33
> pokebit 1000 3 1
> profile 1
> pwd
> reboot
> rename oldfilename newfilename
> restart
> rmdir test-dir
> run hello-world
> screensaver
> shutdown
> status
> stopwatch
> sys 64738
> sysinfo
> time
> type chicli
> type chicli -hex
> type chicli-readme
> sd2iec -hide-ext
> sd2iec -show-ext
> sd2iec -save
> unmount
> validate
> vars
> viewmem 65535
> viewmem 56296 56303
> viewmem rom kernal

---------------------------------------
ChiCLI - Detailed Help
---------------------------------------
USER INTERFACE
---------------------------------------
Text Entry

ChiCLI uses custom routines to
manage the input and editing of text
on the command line. It automatically
inserts characters, and supports 
backspace and delete. It also supports
cursor keys and a home and end keys.

The Commodore cursor keys are supported
as well as extra keys to make it easier
for people who are coming from the
Linux or Windows keyboard layouts.

Note: 
DEL and HOME are treated the same.

CURSOR-LEFT CTRL+J - Cursor Left
CURSOR-LEFT CTRL+L - Cursor Right
CURSOR-UP   CTRL+I - Last Entered Text
CURSOR-DOWN CTRL+K - Erase Line

DEL CTRL+H         - Backspace
SHIFT+DEL CTRL+:   - Forward Delete

CTRL+U             - Start of Line
CTRL+O             - End of Line

RUNSTOP CTRL+C     - Cancels input
---------------------------------------
Tab Completion

Commodore 64: Use CTRL+Left-Arrow

On the Commodore 64 there is no TAB 
key on the keyboard. The closest is the
LEFT ARROW key above the CTRL key at 
the far left of the keyboard. 

In order to enter a left arrow 
character, for example if you need to 
load a file name that contains the 
left arrow character, you can press:
LEFT ARROW 
to enter this character.

The tab completion will start searching
the current folder for files whose
beginning match the last string of 
text entered on the command line that's
separated by an empty space. 

Please note: I've made this way faster
now. However, if you have an SD2IEC 
with an enormous number of files, it's
going to take time to load each file 
name and compare it to the text you're
trying to auto-complete.
---------------------------------------
Hotkeys

Hotkey manages the ability to assign
command strings to the function keys. 

Examples:
hotkey                  - Lists hotkeys
hotkey 1 "echo Hi!"     - Assigns F1
hotkey 2 ls             - Assigns F2
hotkey 1 -clear         - Clears F1
hotkey -clear           - Clears all

You can also do something a little more
complex like this: 

hotkey 1 = "echo ;ChiCLI Rules!;"

The semi-colon is automatically 
substituted for quotes when the system
processes the echo command. 
---------------------------------------
Aliases

Alias managed the aliasing system. You
can set your own, clear them, or clear
all of the aliases. 

There are a number of pre-configured 
aliases already set, based on commands
from Commodore DOS, AmigaDOS, MS-DOS,
and Linux.

alias                 - Lists aliases
alias hi "echo Hi!"   - Sets an alias
alias vm view-mem     - Sets an alias
alias vm -clear       - Clears an alias
alias -clear          - Clears all

alias cr = "echo ;ChiCLI Rules!;"

The semi-colon is automatically 
substituted for quotes when the system
processes the echo command.
---------------------------------------
Arguments

ChiCLI supports arguments passed by
Commodore Basic.

It uses the following format:
RUN:REM ARG1 "ARG2 QUOTED" ARG3 "" ARG5

Supported options:

Disable Drive Detect: -ddd

This disabled drive detection. This is
also disabled once you change drive 
numbers in software. In the future, 
this will be addressed differently. 

Example:
LOAD"CHICLI",8
RUN:REM -DDD

Skip Title: -skiptitle or -st

This skips the hardware screen gets 
you to the prompt faster. This is also 
created for external programs that are 
launched from it and then re-load and 
re-run it, created a more seamless 
experience. 

Example from Basic:
LOAD"CHICLI",8
RUN:REM -ST

Example from within ChiCLI:
./chicli -st
run chicli -st
---------------------------------------
COMMANDS
---------------------------------------
./ Command

This loads and runs a command after
exiting the ChiCLI system. This doesn't
work with autocomplete.

The was created with the assumption
that external commands will be created
to work with ChiCLI. The intention is
that an external command or program
could be called, and passed arguments,
and that this program would then run
ChiCLI in a special mode that doesn't
clear the screen and display the output
of sysinfo.

For a detailed example, I'd like to
eventually create ChiEdit, which is a
text editor program to compliment
ChiCLI. If you wanted to use it, you
would type:

./chiedit some-file.txt

...and this would start the text
editor and open the some-file.txt file
for editing. Then, once you've finished
and saved the file, and then exited
the program, the program would know
that it was called from within ChiCLI
and automatically run ChiCLI again,
but this time without the sysinfo
title screen.

You can also use this with any program
you'd like, however, you might have
to use the "run" command instead, if
for some reason the program doesn't
work properly when called using the
"./" method.

Although this looks like a very UNIX
or POSIX method, it's just a command
with the name "./" and doesn't work
with complex paths. It may support
this in the future. The reason you
can't just call a file by it's name,
as you would in MS-DOS, is that would
involve scanning the entire current
directory for PRG files that can
actually be executed. This would take
too long on slow drives and make the
whole of ChiCLI less responsive.
---------------------------------------
= MATHS COMMAND

Maths: The equals command lets you do 
very simple calculations. It's limited 
to integers and only two terms and the
basic operations of plus, minus, 
multiply, and divide.

Note: You need to have spaces between
the command, terms, and operation.
Please see the example below.

Example: = 2 + 2
Example: = 2 - 2
Example: = 4 * 4
Example: = 8 / 2
---------------------------------------
ABOUT COMMAND

This displays information about 
the software, as well as links to the 
project on the web.

Example: about
---------------------------------------
CD COMMAND

This let's you change the current 
working folder. It also lets you move
to the previous or root folders

you can also change to different 
devices and partitions as well.

Example: move into a folder
cd sidfiles

Example: move up one folder
cd .. or cd..

Example: move to the root foler
cd /

Example: change to device 9
cd d09:

Example: change to:
device 9 partition b
cd d09b:
---------------------------------------
CHANGELABEL COMMAND

This command let's you change the name
or label of a 1541 formatted or
compatible floppy disk.

Currently supported drives:
- Commodore 1541
- Commodore SFD 1001
- MSD SD-2

Drives that should work but don't:
- Commodore 4040

Example: changelabel new-disk
---------------------------------------
CHANGEID COMMAND

This command let's you change the ID
of a 1541 formatted or compatible
floppy disk.

Currently supported drives:
- Commodore 1541
- Commodore SFD 1001
- MSD SD-2

Drives that should work but don't:
- Commodore 4040

Example: changeid fx
---------------------------------------
CHIRP COMMAND

Plays the ChiCLI startup sound!

Example: chirp
---------------------------------------
CLEAR COMMAND

This clears the screen of text.

Example: clear
---------------------------------------
COLORSET COMMAND

Changes the current text, background 
and border colors, allowing you to set
each individually.

color-set [Text] [Background] [Border]

1 - Black        9 - Orange
2 - White       10 - Brown
3 - Red         11 - Lt. Red
4 - Cyan        12 - Dk. Grey
5 - Purple      13 - Grey
6 - Green       14 - Lt. Green
7 - Blue        15 - Lt. Blue
8 - Yellow      16 - Lt. Grey

Example: color-set 1 15 7
---------------------------------------
COPY COMMAND

This let's you copy files. You can 
also use * to copy all the files 
within the current disk, image, or 
directory/folder you are currently in, 
to another drive number. 

Folders must end with a slash / to 
show they are folders and not a new
filename for the copied file.

You can abort multi-file copying
by pressing CTRL-C or RUN/STOP
while copying. Note: You may have to
press CTRL-C repeatly until it
stops.

Examples:

Copy a single file within the current
directory (duplicate):
copy somefile.txt somefile-copy.txt

Copy a file into a folder
copy helloworld test-dir/helloworld
copy somefile /somedir/somefile

Copy a file to drive 8:
copy somefile.txt d8:

Copy all the files in the current 
directory to another drive:
copy * d08:

Copy all the files in the current
directory to another partition on the
same device:
copy * d08b:
---------------------------------------
D##: COMMAND

Changes the currently active drive.
This is the drive you're "in".

Drive numbers 8 through 15 are 
supported, depending on your 
hardware.

Example: d09:
---------------------------------------
D##?: COMMAND

Changes the currently active drive.
This is the drive you're "in".

This is the recommended way to change
drives and devices, even when only
one drive or partition exists on any
given device.

The character after the 2 digits is 
the partition. Use a for the first
partition, and b for the second,
etc. The maximum partitions supported
is 9, using letters a through i.

Drive numbers 8 through 15 are 
supported, depending on your 
hardware.

Example: d09b:
---------------------------------------
DATETIME COMMAND

Displays the current date and time.

All time and date handling by ChiCLI
is done on the SD2IEC device
exclusively.

Some versions of the SD2IEC, depending
on hardware or firmware, have a clock
built into the device. One version has
a software clock that only operates
while the device is powered. Once it is
powered off the date and time are lost.

However, there are versions of the
SD2IEC that have, or can be upgraded,
with a real-time clock (RTC).

ChiCLI supports both. You can view
the date and time, and even set the
clock, all without leaving the
environment.

The versions that support some form of
a clock, also automatically save the
time stamp when files are created or
modified on the SD card of the SD2IEC.

Also see the 'filedate' command,
as it allows you to look-up a given
file name and get it's time stamp.

To set the date and time use the
parameter "-set".

Example: datetime
Example: datetime -set
---------------------------------------
DEC2BIN COMMAND

Maths: Converts from a decimal number 
to a binary number.

Example: dec2bin 254
Outputs: BIN:11111110
---------------------------------------
DEC2HEX COMMAND

Maths: Converts from a decimal number 
to a hexidecimal number.

Example: dec2hex 234
Outputs: HEX:00ea
---------------------------------------
DELETE COMMAND

This let's you delete files. You can 
also use * to delete all the files 
within the current disk, image, or 
folder you are currently in.

You can abort multi-file deleting
by pressing CTRL-C or RUN/STOP
while deleting. Note: You may have to
press CTRL-C repeatly until it
stops.

Examples:
delete somefile.txt - Deletes a file
delete *            - Deletes all files
                      in current folder
---------------------------------------
DOS COMMAND

This lets you execute a custom command
intended for a Commodore DOS device.
It expects a second argument to be the
command to send to the currently
selected drive. It uses the command
channel 1. It passes the given string
directly to the Commodore DOS device.

Note: Remember that most examples are
given in upper case because that is the
default Commodore Basic startup mode.
Commodore DOS expects lower case, so
when using this command in ChiCLI, you
need to use lower case, as in the 
example given below. 

Example: 
dos initialize0
---------------------------------------
DRIVEDETECT COMMAND

Gets the drive name string from the
current device.

Example: drive-detect
---------------------------------------
DRIVESET COMMAND

This lets you change the drive number 
of the current drive.

The current drive is automatically
detected and the right method is
used if the drive is supported.

For SD2IEC devices, you can use the
sd2iec-save command to make this 
change in device number permanent.

Currently supported drives:
- Commodore 1542
- SD2IEC
- MSD SD-2

Examples:
drive-set 10
drive-set 11
---------------------------------------
DUMPMEM COMMAND

This let's you dump an area of memory
to a file in the current directory 
you're in. 

The first argument is the starting
location in memory expressed as a
decimal number. 

The second is the ending address in
memory.

The final argument is the name of the
file that should be used to save the
bytes read from memory.

Here's an example:
dump-mem 1000 2000 memory.bin

However, there is also a shortcut,
for situation when you want to
save the content of your kernal or
BASIC ROMs. Use "rom" without 
quote for the first argument, and
either "kernal" or "basic" for 
the second argument. The third
argument is still the file name.

Here's an example:
dump-mem rom kernal kernal.rom
---------------------------------------
ECHO COMMAND

Displays the text as the first
argument. You can also use quotes
around the text you want echoed.

Example: echo "ChiCLI is the COOLEST!"
---------------------------------------
EXIT COMMAND

This exits ChiCLI.

Example: exit
---------------------------------------
FILEDATE COMMAND

If you have an SD2IEC, it is capable
of displaying time stamps for a given
file. This command shows you a time
stamp for the file given, as it has
been recorded on the FAT32 SD card.
If you don't have a RTC installed on
your SD2IEC, then any files saved
by the SD2IEC itself while using a
Commodore 64 will not have accurate
time stamps. However, any files saved
on the SD by a device with a clock
that's set correctly will have the
correct time for when the file was
last modified.

Additionally, if your SD2IEC has the
software clock enabled, and the time
and date have been set since it was
powered on, then it may be possible
to save files with the correct time
stamp information as well.

Please note, as of the writing of this
document, it is possible your modern
computer or device will save the time
stamp information in UTC and then
locally display the time stamps
correctly for your time zone. The
SD2IEC will display whatever time
stamp is saved on the SD card, so
there may be a discrepancy between
the saved time and what you expected.

Attempting to account and adjust for
this is beyond the current scope of
this program, since ChiCLI has moved
all time and date handling to the
SD2IEC device exclusively.

Example: filedate chicli-readme
---------------------------------------
FORMAT COMMAND

Format a disk, based on the device 
your currently in. 
First arg is the disk name.
Second arg is the disk number

If you omit the name, format will 
default to calling the disk by the 
name "untitled. If you omit the disk
ID, format will give it "0" by 
default.

The "format" command now supports the 
Commodore DOS ability to do a quick 
format, instead of a full format. Use 
format -q diskname to quick format 
a disk calling it diskname.

Examples: 
format mydisk 0
format mydisk
format
format -q mydisk
format -q
---------------------------------------
HELP COMMAND

Displays a quick list of commands. 
note: The file chicli-readme.txt 
contains detailed help.

Example: help
---------------------------------------
HEX2DEC COMMAND

Maths: Converts from a hexidecimal 
number to a decimal number.

Example: hex2dec FFFF
Outputs: DEC:65535
---------------------------------------
INITIALIZE COMMAND

Executes the Commodore DOS drive 
command for initialize, based on the 
current drive.

Example: initialize
---------------------------------------
KEYCODES COMMAND

Displays the key code of a key that's 
pressed in decimal value. RUN/STOP or
CTRL+C quits the command.

keycodes
---------------------------------------
LIST COMMAND

This displays a listing of the files
within the disk, image, or folder
you are currently in.

The list command defaults to showing
the detailed, or long, view.

The dir alias default to showing
the two column, or short, view.

Option: -s
This overrides the current default
and displays in the short view.

Option: -l
This overrides the current default
and displays in the long view.

You can also pass a search string to
list, in order to get a list of files
that fit the search pattern.

For example:
list *.d64
...will display all the files ending
in .d64 in the currently directory.

You can also mix options with the
search pattern.

Example: list
Example: list -s
Example: list -l
Example: list *.d64 -s
Example: list -s *.d64
---------------------------------------
LP / LD COMMAND

this command lists the partitions or
drives on the current device.

This will also show the disk label, ID
and free space in blocks. This works
on single drive devices as well as
multi-drive units.

The SD2IEC with it's partitions is
also supported.

The 1581 with it's concept of
partitions is currently untested.

Example: ld
---------------------------------------
MKDIR COMMAND

Makes a new, empty directory, using 
the provided name.

Example: mkdir game-images
---------------------------------------
MOUNT COMMAND

This mounts a disk image on a SD2IEC
device, like the SD2IEC. You can then
copy some of all of the files to a real
floppy disk. 

Example: 
mount chicli-disk.d64
---------------------------------------
PART / DRIVE COMMAND

This quickly changes the current 
partition based on the supplied letter.
The shortcut "part" makes using this 
handy to change partitions fast. Only
works on drives that support partitions.

Example: part c
Example: drive b
---------------------------------------
PEEK COMMAND

Returns the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This takes the address as a 
decimal value.

Example: peek 1000
---------------------------------------
PEEKBIT COMMAND

Returns the value of a given bit 
stored in memory at the given address. 

Example: 
peek 1024 2
This grabs the third least significant 
bit from memory address 1024 in 
decimal format.
---------------------------------------
POKE COMMAND

Updates the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This version takes the address and 
value as a decimal number.

Example: poke 1000 33
---------------------------------------
POKEBIT COMMAND

Writes the value of a given bit 
stored in memory at the given address. 

Example: 
peek 1024 2 1
Writes 1 to the third least significant
bit from memory address 1024 in
decimal format
---------------------------------------
PROFILE COMMAND

Changes the current text, background 
and border colors, to one of a set of
pre-determined profiles.

profile [Profile Number]

1 - PET    5 - C128 - 2
2 - VIC-20 6 - Black on White (Default)
3 - C64    7 - White on Black
4 - C128   8 - Grey

Example: profile 1
---------------------------------------
PWD COMMAND

Shows you the currently set device 
and partition.

Example: pwd
---------------------------------------
REBOOT COMMAND

This restarts the Commodore by calling
the system reset vector.

Note: If you're using a Fast Load
cartridge, this will reset without
Fast Load. You'll have to sys to a
special value to reload the Fast Load
cartidge.

Note: The Epyx Fast Load cartridge
will not be loaded if you call the
reboot command. To re-enable it,
you need to run this command in
Commoodre Basic: sys 57194

Example: reboot
---------------------------------------
RENAME COMMAND

Renames a file.

If a file is using a weird or hard to 
enter character, you can use a wildcard
to rename it.

Example: rename oldfilename newfilename
Example: rename weirdfile* friendlyfile
---------------------------------------
RESTART COMMAND

This exits ChiCLI and automatically
loads and runs ChiCLI from disk again.

Example: restart
---------------------------------------
REMOVEDIR COMMAND

Remvoe a directory.

Example: remove-dir testdir
---------------------------------------
RUN COMMAND

This loads and runs a command after 
exiting the ChiCLI system. You can use
tab completion with this command.

If a file is using a weird or hard to
enter character, you can use a wildcard
to run it.

Example: run kong*
---------------------------------------
SCREENSAVER COMMAND

Displays a screensaver that is a moving
Commodore inspired logo.

The screensaver also displays the date
and time, if used with an SD2IEC.

You can also enable or disable whether
the date and/or time is displayed.

By default the time and date are 
displayed, and only if a SD2IEC 
software or RTC clock is detected.

You can use the following agruments
to toggle the options:

-enable-time
-enable-date
-disable-time
-disable-date

Examples:
screensaver
screensaver -disable-time
screensaver -enable-date
---------------------------------------
SHUTDOWN COMMAND

This exits the program and displays
a message letting you know it's safe
to turn off your Commodore.  

Example: shutdown
---------------------------------------
STATUS COMMAND

This retrives the status of the 
current drive.

Example: status
---------------------------------------
STOPWATCH COMMAND

The "stopwatch" command starts when 
you run it, and when run again, stops 
and outputs the elapsed seconds since 
it was last called. It resets on the 
second call as well. The maximum time 
is about 18 mins.

Example: stopwatch
---------------------------------------
SYS COMMAND

This causes execution to jump to the 
memory location given. It expects a
number in decimal. This works just 
like the Commodore Basic command.

Example: sys 64738
---------------------------------------
SYSINFO COMMAND

Attempts to detect your hardware and
displays information about your system.
It assumes that you've got a standard
stock Commodore. It also plays a chime
based on the chime used on the 
Commodore PET and Commodore PC 
computers.

It will also detect if the system is 
running under emulation. Although it's
not infallible, it will recognise 
VICE as well as TheC64.

Feel free to donate additional hardware
if you'd like more specialized systems
to be recognised.

Example: sysinfo
---------------------------------------
TIME COMMAND

Displays the current time.

To set the date and time use the
command datetime with the
parameter "-set".

Example: time
---------------------------------------
TYPE COMMAND

This is a file viewer. It can display
files in the formats SEQ, PRG, or any
other type of file. 

It attempts to auto-detect the file 
type and shows the relevant output.

For SEQ files, it displays them as if
they were text files. 

For PRG files, it displays them as 
Commodore Basic would display them.
It substitutes the codes for their 
related basic tokens.

If you'd like to view the file's raw
hexidecimal values, you can append
-hex as the last argument. 

Example: 
type chicli
type chicli-readme
type chicli -hex
---------------------------------------
SD2IEC COMMAND

This command does SD2IEC specific 
functions. The second argument
determines which function you're
calling on the device.

-hide-ext

This is a sd2iec specific setting that
causes it to hide files ending in
Commodore DOS extensions, such as
.prg or .seq and instead automatically
reports them to the system as if they
were in fact that type of file.

Please see your sd2iec documentation.

Example:
sd2iec -hide-ext

-show-ext

This is a sd2iec specific setting that
causes it to show files ending in 
Commodore DOS extensions, such as
.prg or .seq and reports all files 
as PRG files unless Commodore DOS
as set specifically.

Please see your sd2iec documentation. 

Example: 
sd2iec -show-ext

-save

Saves the current settings to the 
SD2IEC. For example, if you re-assign
the SD2IEC device 10, the next time
the device is power-cycled, it will
revert back to it's previous setting.
However, using this command, you can 
cause it to save the current settings.
Therefore, in our example, when the 
device is power-cycled, it will start
up set to device 10.

Example: 
sd2iec -save
---------------------------------------
UNMOUNT COMMAND

This unmounts a previously mounted 
disk image. It automatically pops you 
back to the previous folder. 

Example: unmount
---------------------------------------
VALIDATE COMMAND

Executes the Commodore DOS drive 
command for validate, based on the 
current drive.

Example: validate
---------------------------------------
VARS COMMAND

This command shows a list of the
bytes that can be used for certain
system functions.

Command Line:
The number of characters that can be
entered into the command line.

Args:
The number of characters that can be
used as an argument in any given
command.

Disk Buffer:
This is the size, in bytes, of the
buffer used to read and write to the
disk when using custom routines. For
example, when copying files from one
device to another.

Aliases:
This is the total number of aliases
that are available to be used.

Alias:
This is how long, in characters, any
given alias can be.

Hotkeys:
This is the total number of hotkeys
that are available to be used.
Currently, the only hotkeys availble
are the function keys.

Hotkey:
This is how long, in characters, any
given hotkey can be.

Example: vars
---------------------------------------
VIEWMEM COMMAND

Displays memory in hexidecimal and in 
plain text value.

Example: view-mem a000
---------------------------------------

I had fun making ChiCLI and I hope you
enjoy using it!

- Chiron Bramberger

---------------------------------------
</pre>
