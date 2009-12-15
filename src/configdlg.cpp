//
// C++ Interface: ConfigDlg
//
// Description:
//
// august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
#include "configdlg.h"
#include "highlighter.h"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QComboBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QColorDialog>
#include <QtCore/QSettings>

ConfigDlg::ConfigDlg(QWidget* parent) : QDialog(parent)
        ,b_update_data(false)
{
    setWindowTitle(tr("Options"));
    initGUI();
    readData();
     slot_preview();
}

void ConfigDlg::initGUI()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    QLabel *lab1 = new QLabel(tr("Text font"));
    QLabel *lab2 = new QLabel(tr("Text size"));
    QLabel *lab3 = new QLabel(tr("Color scheme"));
    QLabel *lab4 = new QLabel(tr("Preview:"));

    m_listItems = new QTreeWidget;

    m_textFont = new QFontComboBox;
    QFont font= Highlighter::readFontSettings();
    m_textFont->setCurrentFont(font);
    connect(m_textFont,SIGNAL(currentFontChanged(QFont)),this,SLOT(slot_preview()));

    m_font_size = new QComboBox;
    m_font_size->addItems(QStringList()<<"8" <<"10" <<"11" <<"14" <<"15");
    m_font_size->setEditable(true);
    m_font_size->setValidator(new QIntValidator(3,100,  m_font_size));
    m_font_size->setEditText(QString::number(font.pointSize()));
    connect(m_font_size,SIGNAL(activated(int)),this,SLOT(slot_preview()));

    m_preview = new QTextEdit;
    m_preview->setReadOnly(true);
    m_preview->setFixedHeight(100);
    QString text = "<br><div style=\"margin-left: 35px;\">Sample text </div>";
    m_preview->setFont(font);
    m_preview->setHtml(text);

    hLayout->addWidget(lab1);
    hLayout->addWidget(m_textFont);
    hLayout->addWidget(lab2);
    hLayout->addWidget(m_font_size);

    vLayout->addLayout(hLayout);
    vLayout->addWidget(lab3, 0, Qt::AlignCenter);
    vLayout->addWidget(m_listItems);
    vLayout->addWidget(lab4,0,Qt::AlignLeft);
    vLayout->addWidget(m_preview);

    QPushButton *btnok = new QPushButton(tr("OK"),this);
    QPushButton *btncancel = new QPushButton(tr("Cancel"),this);
    connect(btnok,SIGNAL(clicked()),this, SLOT(slot_save()));
    connect(btncancel,SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *h = new QHBoxLayout;
    h->addStretch(100);
    h->addWidget(btnok);
    h->addWidget(btncancel);

    vLayout->addLayout(h);

    setLayout(vLayout);
}

void ConfigDlg::readData()
{
    QStringList labels;
    labels << tr("Name item") << tr("Color") << tr("Bold") << tr("Italic");
    m_listItems->setHeaderLabels(labels);

    QHeaderView *h = m_listItems->header();
    h->setMovable(FALSE);
    h->setResizeMode(0, QHeaderView::Fixed);
    h->setResizeMode(1, QHeaderView::Fixed);
    h->setResizeMode(2, QHeaderView::Fixed);
    h->setResizeMode(3, QHeaderView::Fixed);

    m_listItems->setDragEnabled(FALSE);
    m_listItems->setAllColumnsShowFocus(true);
    m_listItems->setColumnWidth(0,100);
    m_listItems->setRootIsDecorated(false);
    m_listItems->setFocusPolicy(Qt::NoFocus);
    m_listItems->setSelectionMode(QAbstractItemView::NoSelection);


    connect(m_listItems, SIGNAL(itemClicked(QTreeWidgetItem * , int )), this, SLOT(slot_color(QTreeWidgetItem*,int)));

    QTreeWidgetItem *first = NULL;//set for correctly work select font namer and size


    for(Highlighter::FormatType f = Highlighter::Keyword; f<Highlighter::FormatType_Max; ++f)
    {
        QTextCharFormat format = Highlighter::readSettings(f);

        QTreeWidgetItem *item = new QTreeWidgetItem(m_listItems);

        switch(f)
        {
            case Highlighter::Keyword : item->setText(0,"Keyword"); first = item;
                           break;
            case Highlighter::Comment : item->setText(0,"Comment");
                           break;
            case Highlighter::DataType :item->setText(0,"Data Type");
                           break;
            case Highlighter::BuiltinVar : item->setText(0,"Builtin Var");
                           break;
            case Highlighter::BuiltinFunction : item->setText(0,"Builtin Function");
                           break;
            case Highlighter::Number : item->setText(0,"Number");
                           break;
            case Highlighter::String : item->setText(0,"String");
                           break;
            case Highlighter::Misc : item->setText(0,"Misc");
                           break;
            //case Highlighter::FormatType_Max: break; //for compiler :)
            default: return;
        }


        //item->setData(1,Qt::DisplayRole,format.foreground().color());
        item->setBackgroundColor(1,format.foreground().color());
        //item->setTextColor(1,format.foreground().color());
        QCheckBox *box1 = new QCheckBox;
        QCheckBox *box2 = new QCheckBox;
        box1->setChecked(false);
        box1->setChecked(false);

        connect(box1,SIGNAL(clicked()),this,SLOT(slot_preview()));
        connect(box2,SIGNAL(clicked()),this,SLOT(slot_preview()));

        if(format.fontWeight() == QFont::Bold)
            box1->setChecked(true);

        if(format.fontItalic())
            box2->setChecked(true);

        m_listItems->setItemWidget(item, 2, box1);
        m_listItems->setItemWidget(item, 3, box2);
    }

    m_listItems->setCurrentItem(first);
}


void ConfigDlg::slot_save()
{
    b_update_data = true;

    QFont font = m_textFont->currentFont();
    int size = m_font_size->currentText().toInt();
    font.setPointSize(size);

    Highlighter::saveFontSettings(font);

    for(Highlighter::FormatType f = Highlighter::Keyword; f<Highlighter::FormatType_Max; ++f)
    {
        QTextCharFormat format;

        QTreeWidgetItem *item = m_listItems->topLevelItem(f);

        QColor c = item->backgroundColor(1);

        format.setForeground(c);

        QCheckBox *bold = (QCheckBox *)m_listItems->itemWidget(item,2);//bold
        QCheckBox *italic = (QCheckBox *)m_listItems->itemWidget(item,3);//italic

        if(!bold->isChecked() && !italic->isChecked())
            format.setFontWeight(QFont::Normal);
        else if(bold->isChecked())
            format.setFontWeight(QFont::Bold);

        if(italic->isChecked())
            format.setFontItalic(true);

         Highlighter::writeSettings(f,format);
        }

    accept();

}

void ConfigDlg::slot_color(QTreeWidgetItem * item, int column)
{
    if(!item)
        return;
    if(column == 1)
    {
        QColor col1 = item->backgroundColor(1);
        QColor col = QColorDialog::getColor(col1, this);
        //if your Qt version < 4.5 uncomment this and remove ^
        //QColor col = QColorDialog::getColor(this);

        if(col.isValid())
           item->setBackgroundColor(1,col);
    }
    m_listItems->setCurrentItem(item);

    slot_preview();
}

void ConfigDlg::slot_preview()
{
    QTreeWidgetItem *item = m_listItems->currentItem();
    if(!item)
        return;

    QFont font = m_textFont->currentFont();

    QCheckBox *box1 = (QCheckBox*)m_listItems->itemWidget(item,2);
    QCheckBox *box2 = (QCheckBox*)m_listItems->itemWidget(item,3);

    if(!box1 && !box2)
        return;

    if(box1->isChecked())
        font.setBold(true);
    if(box2->isChecked())
        font.setItalic(true);

    int ps = m_font_size->currentText().toInt();
    font.setPointSize(ps);

    m_preview->setFont(font);

    QColor c = item->backgroundColor(1);
    //m_preview->setTextColor(c);// - don't work because in stylesheets light and dark sets text color

    QString text =QString("<br><font color=\"%1\">").arg(c.name());
    text += "<div style=\"margin-left: 35px;\">Sample text </div></font>";
    m_preview->setHtml(text);
}
