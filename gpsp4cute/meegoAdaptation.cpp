/* AntSnes
 *
 * Copyright (C) 2011 Summeli <summeli@summeli.fi>
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

#include "meegoAdaptation.h"

#include <QApplication>
#include <QWidget>

#include "cutedebug.h"

#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

meegoAdaptation::meegoAdaptation(QObject *parent)
    : QObject(parent), m_SwipeEnabled( true ), m_timer(NULL)
{
}

meegoAdaptation::~meegoAdaptation()
{
}

void meegoAdaptation::enableSwipe()
{
    __DEBUG1("Enable swipe");

    doEnableSwipe();
}

void meegoAdaptation::disableSwipe()
{
    if( m_timer )
        return;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(doDisableSwipe()));
    m_timer->start(2000);
}

void meegoAdaptation::doEnableSwipe()
{
   //     setSwipeEnabled( true );
    if( m_SwipeEnabled )
        return;
    QWidget * activeWindow = QApplication::activeWindow();
    Display *dpy = QX11Info::display();
    Atom atom;

    atom = XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);
    if(XDeleteProperty(dpy, activeWindow->effectiveWinId(), atom) < 0){
      qWarning("XDeleteProperty for _MEEGOTOUCH_CUSTOM_REGION returns <0");
      }
    m_SwipeEnabled = true;
}

void meegoAdaptation::doDisableSwipe()
{
    if( m_timer ){
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(doDisableSwipe()));
        delete m_timer;
        m_timer = NULL;
    }
    if( !m_SwipeEnabled )
        return;

    QWidget * activeWindow = QApplication::activeWindow();
    Display *dpy = QX11Info::display();
    Atom atom;

    unsigned int customRegion[4];
    customRegion[0] = 0;
    customRegion[1] = 0;
    customRegion[2] = 854;
    customRegion[3] = 480;

    atom = XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);
    XChangeProperty(dpy, activeWindow->effectiveWinId(),
            atom, XA_CARDINAL, 32, PropModeReplace,
            reinterpret_cast<unsigned char *>(&customRegion[0]), 4);

    m_SwipeEnabled = false;

}
