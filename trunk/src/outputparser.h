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
