
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
	
	
	// Image plugin that supports all the image types that Qt supports.
	class QtImagePlugin
	{
	public:
	
		virtual bool canLoad(QString name) const
		{
			Q_UNUSED(name);
			return true;
		}
	
		virtual QImage load(QString name, GLuint obj, GLuint * target) const
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

	// @@ Add dds plugin.
	// @@ Add exr plugin.
	// @@ Add hdr plugin.

	static QtImagePlugin s_imageLoader;

} // namespace


class GLTexture::Private : public QSharedData
{
public:
	Private()
	{
		glGenTextures(1, &m_object);
		
		// load default texture
		m_name = "default.png";
		QImage image = s_imageLoader.load(":images/default.png", m_object, &m_target);
		m_image = QPixmap::fromImage(image);
		m_icon = QPixmap::fromImage(image.scaled(16, 16, Qt::KeepAspectRatio, Qt::FastTransformation));
	}
	Private(const QString & name) : m_name(name)
	{
		glGenTextures(1, &m_object);
		
		// @@ Traverse image plugins.
		if( s_imageLoader.canLoad(m_name) ) {
			QImage image = s_imageLoader.load(m_name, m_object, &m_target);
			m_image = QPixmap::fromImage(image);
			m_icon = QPixmap::fromImage(image.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
	QPixmap icon() const { return m_icon; }
	QPixmap image() const { return m_image; }

	static QMap<QString, GLTexture::Private *> s_textureMap;

private:
	QString m_name;
	GLuint m_object;
	GLuint m_target;

	QPixmap m_icon;
	QPixmap m_image;
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

const QString& GLTexture::name() const
{
	return m_data->name();
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

QPixmap GLTexture::icon() const
{
	return m_data->icon();
}

QPixmap GLTexture::image() const
{
	return m_data->image();
}

GLint GLTexture::wrapS() const
{
	glBindTexture(m_data->target(), m_data->object());
	GLint wrap;
	glGetTexParameteriv(m_data->target(), GL_TEXTURE_WRAP_S, &wrap);
	return wrap;
}

GLint GLTexture::wrapT() const
{
	glBindTexture(m_data->target(), m_data->object());
	GLint wrap;
	glGetTexParameteriv(m_data->target(), GL_TEXTURE_WRAP_T, &wrap);
	return wrap;
}

void GLTexture::setWrapMode(GLint s, GLint t) const
{
	glBindTexture(m_data->target(), m_data->object());
 	glTexParameteri(m_data->target(), GL_TEXTURE_WRAP_S, s);
 	glTexParameteri(m_data->target(), GL_TEXTURE_WRAP_T, t);
}

GLint GLTexture::minifyingFilter() const
{
	glBindTexture(m_data->target(), m_data->object());	
	GLint mode;
	glGetTexParameteriv(m_data->target(), GL_TEXTURE_MIN_FILTER, &mode);
	return mode;
}

GLint GLTexture::magnificationFilter() const
{
	glBindTexture(m_data->target(), m_data->object());
	GLint mode;
	glGetTexParameteriv(m_data->target(), GL_TEXTURE_MAG_FILTER, &mode);
	return mode;
}

void GLTexture::setFilteringMode(GLint min, GLint mag) const
{
	glBindTexture(m_data->target(), m_data->object());
 	glTexParameteri(m_data->target(), GL_TEXTURE_MIN_FILTER, min);
 	glTexParameteri(m_data->target(), GL_TEXTURE_MAG_FILTER, mag);
}
