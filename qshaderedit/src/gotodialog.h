#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QtGui/QDialog>

#include "ui_gotodialog.h"


class GotoDialog : public QDialog
{
	Q_OBJECT
	public:
		GotoDialog(QWidget *parent = 0);
		
		int line() const;
		
	private:
		Ui::GotoDialog ui;
};


#endif // NEWDIALOG_H
