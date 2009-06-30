//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef SCENE_HH_INCLUDED_20090606
#define SCENE_HH_INCLUDED_20090606

#include <string>

#include "../../redshift/include/redshift.hh"

//class HeightmapLayersImpl;

namespace scene {

        struct SyncToView {
                virtual void syncToView () = 0;
        };

        /*struct syncFromView {
                virtual void operator () () = 0;
        };*/

namespace heightmap {
        enum CompositionType {
                Addition,
                Subtraction,
                Multiplication,
                Division,
                Lerp
        };


        struct RowParameters {
                // Let's not make this an enum and simply save
                // everything. The user can thus change the type
                // of the layer, try something out (maybe accidentally!),
                // and then just return back to the old settings.
                std::string code;

                RowParameters ()
                : code ("// Example code:\n"
                        "(defun $main (x y) (if (< x y) x y))")
                {}

                RowParameters (RowParameters const &rhs)
                : code (rhs.code)
                {}

                RowParameters &operator = (RowParameters const &rhs) {
                        code = rhs.code;
                        return *this;
                }
        };


        class RowParametersMerger {
                RowParameters parameters;
                bool codeChanged;
        public:
                RowParametersMerger () : codeChanged (false) {}

                std::string code () const { return parameters.code; }
                void setCode (std::string const &s) {
                        codeChanged = true;
                        parameters.code = s;
                }

                RowParameters merge (RowParameters const &in) const {
                        RowParameters ret = in;
                        if (codeChanged)
                                ret.code = parameters.code;
                        return ret;
                }
        };



        struct RowData {
                enum Type {
                        // primitives
                        FirstClass,
                        Bitmap,
                        Composition,
                        Code,

                        // aggregates
                        Group,
                        GroupAddition = Group,
                        GroupSubtraction,
                        GroupMultiplication,
                        GroupDivision,
                        GroupLerp
                };

                int id;
                bool visible;
                bool locked;
                bool hardLock;
                Type type;
                std::string name;
                //CompositionType groupType;

                RowParameters parameters;

                RowData (int id_, bool visible_, bool locked_, bool hardLock_,
                        Type type_, std::string name_,
                        //CompositionType groupType_=Addition,
                        RowParameters params=RowParameters())
                : id (id_), visible(visible_)
                , locked(locked_), hardLock (hardLock_)
                , type(type_)
                , name(name_)
                //, groupType(groupType_)
                , parameters (params)
                {}

                RowData (RowData const &r)
                : id (r.id), visible (r.visible), locked (r.locked)
                , hardLock(r.hardLock)
                , type (r.type), name (r.name)//, groupType (r.groupType)
                , parameters (r.parameters)
                {
                }

                RowData &operator = (RowData const &r) {
                        id = r.id;
                        visible = r.visible;
                        locked = r.locked;
                        hardLock = r.hardLock;
                        type = r.type;
                        name = r.name;
                        //groupType = r.groupType;
                        parameters = r.parameters;
                        return *this;
                }

                void swap (RowData &r) {
                        RowData tmp (r);
                        r = *this;
                        *this = tmp;
                }

                RowData () {}
        };


        struct Composition {
                Composition (SyncToView* syncToView_);
                Composition ();

                CompositionType type;
                std::vector <RowData> data;

                unsigned int generateId () const ;
                ::redshift::optional<RowData> getRowById (int id) const ;
                bool setRowById (int id, RowData rowData) ;
                void setSyncObject (SyncToView* syncToView_);
                void resetSyncObject ();

        private:
                SyncToView *syncToView;

                // forbid
                Composition (Composition const &);
                Composition &operator = (Composition const &);
        };
}


struct Scene {
        redshift::shared_ptr<heightmap::Composition> terrainComposition;
};


} // namepsace scene
#endif // SCENE_HH_INCLUDED_20090606
