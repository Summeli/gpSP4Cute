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

#ifndef AUDIO_H_
#define AUDIO_H_

#include <QObject>

#ifdef ENABLE_QTAUDIO
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QTimer>
#endif

/**
 * Abstracted the audio stuff for emulator port
 * the audio can use platform specific audio output,
 * or Qt's mobile extensions
 */
class audio : public QObject 
{
	Q_OBJECT

public:
    audio( QObject* parent = 0 );
    ~audio();
 
public slots:
	 void initAudio();
	 void startAudio();
	 void stopAudio();
	 bool isStarted();
	 void setVolume( int vol );
	 
#ifdef ENABLE_QTAUDIO    
private:
    void initQtAudio();
    void checkAudioDevices();
    
public slots:
	void pullTimerExpired();
	void notified();
	void stateChanged(QAudio::State state);

private:	

    QAudioOutput* qaudio;
    QAudioFormat audioformat;
    QBuffer* audioOut;
    QIODevice* m_output; //now owned
    QByteArray* audioBuf;
    QTimer* m_pullTimer;
#endif
private:
    bool m_started;
    bool m_initialized;
    int m_volume;
};
	
#endif /* AUDIO_H_ */
