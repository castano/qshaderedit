
#include "imageplugin.h"

#include <QtCore/QList>
//#include <QtGui/QImage>
#include <QtGui/QImageReader>

namespace
{
	
	static QList<const ImagePlugin *> * s_pluginList = NULL;
	
	
	// Taken from Qt, but do not mirror.
	static QImage convertToBGRA(const QImage &image)
	{
		QImage img = image;
		if (image.format() != QImage::Format_ARGB32) {
			img = image.convertToFormat(QImage::Format_ARGB32);
		}
		
		if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
			// mirror + swizzle
			QImage res = img.copy();
			for (int i=0; i < img.height(); i++) {
				uint *p = (uint*) img.scanLine(i);
			//	uint *q = (uint*) res.scanLine(img.height() - i - 1);
				uint *q = (uint*) res.scanLine(i);
				uint *end = p + img.width();
				while (p < end) {
					*q = ((*p << 24) & 0xff000000)
						 | ((*p >> 24) & 0x000000ff)
						 | ((*p << 8) & 0x00ff0000)
						 | ((*p >> 8) & 0x0000ff00);
					p++;
					q++;
				}
			}
			return res;
		}
		else {
			return img;
		//	return img.mirrored();
		}
	}
	
	inline static uint nextPowerOfTwo( uint x ) 
	{
		uint p = 1;
		while( x > p ) {
			p += p;
		}
		return p;
	}
	
} // namespace


// @@ Add plugin priorities. Use sorted list.
void ImagePluginManager::addPlugin(const ImagePlugin * plugin)
{
	Q_ASSERT(plugin != NULL);
	if( s_pluginList == NULL ) {
		s_pluginList = new QList<const ImagePlugin *>();
	}
	s_pluginList->append(plugin);
}

void ImagePluginManager::removePlugin(const ImagePlugin * plugin)
{
	Q_ASSERT(plugin != NULL);
	
	const int index = s_pluginList->lastIndexOf(plugin);
	Q_ASSERT(index != -1);
	s_pluginList->removeAt(index);
	
	if( s_pluginList->isEmpty() ) {
		delete s_pluginList;
		s_pluginList = NULL;
	}
}

QImage ImagePluginManager::load(QString name, GLuint obj, GLuint * target)
{
	if(s_pluginList != NULL) {
		foreach(const ImagePlugin * plugin, *s_pluginList) {
			if(plugin->canLoad(name)) {
				return plugin->load(name, obj, target);
			}
		}
	}

	return QImage();
}


// @@ Add dds plugin.
// @@ Add exr plugin.
// @@ Add hdr plugin.
// @@ Add tga plugin.


class TgaImagePlugin : public ImagePlugin
{
	virtual QList<QByteArray> supportedFormats() const
	{
		return QList<QByteArray>() << ".tga";
	}
	
	virtual bool canLoad(const QString & fileName) const
	{
		Q_UNUSED(fileName);
		return false;
	}
	
        virtual QImage load(const QString & /*name*/, GLuint obj, GLuint * target) const
	{
		Q_ASSERT(obj != 0);
		Q_ASSERT(target != NULL);
		
		return QImage();
	}
};

//REGISTER_IMAGE_PLUGIN(TgaImagePlugin);


// Image plugin that supports all the image types that Qt supports.
class QtImagePlugin : public ImagePlugin
{
public:

	virtual QList<QByteArray> supportedFormats() const
	{
		return QImageReader::supportedImageFormats();
	}
	
	virtual bool canLoad(const QString & fileName) const
	{
		Q_UNUSED(fileName);
		return true;
	}
	
	virtual QImage load(const QString & name, GLuint obj, GLuint * target) const
	{
		Q_ASSERT(obj != 0);
		Q_ASSERT(target != NULL);
		
		QImage image;
		if( name.isEmpty() || !image.load(name) ) {
			image.load(":/images/default.png");
		}
		
		QImage glImage = convertToBGRA(image);
		
		int w = glImage.width();
		int h = glImage.height();
		
		// Resize texture if NP2 not supported.
		if( !GLEW_ARB_texture_non_power_of_two ) {
			w = nextPowerOfTwo(w);
			h = nextPowerOfTwo(h);
		}
		
		// Clamp to maximum texture size.
		GLint maxTextureSize = 256;
		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
		if( w > maxTextureSize ) w = maxTextureSize; 
		if( h > maxTextureSize ) h = maxTextureSize; 
		
		if(glImage.width() != w || glImage.height() != h) {
			glImage = glImage.scaled(w, h);
		}
		
		*target = GL_TEXTURE_2D;
		glBindTexture(GL_TEXTURE_2D, obj);
		
		if(GLEW_SGIS_generate_mipmap || GLEW_VERSION_1_4) {
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, glImage.bits());
		}
		else {
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, glImage.width(), glImage.height(), GL_BGRA, GL_UNSIGNED_BYTE, glImage.bits());
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		ReportGLErrors();
		
		return image;
	}
};

REGISTER_IMAGE_PLUGIN(QtImagePlugin);

