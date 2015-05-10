C CHIP-8 Emulator (c8)
======================

c8 is a CHIP-8 emulator written in C11 and SDL2. CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for said computers.

Dependencies
------------

This project depends on [SDL2](https://www.libsdl.org/) and has no other dependencies other than being compiled with --std=c11.

Usage
-----

`$ c8 game` where game is the path to a CHIP-8 binary.

Motivation
----------

I want to begin writing emulators for fun and become more experienced with both C11 and SDL2. Writing a CHIP-8 emulator is one of the best starting points for this endeavor.

References, Credits, and Thanks
-------------------------------

-	http://en.wikipedia.org/wiki/CHIP-8
-	http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
-	http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
-	https://github.com/mbcrawfo/GenericMakefile (Magic Makefile)
-	http://www.zophar.net/pdroms/chip8/chip-8-games-pack.html (Collection of game binaries)
-	Various other open source projects
-	The awesome guys and gals in ##c and #sdl on Freenode

License
-------

While I wouldn't recommend learning from this project, it is licensed under [WTHPL](LICENSE) so feel free to do whatever you wish with it.
