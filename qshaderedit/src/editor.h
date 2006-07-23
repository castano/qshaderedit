#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QTabWidget>

class QTextEdit;

class Editor : public QTabWidget
{
	Q_OBJECT
public:
	
	Editor(QWidget * parent = 0);
	
	QTextEdit * addEditor(const QString & name);
	QTextEdit * currentTextEdit() const;

public slots:
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();

	void gotoLine(int tab, int line, int column = 0);

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
	
};



#endif // EDITOR_H
