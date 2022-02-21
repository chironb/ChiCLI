# ChiCLI Script to Build the program, support files, and generate the 1541 D64 Disk Image.
# See: https://github.com/chironb/ChiCLI
#
# This script has the following dependancies:
# Linux or MacOS
# The cc65 Compiler:
# cc65   See: https://github.com/cc65/cc65 and also: https://cc65.github.io/
#
# The 1541 D64 Image Management Tool cc1541:
# cc1541 See: https://bitbucket.org/PTV_Claus/cc1541/src/master/ and also: https://csdb.dk/release/?id=196448

if [ -f "./chicli-package/chicli-files/chicli.prg" ];          then rm ./chicli-package/chicli-files/chicli.prg;          fi;

if [ -f "./chicli-package/chicli-files/chicli-help-0.seq" ];   then rm ./chicli-package/chicli-files/chicli-help-0.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-help-1.seq" ];   then rm ./chicli-package/chicli-files/chicli-help-1.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-help-2.seq" ];   then rm ./chicli-package/chicli-files/chicli-help-2.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-help-3.seq" ];   then rm ./chicli-package/chicli-files/chicli-help-3.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-help-h.seq" ];   then rm ./chicli-package/chicli-files/chicli-help-h.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-licence.seq" ];  then rm ./chicli-package/chicli-files/chicli-licence.seq;  fi;
if [ -f "./chicli-package/chicli-files/chicli-manual.seq" ];   then rm ./chicli-package/chicli-files/chicli-manual.seq;   fi;
if [ -f "./chicli-package/chicli-files/chicli-readme.seq" ];   then rm ./chicli-package/chicli-files/chicli-readme.seq;   fi;

if [ -f "./chicli-package/chicli-diskimage/chicli-disk.d64" ]; then rm ./chicli-package/chicli-diskimage/chicli-disk.d64; fi;

if [ -f "./chicli-package/chicli-package.zip" ];               then rm ./chicli-package/chicli-package.zip;               fi;

cl65 -g -Osr -t c64 --static-locals source/chicli.c source/string_processing.c source/alias.c source/hotkeys.c source/hardware.c source/commands.c source/cursor.c -o ./chicli-package/chicli-files/chicli.prg

if [ $? -eq 0 ] 
then
   echo "Compiled okay!"
else
   echo "Compilation error. Aborting..."
   exit
fi

rm source/*.o 2>/dev/null # Is this really okay?
rm source/*~  2>/dev/null # Is this really okay?
rm ./*~       2>/dev/null # Is this really okay?

# Invert the character case for use on the C64, and save the files into the correct location.
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-help-0.seq.txt  > ./chicli-package/chicli-files/chicli-help-0.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-help-1.seq.txt  > ./chicli-package/chicli-files/chicli-help-1.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-help-2.seq.txt  > ./chicli-package/chicli-files/chicli-help-2.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-help-3.seq.txt  > ./chicli-package/chicli-files/chicli-help-3.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-help-h.seq.txt  > ./chicli-package/chicli-files/chicli-help-h.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-licence.seq.txt > ./chicli-package/chicli-files/chicli-licence.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-readme.seq.txt  > ./chicli-package/chicli-files/chicli-readme.seq
sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/' source/chicli-manual.seq.txt  > ./chicli-package/chicli-files/chicli-manual.seq

./cc1541 -q -n chicli-disk -i 15 ./chicli-package/chicli-diskimage/chicli-disk.d64
echo "Created disk image."

./cc1541 -q -f chicli         -T PRG -w ./chicli-package/chicli-files/chicli.prg         ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
echo "Added program file to disk image."

./cc1541 -q -f chicli-readme  -T SEQ -w ./chicli-package/chicli-files/chicli-readme.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-help-0  -T SEQ -w ./chicli-package/chicli-files/chicli-help-0.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-help-1  -T SEQ -w ./chicli-package/chicli-files/chicli-help-1.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-help-2  -T SEQ -w ./chicli-package/chicli-files/chicli-help-2.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-help-3  -T SEQ -w ./chicli-package/chicli-files/chicli-help-3.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-help-h  -T SEQ -w ./chicli-package/chicli-files/chicli-help-h.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
./cc1541 -q -f chicli-manual  -T SEQ -w ./chicli-package/chicli-files/chicli-manual.seq  ./chicli-package/chicli-diskimage/chicli-disk.d64 1> /dev/null
echo "Added help files to disk image."

echo "Adding the last file to the disk image..."
./cc1541 -f chicli-licence -T SEQ -w ./chicli-package/chicli-files/chicli-licence.seq ./chicli-package/chicli-diskimage/chicli-disk.d64 

zip -r -q ./chicli-package/chicli-package.zip ./chicli-package/*

ls -l ./chicli-package/chicli-files/chicli.prg
