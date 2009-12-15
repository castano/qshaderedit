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

        friend FormatType& operator ++(FormatType& f){
            int k = (int)f;
            ++k;
            f = (FormatType)k;
            return f; }

    //Highlighter(QTextDocument* parent);
        Highlighter(QObject *parent=0);

	void setRules(const QList<Rule>& rules);
	void setMultiLineCommentStart(const QString& s);
	void setMultiLineCommentEnd(const QString& s);

        static void createFormats();

        /*****************************Peter Komar code, august 2009 ***************************/
        static  QTextCharFormat readSettings(FormatType type);
        static void writeSettings(FormatType type, const QTextCharFormat& format);
        static QFont readFontSettings();
        static void saveFontSettings(const QFont& font);
        static QString getSyntaxKeyWords(const QString& extation, const char* type);
        static QStringList getKeyWords(const QString& extation);
       /********************************************************************************/


protected:
	void highlightBlock(const QString& text);

private:
	QList<Rule> m_rules;
	QString m_multiLineCommentStart;
	QString m_multiLineCommentEnd;

	static QVector<QTextCharFormat> s_formats; // idx is FormatType
};

#endif
