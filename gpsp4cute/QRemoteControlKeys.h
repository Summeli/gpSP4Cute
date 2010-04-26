/*
 * QRemoteControlKeys.h
 *
 *  Created on: Jan 18, 2010
 *      Author: Administrator
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
