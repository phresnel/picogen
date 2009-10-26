/********************************************************************************
** Form generated from reading UI file 'qtsunskyeditor.ui'
**
** Created: Mon Oct 26 08:06:40 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSUNSKYEDITOR_H
#define UI_QTSUNSKYEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qtsunskyeditor
{
public:
    QGridLayout *gridLayout;
    QLabel *previewScreen;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QDial *dial;
    QSpinBox *spinBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QDial *dial_2;
    QSpinBox *spinBox_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *qtsunskyeditor)
    {
        if (qtsunskyeditor->objectName().isEmpty())
            qtsunskyeditor->setObjectName(QString::fromUtf8("qtsunskyeditor"));
        qtsunskyeditor->resize(496, 300);
        qtsunskyeditor->setStyleSheet(QString::fromUtf8("QWidget {\n"
"	background-color: rgb(90, 90, 90);\n"
"	color: rgb(255,255,255);\n"
"	border-width: 0px;\n"
"	border-style: solid;\n"
"	border-color: transparent;\n"
"	border-radius:5px;\n"
"}\n"
"QPushButton:disabled, \n"
"QComboBox:disabled,\n"
"QSpinBox:disabled, \n"
"QDoubleSpinBox:disabled {\n"
"	background-color: rgb(90, 90, 90);\n"
"	color: rgb(140,140,140);\n"
"	border-color: rgb(120,120,120)\n"
"}\n"
"\n"
"QPushButton {\n"
"	border-width: 2px;\n"
"	border-color: rgb(150,150,150);\n"
"}\n"
"QLabel {\n"
"	border-color: transparent;\n"
"	color: rgb(170,170,170);\n"
"}\n"
"\n"
"QGroupBox {\n"
"	background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 rgb(110, 110, 110), stop: 1 rgb(140, 130, 120));\n"
"	margin-top: 1.5ex; /* leave space at the top for the title */\n"
"	border-radius: 16px;\n"
"}\n"
"QGroupBox::title {\n"
"	subcontrol-origin: margin;\n"
"	subcontrol-position: top center; /* position at the top center */\n"
"	color: rgb(255, 193, 100);\n"
""
                        "}\n"
"QGraphicsView, #previewScreen {\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,\n"
"             stop:0 rgb(108, 128, 108), \n"
"             stop:1 rgb(0, 0, 0));\n"
"	border: 2px solid gray;\n"
"	border-radius: 5px;\n"
"	border-color: rgb(120, 120, 120);\n"
"	margin-top: 1ex; /* leave space at the top for the title */\n"
"}\n"
""));
        gridLayout = new QGridLayout(qtsunskyeditor);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        previewScreen = new QLabel(qtsunskyeditor);
        previewScreen->setObjectName(QString::fromUtf8("previewScreen"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(previewScreen->sizePolicy().hasHeightForWidth());
        previewScreen->setSizePolicy(sizePolicy);
        previewScreen->setMinimumSize(QSize(256, 256));
        previewScreen->setMaximumSize(QSize(16777215, 16777215));
        previewScreen->setSizeIncrement(QSize(0, 0));

        gridLayout->addWidget(previewScreen, 0, 0, 3, 1);

        groupBox = new QGroupBox(qtsunskyeditor);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        dial = new QDial(groupBox);
        dial->setObjectName(QString::fromUtf8("dial"));
        sizePolicy1.setHeightForWidth(dial->sizePolicy().hasHeightForWidth());
        dial->setSizePolicy(sizePolicy1);
        dial->setStyleSheet(QString::fromUtf8(""));
        dial->setMaximum(360);
        dial->setTracking(true);
        dial->setInvertedAppearance(false);
        dial->setInvertedControls(false);
        dial->setWrapping(true);
        dial->setNotchesVisible(true);

        verticalLayout->addWidget(dial);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximum(360);

        verticalLayout->addWidget(spinBox);


        gridLayout->addWidget(groupBox, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(qtsunskyeditor);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        dial_2 = new QDial(groupBox_2);
        dial_2->setObjectName(QString::fromUtf8("dial_2"));
        dial_2->setMaximum(90);
        dial_2->setNotchesVisible(true);

        verticalLayout_2->addWidget(dial_2);

        spinBox_2 = new QSpinBox(groupBox_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMaximum(90);

        verticalLayout_2->addWidget(spinBox_2);


        gridLayout->addWidget(groupBox_2, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);


        retranslateUi(qtsunskyeditor);
        QObject::connect(dial, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));
        QObject::connect(dial_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
        QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), dial_2, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(qtsunskyeditor);
    } // setupUi

    void retranslateUi(QWidget *qtsunskyeditor)
    {
        qtsunskyeditor->setWindowTitle(QApplication::translate("qtsunskyeditor", "QtSunSkyEditor", 0, QApplication::UnicodeUTF8));
        previewScreen->setText(QString());
        groupBox->setTitle(QApplication::translate("qtsunskyeditor", "Yaw", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("qtsunskyeditor", "Pitch", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class qtsunskyeditor: public Ui_qtsunskyeditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSUNSKYEDITOR_H
