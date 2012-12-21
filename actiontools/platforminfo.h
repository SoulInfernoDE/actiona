/*
    Actionaz
    Copyright (C) 2008-2012 Jonathan Mercier-Ganady

    Actionaz is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actionaz is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#ifndef PLATFORMINFO_H
#define PLATFORMINFO_H

#include "actiontools_global.h"

#include <QWidget>

#ifdef Q_OS_UNIX
typedef struct _XDisplay Display;
typedef unsigned long XID;
typedef XID Cursor;
#endif

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT PlatformInfo
    {
    public:
        PlatformInfo();
#ifdef Q_OS_UNIX
        static Display *display();
        static WId appRootWindow();
        static Cursor cursorNativeHandle(const QCursor &cursor);
#endif
    };
}

#endif // PLATFORMINFO_H
