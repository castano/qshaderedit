//
// C++ Interface: ConfigDlg
//
// Description:
//
//august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QtGui/QDialog>

class QTreeWidget;
class QTreeWidgetItem;
class QComboBox;
class QFontComboBox;
class QTextEdit;

class ConfigDlg : public QDialog
{
    Q_OBJECT
public:
    ConfigDlg(QWidget* parent=0);
    bool b_update_data;

public slots:
    void slot_save();
    void slot_color(QTreeWidgetItem * , int );
    void slot_preview();

private:
    void initGUI();
    void readData();

    QTreeWidget *m_listItems;
    QFontComboBox *m_textFont;
    QComboBox *m_font_size;
    QTextEdit *m_preview;
};

#endif // CONFIGDLG_H
