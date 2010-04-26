/********************************************************************************
** Form generated from reading UI file 'buttonwidget.ui'
**
** Created: Thu Feb 11 19:26:45 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONWIDGET_H
#define UI_BUTTONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_buttonwidgetClass
{
public:
    QLabel *buttonwidgetlabel;

    void setupUi(QWidget *buttonwidgetClass)
    {
        if (buttonwidgetClass->objectName().isEmpty())
            buttonwidgetClass->setObjectName(QString::fromUtf8("buttonwidgetClass"));
        buttonwidgetClass->resize(128, 360);
        buttonwidgetlabel = new QLabel(buttonwidgetClass);
        buttonwidgetlabel->setObjectName(QString::fromUtf8("buttonwidgetlabel"));
        buttonwidgetlabel->setGeometry(QRect(0, 0, 138, 360));
        buttonwidgetlabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"background-image: url(:/graphics/gfx/SnesButtons.bmp);\n"
"}"));

        retranslateUi(buttonwidgetClass);

        QMetaObject::connectSlotsByName(buttonwidgetClass);
    } // setupUi

    void retranslateUi(QWidget *buttonwidgetClass)
    {
        buttonwidgetClass->setWindowTitle(QApplication::translate("buttonwidgetClass", "buttonwidget", 0, QApplication::UnicodeUTF8));
        buttonwidgetlabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class buttonwidgetClass: public Ui_buttonwidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONWIDGET_H
