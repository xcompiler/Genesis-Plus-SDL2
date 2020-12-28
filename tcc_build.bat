tcc.exe fileio.c genesis.c io.c loadrom.c mem68k.c membnk.c memvdp.c memz80.c render.c system.c unzip.c vdp.c cpu/z80.c m68k/m68kcpu.c m68k/m68kopac.c m68k/m68kopdm.c m68k/m68kopnz.c m68k/m68kops.c sound/fm.c sound/sn76496.c sound/sound.c win/main.c win/error.c -I. -Icpu -Im68k -Isound -Iwin -ISDL2 -o genesisplus.exe -lSDL2 -lzlib

