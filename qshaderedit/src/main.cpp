
#include "qshaderedit.h"

#include <QtGui/QApplication>
#include <QtGui/QWindowsXPStyle>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

//	QApplication::setStyle(new QWindowsXPStyle);
	
	QString filename;
	if (qApp->arguments().size() > 1)
		filename = qApp->arguments().at(1);

	QShaderEdit * shaderEdit = new QShaderEdit(filename);
	
	
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
