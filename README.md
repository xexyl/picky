This is a copy of what was once available at
http://grail.eecs.csuohio.edu/~somos/picky.html. It was written by Michael
Somos. I claim no authorship whatever.


README            for picky version 2.6       14 Dec 2014

Picky is a simple "C" program for validating ASCII text files.
It is designed to run under UNIX-like systems but all you
really need is a C compiler to build an executable program.
I have not tested the program under Windows but it may work.
The following files are provided in this "tarball" package:

  Makefile   A simple "makefile" for make.
  picky.1    The "man page" for picky.
  picky.c    The "C" source code for picky.
  picky.txt  The "man page" in plain text for picky.
  picky.exe  The DOS/Windows command line executable.
  README     This UNIX text file.

INSTALLATION

For most environments except DOS/Windows the simple "makefile"
provided will create the executable. You should then copy
the executable to a directory somewhere in your $PATH. You
may also copy the "man page" to an appropriate location. If
you don't have root privileges and/or you want to install
it for your own use, just copy the executable file to some
directory you have write privileges to and ensure that that
directory is in your $PATH. A typical normal user build,
test and install sequence of commands (assuming both ~/bin and
~/man/man1 exist, ~/bin is in your $PATH, ~/man in $MANPATH) is:

  $ tar zxf picky-2.6.tar.gz
  $ cd picky-2.6
  $ cc -o picky picky.c
  $ ./picky -s Makefile -t picky.*
  $ cp picky   ~/bin/
  $ cp picky.1 ~/man/man1/

UNINSTALLATION

To uninstall, just delete "picky" and "picky.1" from where you
copied them, delete the "picky-2.6" directory, and delete the
"picky-2.6.tar.gz" tarball file.
