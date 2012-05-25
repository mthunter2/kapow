/***********************************************************************
 *
 * Copyright (C) 2008, 2010, 2011, 2012 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "locale_dialog.h"
#include "window.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	app.setApplicationName("Kapow");
	app.setApplicationVersion("1.3.4");
	{
		QIcon fallback(":/hicolor/256x256/apps/kapow.png");
		fallback.addFile(":/hicolor/128x128/apps/kapow.png");
		fallback.addFile(":/hicolor/64x64/apps/kapow.png");
		fallback.addFile(":/hicolor/48x48/apps/kapow.png");
		fallback.addFile(":/hicolor/32x32/apps/kapow.png");
		fallback.addFile(":/hicolor/24x24/apps/kapow.png");
		fallback.addFile(":/hicolor/22x22/apps/kapow.png");
		fallback.addFile(":/hicolor/16x16/apps/kapow.png");
		app.setWindowIcon(QIcon::fromTheme("kapow", fallback));
	}

	LocaleDialog::loadTranslator("kapow_");

	QString path;
	{
		// Check for command-line paths
		QStringList args = app.arguments();
		QString appdir = app.applicationDirPath();
#if defined(Q_OS_MAC)
		QString home_path = appdir + "/../../../Data";
#else
		QString home_path = appdir + "/Data";
#endif
		QString data_path;
		foreach (const QString& arg, args) {
			if (arg.startsWith("--home=")) {
				home_path = arg.mid(7);
			} else if (arg.startsWith("--data=")) {
				data_path = arg.mid(7);
			}
		}
		if (args.back().endsWith(".xml")) {
			data_path = args.back();
		}

		// Handle portability
		QFileInfo portable(home_path);
		if (portable.exists() && portable.isWritable()) {
			path = portable.absoluteFilePath();
			QSettings::setDefaultFormat(QSettings::IniFormat);
			if (QFile::exists(path + "/Settings/GottCode/Kapow.ini") &&
					!QFile::rename(path + "/Settings/GottCode/Kapow.ini", path + "/Settings/Kapow.ini")) {
				QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, path + "/Settings");
				app.setOrganizationName("GottCode");
			} else {
				QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, path);
				app.setOrganizationName("Settings");
			}
		} else {
			app.setOrganizationDomain("gottcode.org");
			app.setOrganizationName("GottCode");
		}

		// Handle command-line data path
		QFileInfo override(data_path);
		if (override.exists() && override.isWritable() && (override.isDir() || (override.suffix() == "xml"))) {
			path = override.absoluteFilePath();
		}
	}

	// Make sure data path exists
	if (path.isEmpty()) {
		path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

		// Update data location
		if (!QFile::exists(path)) {
#if defined(Q_OS_MAC)
			QString oldpath = QDir::homePath() + "/Library/Application Support/GottCode/Kapow/";
#elif defined(Q_OS_UNIX)
			QString oldpath = getenv("$XDG_DATA_HOME");
			if (oldpath.isEmpty()) {
				oldpath = QDir::homePath() + "/.local/share/";
			}
			oldpath += "/gottcode/kapow/";
#elif defined(Q_OS_WIN32)
			QString oldpath = QDir::homePath() + "/Application Data/GottCode/Kapow/";
#endif

			// Create data location if old data location doesn't exist
			if (!QFile::exists(oldpath)) {
				QDir dir(path);
				if (!dir.mkpath(dir.absolutePath())) {
					QMessageBox::critical(0, Window::tr("Error"), Window::tr("Unable to create time data location."));
					return 1;
				}
			// Otherwise, move old data location
			} else if (!QFile::rename(oldpath, path)) {
				QMessageBox::critical(0, Window::tr("Error"), Window::tr("Unable to move time data location."));
				return 1;
			}
		}
	}

	// Find data file
	if (!path.endsWith(".xml")) {
		if (QFile::exists(path + "/data.xml") && !QFile::rename(path + "/data.xml", path + "/kapow.xml")) {
			path += "/data.xml";
		} else {
			path += "/kapow.xml";
		}
	}

	Window window(path);
	if (window.isValid()) {
		return app.exec();
	} else {
		return 1;
	}
}
