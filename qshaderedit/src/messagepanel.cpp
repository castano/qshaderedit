
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
	if (!parser)
		log(s, MessagePanel::Error, inputNumber);

	QStringList lines = s.trimmed().split('\n', QString::SkipEmptyParts);
	foreach (QString line, lines) {
		parser->parseLine(line);
		log(line, parser->type(), inputNumber, parser->line(), parser->column());
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
