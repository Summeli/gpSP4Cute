/* gpsp4cute
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <QFileDialog>
#include <QString>
#include <QSettings>
#include <QDir>

#include "emusettings.h"
#include "cuteDebug.h"

#define KSettingsVersion 4

EmuSettings::EmuSettings(QWidget *parent)
    : QMainWindow(parent)
{
	loadSettings();
	
	ui.setupUi(this);
	showFullScreen();
	
	saveSlotIndexChanged(gpspsettings.iLastSlot);
	//int audioOn, int samplerte, int stereoOn, int volume, bool enablespeedhack, QWidget *parent 
	//TODO: read settings
	audiosettings = new AudioSettings(gpspsettings.iAudioOn, gpspsettings.iSampleRate, 
										gpspsettings.iStereo, gpspsettings.iVolume, this );
	audiosettings->setGeometry(QRect(0, 0, 640, 150));
	audiosettings->hide();
	
	antvideosettings =new videosettings( gpspsettings.iFrameSkip, 
					gpspsettings.iShowFPS, this );
	antvideosettings->setGeometry(QRect(0, 0, 640, 150));
	antvideosettings->hide();
	
	fileview = new filewidget( gpspsettings.iLastROM, gpspsettings.iBios,
			this );
	fileview->setGeometry(QRect(0, 0, 640, 150));
	fileview->hide();
		
	keysettings =new controlsettings( gpspsettings.iScreenSettings, this );
	keysettings->setGeometry(QRect(0, 0, 640, 150));
	keysettings->hide();
	
	currentWidget = EMainWidget;
	
	ui.saveSlotBox->setCurrentIndex(gpspsettings.iLastSlot);
	
	//connect buttons
	connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(showFileMenu()));
	connect(ui.audioButton, SIGNAL(clicked()), this, SLOT(showAudioSettings()));
	connect(ui.videoButton, SIGNAL(clicked()), this, SLOT(showVideoSettings()));
	connect(ui.controlsButton, SIGNAL(clicked()), this, SLOT(showControlSettings()));
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui.loadStateButton, SIGNAL(clicked()), this, SLOT(loadStateButtonClicked()));
	connect(ui.saveStateButton, SIGNAL(clicked()), this, SLOT(saveStateButtonClicked()));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
	connect(ui.ContinueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
	connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(aboutClicked()));
	connect(ui.saveSlotBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveSlotIndexChanged(int)));
	
	//audio is not yet supported
	ui.audioButton->setEnabled( FALSE );
	
	//connect audio settins
	connect( audiosettings, SIGNAL(AudioOn(int)), this, SLOT( setAudioOn(int)));
	connect( audiosettings, SIGNAL(SampleRate(int)), this, SLOT( setSampleRate(int)));
	connect( audiosettings, SIGNAL(enableStereo(int)), this, SLOT( setStereoOn(int)));
	connect( audiosettings, SIGNAL(Volume(int)), this, SLOT( setVolume(int)));
	
	//connect video settings
	connect( antvideosettings, SIGNAL(frameskip(int)), this, SLOT( frameskip(int) ));
	connect( antvideosettings, SIGNAL(showFPS(bool)), this, SLOT( showFPS(bool) ));
	

	//connect filewidget
	connect( fileview, SIGNAL(loadROM()), this, SLOT( loadROM()));
	connect( fileview, SIGNAL(setBios()), this, SLOT( setBios()));
	
	//connect control settings
	connect( keysettings, SIGNAL(runkeyconfig()), this, SLOT( keyConfig() ));
	connect( keysettings, SIGNAL(screensettings(int)), this, SLOT( screensettings(int) ));
	
	romloaded = false;
	settingsChanged = false;
	errorDialog = NULL;
}

EmuSettings::~EmuSettings()
{
	savecurrentSettings();
	
	//delete all views
	delete keysettings;
	delete antvideosettings;
	delete audiosettings;
	delete fileview;
}

void EmuSettings::setRemoteControl( QRemoteControlKeys* remote )
	{
	remotecontrol = remote;
	}
void EmuSettings::loadROM()
    {    
    __DEBUG_IN
    QDir dir(gpspsettings.iLastROM);
    QString rom = QFileDialog::getOpenFileName(this,
                    tr("Open File"),
                    dir.absolutePath(),
                    tr("ROMs (*.GBA *.gba *.zip *.ZIP);;"));
    if( rom.isEmpty() )
        {
		return;
        }
    
    gpspsettings.iLastROM = rom;
    savecurrentSettings();
    fileview->setRomLabel( rom );
    emit( LoadROM( rom,gpspsettings ));
    romloaded = true;
    __DEBUG_OUT
    }

void EmuSettings::setBios()
	{
    savecurrentSettings();
    QDir dir(gpspsettings.iBios);
    QString bios = QFileDialog::getOpenFileName(this,
                    tr("Bios"),
                    dir.absolutePath(),
                    tr("ROMs (*.bin *.BIN);;"));
    if( bios.isEmpty() )
        {
		return;
        }
    
    fileview->setBiosLabel( bios );
    gpspsettings.iBios = bios;
    
    //check that bios is valid
    if( !isBiosValid( bios.toStdString().c_str() ) )
    	{
		//dispatch error note
		QString error;
		error.append("You have an incorrect BIOS image.                 \n");
		error.append("While many games will work fine, some will not. It\n");
		error.append("is strongly recommended that you obtain the       \n");
		error.append("correct BIOS file. Do NOT report any bugs if you  \n");
		error.append("are seeing this message.                          \n");
		errorDialog = new cuteErrorDialog( error, this );
		connect(errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
		errorDialog->show();
		errorDialog->setFocus();
    	}
	}

void EmuSettings::showFileMenu()
	{
	if( currentWidget == EFileWidget )
		{
		//hide file Widget, and continue
		fileview->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		fileview->show();
		currentWidget = EFileWidget;
		}
	}

void EmuSettings::keyConfig()
    {
    keydialog = new keyconfigdialog( this );
    connect(keydialog, SIGNAL(configDone()), this, SLOT(keyconfigDone()));
    remotecontrol->subscribeKeyEvent( keydialog );
    keydialog->show();
    keydialog->setFocus();
    }

void EmuSettings::frameskip( int skip )
    {
	__DEBUG_IN
	__DEBUG2("Frameskip is", skip );
	settingsChanged = true;
    gpspsettings.iFrameSkip = skip;
	__DEBUG_OUT
    }

void EmuSettings::screensettings( int settings )
	{
	__DEBUG_IN
	__DEBUG2("current screensetetings are", settings );
	settingsChanged = true;
    gpspsettings.iScreenSettings = settings;
	__DEBUG_OUT
	}

void EmuSettings::showFPS( bool showFPS )
	{
	__DEBUG_IN
	settingsChanged = true;
	gpspsettings.iShowFPS = showFPS;
	__DEBUG_OUT
	}

void EmuSettings::showAudioSettings()
	{
	if( currentWidget == EAudioWidget )
		{
		//hide audio Widget, and continue
		audiosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		audiosettings->show();
		currentWidget = EAudioWidget;
		}
	}

void EmuSettings::showVideoSettings()
	{
	if( currentWidget == EVideoWidget )
		{
		//hide video Widget, and continue
		antvideosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		antvideosettings->show();
		currentWidget = EVideoWidget;
		}
	
	}

void EmuSettings::showControlSettings()
	{
	if( currentWidget == EKeyWidget )
		{
		//hide keysettings Widget, and continue
		keysettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		keysettings->show();
		currentWidget = EKeyWidget;
		}
	}

void EmuSettings::setAudioOn( int audioOn )
    {
	settingsChanged = true;
    gpspsettings.iAudioOn = (bool) audioOn;
    }

void EmuSettings::setSampleRate( int sampleRate )
    {
	__DEBUG_IN
	settingsChanged = true;
	__DEBUG2("SampleRate is", sampleRate );
    gpspsettings.iSampleRate = sampleRate;
	__DEBUG_OUT
    }

void EmuSettings::setStereoOn( int stereoOn )
    {
	__DEBUG_IN
	settingsChanged = true;
	gpspsettings.iStereo = (bool) stereoOn;
	__DEBUG_OUT
    }

void EmuSettings::setVolume( int volume )
    {
	settingsChanged = true;
	gpspsettings.iVolume = volume;
    }

void EmuSettings::keyconfigDone()
    {
	settingsChanged = true;
	//get the key config
	for( int i=0; i<10; i++)
		{
		__DEBUG2("keyconfigDone: keyID is ", keydialog->getKeyBind(i) );
		gpspsettings.iScanKeyTable[i] = keydialog->getKeyBind(i);
		}
    //Delete the dialog
	keydialog->hide();
    delete keydialog;
    keydialog = NULL;
    }

void EmuSettings::exit()
	{
	savecurrentSettings();
	QApplication::exit(0);
	}

void EmuSettings::resetButtonClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(gpspsettings) );
		settingsChanged = false;
    	}
    
	if( romloaded )
		emit( resetGame() );
	}

void EmuSettings::continueClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(gpspsettings) );
		settingsChanged = false;
    	}
    
	//if there was no ROM loaded, load previous ROM
	if( !romloaded )
		{
		emit( LoadROM( gpspsettings.iLastROM, gpspsettings ));
	    romloaded = true;
		}
	//just continue the game
	else
		{
		emit( continueGame() );
		}
	}

void EmuSettings::aboutClicked()
	{
	__DEBUG_IN
	//show about dialog
	about = new aboutdialog( this ); 
	connect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	about->show();
	about->setFocus();
	__DEBUG_OUT
	}

void EmuSettings::closeAboutDialog()
	{
	__DEBUG_IN
	disconnect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	delete about;
	about = NULL;
	__DEBUG_OUT
	}

void EmuSettings::errorNoteDismissed()
	{
	__DEBUG_IN
	disconnect(errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
	delete errorDialog;
	errorDialog = NULL;
	__DEBUG_OUT
	}

void EmuSettings::saveSlotIndexChanged( int index )
	{
	__DEBUG_IN
	//change the pixmap
	/*
	QPixmap *pixmap = new QPixmap();
	QString filename = antsettings.iLastROM.left( antsettings.iLastROM.size() - 4 );
	filename.append( QString::number(index) );
	filename.append( ".jpg");
	
	bool loaded = pixmap->load(filename);
	if( loaded )
		{
		ui.stateLabel->setPixmap(*pixmap);
		ui.stateLabel->setScaledContents(true);
		}
	*/
	__DEBUG_OUT
	}

void EmuSettings::loadStateButtonClicked()
	{
	__DEBUG_IN
	gpspsettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
    if( settingsChanged )
    	{
		emit( updateSettings(gpspsettings) );
		settingsChanged = false;
    	}
	int state = ui.saveSlotBox->currentIndex();
	emit( LoadState(state) );
	__DEBUG_OUT
	}

void EmuSettings::saveStateButtonClicked()
	{
	__DEBUG_IN
	gpspsettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
	if( settingsChanged )
	   	{
		emit( updateSettings(gpspsettings) );
		settingsChanged = false;
	    }
	   
	int state = ui.saveSlotBox->currentIndex();
	emit( SaveState(state) );

	__DEBUG_OUT
	}

void EmuSettings::hidecurrentWidget()
	{
	switch( currentWidget )
		{
		case EMainWidget:
			ui.settingsViewWidget->hide();
			break;
		case EAudioWidget:
			audiosettings->hide();
			break;
		case EVideoWidget:
			antvideosettings->hide();
			break;
		case EKeyWidget:
			keysettings->hide();
			break;
		case EFileWidget:
			fileview->hide();
			break;
		}
	}

void EmuSettings::setDefaultSettings()
	{
	__DEBUG_IN
	gpspsettings.iVersion = KSettingsVersion;

	for(int i=0;i<10;i++)
		{
		gpspsettings.iScanKeyTable[i] = 0;
		}
	
	gpspsettings.iLastROM = "";
	gpspsettings.iBios = "";
	gpspsettings.iLastSlot = 0;
	gpspsettings.iShowFPS = false;
	gpspsettings.iFrameSkip = 0;
	gpspsettings.iAudioOn = false;
	gpspsettings.iSampleRate = 22050;
	gpspsettings.iStereo = false;
	gpspsettings.iVolume = 4;
	gpspsettings.iLastSlot = 1;
	gpspsettings.iScreenSettings = 0;
	__DEBUG_OUT
	}

void EmuSettings::savecurrentSettings()
	{
	__DEBUG_IN

	QSettings settings;
	settings.setValue("version", KSettingsVersion );

	for(int i=0;i<10;i++)
		{
	    QString keyval = "kebind";
	    keyval.append( QString::number(i) );
		settings.setValue(keyval, gpspsettings.iScanKeyTable[i]);
		}
	settings.setValue("lastrom",gpspsettings.iLastROM);
	settings.setValue("bios",gpspsettings.iBios);
	settings.setValue("lastslot",gpspsettings.iLastSlot);
	settings.setValue("showfps",gpspsettings.iShowFPS);
	settings.setValue("frameskip",gpspsettings.iFrameSkip);
	settings.setValue("audioOn",gpspsettings.iAudioOn);
	settings.setValue("samplerate",gpspsettings.iSampleRate);
	settings.setValue("stereo",gpspsettings.iStereo);
	settings.setValue("volume",gpspsettings.iVolume);
	settings.setValue("lastslot",gpspsettings.iLastSlot);
	settings.setValue("screensettings", gpspsettings.iScreenSettings);
	settings.sync();
	__DEBUG_OUT
	}

void EmuSettings::loadSettings()
	{
	__DEBUG_IN
	QSettings settings;
	int version = settings.value("version").toInt();
	if( version != KSettingsVersion )
		{
		__DEBUG1("No version was set, creating default settings");
		setDefaultSettings();
		savecurrentSettings();
		return;
		}
	gpspsettings.iVersion = version;

	for(int i=0;i<10;i++)
		{
	    QString keyval = "kebind";
	    keyval.append( QString::number(i) );
	    gpspsettings.iScanKeyTable[i] = settings.value(keyval).toUInt();
		}
	gpspsettings.iLastROM = settings.value("lastrom").toString();
	gpspsettings.iBios = settings.value("bios").toString();
	gpspsettings.iShowFPS = settings.value("showfps").toBool();
	gpspsettings.iFrameSkip = settings.value("frameskip").toInt();
	gpspsettings.iAudioOn = settings.value("audioOn").toBool();
	gpspsettings.iSampleRate = settings.value("samplerate").toInt();
	gpspsettings.iStereo = settings.value("stereo").toBool();
	gpspsettings.iVolume = settings.value("volume").toInt();
	gpspsettings.iLastSlot = settings.value("lastslot").toInt();
	gpspsettings.iScreenSettings = settings.value("screensettings").toInt();
	__DEBUG_OUT
	}
