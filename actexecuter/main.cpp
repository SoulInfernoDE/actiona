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

#include "globalshortcut/globalshortcutmanager.h"
#include "qxtcommandoptions/qxtcommandoptions.h"
#include "actioninstance.h"
#include "version.h"
#include "mainclass.h"
#include "nativeeventfilteringapplication.h"
#include "global.h"
#include "settings.h"

#include <ctime>

#include <QApplication>
#include <QtGlobal>
#include <QDir>
#include <QTextCodec>
#include <QFile>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>
#include <QUrl>
#include <QNetworkProxy>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QUrlQuery>
#endif

#ifdef Q_OS_UNIX
#undef signals
#include <libnotify/notify.h>
#define signals
#include "keysymhelper.h"
#endif

#ifdef Q_WS_WIN
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#endif

static void cleanup()
{
	ActionTools::GlobalShortcutManager::clear();
}

#ifdef Q_WS_WIN
static void pause()
{
	system("pause");
}

static void createConsole()
{
	AllocConsole();

	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stdout = *_fdopen(conHandle, "w");
	setvbuf(stdout, NULL, _IONBF, 0);

	stdHandle = GetStdHandle(STD_INPUT_HANDLE);
	conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stdin = *_fdopen(conHandle, "r");
	setvbuf(stdin, NULL, _IONBF, 0);

	stdHandle = GetStdHandle(STD_ERROR_HANDLE);
	conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stderr = *_fdopen(conHandle, "w");
	setvbuf(stderr, NULL, _IONBF, 0);
}

#endif

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x040700)
		#error("You need Qt 4.7.0 or later to compile Actionaz Executer");
#endif

	ActionTools::NativeEventFilteringApplication app("actionaz-exec", argc, argv);
	app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

#ifdef Q_OS_UNIX
	notify_init("Actionaz executer");
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

	const QStringList &arguments = QCoreApplication::arguments();

	//TODO: Fix the bug where the command help text is not translated
	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("code", QObject::tr("switch to code mode, may not be used with -s"));
	options.alias("code", "c");
	options.add("script", QObject::tr("switch to script mode, may not be used with -c"));
	options.alias("script", "s");
	options.add("nocodeqt", QObject::tr("do not include the Qt library into the code"));
	options.alias("nocodeqt", "Q");
	options.add("portable", QObject::tr("starts in portable mode, storing the settings in the executable folder"));
	options.alias("portable", "p");
	options.add("proxy-mode", QObject::tr("sets the proxy mode, values are \"none\", \"system\" (default) or \"custom\""));
	options.add("proxy-type", QObject::tr("sets the custom proxy type, values are \"http\" or \"socks\" (default)"));
	options.add("proxy-host", QObject::tr("sets the custom proxy host"));
	options.add("proxy-port", QObject::tr("sets the custom proxy port"));
	options.add("proxy-user", QObject::tr("sets the custom proxy user"));
	options.add("proxy-password", QObject::tr("sets the custom proxy password"));
#ifdef Q_WS_WIN
	options.add("console", QObject::tr("create a console to see debug output"));
	options.add("pause-at-end", QObject::tr("wait for user input at the end of the execution, used only with --console"));
#endif
	options.add("version", QObject::tr("show the program version"));
	options.alias("version", "v");
	options.add("help", QObject::tr("show this help text"));
	options.alias("help", "h");
	options.parse(arguments);

#ifdef Q_WS_WIN
	if(options.count("console"))
	{
		createConsole();

		if(options.count("pause-at-end"))
			qAddPostRoutine(pause);
	}
#endif

	if(options.count("portable"))
	{
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() + "/userSettings");
		QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath() + "/systemSettings");
		QSettings::setDefaultFormat(QSettings::IniFormat);
	}

	QSettings settings;

	QString locale = settings.value("locale", QLocale::system().name()).toString();

	QTranslator qtTranslator;
#ifdef Q_WS_WIN
	qtTranslator.load(QString("%1/locale/qt_%2").arg(QApplication::applicationDirPath()).arg(locale));
#else
	qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
	app.installTranslator(&qtTranslator);

	QTranslator toolsTranslator;
	if(!toolsTranslator.load(QString("%1/locale/tools_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		toolsTranslator.load(QString("%1/share/actionaz/locale/tools_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&toolsTranslator);

	QTranslator actionToolsTranslator;
	if(!actionToolsTranslator.load(QString("%1/locale/actiontools_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		actionToolsTranslator.load(QString("%1/share/actionaz/locale/actiontools_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&actionToolsTranslator);

	QTranslator executerTranslator;
	if(!executerTranslator.load(QString("%1/locale/executer_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		executerTranslator.load(QString("%1/share/actionaz/locale/executer_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&executerTranslator);

	QTranslator actexecuterTranslator;
	if(!actexecuterTranslator.load(QString("%1/locale/actexecuter_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		actexecuterTranslator.load(QString("%1/share/actionaz/locale/actexecuter_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&actexecuterTranslator);

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<Tools::Version>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Version");

	if(options.count("version"))
	{
		QTextStream stream(stdout);
		stream << "Actionaz Executer version " << Global::ACTIONAZ_VERSION.toString() << ", script version " << Global::SCRIPT_VERSION.toString() << "\n";
		stream.flush();
		return 0;
	}
	if(options.count("help") || options.showUnrecognizedWarning() || options.positional().count() < 1 || (options.count("code") && options.count("script")))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage: ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("filename") << "\n";
		stream << QObject::tr("Parameters are:") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	app.addLibraryPath(QApplication::applicationDirPath() + "/actions");
	app.addLibraryPath(QApplication::applicationDirPath() + "/plugins");

	if(!options.count("nocodeqt"))
		app.addLibraryPath(QApplication::applicationDirPath() + "/code");

#ifdef Q_OS_UNIX
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	// Proxy settings
	int proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	if(options.value("proxy-mode").toString() == "none")
		proxyMode = ActionTools::Settings::PROXY_NONE;
	else if(options.value("proxy-mode").toString() == "custom")
		proxyMode = ActionTools::Settings::PROXY_CUSTOM;
	else if(options.value("proxy-mode").toString() == "system")
		proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	else if(!options.value("proxy-mode").toString().isEmpty())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Unknown proxy mode, values are \"none\", \"system\" (default) or \"custom\"") << "\n";
		stream.flush();
		return -1;
	}

	QNetworkProxy proxy;

	switch(proxyMode)
	{
	case ActionTools::Settings::PROXY_NONE:
		proxy.setType(QNetworkProxy::NoProxy);
		break;
	case ActionTools::Settings::PROXY_SYSTEM:
		{
			QUrl url(Global::CONNECTIVITY_URL);
			QNetworkProxyQuery networkProxyQuery(url);
			QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(networkProxyQuery);
			if(!listOfProxies.isEmpty())
				proxy = listOfProxies.first();
			else
				proxy.setType(QNetworkProxy::NoProxy);
		}
		break;
	case ActionTools::Settings::PROXY_CUSTOM:
		{
			int type = ActionTools::Settings::PROXY_TYPE_SOCKS5;
			if(options.value("proxy-type").toString() == "http")
				type = ActionTools::Settings::PROXY_TYPE_HTTP;
			else if(options.value("proxy-type").toString() == "socks")
				type = ActionTools::Settings::PROXY_TYPE_SOCKS5;
			else if(!options.value("proxy-type").toString().isEmpty())
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown proxy type, values are \"http\" or \"socks\" (default)") << "\n";
				stream.flush();
				return -1;
			}

			QNetworkProxy proxy;

			if(type == ActionTools::Settings::PROXY_TYPE_HTTP)
				proxy.setType(QNetworkProxy::HttpProxy);
			else
				proxy.setType(QNetworkProxy::Socks5Proxy);

			proxy.setHostName(options.value("proxy-host").toString());
			proxy.setPort(options.value("proxy-port").toInt());
			proxy.setUser(options.value("proxy-user").toString());
			proxy.setPassword(options.value("proxy-password").toString());
		}
		break;
	}

	QNetworkProxy::setApplicationProxy(proxy);

	QUrl protocolUrl = QUrl::fromEncoded(arguments.at(1).toUtf8());
	if(protocolUrl.isValid() && protocolUrl.scheme() != "actionaz")
		protocolUrl = QUrl();

	MainClass::ExecutionMode executionMode = MainClass::Unknown;
	MainClass mainClass;

	if(protocolUrl.isValid())
	{
		QString mode;
		typedef QPair<QString, QString> QStringPair;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        foreach(const QStringPair &queryItem, QUrlQuery(protocolUrl.query()).queryItems())
#else
        foreach(const QStringPair &queryItem, protocolUrl.queryItems())
#endif
		{
			if(queryItem.first == "mode")
			{
				mode = queryItem.second;
				break;
			}
		}

		if(mode == "code")
			executionMode = MainClass::Code;
		else if(mode == "script")
			executionMode = MainClass::Script;
		else
		{
			if(protocolUrl.path().endsWith(".ascr"))
				executionMode = MainClass::Script;
			else if(protocolUrl.path().endsWith(".acod"))
				executionMode = MainClass::Code;
			else
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown execution mode, please specify mode=script or mode=code") << "\n";
				stream.flush();
				return -1;
			}
		}

		if(!mainClass.start(executionMode, protocolUrl))
			return -1;
	}
	else
	{
		QString filename = options.positional().at(0);

		if(options.count("code"))
			executionMode = MainClass::Code;
		else if(options.count("script"))
			executionMode = MainClass::Script;
		else
		{
			if(filename.endsWith(".ascr"))
				executionMode = MainClass::Script;
			else if(filename.endsWith(".acod"))
				executionMode = MainClass::Code;
			else
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown execution mode, please specify -s (script) or -c (code)") << "\n";
				stream.flush();
				return -1;
			}
		}

		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly))
		{
			QTextStream stream(stdout);
			stream << QObject::tr("Unable to read input file") << "\n";
			stream.flush();
			return -1;
		}

		if(!mainClass.start(executionMode, &file, file.fileName()))
		{
			file.close();

			return -1;
		}
	}

	return app.exec();
}
