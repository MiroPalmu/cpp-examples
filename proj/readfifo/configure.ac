#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([reafifo_proj], [0.0.1], [email@miropalmu.cc])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])


# Checks for programs.
AC_PROG_CXX

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

AC_OUTPUT
