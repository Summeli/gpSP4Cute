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

#include "gpsp4Qt.h"
#include "gpspadaptation.h"

#include "cuteDebug.h"
#include "symbian_memory_handler.h"  
#include "input.h"
#include "gpspSettings.h"
#include "buttonpositions.h"

uint KAntKeyTable[10]={BUTTON_UP,BUTTON_DOWN,BUTTON_LEFT,BUTTON_RIGHT,BUTTON_A,
		BUTTON_B,BUTTON_L,BUTTON_R,BUTTON_START,BUTTON_SELECT};

MEmulatorAdaptation* g_adaption;

//gpsp general includes for the blit
#include "common.h"

extern u16* g_screenptr;

gpsp4Qt::gpsp4Qt(QWidget *parent)
    : QGLWidget(parent), m_buf(NULL), m_softKeys(0), m_hardKeys(0), m_errorDialog(NULL)
{
    QWidget::setAttribute(Qt::WA_AcceptTouchEvents);
    QThread::currentThread()->setPriority( QThread::NormalPriority );
    showFullScreen();
    setFocusPolicy(Qt::StrongFocus);
	
    //create button widgets
   
    m_dpad = new DPadWidget( this );
    connect(m_dpad, SIGNAL(showMenu()), this, SLOT( showAntSnesMenu()) );
    
    m_rightButtons = new rightbuttonwidget( this );

    m_audio = new CAntAudio();
    
    m_adaptation = new gpspadaptation( this, m_audio );
    
    //connect all adaptation stuff
    connect(this, SIGNAL(Start()), m_adaptation, SLOT(Start()) );
    connect(this, SIGNAL(Stop()), m_adaptation, SLOT(Stop()) );
     
    connect(this, SIGNAL(saveState(int)), m_adaptation, SLOT(savegpspState(int)) );
    connect(this, SIGNAL(loadState(int)), m_adaptation, SLOT(loadgpspState(int)) );
    connect(this, SIGNAL(resetgpsp()), m_adaptation, SLOT(ResetGame()) );
    connect(this, SIGNAL(exitgpsp()), m_adaptation, SLOT(exitgpsp()) );
    connect(this, SIGNAL(doLoadROM(QString,TGPSPSettings)), m_adaptation, SLOT(LoadRom(QString,TGPSPSettings)) );
    connect(m_adaptation, SIGNAL(dispatchErrorNote(QString)), this, SLOT(showErrorNote(QString)) );
    
    connect(this, SIGNAL(Start()), this, SLOT(listencontrols()) );
    
    g_adaption = this;
    //create graphics for the button overlay
    LoadButtons();
    
}

gpsp4Qt::~gpsp4Qt()
{
    delete m_dpad;
}

void gpsp4Qt::render()
{
    __DEBUG_IN
    __DEBUG2("Rendering.. QThread::currentThreadId():", QThread::currentThreadId());

    if (m_buf != NULL)
    {
        delete m_buf;
        m_buf = NULL;
    }

    __DEBUG1("creating buffer")
    m_buf = new QImage( (uchar*)g_screenptr, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_BYTES_PER_LINE, QImage::Format_RGB16);

    if ( m_buttonOpacity != m_settings.iButtonOpacity )
    {
        m_buttonOpacity = m_settings.iButtonOpacity;
        LoadButtons();
    }
    __DEBUG1("calling repaint")
    repaint();
    __DEBUG_OUT
}

void gpsp4Qt::paintEvent(QPaintEvent *)
{
    __DEBUG_IN
    __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());

    QPainter painter;
    painter.begin(this);


    if ( (m_buf == NULL) || (m_stretch != m_settings.iStretch) )
        m_stretch = m_settings.iStretch;
    {   QRect fullrect(0,0,width(), height() );
        painter.fillRect(fullrect, QColor(0x11, 0x11, 0x11 ));
    }

    if (m_buf != NULL)
    {
        __DEBUG1("Creating QRectF");
        int newWidth = width();
        int left = 0;
        if(m_stretch == TGPSPSettings::EMedium ){
            // 536 x 360 res
            left = 52;
            newWidth = 533;
        }
        QRect target(left, SCREEN_TOP, newWidth, height() );

        __DEBUG1("Drawing image");
        painter.drawImage(target, *m_buf);
        __DEBUG1("Ending QPainter");
    }

    //Draw pad
    painter.drawPixmap(tl_point, m_tlGraphics);
    painter.drawPixmap( dpad_point, m_dpadGraphics);
    painter.drawPixmap(tr_point, m_trGraphics);
    painter.drawPixmap( buttons_point, m_buttonsGraphics);
    painter.drawPixmap( menu_point, m_menuGraphics);
    painter.drawPixmap( start_select_point, m_startSelectGraphics);

    if( m_settings.iShowFPS )
        {
        painter.setPen ( Qt::green );
        //TODO
       // painter.drawText ( showFPS_point, QString::number(g_fps) );
        }

    painter.end();
    __DEBUG_OUT
}


bool gpsp4Qt::event(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        {
            QList<QTouchEvent::TouchPoint> touchPoints = (static_cast<QTouchEvent*>(event))->touchPoints();
            m_softKeys = 0;
            for ( int i = 0; i < touchPoints.length(); i++ )
            {
                QTouchEvent::TouchPoint tp = touchPoints[i];
                if ( tp.state() == Qt::TouchPointPressed || tp.state() == Qt::TouchPointMoved || tp.state() == Qt::TouchPointStationary)
                {
                    if ( tp.screenPos().x() < DPAD_WIDTH )
                    {
                        m_softKeys |= m_dpad->getGpspKeys(tp.screenPos().x(), tp.screenPos().y());
                    }
                    else if ( tp.screenPos().x() > BUTTON_LEFT_POS )
                    {
                        m_softKeys |= m_rightButtons->getGpspKeys(tp.screenPos().x() - BUTTON_LEFT_POS, tp.screenPos().y());
                    }
                }
            }

            event->accept();
            return true;
        }
        case QEvent::TouchEnd:
        {
            m_softKeys = 0;
            break;
        }
        case QEvent::FocusOut:
        {
            showmenu();
        }
        default:
            break;
    }
    return QWidget::event(event);
}

void gpsp4Qt::setRemoteControl( QRemoteControlKeys* remote )
{
    m_remotecontrol = remote;
}

void gpsp4Qt::keyPressEvent( QKeyEvent * event)
{
    __DEBUG_IN
    __DEBUG2("Key Pressed: ", event->text());

    quint32 c = event->nativeScanCode();

    __DEBUG2("key pressed, scancode is ", c );
    for(TInt i=0;i<10;i++)
    {
    if(c==m_settings.iScanKeyTable[i])
        {
        m_hardKeys |= KAntKeyTable[i];
        }
    }

}

void gpsp4Qt::keyReleaseEvent(QKeyEvent* event)
{
    __DEBUG_IN
    quint32 c = event->nativeScanCode();

    __DEBUG2("key released, scancode is ", c );
    for(TInt i=0;i<10;i++)
        {
        if(c==m_settings.iScanKeyTable[i])
            {
            m_hardKeys &= ~KAntKeyTable[i];
            }
        }
    __DEBUG_OUT
    }

void gpsp4Qt::LoadROM(QString rom, TGPSPSettings antSettings )
{
    __DEBUG_IN
    m_currentROM = rom;
    updateSettings( antSettings );
    emit( doLoadROM( rom, antSettings ));
    emit(Start());
    __DEBUG_OUT
}

void gpsp4Qt::showAntSnesMenu()
{
    __DEBUG_IN
    emit( Stop() );
    emit( showmenu() );
    __DEBUG_OUT
}

void gpsp4Qt::LoadState( int state )
{
    __DEBUG_IN
    emit( loadState( state ) );
    __DEBUG2("state loaded", state);
    emit(Start());
    __DEBUG_OUT
}

void gpsp4Qt::SaveState( int state )
{
    __DEBUG_IN
    emit ( saveState( state ) );
    __DEBUG2("state saved", state);
    emit(Start());
    __DEBUG_OUT
}

void gpsp4Qt::reset()
{
    __DEBUG_IN
    emit( resetgpsp() );
    emit( Start() );
    __DEBUG_OUT
}

void gpsp4Qt::continueGame()
{
    emit( Start() );
}

void gpsp4Qt::updateSettings( TGPSPSettings antSettings )
{
    __DEBUG_IN
    m_settings = antSettings;
    m_dpad->setDpadMode( antSettings.iDpadSettings );
    m_rightButtons->setButtonSettings( antSettings.iButtonSettings );
    m_adaptation->updateSettings( antSettings );
    __DEBUG_OUT
}

void gpsp4Qt::showErrorNote( QString message )
{
    __DEBUG_IN
    emit(Stop());
    m_errorDialog = new cuteErrorDialog( message, this );
    connect(m_errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
    m_errorDialog->show();
    m_errorDialog->setFocus();
    __DEBUG_OUT
}

void gpsp4Qt::errorNoteDismissed()
{
    disconnect(m_errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
    delete m_errorDialog;
    m_errorDialog = NULL;
    emit(Start());
}


quint32 gpsp4Qt::getGpspKeys()
{
    return m_softKeys | m_hardKeys;
}

void gpsp4Qt::listencontrols()
{
    m_remotecontrol->subscribeKeyEvent(this);
}

void gpsp4Qt::LoadButtons()
{
    ApplyTransparency(m_dpadGraphics, ":/gfx/dpad.png");
    ApplyTransparency(m_buttonsGraphics, ":/gfx/BAButtons.png");
    ApplyTransparency(m_tlGraphics, ":/gfx/tl_button_top.png");
    ApplyTransparency(m_trGraphics, ":/gfx/tr_button_top.png");
    ApplyTransparency(m_startSelectGraphics, ":/gfx/select_start_buttons.png");
    ApplyTransparency(m_menuGraphics, ":/gfx/menu.png");

}

void gpsp4Qt::ApplyTransparency(QPixmap &pm, QString png)
{
    pm.load(png);
    QPixmap transparent(pm.size());
    transparent.fill(Qt::transparent);
    QPainter p(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, pm);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(transparent.rect(), QColor(0, 0, 0, (m_settings.iButtonOpacity * 255) / 4));
    p.end();
    pm = transparent;
}
