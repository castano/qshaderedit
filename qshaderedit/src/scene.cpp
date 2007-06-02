
#include "scene.h"
#include "effect.h"

// Include GLEW before anything else.
#include <GL/glew.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtGui/QAction>
#include <QtGui/QMenu>

#include <math.h>


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
	

	virtual void draw(Effect* effect) const
	{
		Q_UNUSED(effect);
		
		// Reset default material.
		GLfloat ka[] = {0.1f, 0.0f, 0.0f, 0.0f};
		GLfloat kd[] = {0.9f, 0.0f, 0.0f, 0.0f};
		GLfloat ks[] = {1.0f, 1.0f, 1.0f, 0.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
		glMaterialf(GL_FRONT, GL_SHININESS, 8);
		
		glCallList(m_dlist);
	}
	
	virtual void transform() const
	{
	}
	
	virtual void setupMenu(QMenu * menu) const
	{
		Q_ASSERT(menu != NULL);
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
		const bool hasMultiTexture = GLEW_ARB_multitexture || GLEW_VERSION_1_3;
		
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
	
			if( hasMultiTexture ) {
				glMultiTexCoord3f(GL_TEXTURE6, 1, 0, 0);
				glMultiTexCoord3f(GL_TEXTURE7, 0, -1, 0);
			}
		
			glTexCoord2f(0, 1);
			glVertex2f(-1, -1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(1, 1);
			glVertex2f(1, -1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(1, 0);
			glVertex2f(1, 1);

			glNormal3f(0, 0, 1);
			glTexCoord2f(0, 0);
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

// Experimental scenes for tessellation
#if 1

class EvenQuadScene : public DisplayListScene
{
public:
	EvenQuadScene()
	{
		const bool hasMultiTexture = GLEW_ARB_multitexture || GLEW_VERSION_1_3;
		
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		glBegin(GL_TRIANGLES);
		
		for(int i = 0; i < 16; i++) {
			float v0 = float(i) / 16;
			float v1 = float(i+1) / 16;
			
			for(int e = 0; e < 16; e++) {
				float u0 = float(e) / 16;
				float u1 = float(e+1) / 16;
				
				int dir = (i < 8) ^ (e < 8);
				
				if (dir)
				{
					glVertex2f(2*u0-1, 2*v0-1);
					glVertex2f(2*u1-1, 2*v0-1);
					glVertex2f(2*u0-1, 2*v1-1);
					
					glVertex2f(2*u0-1, 2*v1-1);
					glVertex2f(2*u1-1, 2*v0-1);
					glVertex2f(2*u1-1, 2*v1-1);
				}
				else
				{
					glVertex2f(2*u0-1, 2*v0-1);
					glVertex2f(2*u1-1, 2*v0-1);
					glVertex2f(2*u1-1, 2*v1-1);
					
					glVertex2f(2*u1-1, 2*v1-1);
					glVertex2f(2*u0-1, 2*v1-1);
					glVertex2f(2*u0-1, 2*v0-1);
				}
			}
		}		
		glEnd();
		glEndList();
	}
};

// Even quad scene factory.
class EvenQuadSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Even Quad");
	}
	virtual QString description() const
	{
		return tr("Even Quad");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new EvenQuadScene();
	}
};

REGISTER_SCENE_FACTORY(EvenQuadSceneFactory);


class OddQuadScene : public DisplayListScene
{
public:
	OddQuadScene()
	{
		const bool hasMultiTexture = GLEW_ARB_multitexture || GLEW_VERSION_1_3;
		
		m_dlist = glGenLists(1);
		glNewList(m_dlist, GL_COMPILE);
		glBegin(GL_TRIANGLES);
		
		for(int i = 0; i < 15; i++) {
			float v0, v1;
			if (i < 7) {
				v0 = float(i-8) / 8;
				v1 = float(i-7) / 8;
			}
			else if(i == 7) {
				v0 = float(i-8) / 8;
				v1 = float(i-6) / 8;
			}
			else {
				v0 = float(i-7) / 8;
				v1 = float(i-6) / 8;
			}
			
			for(int e = 0; e < 15; e++) {
				float u0, u1;
				
				if (e < 7) {
					u0 = float(e-8) / 8;
					u1 = float(e-7) / 8;
				}
				else if(e == 7) {
					u0 = float(e-8) / 8;
					u1 = float(e-6) / 8;
				}
				else {
					u0 = float(e-7) / 8;
					u1 = float(e-6) / 8;
				}
				
				int dir = (i < 8) ^ (e < 8);
				
				if (dir) {
					glVertex2f(u0, v0);
					glVertex2f(u1, v0);
					glVertex2f(u0, v1);
					
					glVertex2f(u0, v1);
					glVertex2f(u1, v0);
					glVertex2f(u1, v1);
				}
				else
				{
					glVertex2f(u0, v0);
					glVertex2f(u1, v0);
					glVertex2f(u1, v1);
					
					glVertex2f(u0, v0);
					glVertex2f(u1, v1);
					glVertex2f(u0, v1);
				}
			}
		}
		glEnd();
		glEndList();
	}
};

// OddQuad scene factory.
class OddQuadSceneFactory : public SceneFactory
{
public:
	virtual QString name() const
	{
		return tr("Odd Quad");
	}
	virtual QString description() const
	{
		return tr("Odd Quad");
	}
	virtual QIcon icon() const
	{
		return QIcon();
	}
	virtual Scene * createScene() const
	{
		return new OddQuadScene();
	}
};

REGISTER_SCENE_FACTORY(OddQuadSceneFactory);


static void baryTexCoord(float x, float y)
{
	float z = 1 - x - y;
	
	if (fabs(x) < 0.001f) {
		x = 0.0f;
		float sum = y + z;
		y /= sum;
		z /= sum;
	}
	if (fabs(y) < 0.001f) {
		y = 0.0f;
		float sum = x + z;
		x /= sum;
		z /= sum;
	}
	if (fabs(z) < 0.001f) {
		z = 0.0f;
		float sum = x + y;
		x /= sum;
		y /= sum;
	}
	
	glTexCoord3f(x, y, z);
}


class TriangleScene : public DisplayListScene
{
	public:
		TriangleScene()
		{
			m_dlist = glGenLists(1);
			glNewList(m_dlist, GL_COMPILE);
			glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, 1);
		
			for(int i = 0; i < 15; i++) {
				float v0 = float(i) / 15;
				float v1 = float(i+1) / 15;
				
				for(int e = 0; e <= i; e++) {
					float u0 = float(e) / 15;
					float u1 = float(e+1) / 15;

					if( e < i ) {
						baryTexCoord(u0, 1-v0);
						glVertex2f(2*u0-1, 2*v0-1);
						
						baryTexCoord(u1, 1-v0);
						glVertex2f(2*u1-1, 2*v0-1);
						
						baryTexCoord(u1, 1-v1);
						glVertex2f(2*u1-1, 2*v1-1);
					}

					baryTexCoord(u0, 1-v0);
					glVertex2f(2*u0-1, 2*v0-1);
					
					baryTexCoord(u1, 1-v1);
					glVertex2f(2*u1-1, 2*v1-1);
					
					baryTexCoord(u0, 1-v1);
					glVertex2f(2*u0-1, 2*v1-1);
				}
			}
			
			glEnd();
			glEndList();
		}
};

// Triangle scene factory.
class TriangleSceneFactory : public SceneFactory
{
	public:
		virtual QString name() const
		{
			return tr("Triangle");
		}
		virtual QString description() const
		{
			return tr("Triangle");
		}
		virtual QIcon icon() const
		{
			return QIcon();
		}
		virtual Scene * createScene() const
		{
			return new TriangleScene();
		}
};

REGISTER_SCENE_FACTORY(TriangleSceneFactory);

#endif


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



class ObjScene : public Scene
{
public:
	ObjScene(): m_dlistBase(0), m_dlistCount(0)
	{
		QString fileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Open File"),
			SceneFactory::lastFile(), QString(QObject::tr("OBJ Files (%1)")).arg("*.obj"));

		if( !fileName.isEmpty() ) {
			load( fileName );
			SceneFactory::setLastFile( fileName );
		}		
	}
	
	~ObjScene()
	{
		if (m_dlistBase)
			glDeleteLists(m_dlistBase, m_dlistCount);
	}
	
	virtual void transform() const
	{
		glScalef(m_scale, m_scale, m_scale);
		glTranslatef(-m_center.x, -m_center.y, -m_center.z);
	}
	
	virtual void draw(Effect* effect) const
	{
		GLuint lastList = m_dlistBase + m_dlistCount;  
		for (GLuint n = m_dlistBase; n < lastList; n++) {
			if(effect) effect->beginMaterialGroup();
			glCallList(n);
		}
	}
	
	virtual void setupMenu(QMenu * menu) const
	{
		Q_UNUSED(menu);
	}
	
private:
	struct vec4 {
		vec4() { }
		vec4(float _x, float _y, float _z, float _w = 1.0f): x(_x), y(_y), z(_z), w(_w) { }
		
		float x, y, z, w;
	};
	
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
	
	struct Material 
	{
		QString name;
		vec4 ka, kd, ks;
		float ns;
		
		Material(): ka(0.1f, 0.1f, 0.1f, 1.0f), kd(1.0f, 1.0f, 1.0f, 1.0f), ks(0.0f, 0.0f, 0.0f, 0.0f), ns(20.0f)		 
		{ }
		
		void bind() 
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)&ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)&kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&ks);
			glMaterialf(GL_FRONT, GL_SHININESS, (GLfloat)ns);
		}
	};
	
	class MaterialLib
	{
		public:
			~MaterialLib()
			{
				qDeleteAll(m_materials);
			}
			
			QString name() const 
			{
				return m_name;
			}
			
			bool load(const QString& name, const QString& fileName)
			{
				QFile file(fileName);
				
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
					return false;
				
				m_name = name;
				
				QRegExp newmtlPattern("^newmtl\\s(.*)(\\#.*)?");
				QRegExp kaPattern("^Ka\\s(.*)\\s(.*)\\s(.*)");
				QRegExp kdPattern("^Kd\\s(.*)\\s(.*)\\s(.*)");
				QRegExp ksPattern("^Ks\\s(.*)\\s(.*)\\s(.*)");
				QRegExp nsPattern("^Ns\\s(.*)");
				
				if (!m_materials.isEmpty()) {
					qDeleteAll(m_materials);
					m_materials.clear();
				}
				
				Material* current = NULL;
				while (!file.atEnd()) {
					QString line = file.readLine().simplified();
					
					if (line.contains(newmtlPattern)) {
						current = new Material;
						m_materials.append(current);
						current->name = newmtlPattern.cap(1);
					}
					else if (line.contains(kaPattern)) {
						Q_ASSERT(current);
						current->ka = vec4(kaPattern.cap(1).toFloat(), kaPattern.cap(2).toFloat(), kaPattern.cap(3).toFloat());
					}
					else if (line.contains(kdPattern)) {
						Q_ASSERT(current);
						current->kd = vec4(kdPattern.cap(1).toFloat(), kdPattern.cap(2).toFloat(), kdPattern.cap(3).toFloat());
					}
					else if (line.contains(ksPattern)) {
						Q_ASSERT(current);
						current->ks = vec4(ksPattern.cap(1).toFloat(), ksPattern.cap(2).toFloat(), ksPattern.cap(3).toFloat());
					}
					else if (line.contains(nsPattern)) {
						Q_ASSERT(current);
						current->ns = nsPattern.cap(1).toFloat();
					}	
				}
				
				return true;
			}
			
			Material* material(const QString& name)
			{
				foreach (Material* mat, m_materials) {
					if (mat->name == name)
						return mat;
				}
				return NULL;
			}
			
		private:
			QString m_name;
			QVector<Material*> m_materials;		
	};
	
	struct Vertex
	{
		int pos;
		int normal;
		int texcoord;
	};
	
	struct Surface 
	{
		Material* material;
		QVector< QVector<Vertex> > faces;
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
		
		if (m_dlistBase)
			glDeleteLists(m_dlistBase, m_dlistCount);
		
		QRegExp vertexPattern("^v\\s+(.*)\\s+(.*)\\s+(.*)");
		QRegExp normalPattern("^vn\\s+(.*)\\s+(.*)\\s+(.*)");
		QRegExp texcoordPattern("^vt\\s+(.*)\\s+(.*)(\\s+.*)?");
		QRegExp mtllibPattern("^mtllib\\s+(.*\\.mtl)");
		QRegExp usemtlPattern("^usemtl\\s+(.*)(\\#.*)?");
		
		QVector<vec3> vertices, normals;
		QVector<vec2> texcoords;
		QVector<MaterialLib*> materialLibs;
		QVector<Surface*> surfaces;
		
		vec3 vmin(1e10, 1e10, 1e10), vmax(-1e10, -1e10, -1e10);
		
		Material * defaultMaterial = new Material; 
		MaterialLib * currentMaterialLib = NULL;
		Surface * currentSurface = new Surface;
		currentSurface->material = defaultMaterial;
		surfaces.append(currentSurface);
		
		while (!file.atEnd()) {
			QString line = file.readLine().simplified();
			
			if (line.isEmpty() || line.startsWith('#'))
				continue;
			
			if (line.contains(vertexPattern)) {
				vec3 v(vertexPattern.cap(1).toFloat(), vertexPattern.cap(2).toFloat(), vertexPattern.cap(3).toFloat());
				vertices.append(v);
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
				
				QVector<Vertex> face;
				
				for (int i = 1; i < faces.size(); i++) {
					QStringList indices = faces[i].split('/');
					
					Vertex v;
					
					v.pos = indices[0].toInt() -1;
					if (v.pos < 0) v.pos += vertices.size() +1;
					Q_ASSERT(v.pos >= 0 && v.pos < vertices.size());
					
					if (indices.size() > 1 && !indices[1].isEmpty()) {
						v.texcoord = indices[1].toInt() -1;
						if (v.texcoord < 0) v.texcoord += texcoords.size() +1;
						Q_ASSERT(v.texcoord >= 0 && v.texcoord < texcoords.size());
					}
					else
						v.texcoord = -1;
					
					if (indices.size() > 2) {
						v.normal = indices[2].toInt() -1;
						if (v.normal < 0) v.normal += normals.size() +1;
						Q_ASSERT(v.normal >= 0 && v.normal < normals.size()); 
					}
					else
						v.normal = -1;
					
					face.append(v);
				}
				
				currentSurface->faces.append(face);
			}
			
			else if (line.contains(usemtlPattern)) {
				QString name = usemtlPattern.cap(1);
				
				Material* material = NULL;
				if (currentMaterialLib)
					material = currentMaterialLib->material(name);
				
				if (!material) {
					currentSurface = surfaces[0]; // default material
				}
				else {
					currentSurface = NULL;
					foreach (Surface* surf, surfaces) {
						Q_ASSERT(surf != NULL);
						Q_ASSERT(surf->material != NULL);
					
						if (surf->material->name == name) {
							currentSurface = surf;
							break;
						}
					}
					
					if (!currentSurface) {
						currentSurface = new Surface;
						currentSurface->material = material;
						surfaces.append(currentSurface);
					}
				}
			}
			
			else if (line.contains(mtllibPattern)) {				
				QString name = mtllibPattern.cap(1);
				currentMaterialLib = NULL;
				foreach (MaterialLib* mtlLib, materialLibs) {
					if (mtlLib->name() == name) {
						currentMaterialLib = mtlLib;
						break; 
					}
				}
				
				if (!currentMaterialLib) {
					QFileInfo info(fileName);
					QString mtlLibFileName = info.dir().filePath(name); 
					
					MaterialLib* mtlLib = new MaterialLib;
					if (!mtlLib->load(name, mtlLibFileName)) {
						qWarning("Could not open material file: %s", qPrintable(mtlLibFileName));
						delete mtlLib;
						continue;
					}
					materialLibs.append(mtlLib);
					currentMaterialLib = mtlLib;
				}
			}
		}
		
		
		m_center.x = (vmax.x + vmin.x) * 0.5;
		m_center.y = (vmax.y + vmin.y) * 0.5;
		m_center.z = (vmax.z + vmin.z) * 0.5;
		
		m_scale = 1.0f / max(vmax.x - m_center.x, max(vmax.y - m_center.y, vmax.z - m_center.z));
		
		
		// create display lists
		if (surfaces[0]->faces.isEmpty())
			surfaces.remove(0);
		m_dlistCount = surfaces.size();
		m_dlistBase = glGenLists(m_dlistCount);
		
		for (GLuint n = 0; n < m_dlistCount; n++) {
			glNewList(m_dlistBase + n, GL_COMPILE);			
			Surface* surf = surfaces[n];			
			surf->material->bind();
			
			for (int face = 0; face < surf->faces.count(); face++) {
				QVector<Vertex> verts = surf->faces[face];
				
				glBegin(GL_POLYGON);
				for (int vert = 0; vert < verts.count(); vert++) {
					
					if (verts[vert].texcoord >= 0)
						glTexCoord2fv((GLfloat*)&texcoords[verts[vert].texcoord]);
					
					if (verts[vert].normal >= 0)
						glNormal3fv((GLfloat*)&normals[verts[vert].normal]);
					
					glVertex3fv((GLfloat*)&vertices[verts[vert].pos]);
				}
				glEnd();
			}
			
			glEndList();
		}		
		
		delete defaultMaterial;
		qDeleteAll(materialLibs);
		qDeleteAll(surfaces);
	}
	
	vec3 m_center;
	float m_scale;
	
	GLuint m_dlistBase, m_dlistCount;
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
	return NULL;
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
