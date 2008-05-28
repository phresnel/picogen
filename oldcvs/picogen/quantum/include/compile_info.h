/*
 * compile_info.h
 * Copyright (C) Sebastian Mach 2008 <seb@greenhybrid>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef compile_h__included
#define compile_h__included

// for a fine list of predefined compiler macros: http://predef.sourceforge.net/

// compiler info

#ifdef __GNUC__
//>>> GCC <<<
# ifdef __GNUC_PATCHLEVEL__ // introduced with GCC 3.0
//        the xxx version is so that we can first expand __GNUC__ to a number, and then stringify it
#  define GCC_xxxVERSION( major, minor, patch ) #major "." #minor "." #patch
#  define GCC_VERSION( major, minor, patch ) GCC_xxxVERSION( major, minor, patch )
#  define COMPILER_NAME "GCC " GCC_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# else
#  define GCC_xxxVERSION( major, minor ) #major "." #minor ".X"
#  define GCC_VERSION( major, minor ) GCC_VERSION( major, minor )
#  define COMPILER_NAME "GCC " GCC_VERSION(__GNUC__, __GNUC_MINOR__)
# endif
#else
//>>> UNKNOWN <<<
#endif


// target arch
#if defined(_POSIX_VERSION) or defined(unix) or defined(__unix__) or defined(linux)
//#define UI_NCURSES
#define UI_WX
#define POSIX
#define ARCH_NAME "POSIX"
#else
#error "meh, unsupported target architecture ..."
#endif

// user interface info
#if 0
#if defined(UI_WX)
# define UI_NAME "wxWigets"
#elif defined(UI_NCURSES)
# define UI_NAME "ncurses"
#else
# error "unknown user interface or user interface not set."
#endif
#endif


#define UI_NAME "SDL"
#define COMPILATION_INFO  "compiler:" COMPILER_NAME "/arch:" ARCH_NAME "/ui:" UI_NAME "/t:" __DATE__ "(" __TIME__ ")"


#endif
