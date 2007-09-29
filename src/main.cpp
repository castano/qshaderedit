
#include "qshaderedit.h"

#include <QtGui/QApplication>
#include <QtGui/QPlastiqueStyle>

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#endif


int main(int argc, char **argv)
{
#if defined(Q_WS_X11)
	// This causes lockups in some X servers.
	XInitThreads();
#endif
	
    QApplication app(argc, argv);

#if defined(Q_WS_WIN)
	app.setStyle(new QPlastiqueStyle());
#endif
	
	QString filename;
	if (qApp->arguments().size() > 1) {
		filename = qApp->arguments().at(1);
	}

	QShaderEdit * shaderEdit = new QShaderEdit(filename);
	
    return app.exec();
}
