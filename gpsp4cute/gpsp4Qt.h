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


#ifndef GPSP4QT_H
#define GPSP4QT_H

#include <QtGui/QMainWindow>
#include <QKeyEvent>
#include <QList>
#include <QThread>

#include "QBlitterWidget.h"
#include "dpadwidget.h" 
#include "gpspSettings.h"
#include "MEmulatorAdaptation.h"
#include "QRemoteControlKeys.h"
#include "cuteErrorDialog.h"

#include "dpadwidget.h"
#include "smalloptionswidget.h"
#include "rightbuttonwidget.h"
#include "gpspadaptation.h"
#include "AntAudio.h"

/* This class is the UI controller in the UI thread side
 * it's only purpose is to keep things in order in the UI
 * thread side */
class gpsp4Qt : public QMainWindow, MEmulatorAdaptation
{
    Q_OBJECT

public:
    gpsp4Qt(QWidget *parent = 0);
    ~gpsp4Qt();

public:
    void setRemoteControl( QRemoteControlKeys* remote );
    void keyPressEvent( QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent* event);
public: //From MEmulatorAdaptation
    int getKeyEvent( antKeyEvent& keyEvent );
public:
    void LoadROM(QString rom, TGPSPSettings antSettings);
    void LoadState( int state );
    void SaveState( int state );
    void reset();
    void continueGame();
    
signals:
    void Start();
    void Stop();
    void doLoadROM(QString rom, TGPSPSettings antSettings);
    void showmenu();
    void saveState(int state);
    void loadState( int state);
    void resetgpsp();
    void exitgpsp();
    
    
public slots:
    void showAntSnesMenu();
    void updateSettings( TGPSPSettings );
    void virtualKeyEvent( quint32 aKey, bool isDown );
    void showErrorNote( QString message );
    void errorNoteDismissed();
    		
private slots:
	void listencontrols();
    
private:
    QBlitterWidget* widget;
    CAntAudio* m_audio;
    TGPSPSettings iSettings;
    QList<antKeyEvent> iPressedKeys;
    QRemoteControlKeys* remotecontrol;
    DPadWidget* dpad;
    gpspadaptation* adaptation;
    QString currentROM;
    cuteErrorDialog* errorDialog;
    smalloptionswidget* smallwidget;
    rightbuttonwidget* rsmallwidget;
};

#endif // GPSP4QT_H
