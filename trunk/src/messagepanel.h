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
