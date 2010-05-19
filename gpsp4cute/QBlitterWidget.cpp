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


#include "QBlitterWidget.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QThread>

#include "bitmapblit.h"
#include "cuteDebug.h"

//Symbian DSA stuff
#include <eikenv.h>
#include <coecntrl.h>
#include <w32std.h>

void (*bitmapBlit)(TUint8* aScreen, TUint8* aBitmap) = 0;

#include "common.h"
extern u16* g_screenptr;

QBlitterWidget::QBlitterWidget( QWidget *parent )
: QWidget( parent ), CActive( CActive::EPriorityStandard )
    {
	__DEBUG_IN

    iDSA = NULL;
    iDSBitmap = NULL;
    
    CActiveScheduler::Add( this );
    
    //no choises in here :-)
    screenmode = 0;
    keepratio = true;
    
    logo = new smallgpsplogo( parent );
    logo->setGeometry(QRect(160, 320, 640, 360));
    
    __DEBUG_OUT
    }

QBlitterWidget::~QBlitterWidget()
    {
	
    Cancel();
    delete iDSA;
    delete iDSBitmap;
    
    }

void QBlitterWidget::render()
    {
	__DEBUG_IN
    __DEBUG2("Rendering.. QThread::currentThreadId():", QThread::currentThreadId());
	
	if( !iDSA )
		{
		__DEBUG1("NO DSA, returning");
		return;
		}
    if (IsActive())
    	{
		__DEBUG1("DSA Still active, cancel");
    	Cancel();
    	}
    
    TAcceleratedBitmapInfo bitmapInfo;
    iDSBitmap->BeginUpdate(bitmapInfo);        
    
    bitmapdata = (TUint8*) bitmapInfo.iAddress;
    bitmapBlit( (TUint8*) g_screenptr, bitmapdata);
    
    iDSBitmap->EndUpdate(iStatus);
    SetActive();
    
    __DEBUG_OUT
    }
/*
 * We have to make a choise in here. we can allocate the memory in advance in the constructor
 * or we can create a new qimage each time when we got a paint event. Using pre-allocated
 * memory should be faster.
*/
 void QBlitterWidget::paintEvent(QPaintEvent *)
     {
     __DEBUG_IN
     __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());
     // QImage( uchar * data, int width, int height, Format format )
     //uchar* g_data = NULL; //temp
     
     QImage im( "C:\\data\\mytest.jpg", "JPG");

     QRectF target(0.0, 0.0, 256.0, 234.0);
     QRectF source(0.0, 0.0, 256.0, 234.0);

     QPainter painter(this);
     painter.drawImage(target, im, source);
     __DEBUG_OUT
     }

void QBlitterWidget::stopDSA()
	{
	__DEBUG_IN
	if (iDSA)
		{
		iDSA->Cancel();
		delete iDSA;
		iDSA = NULL;
		iDSBitmap->Close();
		}
	__DEBUG_OUT
	}
 
void QBlitterWidget::startDSA()
	{
	__DEBUG_IN
	if(!iDSA)
		{
	    iDSBitmap = CDirectScreenBitmap::NewL();
	    iDSA = CDirectScreenAccess::NewL(CEikonEnv::Static()->WsSession(),
	  		*CEikonEnv::Static()->ScreenDevice(),
	    		*winId()->DrawableWindow(),
	    		*this);
	    CEikonEnv::Static()->WsSession().Flush();
	    iDSA->StartL();
	    CFbsBitGc *gc = iDSA->Gc();
	    RRegion *region = iDSA->DrawingRegion();
	    
	    gc->SetClippingRegion(region);
	    createScreenBuffer();
	    
	    }
	__DEBUG_OUT
	}

void QBlitterWidget::setScreenMode( int mode, bool keepaspectratio )
	{
	screenmode = mode;
	keepratio = keepaspectratio;
	}

void QBlitterWidget::Restart(RDirectScreenAccess::TTerminationReasons aReason)
	{
	__DEBUG_IN
    TRAPD( err, iDSA->StartL() ); // You may panic here, if you want
    CFbsBitGc* gc = iDSA->Gc();
    RRegion* region = iDSA->DrawingRegion();
    gc->SetClippingRegion(region);
 
    createScreenBuffer();
    __DEBUG_OUT
	}

void QBlitterWidget::AbortNow(RDirectScreenAccess::TTerminationReasons aReason)
	{
	iDSBitmap->Close();
	}

    // from CActive 
void QBlitterWidget::DoCancel()
	{
	
	}

void QBlitterWidget::RunL()
	{
	
	}

void QBlitterWidget::createScreenBuffer( )
	{
	if( keepratio )
		{
		switch( screenmode )
			{
			case 0:
				iDSBitmap->Create(
						TRect(160,0,640,320), CDirectScreenBitmap::EDoubleBuffer); 
				bitmapBlit = Blitkeepratio;
				logo->setGeometry(QRect(160, 320, 640, 360));
				break;
			case 1:
				 iDSBitmap->Create(
									TRect(80,0,560,320), CDirectScreenBitmap::EDoubleBuffer); 
				bitmapBlit = Blitkeepratio;
				logo->setGeometry(QRect(80, 320, 560, 360));
				break;
			case 2:
				iDSBitmap->Create(
								TRect(0,0,640,360), CDirectScreenBitmap::EDoubleBuffer); 
				 bitmapBlit = BlitWidgetFullScreen;
				break;
			default:
				break;
			}
		logo->show();
		}
	else
		{
		logo->hide();
		switch( screenmode )
			{
			case 0:
				iDSBitmap->Create(
						TRect(160,0,640,360), CDirectScreenBitmap::EDoubleBuffer); 
				bitmapBlit = BlitWidgetDSA;
				break;
			case 1:
				 iDSBitmap->Create(
									TRect(80,0,560,360), CDirectScreenBitmap::EDoubleBuffer); 
				bitmapBlit = BlitWidgetDSA;
				break;
			case 2:
				iDSBitmap->Create(
								TRect(0,0,640,360), CDirectScreenBitmap::EDoubleBuffer); 
				 bitmapBlit = BlitWidgetFullScreen;
				break;
			default:
				break;
			}
		}
	}

void QBlitterWidget::setLogo()
	{
	if( keepratio )
		{
		switch( screenmode )
			{
			case 0:
				logo->setGeometry(QRect(160, 320, 640, 360));
				break;
			case 1:
				logo->setGeometry(QRect(80, 320, 560, 360));
				break;
			case 2:
				break;
			default:
				break;
			}
		logo->show();
		}
	else
		{
		logo->hide();
		}	
	}

