
#include "texmanager.h"

#include <GL/glew.h>

#include <QtCore/QSharedData>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtOpenGL/QGLContext>


namespace {

	// Image plugin that supports all the image types that Qt supports.
	class QtImagePlugin
	{
	public:
	
		virtual bool canLoad(QString name) const
		{
			Q_UNUSED(name);
			return true;
		}
	
		virtual bool load(QString name, uint obj, uint * target) const
		{
			Q_ASSERT(obj != 0);
			Q_ASSERT(target != NULL);
			
			QImage image;
			if( name.isEmpty() || !image.load(name) ) {
				image.load(":/images/default.png");
			}

			*target = GL_TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, obj);
		
			if(false && (GLEW_SGIS_generate_mipmap || GLEW_VERSION_1_4)) {
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());
			}
			else {
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image.width(), image.height(), GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			return true;
		}
	};

	// @@ Add dds plugin.
	// @@ Add exr plugin.
	// @@ Add hdr plugin.

	static QtImagePlugin s_imageLoader;

} // namespace


class GLTexture::Private : public QSharedData
{
public:
	Private() : m_object(0)
	{
	}
	Private(const QString & name) : m_name(name)
	{
		glGenTextures(1, &m_object);
		
		// @@ Traverse image plugins.
		if( s_imageLoader.canLoad(m_name) ) {
			s_imageLoader.load(m_name, m_object, &m_target);
		}	
	}
	~Private()
	{
		if(!m_name.isNull())
		{
			// Remove from the cache.
			s_textureMap.remove(m_name);
			qDebug() << "Eliminate" << m_name;
		}
		
		if(m_object != 0) {
			glDeleteTextures(1, &m_object);
		}
	}

	const QString & name() const { return m_name; }
	uint object() const { return m_object; }
	uint target() const { return m_target; }

	static QMap<QString, GLTexture::Private *> s_textureMap;

private:
	QString m_name;
	uint m_object;
	uint m_target;
};

//static
QMap<QString, GLTexture::Private *> GLTexture::Private::s_textureMap;


/// Constructor.
GLTexture::GLTexture() : m_data(new Private)
{
}
GLTexture::GLTexture(const GLTexture & t) : m_data(t.m_data)
{
}
void GLTexture::operator= (const GLTexture & t)
{
	m_data = t.m_data;
}
GLTexture::~GLTexture()
{
}

/// Constructor.
GLTexture::GLTexture(GLTexture::Private * p) : m_data(p)
{
}

// static
GLTexture GLTexture::open(const QString & name)
{
	Private * p;
	if( Private::s_textureMap.contains(name) ) {
		p = Private::s_textureMap[name];
	}
	else {
		p = new GLTexture::Private(name);
		Private::s_textureMap[name] = p;
	}
	return GLTexture(p);
}

/// Get texture object.
uint GLTexture::object() const
{
	return m_data->object();
}

/// Get texture target.
uint GLTexture::target() const
{
	return m_data->target();
}

