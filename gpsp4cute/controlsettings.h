#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QtGui/QWidget>
#include "ui_controlsettings.h"

class controlsettings : public QWidget
{
    Q_OBJECT

public:
    controlsettings(int screensettings, QWidget *parent = 0);
    ~controlsettings();

public slots: 
	 void keyConfig();
	 void setScreenSettings( int settings );

signals:
	 void runkeyconfig();
	 void screensettings(int settings);
	 
private:
    Ui::controlsettingsClass ui;
};

#endif // CONTROLSETTINGS_H
