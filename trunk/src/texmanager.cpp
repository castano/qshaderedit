
#include "texmanager.h"
#include "glutils.h"
#include "imageplugin.h"

#include <QtCore/QSharedData>
#include <QtCore/QDebug>
#include <QtGui/QImage>


class GLTexture::Private : public QSharedData
{
public:
	Private()
	{
		glGenTextures(1, &m_object);
		
		// load default texture
		m_image = ImagePluginManager::load(":images/default.png", m_object, &m_target);
		m_icon = m_image.scaled(16, 16, Qt::KeepAspectRatio, Qt::FastTransformation);
	}
	Private(const QString & name) : m_name(name)
	{
		glGenTextures(1, &m_object);
		
		m_image = ImagePluginManager::load(m_name, m_object, &m_target);
		m_icon = m_image.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
	QImage icon() const { return m_icon; }
	QImage image() const { return m_image; }

	static QMap<QString, GLTexture::Private *> s_textureMap;

private:
	QString m_name;
	GLuint m_object;
	GLuint m_target;

	QImage m_icon;
	QImage m_image;
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

QImage GLTexture::icon() const
{
	return m_data->icon();
}

QImage GLTexture::image() const
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
