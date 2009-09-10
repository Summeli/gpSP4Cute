/* gpSP4Symbian
 *
 * Copyright (C) 2009 Summeli <summeli@summeli.fi>
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

#ifndef __ANT_AUDIO_H
#define __ANT_AUDIO_H

#include <Mda\Common\Audio.h>
#include <MdaAudioOutputStream.h>
#include <e32base.h>

const TInt KSoundBuffers = 4;
const TInt KBlockTime = 1000000 / 5; // hardcoded: 5 updates/sec
const TInt KMaxLag = 260000; // max sound lag, lower values increase chanse of underflow
const TInt KMaxUnderflows = 50; // max underflows/API errors we are going allow in a row (to prevent lockups)

class MAudioListener;

class CAntAudio : public CBase,
					 public MMdaAudioOutputStreamCallback
{
public:
	void Reset();
	TUint8* NextFrameL();
	void FrameMixed();
	TInt FreeBufferCount();
	void Stop();
	void SetVolume( TInt aVolume );
public:
	~CAntAudio();
	CAntAudio(TInt aRate, TBool aStereo, TInt aPcmFrames, TInt aVolume );
	void ConstructL();
	static CAntAudio* NewL(TInt aRate, TBool aStereo, TInt aPcmFrames, TInt aVolume );
public:
	void MaoscOpenComplete(TInt aError);
	void MaoscBufferCopied(TInt aError, const TDesC8& );
	void MaoscPlayComplete(TInt aError);

protected:

	TInt					iRate;
	TBool					iStereo;

	CMdaAudioOutputStream	*iMdaAudioOutputStream;
	TMdaAudioDataSettings	iMdaAudioDataSettings;

	HBufC8*					iSoundBuffers[KSoundBuffers+1];
	TInt					iBufferedFrames;
	TUint8*					iCurrentPosition;
	TInt					iPcmFrames;
	CActiveSchedulerWait* 	iWait;

	RArray<TInt> 			iEmptyBufQ;
	TInt					iFrameCount;
	TInt					iVolume;
};
#endif //__ANT_AUDIO_H
