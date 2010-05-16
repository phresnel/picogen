//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef COMPILER_HH_20100516
#define COMPILER_HH_20100516

// for a fine list of predefined compiler macros: http://predef.sourceforge.net/

// compiler info

#ifdef __GNUC__
//>>> GCC <<<
# ifdef __GNUC_PATCHLEVEL__ // introduced with GCC 3.0
//        the xxx version is so that we can first expand __GNUC__ to a number, and then stringify it
#  define PICOGEN_GCC_xxxVERSION( major, minor, patch ) #major "." #minor "." #patch
#  define PICOGEN_GCC_VERSION( major, minor, patch ) PICOGEN_GCC_xxxVERSION( major, minor, patch )
#  define PICOGEN_COMPILER_NAME "GCC " PICOGEN_GCC_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
# else
#  define PICOGEN_GCC_xxxVERSION( major, minor ) #major "." #minor ".X"
#  define PICOGEN_GCC_VERSION( major, minor ) PICOGEN_GCC_xxxVERSION( major, minor )
#  define PICOGEN_COMPILER_NAME "GCC " PICOGEN_GCC_VERSION(__GNUC__, __GNUC_MINOR__)
# endif
#else
//>>> UNKNOWN <<<
#endif


// target arch
#if defined(_POSIX_VERSION) or defined(unix) or defined(__unix__) or defined(linux)
//#define UI_NCURSES
# define UI_WX
# define POSIX
# define PICOGEN_ARCH_NAME "POSIX"
#elif defined(_WIN32) or defined(WIN32)
# define UI_WX
# ifndef WIN32
#  define WIN32
# endif
# define PICOGEN_ARCH_NAME "WIN32"
#else
# error "meh, unsupported target architecture ..."
#endif

// user interface info
/*
#if 0
#if defined(UI_WX)
# define UI_NAME "wxWigets"
#elif defined(UI_NCURSES)
# define UI_NAME "ncurses"
#else
# error "unknown user interface or user interface not set."
#endif
#endif
*/

//#define UI_NAME "SDL"
#define PICOGEN_COMPILATION_INFO  "compiler:" PICOGEN_COMPILER_NAME "/arch:" PICOGEN_ARCH_NAME "/t:" __DATE__ "(" __TIME__ ")"

namespace redshift {
        const char *getCompilationInfo () ;
}

#endif // COMPILER_HH_20100516
