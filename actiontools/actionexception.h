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

#ifndef ACTIONEXCEPTION_H
#define ACTIONEXCEPTION_H

#include "actiontools_global.h"

#include <QMetaType>
#include <QString>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ActionException
	{
	public:
		enum Exception
		{
			BadParameterException,
			CodeErrorException,
			TimeoutException,

			ExceptionCount,
			UserException = 32
		};
		enum ExceptionAction
		{
			StopExecutionExceptionAction,
			SkipExceptionAction,
			GotoLineExceptionAction,

			ExceptionActionCount
		};

		class ExceptionActionInstance
		{
		public:
			ExceptionActionInstance()
				: mAction(StopExecutionExceptionAction)	{}
			ExceptionActionInstance(ExceptionAction action, const QString &line)
				: mAction(action), mLine(line)			{}

			void setAction(ExceptionAction action)		{ mAction = action; }
			void setLine(const QString &line)			{ mLine = line; }

			ExceptionAction action() const				{ return mAction; }
			QString line() const						{ return mLine; }
			
			bool operator ==(const ExceptionActionInstance &other) const
			{
				return (mAction == other.mAction && mLine == other.mLine);
			}

		private:
			ExceptionAction mAction;
			QString mLine;
		};

		ActionException(int id, const QString &name)
			: mId(id), mName(name)								{}

		int id() const											{ return mId; }
		QString name() const									{ return mName; }

		static QString ExceptionName[ExceptionCount];
		static ExceptionAction ExceptionDefaultAction[ExceptionCount];
		static QString ExceptionActionName[ExceptionActionCount];

	private:
		int mId;
		QString mName;
	};

	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionException::Exception &exception);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionException::ExceptionAction &exceptionAction);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionException::ExceptionActionInstance &exceptionActionInstance);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const ActionException::ExceptionActionInstance &exceptionActionInstance);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const ActionException::ExceptionActionInstance &exceptionActionInstance);
}

Q_DECLARE_METATYPE(ActionTools::ActionException::Exception)

#endif // ACTIONEXCEPTION_H
