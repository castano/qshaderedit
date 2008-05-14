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

#ifndef IMAGEPLUGIN_H
#define IMAGEPLUGIN_H

#include "glutils.h"

#include <QtCore/QString>
#include <QtGui/QImage>


// Image plugin interface.
class ImagePlugin
{
public:
	virtual ~ImagePlugin() {}
	virtual QList<QByteArray> supportedFormats() const = 0;
	virtual bool canLoad(const QString & name) const = 0;
	virtual QImage load(const QString & name, GLuint obj, GLuint * target) const = 0;
};


// Image plugin factory.
namespace ImagePluginManager
{
	void addPlugin(const ImagePlugin * plugin);
	void removePlugin(const ImagePlugin * factory);
	
	QImage load(QString name, GLuint obj, GLuint * target);	
};


#define REGISTER_IMAGE_PLUGIN(Plugin) \
	namespace { \
		static Plugin plugin##Plugin; \
		struct Plugin##Registrar { \
			Plugin##Registrar() { ImagePluginManager::addPlugin(&plugin##Plugin); } \
			~Plugin##Registrar() { ImagePluginManager::removePlugin(&plugin##Plugin); } \
		}; \
		static Plugin##Registrar registrar##Plugin; \
	}


#endif // IMAGEPLUGIN_H
