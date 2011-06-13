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

#ifndef DEBUG_H_
#define DEBUG_H_

//#define _DEBUG
void initdebug();

#include <QDebug>
#ifdef _DEBUG
#define __DEBUG_IN        qDebug() << __PRETTY_FUNCTION__ << "in";
#define __DEBUG_OUT       qDebug() << __PRETTY_FUNCTION__ << "out";
#define __DEBUG1(a)        qDebug() << a;
#define __DEBUG2(a,b)      qDebug() << a << b;
#define __DEBUG3(a,b,c)    qDebug() << a << b << c;
#define __DEBUG4(a,b,c,d)    qDebug() << a << b << c << d;
#else
#define __DEBUG_IN
#define __DEBUG_OUT
#define __DEBUG1(a)
#define __DEBUG2(a,b)
#define __DEBUG3(a,b,c)
#define __DEBUG4(a,b,c,d) 
#endif

#endif /* DEBUG_H_ */
