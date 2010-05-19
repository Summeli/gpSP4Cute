#ifndef SMALLGPSPLOGO_H
#define SMALLGPSPLOGO_H

#include <QtGui/QWidget>
#include "ui_smallgpsplogo.h"

class smallgpsplogo : public QWidget
{
    Q_OBJECT

public:
    smallgpsplogo(QWidget *parent = 0);
    ~smallgpsplogo();

private:
    Ui::smallgpsplogoClass ui;
};

#endif // SMALLGPSPLOGO_H
