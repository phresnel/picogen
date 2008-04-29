/*
 *  picossdf.h
 *  (C) 2008 sebastian mach
 *  seb@greenhybrid.net
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

#ifndef PICOSSDF_H__INCLUDED
#define PICOSSDF_H__INCLUDED


#include <string>
#include <iostream>
#include <vector>

class PicoSSDF{
	private:

		// --- data for printing verbose error messages -----------------
		std::string filename;
		std::string errcurrentline; // only used on error
		std::string errreason;      // ditto
		unsigned int linenumber;
		enum parse_err{
			OKAY,
			FILE_NOT_FOUND,
			SYNTAX_ERROR
		};
		// --------------------------------------------------------------


		// --- definition of blocks -------------------------------------
		enum BLOCK_TYPE{
			BLOCK_LIST,
			BLOCK_TRI_BIH
		};
		static const inline std::string blockTypeAsString( BLOCK_TYPE type ){
			switch( type ){
				case BLOCK_LIST: return std::string( "list" );
				case BLOCK_TRI_BIH: return std::string( "tri-bih" );
			};
			return std::string("<unknown>");
		}

		class Block{
			public:
				const BLOCK_TYPE type;
				Block( BLOCK_TYPE type ) : type(type) {};
				Block( const Block &block ) : type(block.type){
					//const_cast<BLOCK_TYPE&>(type) = block.type;
				}
				const Block &operator = ( const Block &block ){
					const_cast<BLOCK_TYPE&>(type) = block.type;
					return *this;
				}
				virtual bool isBlockAllowed( BLOCK_TYPE ) { return true; }
		};
		class ListBlock : public Block{
			public:
				ListBlock() : Block( BLOCK_LIST ) {}
		};
		class TriBIHBlock : public Block{
			public:
				TriBIHBlock() : Block( BLOCK_TRI_BIH ) {}
				virtual bool isBlockAllowed( BLOCK_TYPE ) { return false; }
		};
		std::vector<Block*> blockStack;
		parse_err push_block( BLOCK_TYPE );
		parse_err pop_block();

		// --------------------------------------------------------------


		// ---- parsing functions ---------------------------------------
		inline static bool isidchar( char s ){
			return (isalnum(s)||s=='-');
		}
		parse_err parse();
		parse_err interpretLine( char *line );
		// --------------------------------------------------------------
	public:


		PicoSSDF( const std::string &filename );

		// ---- exceptions ----------------------------------------------
		class exception_file_not_found{
			public:
				const std::string file;
				exception_file_not_found( const std::string &file ) : file(file) {}
		};
		class exception_unknown{
			public:
				const std::string file;
				exception_unknown( const std::string &file ) : file(file) {}
		};
		class exception_syntax_error{
			public:
				const std::string file, curr, reason;
				const unsigned int line;
				exception_syntax_error(
					const std::string &file,
					const std::string &curr,
					const std::string &reason,
					unsigned int line
				): file(file), curr(curr), reason(reason), line(line)
				{}
		};
		// ---- parsing functions ---------------------------------------
};


#endif // PICOSSDF_H__INCLUDED
