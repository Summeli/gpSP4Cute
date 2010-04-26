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

#ifndef KEYCONFIGDIALOG_H
#define KEYCONFIGDIALOG_H

#include <QtGui/QWidget>
#include <QKeyEvent>

#include "ui_keyconfigdialog.h"

class keyconfigdialog : public QWidget
{
    Q_OBJECT

public:
    keyconfigdialog(QWidget *parent = 0);
    ~keyconfigdialog();
    void keyPressEvent( QKeyEvent * event);
    
    quint32 getKeyBind( int key );
public slots:
    void cancelClicked();

    signals:
    void configDone();
    
private:
    void updateText();
    
private:
    Ui::keyconfigdialogClass ui;
    int round;
    quint32 scanKeyTable[10];
};

#endif // KEYCONFIGDIALOG_H
