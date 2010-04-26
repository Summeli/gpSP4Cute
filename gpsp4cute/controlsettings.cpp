#include "controlsettings.h"

controlsettings::controlsettings(int screensettings, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.screensettings->setCurrentIndex( screensettings);
	
	connect(ui.keyconfigButton, SIGNAL(clicked()), this, SLOT(keyConfig()));
	connect(ui.screensettings, SIGNAL(currentIndexChanged(int)), this, SLOT(setScreenSettings(int)));
}

controlsettings::~controlsettings()
{

}

void controlsettings::keyConfig()
	{
	emit( runkeyconfig() );
	}

void controlsettings::setScreenSettings( int settings )
	{
	emit( screensettings(settings) );
	}
