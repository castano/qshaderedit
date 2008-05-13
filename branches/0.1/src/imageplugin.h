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
