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

#ifndef HEIGHTMAP_LAYERS_HH_INCLUDED_20090408
#define HEIGHTMAP_LAYERS_HH_INCLUDED_20090408

#include <iostream>
#include <map>

#include <QCheckBox>
#include <QMdiArea>

#include "../../redshift/include/setup.hh"

#include "ui_heightmap-layers.h"
#include "heightmap-display.hh"

#include "heightmap/codegen.hh"



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
        QString code;

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

        QString code () const { return parameters.code; }
        void setCode (QString const &s) {
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
        QString name;
        //CompositionType groupType;

        RowParameters parameters;

        RowData (int id_, bool visible_, bool locked_, bool hardLock_,
                Type type_, QString name_,
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


class HeightmapLayersImpl;
struct Composition {
        Composition () {}
        /*Composition (Composition const &c) {

        }*/

        CompositionType type;
        std::vector <RowData> data;

        unsigned int generateId () const ;
        redshift::optional<RowData> getRowById (int id) const ;
        bool setRowById (int id, RowData rowData) ;

private:
        friend class HeightmapLayersImpl;
        HeightmapLayersImpl *window;
};




class HeightmapLayersImpl : public QWidget, private Ui::HeightmapLayers {
        Q_OBJECT

public:
        HeightmapLayersImpl(QMdiArea *mdiArea);
        virtual ~HeightmapLayersImpl();

private slots:

        void on_newLayer_clicked();
        void on_newGroup_clicked();
        void on_deleteLayer_clicked();

        void on_moveUp_clicked();
        void on_moveDown_clicked();
        void on_openDefinition_clicked();

        void on_showJuxCode_clicked();
        void on_updatePreview_clicked ();


        void storeRowParameters(int rowId, RowParametersMerger const &merge);
        void closingDefinitionWindow(int rowId);

        void slot_syncFromView ();


signals:
        //void setRowParameters(int rowId, RowParameters const &params);

private:

        QMdiArea *mdiArea;

        struct Indices {
                const static int Visible = 1;
                const static int Locked = 2;
                const static int Type = 3;
                const static int Preview = 4;
                const static int Name = 5;
                const static int Id = 6;
                const static int RightMostData = Id;

                const static int LeftBorder = 0;
                const static int RightBorder = 7;
        };
public:
        friend struct Composition;

protected:
        virtual void changeEvent(QEvent *e);

private:

        SceneDisplayImpl *display;

        std::map<int, QWidget*> openDefinitionWindows;

        redshift::shared_ptr<Composition> composition;
        void syncFromView ();
        void syncToView ();


        RowData rowFromView (int row) const ;
        void rowToView (int row, RowData data) ;
        void swapRows(int rowA, int rowB);
        void moveRowUp(int row);
        void moveRowDown(int row);

        void colorizeGroups ();


        QString generateJuxCode(int &juxFunctionId, QString &callCode,
                           redshift::shared_ptr<const Composition> composition,
                           int recDepth=0) const;
        redshift::tuple<QString, QString> generateJuxCode (
                 redshift::shared_ptr<const Composition> composition,
                 redshift::shared_ptr<heightmap_codegen::NamespaceMaker> nm,
                 int indent,
                 size_t &rowIndex,
                 bool startsInGroup = false
        ) const;
        QString getJuxIndendationString (int indent) const ;

        void makeVisibilityCheckBox (QCheckBox *box, bool checked) const ;
        void makeLockingCheckBox (QCheckBox *box, bool checked) const ;

        void fillCompositionTypeComboBox (QComboBox *box) const ;
};


#endif // HEIGHTMAP_LAYERS_HH_INCLUDED_20090408
