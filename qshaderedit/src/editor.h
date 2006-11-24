#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QTabWidget>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>

class QTextEdit;

class Editor : public QTabWidget
{
	Q_OBJECT
public:
	
	Editor(QWidget * parent = 0);
	
	QTextEdit * addEditor(const QString & name);
	QTextEdit * currentTextEdit() const;

	int line() const;
	int column() const;
	
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
