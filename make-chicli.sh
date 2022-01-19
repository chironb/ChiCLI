cl65 -g -Osr -t c64 --static-locals source/chicli.c source/string_processing.c source/alias.c source/hotkeys.c source/hardware.c source/commands.c -o chicli.prg
rm source/*.o
ls -l *.prg
