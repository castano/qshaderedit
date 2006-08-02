
#include "texmanager.h"

#include <GL/glew.h>

#include <QtGui/QImage>
#include <QtOpenGL/QGLContext>


namespace {

	struct Texture {
		int refCount;
		uint texObj;
	};

	QMap<QString, Texture> s_textureMap;

	static TexManager * s_texManager = NULL;

	// Image plugin that supports all the image types that Qt supports.
	class QtImagePlugin
	{
	public:
	
		virtual bool canLoad(QString name) const
		{
			Q_UNUSED(name);
			return true;
		}
	
		virtual bool load(QString name, uint * obj) const
		{		
			QImage image;
			if( name.isEmpty() || !image.load(name) ) {
				image.load(":/images/default.png");
			}
		
			glGenTextures(1, obj);
			glBindTexture(GL_TEXTURE_2D, *obj);
		
			if(GLEW_SGIS_generate_mipmap || GLEW_VERSION_1_4) {
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			}
		
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());
		
			return true;
		}
	};

	// @@ Add dds plugin.
	// @@ Add exr plugin.
	// @@ Add hdr plugin.

	static QtImagePlugin s_imageLoader;

} // namespace




/// Constructor.
GLTexture::GLTexture(const QString & name)
{
	// @@ TBD
}

/// Destructor.
GLTexture::~GLTexture()
{
	// @@ TBD
}

/// Cast operator.
/*GLTexture::operator uint() const
{
	return 0;
}*/



// static
TexManager * TexManager::instance()
{
	Q_ASSERT(s_texManager != NULL);
	return s_texManager;
}

/// Constructor.
TexManager::TexManager()
{
	Q_ASSERT(s_texManager == NULL);
	s_texManager = this;
}

/// Destructor.
TexManager::~TexManager()
{
	Q_ASSERT(s_texManager != NULL);
	s_texManager = NULL;
}


/// Add a texture object for the given file name. Increase the reference count.
uint TexManager::addTexture(QString name)
{
	if( s_textureMap.contains(name) ) {
		Texture & texture = s_textureMap[name];
		texture.refCount++;
		return texture.texObj;
	}

	if( s_imageLoader.canLoad(name) ) {
		Texture texture;
		if( s_imageLoader.load(name, &texture.texObj) ) {
			texture.refCount = 1;
			s_textureMap.insert(name, texture);
		}
		return texture.texObj;
	}
	return 0;
}

/// Get the texture object for the given name.
uint TexManager::getTexture(QString name)
{
	if( s_textureMap.contains(name) ) {
		Texture & texture = s_textureMap[name];
		return texture.texObj;
	}
	return 0;
}

/// Release the texture object associated to the given file name. Decrease the reference count.
void TexManager::releaseTexture(QString name)
{
	Q_ASSERT( s_textureMap.contains(name) );

	Texture texture = s_textureMap.value(name);
	texture.refCount--;
	if( texture.refCount == 0 ) {
		glDeleteTextures(1, &texture.texObj);
		s_textureMap.remove(name);
	}
}

