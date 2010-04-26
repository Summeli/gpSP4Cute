/*
 * QRemoteControlKeys.cpp
 *
 *  Created on: Jan 18, 2010
 *      Author: Administrator
 */

#include <QKeyEvent>
#include <QApplication>
#include "QRemoteControlKeys.h"

#include "cuteDebug.h"

#define KTimeOut 80

#define KPlayButtonPress 0x101
#define KStopButtonPress 0x102
#define KForwardButtonPress 0x103
#define KBackwardButtonPress 0x104
#define KVolUpButtonPress 0x105
#define KVolDownButtonPress 0x106

QRemoteControlKeys::QRemoteControlKeys()
	: CActive ( EPriorityNormal )
	{
	CActiveScheduler::Add( this );
    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    iRemConCore = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
    iInterfaceSelector->OpenTargetL(); 
    
   playtimer = new QTimer(this);
   connect(playtimer, SIGNAL(timeout()), this, SLOT(playtimerexpired()));
   stoptimer = new QTimer(this);
   connect(stoptimer, SIGNAL(timeout()), this, SLOT(stoptimerexpired()));
   forwardtimer = new QTimer(this);
   connect(forwardtimer, SIGNAL(timeout()), this, SLOT(forwardtimerexpired()));
   backwardtimer = new QTimer(this);
   connect(backwardtimer, SIGNAL(timeout()), this, SLOT(backwardtimerexpired()));
   voluptimer = new QTimer(this);
   connect(voluptimer, SIGNAL(timeout()), this, SLOT(voluptimerexpired()));
   voldowntimer = new QTimer(this);
   connect(voldowntimer, SIGNAL(timeout()), this, SLOT(voldowntimerexpired()));
	}

QRemoteControlKeys::~QRemoteControlKeys()
	{
	// TODO Auto-generated destructor stub
    delete iInterfaceSelector;
    iRemConCore = NULL; //owned by interfaceselector
	Cancel();
	iResponseQ.Reset();
	iResponseQ.Close();
	}

void QRemoteControlKeys::subscribeKeyEvent(QObject* aObject )
	{
	receiver = aObject;
	}

/*
 * it seems that it takes about 600ms to get an update after buttonpress
 * */
void QRemoteControlKeys::MrccatoCommand(TRemConCoreApiOperationId aOperationId, 
	TRemConCoreApiButtonAction aButtonAct)
	{
	 QKeyEvent *event = NULL;
	TRequestStatus status;
       switch( aOperationId )
       {
       case ERemConCoreApiPausePlayFunction:
           {
           switch (aButtonAct)
               {
               case ERemConCoreApiButtonPress:
                   // Play/Pause button pressed 
            	   __DEBUG1("ERemConCoreApiPausePlayFunction, ERemConCoreApiButtonPress");
            	    event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KPlayButtonPress, Qt::NoModifier,
															KPlayButtonPress, KPlayButtonPress,Qt::NoModifier);
                   break;
               case ERemConCoreApiButtonRelease:
            	   __DEBUG1("ERemConCoreApiPausePlayFunction, ERemConCoreApiButtonRelease");
              	    event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KPlayButtonPress, Qt::NoModifier,
           															KPlayButtonPress, KPlayButtonPress,Qt::NoModifier);
            	   // Play/Pause button released
                   break;
               case ERemConCoreApiButtonClick:
            	   __DEBUG1("ERemConCoreApiPausePlayFunction, ERemConCoreApiButtonClick");
                   // Play/Pause button clicked
            	   playtimer->start(KTimeOut);
            	   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KPlayButtonPress, Qt::NoModifier,
            	 															KPlayButtonPress, KPlayButtonPress,Qt::NoModifier);
                   break;
               default:
                   // Play/Pause unknown action
                   break;
               }                               
           break;
           }   
         
       case ERemConCoreApiStop:
           {
           switch (aButtonAct)
               {
        	   case ERemConCoreApiButtonPress:
        		   __DEBUG1("ERemConCoreApiStop, ERemConCoreApiButtonPress");
        		   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KStopButtonPress, Qt::NoModifier,
        				   KStopButtonPress, KStopButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonRelease:
            	   __DEBUG1("ERemConCoreApiStop, ERemConCoreApiButtonRelease");
            	   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KStopButtonPress, Qt::NoModifier,
            	           				   KStopButtonPress, KStopButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonClick:
            	   __DEBUG1("ERemConCoreApiStop, ERemConCoreApiButtonClick");
            	   stoptimer->start(KTimeOut);
            	   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KStopButtonPress, Qt::NoModifier,
            	           				   KStopButtonPress, KStopButtonPress,Qt::NoModifier);
                   break;   
               default:
                     
                break; 
               }
           break;
           }
       case ERemConCoreApiRewind:
           {
           switch (aButtonAct)
               {
        	   case ERemConCoreApiButtonPress:
        		   __DEBUG1("ERemConCoreApiRewind, ERemConCoreApiButtonPress");      
        		   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KBackwardButtonPress, Qt::NoModifier,
        		              			 KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonRelease:
            	   __DEBUG1("ERemConCoreApiRewind, ERemConCoreApiButtonRelease");  
         		   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KBackwardButtonPress, Qt::NoModifier,
                		              			 KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonClick:
            	   __DEBUG1("ERemConCoreApiRewind, ERemConCoreApiButtonClick");  
            	   backwardtimer->start(KTimeOut);
         		   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KBackwardButtonPress, Qt::NoModifier,
                		              			 KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
               default:
                break; 
               }
           break;
           }    
       case ERemConCoreApiFastForward:
           {
           switch (aButtonAct)
               {
        	   case ERemConCoreApiButtonPress:
        		   __DEBUG1("ERemConCoreApiFastForward, ERemConCoreApiButtonPress");   
        		   //event = new QKeyEvent ( QEvent::KeyPress, KForwardButtonPress,  Qt::NoModifier);
        		   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KForwardButtonPress, Qt::NoModifier,
        				   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonRelease:
            	   __DEBUG1("ERemConCoreApiFastForward, ERemConCoreApiButtonRelease");  
            	   //event = new QKeyEvent ( QEvent::KeyRelease, KForwardButtonPress,  Qt::NoModifier);
            	   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KForwardButtonPress, Qt::NoModifier,
            	        				   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
                  break;
               case ERemConCoreApiButtonClick:
            	   __DEBUG1("ERemConCoreApiFastForward, ERemConCoreApiButtonClick"); 
            	   forwardtimer->start(KTimeOut);
            	  // event = new QKeyEvent ( QEvent::KeyPress, KForwardButtonPress,  Qt::NoModifier);
            	   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KForwardButtonPress, Qt::NoModifier,
            	        				   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
               default:      
                break; 
               }
           break;
           }       
       case ERemConCoreApiVolumeUp:
           {   
           switch (aButtonAct)
               {
           	   case ERemConCoreApiButtonPress:
				   __DEBUG1("ERemConCoreApiVolumeUp, ERemConCoreApiButtonPress");   
				   //event = new QKeyEvent ( QEvent::KeyPress, KVolUpButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KVolUpButtonPress, Qt::NoModifier,
						   KVolUpButtonPress, KVolUpButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonRelease:
				   __DEBUG1("ERemConCoreApiVolumeUp, ERemConCoreApiButtonRelease");  
				   //event = new QKeyEvent ( QEvent::KeyRelease, KVolUpButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KVolUpButtonPress, Qt::NoModifier,
				 						   KVolUpButtonPress, KVolUpButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonClick:
				   __DEBUG1("ERemConCoreApiVolumeUp, ERemConCoreApiButtonClick"); 
				   voluptimer->start(KTimeOut);
				   //event = new QKeyEvent ( QEvent::KeyPress, KVolUpButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KVolUpButtonPress, Qt::NoModifier,
				 						   KVolUpButtonPress, KVolUpButtonPress,Qt::NoModifier);
			   default:      
				break; 
               }
           break;
           }       
       case ERemConCoreApiVolumeDown:
           {
           switch (aButtonAct)
               {
          	   case ERemConCoreApiButtonPress:
				   __DEBUG1("ERemConCoreApiVolumeDown, ERemConCoreApiButtonPress");   
				   //event = new QKeyEvent ( QEvent::KeyPress, KVolDownButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KVolDownButtonPress, Qt::NoModifier,
						   KVolDownButtonPress, KVolDownButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonRelease:
				   __DEBUG1("ERemConCoreApiVolumeDown, ERemConCoreApiButtonRelease");  
				   //event = new QKeyEvent ( QEvent::KeyRelease, KVolDownButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KVolDownButtonPress, Qt::NoModifier,
									   KVolDownButtonPress, KVolDownButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonClick:
				   __DEBUG1("ERemConCoreApiVolumeDown, ERemConCoreApiButtonClick"); 
				   voldowntimer->start(KTimeOut);
				  // event = new QKeyEvent ( QEvent::KeyPress, KVolDownButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KVolDownButtonPress, Qt::NoModifier,
									   KVolDownButtonPress, KVolDownButtonPress,Qt::NoModifier);
			   default:      
				break; 
               }
           break;
           }  
       case ERemConCoreApiBackward:
           {
           switch (aButtonAct)
               {
          	   case ERemConCoreApiButtonPress:
				   __DEBUG1("ERemConCoreApiBackward, ERemConCoreApiButtonPress");   
				   //event = new QKeyEvent ( QEvent::KeyPress, KBackwardButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KBackwardButtonPress, Qt::NoModifier,
						   KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonRelease:
				   __DEBUG1("ERemConCoreApiBackward, ERemConCoreApiButtonRelease");  
				   //event = new QKeyEvent ( QEvent::KeyRelease, KBackwardButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KBackwardButtonPress, Qt::NoModifier,
						   KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonClick:
				   __DEBUG1("ERemConCoreApiBackward, ERemConCoreApiButtonClick"); 
				   backwardtimer->start(KTimeOut);
				   //event = new QKeyEvent ( QEvent::KeyPress, KBackwardButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KBackwardButtonPress, Qt::NoModifier,
						   KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
			   default:      
				break; 
               }
           break;
           }       
       case ERemConCoreApiForward:
           {
           switch (aButtonAct)
               {
          	   case ERemConCoreApiButtonPress:
				   __DEBUG1("ERemConCoreApiForward, ERemConCoreApiButtonPress");   
				  // event = new QKeyEvent ( QEvent::KeyPress, KForwardButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KForwardButtonPress, Qt::NoModifier,
						   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonRelease:
				   __DEBUG1("ERemConCoreApiForward, ERemConCoreApiButtonRelease");  
				  // event = new QKeyEvent ( QEvent::KeyRelease, KForwardButtonPress,  Qt::NoModifier);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyRelease, KForwardButtonPress, Qt::NoModifier,
				  						   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
				  break;
			   case ERemConCoreApiButtonClick:
				   __DEBUG1("ERemConCoreApiForward, ERemConCoreApiButtonClick"); 
				   forwardtimer->start(KTimeOut);
				   event =  QKeyEvent::createExtendedKeyEvent( QEvent::KeyPress, KForwardButtonPress, Qt::NoModifier,
				  						   KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
				   
			   default:      
				break; 
               }
           break;
           }
       	
       default:
           break;
       }
       //complete key event
       QCoreApplication::postEvent (receiver, event);
       CompleteMediaKeyEvent( aOperationId );
	}

void QRemoteControlKeys::MrccatoPlay(TRemConCoreApiPlaybackSpeed aSpeed, 
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void QRemoteControlKeys::MrccatoTuneFunction(TBool aTwoPart, 
	TUint aMajorChannel, 
	TUint aMinorChannel,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void QRemoteControlKeys::MrccatoSelectDiskFunction(TUint aDisk,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void QRemoteControlKeys::MrccatoSelectAvInputFunction(TUint8 aAvInputSignalNumber,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void QRemoteControlKeys::MrccatoSelectAudioInputFunction(TUint8 aAudioInputSignalNumber,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void QRemoteControlKeys::CompleteMediaKeyEvent( TRemConCoreApiOperationId aOperationId )
	{
	if	( !IsActive() )
		{
		switch ( aOperationId )
			{
			case ERemConCoreApiVolumeUp:
				{
				iRemConCore->VolumeUpResponse( iStatus, KErrNone );
				SetActive();
				}
				break;
	
			case ERemConCoreApiVolumeDown:
				{
				iRemConCore->VolumeDownResponse( iStatus, KErrNone );	
				SetActive();
				}
				break;
			case ERemConCoreApiPlay:
				{
				iRemConCore-> PlayResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiStop:
				{
				iRemConCore->StopResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			
			case ERemConCoreApiPause:
				{
				iRemConCore->PauseResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiRewind:
				{
				iRemConCore->RewindResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiFastForward:
				{
				iRemConCore->FastForwardResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiForward:
				{
				iRemConCore->ForwardResponse( iStatus, KErrNone );
				SetActive();
				}
				break;
			case ERemConCoreApiBackward:
				{
				iRemConCore->BackwardResponse(iStatus, KErrNone );
				SetActive();
				}
				break;
			default:
				{
				}
				break;
			}
		}
	else
		{
		//active, append to queue
		iResponseQ.Append( aOperationId );
		}
	}

void QRemoteControlKeys::RunL()
	{
	 if ( iResponseQ.Count() )
		{
		CompleteMediaKeyEvent( iResponseQ[0] );
		//remove old response from que
		iResponseQ.Remove(0);
		iResponseQ.Compress();
		}
	}

void QRemoteControlKeys::DoCancel()
	{
	__DEBUG_IN
	}

void QRemoteControlKeys::playtimerexpired()
	{
	__DEBUG_IN
	playtimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KPlayButtonPress, Qt::NoModifier,
			   KPlayButtonPress, KPlayButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}

void QRemoteControlKeys::stoptimerexpired()
	{
	__DEBUG_IN
	stoptimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KStopButtonPress, Qt::NoModifier,
			KStopButtonPress, KStopButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}
void QRemoteControlKeys::forwardtimerexpired()
	{
	__DEBUG_IN
	forwardtimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KForwardButtonPress, Qt::NoModifier,
			KForwardButtonPress, KForwardButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}
void QRemoteControlKeys::backwardtimerexpired()
	{
	__DEBUG_IN
	backwardtimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KBackwardButtonPress, Qt::NoModifier,
			KBackwardButtonPress, KBackwardButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}
void QRemoteControlKeys::voluptimerexpired()
	{
	__DEBUG_IN
	voluptimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KVolUpButtonPress, Qt::NoModifier,
			KVolUpButtonPress, KVolUpButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}

void QRemoteControlKeys::voldowntimerexpired()
	{
	__DEBUG_IN
	voldowntimer->stop();
	QKeyEvent* event =  QKeyEvent::createExtendedKeyEvent(QEvent::KeyRelease, KVolDownButtonPress, Qt::NoModifier,
			KVolDownButtonPress, KVolDownButtonPress,Qt::NoModifier);
	QCoreApplication::postEvent (receiver, event);
	}
