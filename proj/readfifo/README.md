This project implements simple fifo reader,
but it has some other purposes as well:

    - simple example project for GNU build system
    - example project on how to use gnulib with GNU build system

# gnulib

These steps follow gnulib manual section 3.2 Initial import.

In proj/reafifo directory following invocation of gnulib-tool was used
to initialize the gnulib folder:

```
/path/to/gnuli-tool --source-base=./gnulib --m4-base=./gnulib/m4 --import full-read
```

It then prompted with following advice:

```
You may need to add #include directives for the following .h files.
  #include "full-read.h"

Don't forget to
  - add "./gnulib/Makefile" to AC_CONFIG_FILES in ./configure.ac,
  - mention "gnulib" in SUBDIRS in ./Makefile.am,
  - mention "-I ./gnulib/m4" in ACLOCAL_AMFLAGS in Makefile.am
    or add an AC_CONFIG_MACRO_DIRS([./gnulib/m4]) invocation in ./configure.ac,
  - invoke gl_EARLY in ./configure.ac, right after AC_PROG_CC,
  - invoke gl_INIT in ./configure.ac.
```

Also one has to realize that gnulib is C library so it needs the C compiler.
This means that following is also added to configure.ac:

```
AC_PROG_CC
```

Gnulib manual advices following:

  > Finally, you have to add compiler and linker flags in the appropriate source directories,
  > so that you can make use of the gnulib library. Since some modules (‘getopt’, for example)
  > may copy files into the build directory, top_builddir/lib is needed as well as top_srcdir/lib. 

accompanied with example:

```
AM_CPPFLAGS = -I$(top_builddir)/lib -I$(top_srcdir)/lib
...
LDADD = lib/libgnu.a
```

These flags go to src/Makefile.am so LDADD has to be modified to:

```
LDADD = $(top_builddir)/gnulib/libgnu.a
```

for automake to generate right dependencies.
