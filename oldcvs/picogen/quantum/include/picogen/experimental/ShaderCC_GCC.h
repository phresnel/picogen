/***************************************************************************
 *            ShaderCC_GCC.h
 *
 *  Fri May  2 10:54:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or (at your
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

#ifndef SHADERCC_GG_H__INCLUDED
#define SHADERCC_GG_H__INCLUDED

#include <string>

#ifndef POSIX
// Stub for non-posix.
class ShaderCC_GCC : public ::picogen::graphics::material::abstract::IShader {
    public:
        typedef ::picogen::graphics::material::abstract::IShader IShader;
        typedef ::picogen::real real;
        typedef ::picogen::geometrics::Vector3d Vector3d;
        typedef ::picogen::graphics::color::Color Color;

        bool isValid() const {return false;}
        virtual void shade (
            Color &color,
            const Vector3d &normal,
            const Vector3d &position
        ) const {}

        ShaderCC_GCC (::std::string fileName, ::std::string shaderName = std::string ("")) {}
        virtual ~ShaderCC_GCC() {}
};

#else // #ifndef POSIX

#include <dlfcn.h>

// this shader is highly experimantal
// it loads/compiles a quasi-c++ file, containing the shader code.
// WARNING: this class strongly depends on the existence of g++ and the bash!!!
class ShaderCC_GCC : public ::picogen::graphics::material::abstract::IShader {
        typedef ::picogen::graphics::material::abstract::IShader IShader;
        typedef ::picogen::graphics::color::Color Color;
        typedef ::picogen::geometrics::Vector3d Vector3d;
        ::std::string fileName;
        void *fileHandle;
        bool  valid;
        IShader *shader;
    public:
        bool isValid() const {
            return valid;
        }
        virtual void shade (
            Color &color,
            const Vector3d &normal,
            const Vector3d &position
        ) const {
            if (shader != 0)
                shader->shade (color, normal, position);
        }

        ShaderCC_GCC (::std::string fileName, ::std::string shaderName = std::string ("")) : fileName (fileName), fileHandle (0), valid (false), shader (0) {
            using ::std::string;
            string cmdLine;
            string prologue (
                "#include <stdio.h>\n"
                "#include <picogen/picogen.h>\n"
                "using picogen::graphics::color::Color;\n"
                "using picogen::geometrics::Vector3d;\n"
                "#define publish(name) extern \\\"C\\\" picogen::graphics::material::abstract::IShader* init_##name(){ return new name(); }\n"
                "#define default(name) extern \\\"C\\\" picogen::graphics::material::abstract::IShader* init(){ return new name(); }\n"
                "#define shade()       virtual void Shade( Color &color, const Vector3d &normal, const Vector3d &position ) const\n"
                "#define shader(name)  struct name : public picogen::graphics::material::abstract::IShader\n"
            );
            string epilogue ("");

            // cat together an actual c++ file (basically 'epilogue' + 'shader-file' + 'epilogue') with a system call
            string tmpName = fileName + ".tmp";
            /// \todo create (if not exists) cache directory
            cmdLine =
                "echo \"" + prologue + "\" > "  + tmpName
                + "&& cat "    + fileName + " >> "   + tmpName
                + "&& echo \"" + epilogue + "\" >> " + tmpName + "\n"
                ;
            if (0 != system (cmdLine.c_str())) {
                printf ("failed to execute: {{\n%s\n}}\n", cmdLine.c_str());
            } else {
                /// \todo  only compile library if not exists, or if older than source file
                /// then \todo include compiler/architecture information in library name
                using std::string;

                // compile with a system call
                string libName (fileName);
                libName = libName + ".gplusplus.x";
                cmdLine =
                    "g++ -shared -s -x c++ -o "+libName + " " + tmpName
                    + "&& rm " + tmpName + "\n"
                    ;
                if (0 != system (cmdLine.c_str())) {
                    printf ("failed to execute: {{\n%s\n}}\n", cmdLine.c_str());
                } else {
                    // load the library
                    fileHandle = dlopen (libName.c_str(), RTLD_LOCAL | RTLD_LAZY);
                    if (fileHandle != 0) {
                        // either load the default shader (when shaderName=""), or the specified one
                        string initName ("init");
                        if (shaderName != string ("")) {
                            initName = initName + "_" + shaderName;
                        }
                        IShader* (*f) () = (IShader* (*) ()) dlsym (fileHandle, initName.c_str());
                        if (f != 0) {
                            shader = f();
                            valid = true;
                        } else {
                            printf ("could not dlsym function.\n");
                        }
                    } else {
                        printf ("could not dlopen file.\n");
                    }
                }
            }
        }

        virtual ~ShaderCC_GCC() {
            if (shader != 0) delete shader;
            if (fileHandle!=0) dlclose (fileHandle);
        }

};
#endif // #ifndef POSIX -- selse

#endif // #ifndef SHADERCC_GG_H__INCLUDED
