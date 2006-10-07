
#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"

#include <QtCore/QDebug>
#include <QtGui/QTabBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QTextLayout>
#include <QtGui/QTextBlock>
//#include <QtGui/QTextDocument>
#include <QtGui/QMessageBox>


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
#if QT_VERSION >= 0x040200
    return currentTextEdit()->textCursor().blockNumber();
#else
	return 0;
#endif
}

int Editor::column() const
{
#if QT_VERSION >= 0x040200
	return currentTextEdit()->textCursor().columnNumber();
#else
	return 0;
#endif
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

void Editor::findDialog()
{
	FindDialog dialog(this);
	
	// @@ Get current selection and set default value.
	QTextCursor cursor = currentTextEdit()->textCursor();
	QString selection = cursor.selectedText();
	if( selection != "" ) {
		dialog.setSearchText(selection);
	}
	
	int result = dialog.exec();
	if( result == QDialog::Accepted ) {
		
		lastSearch = dialog.searchText();
		
		lastSearchOptions = 0;
		if( dialog.caseSensitive() ) lastSearchOptions |= QTextDocument::FindCaseSensitively;
		if( dialog.wholeWord() ) lastSearchOptions |= QTextDocument::FindWholeWords;
		if( dialog.direction() == FindDialog::Direction_Backward ) lastSearchOptions |= QTextDocument::FindBackward;
		
		if( !find(lastSearch, lastSearchOptions) ) {
			QMessageBox::information(this, tr("Find"), tr("String '%0' not found").arg(lastSearch));
		}
	}
}

void Editor::gotoDialog()
{
	GotoDialog dialog(this);
	
	// @@ get current line and set default value.
	
	int result = dialog.exec();
	if( result == QDialog::Accepted ) {
		gotoLine(currentIndex(), dialog.line());
	}
}

void Editor::findNext()
{
	if( lastSearch == "" ) {
		findDialog();
	}
	else {
		if( !find(lastSearch, lastSearchOptions) ) {
			QMessageBox::information(this, tr("Find Next"), tr("String '%0' not found").arg(lastSearch)); 
		}
	}
}

void Editor::findPrevious()
{
	if( lastSearch == "" ) {
		findDialog();
	}
	else {
		if( !find(lastSearch, lastSearchOptions ^ QTextDocument::FindBackward) ) {
			QMessageBox::information(this, tr("Find Previous"), tr("String '%0' not found").arg(lastSearch)); 
		}
	}	
}

bool Editor::find(const QString & text, QTextDocument::FindFlags flags/*=0*/)
{
	QTextCursor cursor = currentTextEdit()->textCursor();
	
	if( flags & QTextDocument::FindBackward )
	{
		// Move cursor to the beginning of selection.
		cursor.setPosition(cursor.selectionStart());
		currentTextEdit()->setTextCursor(cursor);
	}
	else
	{
		// Move cursor to the end of selection.
		cursor.setPosition(cursor.selectionEnd());
		currentTextEdit()->setTextCursor(cursor);
	}
	
	return currentTextEdit()->find(text, flags);
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

