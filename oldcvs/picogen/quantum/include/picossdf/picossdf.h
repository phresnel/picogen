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

class SSDFBackend {

    public:
        // Init/Finish
        virtual int initialize() = 0;
        virtual int finish() = 0;


        // Blocks.
        virtual int beginGlobalBlock () = 0;
        virtual int endGlobalBlock () = 0;
        virtual int beginListBlock () = 0;
        virtual int endListBlock () = 0;
        virtual int beginTriBIHBlock () = 0;
        virtual int endTriBIHBlock () = 0;

        // State setters.
        virtual int setBRDFToLambertian (::picogen::real reflectance) = 0;
        virtual int setBRDFToSpecular (::picogen::real reflectance) = 0;

        virtual int preethamEnable (bool enable) = 0;
        virtual int preethamSetTurbidity (::picogen::real T) = 0;
        virtual int preethamSetSunSolidAngleFactor (::picogen::real f) = 0;
        virtual int preethamSetColorFilter (const ::picogen::graphics::color::Color &color) = 0;
        virtual int preethamSetSunColor (const ::picogen::graphics::color::Color &color) = 0;
        virtual int preethamSetSunDirection (const ::picogen::geometrics::Vector3d &direction) = 0;
        //virtual int preethamEnableFogHack (::picogen::real exponent, ::picogen::real maxDist) = 0;
        virtual int preethamSetFogExp (::picogen::real exp) = 0;
        virtual int preethamSetFogMaxDist (::picogen::real dist) = 0;

        virtual int cameraSetPositionYawPitchRoll (
            const ::picogen::geometrics::Vector3d &position,
            ::picogen::real yaw,
            ::picogen::real pitch,
            ::picogen::real roll
        ) = 0;

        // Object adders.
        virtual int addSphereTerminal (
            ::picogen::real radius,
            const ::picogen::geometrics::Vector3d &center,
            const ::picogen::graphics::color::Color &color
        ) = 0;

        virtual int addHeightfield (
            const ::picogen::misc::functional::Function_R2_R1 &fun,
            unsigned int resolution,
            const ::picogen::geometrics::Vector3d &center,
            const ::picogen::geometrics::Vector3d &size
        ) = 0;

        virtual int addImplicitHeightfield (
            ::picogen::misc::functional::Function_R2_R1 **fun,
            const ::picogen::geometrics::Vector3d &center,
            const ::picogen::geometrics::Vector3d &size
        ) = 0;
};



class PicoSSDF {
    private:

        // --- syntax stuff ---------------------------------------------
        std::string filename;
        std::string errcurrentline; // only used on error
        std::string errreason;      // ditto
        unsigned int linenumber;
        enum parse_err {
            OKAY,
            FILE_NOT_FOUND,
            SYNTAX_ERROR
        };
        unsigned int globalBlockCount;
        // --------------------------------------------------------------



        // --- communication --------------------------------------------
        SSDFBackend *backend;
        // --------------------------------------------------------------



        // --- definition of state types --------------------------------
        typedef enum {
            STATE_MATERIAL
        } STATE_TYPE;
        static const inline std::string stateTypeAsString (STATE_TYPE type) {
            switch (type) {
                case STATE_MATERIAL:
                    return std::string ("material");
            };
            return std::string ("<unknown>");
        }
        parse_err read_state (STATE_TYPE, const char *&line);
        // --------------------------------------------------------------



        // --- definition of terminal types -----------------------------
        typedef enum {
            TERMINAL_SPHERE
            ,TERMINAL_PREETHAM
            ,TERMINAL_SET_CAMERA_YAW_PITCH_ROLL
            ,TERMINAL_HEIGHTSLANG_HEIGHTFIELD
            ,TERMINAL_IMPLICIT_HEIGHTSLANG_HEIGHTFIELD
        } TERMINAL_TYPE;
        static const inline std::string terminalTypeAsString (TERMINAL_TYPE type) {
            switch (type) {
                case TERMINAL_SPHERE:
                    return std::string ("sphere");
                case TERMINAL_PREETHAM:
                    return std::string ("sunsky");
                case TERMINAL_SET_CAMERA_YAW_PITCH_ROLL:
                    return std::string ("camera-yaw-pitch-roll");
                case TERMINAL_HEIGHTSLANG_HEIGHTFIELD:
                    return std::string ("hs-heightfield");
                case TERMINAL_IMPLICIT_HEIGHTSLANG_HEIGHTFIELD:
                    return std::string ("hs-implicit-heightfield");
            };
            return std::string ("<unknown>");
        }

        class Terminal {
            public:
                const TERMINAL_TYPE type;
                explicit Terminal (TERMINAL_TYPE type) : type (type) {};
        };
        class SphereTerminal : public Terminal {
            public:
                explicit SphereTerminal () : Terminal (TERMINAL_SPHERE) {};
        };
        parse_err read_terminal (TERMINAL_TYPE, const char *&line);
        // --------------------------------------------------------------



        // --- definition of blocks -------------------------------------
        typedef enum {
            BLOCK_LIST,
            BLOCK_TRI_BIH,
            BLOCK_GLOBAL
        } BLOCK_TYPE;
        static const inline std::string blockTypeAsString (BLOCK_TYPE type) {
            switch (type) {
                case BLOCK_GLOBAL:
                    return std::string ("global");
                case BLOCK_LIST:
                    return std::string ("list");
                case BLOCK_TRI_BIH:
                    return std::string ("tri-bih");
            };
            return std::string ("<unknown>");
        }

        class Block {
            public:
                const BLOCK_TYPE type;
                explicit Block (BLOCK_TYPE type) : type (type) {};
                explicit Block (const Block &block) : type (block.type) {
                    //const_cast<BLOCK_TYPE&>(type) = block.type;
                }
                const Block &operator = (const Block &block) {
                    const_cast<BLOCK_TYPE&> (type) = block.type;
                    return *this;
                }
                virtual bool isBlockAllowed (BLOCK_TYPE) {
                    return false;
                }
                virtual bool isTerminalAllowed (TERMINAL_TYPE) {
                    return false;
                }
        };

        class GlobalBlock : public Block {
            public:
                explicit GlobalBlock() : Block (BLOCK_GLOBAL) {}
                virtual bool isBlockAllowed (BLOCK_TYPE) {
                    return true;
                }
                virtual bool isTerminalAllowed (TERMINAL_TYPE type) {
                    switch (type) {
                        case TERMINAL_PREETHAM:
                            return true;
                        case TERMINAL_SET_CAMERA_YAW_PITCH_ROLL:
                            return true;
                    };
                    return false;
                }
        };

        class ListBlock : public Block {
            public:
                explicit ListBlock() : Block (BLOCK_LIST) {}
                virtual bool isBlockAllowed (BLOCK_TYPE) {
                    return true;
                }
                virtual bool isTerminalAllowed (TERMINAL_TYPE type) {
                    switch (type) {
                        case TERMINAL_SPHERE:
                            return true;
                        case TERMINAL_HEIGHTSLANG_HEIGHTFIELD:
                            return true;
                        case TERMINAL_IMPLICIT_HEIGHTSLANG_HEIGHTFIELD:
                            return true;
                    };
                    return false;
                }
        };

        class TriBIHBlock : public Block {
            public:
                explicit TriBIHBlock() : Block (BLOCK_TRI_BIH) {}
                virtual bool isBlockAllowed (BLOCK_TYPE) {
                    return false;
                }
                virtual bool isTerminalAllowed (TERMINAL_TYPE type) {
                    switch (type) {
                        case TERMINAL_HEIGHTSLANG_HEIGHTFIELD:
                            return true;
                    };
                    return false; //type==TERMINAL_TRIANGLE;
                }
        };

        std::vector<Block*> blockStack;
        parse_err push_block (BLOCK_TYPE);
        parse_err pop_block();
        // --------------------------------------------------------------




        // ---- parsing functions ---------------------------------------
        inline static bool isidchar (char s) {
            return (isalnum (s) ||s=='-');
        }
        parse_err parse();
        parse_err interpretLine (const char *line);
        parse_err interpretCode (const std::string &code, std::string::const_iterator &it);
        // --------------------------------------------------------------
    public:


        PicoSSDF (const std::string &filename, SSDFBackend *backend);

        // ---- exceptions ----------------------------------------------
        class exception_file_not_found {
            public:
                const std::string file;
                exception_file_not_found (const std::string &file) : file (file) {}
        };
        class exception_unknown {
            public:
                const std::string file;
                exception_unknown (const std::string &file) : file (file) {}
        };
        class exception_syntax_error {
            public:
                const std::string file, curr, reason;
                const unsigned int line;
                exception_syntax_error (
                    const std::string &file,
                    const std::string &curr,
                    const std::string &reason,
                    unsigned int line
                ) : file (file), curr (curr), reason (reason), line (line) {}
        };
        // ---- parsing functions ---------------------------------------
};


#endif // PICOSSDF_H__INCLUDED
