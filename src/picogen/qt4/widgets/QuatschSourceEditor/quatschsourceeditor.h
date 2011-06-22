//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#include "ui_quatschsourceeditor.h"

//#include "heightmap-layers.hh"

#include <QSyntaxHighlighter>
#include <QPlainTextEdit>

#include <QHash>
#include <QTextCharFormat>

#include "../Picohelp/picohelpbrowser.h"



// Qt Quarterly 31
struct ParenthesisInfo
{
    char character;
    int position;
};

class TextBlockData : public QTextBlockUserData
{
public:
    TextBlockData();

    QVector<ParenthesisInfo *> parentheses();
    void insert(ParenthesisInfo *info);

private:
    QVector<ParenthesisInfo *> m_parentheses;
};




// lifted from http://doc.trolltech.com/4.2/richtext-syntaxhighlighter.html
class QuatschHighlighter : public QSyntaxHighlighter {
        Q_OBJECT

public:
        QuatschHighlighter(QTextDocument *parent = 0);

        void setFunctionNames (QStringList list);

protected:
        void highlightBlock(const QString &text);

private:
        struct HighlightingRule
        {
                QRegExp pattern;
                QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;
        QVector<HighlightingRule> callHighlightingRules;

        QRegExp commentStartExpression;
        QRegExp commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat functionFormat;

        QTextCharFormat parametersFormat;
        QTextCharFormat numberFormat;
};



class QuatschSourceEditor : public QWidget
{
        Q_OBJECT

public:
        QuatschSourceEditor(QWidget *parent=0);
        virtual ~QuatschSourceEditor() {}

        void setCode (QString code);
        QString code () const;

        void setPicohelpBrowser (picogen::qt4::PicohelpBrowser *browser);

signals:
        void codeChanged () ;
        void helpBrowserVisibilityRequested();

private slots:
        void on_interestingPrograms_clicked();
        void on_edit_selectionChanged();
        void on_help_clicked();
        void contextHelpTriggered();

        void on_compileAndRunButton_clicked();
        void on_edit_textChanged ();
        void on_edit_cursorPositionChanged();

private:
        void closeEvent(QCloseEvent *event);

        bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses);
        bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses);
        void createParenthesisSelection(int pos);

        Ui::QuatschEditor *ui;
        QuatschHighlighter *highlighter;
        bool softLock;

        picogen::qt4::PicohelpBrowser *helpBrowser;
};


#endif // QUATSCH_EDITOR_HH_INCLUDED_20090419
