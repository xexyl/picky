# picky


## Disclaimer

This is a copy of what was once available at
http://grail.eecs.csuohio.edu/~somos/picky.html. It was written by Michael
Somos. I claim no authorship whatever.

I have updated the README a bit to address some differences in the sense that
this is not a tarball but a GitHub repo as well as formatting.

I do not expect to make many changes at all once everything looks well as it's
not my program. I'm only making it available for those who want it.


## README for picky version 2.6 - 14 Dec 2014

Picky is a simple C program for validating ASCII text files.  It is designed to
run under UNIX-like systems but all you really need is a C compiler to build an
executable program.  Michael did not test the program under Windows but it may
work.  The following files are provided in this repo:

      Makefile   A simple Makefile for compiling the tool.
      picky.1    The man page for picky.
      picky.c    The C source code for picky.
      README.md  This file.


## To compile:

For most environments except DOS/Windows the simple `Makefile` provided will
create the executable.

To clone and compile you can do e.g.:


```sh
    git clone https://github.com/lcn2/picky
    cd picky
    make
```


## To install"

If you have root privileges you can just run:

```sh
    make install
```

Otherwise, if you do not have root privileges or you just want to install it for your own use,
just copy the executable file to some directory you have write privileges
to and ensure that that directory is in your `$PATH`. For the man page copy the
`picky.1` file to a directory that is in your `$MANPATH`. For example:


```sh
    # if you do not have root privileges, just install under your home:
    #
    mkdir -p -v ~/bin
    cp -v -f picky  ~/bin
    mkdir -p -v ~/man
    mkdir -p -v ~/man/man1
    cp -v -f picky.1 ~/man/man1
```


## To test:

To test the picky files themselves:


```sh
    make test
```


# Examples:

The `-s` means "_Trailing white space not okay_":

```sh
    ./picky -s Makefile
```

The `-t` means "_Tab anywhere not okay_":

```sh
    ./picky -t picky.1 picky.c
```

The `-b` means "_Backspace anywhere okay_":

```sh
    make test.txt
    ./picky -b test.txt
```

For more information, see the `picky(1)` man page for more details:

```sh
    man ./picky.1   # if before installing
```

or:

```sh
    man picky       # if after installing
```


## Uninstalling

To uninstall:

```sh
    make uninstall
```
