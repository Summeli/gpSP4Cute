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

#ifndef VIEWCONTROLLER_H_
#define VIEWCONTROLLER_H_

#include <QtGui/QMainWindow>
#include <QObject>
#include <QString>

//#include "settingsview.h"
#include "gpsp4qt.h"
#include "emusettings.h"
#include "QRemoteControlKeys.h"

class ViewController : public QObject
{
    Q_OBJECT
public:
    ViewController();
    ~ViewController();

public slots:
    void loadROM( QString rom, TGPSPSettings antSettings );
    void showSettings();
    void LoadState( int state );
    void SaveState( int state );
    void reset();
    void continueGame();
    
private:
    gpsp4Qt* emuView;
    EmuSettings* settingsView;
    QRemoteControlKeys* remotekeys;
    
#ifdef Q_OS_SYMBIAN
    qint32 iMenuKeyHandle;
    qint32 iNoKeyHandle;
    qint32 iNoKeyHandle2;
    qint32 iMenuKeyHandle2;
    qint32 iCameraKeyHandle;
#endif
    
    
};
#endif /* VIEWCONTROLLER_H_ */
