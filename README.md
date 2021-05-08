# ymfm
BSD-licensed Yamaha FM sound cores (OPM, OPN, OPL, and others)

## Supported environments

This code should compile cleanly in any environment that has C++17 support.
It has been tested on gcc, clang, and Microsoft Visual C++ 2019.

## Supported chip families

Currently, support is present for the following chips:

* ymfm_opm.h:
** YM2151 (OPM)
** YM2164 (OPP)
* ymfm_opn.h:
** YM2149 (SSG)
** YM2203 (OPN)
** YM2608 (OPNA)
** YM2610 (OPNB)
** YM2610B (OPNB2)
** YM2612 (OPN2)
** YM3438 (OPN2C)
** YMF276 (OPN2L)
* ymfm_opl.h:
** YM3526 (OPL)
** Y8950 (MSX-Audio)
** YM3812 (OPL2)
** YMF262 (OPL3)
** YM2413 (OPLL)
** YM2423 (OPLL-X)
** YMF281 (OPLLP)
** DS1001 (Konami 053982)
* ymfm_opq.h:
** YM3806 (OPQ) -- preliminary
* ymfm_opz.h:
** YM2414 (OPZ) -- preliminary

