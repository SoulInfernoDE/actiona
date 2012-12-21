include(../common.pri)
unix:!mac:QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN -Wl,--rpath=$${PREFIX}/lib/actionaz
QT += xml \
    network \
    script \
    scripttools
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
TARGET = actexec
VERSION = $$ACTIONAZ_VERSION
DESTDIR = ..
SOURCES += main.cpp \
    executer.cpp \
    codeexecuter.cpp \
    scriptexecuter.cpp \
	mainclass.cpp
HEADERS += executer.h \
    codeexecuter.h \
    scriptexecuter.h \
	mainclass.h \
    global.h
INCLUDEPATH += . \
    .. \
    ../tools \
	../actiontools
FORMS += 
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lole32
unix:LIBS += -lXtst
LIBS += -L.. \
    -ltools \
    -lactiontools \
    -lexecuter
win32:RC_FILE = actexecuter.rc
TRANSLATIONS = ../locale/actexecuter_fr_FR.ts
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify

unix {
	target.path = $${PREFIX}/bin

	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../locale/actexecuter_fr_FR.qm
	locales.CONFIG = no_check_exist

	manfile.path = $${PREFIX}/share/man/man1
	manfile.files = ../docs/actexec.1

	INSTALLS += target locales manfile
}
