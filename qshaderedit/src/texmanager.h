#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include <QtCore/QString>


// Simple texture manager.
class TexManager {
public:
	
	// get singleton instance.
	static TexManager * instance();


	TexManager();
	~TexManager();
	
	uint addTexture(QString name);
	uint getTexture(QString name);
	void releaseTexture(QString name);
	
};

// @@ Texture class to handle refcounting automatically.
class GLTexture
{
public:
	GLTexture(const QString & name);
	~GLTexture();
	
	operator uint() const;
	
private:
	// @@ private data.
};


// Image plugin interface.
class ImagePlugin
{
public:
	virtual bool canLoad(QString name) const = 0;
	virtual bool load(QString name, uint * obj) const = 0;
};




#endif // TEXMANAGER_H
