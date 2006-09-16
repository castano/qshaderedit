#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QTabWidget>
#include <QtGui/QTextDocument>

class QTextEdit;

class Editor : public QTabWidget
{
	Q_OBJECT
public:
	
	Editor(QWidget * parent = 0);
	
	QTextEdit * addEditor(const QString & name);
	QTextEdit * currentTextEdit() const;

	int line() const;
	
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



#endif // EDITOR_H
