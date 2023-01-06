/* picky.c -- validate ASCII text file and report deviations */

/* Jan 31, 1995 by Suzanne Skinner based on an idea by Michael Somos */
/* Feb 01, 1995 Modified by Michael Somos with il_char_table */
/* Feb 09, 1995 Modified by Michael Somos with tab adjust */
/* Feb 24, 1995 Modified by Michael Somos with line counter */
/* Jun 10, 1995 Modified by Michael Somos with directory check and rc */
/* Jun 17, 1995 Modified by Michael Somos with verbose bug fixed */
/* Dec  4, 1995 Modified by Michael Somos improved option processing */
/* Dec 13, 1995 Modified by Michael Somos removed pedantic warnings */
/* Jan 28, 1996 Modified by Michael Somos added -c and -8 options */
/* Feb  1, 1996 Modified by Michael Somos added -t option */
/* Feb  8, 1996 Modified by Michael Somos added <sys/types.h> for Ultrix */
/* Feb 12, 1996 Modified by Michael Somos fixed backspace column bug */
/* Feb 18, 1996 Modified by Michael Somos fixed cil logic */
/* Feb 21, 1996 Modified by Michael Somos added -0 option */
/* Feb 25, 1996 Modified by Michael Somos fixed -0 option char count */
/* Mar  3, 1996 Modified by Michael Somos added -s option */
/* Jul 27, 1996 Modified by Michael Somos added biggest line reporting */
/* Sep 14, 1996 Modified by Michael Somos changed c to char */
/* Oct  7, 1996 Modified by Michael Somos changed c back to int */
/* Aug 24, 1997 Modified by Michael Somos added -e option */
/* Sep 30, 1997 Modified by Michael Somos fixed error reporting bug */
/* Oct  9, 1997 Modified by Michael Somos added octal output */
/* Jan 24, 1998 Modified by Michael Somos added -c option */
/* Nov 20, 2002 Modified by Michael Somos fixed -r option */
/* Sep 15, 2007 Modified by Michael Somos fixed infile dir problem */
/* Aug 07, 2012 Modified by Michael Somos check for file not end in LF */
/* Jul 07, 2013 Modified by Michael Somos added -- and -o options */
/* Jul 28, 2013 Modified by Michael Somos fixed -8 option 128..255 */
/* Aug 11, 2013 Modified by Michael Somos changed print messages. added -V */
/* Aug 12, 2013 Modified by Michael Somos added -a<n> option */
/* Aug 13, 2013 Modified by Michael Somos changed -q, added -d -A options. */
/* Aug 13, 2013 Modified by Michael Somos improved logic and error handling */
/* Aug 14, 2013 Modified by Michael Somos fixed -c bug */
/* Aug 16, 2013 Modified by Michael Somos fixed stdin processing */
/* Aug 19, 2013 Modified by Michael Somos minor output message formatting */
/* Sep 23, 2013 Modified by Michael Somos fixed bug in col reporting */
/* Sep 29, 2013 Modified by Michael Somos fixed 2nd bug in col reporting */
/* Nov 01, 2013 Modified by Michael Somos plural and fixed long line report */
/* Dec 12, 2014 Modified by Michael Somos fixed -t tabspace columns bug */
/* Dec 18, 2014 Modified by Michael Somos added -u option, light edits */

/* This program is public domain. You can do what you want with it. */
/* Author: Suzanne Skinner formerly of Cleveland State University */
/* Author: Michael Somos <ms639@georgetown.edu> at Georgetown University */

/* Purpose: validate ASCII text files by reporting all illegal characters
   and lines exceeding a maximum length up to maximum number of errors.
   Check if last line is terminated by line feed. It assumes default tabs
   at every eight columns and eighty column maximum length lines. */
/* Note: this program doesn't understand Unicode text files. However, using
   option -8 allows some UTF-8 encoded Unicode text files to be valid even
   if they are not really checked for valid Unicode, and even if line width
   and column calculations will be inaccurate in such cases. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  /* for ULTRIX */
#include <sys/stat.h>
#include <errno.h>

int scan_file(FILE *infile, char *filename); /* forward declaration */

#define VERSION "2.6"
#define myTRUE  1
#define myFALSE 0
#define chNUL '\0'
#define chCR  '\r'
#define chLF  '\n'
#define chTAB '\t'
#define chBS  '\b'
#define chBL  ' '
#define plural(x) (((x)>1)?"s":"")

/* legal control chars are form feed, line feed and tab */
char legal_chars[] = " \f\n\t!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~0123456789"
                     "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char il_char_table[256];     /* illegal character boolean table */
int verbose = myFALSE;       /* verbose response */
int vverbose = myFALSE;      /* very verbose response */
int quiet = myFALSE;         /* no standard output mode */
int trailing = myFALSE;      /* trailing space check */
int aborterr = myFALSE;      /* abort file if errors maxed */
int paborterr = myFALSE;     /* abort program if file errors maxed */
int crlfend = myFALSE;       /* CRLF line ending */
int final_LF = myFALSE;      /* missing final LF allowed */
int errsmax = 10;            /* maximum errors logged */
int abortmax = 10;           /* maximum errors to abort file */
int pabortmax = 10;          /* maximum errors to abort program */
int tabspace = 8;            /* tab spacing width */
long linemax = 80;           /* maximum line width in bytes (0 = no max) */

int main(int argc, char *argv[]) {

  FILE *f;
  struct stat filestat;
  int i, arg;
  int error_count = 0;       /* count files with errors from scan_file */
  int not_opts = myFALSE;    /* encounterd not option possibly a file? */
  int end_opts = myFALSE;    /* are we out of options processing? */
  int in_files = myFALSE;    /* have we seen any input files yet? */
  for (i=0; i<256; i++)               /* initially all characters illegal */
     il_char_table[i] = myTRUE;
  for (i=0; legal_chars[i]; i++)      /* set all the legal chars from list */
     il_char_table[ 255 & (int)legal_chars[i] ] = myFALSE;

  for (arg = 1; arg < argc; arg++) {
/* begin arg processing */
    char *cp, opt;

    cp = argv [ arg ];
       if ( * cp == '-' && ( ! end_opts ) ) {
/* begin option processing */
          opt = *( ++ cp );
          switch ( opt ) {
/* begin opt switches */
             default :
                printf("picky: invalid option: %c\n", opt );
                error_count = 1;
                /* fallthru */

             case 'h' : /* -h */
                printf("Usage: picky [[options][inputfile]]...\n");
                printf("Options:\n");
                printf("  -     Input from stdin: default if no inputfile.\n");
                printf("  --    End of option arguments.\n");
                printf("  -8    High 8-bit characters (128..255) okay.\n");
                printf("  -a<n> Maximum errors to abort file is n (10).\n");
                printf("  -a    No maximum errors to abort file.\n");
                printf("  -A<n> Maximum error files to exit is n (10).\n");
                printf("  -A    No maximum error files to exit.\n");
                printf("  -b    Backspace anywhere okay.\n");
                printf("  -c    CRLF line endings only.\n");
                printf("  -d    DOS/Windows mode. Implies both -c and -f.\n");
                printf("  -e<n> Maximum errors logged is n (10).\n");
                printf("  -e    No maximum errors.\n");
                printf("  -f    Missing final LF okay.\n");
                printf("  -h    Print brief help message and exit.\n");
                printf("  -o<n> ASCII code n in (0..255) anywhere okay.\n");
                printf("  -o    ASCII nulls anywhere okay.\n");
                printf("  -q    Quiet mode. No standard output produced.\n");
                printf("  -r    Carriage Return anywhere okay.\n");
                printf("  -s    Trailing white space not okay.\n");
                printf("  -t<n> Tab spacing width is n (8).\n");
                printf("  -t    Tab anywhere not okay.\n");
                printf("  -u    UNIX mode. LF line ending always (default).\n");
                printf("  -v    Verbose mode (repeat for more verbose).\n");
                printf("  -V    Print version and exit.\n");
                printf("  -w<n> Maximum line width is n (80).\n");
                printf("  -w    No maximum line width.\n");
                return error_count>0 ? EXIT_FAILURE : EXIT_SUCCESS;

             case 'V' : /* -V */
                printf("picky %s\n", VERSION);
                return EXIT_SUCCESS;

             case 'a' : /* -a */
                aborterr = myTRUE;
                if ( (i=atoi(cp+1)) >= 0) abortmax = i;
                break;

             case 'A' : /* -A */
                paborterr = myTRUE;
                if ( (i=atoi(cp+1)) >= 0) pabortmax = i;
                break;

             case 'c' : /* -c */
                crlfend = myTRUE;
                break;

             case 'f' : /* -f */
                final_LF  = myTRUE;
                break;

             case 'd' : /* -d */
                crlfend = myTRUE;
                final_LF = myTRUE;
                break;

             case 'u' : /* -u */
                crlfend = myFALSE;
                final_LF = myFALSE;
                break;

             case 'q' : /* -q */
                quiet = myTRUE;
                break;

             case 's' : /* -s */
                trailing = myTRUE;
                break;

             case 'v' : /* -v */
                if ( verbose ) { vverbose = myTRUE; } else
                   verbose = myTRUE;
                break;

             case 'o' : /* -o */
                if ( (i=atoi(cp+1)) >= 0 && i<256 )
                   il_char_table[ i ] = myFALSE;
                break;

             case '8' : /* -8 */
                for (i=128; i<256; i++) il_char_table[i] = myFALSE;
                break;

             case 'b' : /* -b */
                il_char_table[ (int) chBS ] = myFALSE;
                break;

             case 'r' : /* -r */
                il_char_table[ (int) chCR ] = myFALSE;
                break;

             case 't' : /* -t */
                if ( (i=atoi(cp+1)) >= 0) {
                   il_char_table[ (int) chTAB ] = ( i == 0);
                   tabspace = i>0 ? i : 1; }
                break;

             case 'w' : /* -w */
                if ( (i=atoi(cp+1)) >= 0) linemax = i;
                break;

             case 'e' : /* -e */
                if ( (i=atoi(cp+1)) >= 0) errsmax = i;
                break;

             case '-' : /* -- */
                end_opts = myTRUE;
                break;

             case chNUL : /* - */
                in_files = myTRUE;
                if ( scan_file ( stdin , "stdin" ) ) error_count ++;
                break;

/* end opt switches */
              }
/* end option processing */
          } else {
/* begin if not an option */
      not_opts = myTRUE;
      if ( ( f = fopen ( cp, "rb" ) ) == NULL ) {
        fprintf(stderr, "picky: %s Can not be opened for reading\n", cp);
        continue;
      }
      if ( -1 == stat(cp , & filestat) ) {
        fprintf(stderr, "picky: %s Can not be statted\n", cp);
        continue;
      }
      if ( ! S_ISDIR( filestat . st_mode ) ) {
        in_files = myTRUE;         /* seen input file */
        if ( scan_file(f, cp) ) error_count ++;
      }
      fclose(f);                  /* ignore any errors in closing it */
/* end not an option */
    }
    if ( paborterr && pabortmax > 0 && error_count >= pabortmax ) {
       if ( ! quiet )
          printf("picky: at least %d file%s in error before run aborted\n",
             error_count, plural(error_count));
       return EXIT_FAILURE;
    }
/* end for (arg ...) */
  }
  if ( ( ! in_files ) && ( ! not_opts ) )  /* no input files and no not opts */
     if ( scan_file ( stdin , "stdin" ) ) error_count ++;

  return error_count>0 ? EXIT_FAILURE : EXIT_SUCCESS;
} /* end main() */

int scan_file(FILE *infile, char *filename) {

/* these integers are small */
  int warn_line = myFALSE;
  int aborted = myFALSE;
  int last_CR = myFALSE;
  int read_error = myFALSE;    /* an I/O error while reading infile? */
  int lastLFerr = myFALSE;    /* was the last character a line feed? */
  int error_status = myFALSE;    /* any errors from scanning infile? */
  int c = 0;
  int oldc = 0;
/* these integers may be big */
  long cil = 0;                  /* character in line index 1 origin */
  long cilsave = 0;          /* save character in line for reporting */
  long bigline = 0;
  long il_chars = 0;
  long il_lines = 0;
  long cif = 0;                  /* character in file index 1 origin */
  long lif = 1;               /* current line in file index 1 origin */
  long errs = 0;

  while ( ( c = getc(infile) ) != EOF) {
/* begin while not eof */
    cif ++;                       /* character c is index cif in file */

    /* act on certain ASCII control characters */
    switch ( c ) {

    case chCR :                          /* carriage return character */

       if ( cil > bigline ) bigline = cil;
       cilsave = cil; cil = 0;
       break;

    case chTAB :                          /* horizontal tab character */

      if ( tabspace > 0 ) {
         cil =  (1+( cil / tabspace ))* tabspace; }
      break;

    case chBS :                                /* backspace character */

       if ( cil > 0 ) cil --;           /* backup by one byte on line */
       break;

    case chLF :                     /* line feed (new-line) character */

       { lif ++;                              /* bump up line counter */

         warn_line = myFALSE;
         if ( cilsave > 0 ) cil = cilsave;    /* restore cil from CR */
         if ( cil > bigline ) bigline = cil;

         if ( crlfend && ( ! last_CR ) ) {
           if ( aborterr && abortmax > 0 && errs >= abortmax )
              { aborted = myTRUE; break; }           /* break while not eof */
           if ( verbose && ( errsmax == 0 || errs < errsmax ) )
              printf("picky: %s line %ld does not end in CRLF\n",
                 filename,  lif - 1);
           errs ++;
         } /* CRLF error */

         if ( trailing && (oldc == chBL || oldc == chTAB ) ) {
           if ( aborterr && abortmax > 0 && errs >= abortmax )
              { aborted = myTRUE; break; }           /* break while not eof */
           if ( verbose && ( errsmax == 0 || errs < errsmax ) )
              printf("picky: %s line %ld ends at col %ld: %3d %04o 0x%02X\n",
                  filename, lif - 1, cil, oldc, oldc, oldc);
           il_chars ++; errs ++;
         } /* if trailing error */

         cil = cilsave = 0;                  /* assume a CR also init cil */
       } /* end LF char */
       break;

    default :                                      /* all other cases */

        cil ++;                                 /* bump column number */

    } /* end switch */

    if ( ! (last_CR = (c == chCR ) ) ) oldc = c; /* save as old character */

    if ( ( ! (last_CR && crlfend) ) && il_char_table [ c ] ) {
      if ( aborterr && abortmax > 0 && errs >= abortmax )
         { aborted = myTRUE; break; }           /* break while not eof */
      if ( verbose && ( errsmax == 0 || errs < errsmax ) ) {
         if ( cilsave > 0 ) cil = cilsave;    /* restore cil from CR */
         printf("picky: %s line %ld bad char at col %ld: %3d %04o 0x%02X\n",
             filename, lif, cil, c, c, c);
      }
      il_chars ++; errs ++;
    } /* end illegal char error */

    if ( ( linemax > 0 ) && ( ! warn_line ) && ( cil > linemax ) ) {
       warn_line = myTRUE;
       if ( aborterr && abortmax > 0 && errs >= abortmax )
          { aborted = myTRUE; break; }
       if ( verbose && ( errsmax == 0 || errs < errsmax ) )
          printf("picky: %s line %ld char at col %ld: line length past %ld\n",
             filename, lif, cil, linemax);
       il_lines ++; errs ++;
    } /* end line max error */
  } /* end while not eof */

  if ( ferror(infile) ) {
     if ( aborterr && abortmax > 0 && errs >= abortmax ) aborted = myTRUE;
     clearerr(infile); read_error = myTRUE; errs ++;
  } else {
    if ( ( ! final_LF ) && cif > 0 && oldc != chLF && ( ! aborted ) ) {
       if ( aborterr && abortmax > 0 && errs >= abortmax ) aborted = myTRUE;
       lastLFerr = myTRUE; errs ++;
    }
  } /* end if ferror() */
/* report the results if requested */
  if ( errs == 0 ) {
     if ( ( ! quiet ) && vverbose ) printf("picky: %s is ok\n", filename);
  } else {
/* begin errors report processing */
    error_status = myTRUE;
    if ( ! quiet ) {
      if ( verbose )
        if ( errs > errsmax && errsmax > 0 )
          printf("picky: %s more errors were not reported\n", filename);
      if ( read_error )
         printf("picky: %s error encountered when reading\n", filename);
      if ( lastLFerr )
         printf("picky: %s last line %ld does not end in LF\n", filename, lif);
      if ( il_chars > 0 ) {
         if ( aborted ) {
            printf("picky: %s at least %ld illegal char%s before abort\n",
            filename, il_chars, plural(il_chars));
         } else {
            printf("picky: %s has %ld illegal char%s\n",
            filename, il_chars, plural(il_chars));
         }
      }
      if ( il_lines > 0 ) {
         if ( aborted ) {
            printf("picky: %s at least %ld too long line%s before abort\n",
              filename, il_lines, plural(il_lines));
            printf("picky: %s maximum line width at least %ld before abort\n",
              filename, bigline);
         } else {
            printf("picky: %s has %ld too long line%s\n",
              filename, il_lines, plural(il_lines));
            printf("picky: %s maximum line width was %ld\n",
              filename, bigline);
         }
      }
      if ( aborted ) {
         printf("picky: %s at least %ld error%s before aborted\n",
           filename, errs, plural(errs));
      } else {
         printf("picky: %s has %ld error%s\n",
           filename, errs, plural(errs));
      }
    } /* end if !quiet */
  } /* end if errs */
  return error_status;
} /* end scan_file() */

/* end of file picky.c */
