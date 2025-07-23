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

#include "actiontools/actiondefinition.hpp"
#include "openurlinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class OpenURLDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit OpenURLDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &url = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("url"), tr("URL")});
            url.setTooltip(tr("The url to open"));

			addException(OpenURLInstance::FailedToOpenURL, tr("Failed to open URL"));
		}

		QString name() const override													{ return QObject::tr("Open URL"); }
		QString id() const override														{ return QStringLiteral("ActionOpenURL"); }
		QString description() const override												{ return QObject::tr("Opens an URL"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new OpenURLInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/openurl.png")); }

	private:
		Q_DISABLE_COPY(OpenURLDefinition)
	};
}

