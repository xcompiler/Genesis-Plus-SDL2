#ifndef _SHARED_H_
#define _SHARED_H_


#include <stdio.h>
#include <math.h>
#include <zlib.h>


/* The error definitions below originally came from error.h */

/* Global variables */
extern FILE *error_log;

/* Function prototypes */
extern void error_init(void);
extern void error_shutdown(void);
extern void error(char *format, ...);


#include "types.h"
#include "macros.h"
#include "m68k.h"
#include "z80.h"
#include "genesis.h"
#include "vdp.h"
#include "render.h"
#include "mem68k.h"
#include "memz80.h"
#include "membnk.h"
#include "memvdp.h"
#include "system.h"
#include "unzip.h"
#include "fileio.h"
#include "loadrom.h"
/* #include "io.h" */
#include "io_genesisplus.h" /* io.h has been renamed to io_genesisplus.h, as it was conflicting with the io.h file used by TCC */
#include "sound.h"
#include "fm.h"
#include "sn76496.h"
#include "osd.h"

#endif /* _SHARED_H_ */

