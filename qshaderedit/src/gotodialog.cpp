
#include "gotodialog.h"


GotoDialog::GotoDialog(QWidget *parent/*=0*/) : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.spinBox->setMinimum(0);
	ui.spinBox->setMaximum(100);	// @@ Get number of lines in current doc.
}

int GotoDialog::line() const
{
	return ui.spinBox->value();
}
