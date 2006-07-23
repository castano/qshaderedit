
#include "editor.h"

#include <QtGui/QTabBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>

Editor::Editor(QWidget * parent) : QTabWidget(parent) 
{
	tabBar()->hide();

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

//	m_font.setFamily("monospace");
//	m_font.setFixedPitch(true);
//	m_font.setPointSize(10);
}

QTextEdit * Editor::currentTextEdit() const
{
	return static_cast<QTextEdit *>(currentWidget());
}

// slots.
void Editor::undo()
{
	currentTextEdit()->document()->undo();
}
void Editor::redo()
{
	currentTextEdit()->document()->redo();
}
void Editor::cut()
{
	currentTextEdit()->cut();
}
void Editor::copy()
{
	currentTextEdit()->copy();
}
void Editor::paste()
{
	currentTextEdit()->paste();
}

void Editor::gotoLine(int tab, int line, int column)
{
	setCurrentIndex(tab);
	QTextCursor cursor(currentTextEdit()->textCursor());
	cursor.movePosition(QTextCursor::Start);
	cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line -1);
	if (column >= 0)
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column);
	currentTextEdit()->setTextCursor(cursor);
	currentWidget()->setFocus();
}

void Editor::onCurrentChanged(int idx)
{
	Q_ASSERT(idx >= 0);
	Q_UNUSED(idx);
	emit cursorPositionChanged();

	QTextDocument * document = currentTextEdit()->document();
	QTextCursor cursor = currentTextEdit()->textCursor();
	
	// @@ How do we get the position?
	//currentTextEdit()->textCursor();

	// update copy/undo/redo available.
	emit copyAvailable(cursor.position() != cursor.anchor());
	emit undoAvailable(document->isUndoAvailable());
	emit redoAvailable(document->isRedoAvailable());
	emit pasteAvailable(true);
	
	connect(currentTextEdit(), SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
	connect(currentTextEdit(), SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
	connect(currentTextEdit(), SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
}

void Editor::onCopyAvailable(bool available)
{
	emit copyAvailable(available);
}
void Editor::onUndoAvailable(bool available)
{
	emit undoAvailable(available);
}
void Editor::onRedoAvailable(bool available)
{
	emit redoAvailable(available);
}

QTextEdit * Editor::addEditor(const QString & name)
{
	QTextEdit * textEdit = new QTextEdit();
	this->addTab(textEdit, name);
//	textEdit->setFont(m_font);
	textEdit->setFontFamily("Monospace");
//	textEdit->setFontFamily("Courier");
	textEdit->setFontPointSize(8);
	textEdit->setLineWrapMode(QTextEdit::NoWrap);
	textEdit->setTabStopWidth(28);

	connect(textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
	connect(textEdit, SIGNAL(cursorPositionChanged()), this, SIGNAL(cursorPositionChanged()));

	if( count() == 1 ) {
		emit onCurrentChanged(currentIndex());
	}
	
	return textEdit;
}

void Editor::tabInserted(int index)
{
	QTabWidget::tabInserted(index);
	
	if( count() == 2 ) {
		tabBar()->show();
	}
}

void Editor::tabRemoved(int index)
{
	QTabWidget::tabRemoved(index);
	
	if( count() == 1 ) {
		tabBar()->hide();
	}
}

