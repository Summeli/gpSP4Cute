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


#include "cuteDebug.h"
#include "viewcontroller.h"

#include <QtGui>
#include <QApplication>

#ifdef __SYMBIAN32__
#include "relocutils.h"
extern "C" void init_symbian_stuff();
#endif

void loadStyleSheet()
{
    QFile file(":/style/meegostyle.qss");
    if(!file.open(QFile::ReadOnly))
        {
		__DEBUG1("Unable to open file");
        }
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    file.close();
}

int main(int argc, char *argv[])
{
#ifdef __SYMBIAN32__
BEGIN_RELOCATED_CODE(0x10000000);
//init all the basic symbian stuff
init_symbian_stuff();
#endif
   QApplication a(argc, argv);
 
   loadStyleSheet();
    
   ViewController* vc = new ViewController();

   return a.exec();
#ifdef __SYMBIAN32__
END_RELOCATED_CODE(); // return to the original code section
#endif
}
