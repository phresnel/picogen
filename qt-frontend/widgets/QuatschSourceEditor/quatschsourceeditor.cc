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

#include <iostream>
#include "redshift/include/redshift.hh"
#include "quatschsourceeditor.hh"
#include <QCloseEvent>





QuatschSourceEditor::QuatschSourceEditor(QWidget *parent)
: QWidget(parent), ui(new Ui::QuatschEditor)
{
        setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(this);

        highlighter = new QuatschHighlighter (ui->edit->document());

        QFont font;
        font.setStyleHint (QFont::TypeWriter, QFont::PreferAntialias);
        font.setFamily("Monospace");
        font.setFixedPitch(true);
        font.setPointSize(9);
        ui->edit->setFont (font);

        ui->edit->setText (
                "// Example code:\n"
                "(defun $main (x y) (if (< x y) x y))"
        );
}



void QuatschSourceEditor::setCode(QString code) {
        ui->edit->setText (code);
}



QString QuatschSourceEditor::code () const {
        return ui->edit->toPlainText();
}



void QuatschSourceEditor::on_edit_textChanged () {
        emit codeChanged();
}



void QuatschSourceEditor::closeEvent(QCloseEvent *event) {
}






#include <QtGui>

QuatschHighlighter::QuatschHighlighter (QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
        HighlightingRule rule;

        keywordFormat.setForeground(QBrush(QColor(200,200,255)));
        //keywordFormat.setFontWeight(QFont::Bold);
        keywordFormat.setFontItalic(true);
        QStringList keywordPatterns;

        keywordPatterns
                << QRegExp::escape("+")
                << QRegExp::escape("-")
                << QRegExp::escape("*")
                << QRegExp::escape("/")
                << QRegExp::escape("^")
                << QRegExp::escape("<")
                << QRegExp::escape(">")
                << QRegExp::escape("<=")
                << QRegExp::escape(">=")
                << QRegExp::escape("<>")
                << QRegExp::escape("=")
                << QRegExp::escape("!=")
                << QRegExp::escape("[]")
                << QRegExp::escape("[[")
                << QRegExp::escape("]]")
                << QRegExp::escape("][")
                << "\\bmin\\b"
                << "\\bmax\\b"
                << "\\band\\b"
                << "\\bor\\b"
                << "\\blerp\\b"

                << "\\bsin\\b"
                << "\\bcos\\b"
                << "\\bfloor\\b"
                << "\\babs\\b"
                << "\\btrunc\\b"
                << "\\bfrac\\b"
                << "\\bneg\\b"
                << "\\bnot\\b"
                << "\\bsqrt\\b"
                << "\\blog\\b"
                << "\\blog10\\b"
                << "\\bexp\\b"

                << "\\bdelta\\b"
                << "\\bxor\\b"

                << "\\bif\\b"

                << "\\bdefun\\b"
                ;

        foreach (QString pattern, keywordPatterns) {
                rule.pattern = QRegExp(pattern);
                rule.format = keywordFormat;
                highlightingRules.append(rule);
        }

        singleLineCommentFormat.setForeground(QBrush(QColor(255,200,200)));
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);


        /*
        parametersFormat.setForeground(Qt::darkGreen);
        parametersFormat.setFontWeight(QFont::Bold);
        parametersFormat.setFontItalic(true);
        rule.pattern = QRegExp("\\b[xy]\\b");
        rule.format = parametersFormat;
        highlightingRules.append(rule);*/

        numberFormat.setForeground(Qt::magenta);
        rule.pattern = QRegExp("\\b[0-9]+(\\.[0-9]+)?\\b");
        rule.format = numberFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(QBrush(QColor(255,200,200)));

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(QBrush(QColor(200,255,200)));
        //rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
        rule.pattern = QRegExp("\\$[A-Za-z\\-_][A-Za-z\\-_0-9]*\\b");
        rule.format = functionFormat;
        highlightingRules.append(rule);

        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
}



void QuatschHighlighter::highlightBlock(const QString &text) {
        foreach (HighlightingRule rule, highlightingRules) {
                QRegExp expression(rule.pattern);
                int index = text.indexOf(expression);
                while (index >= 0) {
                        int length = expression.matchedLength();
                        setFormat(index, length, rule.format);
                        index = text.indexOf(expression, index + length);
                }
        }
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
                startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0) {
                int endIndex = text.indexOf(commentEndExpression, startIndex);
                int commentLength;
                if (endIndex == -1) {
                        setCurrentBlockState(1);
                        commentLength = text.length() - startIndex;
                } else {
                        commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
                }
                setFormat(startIndex, commentLength, multiLineCommentFormat);
                startIndex = text.indexOf(commentStartExpression,
                                                 startIndex + commentLength);
        }
}
