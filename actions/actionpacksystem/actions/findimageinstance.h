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

#ifndef FINDIMAGEINSTANCE_H
#define FINDIMAGEINSTANCE_H

#include "actioninstance.h"
#include "matchingpointlist.h"
#include "windowhandle.h"

namespace ActionTools
{
	class OpenCVAlgorithms;
}

namespace Actions
{
	class FindImageInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Source)

	public:
		enum Source
		{
			ScreenshotSource,
			WindowSource,
			ImageSource
		};
		enum Exceptions
		{
			ErrorWhileSearchingException = ActionTools::ActionException::UserException,
			CannotFindTheImageException
		};

		FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
		~FindImageInstance();

		static ActionTools::StringListPair sources;

		void startExecution();

	private slots:
		void searchFinished(const ActionTools::MatchingPointList &matchingPointList);

	private:
		void validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum = INT_MAX);

		ActionTools::OpenCVAlgorithms *mOpenCVAlgorithms;
		QString mPositionVariableName;
		bool mWindowRelativePosition;
		ActionTools::WindowHandle mWindow;
		Source mSource;
		int mMaximumMatches;

		Q_DISABLE_COPY(FindImageInstance)
	};
}

#endif // FINDIMAGEINSTANCE_H