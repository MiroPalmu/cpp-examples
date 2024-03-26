#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([reafifo_proj], [0.0.1], [email@miropalmu.cc])

# From Autoconf manual 4.4 Configure Input:
# https://www.gnu.org/software/autoconf/manual/autoconf-2.70/html_node/Input.html

# As sanity check give unique file from project that configure will check.
AC_CONFIG_SRCDIR([src/readfifo.cpp])
AC_CONFIG_MACRO_DIRS([gnulib/m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC
gl_EARLY
AC_PROG_CXX

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    gnulib/Makefile
])

# Checks for header files.
gl_INIT
AC_CHECK_HEADERS([fcntl.h unistd.h])

AC_OUTPUT
