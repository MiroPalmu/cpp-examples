bin_PROGRAMS = readfifo

# Gnulib stuff:
#
# From gnulib manual:
#
#   > Finally, you have to add compiler and linker flags in the appropriate source directories,
#   > so that you can make use of the gnulib library. Since some modules (‘getopt’, for example)
#   > may copy files into the build directory, top_builddir/lib is needed as well as top_srcdir/lib.
GNULIB_INCS = -I$(top_srcdir)/gnulib -I$(top_builddir)/gnulib

readfifo_SOURCES = fifo_utils.cpp readfifo.cpp
readfifo_CPPFLAGS = $(GNULIB_INCS) -I$(top_srcdir)/include
readfifo_CXXFLAGS = -std=c++23
readfifo_LDADD = $(top_builddir)/gnulib/libgnu.a
