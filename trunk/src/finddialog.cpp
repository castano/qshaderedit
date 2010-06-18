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

#include "finddialog.h"
#include <QtGui/QLineEdit>

FindDialog::FindDialog(QWidget *parent/*=0*/, bool forward/*=true*/) : QDialog(parent)
{
	ui.setupUi(this);
	
	if(forward) {
		ui.forwardRadioButton->setChecked(true);
	}
	else {
		ui.backwardRadioButton->setChecked(true);
	}
	
	// @@ Remember last settings.
	// @@ Save last searches.
	// @@ Fill combo box with last searches.
}

QString FindDialog::searchText() const
{
	return ui.findComboBox->currentText();
}
void FindDialog::setSearchText(const QString & str)
{
	ui.findComboBox->lineEdit()->setText(str);
	ui.findComboBox->lineEdit()->selectAll();
}

FindDialog::Direction FindDialog::direction() const
{
	if(ui.backwardRadioButton->isChecked()) return Direction_Backward;
	/*if(ui.forwardRadioButton->isChecked())*/ return Direction_Forward;
}

bool FindDialog::wholeWord() const
{
	return ui.wholeWordsCheckBox->isChecked();
}

bool FindDialog::caseSensitive() const
{
	return ui.caseSensitiveCheckBox->isChecked();
}

