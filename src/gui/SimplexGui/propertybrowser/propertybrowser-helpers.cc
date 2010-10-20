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

#include <stdexcept>

#include <QtProperty>
#include <QtTreePropertyBrowser>

QString readValueText (QString name, QList<QtProperty*> list) {
        foreach (QtProperty* looky, list) {
                if (name == looky->propertyName()) {
                        return looky->valueText();
                }
        }
        return "";
}
QString readValueText (QString name, QtProperty *prop) {
        return readValueText(name, prop->subProperties());
}



QList<QtProperty*> readSubProperties (QString name, QList<QtProperty*> list) {
        foreach (QtProperty* looky, list) {
                if (name == looky->propertyName()) {
                        return looky->subProperties();
                }
        }
        return QList<QtProperty*>();
}
QList<QtProperty*> readSubProperties (QString name, QtProperty* prop) {
        return readSubProperties(name, prop->subProperties());
}
QtProperty* readSubProperty (QString name, QList<QtProperty*> list) {
        foreach (QtProperty* looky, list) {
                if (name == looky->propertyName()) {
                        return looky;
                }
        }
        return 0;
}
QtProperty* readSubProperty (QString name, QtProperty* prop) {
        return readSubProperty (name, prop->subProperties());
}



QtProperty *findParent(QtProperty *root, QtProperty* child) {
        if (root == child)
                return 0;
        foreach (QtProperty *prop, root->subProperties()) {
                if(prop == child)
                        return root;
                if (QtProperty *n = findParent(prop, child))
                        return n;
        }
        return 0;
}
QtProperty *findParent(QList<QtProperty*> props, QtProperty* child) {
        foreach (QtProperty *prop, props) {
                if (QtProperty *curr = findParent (prop, child))
                        return curr;
        }
        return 0;
}



void collapse (QtTreePropertyBrowser *browser,
               QtProperty* property
) {
        foreach (QtBrowserItem *item, browser->items (property)) {
                browser->setExpanded(item, false);
        }
}



bool isFirst(QtProperty *root, QtBrowserItem* node) {
        if (0 == root->subProperties().count())
                throw std::runtime_error("mainwindow.cc::isFirst() called on empty collection");
        return root->subProperties().first() == node->property();

}
bool isLast(QtProperty *root, QtBrowserItem* node) {
        if (0 == root->subProperties().count())
                throw std::runtime_error("mainwindow.cc::isFirst() called on empty collection");
        return root->subProperties().last() == node->property();
}



void moveUp(QtTreePropertyBrowser *browser, QtProperty *root, QtBrowserItem* node) {
        QtProperty *parent = findParent(root, node->property());
        if (parent == 0)
                return;

        QtProperty *prev = 0;
        foreach (QtProperty *curr, parent->subProperties()) {
                if (curr == node->property()) {
                        if (prev == 0) return;

                        const bool isExpandedPrev = browser->isExpanded(browser->items(prev)[0]);
                        const bool isExpandedCurr = browser->isExpanded(browser->items(curr)[0]);
                        parent->removeSubProperty(prev);
                        parent->insertSubProperty(prev, curr);
                        browser->setExpanded(browser->items(curr)[0], isExpandedCurr);
                        browser->setExpanded(browser->items(prev)[0], isExpandedPrev);
                        return;
                }
                prev = curr;
        }
}
void moveDown(QtTreePropertyBrowser *browser, QtProperty *root, QtBrowserItem* node) {
        QtProperty *parent = findParent(root, node->property());
        if (parent == 0)
                return;

        QtProperty *prev = 0;
        foreach (QtProperty *curr, parent->subProperties()) {
                if (prev == node->property()) {
                        if (prev == 0) return;

                        const bool isExpandedPrev = browser->isExpanded(browser->items(prev)[0]);
                        const bool isExpandedCurr = browser->isExpanded(browser->items(curr)[0]);
                        parent->removeSubProperty(prev);
                        parent->insertSubProperty(prev, curr);
                        browser->setExpanded(browser->items(curr)[0], isExpandedCurr);
                        browser->setExpanded(browser->items(prev)[0], isExpandedPrev);
                        return;
                }
                prev = curr;
        }
}



