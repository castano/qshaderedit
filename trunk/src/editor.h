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

#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QTabWidget>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>

class QTextEdit;

class Effect;

class Editor : public QTabWidget
{
	Q_OBJECT
public:
	
	Editor(QWidget * parent = 0);
	
	void setEffect(Effect * effect);
	
	QTextEdit * addEditor(const QString & name, const Effect * effect, int i);
	QTextEdit * currentTextEdit() const;

	int line() const;
	int column() const;
	
	bool isModified() const;
	void setModified(bool b);
	
public slots:
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();

	void gotoLine(int tab, int line, int column = 0);
	void nextTab();
	void previousTab();

	void findDialog();
	void gotoDialog();
	void findNext();
	void findPrevious();
	
signals:
	void cursorPositionChanged();
	void copyAvailable(bool available);
	void pasteAvailable(bool available);
	void undoAvailable(bool available);
	void redoAvailable(bool available);
	void textChanged();

	void modifiedChanged(bool modified);
	
protected:
	virtual void tabInserted(int index);
	virtual void tabRemoved(int index);

	bool find(const QString & text, QTextDocument::FindFlags flags=0);
	
protected slots:
	void onCurrentChanged(int idx);
	void onCopyAvailable(bool available);
	void onUndoAvailable(bool available);
	void onRedoAvailable(bool available);

private:
	QFont m_font;
	
	// @@ This should be pdata, so that QTextDocument is not included in the header
	QString lastSearch;
	QTextDocument::FindFlags lastSearchOptions;
	
};


class SourceEdit : public QTextEdit
{
Q_OBJECT

public:
	SourceEdit(QWidget * parent = 0);

protected:
	void keyPressEvent(QKeyEvent * event);
	void paintEvent(QPaintEvent * event);

	QRect lineRect();
	
protected slots:
	void cursorChanged();

private:
	int m_line;
	QRect m_lineRect;
};

#endif // EDITOR_H
