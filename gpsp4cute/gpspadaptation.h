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

#include "gpspSettings.h"
#include "gpsp4Qt.h"

#ifdef __SYMBIAN32__
#include "AntAudio.h"
#else
#include "meegoAudio.h"
#endif

/* the gpspadapation is basically the emulation thread
 * it's also passing commands safely from the UI thread
 * into the gpsp via Qts signal/slot system */
class gpspadaptation : public QThread 
{
    Q_OBJECT

public:
    gpspadaptation( gpsp4Qt* widget, CAntAudio* audioInterface );
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
    void updateSettings( TGPSPSettings settings );
    QString gameconfigpath();
    QString getMainpath();
    void showErrorNote( QString message );
    void audioFrameMixed();

signals:
    void frameblit();
    void dispatchErrorNote( QString message );
    void audioFrameReady();

private:
    gpsp4Qt* m_blitter; //not owned
    
    CAntAudio* m_audio;
    TGPSPSettings m_settings;
    QString m_rom;
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
