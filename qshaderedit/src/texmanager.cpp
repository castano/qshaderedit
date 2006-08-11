
#include "texmanager.h"

#include <QtCore/QSharedData>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtOpenGL/QGLContext>

namespace {
	
	// Report OpenGL errors.
	static void ReportGLErrors() {
		int error = glGetError();

		if( error != GL_NO_ERROR ) {
			switch( error ) {
				case GL_INVALID_ENUM:
					qDebug( "*** ReportGLErrors: Invalid enum.\n" );
				break;
				case GL_INVALID_VALUE:
					qDebug( "*** ReportGLErrors: Invalid value.\n" );
				break;
				case GL_INVALID_OPERATION:
					qDebug( "*** ReportGLErrors: Invalid operation.\n" );
				break;
				case GL_STACK_OVERFLOW:
					qDebug( "*** ReportGLErrors: Stack overflow.\n" );
				break;
				case GL_STACK_UNDERFLOW:
					qDebug( "*** ReportGLErrors: Stack underflow.\n" );
				break;
				case GL_OUT_OF_MEMORY:
					qDebug( "*** ReportGLErrors: Out of memory.\n" );
				break;
				default:
					qDebug( "*** ReportGLErrors: Unknown error!\n" );
			}
		}
	}

	// Taken from Qt.
	QImage convertToBGRA(const QImage &image)
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
				uint *q = (uint*) res.scanLine(img.height() - i - 1);
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
			return img.mirrored();
		}
	}
	
	// Image plugin that supports all the image types that Qt supports.
	class QtImagePlugin
	{
	public:
	
		virtual bool canLoad(QString name) const
		{
			Q_UNUSED(name);
			return true;
		}
	
		virtual bool load(QString name, GLuint obj, GLuint * target) const
		{
			Q_ASSERT(obj != 0);
			Q_ASSERT(target != NULL);
			
			QImage image;
			if( name.isEmpty() || !image.load(name) ) {
				image.load(":/images/default.png");
			}
			
			image = convertToBGRA(image);
			
			*target = GL_TEXTURE_2D;
			glBindTexture(GL_TEXTURE_2D, obj);
			
			if(GLEW_SGIS_generate_mipmap || GLEW_VERSION_1_4) {
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else {
			//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image.width(), image.height(), GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			ReportGLErrors();
			
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
		if(m_object != 0) {
			qDebug() << "eliminate:" << m_name;
			
			// Remove from the cache.
			s_textureMap.remove(m_name);
			
			glDeleteTextures(1, &m_object);
			m_object = 0;
		}
	}

	const QString & name() const { return m_name; }
	GLuint object() const { return m_object; }
	GLuint target() const { return m_target; }

	static QMap<QString, GLTexture::Private *> s_textureMap;

private:
	QString m_name;
	GLuint m_object;
	GLuint m_target;
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
	qDebug() << "open:" << name;
	
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
GLuint GLTexture::object() const
{
	return m_data->object();
}

/// Get texture target.
GLuint GLTexture::target() const
{
	return m_data->target();
}

