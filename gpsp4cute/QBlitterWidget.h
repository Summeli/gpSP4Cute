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

#ifndef QBLITTERWIDGET_H_
#define QBLITTERWIDGET_H_

#include <QtGui/QWidget>

#include <w32std.h>
#include <e32base.h>
#include <cdsb.h>

class QBlitterWidget : public QWidget, public CActive, public MDirectScreenAccess
{
Q_OBJECT

public:
    QBlitterWidget();
    ~QBlitterWidget();
    
public slots:
    void render();
    void stopDSA();
    void startDSA();
    void setScreenMode( int mode);
    
protected:
    void paintEvent(QPaintEvent *);
    void createScreenBuffer();
    
public:
    // from MDirectScreenAccess class  
    void Restart(RDirectScreenAccess::TTerminationReasons aReason); 
    void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);

public:
    // from CActive 
    virtual void DoCancel();
    virtual void RunL();

protected: //data
    QImage* buf;
    TUint8* bitmapdata;
    
    int screenmode;
    CDirectScreenAccess *iDSA;
    CDirectScreenBitmap *iDSBitmap;

};

#endif /* QBLITTERWIDGET_H_ */
