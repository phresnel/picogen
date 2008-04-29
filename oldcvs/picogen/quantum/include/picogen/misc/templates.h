/***************************************************************************
 *            root/templates/templates.h
 *
 *  Mon Oct  8 18:49:00 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
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


#ifndef _ROOT__TEMPLATES__TEMPLATES_H
#define _ROOT__TEMPLATES__TEMPLATES_H

namespace templates{


	template <class T> class vector{
		private:
			T *m;
			size_t length;
		public:
			template <class U> inline T &operator [] ( U u ){
				XRT_CALL_STACK_PUSH( "template <class T> inline real& vector::operator [] ( U u )" );
				#ifdef XRT_DEBUG
					if( u < 0 || u >= static_cast<U>(length) ) throw exceptions::index_out_of_bounds(__FILE__,__LINE__,0,length,u);
				#endif
				XRT_CALL_STACK_POP();
				return m[u]; // do not make any type conversion, so compiler-warnings are produced
			}
			template <class U> const inline T operator [] ( U u ) const{
				XRT_CALL_STACK_PUSH( "template <class T> const inline real vector::operator [] ( T u ) const" );
				#ifdef XRT_DEBUG
					if( u < 0 || u >= static_cast<U>(length) ) throw exceptions::index_out_of_bounds(__FILE__,__LINE__,0,length,u);
				#endif
				XRT_CALL_STACK_POP();
				return m[u]; // do not make any type conversion, so compiler-warnings are produced
			}
			vector() : m(NULL), length(0) {
				XRT_CALL_STACK_PUSH( "vector::vector()" );
				XRT_CALL_STACK_POP();
			}
			vector( size_t size ) : m(NULL), length(0) {
				XRT_CALL_STACK_PUSH( "vector::vector( size_t size )" );
				reset( size );
				XRT_CALL_STACK_POP();
			}
			virtual ~vector() {
				XRT_CALL_STACK_PUSH( "virtual vector::~vector()" );
				if( m != NULL )
					delete [] m;
				m = NULL;
				length = 0;
				XRT_CALL_STACK_POP();
			}
			void reset( size_t new_length ) {
				XRT_CALL_STACK_PUSH( "void vector::reset( size_t new_length )" );
				if( m != NULL )
					delete [] m;
				m = NULL;
				length = 0;
				try{
					m = new T[new_length];
				}catch(...){
					throw exceptions::out_of_memory( __FILE__, __LINE__, new_length );
				}

				if( m != NULL ){
					length = new_length;
				}else{
					length = 0;
					throw exceptions::out_of_memory( __FILE__, __LINE__, new_length );
				}
				XRT_CALL_STACK_POP();
			}
			inline size_t size() const {
				XRT_CALL_STACK_PUSH( "inline size_t vector::size() const" );
				XRT_CALL_STACK_POP();
				return length;
			}
			// TODO write comment about the hackish character of the following two functions
			bool saveBinaryToFile( std::string fileName ) const{
				FILE *f = fopen( fileName.c_str(), "wb" );
				if( NULL != f ){
					fwrite( m, sizeof(T), length, f );
					fclose( f );
					return true;
				}
				return false;
			}
			bool loadBinaryFromFile( std::string fileName ){
				FILE *f = fopen( fileName.c_str(), "rb" );
				if( NULL != f ){
					fread( m, sizeof(T), length, f );
					fclose( f );
					return true;
				}
				return false;
			}
	};
	template <class T> class surface{
		private:
			templates::vector<T> m_surface;
			unsigned int m_width, m_height;
		public:
			surface() : m_surface(), m_width(0), m_height(0) {
				XRT_CALL_STACK_PUSH( "surface::surface()" );
				XRT_CALL_STACK_POP();
			}
			surface( unsigned int w, unsigned int h ) {
				XRT_CALL_STACK_PUSH( "surface::surface( unsigned int w, unsigned int h )" );
				reset( w, h );
				XRT_CALL_STACK_POP();
			}
			virtual ~surface() {
				XRT_CALL_STACK_PUSH( "virtual ~surface::surface()" );
				m_width = 0;
				m_height = 0;
				XRT_CALL_STACK_POP();
			}
			bool saveBinaryToFile( std::string fileName ) const {
				return m_surface.saveBinaryToFile( fileName );
			}
			bool loadBinaryFromFile( std::string fileName ){
				return m_surface.loadBinaryFromFile( fileName );
			}
			void reset( unsigned int w, unsigned int h ) {
				XRT_CALL_STACK_PUSH( "void surface::reset( unsigned int w, unsigned int h )" );
				m_width = 0;
				m_height = 0;
				m_surface.reset( static_cast<size_t>(w*h) );
				m_width = w;
				m_height = h;
				XRT_CALL_STACK_POP();
			}
			unsigned int width()  const { return m_width;  }
			unsigned int height() const { return m_height; }
			T operator () ( unsigned int u, unsigned int v ) const {
				XRT_CALL_STACK_PUSH( "T surface::operator () ( unsigned int u, unsigned int v ) const" );
				#ifdef XRT_DEBUG
					if( u >= m_width || v >= m_height )
						throw exceptions::coords_out_of_bounds( __FILE__, __LINE__, 0, 0, m_width, m_height, u, v );
				#endif
				XRT_CALL_STACK_POP();
				return m_surface[u+v*m_width];
			}
			T operator () ( int u, int v ) const {
				XRT_CALL_STACK_PUSH( "T surface::operator () ( int u, int v ) const" );
				#ifdef XRT_DEBUG
					if( u >= m_width || v >= m_height )
						throw exceptions::coords_out_of_bounds( __FILE__, __LINE__, 0, 0, m_width, m_height, u, v );
				#endif
				XRT_CALL_STACK_POP();
				return m_surface[u+v*m_width];
			}
			T operator () ( float fu, float fv ) const {
				XRT_CALL_STACK_PUSH( "T surface::operator () ( float u, float v ) const" );
				unsigned int u = (unsigned int)( fu * (double)(m_width) );
				unsigned int v = (unsigned int)( fv * (double)(m_height) );
				#ifdef XRT_DEBUG
					if( u >= m_width || v >= m_height )
						throw exceptions::coords_out_of_bounds( __FILE__, __LINE__, 0, 0, m_width, m_height, u, v );
				#endif
				XRT_CALL_STACK_POP();
				return m_surface[u+v*m_width];
			}
			T operator () ( double fu, double fv ) const {
				XRT_CALL_STACK_PUSH( "T surface::operator () ( unsigned int u, unsigned int v ) const" );
				unsigned int u = (unsigned int)( fu * (double)(m_width) );
				unsigned int v = (unsigned int)( fv * (double)(m_height) );
				#ifdef XRT_DEBUG
					if( u >= m_width || v >= m_height )
						throw exceptions::coords_out_of_bounds( __FILE__, __LINE__, 0, 0, m_width, m_height, u, v );
				#endif
				XRT_CALL_STACK_POP();
				return m_surface[u+v*m_width];
			}
			T &operator () ( unsigned int u, unsigned int v ) {
				XRT_CALL_STACK_PUSH( "T &surface::operator ()( unsigned int u, unsigned int v )" );
				#ifdef XRT_DEBUG
					if( u >= m_width || v >= m_height )
						throw exceptions::coords_out_of_bounds( __FILE__, __LINE__, 0, 0, m_width, m_height, u, v );
				#endif
				XRT_CALL_STACK_POP();
				return m_surface[u+v*m_width];
			}
	};
};

#endif /* _ROOT/TEMPLATES/TEMPLATES_H */
