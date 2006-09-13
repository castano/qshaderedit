#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <GL/glew.h>

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMetaType>
#include <QtGui/QPixmap>


// Implicitly shared texture class.
class GLTexture
{
public:
	GLTexture();
	GLTexture(const GLTexture & t);
	void operator= (const GLTexture & t);
	~GLTexture();
	
	static GLTexture open(const QString & name);

	const QString& name() const;
	GLuint object() const;
	GLuint target() const;
	QPixmap icon() const;
	QPixmap image() const;

	
	GLint wrapS() const;
	GLint wrapT() const;
	// HACK: the set texture mode functions must be const to prevent QSharedData to copy itself 
	void setWrapMode(GLint s, GLint t) const;

	GLint minifyingFilter() const;
	GLint magnificationFilter() const;
	void setFilteringMode(GLint min, GLint mag) const;
	
	
private:
	class Private;
	GLTexture(Private * p);
	QSharedDataPointer<Private> m_data;
};

Q_DECLARE_METATYPE(GLTexture);


#endif // TEXMANAGER_H
