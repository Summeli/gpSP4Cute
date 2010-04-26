#include "videosettings.h"

videosettings::videosettings(int frameskip, bool showFPS,
		QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.frameSkipBox->setCurrentIndex( frameskip );
	ui.showFPS->setChecked(showFPS);
	
	connect(ui.frameSkipBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setFrameskip(int)));
	connect(ui.showFPS, SIGNAL(toggled(bool )), this, SLOT(showFPSChecked(bool)));
}

videosettings::~videosettings()
{

}

void videosettings::setFrameskip( int framestoskip )
	{
	emit (frameskip( framestoskip ));
	}


void videosettings::showFPSChecked( bool fps )
	{
	emit( showFPS(fps) );
	}
