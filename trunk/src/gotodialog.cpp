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

#include "gotodialog.h"


GotoDialog::GotoDialog(QWidget *parent/*=0*/) : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.spinBox->setMinimum(1);
	ui.spinBox->setMaximum(1000);	// @@ Get number of lines in current doc.
}

int GotoDialog::line() const
{
	return ui.spinBox->value();
}

void GotoDialog::setLine(int l)
{
	ui.spinBox->setValue(l);
	ui.spinBox->selectAll();
}

void GotoDialog::setRange(int minimum, int maximum)
{
	ui.spinBox->setMinimum(minimum);
	ui.spinBox->setMaximum(maximum);
}
