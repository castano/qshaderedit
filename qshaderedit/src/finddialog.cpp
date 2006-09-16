
#include "finddialog.h"


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
	ui.findComboBox->addItem(str);
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

