#ifndef RIGHTBUTTONWIDGET_H
#define RIGHTBUTTONWIDGET_H

#include <QtGui/QWidget>
#include "ui_rightbuttonwidget.h"

class rightbuttonwidget : public QWidget
{
    Q_OBJECT

public:
    rightbuttonwidget(QWidget *parent = 0);
    ~rightbuttonwidget();

private:
    Ui::rightbuttonwidgetClass ui;
};

#endif // RIGHTBUTTONWIDGET_H
