
#include "highlighter.h"

#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// static
QVector<QTextCharFormat> Highlighter::s_formats;

//Highlighter::Highlighter(QTextDocument* parent): QSyntaxHighlighter(parent)
Highlighter::Highlighter(QObject *parent): QSyntaxHighlighter(parent)
{
	if (s_formats.size() == 0)
		createFormats();
}

void Highlighter::setRules(const QList<Rule>& rules)
{
	m_rules = rules;
}

void Highlighter::setMultiLineCommentStart(const QString& s)
{
	m_multiLineCommentStart = s;
}

void Highlighter::setMultiLineCommentEnd(const QString& s)
{
	m_multiLineCommentEnd = s;
}

QFont Highlighter::readFontSettings()
{
    QSettings pref( "Castano Inc", "QShaderEdit" );

    pref.beginGroup("CodeEditor");

    QString name;
    int size=0;

    #if defined(Q_OS_DARWIN)
        name = pref.value("font","Monaco").toString();
        size = pref.value("size",11).toInt();
    #elif defined(Q_OS_WIN32)
        name = pref.value("font","Courier").toString();
        size = pref.value("size",8).toInt();
    #else
        name = pref.value("font","Monospace").toString();
        size = pref.value("size",8).toInt();
    #endif

    QFont m_font;// = QApplication::font();
    m_font.setFamily(name);
    m_font.setPointSize(size);
    m_font.setFixedPitch(true);

    pref.endGroup();

    return m_font;
}

void Highlighter::saveFontSettings(const QFont& font)
{
    QSettings pref( "Castano Inc", "QShaderEdit" );

    pref.beginGroup("CodeEditor");
    pref.setValue("font",font.family());
    pref.setValue("size",font.pointSize());
    pref.endGroup();
}

QTextCharFormat Highlighter::readSettings(FormatType f)
{
    QSettings pref( "Castano Inc", "QShaderEdit" );

    pref.beginGroup("CodeEditor");

    QTextCharFormat format;
    QString color;
    int type=0; //0 - normal,1 - bold,2 - italic 3 - bold&itelic

    switch(f)
        {
            case Keyword : pref.beginGroup("Keyword");
                           color = pref.value("color","#B2D9FF").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case Comment : pref.beginGroup("Comment");
                           color = pref.value("color","#00CC00").toString();
                           type = pref.value("type",2).toInt();
                           break;
            case DataType :pref.beginGroup("DataType");
                           color = pref.value("color","#2DE52D").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case BuiltinVar : pref.beginGroup("BuiltinVar");
                           color = pref.value("color","#B2D9FF").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case BuiltinFunction : pref.beginGroup("BuiltinFunction");
                           color = pref.value("color","#B2D9FF").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case Number : pref.beginGroup("Number");
                           color = pref.value("color","#008080").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case String : pref.beginGroup("String");
                           color = pref.value("color","#8282FF").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case Misc : pref.beginGroup("Misc");
                           color = pref.value("color","#808000").toString();
                           type = pref.value("type",0).toInt();
                           break;
            case FormatType_Max: break; //for compiler :)
            default: color = "#000000";
        }

        pref.endGroup();

        format.setForeground(QColor(color));

        switch(type)
        {
            case 0: format.setFontWeight(QFont::Normal); break;
            case 1: format.setFontWeight(QFont::Bold); break;
            case 2: format.setFontItalic(true); break;
            case 3: format.setFontItalic(true); format.setFontWeight(QFont::Bold); break;
            default: format.setFontWeight(QFont::Normal);
        }


    pref.endGroup();

    return format;
}

void Highlighter::writeSettings(FormatType f, const QTextCharFormat& format)
{
    QSettings pref( "Castano Inc", "QShaderEdit" );

    pref.beginGroup("CodeEditor");

    QString color = format.foreground().color().name();
    int type=0; //0 - normal,1 - bold,2 - italic 3 - bold&itelic

    if(format.fontWeight() == QFont::Normal)
        type = 0;
    else if(format.fontWeight() == QFont::Bold)
        type = 1;

    if((format.fontItalic()) && (format.fontWeight() == QFont::Bold))
        type = 3;
    else if(format.fontItalic())
        type = 2;

    switch(f)
        {
            case Keyword : pref.beginGroup("Keyword");
                           break;
            case Comment : pref.beginGroup("Comment");
                           break;
            case DataType :pref.beginGroup("DataType");
                           break;
            case BuiltinVar : pref.beginGroup("BuiltinVar");
                           break;
            case BuiltinFunction : pref.beginGroup("BuiltinFunction");
                           break;
            case Number : pref.beginGroup("Number");
                           break;
            case String : pref.beginGroup("String");
                           break;
            case Misc : pref.beginGroup("Misc");
                           break;
            case FormatType_Max: break; //for compiler :)
            default: return;
        }

   pref.setValue("color",color);
   pref.setValue("type",type);
   pref.endGroup();
   pref.endGroup();
}

//static
QString Highlighter::getSyntaxKeyWords(const QString& extation, const char* type)
{
    QString ShaderKeyWords = ":/db/"+extation+"."+type;
    QFile file(ShaderKeyWords);

    QString out;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return out;

     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();

         int i = line.indexOf("//");
         if(i != -1) //remove comment
             line.truncate(i);

         i = line.indexOf("(");
         if(i != -1) //remove functions parameters
              line.truncate(i);

         i = line.indexOf("{");
         if(i != -1) //remove structured properties
              line.truncate(i);

         i = line.indexOf("[");
         if(i != -1) //remove array's symbol []
              line.truncate(i);

         line = line.simplified();

         out += line + "|";
     }

     out.chop(1);

     file.close();

     return out;
}

//static
QStringList Highlighter::getKeyWords(const QString& extation)
{
    QStringList list;

    QStringList names;

    names.append(":/db/"+extation+".kw");
    names.append(":/db/"+extation+".dt");
    names.append(":/db/"+extation+".bv");
    names.append(":/db/"+extation+".bf");
    QFile file;

    for(int i=0; i< names.size();i++)
    {
        file.setFileName(names.at(i));

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            line = line.simplified();
            if(line.at(0) != '[')
                list.append(line);
        }
        file.close();
        list.append("---END BLOCK---");
    }

    return list;
}


// static
void Highlighter::createFormats()
{
    s_formats.clear();
    s_formats.resize(FormatType_Max);

    QTextCharFormat format;


    for(FormatType f = Keyword; f<FormatType_Max; ++f)
        s_formats[f] = readSettings(f);

}

void Highlighter::highlightBlock(const QString& text)
{
	foreach (Rule rule, m_rules)
	{
		int index = text.indexOf(rule.pattern);
		while (index >= 0)
		{
			int length = rule.pattern.matchedLength();
			setFormat(index, length, s_formats[rule.type]);
			index = text.indexOf(rule.pattern, index + length);
		}
	}

	if (!m_multiLineCommentStart.isEmpty())
	{
		setCurrentBlockState(0);

		int startIndex = 0;
		if (previousBlockState() != 1)
			startIndex = text.indexOf(m_multiLineCommentStart);

		while (startIndex >= 0) {
			int endIndex = text.indexOf(m_multiLineCommentEnd, startIndex);
			int commentLength;
			if (endIndex == -1) {
				setCurrentBlockState(1);
				commentLength = text.length() - startIndex;
			} else {
				commentLength = endIndex - startIndex + m_multiLineCommentStart.length();
			}
			setFormat(startIndex, commentLength, s_formats[Comment]);
			startIndex = text.indexOf(m_multiLineCommentStart, startIndex + commentLength);
		}
	}
}
