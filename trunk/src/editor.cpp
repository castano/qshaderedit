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

#include "highlighter.h"
#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "effect.h"
#include "completer.h"

#include <QtCore/QDebug>
#include <QtGui/QTabBar>
#include <QtGui/QTextCursor>
#include <QtGui/QTextLayout>
#include <QtGui/QTextBlock>
//#include <QtGui/QTextDocument>
#include <QtGui/QMessageBox>
#include <QtGui/QPainter>


SourceEdit::SourceEdit(QWidget * parent): QPlainTextEdit(parent)
        ,m_highlighter(NULL)
        ,m_completer(NULL)
{
        //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));

    updateLineNumberAreaWidth(0);
}

SourceEdit::~SourceEdit()
{
    if(lineNumberArea)
        delete lineNumberArea;

    if(m_highlighter)
        delete m_highlighter;

    if(m_completer)
        delete m_completer;

}

void SourceEdit::keyPressEvent(QKeyEvent * event)
{
        QPlainTextEdit::keyPressEvent(event);
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		QTextCursor cursor = this->textCursor();
		QTextBlock prevBlock = cursor.block().previous();
		if (prevBlock.isValid()) {
			QString text = prevBlock.text();
			QString indentStr("");
			for (int n = 0; text[n].isSpace(); n++)
				indentStr += text[n];

			// increase indent
			text = text.trimmed();
			QRegExp rx("^(if|else|while|for).*");
			if (text.endsWith('{') || rx.exactMatch(text))
				indentStr += '\t';
			cursor.insertText(indentStr);
		}
	}
	else if (event->key() == Qt::Key_BraceRight)
	{
		// decrease indent
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::Left);
		if (cursor.block().text().endsWith("\t}"))
			cursor.deletePreviousChar();
	}
	else if (event->key() == Qt::Key_BraceLeft)
	{
		// decrease indent
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::Left);
		if (cursor.block().text().endsWith("\t{"))
			cursor.deletePreviousChar();		
	}

        if(((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_Space))
            || (event->key() == Qt::Key_Period))
        {
            if ( m_completer )
                m_completer->invokeCompletion( this );
        }
}

int SourceEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 15 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void SourceEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void SourceEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


void SourceEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void SourceEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor("#8F8F8F"));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    int icursor = this->textCursor().blockNumber();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            if(icursor == blockNumber)
                painter.setPen(Qt::black);
            else
                painter.setPen(QColor("#B3B2B1"));
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

/*********************************Peter Komar code, august 2009 *******************************/
void SourceEdit::setHighlighter(Highlighter *highlighter)
{
    if(m_highlighter)
        delete m_highlighter;

    m_highlighter = highlighter;
    m_highlighter->setDocument(document());
}

Highlighter* SourceEdit::highlighter() const
{
    return  m_highlighter;
}

void SourceEdit::setCompleter(Completer* completer)
{
    if(completer == m_completer)
        return;

    if(m_completer)
        delete m_completer;

    m_completer = completer;
}

/****************************************************************************************/

//void SourceEdit::paintEvent(QPaintEvent * event)
//{
//        //QPainter p(viewport());
        //QRect rect = lineRect().intersected(event->region().boundingRect());
        //p.fillRect(rect, QBrush(QColor(248, 248, 248)));
        //p.end();
	
//	QTextEdit::paintEvent(event);
//}

//QRect SourceEdit::lineRect()
//{
//	QRect rect = cursorRect();
//	rect.setLeft(0);
//	rect.setWidth(viewport()->width());
//	return rect;
//}

//void SourceEdit::cursorChanged()
//{
        /*if (m_line != textCursor().blockNumber())
	{
		viewport()->update();

		// @@ Optimize updated.
		//viewport()->update(m_lineRect);
		
		m_lineRect = lineRect();
		m_line = textCursor().blockNumber();
		
		//viewport()->update(m_lineRect);
        }*/
//}


Editor::Editor(QWidget * parent)
        : QToolBox(parent)
{
        //this->hide();

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

        m_font = Highlighter::readFontSettings();

//	m_font.setStyleHint(QFont::Courier, QFont::PreferQuality);
}

QPlainTextEdit * Editor::currentTextEdit() const
{
    if(this->count())
        return static_cast<QPlainTextEdit *>(currentWidget());
    return NULL;
}

int Editor::line() const
{
    if(currentTextEdit())
       return currentTextEdit()->textCursor().blockNumber() + 1;
    return 0;
}

int Editor::column() const
{
        if(currentTextEdit())
            return currentTextEdit()->textCursor().columnNumber() + 1;
        return 0;
}


bool Editor::isModified() const
{
	bool modified = false;

	const int num = this->count();
	for (int i = 0; i < num; i++)
	{
                QPlainTextEdit * editor = qobject_cast<QPlainTextEdit *>(this->widget(i));
		
		if (editor != NULL && editor->document()->isModified()) {
			modified = true;
		}
	}

	return modified;	
}

void Editor::setModified(bool b)
{
	const int num = this->count();
	for (int i = 0; i < num; i++)
	{
                QPlainTextEdit * editor = qobject_cast<QPlainTextEdit *>(this->widget(i));
		
		if (editor != NULL) {
			editor->document()->setModified(b);
		}
	}
}

// slots.
void Editor::undo()
{
    if(currentTextEdit())
	currentTextEdit()->document()->undo();
}
void Editor::redo()
{
    if(currentTextEdit())
	currentTextEdit()->document()->redo();
}
void Editor::cut()
{
    if(currentTextEdit())
	currentTextEdit()->cut();
}
void Editor::copy()
{
    if(currentTextEdit())
	currentTextEdit()->copy();
}
void Editor::paste()
{
    if(currentTextEdit())
	currentTextEdit()->paste();
}

void Editor::gotoLine(int tab, int line, int column)
{
	setCurrentIndex(tab);
	QTextCursor cursor(currentTextEdit()->textCursor());
	cursor.movePosition(QTextCursor::Start);
	cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
	if (column >= 1)
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column - 1);
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
	
	// Get current selection and set default value.
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
	
	// Get current line and set default value.
	dialog.setLine(line());

	// Set valid range.
	dialog.setRange(1, 1000);
	
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
        Q_UNUSED(idx);
	emit cursorPositionChanged();

        if(!currentTextEdit())
            return;

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

void Editor::setEffect(Effect * effect)
{
	if (effect == NULL)
	{
		// Remove all tabs.
                while (this->count() > 0)
                {
                    this->removeItem(0);
                }
	}
	else
	{
		int inputNum = effect->getInputNum();
		for (int i = 0; i < inputNum; i++)
		{
			this->addEditor(effect->getInputName(i), effect, i);
		}
	}
}

QPlainTextEdit * Editor::addEditor(const QString & name, const Effect * effect, int i)
{
        SourceEdit *m_sourcer = new SourceEdit(this);
        this->addItem(m_sourcer, name);
        m_sourcer->setFont(m_font);
        m_sourcer->setLineWrapMode(QPlainTextEdit::NoWrap);
        m_sourcer->setTabStopWidth(28);
        //textEdit->setAcceptRichText(false);
	
        QTextDocument * textDocument = m_sourcer->document();
	
        //Highlighter *m_highlighter = new Highlighter(textDocument);
        Highlighter *m_highlighter = new Highlighter;
        m_highlighter->setRules(effect->factory()->highlightingRules());
        m_highlighter->setMultiLineCommentStart(effect->factory()->multiLineCommentStart());
        m_highlighter->setMultiLineCommentEnd(effect->factory()->multiLineCommentEnd());
        Completer *m_completer = new Completer(effect->factory()->extension());

        m_sourcer->setHighlighter(m_highlighter);
        m_sourcer->setCompleter(m_completer);

        m_sourcer->setPlainText(effect->getInput(i));
	textDocument->setModified(false);
	
        connect(m_sourcer, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
        connect(m_sourcer, SIGNAL(cursorPositionChanged()), this, SIGNAL(cursorPositionChanged()));
	connect(textDocument, SIGNAL(modificationChanged(bool)), this, SIGNAL(modifiedChanged(bool)));
	
	if (count() == 1) {
		emit onCurrentChanged(currentIndex());
	}
	
        return m_sourcer;
}

/*******************************Peter Komar code, august 2009 *****************************/
void Editor::updateHighlighter()
{
    if(!this->count())
        return;
    for(int i=0; i<count(); i++)
    {
        SourceEdit* ed = static_cast<SourceEdit *>(widget(i));
        if(!ed)
            return;

        Highlighter* m_highlighter = ed->highlighter();
        if(!m_highlighter)
            return;

        m_highlighter->createFormats();
        m_highlighter->rehighlight();
        m_font = Highlighter::readFontSettings();
        ed->setFont(m_font);
        ed->update();
    }
}
/*******************************************************************************************/

void Editor::itemInserted(int index)
{
        QToolBox::itemInserted(index);
	
        //if( count() == 2 ) {
        //        this->show();
        //}
}

void Editor::itemRemoved(int index)
{
        QToolBox::itemRemoved(index);

        //if( count() == 1 ) {
        //        this->hide();
        //}
}

