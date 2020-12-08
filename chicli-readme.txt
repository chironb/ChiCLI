---------------------------------------
ChiCLI - Detailed Help
---------------------------------------

USER INTERFACE

---------------------------------------
Text Entry

TODO: Write this section.
---------------------------------------
Cursor keys

TODO: Write this section.
---------------------------------------
Hotkeys

TODO: Write this section.
---------------------------------------
Aliases

TODO: Write this section.
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
the current directory for files whose
beginning match the last string of 
text entered on the command line that's
separated by an empty space. 

Please note: Nothing appears to happen
as it takes a long time to go through 
all the files in the current directory. 
It's very slow when compared with 
faster systems. 
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
alias

This lets you review aliases, as well 
as create your own.

Example: TODO: All the uses. 
---------------------------------------
hotkey

This lets you review and set function 
keys as hotkeys. 

Example: 
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
within the disk, image, or directory 
you are currently in.

Example: list
---------------------------------------
d#: 

Changes the currently active drive.
This is the drive you're "in".

Example: d9:
---------------------------------------
cd 

This let's you change the current 
working directory. 

Example: cd sidfiles
---------------------------------------
status 

This retrives the status of the 
current drive.

Examples: 
drive-set -1541 10
drive-set -uiec 11 
---------------------------------------
drive-set

This lets you change the drive number 
of the current drive.

Example: 
---------------------------------------
delete

This let's you delete files. You can 
also use * to delete all the files 
within the current disk, image, or 
directory you are currently in.

Examples:

Delete a single file:
delete somefile.txt

Delete all files in current directory
delete * 
---------------------------------------
copy    

This let's you copy files. You can 
also use * to copy all the files 
within the current disk, image, or 
directory/folder you are currently in, 
to another drive number. 

Examples:

Copy a single file within the current
directory (duplicate):
copy somefile.txt somefile-copy.txt

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
format 

Format a disk, based on the device 
your currently in. 
First arg is the disk name.
Second arg is the disk number

Example: format mydisk 0 
---------------------------------------
initialize 
Example: 
---------------------------------------
validate  
Example: 
---------------------------------------
run 

TODO: Write this section.

Example: 
---------------------------------------
./

TODO: Write this section.

Example: 
---------------------------------------
sys   

TODO: Write this section.

Example: 
---------------------------------------
dos-command      

TODO: Write this section.

Example: 
---------------------------------------
peek      

TODO: Write this section.

Example: 
---------------------------------------
poke      

TODO: Write this section.

Example: 
---------------------------------------
peek-hex   

TODO: Write this section.

Example: 
---------------------------------------
poke-hex     

TODO: Write this section.

Example: 
---------------------------------------
view-mem   

TODO: Write this section.

Example: 
---------------------------------------
keycodes    

TODO: Write this section.

Example: 
---------------------------------------
type    

TODO: Write this section.

Example: 
---------------------------------------
sys-info     

TODO: Write this section.

Example: 
---------------------------------------
screensaver    

TODO: Write this section.

Example: 
---------------------------------------
datetime       

TODO: Write this section.

Example: 
---------------------------------------
time     

TODO: Write this section.

Example: 
---------------------------------------
profile-set    

TODO: Write this section.

Example: 
---------------------------------------
color-set   

TODO: Write this section.

Example: 
---------------------------------------
exit      

TODO: Write this section.

Example: 
---------------------------------------
restart      

TODO: Write this section.

Example: 
---------------------------------------
reboot   

TODO: Write this section.

Example: 
---------------------------------------
shutdown   

TODO: Write this section.

Example: 
---------------------------------------