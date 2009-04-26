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

#include <cstdlib>


#include <iostream>
#include <sstream>

#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QResizeEvent>
#include <QMdiSubWindow>

#include "../include/heightmap-layers.hh"
#include "../include/quatsch-editor.hh"


#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/setup.hh"
#include "../../quatsch/quatsch.hh"
#include "../../quatsch/frontend/jux.hh"
#include "../../quatsch/backend/est/backend.hh"

#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include "../../redshift/include/cameras/pinhole.hh"
//#include "../../redshift/include/interaction/sdlcommandprocessor.hh"

#include "../include/qimagerendertarget.hh"



////////////////////////////////////////////////////////////////////////////////
// Composition
////////////////////////////////////////////////////////////////////////////////
unsigned int Composition::generateId () const {
      again:
        const int id = rand();
        for (size_t i=0; i<data.size(); ++i) {
                if (0 == id || id == data [i].id) {
                        goto again;
                }
        }
        return id;
}

redshift::optional<RowData> Composition::getRowById (int id) const {
        for (size_t i=0; i<data.size(); ++i) {
                if (data [i].id == id)
                        return data [i];
        }
        return false;
}

bool Composition::setRowById (int id, RowData rowData) {
        for (size_t i=0; i<data.size(); ++i) {
                if (data [i].id == id) {
                        data [i] = rowData;
                        if (0 != window) {
                                window->syncToView();
                        } else {
                        }
                        return true;
                }
        }
        return false;
}



////////////////////////////////////////////////////////////////////////////////
// HeightmapDisplaySubject
////////////////////////////////////////////////////////////////////////////////


namespace redshift {
class QuatschHeightFunction : public redshift::primitive::HeightFunction {
private:
        // quatsch
        typedef quatsch::backend::est::Backend <real_t, const real_t *> backend_t;

        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;

        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;

        struct FunctionSet {
                Compiler::ConfigurableFunctionsMap cfm;
                operator Compiler::ConfigurableFunctionsMap () {
                        return cfm;
                }
        };

        FunctionSet functionSet;
        Compiler::FunctionPtr fun;

public:
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                const real_t p [] = { u, v };
                return (*fun) (p);
        }


        QuatschHeightFunction (std::string code, std::stringstream &errors_)
        : fun (Compiler::compile (
                "x;y",
                code,
                functionSet,
                errors_))
        {
        }

};
} // namespace redshift

class HeightFunction : public redshift::primitive::HeightFunction {
        typedef redshift::real_t real_t;
        typedef redshift::fixed_point_t fixed_point_t;
        real_t operator ()
         (real_t const & u, real_t const & v) const {
                //real_t const d = sqrt (u*u + v*v);
                return (sin(u*0.7)*sin(v*0.7)) - 4;
        }
};


class Heightmap3dDisplaySubject : public SceneDisplaySubject {
public:
        std::string code;

        Heightmap3dDisplaySubject (std::string code_) : code(code_) {}

        void render (int /*width*/, int /*height*/,
                     redshift::RenderTarget::Ptr renderBuffer,
                     redshift::RenderTarget::Ptr screenBuffer,
                     redshift::shared_ptr<redshift::interaction::
                                          ProgressReporter>  reporter
        ) {
                using namespace redshift;
                using namespace redshift::camera;
                using namespace redshift::interaction;
                using namespace redshift::primitive;

                // TODO replace RenderTarget with Film?
                //    i mean, a "RenderTarget" might be flipable, but a Film not, or so

                shared_ptr<Camera> camera (new Pinhole(renderBuffer));

                shared_ptr<primitive::HeightFunction> heightFunction;
                std::stringstream errors;

                try {
                        heightFunction = shared_ptr<primitive::HeightFunction> (
                                new ::redshift::QuatschHeightFunction(
                                        code,
                                        errors
                                ));
                } catch (...) { // TODO (!!!)
                        std::cerr << "syntax error?\n"
                                  << " code: {\n"
                                  << code
                                  << "}\n"
                                  << " errors: {"
                                  << errors.str()
                                  << "}\n";
                        return;
                }

                shared_ptr<primitive::Primitive> agg (
                        /*new primitive::ClosedSphere(
                                Point(scalar_cast<fixed_point_t>(0),
                                        scalar_cast<fixed_point_t>(0),
                                        scalar_cast<fixed_point_t>(25)),
                                10.0)*/
                        new Heightmap (heightFunction, 0.25)
                );

                Scene scene (renderBuffer, camera, agg);
                UserCommandProcessor::Ptr commandProcessor (
                                new PassiveCommandProcessor());
                scene.render(reporter, commandProcessor);

                copy (renderBuffer, screenBuffer);
                screenBuffer->flip();

                //commandProcessor->waitForQuit();
                //emit fullImage (*((QImageRenderTarget*)&*screenBuffer));
        }
};




class HeightmapDisplaySubject : public SceneDisplaySubject {
public:
        std::string code;

        HeightmapDisplaySubject (std::string code_) : code(code_) {}

        void render (int /*width*/, int /*height*/,
                     redshift::RenderTarget::Ptr renderBuffer,
                     redshift::RenderTarget::Ptr screenBuffer,
                     redshift::shared_ptr<redshift::interaction::
                                          ProgressReporter>  reporter
        ) {
                using namespace redshift;
                using namespace redshift::camera;
                using namespace redshift::interaction;
                using namespace redshift::primitive;

                shared_ptr<primitive::HeightFunction> heightFunction;
                std::stringstream errors;

                try {
                        heightFunction = shared_ptr<primitive::HeightFunction> (
                                new ::redshift::QuatschHeightFunction(
                                        code,
                                        errors
                                ));
                } catch (...) { // TODO (!!!)
                        std::cerr << "syntax error?\n"
                                  << " code: {\n"
                                  << code
                                  << "}\n"
                                  << " errors: {"
                                  << errors.str()
                                  << "}\n";
                        return;
                }

                HeightmapRenderer scene (renderBuffer, heightFunction);
                UserCommandProcessor::Ptr commandProcessor (
                                        new PassiveCommandProcessor());
                scene.render(reporter, commandProcessor);
                copy (renderBuffer, screenBuffer);
                screenBuffer->flip();
        }
};




////////////////////////////////////////////////////////////////////////////////
// HeightmapLayersImpl
////////////////////////////////////////////////////////////////////////////////

HeightmapLayersImpl::HeightmapLayersImpl(QMdiArea *mdiArea_)
: mdiArea (mdiArea_), composition (new Composition())
{
        composition->window = this;

        setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);


        QHeaderView *horizontalHeader = table->horizontalHeader();
        horizontalHeader->setResizeMode(QHeaderView::ResizeToContents);
        horizontalHeader->setResizeMode(Indices::RightMostData, QHeaderView::Stretch);
        //horizontalHeader->setStretchLastSection (true);
        horizontalHeader->hide();

        QHeaderView *verticalHeader = table->verticalHeader();
        verticalHeader->setResizeMode(QHeaderView::Fixed);
        verticalHeader->setDefaultSectionSize (36);
        verticalHeader->hide();

        verticalHeader->setStyleSheet (
                 "QHeaderView::section { \n"
                 "    background-color: #666666 ; \n"
                 "    color: white; \n"
                 "    padding-left: 4px; \n"
                 "    border: 1px solid #6c6c6c; \n"
                 "}"
        );

        fillCompositionTypeComboBox (compositionType);

        display = new SceneDisplayImpl ();
        mdiArea->addSubWindow (display);
        display->show();
        display->setSubject (redshift::shared_ptr<SceneDisplaySubject>(
                new HeightmapDisplaySubject("(sin (* 10 x))")
        ));

        /*table->setStyleSheet (
        "QTableView {\n"
        "  selection-background-color: qlineargradient( \n"
        "                       x1: 0, y1: 0, x2: 0.5, y2: 0.5,\n"
        "                       stop: 0 #FF92BB, stop: 1 white);\n"
        "} \n"
        );*/
}



HeightmapLayersImpl::~HeightmapLayersImpl() {
}




void HeightmapLayersImpl::changeEvent(QEvent *e) {
        switch (e->type()) {
        case QEvent::LanguageChange:
                retranslateUi(this);
                break;
        case QEvent::Close:
                display->close();
                break;
        default:
                break;
        }
}



void HeightmapLayersImpl::fillCompositionTypeComboBox (QComboBox *box) const {
        box->setInsertPolicy (QComboBox::InsertAtBottom);
        box->addItem ("Add", QVariant(Addition));
        box->addItem ("Subtraction", QVariant(Subtraction));
        box->addItem ("Multiplication", QVariant(Multiplication));
        box->addItem ("Division", QVariant(Division));
        box->addItem ("Lerp", QVariant(Lerp));
}



void HeightmapLayersImpl::swapRows(int rowA, int rowB)
{
        syncFromView ();
        if (rowA>=0 && (size_t)rowA < composition->data.size()
            && rowB>=0 && (size_t)rowB < composition->data.size()
        ) {
                RowData tmp = composition->data [rowA];
                composition->data [rowA] = composition->data [rowB];
                composition->data [rowB] = tmp;

                table->selectRow (rowB);

        }
        syncToView ();
}



void HeightmapLayersImpl::moveRowUp(int row) {
        swapRows (row, row-1);
}



void HeightmapLayersImpl::moveRowDown(int row) {
        swapRows (row, row+1);
}



void HeightmapLayersImpl::on_moveUp_clicked() {
        moveRowUp (table->currentRow());
}



void HeightmapLayersImpl::on_moveDown_clicked() {
        moveRowDown (table->currentRow());
}



void HeightmapLayersImpl::storeRowParameters (
        int rowId, RowParametersMerger const &params
) {
        redshift::optional<RowData> data = composition->getRowById (rowId);
        if (data) {
                data->parameters = params.merge (data->parameters);
                composition->setRowById (rowId, *data);
                //emit setRowParameters (rowId, data->parameters);
        }
}



void HeightmapLayersImpl::closingDefinitionWindow (int id) {
        //std::cout << "closing def-editor for id " << id << std::endl;
        if (openDefinitionWindows.find (id) != openDefinitionWindows.end())
                openDefinitionWindows.erase (id);
        redshift::optional<RowData> row = composition->getRowById (id);
        if (row) {
                row->hardLock = false;
                composition->setRowById (id, *row);
                syncToView();
        }
}



RowData HeightmapLayersImpl::rowFromView (int row) const {
        if (0>row)
                return RowData();

        if (0) {
                std::cout << "Row " << row << '\n';
                std::cout << "  visible: "
                        << ((QCheckBox*)table->cellWidget (row,
                                        Indices::Visible))->isChecked()
                        << '\n';
                std::cout << "  locked: "
                        << ((QCheckBox*)table->cellWidget (row,
                                        Indices::Locked))->isChecked()
                        << '\n';
                std::cout << "  name: "
                        << table->item (row,
                                        Indices::Name)->text().toStdString()
                        << '\n';
        }

        const QComboBox *typeComboBox =
                        ((QComboBox*)table->cellWidget (row, Indices::Type));

        const int id = table->item (row, Indices::Id)
                            ->data(Qt::UserRole).toInt();

        // RowParameters aren't stored in the TableWidget, so those have to be
        // pulled from our actual data representation. (hack)
        // > same for hardLock
        redshift::optional<RowData> rp = composition->getRowById (id);
        RowParameters params;
        bool hardLock = false;

        if (rp) {
                params = rp->parameters;
                hardLock = rp->hardLock;
        }


        return RowData(
                id,

                ((QCheckBox*)table->cellWidget (row, Indices::Visible))->
                                                                isChecked(),

                ((QCheckBox*)table->cellWidget (row, Indices::Locked))->
                                                                isChecked(),

                hardLock,

                static_cast<RowData::Type>(typeComboBox->itemData (
                        typeComboBox->currentIndex()).toInt()),

                table->item (row, Indices::Name)->text(),

                params
        );
}



void HeightmapLayersImpl::rowToView (int rowIndex, RowData data) {

        const bool locked = data.locked || data.hardLock;

        // Id
        {
                QTableWidgetItem *item = new QTableWidgetItem(tr(""));
                item->setFlags (Qt::ItemIsSelectable);
                item->setData (Qt::UserRole, QVariant (data.id));
                if (1) {
                        item->setText ("");
                        item->setSizeHint (QSize (0,0));
                } else {
                        item->setText (item->data(Qt::UserRole).toString());
                }
                table->setItem (rowIndex, Indices::Id, item);
        }

        // Enable/Disable (i.e. visibility)
        {

                QCheckBox *box = new QCheckBox ();
                table->setCellWidget (rowIndex, Indices::Visible, box);
                makeVisibilityCheckBox (box, data.visible);
                box->setEnabled (!locked);

                connect(box,SIGNAL(stateChanged (int)),
                        this, SLOT(slot_syncFromView ()));
        }

        // Lock status
        {
                QCheckBox *box = new QCheckBox ();
                table->setCellWidget (rowIndex, Indices::Locked, box);
                makeLockingCheckBox (box, locked);
                box->setEnabled (!data.hardLock);

                connect(box,SIGNAL(stateChanged (int)),
                        this, SLOT(slot_syncFromView ()));
        }

        // Type
        {
                QComboBox *box = new QComboBox ();
                table->setCellWidget (rowIndex, Indices::Type, box);


                if (data.type < RowData::Group) {
                        box->setInsertPolicy (QComboBox::InsertAtBottom);
                        box->addItem ("1st Class", QVariant(RowData::FirstClass));
                        box->addItem ("Composition", QVariant(RowData::Composition));
                        box->addItem ("Bitmap", QVariant(RowData::Bitmap));
                        box->addItem ("Code", QVariant(RowData::Code));
                } else {
                        box->addItem ("Add", QVariant(RowData::GroupAddition));
                        box->addItem ("Subtract", QVariant(RowData::GroupSubtraction));
                        box->addItem ("Multiply", QVariant(RowData::GroupMultiplication));
                        box->addItem ("Divide", QVariant(RowData::GroupDivision));
                        box->addItem ("Lerp", QVariant(RowData::GroupLerp));
                }
                box->setEnabled (!locked);
                box->setSizeAdjustPolicy (QComboBox::AdjustToContents);

                for (int i=0; i<box->count(); ++i) {
                        if (data.type == box->itemData (i).toInt()) {
                                box->setCurrentIndex (i);
                        }
                }

                QTableWidgetItem *item = new QTableWidgetItem(tr(""));
                item->setSizeHint (box->sizeHint());
                item->setFlags (Qt::ItemIsSelectable |
                        (locked?Qt::NoItemFlags:Qt::ItemIsEnabled));
                table->setItem (rowIndex, Indices::Type, item);

                // TODO does resize properly?
        }

        // Icon size preview
        {
                QTableWidgetItem *item = new QTableWidgetItem(tr(""));
                item->setIcon(QIcon(
                               QPixmap(":/redshiftLogo/images/redshift.png")));
                item->setTextAlignment(Qt::AlignVCenter);
                item->setFlags (Qt::ItemIsSelectable |
                        (locked?Qt::NoItemFlags:Qt::ItemIsEnabled));
                table->setItem (rowIndex, Indices::Preview, item);
        }

        // Name
        {
                QTableWidgetItem *item = new QTableWidgetItem(data.name);
                item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                item->setFlags (
                        Qt::ItemIsSelectable |
                        (!locked ?
                           Qt::ItemIsEditable|Qt::ItemIsEnabled :
                           Qt::NoItemFlags
                        )
                );
                table->setItem (rowIndex, Indices::Name, item);
        }

        // This is a hack to align all cell elements correctly
        // (resizeRowsToContents() alone won't align elements; but when the
        // window gets resized, all seems to align properly, so we force a
        // resize here)
        const QSize oldSize (table->size());
        const QSize newSize (oldSize.width(), oldSize.height()+1);
        table->resizeRowsToContents();
        table->resize(newSize);
        table->resize(oldSize);
}



void HeightmapLayersImpl::colorizeGroups () {
        srand (16); // No I am not going to use a Mersenne Twister here. lulz.

        QString color [2] = {
                "#333333", "#CCCCCC"
        };
        const int colorCount = sizeof color / sizeof color[0];
        int colorId = 0;

        for (int i=0; i<table->rowCount(); ++i) {
                const RowData::Type type = composition->data[i].type;
                if (type >= RowData::Group) {
                        ++colorId;
                        colorId %= colorCount;
                }

                const QString stylesheet = "QFrame { background-color: " +
                                                color [colorId] + "}";

                QFrame *gi = new QFrame ();
                gi->setStyleSheet (stylesheet);
                table->setCellWidget (i, Indices::LeftBorder, gi);

                gi = new QFrame ();
                gi->setStyleSheet (stylesheet);
                table->setCellWidget (i, Indices::RightBorder, gi);
        }
}



void HeightmapLayersImpl::on_openDefinition_clicked() {
        const int row = table->currentRow();
        if (0>row) {
                return;
        }

        syncFromView();
        const RowData data = composition->data [row];//getRowData (row);


        // If def-window already open, focus that and return.
        if (openDefinitionWindows.find(data.id)!=openDefinitionWindows.end()) {
                openDefinitionWindows [data.id]->setFocus();
                return;
        } else if (data.locked || data.hardLock) {
                return;
        }


        switch (data.type) {
        case RowData::FirstClass:
                std::cout << "must. open. presets-dialog." << std::endl;
                break;
        case RowData::Bitmap:
                std::cout << "must. open. bitmap-dialog." << std::endl;
                break;
        case RowData::Composition: {
                std::cout << "must. open. composition dialog." << std::endl;
                HeightmapLayersImpl *hd = new HeightmapLayersImpl (mdiArea);
                QMdiSubWindow *sub = mdiArea->addSubWindow(hd);
                sub->show();
                } break;
        case RowData::Code: {
                QuatschEditorImpl *ed = new QuatschEditorImpl (
                                                data.parameters.code, data.id);
                ed->setWindowTitle (data.name);
                connect(
                        ed, SIGNAL(storeRowParameters (int,
                                                RowParametersMerger const &)),
                        this, SLOT(storeRowParameters (int,
                                                RowParametersMerger const &))
                );
                connect(ed, SIGNAL(closingDefinitionWindow (int)),
                        this, SLOT(closingDefinitionWindow (int)));
                QMdiSubWindow *sub = mdiArea->addSubWindow(ed);
                sub->show();

                openDefinitionWindows[data.id] = sub;

                composition->data [row].hardLock = true;
                syncToView();
                } break;

        case RowData::GroupAddition:
                break;
        case RowData::GroupSubtraction:
                break;
        case RowData::GroupMultiplication:
                break;
        case RowData::GroupDivision:
                break;
        case RowData::GroupLerp:
                break;
        };
}



void HeightmapLayersImpl::makeVisibilityCheckBox (
        QCheckBox *box, bool checked
) const {
        box->setChecked (checked);
        box->adjustSize();
        box->setStyleSheet (
          "QCheckBox::indicator {"
          "   width:16px;"
          "   height: 16px;"
          "}"
          "QCheckBox::indicator:unchecked {"
          "   image: url(:/visible/icons/darkglass _reworked/1239215171_stop.png)"
          "}"
          "QCheckBox::indicator:checked {"
          "   image: url(:/visible/icons/darkglass _reworked/1239214390_xeyes.png)"
          "}"
        );
}



void HeightmapLayersImpl::makeLockingCheckBox (
        QCheckBox *box, bool checked
) const {
        box->setChecked (checked);
        box->adjustSize();
        box->setStyleSheet (
               "QCheckBox::indicator {"
               "   width:16px;"
               "   height: 16px;"
               "}"
               "QCheckBox::indicator:unchecked {"
               ""
               "}"
               "QCheckBox::indicator:checked {"
               "   image: url(:/locked/icons/darkglass/1239259860_gpg.png)"
               "}"
        );
}



void HeightmapLayersImpl::on_newLayer_clicked() {
        const int index = table->currentRow() >= 0
                        ? 1+table->currentRow()
                        : table->rowCount();

        syncFromView ();

        const RowData newRow = RowData (composition->generateId(),
                                true, false, false,  // visible, lock, hardLock
                                RowData::FirstClass, tr("New Layer"));

        if (composition->data.size() == 0) {
                composition->data.push_back (newRow);
        } else {
                composition->data.insert (
                        composition->data.begin()+index, newRow);
        }

        syncToView ();
        colorizeGroups ();
}



void HeightmapLayersImpl::on_newGroup_clicked() {
        const int index = table->currentRow() >= 0
                        ? 1+table->currentRow()
                        : table->rowCount();

        syncFromView ();

        const RowData newRow = RowData (composition->generateId(),
                                true, false, false, // visible, lock, hardLock
                                RowData::GroupAddition, tr("New Group"));

        if (composition->data.size() == 0) {
                composition->data.push_back (newRow);
        } else {
                composition->data.insert (
                        composition->data.begin()+index, newRow);
        }

        syncToView ();
        colorizeGroups ();
}



void HeightmapLayersImpl::on_deleteLayer_clicked() {
        if (QMessageBox::Yes == QMessageBox::question (0, "Deleting Row.",
                "Do you want to delete the selected row?",
                QMessageBox::Yes |  QMessageBox::Cancel, QMessageBox::Cancel))
        {
                //table->removeRow (table->currentRow());
                composition->data.erase (
                        composition->data.begin()+table->currentRow()
                );
                syncToView ();
                colorizeGroups ();
        }
}



void HeightmapLayersImpl::on_updatePreview_clicked() {
        int juxFunctionId = 0;
        QString callCode;

        syncFromView();
        const QString code = generateJuxCode(juxFunctionId, callCode,
                                             composition, 0);

        display->setSubject (redshift::shared_ptr<SceneDisplaySubject>(
                new HeightmapDisplaySubject(
                                (code + "\n" + callCode).toStdString()
                )));
        display->update();
}



void HeightmapLayersImpl::on_showJuxCode_clicked() {
        int juxFunctionId = 0;
        QString callCode;

        syncFromView();
        const QString code = generateJuxCode(juxFunctionId, callCode,
                                             composition, 0);
        QMessageBox box (
                QMessageBox::Information,
                "jux code",
                code + "\n" + callCode
        );
        box.exec();
}



QString HeightmapLayersImpl::getJuxIndendationString (int indent) const {
        QString ret = "";
        for (int i=0; i<indent; ++i)
                ret += "    ";
        return ret;
}



redshift::tuple<QString, QString> HeightmapLayersImpl::generateJuxCode (
        redshift::shared_ptr<const Composition> composition,
        redshift::shared_ptr<heightmap_codegen::NamespaceMaker> namespaceMaker,
        int indent_,
        size_t &rowIndex,
        bool startsInGroup
) const {

        const QString indent = getJuxIndendationString (indent_);
        QString prolog;
        QString code;

        while (rowIndex<composition->data.size()) {
                const RowData data = composition->data [rowIndex];
                const RowParameters params = data.parameters;

                switch (data.type) {
                // primitives
                case RowData::FirstClass:
                        code += indent + "(first-class x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Bitmap:
                        code += indent + "([bitmap filename()] x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Composition:
                        code += indent + "([bitmap filename()] x y)\n";
                        ++ rowIndex;
                        break;
                case RowData::Code:
                        prolog += QString("\n")
                               + "//----------------------------------------\n"
                               + "// code for \"" + data.name + "\"\n"
                               + QString (params.code).replace (
                                        "$", namespaceMaker->getNamespace())
                               + "\n"
                               + "//----------------------------------------\n"
                               + "\n";
                        code += indent + QString ("($main x y)\n").replace (
                                "$", namespaceMaker->getNamespace()
                        );
                        namespaceMaker->generateNewNamespace();
                        ++ rowIndex;
                        break;

                // aggregates
                case RowData::GroupAddition:
                        if (startsInGroup)
                                return code;
                        code += indent + "( +\n";
                        goto rest;

                case RowData::GroupSubtraction:
                        if (startsInGroup)
                                return code;
                        code += indent + "( -\n";
                        goto rest;

                case RowData::GroupMultiplication:
                        if (startsInGroup)
                                return code;
                        code += indent + "( *\n";
                        goto rest;

                case RowData::GroupDivision:
                        if (startsInGroup)
                                return code;
                        code += indent + "( /\n";
                        goto rest;

                case RowData::GroupLerp:
                        if (startsInGroup)
                                return code;
                        code += indent + "( lerp X\n";
                        goto rest;

                rest: {
                        redshift::tuple<QString,QString> tmp =
                                generateJuxCode (composition, namespaceMaker,
                                                1+indent_, ++rowIndex, true);
                        prolog += redshift::get<0> (tmp);
                        code += redshift::get<1> (tmp);
                        code += indent + ")\n";
                        } break;
                };
        }
        return redshift::make_tuple (prolog, code);
}



QString HeightmapLayersImpl::generateJuxCode(
        int &juxFunctionId, QString &callCode,
        redshift::shared_ptr<const Composition> composition, int recDepth
) const {

        // TODO should not be created here
        redshift::shared_ptr<heightmap_codegen::NamespaceMaker>
                namespaceMaker (new heightmap_codegen::NamespaceMaker());

        int indent_ = 0;
        QString indent = getJuxIndendationString (indent_);
        QString prolog = "// prolog\n";
        QString code = indent + "// Main\n";

        // Set top level type.
        code += indent + "( ";
        switch (composition->type) {
        case Addition:
                code += indent + "+ \n";
                break;
        case Subtraction:
                code += indent + "- \n";
                break;
        case Multiplication:
                code += indent + "* \n";
                break;
        case Division:
                code += indent + "/ \n";
                break;
        case Lerp:
                code += indent + "lerp X \n";
                break;
        };

        for (size_t i=0; i<composition->data.size(); ++i) {
                redshift::tuple<QString,QString> t =
                                generateJuxCode (composition, namespaceMaker,
                                                                 1+indent_, i);
                prolog += redshift::get<0>(t);
                code += redshift::get<1>(t);
        }

        code += ")";

        return prolog + code;
}



void HeightmapLayersImpl::syncFromView () {
        if ((size_t)table->rowCount() != composition->data.size()) {
                composition->data.resize (table->rowCount());
        }

        composition->type = (CompositionType)compositionType->
                        itemData(compositionType->currentIndex()).toInt();

        for (int i=0; i<table->rowCount(); ++i) {
                composition->data [i] = rowFromView (i);
        }
}



void HeightmapLayersImpl::slot_syncFromView () {
        syncFromView ();
        syncToView(); // A bit messy, sorry. But this will e.g. in case of a
                      // user-row-lock update lock statuses in the view.
}



void HeightmapLayersImpl::syncToView () {
        if ((size_t)table->rowCount() != composition->data.size()) {
                table->setRowCount (composition->data.size());
        }
        for (size_t i=0; i<composition->data.size(); ++i) {
                rowToView (i, composition->data [i]);
        }
        colorizeGroups ();
}
