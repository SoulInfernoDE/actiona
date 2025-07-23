/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#pragma once

#include <QWidget>
#include <QMetaType>

#include "actiontools_global.hpp"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT WindowHandle
	{
	public:
        WindowHandle()                                          = default;
		WindowHandle(WId handle) : mValue(handle)				{}

		bool isValid() const									{ return (mValue != 0); }
		void setInvalid()										{ mValue = 0; }
		WId value() const										{ return mValue; }

		operator bool() const									{ return isValid(); }
		bool operator ==(const WindowHandle &other) const		{ return (other.mValue == mValue); }
		bool operator !=(const WindowHandle &other) const		{ return (other.mValue != mValue); }

		QString title() const;
		QString classname() const;
		QRect rect(bool useBorders = true) const;
		int processId() const;
		bool close() const;
		bool killCreator() const;
		bool setForeground() const;
		bool minimize() const;
		bool maximize() const;
		bool move(QPoint position) const;
		bool resize(QSize size, bool useBorders = true) const;
		bool isActive() const;

		static WindowHandle foregroundWindow();
		static QList<WindowHandle> windowList();
        static QStringList windowTitles();
		static WindowHandle findWindow(const QString &title);
		static WindowHandle findWindow(const QRegularExpression &regExp);
		static QList<WindowHandle> findWindows(const QString &title);
		static QList<WindowHandle> findWindows(const QRegularExpression &regExp);

	private:
		WId mValue{0};
	};
}

Q_DECLARE_METATYPE(ActionTools::WindowHandle)

