
#include "outputparser.h"


void AtiGlslOutputParser::parseLine(const QString& line)
{
	static QRegExp s_errorPattern("^ERROR: \\d+:(\\d+).*$");

	if (s_errorPattern.exactMatch(line)) {
		m_type = MessagePanel::Error;
		m_line = s_errorPattern.cap(1).toInt();
	}
	else if (line.startsWith("ERROR")) {  // generic error without line number
		m_type = MessagePanel::Error;
		m_line = -1;
	}
	else if (line.startsWith("Warning:")) {
		m_type = MessagePanel::Warning;
		m_line = -1;
	}
	else {
		m_type = MessagePanel::Info;
		m_line = -1;
	}
	m_column = -1;
}

void AtiAsmOutputParser::parseLine(const QString& line)
{
	static QRegExp s_errorPattern("^(?:Error on )?line (\\d+):.*$");

	if (s_errorPattern.exactMatch(line)) {
		m_type = MessagePanel::Error;
		m_line = s_errorPattern.cap(1).toInt();
	}
	else {
		m_type = MessagePanel::Info;
		m_line = -1;
	}
	m_column = -1;
}


void NvidiaOutputParser::parseLine(const QString& line)
{
	static QRegExp s_errorPattern("^.*\\((\\d+)\\) : error C\\d+:.*$");
	static QRegExp s_warningPattern("^.*\\((\\d+)\\) : warning C\\d+:.*$");

	if (s_errorPattern.exactMatch(line)) {
		m_type = MessagePanel::Error;
		m_line = s_errorPattern.cap(1).toInt();
	}
	else if (s_warningPattern.exactMatch(line)) {
		m_type = MessagePanel::Warning;
		m_line = s_warningPattern.cap(1).toInt();
	}
	else {
		m_type = MessagePanel::Info;
		m_line = -1;
	}
	m_column = -1;
}

void NvidiaAsmOutputParser::parseLine(const QString& line)
{
	static QRegExp s_errorPattern("^line (\\d+), column (\\-?\\d+):  error:.*$");

	if (s_errorPattern.exactMatch(line)) {
		m_type = MessagePanel::Error;
		m_line = s_errorPattern.cap(1).toInt();
		m_column = s_errorPattern.cap(2).toInt();
	}
	else {
		m_type = MessagePanel::Info;
		m_line = -1;
		m_column = -1;
	}
}
