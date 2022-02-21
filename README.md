# ChiCLI

A CLI for Commodore Computers!

![ChiCLI v1 Screenshot](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_v1_Screenshot.png?raw=true)

![ChiCLI v1 Screensaver Temperature](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_v1_Screenshot_Screensaver_Temperature.png?raw=true)

![ChiCLI v1 Screenshot Commands](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_v1_Screenshot_Commands.png?raw=true)

![ChiCLI v1 Screensaver Date and Time](https://raw.githubusercontent.com/chironb/ChiCLI/main/screenshots/ChiCLI_v1_Screenshot_Screensaver.png?raw=true)

ChiCLI v1.05 (c) 2020 - 2022 Chiron Bramberger

A Commodore 64 command line interface terminal, inspired by: AmigaDOS, MS-DOS, and Linux.

Pronunciation of ChiCLI: ky see-elle-eye

Quotes from public forums:

*"Your program completes me. I think i may even make my C64 boot directly to it. I like it that much. I have never seen anything this slick!! Major kudos and praise to this dev! It is a must have for every C64 enthusiast and raises the bar substantially! I have never seen or used such a perfect terminal on a C64 in all my decades using the platform. It is extremely powerful and easy to use. It modernizes the system and makes it perform equal to or better then a PC MS-DOS distro command line. Full featured copies quickly from folder to folder unlike CBM manager. It breaks new barriers on this platform! I highly recommend it to everyone. It does advanced stuff like mounting D64 containers on SD2IEC. Directory manipulation you name it! Partition manipulation. This terminal is epic!!! Major cheers to the dev! I'm going to make it my core drive boot!"  
~ LordDarkenbeast, Reddit User*

*"Right on! I think that this is exactly the type of thing the Commodore 64 needs right now."*  
~ Privileged_Interface, Reddit User*

*"I smiled when I did ls."  
~ opi, Reddit User*

*"Oh cool, keeping an eye on this project."  
~ Vijfhoek, Reddit User*

*"Seriously, better than fast blue hogs of the hedge this is man. This looks bosssssss. I got so many f****n disks of absolute crap pirate games to murder, this is gonna be handy af dude"  
~ LLH on Facebook*

*"TAB-completion and I'll take it!"  
~ PS on Facebook*

*"Deym thanks for this! Looks amazing! Will try out soon."  
~ RBJeffrey, Reddit User*

*"Awesome work!"  
~ Oldbitcollector*

*"My new GOTO tool for file management! Thank you!"  
~ JL on Facebook*

*Please note: these public quotes do not imply endorsement of any kind.*

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
- A proper help system, so you can lookup help for any command.

Made in Canada. :-)

You can download a disk image with this program and it's supporting Commodore readable text files under the release section of this GitHub

This software, while made with love, comes with no warranty or guarantees of any kind.

Testing successfully with the following hardware:
- Commodore 64 based on NTSC mainboards: 326298, 250407, 250425, 250466, 250469, Kernal's 2 & 3
- The Commodore SX-64
- TheC64 Mini hardware, which is based on Linux and VICE
- VICE: Emulated Commodore machines: 64, 64C, SX-64, 4064, NTSC, PAL
- SD2IEC made by Jim Brain --> Get One Here --> http://store.go4retro.com/uiec-sd/ (Not Sponsored)
- EasyFlash 3 made by Jim Brain --> Get One Here --> https://store.go4retro.com/easyflash-3/ (Not Sponsored)
- IDE64 & Ultimate 64 tested by Leif Bloomquist --> Leif's GitHub --> https://github.com/LeifBloomquist
- MiSTer tested by NML32 in this video: --> https://www.youtube.com/watch?v=DmkLLup9YLY
- Ultimate 64 tested by Oldbitcollector. Note: When using a Virtual IEC, you need to use "dir *". I'm not sure why. Feel free to donate an Ultimate 64 for me to figure it out and update the software!
- CMD HD and Pi1541 testing graciously provided by Raj W. / ZappedC64. Thanks so much!
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
- Commodore 1581 hardware tested by Glenn Holmer as noted on the TPUG mailing list here: https://www.freelists.org/post/torontocbm/1581-Support,8
- Commodore 1581 additionally tested as emulated under VICE.
- Commodore CMD FD-2000 and CMD FD-4000 additionally tested as emulated under VICE.
- Commodore 1570 and 1571 additionally tested as emulated under VICE.
- VICE FS Driver 2.0 additionally tested as emulated under VICE.
- SuperCPU additionally tested as emulated under VICE.
- IDE64 additionally tested as emulated under VICE.
- SuperCPU tested by 8-Bit Show And Tell --> https://www.youtube.com/c/8BitShowAndTell

*Please note: the people above are thanked for their help, but that does not imply endorsement or fitness for any purpose of any kind.*

Compilation Notes:

- You can download everything and run the compilation script on Linux and Mac OS to build the files and disk image.
- You need cc65 to compile it: https://cc65.github.io/ https://github.com/cc65/cc65
- You also can use cc1541 with the compilation script to automatically create the correctly formatted disk image: https://csdb.dk/release/?id=196448 https://bitbucket.org/PTV_Claus/cc1541/src/master/
- *I will update these instructions in the future.*

Latest News:

v1.05 - Exciting Update! - Feb 21 2022
- Added:     A new "temp" command which reports the temperature of the CPU! Works on standard stock C64 and C64C machines *without* any modifications or extra hardware! It will only attempt to run on currently supported machines.
- Updated:   The "ss" screensaver command can now display the CPU temperature instead of the time and date.
- Added:     A much requested feature: command history! You can now go back and forth in your entered command history, up to five levels deep!
- Added:     A new CTRL+D shortcut that cycles through valid devices just like in JiffyDOS.
- Added:     A new "@" command which is similar to the "dos" command, but executes right away without asking. For people who are usd to JiffyDOS.
- Added:     A new "$" shortcut which is similar to the "list" command. For people who are usd to JiffyDOS.
- Added:     A "supercpu" command that lets you run in Turbo Mode or Normal mode.
- Faster:    The "help" command now loads help much faster than before! The help files have been split up so there's less text to search through.
- Faster:    The "list" command and "cd" command are now positioned to run as quickly as possible, making the program feel faster and more responsive.
- Faster:    SuperCPU users can now enjoy the program program running in Turbo Mode whenever it isn't doing anything disk related. Disk commands are now wrapped such that the system throttles down to normal speed, does the disk stuff, and then immediately returns to Turbo Mode.
- Merged:    The changelabel and changeid commands into one new command called "relabel". It works like this: "relabel chicli-disk 15".
- Merged:    The dec2bin, dec2hex, hex2dec, into one new comamnd "convert" which takes an option to determine what is being converted.
- Updated:   The "stopwatch" command now supports up to 59:59.9 mins/secs/decisecs. It also uses the TOD clock on the CIA instead of counting jiffies.
- Updated:   The "pwd" command now displays the current directory, or disk label, along with the device and partition / drive information.
- Updated:   The "help" command now checks to make sure the help file it's trying to load is in fact a SEQ file.
- Updated:   The help files and manual files with updated information on many commands.
- Updated:   The drive detection takes less in space, is faster, and also now the BusCardII devices that aren't connected are ignored properly.
- Fixed:     If something screws up the cursor color, it is restored the next time the prompt is displayed.
- Fixed:     The "=" maths command now tolerates unspaced input, such as = 2*2 instead of = 2 * 2.
- Fixed:     The alias command now trims whitespace before it runs so it isn't thrown off by something like "myalias " is entered.
- Fixed:     The speed command now returns the expected speed in MHz on systems like the SuperCPU and Ultimate 64.
- Fixed:     The following command will now check the arguments given and will not run unless they are correct: dos, echo, part, peek, peeking, poke, run, sd2iec, sys, run.
- Fixed:     The "run" command and the "./" now check to make sure the given file exists and is actually a PRG program file.
- Fixed:     The "poke" command will now fail if you give it commas. This isn't BASIC.
- Fixed:     A bug in "list" that also revealed itself in the "$" shortcut. Using this could overwrite options for the "ss" screensaver command. This is a strcpy() / strcat() string copying error, as the second string is bigger than the first string, which is blowing past the end of the first string. The solution is *strncpy()* or *strncat()*, since these let you set a specific limit on how many characters you end up copying.
- Renamed:   The "screensaver" command is now just the "ss" command.
- Renamed:   The "driveset" command is now "drvset".
- Renamed:   The "drivedetect" command is now "drvdetect".
- Renamed:   The "initialize" command is now "init".
- Dropped:   The "restart" command was dropped as it wasn't really worthwhile and the RAM and program size could be put to better use.
- Dropped:   The "vars" command, which took up too much room, and wasn't that helpful beyond debugging. The alias and hotkey commands now show the information you would have used vars for in the part. For example the number of aliases and how many characters long they can be.
- Updated:   The build script and automated many tasks to improve the speed with which I can work on and test ChiCLI.

v1.04 - Another Big Update! - Feb 1 2022
- Added:     Essential support for the IDE64! Also supports the real-time clock functions, and the driveset command! Note: Partitions are not yet supported as well as other advanced features of the IDE64. Look out for future updates!
- Added:     A ide64 command. There are only two options at the moment: "ide64 -wp" which turns on write-protection and "ide64 -we" which write-enables the drive.
- Added:     I wrote functions to wait one second and another to wait one decisecond to create delays that don't use CPU cycles as the timing method, so fast C64's still work.
- Added:     Support for SuperCPU and Ultimate 64! Now things don't hang on fast systems. Also, the SuperCPU is automatically disabled and enabled for DOS disk operations. Without this, fast systems screw up the kernal routines that expact exact timing based on the C64's default CPU 6502 1 MHz speed.
- Added:     A new speed command, which shows the estimated speed of the CPU in MHz. It also shows the number of raster scan lines it takes for running it's NOP payload. For PAL systems, this is a little less accurate on the C128, but it's close enough. I don't have any real life PAL machines to test with. This gets less accurate on *shenanigan setups* :-P that run faster than 20 MHz like the Ultimate 64. Who needs a 40 MHz Commodore 64? This is out-of-control. At some point I'll get a Turbo Chameleon 64 for future refinements since I can't find or afford a real SuperCPU and it's ridiculous eBay prices!
- Added:     Support for CMD heads that like using the back arrow for moving up a directory. Like this: "cd←".
- Fixed:     The re-written cd command needs to send the cd DOS command even if the current drive isn't known to explictly support it. This should help with custom SD2IEC firmware.
- Fixed:     The ./ command used to exit the program when no file name was given. Now it gives an error when you don't give it a file name to execute.
- Fixed:     Some devices are sensitive to the amount of time they are given to reset. I've added an additional decisecond delay to the drive reset command. 
- Updated:   The sysinfo command, which now displays the current estimated speed in MHz using the speed command. For the C128, it shows it's maximum speed, even though ChiCLI runs at regular 1 MHz speed.
- Updated:   The sysinfo command, which now detects and displays the WDC 65816 cpu.
- Updated:   The basic keywords array is now only as big as the text needs, through an array of pointers to strings. This saved about 111 bytes!
- Updated:   The list command to use only KB even with decimal. For example it now lists 0.25 KB instead of 256 B. So now you can use VICE with a listing and copy and paste the text into a spreadsheet and all the file sizes are in the same unit of measurement.
- Added:     More space to suppport all this new stuff! I can't believe how, with every version, I run out of room. But then somehow I find another trick to free up some space. But that's why I use C Programming and not assembly. I want to get better at writing tight C code. I had to reduce the amount of aliases from 8 to 7 for now, but in a future I'll bring this back to 8.

v1.03 - Small But Important Update! - Jan 17 2022
- Fixed:     A very rare but old bug where the cursor home key (CTRL+U) and cursor end key (CTRL+O) would lead to a corrupt display.
- Updated:   Project on GitHub and file organization so things are little better put together.

v1.02 - Mega Refinement Update! - Jan 17 2022
- Added:     Support for FD-2000 and FD-4000 drives.
- Added:     Support for 1571 and 1570 drives.
- Added:     Support for the VICE FS Driver 2.0 which currently *does not* support the local DOS copy command. But you can still copy from another device into a VICE FS device and vice-versa.
- Added:     A command, "1571", to put the drive into single-sided or double-sided mode.
- Added:     Compatibility for Pi1541 by waiting just a little longer for it to reset when doing drive detection.
- Added:     A new mode to the help command for hardware, which searches a separate hardware file. To use it type: "help 1581 -h" and it will output specific support information for the 1581.
- Updated:   Usability is better now. The commands: cd, d##?:, d##:, and copy, all accept d08: d08; or d08[, which means that muscle memory between using VICE on a PC and using a *real* Commodore 64 is the same.
- Updated:   The command "chirp" and "sysinfo" now produce a more authentic Commodore PET and Commodore PC style startup chime.
- Updated:   The command "chirp" and "sysinfo" now also produce almost in-tune chimes on PAL. I say almost because using stretch tuning on the highest note doesn't work, because PAL systems can't go that high in terms frequency.
- Updated:   The SID detection works a little more flexibly. It now doesn't hang when there is no SID installed, or if a non-standard SID is installed, such as the modern ARMSID.
- Updated:   The overall hardware detection now gracefully only displays basic information when it can't reliably determine the machine's details. For example it will show things like SID and VIC-II without any more details.
- Updated:   The sysinfo command so that it detects from device 8 to 15.
- Updated:   The sysinfo command now has nicer looking output, starting at the top and working it's way down, without leaving any empty lines.
- Updated:   The type command no longer tries to type CBM "files" which are actually 1581 partitions.
- Updated:   The type command so that you can force outputing as if the file were a program file. You can also use just one letter for the option.
- Updated:   The driveset command now supports changing the device number for the following devices: 1541, SD-2, SD2IEC, CMD HD, 1570, 1571, 1581.
- Note:      There are now, technically, 64 commands, which is fitting. :-)

v1.01 - First Post-Final-Release Update! - Jan 7 2022
- Added:     Support for the CMD HD! Also supports the real-time clock functions!
- Added:     Support for the VICE FS Driver v2.0!
- Added:     More support for the 1581. You can now move in and out of partitions like folders using the 'cd' command. To return to the root partition, use 'cd /' just like any other drive that supports folders. This should also let you enter nested sub-partitions. Also... the 1581... OMG shoot me in the face...
- Help:      Added a little section in the help for hardware support. Use 'help hardware' to bring it up.
- Fixed:     Much rework done to the 'cd' and 'part' partition commands.
- Fixed:     The delete command wasn't getting all the files on the first pass. Now it does. Needed to do some jumping around to restart the scan after every delete.
- Fixed:     The Commodore 128 detection wasn't quite right for detecting DCR. These have the newer 9v SID, so this has been updated.
- Optimized: Reworked many things to make more room for all the added hardware support.
- Note:      The VICE FS Driver doesn't support copy, as noted in the VICE source code: VICE in file fsdevice-flush.c line 809 and 810:     } else if (*cmd == 'c' && arg != NULL) {        /* FIXME: not implemented */
- Comments:  Swearing... so much swearing...

v1.00 - Final Release - Jan 1 2022
- Updated:   This is ready for a version 1.0 release!
- Fixed:     Added support so that now both stock 1581 drives and JiffyDOS upgraded 1581 drives are detected and handled properly.
