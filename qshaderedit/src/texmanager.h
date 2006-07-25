#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <QtCore/QString>

class QGLContext;


// Simple texture manager.
class TexManager {
public:
	
	// get singleton instance.
	static TexManager * instance();


	TexManager(QGLContext * ctx);
	~TexManager();
	
	uint addTexture(QString name);
	uint getTexture(QString name);
	void releaseTexture(QString name);
	

private:
	
	QGLContext * m_context;
	
};


// Image plugin interface.
class ImagePlugin
{
public:
	virtual bool canLoad(QString name) const = 0;
	virtual bool load(QGLContext * ctx, QString name, uint * obj) const = 0;
};




#endif // TEXMANAGER_H
