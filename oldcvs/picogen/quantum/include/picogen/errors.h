/***************************************************************************
 *            errors.h
 *
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 *  date: 2008-12-20
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


#ifndef PICOGEN_ERRORS_H__INCLUDED
#define PICOGEN_ERRORS_H__INCLUDED

namespace picogen {
    namespace error_codes {
        typedef enum code_t {
            rangescale__ = 10*1024,
            
            // generic {{
            generic_okay                  = 0,
            generic_min                   = 1*rangescale__,
            generic_max                   = 2*rangescale__,
            // }} generic
            
            // ssdf {{
            ssdf_okay                   = generic_okay,
            ssdf_error_min              = 2*rangescale__,
            ssdf_syntax_error           = 0 + ssdf_error_min,
            ssdf_unknown_error          = 1 + ssdf_error_min,
            ssdf_file_not_found         = 2 + ssdf_error_min,
            ssdf_sdl_not_initialised    = 3 + ssdf_error_min,
            ssdf_set_video_mode_failed  = 4 + ssdf_error_min,
            ssdf_missing_source_file_name = 5 + ssdf_error_min,
            ssdf_missing_argument_to_option_width = 6 + ssdf_error_min,
            ssdf_missing_argument_to_option_height = 7 + ssdf_error_min,
            ssdf_missing_argument_to_option_output = 8 + ssdf_error_min,
            ssdf_missing_argument_to_option_aa = 9 + ssdf_error_min,
            ssdf_missing_argument_to_option_n = 10 + ssdf_error_min,
            ssdf_missing_argument_to_option_l = 11 + ssdf_error_min,
            ssdf_missing_argument_to_option_surface_integrator = 12 + ssdf_error_min,
            ssdf_unknown_option         = 13 + ssdf_error_min,
            ssdf_error_max              = 3*rangescale__,
            // }} ssdf
            
            // mkskymap {{
            mkskymap_okay               = generic_okay,
            mkskymap_error_min          = 3*rangescale__,
            mkskymap_error_max          = 4*rangescale__,
            // }} mkskymap
            
            // mkheightmap {{
            mkheightmap_okay             = generic_okay,
            mkheightmap_error_min        = 4*rangescale__,
            mkheightmap_unknown_error    = 0 + mkheightmap_error_min,
            mkheightmap_syntax_error     = 1 + mkheightmap_error_min,
            mkheightmap_unknown_language = 2 + mkheightmap_error_min,
            mkheightmap_unknown_commandline_option = 3 + mkheightmap_error_min,
            mkheightmap_bad_option_syntax     = 4 + mkheightmap_error_min,
            mkheightmap_sdl_not_initialised   = 5 + ssdf_error_min,
            mkheightmap_set_video_mode_failed = 6 + ssdf_error_min,
            mkheightmap_no_options_given = 7 + ssdf_error_min,
            mkheightmap_error_max       = 5*rangescale__,
            // }} mkheightmap
        };
    };
};

#endif // PICOGEN_ERRORS_H__INCLUDED
