
#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QtGui/QDialog>

#include "ui_newdialog.h"


class NewDialog : public QDialog
{
	Q_OBJECT
	
	public:
		NewDialog(QWidget *parent = 0);
		
		QString shaderType() const;
		
	private:
		Ui::Dialog ui;
};


#endif // NEWDIALOG_H
