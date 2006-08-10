
#include <QtCore/QString>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtCore/QFile>
#include <math.h>

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


class CubeScene : public DisplayListScene
{
public:
	CubeScene()
	{
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		glBegin(GL_QUADS);
		
		// Front Face
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f(0, 0, -1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f(0, -1, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f(1, 0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		
		glEnd();
		glEndList();
	}
};

// Cube scene factory.
class CubeSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Cube");
	}
	virtual QString description() const
	{
		return tr("Cube");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new CubeScene();
	}
};

REGISTER_SCENE_FACTORY(CubeSceneFactory);



class ObjScene : public DisplayListScene
{
public:
	ObjScene()
	{
		QString fileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Open File"),
			SceneFactory::lastFile(), QString(QObject::tr("OBJ Files (%1)")).arg("*.obj"));

		if( !fileName.isEmpty() ) {
			load( fileName );
			SceneFactory::setLastFile( fileName );
		}		
	}
	
	virtual void transform() const
	{
		glScalef(m_scale, m_scale, m_scale);
		glTranslatef(-m_center.x, -m_center.y, -m_center.z);
	}

	
private:
	struct vec3 {
		vec3() { }
		vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) { }
		
		float x, y, z;
	};
	
	struct vec2 {
		vec2() { }
		vec2(float _s, float _t): s(_s), t(_t) { }
		
		float s, t;
	};
		
	static float min(float a, float b) 
	{
		return a < b ? a : b;
	}
	
	static float max(float a, float b) 
	{
		return a > b ? a : b;
	}
	
	void load(const QString & fileName)
	{
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		
		if (m_dlist)
			glDeleteLists(m_dlist, 1);			
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
				
		QRegExp vertexPattern("^v\\s+(.*)\\s+(.*)\\s+(.*)");
		QRegExp normalPattern("^vn\\s+(.*)\\s+(.*)\\s+(.*)");
		QRegExp texcoordPattern("^vt\\s+(.*)\\s+(.*)(\\s+.*)?");

		
		QVector<vec3> verts, normals;
		QVector<vec2> texcoords;
		vec3 vmin(1e10, 1e10, 1e10), vmax(-1e10, -1e10, -1e10);
		
		while (!file.atEnd()) {
			QString line = file.readLine().simplified();
			
			if (line.isEmpty() || line.startsWith('#'))
				continue;
			
			if (line.contains(vertexPattern)) {
				vec3 v(vertexPattern.cap(1).toFloat(), vertexPattern.cap(2).toFloat(), vertexPattern.cap(3).toFloat());
				verts.append(v);
				vmin.x = min(v.x, vmin.x);
				vmin.y = min(v.y, vmin.y);
				vmin.z = min(v.z, vmin.z);
				
				vmax.x = max(v.x, vmax.x);
				vmax.y = max(v.y, vmax.y);
				vmax.z = max(v.z, vmax.z);				
			}
			
			else if (line.contains(normalPattern))
				normals.append(vec3(normalPattern.cap(1).toFloat(), normalPattern.cap(2).toFloat(), normalPattern.cap(3).toFloat()));
			
			else if (line.contains(texcoordPattern))
				texcoords.append(vec2(texcoordPattern.cap(1).toFloat(), texcoordPattern.cap(2).toFloat()));
			
			else if (line.startsWith("f ")) {
				QStringList faces = line.split(' ');
				
				glBegin(GL_POLYGON);
				
				for (int i = 1; i < faces.size(); i++) {
					QStringList indices = faces[i].split('/');
					
					int v = indices[0].toInt() -1;
					if (v < 0) v += verts.size() +1;
					Q_ASSERT(v >= 0 && v < verts.size());
					
					if (indices.size() > 1 && !indices[1].isEmpty()) {
						int t = indices[1].toInt() -1;
						if (t < 0) t += texcoords.size() +1;
						Q_ASSERT(t >= 0 && t < texcoords.size());
						glTexCoord2fv((GLfloat*)&texcoords[t]);
					}
					
					if (indices.size() > 2) {
						int n = indices[2].toInt() -1;
						if (n < 0) n += normals.size() +1;
						Q_ASSERT(n >= 0 && n < normals.size()); 
						glNormal3fv((GLfloat*)&normals[n]);
					}
					
					glVertex3fv((GLfloat*)&verts[v]);
				}
				
				glEnd();
			}			
		}
		
		glEndList();
		
		
		m_center.x = (vmax.x + vmin.x) * 0.5;
		m_center.y = (vmax.y + vmin.y) * 0.5;
		m_center.z = (vmax.z + vmin.z) * 0.5;
		
		m_scale = 1.0f / max(vmax.x - m_center.x, max(vmax.y - m_center.y, vmax.z - m_center.z));
	}

	vec3 m_center;
	float m_scale;
};

// Obj scene factory.
class ObjSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("OBJ file");
	}
	virtual QString description() const
	{
		return tr("WaveFront 3D Object");
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
	static QString s_lastFile;
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

//static
Scene * SceneFactory::defaultScene()
{
	return new TeapotScene();
}

//static
const QString & SceneFactory::lastFile()
{
	return s_lastFile;
}

//static 
void SceneFactory::setLastFile(const QString & lastFile)
{
	s_lastFile = lastFile;
}
