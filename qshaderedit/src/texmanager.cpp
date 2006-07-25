
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
	
	
	class QtImagePlugin
	{
	public:
		
		virtual bool canLoad(QString name) const
		{
			Q_UNUSED(name);
			return true;
		}
		
		virtual bool load(QGLContext * context, QString name, uint * obj) const
		{
			//const QGLContext * context = QGLContext::currentContext();
			Q_ASSERT(context != NULL);
			
			if( name.endsWith(".dds") ) {
				*obj = context->bindTexture(name);
			}
			else {
				QImage image(name);
				*obj = context->bindTexture(image);
			}
			
			/*
			glGenTextures(1, obj);
			glBindTexture(GL_TEXTURE_2D, *obj);
			
			// if
			
			QImage image(
			*/
			
			return true;
		}
	};
	
	static QtImagePlugin s_imageLoader;
	
	static TexManager * s_texManager = NULL;

} // namespace


// static
TexManager * TexManager::instance()
{
	Q_ASSERT(s_texManager != NULL);
	return s_texManager;
}

/// Constructor.
TexManager::TexManager(QGLContext * ctx) : m_context(ctx)
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
		if( s_imageLoader.load(m_context, name, &texture.texObj) ) {
			texture.refCount = 1;			
			s_textureMap.insert(name, texture);
		}
	}
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

