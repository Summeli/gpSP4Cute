/* AntSnes
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

#include "meegoAudio.h"

#include "cutedebug.h"


/*******************************************
 *
 * CAntAudio
 *
 *******************************************/

CAntAudio::CAntAudio():
    m_buf(NULL), m_s(NULL),m_audioBufSize(0)
{
}


CAntAudio::~CAntAudio()
{
    if( m_s ){
        pa_simple_free(m_s);
        m_s = NULL;
    }
}

void CAntAudio::setVolume( int aVolume )
{
    __DEBUG1("set audio settings");
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16NE;
    ss.channels = 2;
    ss.rate = KSampleRate;

    if( m_s ){
        pa_simple_free(m_s);
        m_s = NULL;
    }

    m_s = pa_simple_new(NULL,               // Use the default server.
                   "antsnes",           // Our application's name.
                   PA_STREAM_PLAYBACK,
                   NULL,               // Use the default device.
                   "gameaudio",            // Description of our stream.
                   &ss,                // Our sample format.
                   NULL,               // Use default channel map
                   NULL,               // Use default buffering attributes.
                   NULL               // Ignore error code.
                   );

    if ( m_s == NULL )
        __DEBUG1("pulse audio creation failed!!");

    if( m_buf ){
        delete m_buf;
        m_buf = NULL;
    }

    m_audioBufSize = KFramesize;
    m_buf = new quint8[ m_audioBufSize ];
    __DEBUG1("set audio settings SET");
}

void CAntAudio::Reset()
{
}

// returns a pointer to buffer for next frame,
// to be used when iSoundBuffers are used directly
int *CAntAudio::NextFrameL()
{
    return (int*) m_buf;
}

void CAntAudio::FrameMixed()
{
    int error;
    if (pa_simple_write(m_s, m_buf, (size_t) m_audioBufSize, &error) < 0) {
      __DEBUG2("frame midex, audio error", error )
    }
}

void CAntAudio::Stop()
{
}

int CAntAudio::FreeBufferCount()
{
}
