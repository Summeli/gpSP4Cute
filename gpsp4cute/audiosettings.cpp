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


#include "audiosettings.h"

AudioSettings::AudioSettings(int audioOn, int samplerate, int stereoOn, int volume, 
							QWidget *parent )
    : QWidget(parent)
{
	ui.setupUi(this);
	int sampeIndex = 0;
	if( samplerate == 8000 )
		sampeIndex = 0;
	else if( samplerate == 11025 )
		sampeIndex = 1;
	else if( samplerate == 16000 )
		sampeIndex = 2;
	else if( samplerate == 22050 )
		sampeIndex = 3;
	else if( samplerate == 44100 )	
		sampeIndex = 4;
	ui.AudioOnOffBox->setCurrentIndex( audioOn );
	ui.sampleRateBox->setCurrentIndex( sampeIndex );
	ui.stereoBox->setCurrentIndex( audioOn );
	ui.volumeSlider->setValue( volume );
	
	connect(ui.AudioOnOffBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAudioOn(int)));
	connect(ui.sampleRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSampleRate(int)));
	connect(ui.stereoBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStereoOn(int)));
	connect(ui.volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVolume(int)));
	
}

AudioSettings::~AudioSettings()
{

}

void AudioSettings::setAudioOn( int audioOn )
{
	emit( AudioOn(audioOn) );
}


void AudioSettings::setSampleRate( int sampleRate )
{
	int sample;
	if( sampleRate == 0 )
		sample = 8000;
	else if( sampleRate == 1 )
		sample = 11025;
	else if( sampleRate == 2 )
		sample = 16000;
	else if( sampleRate == 3 )
		sample = 22050;
	else if( sampleRate == 4 )	
		sample = 44100;
	
	emit( SampleRate(sample) );
}

void AudioSettings::setStereoOn( int stereoOn )
	{
	emit( enableStereo(stereoOn) );
	}

void AudioSettings::setVolume( int volume )
	{
	emit( Volume(volume) );
	}

