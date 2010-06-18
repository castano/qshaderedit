/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>

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

#include "messagepanel.h"
#include "outputparser.h"

#include <QtCore/Qt>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextEdit>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>


class LogData: public QTextBlockUserData
{
public:
	LogData(int i, int l, int c):
		inputNumber(i), line(l), column(c)
	{ }

	int inputNumber;
	int line;
	int column;
};


MessagePanel::MessagePanel(const QString & title, QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
	QDockWidget(title, parent, flags), m_log(NULL)
{
	initWidget();
}

MessagePanel::MessagePanel(QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
	QDockWidget(parent, flags), m_log(NULL)
{
	initWidget();
}

MessagePanel::~MessagePanel()
{
	delete m_log;
	m_log = NULL;
}

void MessagePanel::log(const QString& s, Type type, int inputNumber, int line, int column)
{
	if (s.trimmed().isEmpty())
		return;

	QTextCursor cursor(m_log->textCursor());

	QTextCharFormat format;
	switch (type) {
		case Info:
			format.setForeground(QColor(0, 160, 0));
			break;
		case Warning:
			format.setForeground(QColor(160, 160, 0));
			break;
		case Error:
			format.setForeground(QColor(160, 0, 0));
			break;
	}

	cursor.setCharFormat(format);
	cursor.insertText(s);
	cursor.block().setUserData(new LogData(inputNumber, line, column));

	cursor.insertBlock();
	m_log->setTextCursor(cursor);
}

void MessagePanel::log(const QString& s, int inputNumber, OutputParser* parser)
{
	if (parser == NULL) {
		log(s, MessagePanel::Error, inputNumber);
	}
	else {
		QStringList lines = s.trimmed().split('\n', QString::SkipEmptyParts);
		foreach (QString line, lines) {
			parser->parseLine(line);
			log(line, parser->type(), inputNumber, parser->line(), parser->column());
		}
	}
}

QSize MessagePanel::sizeHint() const
{
	return QSize(200, 100);
}

void MessagePanel::initWidget()
{
	m_log = new QTextEdit(this);
	m_log->setUndoRedoEnabled(false);
	m_log->setReadOnly(true);
	m_log->zoomOut();
	m_log->viewport()->installEventFilter(this);
	m_log->setFocusPolicy(Qt::NoFocus);
	setWidget(m_log);
}

void MessagePanel::clear()
{
	m_log->clear();
}

void MessagePanel::error(QString s, int inputNumber, int line, int column)
{
	log(s, Error, inputNumber, line, column);
}

void MessagePanel::warning(QString s, int inputNumber, int line, int column)
{
	log(s, Warning, inputNumber, line, column);
}

void MessagePanel::info(QString s, int inputNumber, int line, int column)
{
	log(s, Info, inputNumber, line, column);
}

bool MessagePanel::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_log->viewport() && event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		QTextCursor cursor = m_log->cursorForPosition(mouseEvent->pos());
		if (!cursor.isNull())
		{
			LogData* data = (LogData*)cursor.block().userData();
			if (data && data->inputNumber >= 0 && data->line >= 0)
				emit(messageClicked(data->inputNumber, data->line, data->column));
			return true;
		}
	}

	return QDockWidget::eventFilter(object, event);
}
