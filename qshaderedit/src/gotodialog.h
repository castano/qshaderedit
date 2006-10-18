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
		void setLine(int l);
		void setRange(int minimum, int maximum);
		
	private:
		Ui::GotoDialog ui;
};


#endif // NEWDIALOG_H
