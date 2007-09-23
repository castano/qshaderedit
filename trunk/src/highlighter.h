
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtGui/QTextCharFormat>


class Highlighter: public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum FormatType {
		Keyword = 0,
		Comment,
		DataType,
		BuiltinVar,
		BuiltinFunction,
		Number,
		String,
		Misc,
		FormatType_Max
	};

	struct Rule
	{
		QRegExp pattern;
		FormatType type;
	};

    Highlighter(QTextDocument* parent);

	void setRules(const QList<Rule>& rules);
	void setMultiLineCommentStart(const QString& s);
	void setMultiLineCommentEnd(const QString& s);

protected:
	void highlightBlock(const QString& text);

private:
	QList<Rule> m_rules;
	QString m_multiLineCommentStart;
	QString m_multiLineCommentEnd;

	static QVector<QTextCharFormat> s_formats; // idx is FormatType
	static void createFormats();
};

#endif
