
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
	
	virtual void draw()
	{
		glCallList(m_dlist);
	}
	
	virtual void transform()
	{
	}

private:
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
	
	virtual void transform()
	{
		// Object transform:
		glRotated(270.0, 1.0, 0.0, 0.0);
		glScaled(0.5, 0.5, 0.5);
		glTranslated(0.0, 0.0, -1.5);			
	}
};


class QuadScene : public Scene
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


class ObjScene : public DisplayListScene
{
public:
	ObjScene()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			m_openDir, QString(tr("Obj Files (%1)")).arg("*.obj"));

		if( !fileName.isEmpty() ) {
			m_openDir = fileName;
			load( fileName );
		}		
	}
	
	virtual void transform()
	{
	}

	
private:
	void load(const QString & fileName)
	{
		// @@ TODO: Load obj file.
	}
};


// @@ TODO: Add scene factory class.

Scene * createTeapot()
{
	return new TeapotScene();
}

Scene * createQuad()
{
	return new QuadScene();
}

Scene * createObj()
{
	return new ObjScene();
}
