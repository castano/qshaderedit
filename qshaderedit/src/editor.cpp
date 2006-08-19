
#include "editor.h"

#include <QtCore/QDebug>
#include <QtGui/QTabBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QTextLayout>
#include <QtGui/QTextBlock>

Editor::Editor(QWidget * parent) : QTabWidget(parent) 
{
	tabBar()->hide();

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

#if defined(Q_OS_DARWIN)
	m_font.setFamily("Monaco");
	m_font.setPointSize(11);
#elif defined(Q_OS_WIN32)
	m_font.setFamily("Courier");
	m_font.setPointSize(8);
#else
	m_font.setFamily("Monospace");
	m_font.setPointSize(8);
#endif
	m_font.setFixedPitch(true);
//	m_font.setStyleHint(QFont::Courier, QFont::PreferQuality);
}

QTextEdit * Editor::currentTextEdit() const
{
	return static_cast<QTextEdit *>(currentWidget());
}

int Editor::line() const
{
	// None of these work...
//	return currentTextEdit()->cursorRect(currentTextEdit()->textCursor()).y();
	/*
	int position = currentTextEdit()->textCursor().position();
	
	QTextBlock block = currentTextEdit()->document()->findBlock(position);
//	qDebug() << block.contains(position);
//	qDebug() << block.text();
	
	QTextLayout * layout = block.layout();
	Q_ASSERT(layout != NULL);
	//qDebug() << layout->text();
	qDebug() << layout->lineCount();
	
//	qDebug() << layout->text();
//	return layout->lineForTextPosition(position).lineNumber();
	
	return position;
	*/
	return 0;
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

void Editor::nextTab()
{
	setCurrentIndex(currentIndex() +1);
}

void Editor::previousTab()
{
	setCurrentIndex(currentIndex() -1);	
}

void Editor::onCurrentChanged(int idx)
{
	Q_ASSERT(idx >= 0);
	Q_UNUSED(idx);
	emit cursorPositionChanged();

	QTextDocument * document = currentTextEdit()->document();
	QTextCursor cursor = currentTextEdit()->textCursor();
	
	// update cursor position.
	emit cursorPositionChanged();

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
	textEdit->setFont(m_font);
	textEdit->setLineWrapMode(QTextEdit::NoWrap);
	textEdit->setTabStopWidth(28);
	textEdit->setAcceptRichText(false);

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

