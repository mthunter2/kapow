TEMPLATE = app
CONFIG += warn_on release
macx {
	# Uncomment the following line to compile on PowerPC Macs
	# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG += x86 ppc
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

unix: !macx {
	TARGET = kapow
} else {
	TARGET = Kapow
}

HEADERS = src/data_model.h \
	src/date_editor.h \
	src/filter_model.h \
	src/locale_dialog.h \
	src/project.h \
	src/report.h \
	src/session.h \
	src/session_dialog.h \
	src/time_editor.h \
	src/window.h

SOURCES = src/data_model.cpp \
	src/date_editor.cpp \
	src/filter_model.cpp \
	src/locale_dialog.cpp \
	src/main.cpp \
	src/project.cpp \
	src/report.cpp \
	src/session.cpp \
	src/session_dialog.cpp \
	src/time_editor.cpp \
	src/window.cpp

TRANSLATIONS = translations/cs.ts \
	translations/de.ts \
	translations/en_US.ts \
	translations/es.ts \
	translations/fr.ts \
	translations/it.ts

RESOURCES = icons/icons.qrc
macx {
	ICON = icons/kapow.icns
}
win32 {
	RC_FILE = icons/icon.rc
}

unix: !macx {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}

	target.path = $$PREFIX/bin/

	icon.files = icons/kapow.png
	icon.path = $$PREFIX/share/icons/hicolor/48x48/apps

	desktop.files = icons/kapow.desktop
	desktop.path = $$PREFIX/share/applications/

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/kapow/translations

	INSTALLS += target icon desktop qm
}
