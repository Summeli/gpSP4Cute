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
#include <QtOpenGL/QGLWidget>
#include <QImage>

#include "dpadwidget.h" 
#include "gpspSettings.h"
#include "MEmulatorAdaptation.h"
#include "cuteErrorDialog.h"
#include "dpadwidget.h"
#include "rightbuttonwidget.h"

#ifdef __SYMBIAN32__
#include "AntAudio.h"
#include "QRemoteControlKeys.h"
#else
#include "meegoAudio.h"
#include "meegoAdaptation.h"
#endif
class gpspadaptation;

/* This class is the UI controller in the UI thread side
 * it's only purpose is to keep things in order in the UI
 * thread side */
class gpsp4Qt : public QGLWidget, MEmulatorAdaptation
{
    Q_OBJECT

public:
    gpsp4Qt(QWidget *parent = 0);
    ~gpsp4Qt();

public:
    void keyPressEvent( QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent* event);
    bool event(QEvent *event);

//rendering stuff
public slots:
    void render();

protected:
    void paintEvent(QPaintEvent *);

public: //From MEmulatorAdaptation
    quint32 getGpspKeys();

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
    void showErrorNote( QString message );
    void errorNoteDismissed();		
    
private:
    void LoadButtons();
    void ApplyTransparency(QPixmap &pm, QString png);

private:
    CAntAudio* m_audio;
    TGPSPSettings m_settings;
    DPadWidget* m_dpad;
    gpspadaptation* m_adaptation;
    QString m_currentROM;
    cuteErrorDialog* m_errorDialog;
    rightbuttonwidget* m_rightButtons;
    
    quint32 m_softKeys;
    quint32 m_hardKeys;

    //drawing
    QImage* m_buf;
    QPixmap m_dpadGraphics;
    QPixmap m_buttonsGraphics;
    QPixmap m_tlGraphics;
    QPixmap m_trGraphics;
    QPixmap m_menuGraphics;
    QPixmap m_startSelectGraphics;
    int m_buttonOpacity;
    int m_stretch;
    int m_screenHeight;
#ifdef __SYMBIAN32__
public:
    void setRemoteControl( QRemoteControlKeys* remote );
private: //data
      QRemoteControlKeys* remotecontrol;
private slots:
    void listencontrols();
#else
private: //Data
    meegoAdaptation* meegoAdapt;
#endif
};

#endif // GPSP4QT_H
