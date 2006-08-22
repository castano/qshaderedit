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
		
private:
	class Private;
	GLTexture(Private * p);
	QSharedDataPointer<Private> m_data;
};

Q_DECLARE_METATYPE(GLTexture);


// Image plugin interface.
class ImagePlugin
{
public:
	virtual bool canLoad(QString name) const = 0;
	virtual bool load(QString name, GLuint obj, GLuint * target) const = 0;
};

// @@ Image plugin factory.


#endif // TEXMANAGER_H
