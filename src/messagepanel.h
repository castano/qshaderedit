/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef MESSAGEPANEL_H
#define MESSAGEPANEL_H

#include <QtGui/QDockWidget>

class QTextEdit;
class OutputParser;


class MessagePanel : public QDockWidget
{
	Q_OBJECT
public:

	enum Type {
		Info,
		Warning,
		Error
	};

	MessagePanel(const QString & title, QWidget * parent = 0, Qt::WFlags flags = 0);
	MessagePanel(QWidget * parent = 0, Qt::WFlags flags = 0);
	~MessagePanel();

	virtual QSize sizeHint() const;


public slots:
	void clear();

	void log(const QString& s, Type type = Info, int inputNumber = -1, int line = -1, int column = -1);
	void log(const QString& s, int inputNumber, OutputParser* parser);
	
	void error(QString s, int inputNumber = -1, int line = -1, int column = -1);
	void warning(QString s, int inputNumber = -1, int line = -1, int column = -1);
	void info(QString s, int inputNumber = -1, int line = -1, int column = -1);


signals:
	void messageClicked(int inputNumber, int line, int column);


protected:
	bool eventFilter(QObject* object, QEvent* event);


private:
	void initWidget();


private:
	QTextEdit * m_log;

};



#endif // MESSAGEPANEL_H
