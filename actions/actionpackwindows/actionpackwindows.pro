include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackWindows
QT += script
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
HEADERS += actionpackwindows.h
RESOURCES += actionpackwindows.qrc
TRANSLATIONS = ../../locale/actionpackwindows_fr_FR.ts

unix {
	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../../locale/actionpackwindows_fr_FR.qm
	locales.CONFIG = no_check_exist

	INSTALLS += locales
}
