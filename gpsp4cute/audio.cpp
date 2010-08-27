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

#include "audio.h"

#include "sound_symbian.h"
extern "C" void sound_callback(void *userdata, u8 *stream, int length);

extern u32 audio_buffer_size;

audio::audio(  QObject* parent )
    : QObject(parent), m_started(false), m_initialized( false ), m_volume( 0 )
{
	
}


audio::~audio()
{
   //deinit audio
}

void audio::initAudio()
	{
	Init_Symbian_Audio( sound_frequency, audio_buffer_size / 4 );
	m_initialized = true;
	}

void audio::startAudio()
	{
	if( m_initialized )
		{
		Start_Symbian_Audio( m_volume );
		m_started = true;
		}
	}

void audio::stopAudio()
	{
	Stop_Symbian_Audio();
	m_started = false;
	}

bool audio::isStarted()
	{
	return m_started;
	}

void audio::setVolume( int vol )
	{
	m_volume = vol;
	if( m_initialized )
		{
		set_SymbianVolume( m_volume );
		}
	}

//the Qt adaptation stuff will be used here in the future..
#ifdef ENABLE_QTAUDIO
#include <QAudioDeviceInfo>
#include <QAudioFormat>
void gpspadaptation::pullTimerExpired()
	{
	sound_callback( NULL,(u8*) audioBuf->data(), AudioBufferSize ); //mix the data
	m_output->write(audioBuf->data(), 16 * 2 * 450 ); //write it
	}

void gpspadaptation::notified()
	{
	
	}

void gpspadaptation::stateChanged(QAudio::State state)
	{
	if( state == QAudio::IdleState )
		{

		}
	if( state == QAudio::ActiveState )
		{
		//render
		}
	}


void gpspadaptation::initQtAudio()
	{
	//TO not care
	/*if( gsettings.iAudioOn )
		{
		
		}*/
	
	//DEUBUG
	checkAudioDevices();
	
//we should use the default device

	QAudioFormat format;
	format.setCodec( QString("audio/pcm"));
	format.setSampleRate( 22500 );
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setSampleType( QAudioFormat::SignedInt );
	format.setChannelCount( 2 );
	format.setSampleSize( 16 );
		
	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(format)) {
		 qWarning()<<"raw audio format not supported by backend, cannot play audio.";
		 return;
	 }
	 
	QAudioDeviceInfo defaultdevice =  QAudioDeviceInfo::defaultOutputDevice();
	qaudio = new QAudioOutput( defaultdevice, format, this );
	
    connect(qaudio, SIGNAL(notify()), SLOT(notified()));
    connect(qaudio, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));
    
	audioOut = new QBuffer();

	audioBuf = new QByteArray();
	audioBuf->resize( AudioBufferSize ); 

	/*
	audioOut->setBuffer( audioBuf );
	bool err = audioOut->open(QIODevice::ReadOnly);
	*/

	m_pullTimer = new QTimer(this);
	
    connect(m_pullTimer, SIGNAL(timeout()), SLOT(pullTimerExpired()));
    

    m_output = qaudio->start();
    m_pullTimer->start(20);
	}


// Used only for debuggin purposes
void gpspadaptation::checkAudioDevices()
	{
	QList<QAudioDeviceInfo> devlist = QAudioDeviceInfo::availableDevices ( QAudio::AudioOutput );

	for (int i = 0; i < devlist.size(); ++i)
    	{
		QString device = devlist.at(i).deviceName();
		__DEBUG1( device );
    	}
	
	//we should use the default device
	QAudioDeviceInfo defaultdevice =  QAudioDeviceInfo::defaultOutputDevice();
	
	__DEBUG2( "default output device is:", defaultdevice.deviceName() );
	
	QStringList list = defaultdevice.supportedCodecs();
	__DEBUG1("gpspadaptation: list of supporetd audio codecs: ");
	for (int i = 0; i < list.size(); ++i)
    	{
		QString codec = list.at(i);
		__DEBUG1( codec );
    	}
	
	__DEBUG1("gpspadaptation: list of supported sampletypes: ");
	 QList<QAudioFormat::SampleType> sampleTypez = defaultdevice.supportedSampleTypes();
	 for (int i = 0; i < sampleTypez.size(); ++i) 
		 {
		 switch(sampleTypez.at(i)) 
			 {
			case QAudioFormat::SignedInt:
			__DEBUG1("SignedInt");
			break;
		 case QAudioFormat::UnSignedInt:
			 __DEBUG1("UnSignedInt");
			 break;
		 case QAudioFormat::Float:
			 __DEBUG1("Float");
			 break;
		 case QAudioFormat::Unknown:
			 __DEBUG1("Unknown");
			 }
		 }
			 
	__DEBUG1("gpspadaptation: list of supported endianess: "); 
	 QList<QAudioFormat::Endian> endianz = defaultdevice.supportedByteOrders();
	 for (int i = 0; i < endianz.size(); ++i) 
		 {
		 switch (endianz.at(i)) 
			 {
			 case QAudioFormat::LittleEndian:
				 __DEBUG1("Little Endian");
				 break;
			 case QAudioFormat::BigEndian:
				 __DEBUG1("Big Endian");
				 break;
			 }
		 }
	 
	 __DEBUG1("gpspadaptation: list of supported frequenzies: "); 
	 QList<int> fequenz = defaultdevice.supportedFrequencies();
	 for (int i = 0; i < fequenz.size(); ++i) 
		 {
		 int freq = fequenz.at(i);
		 __DEBUG2("supported fequence;", freq );
		 }
	 
	 __DEBUG1("gpspadaptation: list of supported channels: "); 
	 QList<int> chanz = defaultdevice.supportedChannels();
	 for (int i = 0; i < chanz.size(); ++i) 
		 {
		 int chan = chanz.at(i);
		 __DEBUG2("supported channels;", chan );
		 }

	 __DEBUG1("gpspadaptation: list of supported samplesizes: "); 
	 QList<int> sampz = defaultdevice.supportedSampleSizes();
	 for (int i = 0; i < sampz.size(); ++i) 
		 {
		 int samp = sampz.at(i);
		 __DEBUG2("supported samplesize;", samp );
		 }	    
			 
	}

#endif
