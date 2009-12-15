//
// C++ Interface: Completer
//
// Description:
//
// august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
#include "completer.h"
#include "highlighter.h"
//
#include <QtGui/QPlainTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextCursor>
#include <QtGui/QToolTip>
#include <QtCore/QPoint>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFontMetrics>
//
//
Completer::Completer( const QString& extation, QWidget* parent, Qt::WFlags f )
	: QFrame( parent )
        ,CLASS_METOD(QListWidgetItem::UserType+1)
        ,m_extation(extation)
{
        // frame
        setWindowFlags( f );
	setFrameShape( QFrame::StyledPanel );
	resize( QSize( 400, 150 ).expandedTo( minimumSizeHint() ) );
	// layout
        QVBoxLayout *vboxLayout = new QVBoxLayout;
	vboxLayout->setSpacing( 0 );
	vboxLayout->setMargin( 0 );

        // listwidget
	lwItems = new QListWidget( this );
        QSizePolicy sizePolicy( static_cast<QSizePolicy::Policy>( 7 ), static_cast<QSizePolicy::Policy>( 7 ) );
	sizePolicy.setHorizontalStretch( 0 );
	sizePolicy.setVerticalStretch( 1 );
	sizePolicy.setHeightForWidth( lwItems->sizePolicy().hasHeightForWidth() );
	lwItems->setSizePolicy( sizePolicy );

        connect(lwItems,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));
        connect(lwItems,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                this,SLOT(showToolTip(QListWidgetItem*,QListWidgetItem*)));
	//
	vboxLayout->addWidget( lwItems );

        setLayout(vboxLayout);

        loadAtocompliteData();
}


void Completer::itemClicked( QListWidgetItem* item )
{
	if ( !item )
		return;
        QTextCursor cursor( mEditor->textCursor() );
        QString s = leftTextCursor();
        QString s1 = item->text();
        if(s1.startsWith(s))
            s1.remove(0,s.length());

        cursor.clearSelection();
        //Remove alow comment if you want towards completer remove an right words part
        //cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
        //cursor.removeSelectedText();
        cursor.insertText( s1 );
	mEditor->setTextCursor( cursor );
	close();
}

void Completer::showToolTip(QListWidgetItem * current, QListWidgetItem * /*previous */)
{
    QString s = current->toolTip();
    if(QToolTip::isVisible())
        QToolTip::hideText();

    if(!s.isEmpty() && mEditor){
        QPoint pos = mEditor->mapToGlobal( mEditor->cursorRect().topLeft() );

        QFontMetrics fm = QApplication::fontMetrics();
        pos.ry() -= fm.height()+3;
        QToolTip::showText(pos, s);
    }
}

//
void Completer::invokeCompletion( QPlainTextEdit* editor )
{
    if ( !isVisible() )
    {
        mEditor = editor;
        setParent( mEditor, windowFlags() );
        QPoint position = mEditor->mapToGlobal( mEditor->cursorRect().topLeft() );
        int h = mEditor->cursorRect().height();
        QDesktopWidget *dw = QApplication::desktop();

        if(position.ry() < (dw->screen()->height()-height()))
            position.ry() += h;
        else
            position.ry() -= (h+height());

        move( position );
        if ( prepareCompletion() )
        {
            show();
            lwItems->setFocus();
        }
    }
}

void Completer::loadAtocompliteData()
{
    QStringList listData = Highlighter::getKeyWords(m_extation);

    if(listData.isEmpty())
        return;

    unsigned char nCount=0; //indicate if keyword, Build Function, ...

    QStringList::const_iterator it;
     for (it = listData.constBegin(); it != listData.constEnd();
            ++it)
     {
         QString line = (*it), comment;

         if(line == "---END BLOCK---")
         {
             ++nCount;
             continue;
         }

         int i = line.indexOf("//");
         if(i != -1)
         {
             comment = line.mid(i+2);
             line.truncate(i);
             line = line.simplified();
         }

         i = line.indexOf("{");
         if(i != -1)
         {
             int j = line.indexOf("}");
             QString prop = line.mid(i+1,j-i-1);
             line.truncate(i);
             QStringList l = prop.split(";");
             if(!l.isEmpty())
             {
                 for(i=0;i<l.size();i++)
                 {
                     QString s = l.at(i);
                     s = s.simplified();
                     if(!s.isEmpty())
                         m_propertiesStruct.insert(line,s);
                 }
             }
         }
         QListWidgetItem *item = new QListWidgetItem(lwItems);
         item->setText(line);
         item->setToolTip(comment);

         switch(nCount)
         {
             case 0: item->setIcon(QIcon(":/images/keyword.png")); break;
             case 1: item->setIcon(QIcon(":/images/DataType.png")); break;
             case 2: item->setIcon(QIcon(":/images/BuiltinVar.png")); break;
             case 3: item->setIcon(QIcon(":/images/BuiltinFunction.png")); break;
             default: item->setIcon(QIcon(":/images/DataType.png"));
         }
     }
    lwItems->sortItems();
}


//
bool Completer::prepareCompletion()
{
    reset();
    QTextCursor cursor( mEditor->textCursor() );

    QString s = leftTextCursor();

    if(s.isEmpty())
        return true;
    if(s.endsWith('.'))
    {
        for(int i=0; i < lwItems->count(); i++)
             lwItems->item(i)->setHidden(true);
        s.remove(".");

        int i = s.indexOf("[");
        if(i != -1)
            s.truncate(i);

        QList<QString> list = m_propertiesStruct.values(s);

        if(list.isEmpty())
            return false;

        QListWidgetItem *first = NULL;

        for(int i=0;i<list.size();i++)
        {
            QListWidgetItem *item = new QListWidgetItem(lwItems, CLASS_METOD);
            if(!i)
                first = item;
            item->setText(list.at(i));
            item->setIcon(QIcon(":/images/DataType.png"));
        }

        lwItems->setCurrentItem(first);

        return true;
    }

    QList<QListWidgetItem*> list = lwItems->findItems(s,Qt::MatchStartsWith);
    if(list.isEmpty())
        return false;

    for(int i=0; i < lwItems->count(); i++)
    {
        QListWidgetItem *item = lwItems->item(i);
        if(list.indexOf(item) == -1)
            item->setHidden(true);
    }

    lwItems->setCurrentItem(list.first());
    lwItems->update();

    return true;
}

QString Completer::leftTextCursor() const
{
    if(!mEditor)
        return QString("");

    QTextCursor cursor( mEditor->textCursor() );

    QString s="", word="";
    QRegExp rx("(&|\\*|\\(|\\)|\\+|=|\\-|/|\\?|;|:)(;)?");

    do{
        cursor.movePosition(QTextCursor::Left);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        s = cursor.selectedText();
        cursor.movePosition(QTextCursor::Left);

        s = s.simplified();
        if(rx.exactMatch(s) || (s == ";"))
            s = "";
        if(!s.isEmpty())
            word.push_front(s);
    }while(s !="");

    return word;
}

void Completer::reset()
{
    QListWidgetItem *item=NULL;

    for(int i=0; i<lwItems->count(); i++)
    {
        item = lwItems->item(i);

        if(item->type() == CLASS_METOD)
        {
            delete item;
            continue;
        }

        item->setHidden(false);
    }
    lwItems->setCurrentRow(0);
}
