/*
 *  picossdf.cc
 *  (C) 2008 Sebastian Mach
 *  seb@greenhybrid.net
 *
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

#define PICOSSDF_CC_STANDALONE 0

#include "picogen.h"
#include "picossdf/picossdf.h"
#include <ctype.h>
#include <stdio.h>

using namespace std;



PicoSSDF::PicoSSDF( const string &filename ) : filename(filename) {
    switch ( parse() ) {
    case OKAY:
        break;
    case FILE_NOT_FOUND:
        throw exception_file_not_found(filename);
    case SYNTAX_ERROR:
        throw exception_syntax_error(
            filename, errcurrentline, errreason, linenumber
        );
    default:
        throw exception_unknown(filename);
    }
}


PicoSSDF::parse_err PicoSSDF::push_block( BLOCK_TYPE type ) {
    if ( blockStack.size()>0 ) {
        // check if the new block is allowed within the current block
        if ( !blockStack.back()->isBlockAllowed( type ) ) {
            errreason =
                string("Block of type '")+blockTypeAsString(type)+
                string("' forbidden within block of type '")+
                blockTypeAsString(blockStack.back()->type)+string("'")
                ;
            return SYNTAX_ERROR;
        }
    }
    switch ( type ) {
    case BLOCK_LIST: {
        blockStack.push_back( new ListBlock() );
    }
    break;
    case BLOCK_TRI_BIH: {
        blockStack.push_back( new TriBIHBlock() );
    }
    break;
    };
    // pretty print log
    for ( unsigned int u=0; u<blockStack.size(); ++u ) printf( "  " );
    printf( "new block\n" );

    return OKAY;
}

PicoSSDF::parse_err PicoSSDF::pop_block() {
    if ( blockStack.size() <= 0 ) {
        errreason = string( "invalid closing bracket '}'" );
        return SYNTAX_ERROR;
    }
    Block *block = blockStack.back();
    delete block;

    // pretty print log
    for ( unsigned int u=0; u<blockStack.size(); ++u ) printf( "  " );
    printf( "compiling block\n" );

    blockStack.pop_back();
    return OKAY;
}

PicoSSDF::parse_err PicoSSDF::interpretLine( char *line ) {
    //return SYNTAX_ERROR;
    // skip whitespace
    while ( isblank(*line) ) ++line;

    // is this a comment?
    if ( line[0] == '/' && line[1] == '/' )
        return OKAY;

    // scan name of block
    char block_name[1024] = { '\0' };
    char *tmp = block_name;
    while ( isidchar( *line ) && tmp != &block_name[sizeof(block_name)] ) {
        *tmp = *line;
        ++tmp;
        ++line;
    }
    *tmp = '\0';
    // get length of block-name and see if it is anonymous
    const unsigned int block_name_len = strlen(block_name);
    const bool anonymous = !(block_name_len > 0);

    //if( !anonymous ) printf( "\n<%s>\n", block_name );
    while ( isblank(*line) ) ++line;

    if ( '{' == *line ) {
        // get block typer out of block_name
        BLOCK_TYPE type;
        if ( anonymous ) {
            errreason = string( "anonymous blocks are not allowed" );
            return SYNTAX_ERROR;
        } else if ( !strcmp( "list", block_name ) ) {
            type = BLOCK_LIST;
        } else if ( !strcmp( "tri-bih", block_name ) ) {
            type = BLOCK_TRI_BIH;
        } else {
            errreason = string( "unknown block type" );
            return SYNTAX_ERROR;
        }
        // push
        parse_err err = push_block( type );
        if ( OKAY != err ) {
            return err;
        }
        line++;
    } else if ( '}' == *line ) {
        if ( !anonymous ) {
            errreason = string("'}' is only allowed standalone" );
            return SYNTAX_ERROR;
        }
        // pop
        parse_err err = pop_block();
        if ( OKAY != err )
            return err;
        line++;
    } else {
        // a) this intended tp be a damn fast text file format
        // b) i allow blank lines
        // c) but i do not allow other format freeness
        // ca) after a block declarator there must be [blank]*'{'
        //     i do not allow putting the '{' into the next line
        //     this is an error, and the lone '{' is then a SCOPE_BLOCK!
        if ( !anonymous ) {
            errreason = string("block name must be followed by optional spaces and '{'" );
            return SYNTAX_ERROR;
        }
    }
    while ( isblank(*line) ) ++line;
    if ( *line != '\0' ) {
        errreason = string( "trailing, non whitespace character(s) forbidden ('" )+string(line)+string("')");
        return SYNTAX_ERROR;
    }
    return OKAY;
}

PicoSSDF::parse_err PicoSSDF::parse() {
    // open file, check if okay
    FILE *fin = fopen( filename.c_str(), "r" );
    if ( 0 == fin ) {
        cerr << "file \"" << filename << "\" not found. quitting parser." << endl;
        return FILE_NOT_FOUND;
    }

    parse_err retcode = OKAY;
    errcurrentline = string("");
    errreason = string("");
    // parse
    linenumber = 0;
    while ( 1 ) {
        // get next line
        char curr[1024];
        if ( NULL == fgets( curr, sizeof(curr), fin ) ) {
            break;
        }
        ++linenumber;
        // remove trailing '\n'
        {
            unsigned int length = strlen( curr );
            if ( curr[length-1] == '\n' )
                curr[length-1] = '\0';
        }

        // interpret current line
        parse_err err = interpretLine( curr );
        if ( err != OKAY ) {
            retcode = err;
            errcurrentline = string(curr);
            break;
        }
        if ( feof( fin ) ) {
            break;
        }
    }

    // clean up
    fclose( fin );
    return retcode;
}


#if PICOSSDF_CC_STANDALONE // to be used when testing standalone
void print_usage() {
    cout << "usage:" << endl;
    cout << "  {this program} filename\n" << endl;
}
int main( int argc, char *argv[] ) {
    --argc;
    ++argv;
    if ( argc == 0 ) {
        print_usage();
        return 0;
    }
    try {
        PicoSSDF( string( argv[0] ) );
    } catch ( PicoSSDF::exception_file_not_found e ) {
    } catch ( PicoSSDF::exception_unknown e ) {
    } catch ( PicoSSDF::exception_syntax_error e ) {
        cerr << e.file << ":" << e.line << ":" << e.reason << "\nhere: \"" << e.curr << "\"" << endl;
    }
}
#endif // PICOSSDF_CC_STANDALONE

