
## Disclaimer

This is a copy of what was once available at
http://grail.eecs.csuohio.edu/~somos/picky.html. It was written by Michael
Somos. I claim no authorship whatever.

I have updated the README a bit to address some differences in the sense that
this is not a tarball but a GitHub repo as well as formatting.

I do not expect to make any changes at all once everything looks well as it's
not my program. I'm only making it available for those who want it. If I do make
any changes it might be to make a make install rule but that's probably all if I
do even that.

## README for picky version 2.6 - 14 Dec 2014

Picky is a simple C program for validating ASCII text files.  It is designed to
run under UNIX-like systems but all you really need is a C compiler to build an
executable program.  Michael did not test the program under Windows but it may
work.  The following files are provided in this repo:

      Makefile   A simple Makefile for compiling the tool.
      picky.1    The man page for picky.
      picky.c    The C source code for picky.
      picky.txt  The man page in plain text for picky.
      README.md  This file.

## Compilation

For most environments except DOS/Windows the simple `Makefile` provided will
create the executable.

Once you've cloned this repo you can just run `make` to compile:


	$ git clone https://github.com/xexyl/picky
	$ make


You should then copy the executable to a directory
somewhere in your `$PATH`. You may also copy the man page `picky.1 to an
appropriate location.

## Installing

If you don't have root privileges and/or you want to install it for your own
use, just copy the executable file to some directory you have write privileges
to and ensure that that directory is in your $PATH. A typical normal user build,
test and install sequence of commands (assuming both ~/bin and ~/man/man1 exist,
~/bin is in your $PATH, ~/man in $MANPATH) is:


	$ cp picky   ~/bin/
	$ cp picky.1 ~/man/man1/


 
## Example


    $ ./picky -s Makefile -t picky.*


## Uninstalling

To uninstall, just delete `picky` and `picky.1` from where you
copied them and then delete the `picky` directory.
