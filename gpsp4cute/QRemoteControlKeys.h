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


#ifndef QREMOTECONTROLKEYS_H_
#define QREMOTECONTROLKEYS_H_

#include <QObject>
#include <QTimer>
#include <e32base.h>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib

class QRemoteControlKeys: public QObject, public CActive, public MRemConCoreApiTargetObserver
	{
	Q_OBJECT
public:
	QRemoteControlKeys();
	virtual ~QRemoteControlKeys();
	
public:
	void subscribeKeyEvent(QObject* aObject );
	
public: //From MRemConCoreApiTargetObserver
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId, 
		TRemConCoreApiButtonAction aButtonAct);
	
	void MrccatoPlay(TRemConCoreApiPlaybackSpeed aSpeed, 
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoTuneFunction(TBool aTwoPart, 
		TUint aMajorChannel, 
		TUint aMinorChannel,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectDiskFunction(TUint aDisk,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectAvInputFunction(TUint8 aAvInputSignalNumber,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectAudioInputFunction(TUint8 aAudioInputSignalNumber,
		TRemConCoreApiButtonAction aButtonAct);
    
private:
    void CompleteMediaKeyEvent( TRemConCoreApiOperationId aOperationId );
    void RunL();
    void DoCancel();
    
public slots:
	void playtimerexpired();
	void stoptimerexpired();
	void forwardtimerexpired();
	void backwardtimerexpired();
	void voluptimerexpired();
	void voldowntimerexpired();
private:

	RArray<TRemConCoreApiOperationId> iResponseQ; //response queue

	CRemConCoreApiTarget* iRemConCore; //the controller
    CRemConInterfaceSelector* iInterfaceSelector;
    
    QObject* receiver;
    
    QTimer* playtimer;
    QTimer* stoptimer;
    QTimer* forwardtimer;
    QTimer* backwardtimer;
    QTimer* voluptimer;
    QTimer* voldowntimer;
	};

#endif /* QREMOTECONTROLKEYS_H_ */
