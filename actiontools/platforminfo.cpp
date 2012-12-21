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

#include "platforminfo.h"

#ifdef Q_OS_UNIX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include <qpa/qplatformcursor.h>
#include <X11/Xlib.h>
#else
#include <QX11Info>
#endif
#endif

namespace ActionTools
{
    PlatformInfo::PlatformInfo()
    {
    }

#ifdef Q_OS_UNIX
    Display *PlatformInfo::display()
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return static_cast<Display *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("display", 0));
#else
        return QX11Info::display();
#endif
    }

    WId PlatformInfo::appRootWindow()
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return reinterpret_cast<WId>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("handle", 0));
#else
        return QX11Info::appRootWindow();
#endif
    }

    Cursor PlatformInfo::cursorNativeHandle(const QCursor &cursor)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return 0;//TODO
#else
        return cursor.handle();
#endif
    }
#endif
}
