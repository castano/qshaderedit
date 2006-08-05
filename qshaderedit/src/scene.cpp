
#include <QtCore/QString>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>

#include "scene.h"

// Include GLEW before anything else.
#include <GL/glew.h>

extern void drawTeapot();

class DisplayListScene : public Scene
{
public:
	DisplayListScene() : m_dlist(0)
	{
	}

	virtual ~DisplayListScene()
	{
		if( m_dlist != 0 ) {
			glDeleteLists(m_dlist, 1);
		}
	}
	
	virtual void draw() const
	{
		glCallList(m_dlist);
	}
	
	virtual void transform() const
	{
	}
	
	virtual void setupMenu(QMenu * menu) const
	{
		Q_UNUSED(menu);
	}

protected:
	GLuint m_dlist;
};


class TeapotScene : public DisplayListScene
{
public:
	TeapotScene()
	{
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		drawTeapot();
		glEndList();
	}
	
	virtual void transform() const 
	{
		// Object transform:
		glRotated(270.0, 1.0, 0.0, 0.0);
		glScaled(0.5, 0.5, 0.5);
		glTranslated(0.0, 0.0, -1.5);			
	}
};

// Teapot scene factory.
class TeapotSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Teapot");
	}
	virtual QString description() const
	{
		return tr("Teapot");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new TeapotScene();
	}
};

REGISTER_SCENE_FACTORY(TeapotSceneFactory);



class QuadScene : public DisplayListScene
{
public:
	QuadScene()
	{
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(1, 0);
			glVertex2f(1, -1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(1, 1);
			glVertex2f(1, 1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
		glEnd();
		glEndList();
	}
};

// Quad scene factory.
class QuadSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Quad");
	}
	virtual QString description() const
	{
		return tr("Quad");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new QuadScene();
	}
};

REGISTER_SCENE_FACTORY(QuadSceneFactory);



class ObjScene : public DisplayListScene
{
public:
	ObjScene()
	{
//		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//			m_openDir, QString(QObject::tr("Obj Files (%1)")).arg("*.obj"));

//		if( !fileName.isEmpty() ) {
//			load( fileName );
//		}		
	}
	
	virtual void transform() const
	{
	}

	
private:
	void load(const QString & fileName)
	{
		// @@ TODO: Load obj file.
	}
};

// Obj scene factory.
class ObjSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Obj");
	}
	virtual QString description() const
	{
		return tr("Obj");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new ObjScene();
	}
};

REGISTER_SCENE_FACTORY(ObjSceneFactory);



// SceneFactory

namespace {
	static QList<const SceneFactory *> * s_factoryList = NULL;
}


//static
const SceneFactory * SceneFactory::findFactory(const QString & name)
{
	foreach(const SceneFactory * factory, factoryList()) {
		if( factory->name() == name ) {
			return factory;
		}
	}
}

//static
const QList<const SceneFactory *> & SceneFactory::factoryList()
{
	static QList<const SceneFactory *> empty;
	if(s_factoryList == NULL) {
		return empty;
	}
	else {
		return *s_factoryList;
	}
}

//static 
void SceneFactory::addFactory(const SceneFactory * factory)
{
	Q_ASSERT(factory != NULL);
	if( s_factoryList == NULL ) {
		s_factoryList = new QList<const SceneFactory *>();
	}
	s_factoryList->append(factory);
}

//static
void SceneFactory::removeFactory(const SceneFactory * factory)
{
	Q_ASSERT(factory != NULL);

	const int index = s_factoryList->lastIndexOf(factory);
	Q_ASSERT(index != -1);
	s_factoryList->removeAt(index);
	
	if( s_factoryList->isEmpty() ) {
		delete s_factoryList;
		s_factoryList = NULL;
	}
}

// static
Scene * SceneFactory::defaultScene()
{
	return new TeapotScene();
}
