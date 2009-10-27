/********************************************************************************
** Form generated from reading UI file 'qtsunskyeditor.ui'
**
** Created: Tue Oct 27 09:54:06 2009
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
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qtsunskyeditor
{
public:
    QGridLayout *gridLayout;
    QLabel *previewScreen;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *previewResolution;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QDial *turbidityDial;
    QDoubleSpinBox *turbiditySpinBox;

    void setupUi(QWidget *qtsunskyeditor)
    {
        if (qtsunskyeditor->objectName().isEmpty())
            qtsunskyeditor->setObjectName(QString::fromUtf8("qtsunskyeditor"));
        qtsunskyeditor->resize(550, 421);
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
"QGroupBox > QGroupBox::title {\n"
"	subcontrol-origin: margin;\n"
"	subcontrol-position: top center; /* position at the top center */\n"
"	color: rgb(200, 160, 90);\n"
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

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_3 = new QGroupBox(qtsunskyeditor);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        formLayout = new QFormLayout(groupBox_3);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        previewResolution = new QSpinBox(groupBox_3);
        previewResolution->setObjectName(QString::fromUtf8("previewResolution"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(previewResolution->sizePolicy().hasHeightForWidth());
        previewResolution->setSizePolicy(sizePolicy2);
        previewResolution->setMinimum(1);
        previewResolution->setMaximum(4);

        formLayout->setWidget(0, QFormLayout::FieldRole, previewResolution);


        verticalLayout_3->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(qtsunskyeditor);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy3);
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_2 = new QGroupBox(groupBox_4);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy4);
        groupBox_2->setFlat(false);
        groupBox_2->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        turbidityDial = new QDial(groupBox_2);
        turbidityDial->setObjectName(QString::fromUtf8("turbidityDial"));
        turbidityDial->setMinimum(165);
        turbidityDial->setMaximum(4000);
        turbidityDial->setNotchTarget(37);
        turbidityDial->setNotchesVisible(true);

        verticalLayout_2->addWidget(turbidityDial);

        turbiditySpinBox = new QDoubleSpinBox(groupBox_2);
        turbiditySpinBox->setObjectName(QString::fromUtf8("turbiditySpinBox"));
        turbiditySpinBox->setMinimum(1.65);
        turbiditySpinBox->setMaximum(40);
        turbiditySpinBox->setSingleStep(0.01);
        turbiditySpinBox->setValue(2.2);

        verticalLayout_2->addWidget(turbiditySpinBox);


        verticalLayout_4->addWidget(groupBox_2);


        verticalLayout_3->addWidget(groupBox_4);


        gridLayout->addLayout(verticalLayout_3, 0, 1, 1, 1);


        retranslateUi(qtsunskyeditor);

        QMetaObject::connectSlotsByName(qtsunskyeditor);
    } // setupUi

    void retranslateUi(QWidget *qtsunskyeditor)
    {
        qtsunskyeditor->setWindowTitle(QApplication::translate("qtsunskyeditor", "QtSunSkyEditor", 0, QApplication::UnicodeUTF8));
        previewScreen->setText(QString());
        groupBox_3->setTitle(QApplication::translate("qtsunskyeditor", "Preview Options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("qtsunskyeditor", "Preview Resolution", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("qtsunskyeditor", "Sun/Sky-Settings", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("qtsunskyeditor", "Turbidity", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class qtsunskyeditor: public Ui_qtsunskyeditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSUNSKYEDITOR_H
