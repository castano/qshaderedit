#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QtGui/QDialog>

#include "ui_finddialog.h"


class FindDialog : public QDialog
{
	Q_OBJECT
	public:
		enum Direction
		{
			Direction_Forward,
			Direction_Backward
		};
		
	public:
		FindDialog(QWidget *parent = 0, bool forward = true);
		
		QString searchText() const;
		void setSearchText(const QString & str);
		
		Direction direction() const;
		bool wholeWord() const;
		bool caseSensitive() const;
		
	private:
		Ui::FindDialog ui;
};


#endif // FINDDIALOG_H
