
#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include <QtCore/QRegExp>
#include "messagepanel.h"


class OutputParser
{
public:
	virtual ~OutputParser() {}

	virtual void parseLine(const QString& line) = 0;

	MessagePanel::Type type() const
	{
		return m_type;
	}

	int line() const
	{
		return m_line;
	}

	int column() const
	{
		return m_column;
	}

protected:
	MessagePanel::Type m_type;
	int m_line;
	int m_column;
};


class AtiGlslOutputParser: public OutputParser
{
public:
	void parseLine(const QString& line);
};

class AtiAsmOutputParser: public OutputParser
{
public:
	void parseLine(const QString& line);
};

/// parses glsl and cg output
class NvidiaOutputParser: public OutputParser
{
public:
	void parseLine(const QString& line);
};

class NvidiaAsmOutputParser: public OutputParser
{
public:
	void parseLine(const QString& line);
};

#endif
