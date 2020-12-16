# ChiCLI
ChiCLI v0.18 (c) 2020 Chiron Bramberger - A CLI for Commodore Computers!

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot.png?raw=true)

This program works in a any that is similar to and inspired by Commodore DOS, AmigaDOS, MS-DOS, and Linux. 

It features a command line interface, which can list files, rename them, copy them, delete them, as well as view them, and with an UIEC device it can extract disk image files and copy them to a real floppy disk drive. It also features a hardware scanning system info tool, a screensaver with clock, the ability to view memory and files with a hex viewer, as well as load and run other from within. It can also change the drive number assignment for 1541 and UIEC drives, peek and poke memory, read and report keyboard key codes, set custom display colors, and execute user input dos commands, all without leaving the interface. Made in Canada. :-)

You can download a disk image with this program and it's supporting Commodore readable text files here: 
(https://github.com/chironb/ChiCLI/raw/main/chicli-disk.d64)

This is beta quality software. It comes with no warranty or guarantees of any kind.

I’ve tried it with the following hardware: 
- uIEC/SD by Jim Brain --> Buy One Here! --> http://store.go4retro.com/uiec-sd/
- IDE64 tested by Leif Bloomquist --> https://github.com/LeifBloomquist
- Commodore 1541 Disk Drive
- Commodore 2031 via a Batteries Included IEEE-488 BusCardII
- Commodore SFD 1001 via a Batteries Included IEEE-488 BusCardII

Latest News:
- Updated type: added option -text to force it display the file as if it were a text file. Now type can auto-detect the filetype and display it accordingly, or it can be put into -hex mode or -text mode. 
- Added a new shortcut alias rn for rename, and a new shortcut cler for clear. This is because if you type fast on a Commodore keyboard, certain key combinations tend to get lost, because the keyboard doesn't have any anti-ghosting diodes. For example pressing and holding R and then E blocks E for registering. Pressing and holding E then A blocks A. 
- Made a little more room in RAM for type update, by re-writing the CBM error codes text to be shorter but mostly as informative. 
- FIXED: Run command now supported running on any drives. NOTE: The command ./ does not, but that's because it's intended for a different purpose, basically letting you run commands that are small and external as if they were built into the system. BUG: It can only load files from the drive it was loaded from. Running programs from a different drive issue. Also, Exomizer version works inconsistently with loading and running files from within ChiCLI. Not sure why, probably needs some tweaking in the way exomizer is configured in terms of memory layout. I think the fix for one is the fix for both, which is to use the dracopy method for loading external programs. 
- FIXED: If you set the date and time more than once, it gets screwy. Fix: needed to reset to zero the time offset variable in set_date();
- FIXED: Displays both Aliases empty and aliases full. Process: start ChiCLI, set datetime, run screensaver, then alias (with none set). Fix was that result, a global var, wasn't being cleared before being used in the alias code. So it was assumed, erroniously, that it would be 0 unless changed by the testing for the alias list being full. This is the problem with global variables, obviously, but in such a limited system, it's faster and makes more sense to use them. 
- FIXED: Doing the command 'type basic-program' would print out minus signs weird. Turns out, I entered another unicode character instead of a normal keyboard hyphen - character. I made a basic program to barf out every single command so I can check that I'm translating the tokens properly.
- Added ss = screensaver to built-in aliases. 
- FIXED: Command screensaver now respects the currently set text color as well. 
- FIXED: Command sys-info wasn't using the current text color. Bug in color-set was fixed, where the global car tracking the current text color is now updated properly. 
- FIXED: When displaying a Basic file using type, it makes the Basic commands in uppercase, but the screen is in lower case mode, so it should be in lowercase. 
- FIXED: When displaying a Basic file using type, case 174 : printf("↑") wouldn't work, because I'm using a unicode character in the cc65 source code. Fixed with printf("%c", 94 ) instead, which sends the correct PETSCII character. 
- FIXED: Remaining free disk space is often reported wrong and too small. It needed a long int.
- FIXED: File sizes reported are sometimes wonky. Was using 1024 instead of 1000 to divide into kilobytes. 
- FIXED: When exiting to dos, you need to enter NEW before loading anything 
- FIXED: Need to inform the user when they can't set another alias because all the slots are full
- FIXED: Setting a background color to red screws up the red part of displaying the Commodore logo on sys-info.
- FIXED: Figured out how to make colors and color profiles consistent and retained after calling an external program and returning back into ChiCLI using the -sf option. 
- Converted last aliases to built-in, saving migs and megs of RAMs, and freeing up slots for users. There are 8 free now. 
- DONE: Updated readme's with a list of the built-in common aliases.
- FIXED: Copy path bugs. Now you can do this: --> copy sauce tartar  --> Copies "sauce" in the current folder to another file in the current folder called "tartar" --> copy sauce /tartar --> Copies "sauce" in the current folder to another file in the root folder called "tartar" --> copy sourcedir/sauce targetdir/tartar --> Copies "sauce" in the current folder's folder called sourcedir to the current folder's folder called targetdir named "tartar"
- Copy now works with paths. You can now do things like copy * //allfiles/ or copy //somefile //somedir/newfilename! You must end folder with a slash / as it doesn't yet automatically figure out if you mean copy the file to this filename, or copy this file to this folder name using the original implied filename. 
- The most common alias are built into the system, so they don't take up alias slots. However, there's only 8 now, at least until I squeeze out some more ram. 
- The list command takes up one less screenline when displaying.
- Tab completion now works when the previous separator is '/' and not just a ' ' space. This means you can type ./somefile and hit tab, and it will complete it something like ./somefilename, which is pretty cool! It also works like this: copy somefile /somed and it would complete it to copy /somedir allowing you to then finish typing copy /somedir/somefile which is also pretty cool!
- Added ability to launch in -skiptitle or -st mode, which skips the hardware screen gets you to the prompt faster. This is also created for external programs that are launched from it and then re-load and re-run it, created a more seamless experience. There are two sample files to test this with: echoarg and lorem. Echoarg just prints whatever you arguments you send it. Lorem just prints out the lorum ipsum text. They both call ChiCLI -sf when they finished so it doesn't appear like you've left the system. 
- Added ability to write current config to the EEPROM of the UIEC 
- FIXED: When copying files, if there’s an error, it borks the whole system.
- FIXED: Pressing RUN/STOP on the bottom line screws the cursor up.
- FIXED: debug-args: 4 args is supported (should be 5).
- Now supports setting UIEC devices to hide or show extensions!
- Added make-dir and remove-dir commands.
- Also supports tab completion!
- Tab completion how hides the cursor until it is finished searching.
- Now supports bulk deleting and copying of files!
- Added support for drive numbers 8 through 15.

Known Bugs:
- Fixed'em for now. Pretty cool, eh?

Known Issues:
- Tough one: debug-args: When using debug-args, or looking at argv[0] directly, the argv program name loads wrong text, usually something from a printf statement. It’s as if the storing of text for the printf statements are overwriting the part of memory where the file name is stored. 
Here are some links to help:
https://github.com/cc65/cc65/blob/master/libsrc/c64/mainargs.s
https://github.com/cc65/cc65/blob/master/asminc/c64.inc

Notes:
- The exomiser version is not guaranteed to work properly. It is provided for people with disk drives but without any fast loaders of any kind. 

Removals:
- Due to trying to squeeze every feature into about 50K, the maximum aliases is now 8 (eight), but common ones are built-in now.
- Due, again, to not having enough RAM, the hotkey command can only be 20 characters in size. 
- Removed displaying the file name when using debug-args, since that's not working and isn't an easy fix at the moment
- Had to trim more text away from about and version. Also, help and licence simply suggest using the 'type' command to view the chicli-readme and chicli-licence files. 

Opportunities for Optimization of CPU and RAM Usage:
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
Quick Summary
PET : 31K of memory is available for C programs.
C64 : 50K of memory is available for C programs.
128 : 41K of memory is available for C programs.
</pre>

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_format.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_del_star.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_copy_star.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_type_seq.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_type_prg.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_type_hex_1.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_type_hex_2.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_datetime.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_hotkey.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_alias.png?raw=true)

![alt text](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_screenshot_screensaver.png?raw=true)

<pre>

---------------------------------------
ChiCLI - Quick Command List
---------------------------------------


help   about    version     sys-info  
alias  hotkey   profile-set color-set 
clear  list     d#:   cd    run   ./  
type   copy     rename      delete    
format status   dos-command drive-set 
peek   sys      validate    view-mem  
poke   keycodes screensaver initialize
exit   reboot   restart     shutdown  
echo   time     datetime    licence   
chirp  make-dir remove-dir  debug-args
uiec-hide-ext   
uiec-show-ext         
uiec-save-config


---------------------------------------
Built-In Aliases
---------------------------------------

help              - ?
clear             - cls
version           - ver
licence           - lic
exit              - quit, bye, endcli
dos-command       - dc
make-dir          - md, mkdir
remove-dir        - rd, rmdir
delete            - dl, del, rm
rename            - ren
copy              - cp
type              - cat
list              - ls, dir, directory
screensaver       - ss

---------------------------------------
ChiCLI - Detailed Help
---------------------------------------

USER INTERFACE

---------------------------------------
Text Entry

ChiCLI uses custom routines to get 
manage the input and editing of text
on the command line. It automatically
inserts characters, and supports 
backspace and delete. It also supports
cursor keys and a home and end keys.

The Commodore cursor keys are supported
as well as extra keys to make it easier
for people who are coming from the 
standard Linux or Windows keyboard 
layouts.

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

Commodore 64: Use the Left Arrow Key

On the Commodore 64 there is no TAB 
key on the keyboard. The closest is the
LEFT ARROW key above the CTRL key at 
the far left of the keyboard. 

In order to enter a left arrow 
character, for example if you need to 
load a file name that contains the 
left arrow character, you can press:
CTRL + LEFT ARROW 
to enter this character.

The tab completion will start searching
the current folder for files whose
beginning match the last string of 
text entered on the command line that's
separated by an empty space. 

Please note: Nothing appears to happen
as it takes a long time to go through 
all the files in the current folder. 
It's very slow when compared with 
faster systems. 
---------------------------------------
Hotkeys

Hotkey manages the ability to assign
command strings to the function keys. 

Examples:
hotkey                  - Lists hotkeys
hotkey 1 = "echo Hi!"   - Assigns F1
hotkey 2 clear          - Assigns F2
hotkey 1 " "            - Clears F1
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

alias                   - Lists aliases
alias hi = "echo Hi!"   - Sets an alias
alias cls clear         - Sets an alias
alias cls               - Clears alias
alias -clear            - Clears all

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

-----

Disable Drive Detect: -ddd

This disabled drive detection. This is
also disabled once you change drive 
numbers in software. In the future, 
this will be addressed differently. 

Example:
LOAD"CHICLI",8
RUN:REM -DDD

-----

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
Compressed ChiCLI for Faster Loading

ChiCLI can be compressed using various
crunching utilities. 

Exomizer is being tested with ChiCLI,
and you can download the compressed
version called chicli-exo.prg in the 
project's folder. 

The program is compressed by about 50%
and decompresses in about 5 seconds. 

The passing of arguments to a 
compressed version of ChiCLI has been 
tested and works.
---------------------------------------

COMMANDS

---------------------------------------
about 

This displays information about 
the software, as well as links to the 
project on the web.

Example: about
---------------------------------------
version

This displays version information as 
well as the compile date and time.

Example: version
---------------------------------------
licence

This displays the full text of this
licencing information.

Example: licence
---------------------------------------
help 

Displays a quick list of commands. 
note: The file chicli-readme.txt 
contains detailed help.

Example: help
---------------------------------------
chirp

Plays the Commodore PET inspired chime.

Example: 
---------------------------------------
echo 

Displays the text as the first 
argument. You can also use quotes 
around the text you want echoed. 

Example: echo "ChiCLI is the COOLEST!"
---------------------------------------
clear  

This clears the screen of text. 

Example: clear
---------------------------------------
list 

This displays a listing of the files 
within the disk, image, or folder 
you are currently in.

Example: list
---------------------------------------
d#: 

Changes the currently active drive.
This is the drive you're "in".

Drive numbers 8 through 15 are 
supported, depending on your 
hardware.

Example: d9:
---------------------------------------
cd 

This let's you change the current 
working folder. It also lets you move
to the previous or root folders

Example: move into a folder
cd sidfiles

Example: move up one folder
cd .. or cd..

Example: move to the root foler
cd /
---------------------------------------
status 

This retrives the status of the 
current drive.
---------------------------------------
drive-set

This lets you change the drive number 
of the current drive.
 
Examples: 
drive-set -1541 10
drive-set -uiec 11 
---------------------------------------
uiec-hide-ext

This is a uiec specific setting that
causes it to hide files ending in 
Commodore DOS extensions, such as
.prg or .seq and instead automatically 
reports them to the system as if they 
were in fact that type of file. 

Please see your UIEC documentation. 

Example:
uiec-hide-ext
---------------------------------------
uiec-show-ext

This is a uiec specific setting that
causes it to show files ending in 
Commodore DOS extensions, such as
.prg or .seq and reports all files 
as PRG files unless Commodore DOS
as set specifically.

Please see your UIEC documentation. 

Example:
uiec-show-ext
---------------------------------------
mount

This mounts a disk image on a UIEC
device, like the SD2IEC. You can then
copy some of all of the files to a real
floppy disk. 

Example: 
mount chicli-disk.d64
---------------------------------------
unmount

This unmounts a previously mounted 
disk image. It automatically pops you 
back to the previous folder. 

Example:
unmount
---------------------------------------
delete

This let's you delete files. You can 
also use * to delete all the files 
within the current disk, image, or 
folder you are currently in.

Examples:
delete somefile.txt - Deletes a file
delete *            - Deletes all files
                      in current folder
---------------------------------------
copy    

This let's you copy files. You can 
also use * to copy all the files 
within the current disk, image, or 
directory/folder you are currently in, 
to another drive number. 

Folders must end with a slash / to 
show they are folders and not a new
filename for the copied file. 

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
copy * d8:
---------------------------------------
rename 

Renames a file. 

Example: rename oldfilename newfilename
---------------------------------------
make-dir

Create a directory. 

Example: 
make-dir test-dir
---------------------------------------
remove-dir

Remvoe a directory. 

Example: 
remove-dir test-dir
---------------------------------------
format 

Format a disk, based on the device 
your currently in. 
First arg is the disk name.
Second arg is the disk number

Example: format mydisk 0 
---------------------------------------
initialize 

Executes the Commodore DOS drive 
command for initialize, based on the 
current drive.
---------------------------------------
validate  

Executes the Commodore DOS drive 
command for validate, based on the 
current drive.
---------------------------------------
run 

This loads and runs a command after 
exiting the ChiCLI system. You can use
tab completion with this command.

Example:
run kong-arcade
---------------------------------------
./

This loads and runs a command after 
exiting the ChiCLI system. This doesn't
work autocomplete.

Example:
./kong-arcade
---------------------------------------
sys   

This causes execution to jump to the 
memory location given. It expects a
number in decimal. This works just 
like the Commodore Basic command.

Example: 
sys 64738
---------------------------------------
dos-command      

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
dos-command initialize0
---------------------------------------
peek      

Returns the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This version takes the address as a 
decimal value.

Example: 
peek 1000
---------------------------------------
poke      

Updates the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This version takes the address and 
value as a decimal number.

Example: 
poke 1000 33
---------------------------------------
peek-hex   

Updates the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This version takes the address and
value as a hexidecimal number, instead 
of a decimal value.

Example: 
peek-hex ffff 
---------------------------------------
poke-hex     

Returns the value stored in memory at
the given address. Works just as in 
Commodore Basic.

This version takes the address as a
hexidecimal value, instead of a 
decimal value.

Example: 
poke-hex ffff ab
---------------------------------------
view-mem   

Displays memory in hexidecimal and in 
plain text value.

Example: 
view-mem a000
---------------------------------------
keycodes    

Displays the key code of a key that's 
pressed in decimal value. RUN/STOP or
CTRL+C quits the command.

keycodes 
---------------------------------------
type    

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
sys-info     

Attempts to detect your hardware and
displays information about your system.
It assumes that you've got a standard
stock Commodore. It also plays a chime
based on the chime used on the 
Commodore PET and Commodore PC 
computers. 

Feel free to donate additional hardware
if you'd like more specialized systems
to be recognised. 
---------------------------------------
screensaver    

Displays a screensaver that is a moving
Commodore logo and the current time. 

Note: Use datetime -set to set the 
correct time. 

If you'd like ChiCLI to support a 
real-time clock, please feel free to 
donate the relevant hardware. 
---------------------------------------
datetime       

This displays the current date and time.

To set the date and time enter:
datetime -set
---------------------------------------
time     

Displays the current time. 

To set the date and time enter:
datetime -set
---------------------------------------
profile-set    

Changes the current text, background 
and border colors, to one of a set of
pre-determined profiles. If you simply
enter profile-set without any args, 
it will display a list of available 
profiles and prompt you to choose one.

Example: 
profile-set 
profile-set 1
---------------------------------------
color-set   

Changes the current text, background 
and border colors, allowing you to set
each individually. It will display a
list of colors and prompt for each of
text, background and border colors
---------------------------------------
detect-filetype

This detects the Commodore file type of
a given file. 

Example: 
detect-filetype chicli-readme
---------------------------------------
exit      

This exits ChiCLI.
---------------------------------------
restart      

This exits ChiCLI and automatically
loads and runs ChiCLI from disk again. 
---------------------------------------
reboot   

This restarts the Commodore by calling
the system reset vector. 

Note: If you're using a FastLoad 
cartridge, this will reset without 
FastLoad. You'll have to sys to a 
special value to reload the FastLoad
cartidge. 

TODO: Update with FastLoad memory value
---------------------------------------
shutdown   

This exits the program and displays
a message letting you know it's safe
to turn off your Commodore.  
---------------------------------------
debug-args   

This program supports cc65's method for
working with arguments passed to the 
program from within Commodore Basic.

It uses the following format: 
RUN:REM ARG1 "ARG2 QUOTED" ARG3 "" ARG5

You can type in this command to read
the arguments passed as well as the 
program file name. 

Example: 
debug-args

Example output:
Number of args:5
Arg 1:arg1
Arg 2:arg2 quoted
Arg 3:arg3
Arg 4: 
---------------------------------------
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

