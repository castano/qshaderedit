
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
