/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>

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
