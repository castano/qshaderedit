
#include "qshaderedit.h"

#include <QtGui/QApplication>
#include <QtGui/QWindowsXPStyle>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

//	QApplication::setStyle(new QWindowsXPStyle);
	
	QShaderEdit * shaderEdit = new QShaderEdit();
	Q_UNUSED(shaderEdit);
	
	/*
	QStyle * arthurStyle = new ArthurStyle();
	shaderEdit.setStyle(arthurStyle);

	QList<QWidget *> widgets = qFindChildren<QWidget *>(shaderEdit);
	foreach(QWidget * w, widgets) {
		w->setStyle(arthurStyle);
	}
	*/
	
	//app.setApplicationName(QApplication::tr("QShaderEditor"));
    return app.exec();
}
