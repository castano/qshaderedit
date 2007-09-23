
#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QtGui/QDialog>

#include "ui_newdialog.h"


class NewDialog : public QDialog
{
	Q_OBJECT
	public:
		enum {
			OpenEffect = 100
		};
		
	public:
		NewDialog(QWidget *parent = 0, bool startup = false);
		
		QString shaderType() const;
		
	private slots:
		void openEffect();
		
	private:
		Ui::NewDialog ui;
};


#endif // NEWDIALOG_H
