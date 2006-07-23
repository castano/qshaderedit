#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <QtCore/QString>

class QGLContext;


class TexManager {
public:
	
	TexManager(QGLContext * ctx) : m_context(ctx) 
	{
	}
	
	virtual ~TexManager()
	{
	}
	
	uint AddTexture(QString name);
	uint GetTexture(QString name);
	void ReleaseTexture(QString name);
	
private:
	
	QGLContext * m_context;
	
};



class ImagePlugin
{
public:
	virtual bool canLoad(QString name) const = 0;
	virtual bool load(QGLContext * ctx, QString name, uint * obj) const = 0;
};




#endif // TEXMANAGER_H
