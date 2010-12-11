/***********************************************************************
 *
 * Copyright (C) 2008, 2009, 2010 Graeme Gott <graeme@gottcode.org>
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

#ifndef WINDOW_H
#define WINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QSystemTrayIcon>
class QComboBox;
class QLabel;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTimer;
class QTreeView;
class QTreeWidget;
class QTreeWidgetItem;
class DataModel;
class Project;

class Window : public QMainWindow {
	Q_OBJECT
public:
	Window(const QString& filename, QWidget* parent = 0);

protected:
	virtual void closeEvent(QCloseEvent* event);

private slots:
	void about();
	void setDecimalTotals(bool decimals);
	void setInlineEditing(bool edit);
	void start();
	void stop();
	void cancel();
	void updateTime();
	void addProject();
	void editProject();
	void removeProject();
	void showReport();
	void projectActivated(QTreeWidgetItem* item);
	void projectChanged(QTreeWidgetItem* item, int column);
	void filterChanged(int index);
	void sessionPressed(const QModelIndex& index);
	void sessionsInserted(const QModelIndex& parent, int start, int end);
	void sessionsScrolled(int value);
	void addSession();
	void editSession();
	void removeSession();
	void toggleColumnHidden(int column);
	void save();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void addProject(const QString& name);
	void removeProject(QTreeWidgetItem* item);
	void updateDetails();
	void updateDisplay();
	void updateSessionButtons();
	void updateTrayIcon();
	void updateWindowTitle(const QString& project);

private:
	QString m_filename;
	QSplitter* m_contents;
	QTreeWidget* m_projects;
	QLabel* m_display;
	QPushButton* m_start;
	QPushButton* m_stop;
	QPushButton* m_cancel;
	QComboBox* m_filter;
	QTreeView* m_details;
	bool m_decimals;
	bool m_inline;

	QTimer* m_timer;
	QDateTime m_current_time;

	Project* m_active_project;
	DataModel* m_active_model;
	int m_active_timers;

	QAction* m_add_project;
	QAction* m_remove_project;
	QAction* m_add_session;
	QAction* m_edit_session;
	QAction* m_remove_session;

	QSystemTrayIcon* m_tray_icon;
};

#endif