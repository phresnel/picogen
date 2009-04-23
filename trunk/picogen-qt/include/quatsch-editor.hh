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

#ifndef QUATSCH_EDITOR_HH_INCLUDED_20090419
#define QUATSCH_EDITOR_HH_INCLUDED_20090419

#include "ui_quatsch-editor.h"

#include "heightmap-layers.hh"

#include <QSyntaxHighlighter>
#include <QTextEdit>

#include <QHash>
#include <QTextCharFormat>


// lifted from http://doc.trolltech.com/4.2/richtext-syntaxhighlighter.html
class QuatschHighlighter : public QSyntaxHighlighter {
        Q_OBJECT

public:
        QuatschHighlighter(QTextDocument *parent = 0);

protected:
        void highlightBlock(const QString &text);

private:
        struct HighlightingRule
        {
                QRegExp pattern;
                QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        QRegExp commentStartExpression;
        QRegExp commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat functionFormat;
        
        QTextCharFormat parametersFormat;
        QTextCharFormat numberFormat;
};

 

class QuatschEditorImpl : public QWidget, private Ui::QuatschEditor
{
        Q_OBJECT

public:
        QuatschEditorImpl(
            QString code,
            int rowId);
        virtual ~QuatschEditorImpl() {}
        
private:
        void closeEvent(QCloseEvent *event);

private slots:

        void on_edit_textChanged();
        
        //void setRowParameters(int rowId, RowParameters const &params);        

signals:

        void storeRowParameters(int rowId, RowParametersMerger const &merge);
        void closingDefinitionWindow (int id);

private:
        QuatschHighlighter *highlighter;
        int id;
};


#endif // QUATSCH_EDITOR_HH_INCLUDED_20090419
