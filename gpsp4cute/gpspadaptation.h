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


#ifndef GPSP_ADAPTATION_H_
#define GPSP_ADAPTATION_H_

#include <QThread>


#include "QBlitterWidget.h"
#include "gpspSettings.h"

//#define ENABLE_AUDIO
#ifdef ENABLE_AUDIO
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QTimer>
#endif

/* the gpspadapation is basically the emulation thread
 * it's also passing commands safely from the UI thread
 * into the gpsp via Qts signal/slot system */
class gpspadaptation : public QThread 
{
    Q_OBJECT

public:
    gpspadaptation( QBlitterWidget* widget );
    ~gpspadaptation();
    
    virtual void run();
public:
    void blit( const quint16* screen );
public slots:
    
    void Start();
    void Stop();
    void ResetGame();
    void LoadRom( QString aFileName,  TGPSPSettings settings );
    
    void savegpspState( int aState );
    void loadgpspState( int aState );    
    void exitgpsp();

public:
    void showgpspFPS( bool fps );
    QString gameconfigpath();
    QString getMainpath();
    void showErrorNote( QString message );

#ifdef ENABLE_AUDIO
private:
    void initAudio();
    void checkAudioDevices();
    
public slots:
	void pullTimerExpired();
	void notified();
	void stateChanged(QAudio::State state);
#endif
	
signals:
    void frameblit();
    void dispatchErrorNote( QString message );
    
private:
    QBlitterWidget* blitter; //not owned
    TGPSPSettings gsettings;
    QString rom;

#ifdef ENABLE_AUDIO    
    //AUDIO
    QAudioOutput* qaudio;
    QAudioFormat audioformat;
    QBuffer* audioOut;
    QIODevice* m_output; //now owned
    QByteArray* audioBuf;
    QTimer* m_pullTimer;
#endif
};


// function directly from the UI to check bios file
#ifdef __cplusplus
extern "C" {
#endif

	bool isBiosValid( const char* bios );
	
#ifdef __cplusplus
};
#endif

#endif /* GPSP_ADAPTATION_H_ */
