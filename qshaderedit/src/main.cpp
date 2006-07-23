
#include "qshaderedit.h"

#include <QtGui/QApplication>
#include <QtGui/QWindowsXPStyle>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

//	QApplication::setStyle(new QWindowsXPStyle);
	
	QShaderEdit * shaderEdit = new QShaderEdit();
	
	//app.setApplicationName(QApplication::tr("QShaderEditor"));
    return app.exec();
}
