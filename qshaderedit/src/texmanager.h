#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>


// Implicitly shared texture class.
class GLTexture
{
public:
	GLTexture();
	GLTexture(const GLTexture & t);
	void operator= (const GLTexture & t);
	~GLTexture();
	
	static GLTexture open(const QString & name);
	
	uint object() const;
	uint target() const;
		
private:
	class Private;
	GLTexture(Private * p);
	QSharedDataPointer<Private> m_data;
};


// Image plugin interface.
class ImagePlugin
{
public:
	virtual bool canLoad(QString name) const = 0;
	virtual bool load(QString name, uint obj, uint * target) const = 0;
};

// @@ Image plugin factory.


#endif // TEXMANAGER_H
