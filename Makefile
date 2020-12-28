# Genesis Plus - Sega Mega Drive emulator
# (c) 1999, 2000, 2001, 2002, 2003  Charles MacDonald

# -DDEBUG     - Enable debugging code

CC	=	gcc

INCLUDE	=	-I.			\
		-Icpu			\
		-Im68k			\
		-Isound			\
		-Iwin			\
		-I/usr/include/SDL2

LIBS	=	-lm -lSDL2 -lz

#Note: m68k/m68k_in.c and m68k/m68kmake.c not included

SOURCE	=	fileio.c		\
		genesis.c		\
		io.c			\
		loadrom.c		\
		mem68k.c		\
		membnk.c		\
		memvdp.c		\
		memz80.c		\
		render.c		\
		system.c		\
		unzip.c			\
		vdp.c

SOURCE	+=	cpu/z80.c

SOURCE	+=	m68k/m68kcpu.c 		\
		m68k/m68kopac.c		\
		m68k/m68kopdm.c		\
		m68k/m68kopnz.c		\
		m68k/m68kops.c

SOURCE	+=	sound/fm.c 		\
		sound/sn76496.c		\
		sound/sound.c

SOURCE	+=	win/main.c		\
		win/error.c

EXE	=	genesisplus

all	:	
		$(CC) $(SOURCE) $(INCLUDE) -o $(EXE) $(LIBS)

clean	:	        
		rm -f genesisplus
	        
#
# end of makefile
#

