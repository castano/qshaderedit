
#include "newdialog.h"
#include "effect.h"


NewDialog::NewDialog(QWidget *parent/*=0*/, bool startup/*=false*/) : QDialog(parent)
{
	ui.setupUi(this);
	
	ui.listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.listWidget->setViewMode(QListView::ListMode);
	
	const QList<const EffectFactory *> & effectFactoryList = EffectFactory::factoryList();
	foreach(const EffectFactory * effectFactory, effectFactoryList) {
		
		if(effectFactory->isSupported()) {
			QListWidgetItem * newItem = new QListWidgetItem;
			newItem->setText(effectFactory->name());
			newItem->setIcon(effectFactory->icon());
			ui.listWidget->addItem(newItem);
		}
	}
	
	// Select first element.
	ui.listWidget->setCurrentRow(0);
	
	connect(ui.openButton, SIGNAL(clicked()), this, SLOT(openEffect()));
	
	// Display the open button only on startup.	
	if(!startup) {
		ui.openButton->setVisible(false);
	}
}

QString NewDialog::shaderType() const
{
	return ui.listWidget->currentItem()->text();
}

void NewDialog::openEffect()
{
	done(OpenEffect);
}
