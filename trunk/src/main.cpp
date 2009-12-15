/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

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

#include "qshaderedit.h"
#include "nofocusstyle.h"

#include <QtGui/QApplication>


int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(qshaderedit);

    QApplication app(argc, argv);
    app.setStyle(new NoFocusStyle);

    QString filename;
    if (qApp->arguments().size() > 1) {
	    filename = qApp->arguments().at(1);
    }

    QShaderEdit shaderEdit(filename);
    shaderEdit.show();

    return app.exec();
}
